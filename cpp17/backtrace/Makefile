.PHONY:all clean
all:
	@mkdir -p build
	@g++ --std=c++17 -g src/backtrace.cc test/demo.cc -Iinclude -fsanitize=address -o build/demo && ./build/demo

clean:
	@rm -rf build/*