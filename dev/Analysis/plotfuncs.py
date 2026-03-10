
#==================================================
# Plotting functions
#==================================================
import os
import matplotlib.pyplot as plt
import matplotlib.colors as mcolors
import numpy as np

import transforms as tf
from config import W_plot_max_reW
from helpers import nocorrect_id, build_annotation, log, is_grid_id, get_MHz_W_lists, get_Zt_list, Vdict_from_W, Z_from_W, W_from_Z
from matplotlib import ticker
from matplotlib.ticker import LogFormatter


def add_Wgrid(v_data, f_MHz):
    id_ = "grid_"+str(f_MHz)
    v_data[id_] = {
        'metadata': {'type':'grid', 'R': None, 'L': None, 'C': None, 'source': 'Internal', 'processing': None},
        'data': []
    }
    reW=np.linspace(-W_plot_max_reW, W_plot_max_reW, 10)
    imW=np.linspace(0.05, .95, 10)
    for re in reW:
        for im in imW:        
            v = Vdict_from_W(re +1j * im)
            v_data[id_]['data'].append({'MHz':f_MHz,'Vf':v['Vf'],'Vr':v['Vr'],'Vz':v['Vz'],'Va':v['Va']})


def make_id_color_map(ids):
    """
    Assign a consistent colour to each measurement ID.
    """
    ids_sorted = sorted(ids)
    cmap = plt.get_cmap('tab20', len(ids_sorted)) 
    return {id_: mcolors.to_hex(cmap(i)) for i, id_ in enumerate(ids_sorted)}

def format_common(ax):
    ax.grid(visible=True, linestyle='--', linewidth=0.5, axis='both', )
    ax.axhline(0, color='gray', linestyle='--', linewidth=0.5)
    ax.axvline(0, color='gray', linestyle='--', linewidth=0.5)

def format_WspacePlots(ax):
    ax.set_xlim(-W_plot_max_reW, W_plot_max_reW)
    ax.set_ylim(0.09, 0.9)
    ax.set_yscale("log")
    ax.set_ylabel("|Gamma|")
    ax.set_xlabel("log10 |Z|/|Zo|")

    # Remove existing locators and formatters
    ax.yaxis.set_minor_locator(plt.NullLocator())  # No minor ticks

    # Set clean, non-sci labels
    res_vals = [40, 75, 100, 220, 470]
    tickvals = [abs(50 - r) / (50 + r) for r in res_vals]
    ticklabels = [f"{v:.2f}\n({r}Ω)" for v, r in zip(tickvals, res_vals)]

    ax.yaxis.set_major_locator(plt.FixedLocator(tickvals))
    ax.yaxis.set_major_formatter(plt.FixedFormatter(ticklabels))


def mark_unused_axes(axs, used_indices=None):
    """
    Dims and labels any axes not in used_indices.
    """
    if used_indices is None:
        used_indices = []

    for i, ax in enumerate(axs):
        if i not in used_indices:
            ax.patch.set_alpha(0.1)  # Dim background
            ax.tick_params(left=False, bottom=False, labelleft=False, labelbottom=False)
            ax.text(0.5, 0.5, "(empty)", ha='center', va='center', fontsize='small', alpha=0.5, transform=ax.transAxes)


def plot_ModZModG(v_data, ax, id_color_map): 

    log("Plotting |Z|,|Gamma|")

    Rvals=[1,5,10,25,50,100,200,500]
    Xvals=[0,10,50,100,200]

    # Plot constant R lines
    for R in Rvals:
        X_range = np.linspace(0, 500, 100)  # Smooth X values 
        W = tf.W_from_Z(np.array(R/50 + 1j* X_range/50))
        line, =ax.plot(np.real(W),np.imag(W), color="grey", lw=1)
        ax.annotate("R="+str(R),line.get_xydata()[0], color=line.get_color())
    # Plot constant X lines
    for X in Xvals:
        R_range = np.geomspace(1, 500, 100)  # Smooth R values    
        W = tf.W_from_Z(np.array(R_range/50 + 1j * X/50 ))
        line, =ax.plot(np.real(W),np.imag(W), color="grey", lw=1, linestyle="dashed")
        ax.annotate("X="+str(X),line.get_xydata()[0], color=line.get_color())
    #plot data
    for id_ in v_data:
        load_type=v_data[id_]['metadata']['type']
        if(load_type!='R' and load_type!='RLC'):
            continue
        MHz, W = get_MHz_W_lists(v_data,id_)
        color = id_color_map.get(id_, "gray")
        line, =ax.plot(np.real(W),np.imag(W),color=color)
        ax.annotate(id_,line.get_xydata()[0], color=color, fontsize=12)

    # Axes and scales
    ax.set_title("All data in W space", alpha=0.8)
    ax.text(0.02, 0.05, "Grid shows constant R (solid grey)\n and constant X (dashed grey)", fontsize=6, alpha=0.6, ha='left', transform=ax.transAxes)
    format_common(ax)
    format_WspacePlots(ax)

def plot_RX_relative(v_data,ax, id_color_map):

    log("Plotting R&X")
    for id_ in v_data:
        load_type=v_data[id_]['metadata']['type']
        if(load_type!='R'):
            continue
        Rtrue = R = v_data[id_]['metadata']['R'] 
        if(Rtrue>0):
            MHz, W = get_MHz_W_lists(v_data,id_)
            Z = Z_from_W(W)
            color = id_color_map.get(id_, "gray")
            line, = ax.plot(MHz, 50*np.real(Z)/Rtrue, color=color)
            ax.annotate(id_ ,line.get_xydata()[0], fontsize=8,  color=color)
            line, = ax.plot(MHz, 50*np.imag(Z)/Rtrue, color=line.get_color(), linestyle="dashed" )
    
    # Axes and labels
    ax.set_ylim(0.6, 1.4)
    ax.set_xlabel("MHz")
    ax.set_ylabel("R / R_true, X / R_true")
    ax.set_title("Relative results for resistive loads", alpha=0.8)
    format_common(ax)


def plot_RX_compare(v_data, ax, id_color_map):
    has_RLC_data = False

    log("Plotting RLC")
    for id_ in v_data:
        md = v_data[id_]['metadata']
        if md['type'] != 'RLC' or 'LtSpice' in id_:
            continue

        has_RLC_data = True  # Found at least one RLC dataset

        log(f"Plotting {id_}")
        MHz, W = get_MHz_W_lists(v_data, id_)
        Z = tf.Z_from_W(W)
        line, = ax.plot(MHz, 50 * np.real(Z), label=id_)
        color = line.get_color()
        ax.plot(MHz, 50 * np.imag(Z), color=color)

        id_LtSpice = id_.replace('_MR300', '_LtSpice')
        if id_LtSpice in v_data:
            log(f"   vs true: {id_LtSpice}")
            MHz, W = get_MHz_W_lists(v_data, id_LtSpice)
            Z = Z_from_W(W)
            ax.plot(MHz, np.real(Z), linestyle="dashed", color=color, label=id_LtSpice)
            ax.plot(MHz, abs(np.imag(Z)), linestyle="dashed", color=color)
        else:
            log(f"   vs calculated Z for {id_}")
            Z_theoretical = get_Zt_list(v_data, id_)
            ax.plot(MHz, np.real(Z_theoretical), linestyle="dashed", color=color, label=id_ + " calculated")
            ax.plot(MHz, abs(np.imag(Z_theoretical)), linestyle="dashed", color=color)

    # Axes and labels
    if(has_RLC_data):
        ax.legend(prop={"size": 6})
        ax.set_ylim(0, 400)
        ax.set_xlabel("MHz")
        ax.set_ylabel("R, X")
        ax.set_title("R & X measured vs true for RLC loads", alpha=0.8)
        format_common(ax)

    return has_RLC_data



def plot_W_CorrectionVectors(uncorrected_data, corrected_data, ax):
    log("Plotting correction vectors in W space grid")

    for id_ in uncorrected_data:
        if 'grid_' not in id_:
            continue
        MHz, W0 = get_MHz_W_lists(uncorrected_data, id_)
        MHz, W1 = get_MHz_W_lists(corrected_data, id_)
        cmap = plt.get_cmap('rainbow')
        color = cmap(float(MHz[0])/ 60)
        
        if W1 is None:
            continue
        step = 10
        for i in range(len(W0)):
            x0,y0 = np.real(W0[i]), np.imag(W0[i])
            dx, dy = float(np.real(W1[i])-x0), float(np.imag(W1[i])-y0)
            ax.arrow(x0, y0, dx, dy, head_width=0.03, head_length=0.04, 
                     length_includes_head=True, color=color, alpha=0.6)

    # Axes and scales
    ax.set_title("Correction vectors in log10|Z|,|Gamma| space", alpha=0.8)
    ax.set_xlabel("log10 |Z|/|Zo|")
    format_WspacePlots(ax)
    format_common(ax)



def plot_W_ErrorVectors(v_data, ax):
    log("Plotting error vectors in |Z|,|Gamma| grid")

    for id_ in v_data:

        load_type=v_data[id_]['metadata']['type']
        if(load_type!='R' and load_type!='RLC'):
            continue

        MHz, W0 = get_MHz_W_lists(v_data, id_)
        cmap = plt.get_cmap('rainbow')
        meta=v_data[id_]['metadata']
        
        if load_type == 'RLC':
            Zt = get_Zt_list(v_data, id_)
            assert len(Zt) == len(W0), f"{id_}: theoretical data missing"

        step = 10
        for i in range(len(W0)):
            if (i % step)!=0:
                continue
            if(load_type == 'R'):
                R = meta['R']
                W1 = W_from_Z(R/50 + 1j*0)
            else:
                W1 = W_from_Z(Zt[i]/50)  
            color = cmap(float(MHz[i])/ 60)
            x0,y0 = np.real(W0[i]), np.imag(W0[i])
            dx, dy = float(np.real(W1)-x0), float(np.imag(W1)-y0)
            ax.arrow(x0, y0, dx, dy, head_width=0.03, head_length=0.04, 
                     length_includes_head=True, color=color, alpha=0.6)

    # Axes and scales
    ax.set_title("Error vectors in W space", alpha=0.8)
    format_WspacePlots(ax)
    format_common(ax)

def prepare_figure():
    fig = plt.figure(figsize=(20, 0.75*20))
    axs = [fig.add_subplot(2, 3, i + 1) for i in range(6)]
    correction_name = ""
    return fig, axs

def build_and_save_plot(v_data, fig, axs, datafilename, plotfilename, correction_name, id_color_map, output_folder="Plots", v_data_uc=None):
    
    plot_ModZModG(v_data, axs[0], id_color_map)
    plot_RX_relative(v_data, axs[1], id_color_map)
    plot_W_ErrorVectors(v_data, axs[2])
    used = [0, 1, 2]

    if (v_data_uc is not None):
        plot_W_CorrectionVectors(v_data_uc,v_data, axs[3])
        used.append(3)
    if(plot_RX_compare(v_data, axs[4], id_color_map)):
        used.append(4)

    mark_unused_axes(axs, used_indices=used)

    toptext=fig.text(0.11,0.93,"Data: "+datafilename + "\nCorrection: "+correction_name, size='small', weight='bold')
    if not os.path.exists(output_folder):
        os.makedirs(output_folder)
    filepath = os.path.join(output_folder, plotfilename)
    fig.savefig(filepath, dpi=300)
    log(f"Saved plot to {filepath}")
