CORE_PATH := ../core
LIB_PATH  := ../../lib
SRC_FILES := $(wildcard *.cpp)
OBJ_FILES := $(patsubst %.cpp,%.o,$(SRC_FILES))
DLL_FILES := $(patsubst %.o,%.so,$(OBJ_FILES))

all: $(DLL_FILES)
%.so: %.o
	g++ -shared -Wl,-soname,$@ -o $(LIB_PATH)/$@ $<
.cpp.o:
	g++ -I $(CORE_PATH) -fPIC -c $<

clean:
	- rm $(LIB_PATH)/*.so