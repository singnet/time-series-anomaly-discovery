#!/bin/bash

################################################
# -------------------------------------------- #
# ------------ SETUP PARAMETERS -------------- #
# -------------------------------------------- #
################################################

# command received flags
COMPILE_VAR=0
RUN_VAR=0
PUBLISH_VAR=0
INSTALL_VAR=0
LOAD_CONFIG_VAR=0
INSTALL_SNET_DAEMON=0

# usage printing method
usage()
{
    echo 
    echo "usage: setup.sh [-b] [-r] [-p] [-e] [-i] [-d] [-h]"
    echo
    echo "This setup allow to build, publish, run, and test the service."
    echo
    echo "  -b    Build the source code."
    echo "  -r    Run the daemon and service GRPC server."
    echo "  -p    Publish service with the specified service info."
    echo "  -i    Install all dependencies to build and run this service."
    echo "  -d    Install SNET Daemon."
    echo "  -h    Display help message and exit."
    echo 
}

# filter all entered options
while getopts 'brpeidh' flag; do
  case "${flag}" in
    b) COMPILE_VAR=1 ;;
    r) RUN_VAR=1 ;;
    p) PUBLISH_VAR=1 ;;
    e) REQUEST_VAR=1 ;;
    i) INSTALL_VAR=1 ;;
    d) INSTALL_SNET_DAEMON=1 ;;
    h) usage
       exit ;;
    *) usage
       exit 1 ;;
  esac
done

################################################
# -------------------------------------------- #
# --------- LOAD SERVICE CONFIGURATION ------- #
# -------------------------------------------- #
################################################

SERVICE_DESCRIPTION_VAR=
REPO_URL_VAR=
NETWORK_VAR=
TAGS_VAR=

DISPLAY_NAME_VAR=
SERVICE_NAME_VAR=
WALLET_VAR=
PRICE_VAR=

# network
SERVICE_SERVER_PORT_VAR=
SERVICE_DAEMON_PORT_VAR=
HOST_IP_ADDRESS_VAR=

# blockchain
ORGANIZATION_TO_PUBLISH_VAR=
IPFS_END_POINT_VAR=
REGISTRY_KEY_VAR=

# this script's path
PROJECT_PATH="$(dirname "$0")"

# try to load config file
if [ -f "$PROJECT_PATH/service_conf" ]; then
    # setup input parameters
    echo
    echo "Loading 'service_conf' file."

    # loading config file
    source $PROJECT_PATH/service_conf
    LOAD_CONFIG_VAR=1

    # set basic service conf
    DISPLAY_NAME_VAR=$DISPLAY_NAME
    SERVICE_DESCRIPTION_VAR=$SERVICE_DESCRIPTION
    REPO_URL_VAR=$REPO_URL
    TAGS_VAR=$TAGS
    NETWORK_VAR=$NETWORK
    REPOSITORY_NAME_VAR=$REPOSITORY_NAME
    SERVICE_NAME_VAR=$SERVICE_NAME
    WALLET_VAR=$WALLET
    PRICE_VAR=$PRICE

    # network
    SERVICE_SERVER_PORT_VAR=$SERVICE_SERVER_PORT
    SERVICE_DAEMON_PORT_VAR=$SERVICE_DAEMON_PORT
    HOST_IP_ADDRESS_VAR=$HOST_IP_ADDRESS

    # blockchain
    ORGANIZATION_TO_PUBLISH_VAR=$ORGANIZATION_TO_PUBLISH
    IPFS_END_POINT_VAR=$IPFS_END_POINT
    REGISTRY_KEY_VAR=$REGISTRY_KEY
        
    echo "Config file loaded."
    echo
else
    echo "Config file not found."
    exit 1
fi

run()
{
    # run daemon for kovan
    # (TODO:check if need to run directly from there)
    # snetd --config ./snetd_configs/snetd.mainnet.json & 

    # run daemon for the ropsten
    snetd --config ./snetd_configs/snetd.ropsten.json & 
    
    # run server
    ./bin/release_server.out
}

compileAndTest()
{
    # build source
    make clean; make

    echo "Running unit tests..."
    ./bin/release_cxxUnitTestsRunner.out

    echo "Running integration tests..."
    ./bin/release_integrationTests.out
}

################################################
# -------------------------------------------- #
# ----- INSTALL ALL DEPS FOR THIS SERVICE ---- #
# -------------------------------------------- #
################################################

if [ $INSTALL_SNET_DAEMON == 1 ]; then
    # install daemon
    mkdir snet-daemon; \
    cd snet-daemon; \
    wget -q https://github.com/singnet/snet-daemon/releases/download/v0.1.8/snet-daemon-v0.1.8-linux-amd64.tar.gz; \
    tar -xvf snet-daemon-v0.1.8-linux-amd64.tar.gz; \
    mv ./snet-daemon-v0.1.8-linux-amd64/snetd /usr/bin/snetd; \
    cd ..; \
    rm -rf snet-daemon
fi

if [ $INSTALL_VAR == 1 ]; then
    apt-get update;\
    apt-get install -y libcurl4-gnutls-dev cxxtest nlohmann-json-dev build-essential autoconf libtool pkg-config \
                       libgflags-dev libgtest-dev clang libc++-dev git curl nano \
                       wget libudev-dev libusb-1.0-0-dev nodejs npm python3 python3-pip libboost-all-dev;\

    # try upgrade pip
    pip install --upgrade pip; \

    # install GRPC
    cd /;\
    git clone -b $(curl -L https://grpc.io/release) https://github.com/grpc/grpc; \
    cd grpc; \
    git submodule update --init; \
    make; \
    make install; \
    cd third_party/protobuf; \
    make install; \
    cd /;\

    # install cli
    cd /opt; \
    git clone https://github.com/singnet/snet-cli; \
    cd snet-cli; \
    ./scripts/blockchain install; \
    pip3 install -e .; \
    cd $PROJECT_PATH 
fi

################################################
# -------------------------------------------- #
# --------------- BUILD SOURCE --------------- #
# -------------------------------------------- #
################################################

# clean environment and object files to guarantee a clean build and build source code
if [ $COMPILE_VAR == 1 ]; then
    compileAndTest
fi

################################################
# -------------------------------------------- #
# ------------ PUBLISH SERVICE --------------- #
# -------------------------------------------- #
################################################

if [ $PUBLISH_VAR == 1 ]; then
    # change to the correct network
    snet network $NETWORK_VAR

    # delete service before trying to publish it
    snet service delete $ORGANIZATION_TO_PUBLISH_VAR $SERVICE_NAME_VAR -y

    # create metadata json for this service with its name and the wallet that will receive money
    snet service metadata-init src/service_spec "$DISPLAY_NAME_VAR" $WALLET_VAR

    # set the price to use this service
    snet service metadata-set-fixed-price $PRICE_VAR

    # set the local port to access this service server
    snet service metadata-add-endpoints https://$HOST_IP_ADDRESS_VAR:$SERVICE_DAEMON_PORT_VAR

    # add description to this service
    JSON="{\"description\":\"$SERVICE_DESCRIPTION_VAR\", \"url\":\"$REPO_URL_VAR\"}"

    # try to delete old file to create a clean one
    if [ -f "desc.json" ]; then        
        rm desc.json
    fi

    # print JSON into configuration file
    echo $JSON >> desc.json

    # set metadata for the generated config
    snet service metadata-add-description --json "$(cat desc.json)"

    # publish the service at the especified organization
    snet service publish $ORGANIZATION_TO_PUBLISH_VAR $SERVICE_NAME_VAR -y

    # add tags to the service    
    snet service update-add-tags $ORGANIZATION_TO_PUBLISH_VAR $SERVICE_NAME_VAR $TAGS_VAR -y
fi

################################################
# -------------------------------------------- #
# ----- RUN THE NEWLY PUBLISHED SERVICE ------ #
# -------------------------------------------- #
################################################

if [ $RUN_VAR == 1 ]; then
    if [ $COMPILE_VAR == 0 ]; then
        # build source
        compileAndTest     
    fi

    # running the service
    run
fi