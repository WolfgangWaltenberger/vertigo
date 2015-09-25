#!/bin/sh

# This file creates a list of all rave headers
# needed by vertigo.
# To accomlish this, it used get_direct_headers.sh and
# add_indirect_headers.sh.

./get_direct_headers.sh

N=`cat headers.txt | wc -l`
echo $N

M=`echo $N + 1 | bc `

while true; do
  ./add_indirect_headers.sh
  M=`cat headers.txt | wc -l`
  echo $M
  [ $M -eq $N ] && exit 0;
  N=$M
done

cat headers.txt
