# e1039-analysis/ChamRealizationDev

A compact program to develop the chamber realization, which adds inefficiencies and position resolutions to chamber and prop tube hits.

## Preparation

The version of e1039-core is selected in `setup.sh`.
If you need modify e1039-core, you build e1039-core and select it in `setup.sh`.


## Source Build

The source code is located in `src/`.
The following commands set up the shell environment and compile the code;

```
source setup.sh
cmake-this
make-this
```

`AnaTrack` is a SubsysReco module to analyze reconstructed tracks.
The track reconstruction and the analysis are done before and after the chamber realization in order to evalaute the effect of the chamber realization.


## Event Generation

```
cd macro
root -b 'Fun4AllStep1.C(10000)'
```

This step includes the track reconstruction and the analysis.
The chamber realization is carried out in the next step.


## Chamber Realization

```
root -b Fun4AllStep2.C
```

This step includes the track reconstruction and the analysis.


## Contact

Kenichi Naknao <knakano0524@gmail.com>
