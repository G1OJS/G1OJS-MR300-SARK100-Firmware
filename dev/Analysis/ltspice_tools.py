
import subprocess
import os
import pandas as pd
from PyLTSpice import SimCommander
from helpers import log

def run_bridge_sweep(
    asc_file,
    ltspice_exe,
    load_values=[5, 50, 500],
    source_voltages=[0.1, 0.5, 1.0],
    frequencies=(1e6, 50e6, 100),
    work_dir="./ltspice_runs",
    save_csv=True
):
    """Run LTSpice bridge voltage sweep over load, frequency, and source voltage."""

    os.makedirs(work_dir, exist_ok=True)
    results = []

    for load in load_values:
        for Vsrc in source_voltages:

            log_file = os.path.join(work_dir, f"Bridge_{load}R_{Vsrc}V.log")

            # Run LTSpice
            cmd = [
                ltspice_exe, "-b", asc_file,
                "-ar", f"Rload={load}",
                "-ar", f"Vsrc={Vsrc}",
                " >> " + log_file
            ]
            log(" ".join(cmd))
            subprocess.run(cmd, check=True)

