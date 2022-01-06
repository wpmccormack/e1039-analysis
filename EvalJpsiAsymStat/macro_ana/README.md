# e1039-analysis/EvalJpsiAsymStat/work

The simulated DSTs can be processed by `AnaSimDst`, as explained in `AnaSimDst/README.md`.
The conditions and qualities of simulated DSTs should be checked by looking into the outputs of `Fun4SimTree.C`.

This branch includes another ROOT macro, `asymmetry.C`.
It reads the tree (like `Fun4SimTree.C`) to compute J/psi TSSA;
```
root -b -q asymmetry.C
```


## Processing `jpsi` event set

```
mkdir  jpsi
cd jpsi
../make-dst-list.sh ../../../SimChainDev/data/stat_jpsi_20210723_0*
root -b ../Fun4SimDst.C
root -b ../Fun4SimMicroDst.C
root -b '../Fun4SimTree.C("jpsi")'
```


## Processing `psip` event set

```
mkdir  psip
cd psip
../make-dst-list.sh ../../../SimChainDev/data/stat_psip_20210723_0*
root -b ../Fun4SimDst.C
root -b ../Fun4SimMicroDst.C
root -b '../Fun4SimTree.C("psip")'
```


## Processing `dy` event set

```
mkdir  dy
cd dy
../make-dst-list.sh ../../../SimChainDev/data/stat_dy_20210723_0*
root -b ../Fun4SimDst.C
root -b ../Fun4SimMicroDst.C
root -b '../Fun4SimTree.C("dy")'
```


## Drawing the anticipated statistics

`draw_stat_exp.cc` is to draw the expected accuracy of J/psi TSSA.
You first write the simulated accuracy (which is obtained by `asymmetry.C`) into this macro manually and then execute it;
```
root -b -q draw_stat_exp.cc
```
