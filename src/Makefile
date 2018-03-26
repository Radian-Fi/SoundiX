CXX=g++
INCLUDES = -I.. -I../include -I/usr/local/include/tcl8.4 -I/usr/local/include/tk8.4 -I/usr/X11R6/include -I/usr/local/include/boost_1_33_0
OPTIONS = -Wall -Wno-long-long -pedantic

ODIR=obj
LIBDIRS = -L/usr/local/lib -L../lib

LIBS= -lsndfile -ltcl84 -ltk84

ALL_OPTIONS = ${OPTIONS} ${INCLUDES} ${LIBDIRS} ${LIBS}

_DEPS = lsndx.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = cpptk.o cpptkbase.o Open2.o freq2midi.o GUI.o FFT2.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(OBJ): $(ODIR)/%.o: %.cpp $(DEPS)
	$(CXX) -std=c++11 -c -o $@ $< ${OPTIONS} ${INCLUDES}

all: SoundiX

SoundiX: $(OBJ)
	g++ -std=c++11 -o $@ $^ $(ALL_OPTIONS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 
