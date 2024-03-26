.PHONY:all clean
all:
	@mkdir -p build
	@g++ --std=c++11 -g test/test_threadpool.cc -Iinclude -fsanitize=address -o build/test_threadpool && ./build/test_threadpool

clean:
	@rm -rf build/*