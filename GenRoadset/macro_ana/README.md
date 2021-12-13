# Macro to generate and evaluate roadset

This directory is to first generate a new roadset, using the signal and BG events.
It then evaluates the new roadset, namely re-analyzes the signal and BG events to compute the signal acceptance and the BG rate.

You need execute `source setup.sh` beforehand.


## Lists of Input Files

It is used in `macro_ana/make-input-list.sh` by default.

```
./make-input-list.sh
```


## Generation of Roadset

* `macro_ana/GenerateRoad.C`
* `src/AnaGmc.(h|cc)`
* `src/AnaNim3.(h|cc)`
* `src/GenRoadset.(h|cc)`

```
root -b GenerateRoad.C
```


## Evaluation of Roadset

* `work/ApplyRoadset2Signal.C`
* `work/ApplyRoasetd2BG.C`
* `work/DrawRS.C`
* `src/ReAnaSignal.(h|cc)`
* `src/ReAnaBG.(h|cc)`
* `src/DrawRoadset.(h|cc)`

```
root -b ApplyRoadset2Signal.C
root -b ApplyRoadset2BG.C
root -b DrawRS.C
```
