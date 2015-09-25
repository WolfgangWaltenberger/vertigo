#!/bin/sh

[ -z "$1" ] && { echo "Usage: $0 <test_number>"; exit 0; }

./test$1.py | tee output$1.txt
