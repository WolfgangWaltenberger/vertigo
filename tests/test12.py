#!/bin/sh

echo "Test RaveLogger"

rm -f rave_log.txt

subtest12.py

echo "rave_log.txt contains"
grep -v loaded rave_log.txt
