DIR_TOP=$(dirname $(readlink -f $BASH_SOURCE))

source /e906/app/software/osg/software/e1039/this-e1039.sh
export LD_LIBRARY_PATH=$DIR_TOP/inst/lib:$LD_LIBRARY_PATH

function cmake-ana-sim-dst {
    cmake -DCMAKE_INSTALL_PREFIX=$DIR_TOP/inst $DIR_TOP/src
    ret=$?
    test $ret -eq 0 && echo "OK, run 'make install'."
}

if [ ${HOSTNAME:0:13} != 'spinquestgpvm' ] ; then
    echo "!!CAUTION!!"
    echo "This AnaSimDst package does not support your computer ($HOSTNAME)."
    echo "It might not run properly even if you follow 'README.md'."
    echo
fi
cat <<EOF 
OK, your shell environment has been set up to use the software prepared in 
'$E1039_ROOT'.
Most likely you will next execute the following commands;
  cd build
  cmake-ana-sim-dst
If you encounter any error in using AnaRealDst, please ask the author for help.
EOF
