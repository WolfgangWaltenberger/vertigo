#!/bin/sh

vertigo -oEventPrinter-short -sgun:simplegauss -n0 -mkalman -v0 | grep -v Rave.Event | sed -e 's,#[0123456789]* ,,'
vertigo -oEventPrinter-short -sgun:simplegauss -n0 -mgsf -v0 | grep -v Rave.Event |  sed -e 's,#[0123456789]* ,,'

