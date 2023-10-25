import numpy as np
from matplotlib import pyplot as plt
import awkward as ak
import uproot

root_file = r'raw_data/latest_run/Run_wall50_pore_50_theta0_phi0.root'

with uproot.open(root_file) as f:

    pore_tree = f['pore']
    print(pore_tree.keys())

    pore_branches = pore_tree.arrays(library='ak')
    pore_branches['CreatorProc'] = ak.str.split_pattern(pore_branches['CreatorProc'], '\n')
    print(pore_branches)
    print(pore_branches['EventNumber'])
    print(pore_branches['ID'])
    print(pore_branches['CreatorProc'])
    fig, ax = plt.subplots()
    ax.hist(ak.flatten(pore_branches['CreatorProc']))
    plt.show()
