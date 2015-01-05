.PHONY: clean 

LDFLAGS+= # link to math library

TARGET=bitslice

all: $(TARGET)

o3: CPPFLAGS+=-o3
o3: all

debug: CPPFLAGS+=-g
debug: all

# type make/make all to compile test_hmm
run: all

clean:
	$(RM) $(TARGET)   # type make clean to remove the compiled file
