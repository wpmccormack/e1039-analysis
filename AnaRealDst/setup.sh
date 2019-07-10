DIR_TOP=$(dirname $(readlink -f $BASH_SOURCE))

source /data2/e1039/this-e1039.sh
export LD_LIBRARY_PATH=$DIR_TOP/inst/lib:$LD_LIBRARY_PATH

function cmake-e1039-ana {
    cmake -DCMAKE_INSTALL_PREFIX=$DIR_TOP/inst $DIR_TOP/src
    ret=$?
    test $ret -eq 0 && echo "OK, run 'make install'."
}

if [   ${HOSTNAME:0:13} != 'seaquestdaq01' \
    -a ${HOSTNAME:0:13} != 'e1039-monitor' \
    -a ${HOSTNAME:0:12} != 'spinquestana' \
    -a ${HOSTNAME:0:13} != 'spinquestgpvm' ] ; then
    echo "!!CAUTION!!"
    echo "This AnaRealDst package does not support your computer ($HOSTNAME)."
    echo "It might not run properly even if you follow 'README.md'."
    echo
fi
echo "OK, your shell environment has been set up to use the E1039 software prepared in '$E1039_ROOT'.  Most likely you will next execute 'cd work' and 'cmake-e1039-ana'.  If you encounter any error in using AnaRealDst, please ask the author for help."
