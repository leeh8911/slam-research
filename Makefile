

.PHONY: conan
conan:
	cd build && \
	conan install .. --build=missing -s build_type=Debug

.PHONY: test
test: conan
	mkdir -p build || \
	cd build && \
	cmake ..&& \
	cmake --build . &&\
	.\\bin\\CONAN_TEST_TEST.exe
	
.PHONY: run
run: conan
	mkdir -p build || \
	cd build && \
	cmake ..&& \
	cmake --build . &&\
	.\\bin\\CONAN_TEST_EXE.exe

	
.PHONY: clean
clean:
	del .\\build /Q /f
