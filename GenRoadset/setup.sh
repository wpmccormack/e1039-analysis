DIR_TOP=$(dirname $(readlink -f $BASH_SOURCE))
DIR_BUILD=$DIR_TOP/build

source /e906/app/software/osg/software/e1039/this-e1039.sh
#source /data2/users/kenichi/e1039/git/knakano0524/core-inst/this-e1039.sh
export   LD_LIBRARY_PATH=$DIR_TOP/inst/lib:$LD_LIBRARY_PATH
export ROOT_INCLUDE_PATH=$DIR_TOP/inst/include:$ROOT_INCLUDE_PATH

function cmake-this {
    if [ -e $DIR_BUILD ] ; then
	echo "Clean up the build directory..."
	\rm -rf $DIR_BUILD
    fi
    if [ -e $DIR_TOP/inst ] ; then
	echo "Clean up the install directory..."
	\rm -rf $DIR_TOP/inst
    fi
    mkdir -p $DIR_BUILD
    echo "Run cmake..."
    ( cd $DIR_BUILD && cmake -DCMAKE_INSTALL_PREFIX=$DIR_TOP/inst $DIR_TOP/src )
    local RET=$?
    if [ $RET -eq 0 ] ; then
	echo "OK.  Run 'make-this' at any time/directory."
    else
	echo "NG.  Need a fix."
    fi
    return $RET
}

function make-this {
    if [ ! -e $DIR_BUILD/Makefile ] ; then
	echo "The build directory is not ready.  Complete 'cmake-this' first."
	return 1
    fi
    ( cd $DIR_BUILD && make install )
    return $?
}

if [ ${HOSTNAME:0:13} != 'spinquestgpvm' ] ; then
    echo "!!CAUTION!!"
    echo "This package does not support your computer ($HOSTNAME)."
    echo "You are recommended to use 'spinquestgpvm01'."
    echo
fi
