

export CPP := clang++
export AR  := ar
export CP  := cp -rf
export RM  := rm -rf
export MKDIR  := mkdir -p
export RANLIB := ranlib
export MAKE := make

CPPFLAGS = -Wall -fPIC -std=c++11 

# .SUFFIXES: .out .c .cc .o .h 
# .cc.o: 
# 	$(CPP) $(CPPFLAGS) -c $< -o $@  $(INCLUDE)
