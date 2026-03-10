import math
import numpy as np
from helpers import log, nocorrect_id, is_grid_id
import bisect
from helpers import get_MHz_W_lists, W_from_Vdict, Vdict_from_W, Vdict_from_Z, W_from_Z, Z_from_Y, Y_from_W, Z_from_W


#==========================================================
# Bridge data analysis and correction - voltage correction
#==========================================================
def build_banded_corrections(v_data, cal_IDs):
   # log("Building "+correction_name)
    banded_corrections = {}
    global band_limits
    band_limits = [(1.0, 1.8, 2.0), (2.0, 3.7, 5.0), (5.0, 7.1, 8.0),  # etc...
                   (8.0, 10.1, 11.0), (11.0, 12.0, 13.0), (13.0, 14.1, 17.0),
                   (17.0, 18.1, 19.0), (19.0, 21.0, 23.0), (23.0, 24.9, 26.0),
                   (26.0, 27.0, 28.0), (28.0, 29.0, 31.0), (31.0, 35.5, 40.0),
                   (40.0, 44.5, 49.0), (49.0, 51.0, 61.0)]

    for f_Low, f_cal, f_High in band_limits:
        values = {}

        # Find closest calibration entry for each cal_id
        for cal_id in cal_IDs:
            closest_entry = min(
                v_data[cal_id]['data'],
                key=lambda e: abs(e['MHz'] - f_cal)
            )
            values[cal_id] = closest_entry

        # Extract voltages
        Vfl = values[cal_IDs[0]]['Vf']
        Vfm = values[cal_IDs[1]]['Vf']
        Vfh = values[cal_IDs[2]]['Vf']
        Vrm = values[cal_IDs[1]]['Vr']
        Vrh = values[cal_IDs[2]]['Vr']
        Vzl = values[cal_IDs[0]]['Vz']
        Vzm = values[cal_IDs[1]]['Vz']
        Val = values[cal_IDs[0]]['Va']
        Vam = values[cal_IDs[1]]['Va']

        # Compute true expected voltages
        vrm_t = Vfm * abs(150 - 50) / (150 + 50)
        vrh_t = Vfh * abs(220 - 50) / (220 + 50)
        vzl_t = Vfl  * 50/(50 + 50)
        vzm_t = Vfm * 150/(150 + 50)
        val_t = Vfl  * 50/(50 + 50)
        vam_t = Vfm * 50/(150 + 50)

        # Compute correction coefficients
        mVr = (vrh_t/Vrh - vrm_t/Vrm) / (Vrh - Vrm)
        cVr = vrh_t/Vrh - mVr * Vrh
        mVz = (vzm_t/Vzm - vzl_t/Vzl) / (Vzm - Vzl)
        cVz = vzm_t/Vzm - mVz * Vzm
        mVa = (vam_t/Vam - val_t/Val) / (Vam - Val)
        cVa = vam_t/Vam - mVa * Vam

        # Store correction for this band
        banded_corrections[f_cal] = [mVr, cVr, mVz, cVz, mVa, cVa]
        log(f"Built correction for f_cal={f_cal:.1f} MHz:", values= [mVr, cVr, mVz, cVz, mVa, cVa])

    return banded_corrections


def apply_banded_corrections(v_data, banded_corrections):
   # log("Applying "+correction_name)
    for id_ in v_data:
        if nocorrect_id(id_):
            continue
        for i, entry in enumerate(v_data[id_]['data']):
            f = entry['MHz']
            f_key = None
            for f_Low, f_center, f_High in band_limits:
                if f_Low <= f < f_High:
                    f_key = f_center
                    break
            mVr, cVr, mVz, cVz, mVa, cVa = banded_corrections[f_key]
            entry['Vr'] = entry['Vr'] * (entry['Vr'] * mVr + cVr)
            entry['Vz'] = entry['Vz'] * (entry['Vz'] * mVz + cVz)
            entry['Va'] = entry['Va'] * (entry['Va'] * mVa + cVa)

    
#==========================================================
# Bridge correction: W warping
#==========================================================
def features(W,MHz):
   reW=np.real(W)
   imW=np.imag(W)
   f= [1,reW,imW, MHz*reW, MHz*abs(reW), MHz*imW]   
   if (max(abs(np.array(f)))>100):
      log("Large values in feature list: "+str(f))
   return f

def fit_W_matrix(v_data, cal_IDs):
    """
    Fit W matrix correction based on calibration loads.
    Uses R from metadata if available, otherwise assumes measured value is target (e.g. RLC).
    """
   # log("Building "+correction_name)

    F_meas, W_targs = [], []

    for id_ in cal_IDs:
        if id_ not in v_data:
            log(f"Warning: Calibration ID {id_} not found in data — skipping")
            continue

        md = v_data[id_]['metadata']
        R = md['R']

        for meas in v_data[id_]['data']:
            F_meas.append(features(W_from_Vdict(meas), meas['MHz']))

            if R is not None:
                # Known resistor load → target is theoretical W
                W_targs.append(W_from_Z(R / 50 + 0j))
            else:
                # RLC or unknown → target is measured value (could be improved if you have true model)
                W_targs.append(W_from_Vdict(meas))

    # Solve for matrix
    MwT, *_ = np.linalg.lstsq(np.array(F_meas), np.array(W_targs), rcond=None)
    Mw = MwT.T
   # log("W Matrix:\n", Mw)
    return Mw

def apply_W_matrix(v_data, Mw):
  #  log("Applying "+correction_name)
    for id_ in v_data:
        if nocorrect_id(id_):
            continue
        for i, entry in enumerate(v_data[id_]['data']):
            F_meas = features(W_from_Vdict(entry), entry['MHz'])
            corrected_entry = Vdict_from_W(Mw @ F_meas)
            corrected_entry['MHz'] = entry['MHz']  # retain frequency
            v_data[id_]['data'][i] = corrected_entry


#==========================================================
# Bridge data analysis and correction 
#      - linear mapping in W space
#==========================================================
def linmap(x,s0,s1,d0,d1):
    return d0+(d1-d0)*(x-s0)/(s1-s0)

def build_WTLC(v_data, cal_IDs):
   # log("Building "+correction_name)
    Wtru_Lo    =   W_from_Z(v_data[cal_IDs[0]]['metadata']['R'] / 50 + 1j * 0)
    Wtru_Match =   W_from_Z(v_data[cal_IDs[1]]['metadata']['R'] / 50 + 1j * 0)
    Wtru_Hi    =   W_from_Z(v_data[cal_IDs[2]]['metadata']['R'] / 50 + 1j * 0)

    Wcal_Lo, Wcal_Match, Wcal_Hi = [], [], []
    for entry_lo, entry_match, entry_hi in zip(
        v_data[cal_IDs[0]]['data'],
        v_data[cal_IDs[1]]['data'],
        v_data[cal_IDs[2]]['data']
    ):
        Wcal_Lo.append(W_from_Vdict(entry_lo))
        Wcal_Match.append(W_from_Vdict(entry_match))
        Wcal_Hi.append(W_from_Vdict(entry_hi))

    return Wcal_Lo, Wtru_Lo, Wcal_Match, Wtru_Match, Wcal_Hi, Wtru_Hi 

def apply_WTLC(v_data, Wcal_Lo, Wtru_Lo, Wcal_Match, Wtru_Match, Wcal_Hi, Wtru_Hi, MHz_common, correctModZ=True, correctModG=True ):
  #  log("Applying "+correction_name)

    for id_ in v_data:
        if nocorrect_id(id_):
            continue
        for i,entry in enumerate(v_data[id_]['data']):
            v_data[id_]['data'][i]['Vr']=v_data[id_]['data'][i]['Vr']
            icom = i if (not is_grid_id(id_)) else bisect.bisect_right(MHz_common, entry['MHz']) - 1
            Wm = W_from_Vdict(v_data[id_]['data'][i])
            imWm=np.imag(Wm)
            reWm=np.real(Wm)
            if(reWm<np.real(Wcal_Match[icom])):
                reWcorr = linmap(reWm,np.real(Wcal_Lo[icom]),np.real(Wcal_Match[icom]),np.real(Wtru_Lo),np.real(Wtru_Match))
            else:
                reWcorr = linmap(reWm,np.real(Wcal_Match[icom]),np.real(Wcal_Hi[icom]),np.real(Wtru_Match),np.real(Wtru_Hi))
            imWcorr_1 = linmap(imWm,np.imag(Wcal_Match[icom]),np.imag(Wcal_Lo[icom]),np.imag(Wtru_Match),np.imag(Wtru_Lo))
            imWcorr_2 = linmap(imWm,np.imag(Wcal_Match[icom]),np.imag(Wcal_Hi[icom]),np.imag(Wtru_Match),np.imag(Wtru_Hi))
            imWcorr = linmap(reWcorr,np.real(Wtru_Lo),np.real(Wtru_Hi), imWcorr_1, imWcorr_2)

            if (correctModZ):
                reWm = reWcorr
            if (correctModG):
                imWm = imWcorr

            corrected_entry = Vdict_from_W(reWm +1j* imWm)
            for key in ('Vf', 'Vr', 'Vz', 'Va'):
                entry[key] = corrected_entry[key]


#==========================================================
# Bridge data analysis and correction - hybrid scheme
# Voltage correction for Vr plus linear map for log|Z|
#==========================================================
def build_VrVoltage_corrections(v_data, cal_IDs, corr_type='linear'):
   # log("Building "+correction_name)
    VrVoltage_corrections = {}
    R0cal = v_data[cal_IDs[1]]['metadata']['R']
    R1cal = v_data[cal_IDs[2]]['metadata']['R']

    Vr0m, Vr0t = [],[]
    for i,entry in enumerate(v_data[cal_IDs[1]]['data']):
        Vf0m = entry['Vf']
        Vr0m.append(entry['Vr'])
        Vr0t.append(Vf0m * abs(R0cal - 50) / (R0cal + 50))
    Vr1m, Vr1t = [],[]
    for i,entry in enumerate(v_data[cal_IDs[2]]['data']):
        Vf1m = entry['Vf']
        Vr1m.append(entry['Vr'])
        Vr1t.append(Vf1m * abs(R1cal - 50) / (R1cal + 50))
        
    if (corr_type=='linear'):
        mVr = (np.array(Vr1t) - np.array(Vr0t)) / (np.array(Vr1m) - np.array(Vr0m))
        cVr = np.array(Vr0t) - mVr * np.array(Vr0m)
    else:
        mVr = (np.array(Vr1t)/np.array(Vr1m) - np.array(Vr0t)/np.array(Vr0m)) / (np.array(Vr1m) - np.array(Vr0m))
        cVr = np.array(Vr1t)/np.array(Vr1m) - np.array(mVr) * np.array(Vr1m)

    return mVr, cVr


def apply_VrVoltage_corrections(v_data, mVr, cVr, MHz_common, corr_type='linear'):
  #  log("Applying "+correction_name)
    for id_ in v_data:
        if nocorrect_id(id_):
            continue
        for i, entry in enumerate(v_data[id_]['data']):
            icom = i if (not is_grid_id(id_)) else bisect.bisect_right(MHz_common, entry['MHz']) - 1
            if (corr_type=='linear'):
                entry['Vr'] = entry['Vr'] * mVr[icom] + cVr[icom]
            else:
                entry['Vr'] = entry['Vr'] * (entry['Vr'] * mVr[icom] + cVr[icom])
             

#==========================================================
# Bridge data analysis and correction - affine warp
#==========================================================
def fit_W_affine(v_data, cal_IDs):
    """
    Build affine correction parameters per frequency.
    Uses calibration IDs to fit per-frequency linear mapping in W space.
    Returns a dictionary indexed by frequency.
    """
  #  log("Building " + correction_name)

    affine_by_freq = {}

    # Assume all calibration IDs have the same frequency list
    MHz_common = [entry['MHz'] for entry in v_data[cal_IDs[0]]['data']]

    for i, f_MHz in enumerate(MHz_common):
        W_meas = []
        W_true = []

        for id_ in cal_IDs:
            entry = v_data[id_]['data'][i]
            W_meas.append(W_from_Vdict(entry))

            R = v_data[id_]['metadata']['R']
            if R is not None:
                W_true.append(W_from_Z(R / 50 + 0j))
            else:
                # Optional: fallback for unknown loads
                W_true.append(W_from_Vdict(entry))

        # Fit affine map Wcorr = M @ Wmeas + offset
        A = np.vstack((W_meas, np.ones(len(W_meas)))).T  # Shape (N, 3)
        B = np.array(W_true)

        # Solve least squares
        coeffs, *_ = np.linalg.lstsq(A, B, rcond=None)
        M = coeffs[:-1]
        offset = coeffs[-1]

        affine_by_freq[f_MHz] = {'M': M, 'offset': offset}

  #  log(f"Affine map built for {len(MHz_common)} frequencies")
    return affine_by_freq


def apply_W_affine(v_data, affine_by_freq, MHz_common):
  #  log("Applying affine correction")

    for id_ in v_data:
        if nocorrect_id(id_):
            continue
        is_grid = is_grid_id(id_)
        for i, entry in enumerate(v_data[id_]['data']):
            icom = i if not is_grid else bisect.bisect_right(MHz_common, entry['MHz']) - 1
            f_key = MHz_common[icom]

            M = affine_by_freq[f_key]['M']
            offset = affine_by_freq[f_key]['offset']

            Wmeas = W_from_Vdict(entry)
            Wcorr = M * Wmeas + offset
            Wcorr = Wcorr.item()
            corrected_entry = Vdict_from_W(Wcorr)
            for key in ('Vf', 'Vr', 'Vz', 'Va'):
                entry[key] = corrected_entry[key]


#==========================================================
# Bridge data analysis and correction - clip to valid Z
#==========================================================
def apply_clip_valid_Z(v_data):
  #  log("Applying "+correction_name)

    for id_ in v_data:
        if nocorrect_id(id_):
            continue
        MHz, W = get_MHz_W_lists(v_data,id_)
        for i, entry in enumerate(v_data[id_]['data']):
            Zcorr = Z_from_W(W[i])
            corrected_entry = Vdict_from_Z(Zcorr)
            for key in ('Vf', 'Vr', 'Vz', 'Va'):
                entry[key] = corrected_entry[key]
            
#==========================================================
# Bridge data analysis and correction - open port
#==========================================================
def apply_port_model(v_data):
  #  log("Applying "+correction_name)

    for id_ in v_data:
        if nocorrect_id(id_):
            continue
        MHz, W = get_MHz_W_lists(v_data,id_)
        for i, entry in enumerate(v_data[id_]['data']):
            Ym = Y_from_W(W[i])
            c = -10e-12
            Yport = 0.001 + 1j * entry['MHz']*6.282*1e6*c
            Zcorr  = Z_from_Y(Ym - Yport)
            corrected_entry = Vdict_from_Z(Zcorr)
            for key in ('Vf', 'Vr', 'Vz', 'Va'):
                entry[key] = corrected_entry[key]
    
def apply_port_measurement(data, portID):
   # log("Applying "+correction_name + " with "+ portID)

    MHz_port, Wport = get_MHz_W_lists(data, portID)
    
    for id_ in data:
        if nocorrect_id(id_) or id_ == portID:
            continue

        MHz_meas, Wmeas = get_MHz_W_lists(data, id_)
        for i, entry in enumerate(v_data[id_]['data']):
            icom = i if (not is_grid_id(id_)) else bisect.bisect_right(MHz_common, entry['MHz']) - 1
            Yport = Y_from_W(Wport[icom])
            Ymeas = Y_from_W(Wmeas[i])
            Zcorr = Z_from_Y(Ymeas - Yport)
            corrected_entry = Vdict_from_Z(Zcorr)
            corrected_entry = Vdict_from_Z(Zcorr)
            for key in ('Vf', 'Vr', 'Vz', 'Va'):
                entry[key] = corrected_entry[key]


