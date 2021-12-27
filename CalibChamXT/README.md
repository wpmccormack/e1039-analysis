# e1039-analysis/CalibChamXT

A program to calibrate the chamber X-T curve, using cosmic-ray events at present.
The file structure, the configuration and the usage are similar to the following programs;

* https://cdcvs.fnal.gov/redmine/projects/calib-cham-rt
* https://github.com/E1039-Collaboration/e1039-analysis/tree/master/AnaRealDst
* https://github.com/E1039-Collaboration/e1039-analysis/tree/master/RecoDev
* /seaquest/users/apun/abi_project/data_manage/e1039-data-mgt_test


## Download

You are recommended to use `spinquestgpvm01`.
You first clone the GitHub repository as usual;

```
ssh spinquestgpvm01.fnal.gov
cd /path/to/your/working/directory
git clone git@github.com:E1039-Collaboration/e1039-analysis.git
cd e1039-analysis/CalibChamXT
```

The top directory of this program is `e1039-analysis/CalibChamXT/`.
All files/directories explained below are relative to the top directory, unless specified.


## Preparation

The runs to be analyzed are listed in `work/list_run.txt`.
The list contains only run 3492 at present.
You could modify the list when necessary.

You copy the DST files of all runs from `data2` to `pnfs` as follows.
It usually takes a few minutes per run;

```
cd work
./copy_dst.sh
```

The source code is located in `src/`.
The following commands set up the shell environment and compile the code;

```
source setup.sh
cmake-this
make-this
```


## Track Reconstruction

The program can execute the track reconstruction, not yet the calibration of the X-T curve.

The following commands carry out a test, using 10,000 events in the 1st DST file.
It should take 10-20 seconds.

```
cd work
./run_reco.sh -d 1 -e 10000 GO
```

The main output file is `scratch/main/run_003492_spill_000000000_spin/out/eval.root`.
The ROOT file (`eval.root`) contains all tracklets.
You had better check the number of tree entries is non-zero.

You then execute the following commands to process all events/runs.
The `-g` option is to run jobs on the grid.

```
source /e906/app/software/script/setup-jobsub-spinquest.sh
kinit
./run_reco.sh -g GO
```

You use `jobsub_q_mine` (or `./check_job.sh`) to wait until all grid jobs finish.

```
~kenichi/bin/hadddraw -o hist_raw data/main/run_*/out/output_*.root
```


## Tracklet Analysis

Then the following commands analyze the tracklets reconstructed by `run_reco.sh`.
Plots will appear in `calib/1/`.

```
cd work
./make_rt.sh
```


## To-Do List

1. n/a


## Reference

Station ID:  https://github.com/E1039-Collaboration/e1039-wiki/wiki/event-reconstruction


## Author

Kenichi Naknao <knakano0524@gmail.com>
