# e1039-analysis/GenRoadset

A program to generate a roadset (i.e. FPGA trigger logic) for E1039,
using the Drell-Yan simulation as signal and the full-BG simulation as BG.

This package was made from `knakano0524/e1039-trigger-tune`.


## Source Files

All source files are placed in `src/`.

```
source setup.sh
cmake-this
make-this
```


## Generation of Signal Events

* `macro_gen_signal/Fun4All.C`
* `src/SubsysRecoSignal.(h|cc)`


## Extraction of E906 NIM3 Events as Background Events

You usually need not carry out this step, since a full set of extracted events is already available.
It is used in `macro_ana/make-input-list.sh` by default.

* `macro_e906_nim3/Fun4All.C`
* `src/SubsysRecoNim3.(h|cc)`


## Generation of Full-BG Events

To be implemented.


## Generation and Evaluation of Roadset

* `macro_ana/`.


## To-Do List

* Take and use RF ID & Turn ID for NIM3 events.


## Contact

Kenichi Nakano <knakano0524@gmail.com>
