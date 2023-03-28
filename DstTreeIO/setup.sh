LIST_FILE=(
    /data2/e1039/this-e1039.sh 
    /e906/app/software/osg/software/e1039/this-e1039.sh 
    /project/ptgroup/spinquest/this-e1039.sh 
)

for FILE in ${LIST_FILE[*]} ; do
    if [ -e $FILE ] ; then
    	echo "Source $FILE."
    	source $FILE
    	break
    fi
done

if [ -z $E1039_ROOT ] ; then
    echo "Cannot find any E1039 setup script.  You could set it up by yourself."
fi
