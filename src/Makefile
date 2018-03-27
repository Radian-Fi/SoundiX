CXX = g++
OPTIONS = -Wall -Wno-long-long -pedantic

ODIR = obj
IDIR = ../include

LIBS= -lsndfile

ALL_OPTIONS = -I$(IDIR) ${LIBS}

_DEPS = lsndx.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = Open2.o freq2midi.o FFT2.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(OBJ): $(ODIR)/%.o: %.cpp $(DEPS)
	$(CXX) -g -std=c++11 -c -o $@ $< -I$(IDIR)

all: SoundiX

SoundiX: $(OBJ)
	g++ -g -std=c++11 -o $@ $^ $(ALL_OPTIONS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 
