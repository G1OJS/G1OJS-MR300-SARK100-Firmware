import numpy as np

# ===== Global log verbosity flag =====
VERBOSE = True



#==================================================
# Z <-> W <-> V transforms 
# W = ln|Z| + j|1-Z|/|1+Z|
#
# Strategy: no domain tranformations outside this block
# allows experimentation with the intermediate domain
#
#==================================================

# ... could have this but is it overkill?
#class Wdef(Enum):
#    ModZ_jModGamma = Auto()
#    LogModZ_jModGamma = Auto()
#def reW(Z):    
#def imW(Z):

def Z_from_W(W):
    mz  =  pow(10,np.real(W))
    mg  =  np.imag(W)
    swr = (1+mg)/(1.0000001-mg)
    R = (1+mz*mz) * swr / (swr*swr+1.0)
    XX = np.clip(mz*mz - R*R,0,None)
    X=np.sqrt(XX)
    return R +  1j * X  

# Y_from_W just to get this code working, but wondering how this looks written
# analogously to the Z_from_W function?
def Y_from_W(W):
    return 1.0 / Z_from_W(W)

def Z_from_Y(Y):
    return 1.0/Y

def W_from_Z(Z):
    return np.log10(np.abs(Z)+1e-12) + 1j * np.abs((1 - Z) / (1 + Z))

def W_from_Vdict(v_data):
    return np.log10(v_data['Vz']/v_data['Va']) + 1j * v_data['Vr']/v_data['Vf']

def Vdict_from_Z(Z):
    return Vdict_from_W(W_from_Z(Z))

def Vdict_from_W(W):
    mz=pow(10,np.real(W))
    return {'Vf':1.0, 'Vr':np.imag(W), 'Vz':mz/(1+mz), 'Va':1/(1+mz)}

def get_MHz_W_lists(v_data, id_):
    MHz, W = [], []
    for datum in v_data[id_]['data']:
        MHz.append(datum['MHz'])
        W.append(W_from_Vdict(datum))
    return MHz, W

def get_Zt_list(v_data, id_):
    Zt = []
    for datum in v_data[id_]['data']:
        assert 'Z_theoretical' in datum and datum['Z_theoretical'] is not None, f"{id_}: missing Z_theoretical"
        Zt.append(datum['Z_theoretical'])
    return Zt

def nocorrect_id(id_):
    """
    Return True if this ID is synthetic, calibration, or otherwise should be skipped in processing.
    """
    return (
        'true' in id_
        or '_ci' in id_
        or 'LtSpice' in id_
    )


def is_grid_id(id_):
    """
    Return True if this ID is part of synthetic grid points (for display suppression).
    """
    return id_.startswith('grid_')


def build_annotation(correction_name, input_file, cal_ids):
    """
    Generate a standardized annotation string for plots.
    """
    cal_list = ', '.join(cal_ids)
    text = f"Correction: {correction_name}\nInput: {input_file}\nCalibration: {cal_list}"
    return text


def log(message, matrix=None, level="INFO", sigfigs=3, values=None):
    """
    Enhanced log function.
    - message: main log message
    - matrix: optional numpy array to display
    - values: optional list/tuple of numbers to format
    """
    if not VERBOSE:
        return

    prefix = f"[{level}] "
    print(prefix + message)

    if values is not None:
        formatted = [f"{v:.{sigfigs}g}" for v in values]
        print(prefix + "Values: " + str(formatted))

    if matrix is not None:
        # Format matrix
        def format_number(x):
            return f"{x:.{sigfigs}g}"
        formatted = np.vectorize(format_number)(matrix)
        print(prefix + "Matrix:")
        for row in formatted:
            row_str = "  ".join(f"{elem:>8}" for elem in row)
            print(prefix + row_str)

