# e1039-analysis/AnaRealDst

A compact program to analyze the "DST" file, which is created by the Main DAQ decoder.
Since it is yet under development and also depends on other software under development,
it might stop working occasionally.  In such cases, please contact the author.

## Basic Usage

The procedure below will read the DST file of run 202 (for example), which is
```
/data2/e1039/dst/run_000202_spin.root
```
and produce basic histograms and ntuple.

1. source setup.sh
1. cd work
1. cmake-e1039-ana
1. make install
1. root -b '../macro/Fun4RealDst.C(202)'

## Advanced Usage

- The contents of the analysis are defined in class "AnaRealDst".
  You can modify this class (i.e. "src/AnaRealDst.cc") to implement your own analysis.
- The run ID and the number of events for analysis are defined in "macro/Fun4ReadDst.C".
  You can change them.

## Tips

- You need execute "source setup.sh" every time you open a new terminal.
- You need execute "make install" every time you modify any file in "src/".
- You can execute the macro at any directory.

## Author

Kenichi Naknao <knakano@nucl.phys.titech.ac.jp>
