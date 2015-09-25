#!/bin/sh

# This simple script takes headers.txt
# and adds all headers that are referenced from there.
# Output is a longer headers.txt.
# WARNING doesnt understand all comments, maybe try g++ -E ....

RAVEDIR=`pkg-config rave --cflags-only-I | sed -e 's/.*-I//' | sed -e 's/ *$//'`
NEWHEADERS=`cat headers.txt`
OLD=$NEWHEADERS
for i in $OLD; do
#echo "->$i<-"
	TMP=`cat $RAVEDIR/$i | grep \#include | grep interface | sed -e 's,//.*,,' | sed -e 's,/\*.*,,' | sed -e 's,rave/impl/,,' | sed -e 's,.include <,,' |sed -e 's,.include .,,' | sed -e 's/>$//' | sed -e 's/ *$//' | sed -e 's/	*$//' | sed -e 's/\.h"/\.h/' | sed -e 's///'`
	NEWHEADERS="$TMP $NEWHEADERS"
#echo "new: $NEWHEADERS"
done

TMP=`tempfile`

for i in $NEWHEADERS; do
  echo "$i" >> ${TMP}
done

cat ${TMP} | sort | uniq > headers.txt
