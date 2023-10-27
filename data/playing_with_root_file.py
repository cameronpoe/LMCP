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

    # print(pore_branches['TrackID'])
    # fig, ax = plt.subplots()
    # ax.hist(ak.flatten(pore_branches['TrackID', pore_branches['CreatorProc'] != 'eIoni']), bins=10, range=(2,12), density=True)

    # fig, ax = plt.subplots()
    # new_array = pore_branches['CreatorProc', pore_branches['TrackID'] == 2]
    # print(ak.flatten(new_array))
    # ax.hist(ak.flatten(new_array))

    # fig, ax = plt.subplots()
    # new_array = pore_branches['CreatorProc', pore_branches['TrackID'] == 3]
    # ax.hist(ak.flatten(new_array))

    # fig, ax = plt.subplots()
    # ax.hist(ak.flatten(pore_branches['CreatorProc', pore_branches['CreatorProc'] != 'eIoni']))

    fig, ax = plt.subplots()
    ax.hist(ak.flatten(pore_branches['EKin', pore_branches['CreatorProc'] == 'phot']), bins=np.linspace(0,600,121,dtype=int))

    fig, ax = plt.subplots()
    ax.hist(ak.flatten(pore_branches['EKin', (pore_branches['CreatorProc'] == 'compt') & (pore_branches['TrackID'] == 2)]), bins=np.linspace(0,600,121,dtype=int))
    plt.show()



# intermediate_dict = {}
        # for item in pore_branches['CreatorProc']:
        #     new_item = np.unique(item)
        #     for label in new_item:
        #         if label not in intermediate_dict:
        #             intermediate_dict[label] = 0
        #         intermediate_dict[label] += 1