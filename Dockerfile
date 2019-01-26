# image to be used
FROM ubuntu:18.04

# working directory
ENV SERVICE_CORE=/home/ubuntu/service
ENV REPO_NAME=TSAD-service

# make folders
RUN mkdir -p ${SERVICE_CORE}

# update
RUN apt-get update

# install GRPC deps, clean build, and tests
RUN apt-get install -y nlohmann-json-dev build-essential autoconf libtool pkg-config \
                       libgflags-dev libgtest-dev clang libc++-dev git curl nano \
                       wget libudev-dev libusb-1.0-0-dev nodejs npm python3 python3-pip libboost-all-dev

# install GRPC
RUN git clone -b $(curl -L https://grpc.io/release) https://github.com/grpc/grpc; \
    cd grpc; \
    git submodule update --init; \
    make; \
    make install; \
    cd third_party/protobuf; \
    make install; \
    cd /

# install daemon
RUN mkdir snet-daemon; \
    cd snet-daemon; \
    wget -q https://github.com/singnet/snet-daemon/releases/download/v0.1.5/snet-daemon-v0.1.5-linux-amd64.tar.gz; \
    tar -xvf snet-daemon-v0.1.5-linux-amd64.tar.gz; \
    mv ./snet-daemon-v0.1.5-linux-amd64/snetd /usr/bin/snetd; \
    cd ..; \
    rm -rf snet-daemon

# Install snet-cli
RUN cd /opt; \
    git clone https://github.com/singnet/snet-cli; \
    cd snet-cli; \
    ./scripts/blockchain install; \
    pip3 install -e .

# clone service project from git
RUN cd ${SERVICE_CORE}; \
    git clone https://github.com/Ophien/TSAD-service.git

WORKDIR ${SERVICE_CORE}/${REPO_NAME}

CMD ["/bin/sh", "-c", "setup.sh -r"]
