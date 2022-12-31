FROM ubuntu:20.04

LABEL description="Container for use with Visual Studio" 
LABEL author="leeh8911@gmail.com"

RUN groupadd -r postgres && useradd --no-log-init -r -g postgres postgres
# USER root
ARG NUM_CORES=8

ENV DEBIAN_FRONTEND=noninteractive

# Basics
RUN apt-get update && apt-get install -qq -y -f --no-install-recommends \
    # install linux dependencies
    ## Base dependencies
    ca-certificates libgoogle-glog-dev \
    libgtest-dev automake wget curl unzip autoconf libtool g++ gcc make gdb \
    cmake git vim \
    sqlitebrowser sqlite3 libsqlite3-dev \ 
    ## Security
    openssh-client \
    ## BLAS & LAPACK
    libatlas-base-dev \
    ## Eigen3
    libeigen3-dev \
    ## SuiteSparse and CXSparse (optional)
    ## - If you want to build Ceres as a *static* library (the default)
    ##   you can use the SuiteSparse package in the main Ubuntu package
    ##   repository:
    libsuitesparse-dev \
    ## Sphinx (for documentation)
    sphinx-doc sphinx-common \
    ## code linter and formatter
    cppcheck clang clang-tidy clang-format \
    ## python pip for cpplint
    python3-pip \
    ## For doxygen
    graphviz npm flex bison && \
    # cpplint
    pip install --no-cache-dir cpplint && \ 
    apt-get autoremove -y && \
    apt-get clean && \
    rm -rf /var/lib/apt/lists/*
RUN npm install mathjax

## ceres-solver
WORKDIR /thirdparty/repo
RUN git clone --depth=1 https://ceres-solver.googlesource.com/ceres-solver && \
    cd ceres-solver && mkdir build && cd build && \
    cmake .. && cmake --build . --target install && \
    cd ../../
# build and install
## doxygen
WORKDIR /thirdparty/repo
RUN git clone https://github.com/doxygen/doxygen.git && \
    cd doxygen && mkdir build && cd build && \
    cmake -G "Unix Makefiles" .. && \
    cmake --build . --target install && \
    cd ../../
## google test
WORKDIR /thirdparty/repo
RUN git clone https://github.com/google/googletest.git && \
    cd googletest && mkdir build && cd build && \
    cmake .. && \
    cmake --build . --target install && \
    cd ../../