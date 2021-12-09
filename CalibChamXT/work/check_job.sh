#!/bin/bash
## A temporary script to check the status of jobs submitted.
DIR_BASE=$(dirname $(readlink -f $BASH_SOURCE))

LIST_RUN="list_run.txt"
VERSION="main"
DIR_DATA=/pnfs/e1039/scratch/$USER/CalibChamXT/$VERSION

if [ ! -e $DIR_DATA/input.tar.gz ] ; then
    echo "Cannot find '$DIR_DATA/input.tar.gz'.  Not yet submit jobs?  Abort."
    exit
fi

LIST_JOB_ID=scratch/list_job_id.txt
echo "LIST_JOB_ID = $LIST_JOB_ID"
if [ ! -e $LIST_JOB_ID -o $LIST_JOB_ID -ot $DIR_DATA/input.tar.gz ] ; then
    echo "Updating the job-ID list..."
    mkdir -p scratch
    while read RUN ; do
	RUN6=$(printf '%06d' $RUN)
	grep '^Use job id ' $DIR_DATA/run_$RUN6_*/log_jobsub_submit.txt | awk '{print $4}'
    done <$LIST_RUN >$LIST_JOB_ID
    echo "...done."
fi

N_JOB_ALL=$(cat $LIST_JOB_ID | wc -l)
echo "N_JOB_ALL  = $N_JOB_ALL"

N_JOB_IDLE=0
N_JOB_RUN=0
N_JOB_HELD=0
while read ID OWNER DATE TIME RUN ST PRI SIZE CMD ; do
    case $ST in
	I ) (( N_JOB_IDEL++ )) ;;
	R ) (( N_JOB_RUN++ )) ;;
	H ) (( N_JOB_HELD++ )) ;;
    esac
done < <(jobsub_q --user=$USER | grep -f $LIST_JOB_ID)
echo "N_JOB_IDLE = $N_JOB_IDLE"
echo "N_JOB_RUN  = $N_JOB_RUN"
echo "N_JOB_HELD = $N_JOB_HELD"
