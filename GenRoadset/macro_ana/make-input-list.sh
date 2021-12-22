#!/bin/bash
## Script to make lists of input signal files and input BG files.

echo "##"
echo "## Signal List"
echo "##"
DIR_IN_SIG=../macro_gen_signal/data/main
FN_IN_SIG=signal_tree.root
FN_OUT_SIG=list_signal.txt
echo "DIR_IN_SIG = $DIR_IN_SIG"
echo "FN_IN_SIG  = $FN_IN_SIG"
echo "FN_OUT_SIG = $FN_OUT_SIG"
find $DIR_IN_SIG -name $FN_IN_SIG | sort >$FN_OUT_SIG
echo "N of inputs found = $(cat $FN_OUT_SIG | wc -l)"
echo

echo "##"
echo "## BG List"
echo "##"
DIR_IN_BG=/pnfs/e1039/persistent/users/kenichi/GenRoadsetCommon # E906 NIM3
#DIR_IN_BG=../macro_full_bg/data/main # Full BG
FN_IN_BG=bg_data.root
FN_OUT_BG=list_bg.txt
echo "DIR_IN_BG = $DIR_IN_BG"
echo "FN_IN_BG  = $FN_IN_BG"
echo "FN_OUT_BG = $FN_OUT_BG"
find $DIR_IN_BG -name $FN_IN_BG | sort >$FN_OUT_BG
echo "N of inputs found = $(cat $FN_OUT_BG | wc -l)"
