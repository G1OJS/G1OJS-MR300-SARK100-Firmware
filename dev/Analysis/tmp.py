import subprocess
import pandas as pd
import os

cir_path = r"C:\Users\drala\Documents\Radio_tools\SARK100\Analysis\ngspice_models\test_bridge.cir"
input_cir = os.path.join(os.path.dirname(cir_path), "input.cir")
output_csv = os.path.join(os.path.dirname(cir_path), "output.csv")


# Clean previous result
if os.path.exists(output_csv):
    os.remove(output_csv)

template = open(cir_path).read()
filled = template.format(freq=1e6, Rload=50, Vsrc=0.1, outpath = output_csv)
with open(input_cir, "w") as f:
    f.write(filled)


# Run ngspice
subprocess.run([
    r"C:\Program Files\ngspice\bin\ngspice.exe",
    "-b", input_cir
], check=True)

# Load and preview results
if os.path.exists(output_csv):
    df = pd.read_csv(output_csv, sep=r'\s+', comment='*')
    print(df.head())
else:
    print("❌ No output file generated.")
