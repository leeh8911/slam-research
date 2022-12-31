

.PHONY: conan_install
conan_install:
	mkdir build || \
	cd build && \
	conan install .. --build=missing -s build_type=Debug

.PHONY: test
test: conan_install
	mkdir build || \
	cd build && \
	cmake ..&& \
	cmake --build . &&\
	.\\bin\\CONAN_TEST_TEST.exe
	
.PHONY: run
run: conan_install
	mkdir build || \
	cd build && \
	cmake ..&& \
	cmake --build . &&\
	.\\bin\\CONAN_TEST_EXE.exe

	
.PHONY: clean
clean:
	del .\\build /Q /f
