### Makefile --- 

## Author:   jpms
## Keywords: 
## Copyright (c) 2015, Joao Marques-Silva

CC	= gcc
CXX	= g++

#CXXFLAGS = -arch x86_64 -DTRACEVERB=0 -O3 -DNDEBUG -DNCHECK -Wall -Wno-deprecated -fno-strict-aliasing

CXXFLAGS = -DTRACEVERB=0 -O3 -DNDEBUG -DNCHECK -Wall -Wno-deprecated -fno-strict-aliasing

SRCS = $(wildcard *.cc)
HDRS = $(wildcard *.hh)
OBJS = $(addsuffix .o, $(basename $(SRCS)))

CXXFLAGS += -std=c++11 

all: chkcpf

chkcpf:	$(OBJS) chkcpf.o
	@echo $(LLIBS)
	$(CXX) $(CXXFLAGS) -o chkcpf $(OBJS)
	@strip $@

deps:	$(SRCS)
	@rm -f Makefile.dependencies
	@touch Makefile.dependencies
	for f in $(SRCS); do \
		$(CXX) $(CXXFLAGS) $(CPPFLAGS) -MM $$f >> Makefile.dependencies; \
	done

clean:
	@rm -f $(OBJS) chkcpf

%.o:	%.cc
	@echo Compiling: $<
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c -o $@ $<

-include Makefile.dependencies

### Makefile ends here
