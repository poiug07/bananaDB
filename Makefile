default: build

generate_cmake:
	cmake -B build

build:
	cmake --build build

build_tests:
	cmake --build build --target Boost_Tests_run

test: build_tests
	build/tests/Boost_Tests_run

run: build
	./bin/bananaDB

clean:
	rm -rf bin
	rm -rf build