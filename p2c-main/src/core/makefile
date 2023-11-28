BIN_PATH  := ../../bin
CORE_PATH := ./

all: p2c

p2c: p2c.o
	g++ -rdynamic $< -o $(BIN_PATH)/$@ -ldl
	rm $<
.cpp.o:
	g++ -I $(CORE_PATH) -c $<

clean:
	- rm *.o $(BIN_PATH)/p2c