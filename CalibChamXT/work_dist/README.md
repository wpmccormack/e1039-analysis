# work_dist

This directory is to extract the X-T curve using the time-distribution method, without the track reconstruction.
The outcome is typically used as the initial X-T curve in the calibration.


## Information on Analyzed Data

By default the macros in this directory uses run 3824 for D0.
https://e906-gat1.fnal.gov/data-summary/e1039/run.php?run=3824

https://e906-gat1.fnal.gov/data-summary/e1039/onlmon-plot-run.php?run=3824#Cham

From 2022-04-15 to 2022-04-21

HV:
* D0Vp    : 1.485 kV
* Other D0: 1.550 kV
* D2U     : 1.880 kV
* Other D2: 1.900 kV
* D3p     : 2.430 & 1.400 kV
* D3m     : 2.480 & 1.420 kV

https://e906-gat1.fnal.gov/hvmon_cham_exp/current/?Y0=2022&M0=04&D0=14&h0=0&Y1=2022&M1=4&D1=22&h1=0

ASDQ threshold:
* D0XXp: 1250
* D0UUp: 1100
* D0VVp: 1400


## Usage

```
root -b 'Fun4All.C(3824, "/data2/e1039/dst/run_003824_spin.root")'
root -b ExtractXTCurve.C
```

The 1st step takes 17 minutes when all events are analyzed.
