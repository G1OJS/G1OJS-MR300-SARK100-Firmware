
import subprocess
import os
import pandas as pd
from PyLTSpice import SimCommander
from helpers import log
import re


def parse_meas_log(log_file):
    """Parse LTSpice .log file with AVG measurement format."""
    with open(log_file, "r") as f:
        flines=f.read()

    result={}
    lines = flines.splitlines()
    for line in lines:
        line = line.strip()
        # Look for lines like: vf_final: AVG(v(vf))=3.74692 FROM ...
        match = re.match(r"(\w+):\s+AVG\(.*?\)\s*=\s*([-\d.eE]+)", line)
        if match:
            name = match.group(1).lower()
            value = float(match.group(2))
            result[name] = value
    return result


def run_bridge_tran_sweep(
    asc_file,
    ltspice_exe,
    load_values=[5, 50, 500],
    source_voltages=[0.1, 0.5, 1.0],
    frequency_values=[1.0e6,5.0e6],
    work_dir="./ltspice_runs_tran",
    save_csv=True
):
    """Run LTSpice transient bridge sweep and parse .meas results."""
    os.makedirs(work_dir, exist_ok=True)
    results = []

    for freq in frequency_values:
        for load in load_values:
            for Vsrc in source_voltages:
                # create unique log path
                log(f"Running LTSpice transient: Rload={load} Ω, Vsrc={Vsrc} V, freq={freq/1e6:.1f} MHz")
                sim_args = [
                    ltspice_exe, "-b", asc_file,
                    "-ar", f"Rload={load}",
                    "-ar", f"Vsrc={Vsrc}",
                    "-ar", f"freq={freq}",
                    " > " + asc_file+".log"
                ]
                print(sim_args)
                process = subprocess.run(sim_args, capture_output=True, text=True)
                meas = parse_meas_log(asc_file+".log")

                results.append({
                    "Load_Ohm": load,
                    "Vsrc_V": Vsrc,
                    "Frequency_Hz": freq,
                    "Va": meas.get("Va_final"),
                    "Vr": meas.get("Vr_final"),
                    "Vz": meas.get("Vz_final"),
                    "Vf": meas.get("Vf_final")
                })

    # Save CSV
    df = pd.DataFrame(results)
    if save_csv:
        filename_base = "bridge_voltage_tran_sweep.csv"
        output_file = os.path.join(work_dir, filename_base)
        df.to_csv(output_file, index=False)
        log(f"Results saved to {output_file}")

    return df

