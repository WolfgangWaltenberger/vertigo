all: test

test:
	./test.sh

defaults:
	./defaults.sh 

LDFLAGS=`pkg-config rave --libs`
CPPFLAGS=`pkg-config rave --cflags`
CPPFLAGS+=-I..
LDFLAGS+=-L/home/walten/install/lib/ -lVertigo

propagatortest: propagatorTest.o
	$(CXX) $(LDFLAGS) -fPIC propagatorTest.o -o propagatorTest
