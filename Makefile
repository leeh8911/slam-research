

.PHONY: conan_install
conan_install:
	mkdir build || \
	cd build && \
	conan install .. --build=missing -s build_type=Debug

.PHONY: build
build: conan_install
	mkdir build || \
	cd build && \
	cmake ..&& \
	cmake --build .
	
.PHONY: test
test: conan_install
	mkdir build || \
	cd build && \
	cmake ..&& \
	cmake --build . &&\
	.\\bin\\CONAN_TEST_TEST.exe
	
.PHONY: test_all
test_all: conan_install
	mkdir build || \
	cd build && \
	cmake .. -DOPTION_TEST_ALL=TRUE && \
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

.PHONY: doc
doc:
	mkdir -p build
	cd build && \
	cmake -DOPTION_BUILD_DOCS=ON .. && \
	make docs