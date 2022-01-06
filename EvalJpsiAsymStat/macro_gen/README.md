# Macro to generate signal events

This directory is to generate signal events to be used for the roadset tuning.
It outputs a small ROOT file named `signal_tree.root`, which will be used later in `macro_ana`.

Use `e1039-analysis/SimChainDev` to generate two sets of events;
 - `stat_jpsi`:  N of generated events = 20k/job * 796 jobs
 - `stat_psip`:  N of generated events = 20k/job * 496 jobs
 - `stat_dy`  :  N of generated events = 20k/job * 697 jobs

Or one J/psi job takes 2(?) hours for 500 accepted events.


## Setting of Event Generation

The setting of the event generation is configured in `Fun4All.C` as usual.
The SubsysReco module that creates `signal_tree.root` is `src/SubsysRecoSignal.(h|cc)`.
You can modify them as you need.


## Test Production

You first execute the following command to execute the macro on local for test,
which runs one job that generates 10 in-acceptance events;

```
./gridsub.sh
```


## Mass Production

You then execute the following commands to submit grid jobs (`-g`),
which runs 100 jobs (`-j 100`) that generate 500 events/job (`-e 500`);

```
source /e906/app/software/script/setup-jobsub-spinquest.sh
kinit
./gridsub.sh -g -j 100 -e 500
```

You might submit more jobs when the statistics are not enough;

```
./gridsub.sh -g -j 101-200 -e 500
```
