# e1039-analysis/AnaSimDst/work in Branch `jpsi_tssa_sim`

This code is a special version of `AnaSimDst` to evaluate
the anticipated statistics of TSSA of J/psi and psi'.

## Event generation

Use `e1039-analysis/SimChainDev` to generate two sets of events;
 - `stat_jpsi`:  N of generated events = 20k/job * 796 jobs
 - `stat_psip`:  N of generated events = 20k/job * 496 jobs
 - `stat_dy`  :  N of generated events = 20k/job * 697 jobs

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

```
root -b draw_stat_exp.cc
```