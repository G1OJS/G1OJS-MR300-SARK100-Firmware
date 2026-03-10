
import os
import copy
import pandas as pd
import argparse
from wsjt_udp import WSJTXListener

from fileutils import read_voltage_file
from helpers import log
from plotfuncs import make_id_color_map, add_Wgrid
from config import calibration_sets, W_plot_max_reW
import transforms as tf
from sim_converter import convert_sim_csv_to_voltagefile
from ltspice_tran_tools import run_bridge_tran_sweep

def get_common_frequencies(v_data, cal_ids):
    if cal_ids:
        try:
            return [entry['MHz'] for entry in v_data[cal_ids['voltage'][0]]['data']]
        except (KeyError, IndexError):
            log("Calibration ID not found in data — falling back to first ID")
    # fallback
    first_id = next(iter(v_data))
    return [entry['MHz'] for entry in v_data[first_id]['data']]


def run_analysis(from_tran_sim=False, tran_args=None):
    if from_tran_sim:
        log("Running LTSpice transient simulation...")

        # 1. Run transient sweep
        df = run_bridge_tran_sweep(**tran_args)
        csv_path = os.path.join(tran_args["work_dir"], "bridge_voltage_tran_sweep.csv")

        # 2. Convert to voltage file format
        sim_output_path = os.path.join(tran_args["work_dir"], "simulated_voltages_tran.csv")
        convert_sim_csv_to_voltagefile(csv_path, sim_output_path)

        # 3. Set that as input file
        datafilename = sim_output_path

    else:
        # Use default data file
        datafilename = 'C:/Users/drala/Documents/Radio_tools/SARK100/Analysis/data/NewMeas.csv'

    log(f"Loading data from {datafilename}")
    v_data = read_voltage_file(datafilename)

    # ==============================================
    # 2. Determine calibration IDs
    # ==============================================
    basename = os.path.basename(datafilename)
    cal_ids = calibration_sets.get(basename)
    if basename not in calibration_sets:
        log(f"No calibration IDs configured for {basename}")
        calibration_ids = None
    else:
        calibration_ids = calibration_sets[basename]
        log(f"Using calibration IDs: {cal_ids}")

    # ==============================================
    # 3. Pre-processing
    # ==============================================
    for id_ in v_data:
        log("Loaded series " + id_ + ": " + str(len(v_data[id_]['data'])))

    MHz_common = get_common_frequencies(v_data, cal_ids)
    log(f"Common frequency list loaded ({len(MHz_common)} points)")

    # Add grid
    add_Wgrid(v_data, 1)
    add_Wgrid(v_data, 10)
    add_Wgrid(v_data, 30)
    add_Wgrid(v_data, 59)
    v_data_uncorrected = copy.deepcopy(v_data)

    # Colour map
    ids_for_colour_code = []
    for id_ in v_data:
        load_type = v_data[id_]['metadata']['type']
        if load_type in 'RLC':
            ids_for_colour_code.append(id_)
    id_color_map = make_id_color_map(ids_for_colour_code)

    # ==============================================
    # 4. Correction schemes and plotting
    # ==============================================
    run_all_corrections(v_data, v_data_uncorrected, cal_ids, id_color_map, datafilename, MHz_common)


def run_all_corrections(v_data, v_data_uncorrected, cal_ids, id_color_map, datafilename, MHz_common):
    # Import plot function & correction functions only here to avoid circular imports
    from plotfuncs import prepare_figure, build_and_save_plot

    # Uncorrected plot
    plotfilename =  "0-Uncorrected.png"
    correction_name = "None"
    fig, axs= prepare_figure()
    build_and_save_plot(v_data, fig, axs, datafilename, plotfilename, correction_name,  id_color_map, output_folder="Plots")

    # W-space schemes
    if (cal_ids):
        
        plotfilename =   "1-OriginalVoltageCorr.png"
        correction_name = "Python model of original firmware ('V13')\n Vx -> Vx(mVx + c) *per band*"
        fig, axs= prepare_figure()
        v_data = copy.deepcopy(v_data_uncorrected)
        banded_corrections = tf.build_banded_corrections(v_data, cal_ids['voltage'])
        tf.apply_banded_corrections(v_data, banded_corrections)
        build_and_save_plot(v_data, fig, axs, datafilename, plotfilename, correction_name,  id_color_map, output_folder="Plots", v_data_uc=v_data_uncorrected)

        plotfilename =   "2-HybridCorrections1.png"
        correction_name = "Hybrid (Vr -> Vr(mVr + c) *per frequency* , nothing for Vz, Va)"
        fig, axs = prepare_figure()
        v_data = copy.deepcopy(v_data_uncorrected)
        mVr, cVr = tf.build_VrVoltage_corrections(v_data,cal_ids['voltage'])
        tf.apply_VrVoltage_corrections(v_data, mVr, cVr, MHz_common)
        build_and_save_plot(v_data, fig, axs, datafilename, plotfilename, correction_name,  id_color_map, output_folder="Plots", v_data_uc=v_data_uncorrected)

        plotfilename =   "3-HybridCorrections2.png"
        correction_name = "Hybrid (Vr -> Vr(mVr + c) *per frequency* , then WTLC for Z only)"
        fig, axs = prepare_figure()
        v_data = copy.deepcopy(v_data_uncorrected)
        mVr, cVr = tf.build_VrVoltage_corrections(v_data,cal_ids['voltage'])
        tf.apply_VrVoltage_corrections(v_data, mVr, cVr, MHz_common)
        Wcal_Lo, Wtru_Lo, Wcal_Match, Wtru_Match, Wcal_Hi, Wtru_Hi = tf.build_WTLC(v_data, cal_ids['threeload'])
        tf.apply_WTLC(v_data, Wcal_Lo, Wtru_Lo, Wcal_Match, Wtru_Match, Wcal_Hi, Wtru_Hi, MHz_common, correctModG = False)
        build_and_save_plot(v_data, fig, axs, datafilename, plotfilename, correction_name,  id_color_map, output_folder="Plots", v_data_uc=v_data_uncorrected)


        plotfilename =  "4-ThreeLoadLinmapCal.png"
        correction_name = "Three Load Linear Map"
        fig, axs= prepare_figure()
        v_data = copy.deepcopy(v_data_uncorrected)
        Wcal_Lo, Wtru_Lo, Wcal_Match, Wtru_Match, Wcal_Hi, Wtru_Hi = tf.build_WTLC(v_data, cal_ids['threeload'])
        tf.apply_WTLC(v_data, Wcal_Lo, Wtru_Lo, Wcal_Match, Wtru_Match, Wcal_Hi, Wtru_Hi, MHz_common)
        build_and_save_plot(v_data, fig, axs, datafilename, plotfilename, correction_name,  id_color_map, output_folder="Plots", v_data_uc=v_data_uncorrected)

        plotfilename =  "5-AffineMap.png"
        correction_name = "Affine Warp"
        fig, axs= prepare_figure()
        v_data = copy.deepcopy(v_data_uncorrected)
        affine_by_freq = tf.fit_W_affine(v_data, cal_ids['threeload'])
        tf.apply_W_affine(v_data, affine_by_freq, MHz_common)
        build_and_save_plot(v_data, fig, axs, datafilename, plotfilename, correction_name,  id_color_map, output_folder="Plots", v_data_uc=v_data_uncorrected)

    # Z-space schemes

    plotfilename =  "6-ClipValidZ.png"
    correction_name = "Clip W to valid Z"
    fig, axs= prepare_figure()
    v_data = copy.deepcopy(v_data_uncorrected)
    tf.apply_clip_valid_Z(v_data)
    build_and_save_plot(v_data, fig, axs, datafilename, plotfilename, correction_name,  id_color_map, output_folder="Plots", v_data_uc=v_data_uncorrected)

    plotfilename =  "7-PortModel.png"
    correction_name = "Port model"
    fig, axs= prepare_figure()
    v_data = copy.deepcopy(v_data_uncorrected)
    tf.apply_port_model(v_data)
    build_and_save_plot(v_data, fig, axs, datafilename, plotfilename, correction_name,  id_color_map, output_folder="Plots", v_data_uc=v_data_uncorrected)

    if (cal_ids):
        if os.path.exists(cal_ids['open_port']):
            plotfilename =  "8-PortMeasurement.png"
            correction_name = "Port file"
            fig, axs= prepare_figure()
            v_data = copy.deepcopy(v_data_uncorrected)
            tf.apply_port_measurement(v_data, cal_ids['open_port'])
            build_and_save_plot(v_data, fig, axs, datafilename, plotfilename, correction_name,  id_color_map, output_folder="Plots", v_data_uc=v_data_uncorrected)


            
if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Bridge data analysis and correction")
    parser.add_argument("--simulate", action="store_true", help="Run LTSpice simulation first")
    args = parser.parse_args()


#    tran_args = {
#        "asc_file": r"C:\Users\drala\Documents\Radio_tools\SARK100\Analysis\ltspice_models\SARK100_Full_Bridge\Test_circuit.asc",
#        "ltspice_exe": r"C:\Users\drala\AppData\Local\Programs\ADI\LTspice\LTspice.exe",
#        "load_values": [5, 500],
#        "source_voltages": [0.1, 1.0],
#        "frequency_values":  [f * 1e6 for f in range(1, 21, 5)],
#        "work_dir": r"C:\Users\drala\Documents\Radio_tools\SARK100\Analysis\ltspice_runs_tran",
#        "save_csv": True
#    }
    #run_analysis(from_tran_sim=True, tran_args=tran_args)
    run_analysis()
