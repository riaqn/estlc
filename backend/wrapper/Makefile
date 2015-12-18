main.o : main.ll
	llc -O0 -filetype=obj $^
main.ll : main.c
	clang -O0 -g -S -emit-llvm $^
