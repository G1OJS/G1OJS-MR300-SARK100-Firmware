
import pandas as pd
import os
import csv

def convert_sim_csv_to_voltagefile(sim_csv_path, output_path):
    """Convert LTSpice simulation CSV to structured voltage file format."""
    df = pd.read_csv(sim_csv_path)

    # Group by load and source voltage to create series
    grouped = df.groupby(['Load_Ohm', 'Vsrc_V'])

    os.system(r'copy C:\Users\drala\Documents\Radio_tools\SARK100\Analysis\ltspice_runs_tran\HEADER.csv ' + output_path)

    with open(output_path, 'a', newline='') as f:
        writer = csv.writer(f)
        f.write('\n')
        for (load, vsrc), group in grouped:
            series_id = f"R_{int(load)}_LtSpiceV{vsrc:.1f}_sim"
         #   writer.writerow([series_id, f"Simulated {load} Ohm {vsrc}V", "None", "None", "None", "Sim", "None"])
            for _, row in group.iterrows():
                MHz = row['Frequency_Hz'] / 1e6
                writer.writerow([
                    f"{MHz:.3f}",
                    f"{series_id}",
                    "2025-04-04-10:00",
                    f"{row['Vf']:.8e}",
                    f"{row['Vr']:.8e}",
                    f"{row['Vz']:.8e}",
                    f"{row['Va']:.8e}",
                    "0"
                ])

    print(f"Converted simulation CSV to voltage file format: {output_path}")
