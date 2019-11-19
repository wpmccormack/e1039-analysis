# e1039-analysis/AnaSimDst

A compact program to analyze the simulated DST file, which is created by the `SimChainDev` package.
Since it is yet under development and also depends on other software under development,
it might stop working occasionally.  In such cases, please contact the author.

## Basic Usage

### Preparation of Simulated DST Files

You first produce a set of simulated DST files by `SimChainDev`.
This document assumes that the top directory of your simDST files is `/your/dst/dir`.

The following command lists up all simDSTs in `list_dst.txt`;
```
cd e1039-analysis/AnaSimDst/work
./make-dst-list.sh /your/dst/dir
```
You might manually create or edit the list file as you need.

### Compilation of Analysis Code

The analysis code is located in `e1039-analysis/AnaSimDst/src`.
The following commands set up the shell environment and compile the code;
```
cd e1039-analysis/AnaSimDst
source setup.sh
cd build
cmake-ana-sim-dst
make install
```
You are expected to later look into the script and the analysis code to make your own analysis.

### Execution of Analysis

The 1st step is to extract a set of variables from simDSTs to TTree.
The following commands reads five simDSTs for test.
The TTree object is outputted to `output.root`.
```
cd e1039-analysis/AnaSimDst
source setup.sh
cd work
root -b -q 'Fun4SimDst.C(5)'
```

The 2nd step is to read TTree to draw plots.
```
root -b -q AnaSimTree.C
```
Various plots are outputted to `result/`.
If all ran fine, you delete `5` (or change to `0`) in the 1st step to use all simDSTs.

## Author

Kenichi Naknao <knakano@nucl.phys.titech.ac.jp>
