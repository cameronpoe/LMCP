import uproot
import awkward as ak
import numpy as np

file_path = r'raw_data/latest_run/Run_e511_w50_p50_theta0_phi0.root'

with uproot.open(file_path) as f:

    pore_tree = f['pore']
    pore_branches = pore_tree.arrays(library='ak')
    for unique_track_id in np.unique(ak.flatten(pore_branches['TrackID'])):
        num_events_with_at_least_one_of_unique_track_id = ak.sum(ak.any(pore_branches['TrackID'] == unique_track_id, axis=1))
        num_of_unique_track_id = ak.sum(ak.flatten(pore_branches['TrackID']) == unique_track_id)
        if num_events_with_at_least_one_of_unique_track_id != num_of_unique_track_id:
            print(unique_track_id)

    # Below is how numerator of efficiency is calculated
    num_electrons_reached_pore = ak.sum(ak.any(pore_branches['PDGID'] == 11, axis=1))