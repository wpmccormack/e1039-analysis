#!/bin/bash

nevents=$1
run_num=$2

if [ -z ${CONDOR_DIR_INPUT+x} ];
  then
    CONDOR_DIR_INPUT=./input;
    echo "CONDOR_DIR_INPUT is initiallized as $CONDOR_DIR_INPUT"
  else
    echo "CONDOR_DIR_INPUT is set to '$CONDOR_DIR_INPUT'";
fi

if [ -z ${CONDOR_DIR_OUTPUT+x} ];
  then
    CONDOR_DIR_OUTPUT=./out;
    mkdir -p $CONDOR_DIR_OUTPUT
    echo "CONDOR_DIR_OUTPUT is initiallized as $CONDOR_DIR_OUTPUT"
  else
    echo "CONDOR_DIR_OUTPUT is set to '$CONDOR_DIR_OUTPUT'";
fi

echo "hello, grid." | tee out.txt $CONDOR_DIR_OUTPUT/out.txt
pwd | tee -a out.txt $CONDOR_DIR_OUTPUT/out.txt

tar -xzf $CONDOR_DIR_INPUT/input.tar.gz
ln -s $CONDOR_DIR_INPUT/*.dat .
ls -lh | tee -a out.txt $CONDOR_DIR_OUTPUT/out.txt

source /cvmfs/seaquest.opensciencegrid.org/seaquest/users/yuhw/e1039/setup.sh
#source /e906/app/software/osg/users/yuhw/e1039/setup.sh
#export LD_LIBRARY_PATH=/cvmfs/seaquest.opensciencegrid.org/seaquest/users/yuhw/install/lib:$LD_LIBRARY_PATH
echo `which root`

ldd /cvmfs/seaquest.opensciencegrid.org/seaquest/users/yuhw/e1039/offline_main/lib/libktracker.so
ldd /cvmfs/seaquest.opensciencegrid.org/seaquest/users/yuhw/e1039/offline_main/lib/libg4detectors.so
echo "LD_LIBRARY_PATH=$LD_LIBRARY_PATH"

time root -b -q Fun4CODA.C\($nevents,$run_num\)

mv *.root $CONDOR_DIR_OUTPUT/

echo "gridrun.sh finished!"
