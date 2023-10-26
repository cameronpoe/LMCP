import numpy as np
from matplotlib import pyplot as plt
import awkward as ak
import uproot

root_file = r'raw_data/latest_run/Run_e510_wall50_pore_50_theta0_phi0.root'

with uproot.open(root_file) as f:

    pore_tree = f['pore']
    print(pore_tree.keys())

    pore_branches = pore_tree.arrays(library='ak')
    pore_branches['CreatorProc'] = ak.str.split_pattern(pore_branches['CreatorProc'], '\n')
    print(pore_branches)
    print(pore_branches['EventNumber'])
    print(pore_branches['ID'])
    print(pore_branches['CreatorProc'])
    # for i, item in enumerate(pore_branches['CreatorProc']):
    #     if len(item) > 1:
    #         print(i)
    
    a = pore_branches['CreatorProc']
    b = a == 'phot'
    print(b[160500])
    print(a == 'phot')
    print(a[160500])
    # fig, ax = plt.subplots()
    # ax.hist(ak.flatten(pore_branches['CreatorProc']))
    # plt.show()



# intermediate_dict = {}
        # for item in pore_branches['CreatorProc']:
        #     new_item = np.unique(item)
        #     for label in new_item:
        #         if label not in intermediate_dict:
        #             intermediate_dict[label] = 0
        #         intermediate_dict[label] += 1