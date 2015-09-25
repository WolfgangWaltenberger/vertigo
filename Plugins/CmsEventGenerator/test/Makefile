all: install run

install:
	cd .. && make install

run: 
	vertigo -scms:bb200pre2.3ev.root -n0 -mavf -v99 -o none

trackerrs:
	vertigo -scms:bb200pre2.3ev.root -n0 -mavf -v99 -oEventPrinter -cEventPrinter:TrackErrors=1


compile:
	g++ `root-config --libs --cflags` bla.cc -o bla; ./bla
