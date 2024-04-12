import numpy as np
from matplotlib import pyplot as plt
import awkward as ak
import uproot

root_file = r"../raw_data/wp_data/Run_e511_w100_p105_theta0_phi0.root"
# root_file = r'data/Run_neutron_test.root'

with uproot.open(root_file) as f:
    # Print keys for pore tree
    pore_tree = f["lamina"]
    print(pore_tree.keys())

    # Format as awkward array and fix weird string-vector issue
    pore_branches = pore_tree.arrays(library="ak")
    print(len(pore_branches["EventNumber"]))
    pore_branches["CreatorProc"] = ak.str.split_pattern(
        pore_branches["CreatorProc"], "\n"
    )

    NUM_EVENTS = 1e5
    NUM_ELECTRONS = ak.sum(pore_branches["PDGID"] == 11)

    # Histograms different secondaries
    labels, counts = np.unique(
        ak.to_numpy(ak.flatten(pore_branches["PDGID"])), return_counts=True
    )
    print(labels)
    particle_dict = {
        "-12": "antie- v",
        "-11": "e+",
        "11": "e-",
        "22": "gamma",
        "2212": "p",
        "1000010020": "deuteron",
        "1000060120": "C-12",
        "1000060130": "C-13",
        "1000060140": "C-14",
        "1000080160": "O-16",
        "1000080170": "O-17",
        "1000080180": "O-18",
    }
    new_labels = []
    for label in labels:
        new_labels.append(particle_dict[str(label)])
    print(new_labels)
    # fig, ax = plt.subplots()
    # fig.set_size_inches(7,6.5)
    # ax.bar(new_labels, counts, label='Secondaries')
    # ax.axhline(1e5, label='Primaries', color='red')
    # ax.tick_params(axis='x', labelrotation=45)
    # ax.set_yscale('log')
    # ax.set_ylim(top=1000000)
    # ax.set_ylabel('Number of secondaries', fontdict=dict(size=14))
    # ax.tick_params(axis='both', which='major', labelsize=12)
    # ax.xaxis.set_ticks_position('both')
    # ax.yaxis.set_ticks_position('both')
    # ax.legend(prop=dict(size=12))
    # plt.minorticks_on()
    # plt.show()

    # Histograms e- creation processes
    proc_labels = np.unique(
        np.array(ak.flatten(pore_branches["CreatorProc", pore_branches["PDGID"] == 11]))
    )
    proc_dict = {
        "compt": "Comptons",
        "eIoni": "Ionized by e-",
        "hIoni": "Ionized by hadron",
    }
    proc_counts = []
    better_proc_labels = []
    for i, proc_label in enumerate(proc_labels):
        event_cut_by_process = ak.any(
            pore_branches["CreatorProc"] == proc_label, axis=1
        )
        proc_counts.append(
            len(
                pore_branches["CreatorProc", pore_branches["PDGID"] == 11][
                    event_cut_by_process
                ]
            )
        )
        better_proc_labels.append(proc_dict[proc_label])
    # fig, ax = plt.subplots()
    # proc_bars = ax.bar(better_proc_labels, np.array(proc_counts)/NUM_EVENTS*100)
    # for rect in proc_bars:
    #     height = rect.get_height()
    #     ax.text(rect.get_x() + rect.get_width() / 2.0, height, f'{height:.2f}%', ha='center', va='bottom')
    # ax.set_ylim(0, 30)
    # ax.set_ylabel('Percent of events', fontdict=dict(size=14))
    # ax.tick_params(axis='both', which='major', labelsize=12)
    # ax.xaxis.set_ticks_position('both')
    # ax.yaxis.set_ticks_position('both')
    # plt.minorticks_on()
    # plt.show()

    # Histograms e- for different creation processes
    fig, ax = plt.subplots()
    bins = np.linspace(
        0, 5, 101
    )  # keV - note, this bounds doesn't capture all electrons due to the high energy e- formed scattering capture gammas
    for proc_label in proc_labels:
        ekin_i = pore_branches["EKin"] - pore_branches["Edep"]
        particle_cut_by_type = pore_branches["PDGID"] == 11
        particle_cut_by_proc = pore_branches["CreatorProc"] == proc_label
        ekin_elec = ekin_i[particle_cut_by_type & particle_cut_by_proc]
        counts, bins = np.histogram(ak.flatten(ekin_elec), bins=bins)
        ax.plot(bins[:-1], counts / NUM_ELECTRONS * 100, label=proc_dict[proc_label])
    ax.legend()
    ax.set_xlabel("Initial e- energy (keV)", fontdict=dict(size=12.5))
    ax.set_ylabel("Percent of total electrons per 50 eV bin", fontdict=dict(size=12.5))
    ax.xaxis.set_ticks_position("both")
    ax.yaxis.set_ticks_position("both")
    plt.minorticks_on()
    plt.show()

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

    # fig, ax = plt.subplots()
    # vals1, __, __ = ax.hist(ak.flatten(pore_branches['EKin', pore_branches['CreatorProc'] == 'phot']), bins=np.linspace(0,600,121,dtype=int), alpha=0.7, label='Photoelectric', histtype='bar')

    # # fig, ax = plt.subplots()
    # vals2, __, __ = ax.hist(ak.flatten(pore_branches['EKin', (pore_branches['CreatorProc'] == 'compt')]), bins=np.linspace(0,600,121,dtype=int), alpha=0.7, label='Compton', histtype='bar')
    # ax.legend()

    # fig, ax = plt.subplots()
    # double_array = [ak.flatten(pore_branches['EKin', pore_branches['CreatorProc'] == 'phot']), ak.flatten(pore_branches['EKin', (pore_branches['CreatorProc'] == 'compt')])]
    # ax.hist(double_array, histtype='bar', bins=np.linspace(0,600,61))
    # plt.show()
