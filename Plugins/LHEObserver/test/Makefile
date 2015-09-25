all: single

single:
	vertigo -v0 -s lhe:my.lhe -n0 -oLHEObserver-tree -mnone

singles:
	vertigo -v0 -s lhe:my.lhe -n-9 -oLHEObserver-tree -mnone

masses:
	vertigo -v1 -s lhe:T2tt_825.0_50.0To825.0_525.0.lhe -oLHEObserver-masses-nopids-file:T2tt.root -mnone -n-99999
T3w:
	vertigo -v1 -s lhe:T3w_0.5_1000.0_175.0to0.5_1025.0_250.0.lhe -oLHEObserver-masses-pids-sms-file:T3w.root -mnone -n-999999999
	dataharvester-merge -v -i T3w.root

T3w075:
	vertigo -v1 -s lhe:T3w_0.75_225.0_75.0To275.0_175.0.lhe -oLHEObserver-masses-sms-pids-file:T3w075a.root -mnone -n-26000000
	dataharvester-merge -v -i T3w075a.root
	vertigo -v1 -s lhe:T3w_0.75_375.0_50.0To400.0_175.0.lhe -oLHEObserver-masses-sms-pids-file:T3w075b.root -mnone -n-26000000
	dataharvester-merge -v -i T3w075b.root

shuttle:
	scp T3w*root shuttle.shacknet.nu:svn/vertigo/Plugins/LHEObserver/test/

run:
	vertigo -v1 -s lhe:my.lhe -o LHEObserver -mnone -n-9999
