#!/bin/sh

vertigo -s input16.txt -mavr -n0 -oEventPrinter-short -v99 | grep -v Rave.Event
