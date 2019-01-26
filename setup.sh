#!/bin/bash



################################################
# -------------------------------------------- #
# ------------ SETUP PARAMETERS -------------- #
# -------------------------------------------- #
################################################

# setup dev alias
alias dev_test='make clean;git add *; git commit -m "fixing dockerfile"; git push origin master; docker rmi dev_test:dev_test --force; docker build --no-cache -t dev_test:dev_test .;docker run -it dev_test:dev_test'

# command received flags
COMPILE_VAR=0
RUN_VAR=0
INTEGRATION_VAR=0
DAEMON_VAR=0
PUBLISH_VAR=0
INSTALL_VAR=0
REQUEST_VAR=0
LOAD_CONFIG_VAR=0
USAGE_VAR=0



# usage printing method
usage()
{
    echo 
    echo "usage: setup.sh [-c | --compile] [-r | --run] [-p | --publish] [-r | --request] [-i | --install_deps ] [-h | --help]"
    echo
    echo "This setup allow to build, publish, run, and test the service."
    echo
    echo "  -c | --compile      Compile the source code."
    echo "  -r | --run          Run the daemon and service GRPC server."
    echo "  -p | --publish      Publish service with the specified service info."
    echo "  -e | --request      Call for this service with specified service info."
    echo "  -i | --install_deps Install all dependencies to build and run this service."
    echo "  -h | --help         Display help message and exit."
    echo 
}

# filter all entered options
while getopts 'crpeih' flag; do
  case "${flag}" in
    c) COMPILE_VAR=1 ;;
    r) RUN_VAR=1 ;;
    p) PUBLISH_VAR=1 ;;
    e) REQUEST_VAR=1 ;;
    i) INSTALL_VAR=1 ;;
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
CRIPTOCURRENCY_NETWORK_VAR=
REGISTRY_KEY_VAR=

# test call input
TEST_CALL_INPUT_VAR=
TEST_CALL_METHOD_VAR=

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
    CRIPTOCURRENCY_NETWORK_VAR=$CRIPTOCURRENCY_NETWORK
    REGISTRY_KEY_VAR=$REGISTRY_KEY

    # test call input
    TEST_CALL_INPUT_VAR=$DEPLOY_TEST_CALL_INPUT
    TEST_CALL_METHOD_VAR=$DEPLOY_TEST_CALL_METHOD
        
    echo "Config file loaded."
    echo
else
    echo "Config file not found."
    exit 1
fi

createDeamonConfig()
{
    # create daemon config file and associate the SERVICE_DAEMON_PORT with the SERVICE_SERVER_PORT
    JSON="{
    \"DAEMON_END_POINT\": \"https://$HOST_IP_ADDRESS_VAR:$SERVICE_DAEMON_PORT_VAR\",
    \"ETHEREUM_JSON_RPC_ENDPOINT\": \"$CRIPTOCURRENCY_NETWORK_VAR\",
    \"IPFS_END_POINT\": \"$IPFS_END_POINT_VAR\",
    \"REGISTRY_ADDRESS_KEY\": \"$REGISTRY_KEY_VAR\",
    \"PASSTHROUGH_ENABLED\": true,
    \"PASSTHROUGH_ENDPOINT\": \"http://localhost:$SERVICE_SERVER_PORT_VAR\",
    \"ORGANIZATION_ID\": \"$ORGANIZATION_TO_PUBLISH_VAR\",
    \"SERVICE_ID\": \"$SERVICE_NAME_VAR\",
    \"LOG\": {
        \"LEVEL\": \"debug\",
        \"OUTPUT\": {
                \"TYPE\": \"stdout\"
            }
    }
    }"

    # try to delete old file to create a clean one
    if [ -f "snetd.config.json" ]; then        
        rm snetd.config.json
    fi

    # print JSON into configuration file
    echo $JSON >> snetd.config.json
}

run()
{
    # create snet daemon config file
    createDeamonConfig

    # run daemon
    snetd --config snetd.config.json &

    # run the service local server
    ./bin/server &
}

compileAndTest()
{
    # build source
    make clean; make

    # run tests
    ./bin/deployTests
}

callService()
{
    echo
    echo "Running a test call to this service daemon with the specified data in the 'service_conf' file."

    # open a channel with the deposited amount to call for this service
    CHANNEL_TIME_OUT=11000000
    RESPONSE="$(snet channel open-init $ORGANIZATION_TO_PUBLISH_VAR $SERVICE_NAME_VAR $PRICE_VAR $CHANNEL_TIME_OUT -y)"

    # get channel ID from the last substring obtained from the RESPONSE variable
    RESPONSES=( $RESPONSE )
    LENGTH=${#RESPONSES[@]}
    CHANNEL_ID_INDEX=$(($LENGTH - 1))
    CHANNEL_ID=${RESPONSES[$(($LENGTH - 1))]}

    # call for the created service
    DAEMON_RESPONSE="$(snet client call "$CHANNEL_ID" "$PRICE_VAR" "$HOST_IP_ADDRESS_VAR:$SERVICE_DAEMON_PORT_VAR" "$TEST_CALL_METHOD_VAR" "$TEST_CALL_INPUT_VAR")"

    # print response from daemon
    echo
    echo "Daemon response:"
    echo
    echo $DAEMON_RESPONSE
    echo
}

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
    # delete service before trying to publish it
    snet service delete $ORGANIZATION_TO_PUBLISH_VAR $SERVICE_NAME_VAR -y

    # create metadata json for this service with its name and the wallet that will receive money
    snet service metadata-init src/service_spec $SERVICE_NAME_VAR $WALLET_VAR

    # set the price to use this service
    snet service metadata-set-fixed-price $PRICE_VAR

    # set the local port to access this service server
    snet service metadata-add-endpoints https://$HOST_IP_ADDRESS_VAR:$SERVICE_DAEMON_PORT_VAR

    # publish the service at the especified organization
    snet service publish $ORGANIZATION_TO_PUBLISH_VAR $SERVICE_NAME_VAR -y
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

################################################
# -------------------------------------------- #
# ----- CALL A REQUEST FOR THIS SERVICE ------ #
# -------------------------------------------- #
################################################

if [ $REQUEST_VAR == 1 ]; then
    # do a test call for this service daemon
    callService
fi

################################################
# -------------------------------------------- #
# ----- INSTALL ALL DEPS FOR THIS SERVICE ---- #
# -------------------------------------------- #
################################################

if [ $INSTALL_VAR == 1 ]; then
    apt-get update;\
    apt-get install -y nlohmann-json-dev build-essential autoconf libtool pkg-config \
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

    # install daemon
    mkdir snet-daemon; \
    cd snet-daemon; \
    wget -q https://github.com/singnet/snet-daemon/releases/download/v0.1.5/snet-daemon-v0.1.5-linux-amd64.tar.gz; \
    tar -xvf snet-daemon-v0.1.5-linux-amd64.tar.gz; \
    mv ./snet-daemon-v0.1.5-linux-amd64/snetd /usr/bin/snetd; \
    cd ..; \
    rm -rf snet-daemon; \

    # install cli
    cd /opt; \
    git clone https://github.com/singnet/snet-cli; \
    cd snet-cli; \
    ./scripts/blockchain install; \
    pip3 install -e .; \
    cd $PROJECT_PATH 

fi