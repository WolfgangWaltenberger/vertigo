#!/bin/sh

SOURCE=gun:simple_d
SOURCE=list:lcio.txt

CMD="vertigo -v1 -s${SOURCE} -mnone -oTrackParameterHistogrammer-lcio.root -cVertices:Association=Distance -n-5000"

echo $CMD

# root draw.C
