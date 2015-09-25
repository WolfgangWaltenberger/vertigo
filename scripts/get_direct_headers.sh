#!/bin/zsh

# A simple script that determines which rave header files vertigo needs
# _directly_. Output is written to headers.txt
# Warning: assumes that includes are of the type "#include <rave/impl/XXX.h>"
# WARNING doesnt understand all comments, maybe try g++ -E ....

# OUT=`cat ../**/*.h ../**/*.cc ../vertigo.i | grep rave.impl | grep \#include | sed -e 's,//.*,,' | sed -e 's,.*rave/impl/,,' | sort | uniq | sed -e 'sZ>$Z\x5C\x5CZ'`

OUT=`cat ../**/*.cpp ../**/*.h ../**/*.cc ../vertigo.i | grep rave.impl | sed -e 's,//.*,,' | sed -e 's,/\*.*,,' | grep \#include | sed -e 's,//.*,,' | sed -e 's,.*rave/impl/,,' | sed -e 's/ *//' | sed -e 's///' | sed -e 's/>$//' | sort | uniq | sed -e 'sZ>$ZZ'`

echo $OUT > headers.txt
