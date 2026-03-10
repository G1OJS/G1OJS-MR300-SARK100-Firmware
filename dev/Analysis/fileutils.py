import csv
import numpy as np
from helpers import log 

def parse_id(id_str):
    parts = id_str.split('_')
    result = {'type': parts[0]}

    if parts[0] == 'R':
        result['R'] = float(parts[1])
        result['L'] = None
        result['C'] = None
        result['source'] = parts[2]
        result['processing'] = parts[3]
    elif parts[0] == 'RLC':
        result['R'] = float(parts[1])
        result['L'] = float(parts[2])
        result['C'] = float(parts[3])
        result['source'] = parts[4]
        result['processing'] = parts[5]
    elif parts[0] == 'X':
        result['R'] = None
        result['L'] = None
        result['C'] = None
        result['source'] = parts[1]
        result['processing'] = parts[2]
    else:
        raise ValueError(f"Unknown ID type: {parts[0]}")

    return result

def rlc_impedance(frequency, R, L, C):
    """Return complex impedance of parallel RLC circuit at given frequency (Hz)."""
    omega = 2 * np.pi * frequency
    Z_R = R
    Z_L = 1j * omega * L *1e-6
    Z_C = 1 / (1j * omega * C *1e-12)  
    Y_total = 1 / Z_R + 1 / Z_L + 1 / Z_C
    return 1 / Y_total

def read_voltage_file(datafilename):
    """Read bridge voltage CSV file and parse metadata."""
    v_data = {}
    print(f"Reading data from {datafilename}")
    with open(datafilename, newline='') as csvfile:
        reader = csv.DictReader(row for row in csvfile if not row.startswith('#'))
        for row in reader:
            id_ = row["ID"]
            if id_ not in v_data:
                log("Reading "+id_)
                v_data[id_] = {
                    'metadata': parse_id(id_),
                    'data': []
                } 

            freq_hz = float(row["Hz"])
            datum = {
                'MHz': freq_hz / 1e6,
                'Vf': float(row['Vf']),
                'Vr': float(row['Vr']),
                'Vz': float(row['Vz']),
                'Va': float(row['Va'])
            }

            # Add expected RLC impedance if applicable
            md = v_data[id_]['metadata']
            if md['type'] == 'RLC':
                Z = rlc_impedance(freq_hz, md['R'], md['L'], md['C'])
                datum['Z_theoretical'] = Z
 
            v_data[id_]['data'].append(datum)

    for id_, v in v_data.items():
        if v['metadata']['type'] == 'RLC':
            count = sum('Z_theoretical' in d and d['Z_theoretical'] is not None for d in v['data'])
            print(f"{id_}: {count}/{len(v['data'])} theoretical values")

    
    return v_data
