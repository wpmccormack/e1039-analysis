DIR_TOP=$(dirname $(readlink -f $BASH_SOURCE))
DIR_BUILD=$DIR_TOP/build
DIR_INST=$DIR_TOP/inst

source /data2/e1039/this-e1039.sh
#source /e906/app/software/osg/software/e1039/this-e1039.sh
export   LD_LIBRARY_PATH=$DIR_INST/lib:$LD_LIBRARY_PATH
export ROOT_INCLUDE_PATH=$DIR_INST/include:$ROOT_INCLUDE_PATH

function cmake-this {
    if [ -e $DIR_BUILD ] ; then
	echo "Clean up the build directory..."
	\rm -rf $DIR_BUILD
    fi
    if [ -e $DIR_INST ] ; then
	echo "Clean up the install directory..."
	\rm -rf $DIR_INST
    fi
    mkdir -p $DIR_BUILD
    echo "Run cmake..."
    ( cd $DIR_BUILD && cmake -DCMAKE_INSTALL_PREFIX=$DIR_INST $DIR_TOP/src )
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

##
## Main
##
if [   ${HOSTNAME:0:13} != 'seaquestdaq01' \
    -a ${HOSTNAME:0:9}  != 'e1039gat1' \
    -a ${HOSTNAME:0:13} != 'e1039-monitor' \
    -a ${HOSTNAME:0:12} != 'spinquestana' \
    -a ${HOSTNAME:0:13} != 'spinquestgpvm' ] ; then
    echo "!!CAUTION!!"
    echo "This AnaRealDst package does not support your computer ($HOSTNAME)."
    echo "It might not run properly even if you follow 'README.md'."
    echo
fi
echo "Your shell environment has been set up to use the E1039 software under '$E1039_ROOT'."
echo
echo "Two commands for source build:"
echo "  cmake-this"
echo "  make-this"
echo
echo "These commands can be executed in any directory."
echo "You need execute 'make-this'  when you modify source files."
echo "You need execute 'cmake-this' when you add/delete source files."
