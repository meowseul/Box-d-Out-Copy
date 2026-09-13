.PHONY: exe clean

# Builds the project using the CMake configuration created by SETUP_ONCE.sh,
# then copies the resulting binary to ./bin/main (where CMakeLists.txt's
# CMAKE_RUNTIME_OUTPUT_DIRECTORY actually places it is ./build/bin/main).
exe:
	cmake --build build -j
	mkdir -p bin
	cp build/bin/main bin/main

clean:
	rm -rf build bin