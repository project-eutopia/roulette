FROM buildpack-deps:zesty
MAINTAINER Christopher Locke <project.eutopia@gmail.com>

# Get required packages
RUN apt-get update && apt-get install -y \
        vim \
        git \
        build-essential \
        wget \
        cmake \
        libboost-all-dev \
        libgtest-dev \
        && \
    apt-get clean && \
    rm -rf /var/lib/apt/lists/*

# Install gtest for importing by CMake
RUN install -d gtest_build && \
    cd gtest_build && \
    cmake /usr/src/gtest/ && \
    make && \
    cp *.a /usr/lib && \
    cd ..

ENV ROULETTE_DIR "/opt/roulette"
RUN mkdir $ROULETTE_DIR
WORKDIR $ROULETTE_DIR

# Add current folder, and remove all extraneous working files
ADD . $ROULETTE_DIR
RUN git reset --hard HEAD
RUN git clean -fd

# Build the main exectable and tests
RUN mkdir build && \
    cd build && \
    cmake .. && \
    make -j8 main tests && \
    cd ..

RUN ln -sf $ROULETTE_DIR/build/main /usr/local/bin/roulette
