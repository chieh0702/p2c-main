CORE_PATH := ../core
BIN_PATH  := ../../bin
SRC_FILES := $(wildcard *.cpp)
OBJ_FILES := $(patsubst %.cpp,%.o,$(SRC_FILES))
OUT_FILES := $(patsubst %.o,%.out,$(OBJ_FILES))
BIN_FILES := $(patsubst %.o,%,$(OBJ_FILES))
COLOR_GREEN := echo "\e[1;32m"
COLOR_BLUE  := echo "\e[1;34m"
COLOR_RESET := echo "\e[0m"

all: argtable liblist 
%.out: %.o
	g++ -rdynamic -o $* $< -ldl
.cpp.o:
	g++ -I $(CORE_PATH) -c $<

argtable: p2c_test_argtable
	@ $(COLOR_GREEN)
	- ./$< -arg1 hello --arg2 ppo oop
	@ $(COLOR_RESET)
	@ make clean
p2c_test_argtable: p2c_test_argtable.o
	g++ -rdynamic -o $@ $< -ldl

liblist: p2c_test_liblist
	@ $(COLOR_GREEN)
	- ./$<
	@ $(COLOR_RESET)
	@ make clean
p2c_test_liblist: p2c_test_liblist.o
	g++ -rdynamic -o $@ $< -ldl

mod_normal: p2c_test_mod_normal
	@ $(COLOR_GREEN)
	- ./$< --website --dockerfile -f debian -c /bin:/bin -a /root/p2c:/root --env "PATH=/bin" -m "curl ifconfig.me >pubip.info" -r "apt update;apt install curl" -p 8080/TCP
	@ $(COLOR_RESET)
	@ make clean	
p2c_test_mod_normal: p2c_test_mod_normal.o
	g++ -rdynamic $< -o $@ -ldl
	rm $<

gen_dockerfile: p2c_test_gen_dockerfile
	@ $(COLOR_GREEN)
	- ./$<
	@ $(COLOR_BLUE)
	@ echo "\nDockerfile generated:\n"
	@ cat Dockerfile
	@ rm Dockerfile
	@ $(COLOR_RESET)
	@ make clean
p2c_test_gen_dockerfile: p2c_test_gen_dockerfile.o
	g++ -rdynamic $< -o $@ -ldl
	rm $<

mod_website: p2c_test_mod_website
	@ $(COLOR_GREEN)
	- ./$<
	@ $(COLOR_RESET)
	@ make clean
p2c_test_mod_website: p2c_test_mod_website.o
	g++ -rdynamic $< -o $@ -ldl
	rm $<

clean:
	-@ rm $(BIN_FILES) $(OBJ_FILES) 2>/dev/null