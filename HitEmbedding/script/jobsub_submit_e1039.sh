#!/bin/bash
# Wrapper script of jobsub_submit for the E1039 enviornment.

if ! which jobsub_submit &>/dev/null ; then
    echo "Command 'jobsub_submit' not found."
    echo "Forget 'source /e906/app/software/script/setup-jobsub-spinquest.sh'?"
    exit
fi

jobsub_submit \
    --grid \
    -l '+SingularityImage=\"/cvmfs/singularity.opensciencegrid.org/e1039/e1039-sl7:latest\"' \
    --append_condor_requirements='(TARGET.HAS_SINGULARITY=?=true)' \
    --use_gftp \
    --resource-provides=usage_model=DEDICATED,OPPORTUNISTIC,OFFSITE \
    -e IFDHC_VERSION \
    --expected-lifetime='medium' \
    --mail_never \
    $*

# Note:
#   lifetime = short (3h), medium (8h) or long (23h)

