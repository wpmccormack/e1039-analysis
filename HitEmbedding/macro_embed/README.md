# `macro_emb`:  Macro to carry out the hit embedding

This macro is to carry out the hit embedding.
It reads
* The signal data (`DST.root`) that you produced with `macro_gen_signal` and
* The embedding data (`embedding_data.root`) that you procuded with `macro_gen_emb`.

It writes out the embedded data into a new DST file (`DST.root`).

## Preparation

You first make lists of signal and embedding data; `list_sig_file.txt` and `list_emb_file.txt`.
You modify `make-list.sh` so that `DIR_SIG` and `DIR_EMB` in the script points to the directories where you store the files.

You then execute the script as follows;
```
./make-lists.sh
```
You should check that the contents of the two files are valid.

## Method of pairing signal data and embedding data

One signal data (`DST.root`) and one embedding data (`embedding_data.root`)
are paired from the top of the lists in sequence.
The pairing will be made by the smaller number of data files.

Per paired data files,
one signal event and one embedding data are paired from the head of event in sequence.
The pairing will be made by the smaller number of events.

Note that the orders of the data files and events are _not_ shuffled at random.
It is fine because
* The orders stored are already random, and
* The fixed order is helpful in checking the reproducibility of the hit embedding.
But the random pairing might be implemented in future.

## Usage

Two scripts (`gridsub.sh` and `gridrun.sh`) are used as usual, although the contents of `gridsub.sh` are quite difference since it needs two input data files.

You first execute the following command for test;
```
./gridsub.sh -j 1 -e 10 20210823_01
```
It processes only the first 10 events in the 1st job (i.e. input file pair) on local.

You then execute the following commands to submit grid jobs;
```
source /e906/app/software/script/setup-jobsub-spinquest.sh
kinit
./gridsub.sh -g 20210823_01
```

## Event reconstruction

The event reconstruction is carried out after the hit embedding.
The setting has to be identical to the one in `macro_gen_signal/Fun4Sim.C`.
The reconstruction here makes a result affected by the hit embedding.
