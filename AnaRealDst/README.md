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
1. cmake-this
1. make-this
1. cd macro
1. root -b -q 'Fun4RealDst.C(202)'
1. display h1_ele_H1T.png &

Here are some tips:

- You need execute "source setup.sh" every time you open a new terminal.
- You need execute "make-this" every time you modify any file in "src/".
- You need execute "cmake-this" every time you add/delete any file in "src/".
- You can execute the macro at any directory.

## Advanced Usage

- The contents of the analysis are defined in class "AnaRealDst".
  You can modify this class (i.e. "src/AnaRealDst.cc") to implement your own analysis.
    - The E1039 data are structured by the SQ interface classes.  To access them, you use several member functions of these classes.  You can refer to the [oline document](https://e1039-collaboration.github.io/e1039-doc/annotated.html).  Below are frequently-used classes;
        - [SQRun](https://e1039-collaboration.github.io/e1039-doc/d7/db7/classSQRun.html)
        - [SQEvent](https://e1039-collaboration.github.io/e1039-doc/d9/dd7/classSQEvent.html)
        - [SQHitVector](https://e1039-collaboration.github.io/e1039-doc/d9/dbc/classSQHitVector.html)
        - [SQHit](https://e1039-collaboration.github.io/e1039-doc/de/d79/classSQHit.html)
        - [GeomSvc](https://e1039-collaboration.github.io/e1039-doc/d0/da0/classGeomSvc.html)
- The run ID and the number of events for analysis are defined in "macro/Fun4RealDst.C".
  You can change them so that you need not give them in the command line.

## Author

Kenichi Naknao <knakano@nucl.phys.titech.ac.jp>
