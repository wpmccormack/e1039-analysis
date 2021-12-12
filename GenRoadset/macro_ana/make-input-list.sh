#!/bin/bash

echo "##"
echo "## Signal List"
echo "##"
DIR_IN=../macro_gen_signal/data/main
FN_IN=signal_tree.root
FN_OUT=list_signal.txt
echo "DIR_IN = $DIR_IN"
echo "FN_IN  = $FN_IN"
echo "FN_OUT = $FN_OUT"
find $DIR_IN -name $FN_IN | sort >$FN_OUT
echo "N of inputs found = $(cat $FN_OUT | wc -l)"
echo

echo "##"
echo "## BG List"
echo "##"
DIR_IN=/pnfs/e1039/persistent/users/kenichi/GenRoadsetCommon
FN_IN=bg_data.root
FN_OUT=list_bg.txt
echo "DIR_IN = $DIR_IN"
echo "FN_IN  = $FN_IN"
echo "FN_OUT = $FN_OUT"
find $DIR_IN -name $FN_IN | sort >$FN_OUT
echo "N of inputs found = $(cat $FN_OUT | wc -l)"
