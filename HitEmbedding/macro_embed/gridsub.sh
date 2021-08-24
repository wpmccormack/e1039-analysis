#!/bin/bash
DIR_MACRO=$(dirname $(readlink -f $BASH_SOURCE))

FN_LIST_SIG=list_sig_file.txt
FN_LIST_EMB=list_emb_file.txt
test ! -f $FN_LIST_SIG && echo "Cannot find '$FN_LIST_SIG'.  Abort." && exit
test ! -f $FN_LIST_EMB && echo "Cannot find '$FN_LIST_EMB'.  Abort." && exit

DO_SUB=0
N_JOB=0 # 0 = All available signal and/or embedding files
N_EVT=0 # 0 = All events in each signal+embedding file
OPTIND=1
while getopts ":gj:e:" OPT ; do
    case $OPT in
        g ) DO_SUB=1
            ;;
        j ) N_JOB=$OPTARG
            ;;
        e ) N_EVT=$OPTARG
            ;;
    esac
done
shift $((OPTIND - 1))
JOB_NAME=$1
test -z $JOB_NAME  && echo "Need a job name.  Abort."        && exit

echo "JOB_NAME = $JOB_NAME"
echo "DO_SUB   = $DO_SUB"
echo "N_JOB    = $N_JOB"
echo "N_EVT    = $N_EVT"

##
## Check and decide N of files/jobs to be processed
##
LIST_SIG=( $(cat $FN_LIST_SIG) )
LIST_EMB=( $(cat $FN_LIST_EMB) )
N_SIG=${#LIST_SIG[*]}
N_EMB=${#LIST_EMB[*]}
N_BOTH=$(( N_SIG < N_EMB ? N_SIG : N_EMB ))
test $N_JOB -eq 0 -o $N_JOB -gt $N_BOTH  &&  N_JOB=$N_BOTH
echo "N of signal    data files = $N_SIG"
echo "N of embedding data files = $N_EMB"
echo "N of files/jobs to be processed = $N_JOB"

##
## Prepare and execute the job submission
##
if [ $DO_SUB == 1 ]; then
    echo "Grid mode."
    DIR_DATA=/pnfs/e1039/scratch/$USER/HitEmbedding/data_embedded
    DIR_WORK=$DIR_DATA/$JOB_NAME
    ln -nfs $DIR_DATA data # for convenience
else
    echo "Local mode."
    DIR_WORK=$DIR_MACRO/scratch/$JOB_NAME
fi

rm -rf         $DIR_WORK
mkdir -p       $DIR_WORK
chmod -R 01755 $DIR_WORK

cd $DIR_MACRO
tar czf $DIR_WORK/input.tar.gz  *.C  ../inst

for (( I_JOB = 0; I_JOB < $N_JOB; I_JOB++ )) ; do
    FN_SIG=${LIST_SIG[$I_JOB]}
    FN_EMB=${LIST_EMB[$I_JOB]}
    mkdir -p       $DIR_WORK/$I_JOB/out
    chmod -R 01755 $DIR_WORK/$I_JOB
    cp -p $DIR_MACRO/gridrun.sh $DIR_WORK/$I_JOB

    if [ $DO_SUB == 1 ]; then
	CMD="$DIR_MACRO/../script/jobsub_submit_e1039.sh"
	CMD+=" -L $DIR_WORK/$I_JOB/log_gridrun.txt"
	CMD+=" -f $DIR_WORK/input.tar.gz"
	CMD+=" -f $FN_SIG"
	CMD+=" -f $FN_EMB"
	CMD+=" -d OUTPUT $DIR_WORK/$I_JOB/out"
	CMD+=" file://$DIR_WORK/$I_JOB/gridrun.sh $N_EVT"
	echo "$CMD"
	$CMD |& tee log_gridsub.txt
    else
	export  CONDOR_DIR_INPUT=$DIR_WORK/$I_JOB/in
	export CONDOR_DIR_OUTPUT=$DIR_WORK/$I_JOB/out
	mkdir -p $DIR_WORK/$I_JOB/in
	cp -p $DIR_WORK/input.tar.gz $DIR_WORK/$I_JOB/in
	ln    -s  $FN_SIG            $DIR_WORK/$I_JOB/in
	ln    -s  $FN_EMB            $DIR_WORK/$I_JOB/in
	cd $DIR_WORK/$I_JOB
	$DIR_WORK/$I_JOB/gridrun.sh $N_EVT |& tee $DIR_WORK/$I_JOB/log_gridrun.txt
    fi
    test $I_JOB -ge $N_JOB && break
done <$FN_LIST_EMB
