.PHONY:all

STATIC_CXX=-static-libstdc++
# DYNAMIC_CXX=-lstdc++
# ASAN=-fsanitize=address
all:
	clang -g xx.c -fPIC -shared -o xx.so
	clang++ -g test.cpp -ldl -pthread ${STATIC_CXX} ${ASAN} ${DYNAMIC_CXX}
