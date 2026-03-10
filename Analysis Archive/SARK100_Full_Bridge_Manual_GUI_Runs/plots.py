import pandas as pd
from PyLTSpice.log.ltsteps import LTSpiceLogReader

def parse_ltspice_log_to_csv(log_path, output_csv="ltspice_results.csv"):
    # Read LTSpice log file
    data = LTSpiceLogReader(log_path)
    
    # Extract step parameters and measurements
    step_names = data.get_step_vars()
    meas_names = data.get_measure_names()

    # Build a dataframe
    rows = []
    for i in range(data.step_count):
        row = {}
        for step in step_names:
            row[step] = data[step][i]
        for meas in meas_names:
            row[meas] = data[meas][i]
        rows.append(row)

    df = pd.DataFrame(rows)
    print(step_names)
    print(meas_names)
    print(df.columns)

    # Save
    df.to_csv(output_csv, index=False)
    print(f"[OK] Saved: {output_csv}")
    return df

import matplotlib.pyplot as plt
import numpy as np
def plot_vr_over_vf(log_path):
    df = parse_ltspice_log_to_csv(log_path)
    
    if 'vr' not in df.columns or 'vf' not in df.columns:
        raise ValueError("Missing required columns 'r' or 'Vr_over_Vf'")

    # Assuming df has 'rload', 'vsrc', 'vr', 'vf' columns
    grouped = df.groupby('vsrc')

    for vsrc_value, group in grouped:
        plt.plot(group['rload'], 2*group['vr'] / group['vf'], marker='o', label=f'Vsrc = {vsrc_value} V')

    rload_vals = np.logspace(np.log10(df['rload'].min()), np.log10(df['rload'].max()), 500)
    expected = np.abs((50 - rload_vals) / (50 + rload_vals))
    plt.plot(rload_vals, expected, 'k--', label='Expected |Γ| (ideal)')

    plt.xlabel('Load Resistance (Ohm)')
    plt.ylabel('Vr / Vf')
    plt.ylim(0,1)
    plt.xscale('log')
    plt.legend()
    plt.grid(True)
    plt.title('Bridge Ratio vs Load Resistance')
    plt.show()




plot_vr_over_vf("Test_circuit.log")
