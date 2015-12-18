main.o : main.ll
	llc $^
main.ll : main.c
	clang -O0 -g -S -emit-llvm $^
