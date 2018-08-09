#!/bin/bash
#HOW TO USE THIS SCRIPT:
#RUN THE COMMAND AS BELOW:
#$bash remove_wspaces.sh /name/of/the/dir
#eg:
#$../../scripts/remove_wspaces.sh `pwd`/src/
RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m' # No Color
echo -e "removing trailing white spaces in .c ,.h,.cpp files in \n ${GREEN} $1${NC}"

#find $1 -type f -name '*.h' -exec sed --in-place 's/[[:space:]]\+$//' {} \+
#find all .h .c .cpp

#find $1 -type f \( -iname \*.c -o -iname \*.h -o -iname \*.cpp \) > /tmp/cpp_files_list.dat
#cat /tmp/cpp_files_list.dat >  sed --in-place 's/\r\n/\n/' 
find $1 -type f \( -iname \*.c -o -iname \*.h -o -iname \*.cpp \) -exec sed --in-place 's/[[:space:]]\+$//' {} \+

echo -e "replacing CRLF with LF at end of line in \n ${GREEN} $1${NC}"

find $1 -type f \( -iname \*.md -o -iname \*.mk -o -iname \*.c -o -iname \*.h -o -iname \*.cpp \) -exec sed --in-place 's/\r\n/\n/' {} \+

#sed --in-place 's/^M$//'
echo "Done"
