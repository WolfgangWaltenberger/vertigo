#!/bin/sh

NUM=`ls test*.py | sort | sed -e 's/test//' | sed -e 's/.py$//' | wc -l`

echo "$NUM Tests"

for i in `seq $NUM`; do
	./test${i}.py > output${i}.txt
done

