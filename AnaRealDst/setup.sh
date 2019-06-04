DIR_TOP=$(dirname $(readlink -f $BASH_SOURCE))

if [ $HOSTNAME = 'seaquestdaq01.fnal.gov' ] ; then
    source /seaquest/users/kenichi/e1039/git/e1039-core/setup-e1039-core.sh
    export LD_LIBRARY_PATH=$DIR_TOP/inst/lib:$LD_LIBRARY_PATH
elif [ ${HOSTNAME:0:12} = 'seaquestgpvm' ] ; then
    echo "Not supported yet."
else
    echo "Not supported yet."
fi

function cmake-e1039-ana {
    cmake -DCMAKE_INSTALL_PREFIX=$DIR_TOP/inst $DIR_TOP/src
    ret=$?
    test $ret -eq 0 && echo "OK, run 'make install'."
}
