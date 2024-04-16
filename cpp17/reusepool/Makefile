.PHONY:all clean
all:
	@mkdir -p build
	@g++ --std=c++17 -g test/test_data.cc -Iinclude -fsanitize=address -o build/test_data && ./build/test_data

clean:
	@rm -rf build/*