IDIR=../include
CXX=g++
CXXFLAGS=-I$(IDIR)

ODIR=obj
LDIR=../lib

LIBS=-lm

#_DEPS = 
#DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = easyFTT.o Open.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(OBJ): $(ODIR)/%.o: src/%.c $(DEPS)
	$(CXX) -c -o $@ $< $(CXXFLAGS) $(pkg-config sndfile --cflags --libs)

SoundiX: $(OBJ)
	g++ -o $@ $^ $(CXXFLAGS) $(LIBS) $(pkg-config sndfile --cflags --libs)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 
