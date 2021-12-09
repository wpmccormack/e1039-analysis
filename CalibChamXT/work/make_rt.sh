#!/bin/bash
## A temporary script to execute 'MakeRT.C'.
## You need wait for all jobs (submitted by 'run_reco.sh') finished
## before executing this script.
DIR_BASE=$(dirname $(readlink -f $BASH_SOURCE))

ITER=1
LIST_RUN="list_run.txt"
VERSION="default"
DIR_DATA=/pnfs/e1039/scratch/$USER/CalibChamXT/$VERSION

LIST_DATA=scratch/list_input_data.txt
mkdir -p scratch

while read RUN ; do
    RUN6=$(printf '%06d' $RUN)
    find $DIR_DATA/run_$RUN6_* -name eval.root
done <$LIST_RUN >$LIST_DATA

root.exe -b -q "MakeRT.C($ITER, \"$LIST_DATA\")"
