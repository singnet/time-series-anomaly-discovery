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
CIRCLECI_VAR=0
RUN_VAR=0
INTEGRATION_VAR=0
DAEMON_VAR=0
PUBLISH_VAR=0
INSTALL_VAR=0
REQUEST_VAR=0
DEPLOY_VAR=0
LOAD_CONFIG_VAR=0
USAGE_VAR=0
BUILD_IMAGE_LATEST=0

# usage printing method
usage()
{
    echo 
    echo "usage: setup.sh [-b | --build] [-c | --circleci] [-r | --run] [-p | --publish] [-e | --request] [-i | --install_deps ] [-h | --help]"
    echo
    echo "This setup allow to build, publish, run, and test the service."
    echo
    echo "  -b | --build        Build the source code."
    echo "  -c | --circleci     Build circleci config file."
    echo "  -r | --run          Run the daemon and service GRPC server."
    echo "  -p | --publish      Publish service with the specified service info."
    echo "  -e | --request      Call for this service with specified service info."
    echo "  -i | --install_deps Install all dependencies to build and run this service."
    echo "  -h | --help         Display help message and exit."
    echo 
}

# filter all entered options
while getopts 'bcrpeidgh' flag; do
  case "${flag}" in
    b) COMPILE_VAR=1 ;;
    c) CIRCLECI_VAR=1 ;;
    r) RUN_VAR=1 ;;
    p) PUBLISH_VAR=1 ;;
    e) REQUEST_VAR=1 ;;
    i) INSTALL_VAR=1 ;;
    d) DEPLOY_VAR=1 ;;
    g) BUILD_IMAGE_LATEST=1 ;;
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
    \"DAEMON_END_POINT\": \"0.0.0.0:$SERVICE_DAEMON_PORT_VAR\",
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

################################################
# -------------------------------------------- #
# ------ GENERATE CIRCLE CI CONFIG FILE ------ #
# -------------------------------------------- #
################################################

buildCircleCiConfig()
{

CIRCLE_CI_CONFIG_STRING="defaults: &defaults
  docker:
  - image: circleci/python:3.6.6-node
  working_directory: ~/singnet
  environment:

    # service info, change this
    SERVICE_DAEMON_PORT: $SERVICE_DAEMON_PORT_VAR
    REPOSITORY: $REPOSITORY_NAME_VAR

    # cpp service base image, do not change this
    BASE_CPP_IMAGE_NAME: cpp_service_base_image
      
version: 2
jobs:
  build:    
    <<: *defaults
    steps:
    - run:
        name: Start container based on base cpp-service image
        command: |
          # check if base image exist
          BASIC_IMAGE_STATUS=\"$(ssh -tt -o \"StrictHostKeyChecking no\" $SSH_USER@$SSH_HOST 'docker images --format \"{{.ID}}: {{.Repository}}\" $BASE_CPP_IMAGE_NAME')\"

          echo $BASE_CPP_IMAGE_NAME
          echo $BASIC_IMAGE_STATUS

          # staging and production container names
          STAGING_DOCKER_CONTAINER_NAME=\"staging_\${CIRCLE_PROJECT_USERNAME}_\${REPOSITORY}\"
          PROD_DOCKER_CONTAINER_NAME=\"prod_\${CIRCLE_PROJECT_USERNAME}_\${REPOSITORY}\"

          ssh -o \"StrictHostKeyChecking no\" \$SSH_USER@\$SSH_HOST << EOF
            # try to remove staging container if it already exists
            echo \"Stopping and removing existing staging container for this user...\"
            docker stop \$STAGING_DOCKER_CONTAINER_NAME || true
            docker rm \$STAGING_DOCKER_CONTAINER_NAME || true
            echo \"Finished.\"

            # check if base cpp image exists, if not then build it with the assigned name
            if [ \"$BASIC_IMAGE_STATUS\" = \"\" ]; then
              echo \"Creating base cpp service image since it does not exist\"
              docker build --no-cache -t $BASE_CPP_IMAGE_NAME:latest https://raw.githubusercontent.com/$CIRCLE_PROJECT_USERNAME/$REPOSITORY/master/Dockerfiles/CppServiceBaseDockerfile
              echo \"Finish...\"
            fi

            # running the cpp service base container
            echo \"Running staging container based on the cpp basic service image...\"
            docker run -tdi --name \$STAGING_DOCKER_CONTAINER_NAME \$BASE_CPP_IMAGE_NAME
            echo \"Finished.\"
         
            # clonning this service repo into the cpp base container
            echo \"Downloading the \$REPOSITORY service to the staging container...\"
            docker exec \$STAGING_DOCKER_CONTAINER_NAME /bin/bash -c \\
              \"mkdir /home/ubuntu; \\
               cd /home/ubuntu; \\
               git clone https://github.com/\$CIRCLE_PROJECT_USERNAME/\$REPOSITORY.git\"
            echo \"Finished.\"
          EOF
    - run:
        name: Build source
        command: |
          # staging and production container names
          STAGING_DOCKER_CONTAINER_NAME=\"staging_\${CIRCLE_PROJECT_USERNAME}_\${REPOSITORY}\"
          PROD_DOCKER_CONTAINER_NAME=\"prod_\${CIRCLE_PROJECT_USERNAME}_\${REPOSITORY}\"

          ssh -o \"StrictHostKeyChecking no\" \$SSH_USER@\$SSH_HOST << EOF
            # Build source and run tests
            echo \"Building source inside staging container and performing integration tests...\"
            docker exec \$STAGING_DOCKER_CONTAINER_NAME /bin/bash -c \\
              \"cd /home/ubuntu/\$REPOSITORY; \\
               make clean; \\
               make\"
            echo \"Finished.\"
          EOF
    - run:
        name: Perform unit tests
        command: |
          # staging and production container names
          STAGING_DOCKER_CONTAINER_NAME=\"staging_\${CIRCLE_PROJECT_USERNAME}_\${REPOSITORY}\"
          PROD_DOCKER_CONTAINER_NAME=\"prod_\${CIRCLE_PROJECT_USERNAME}_\${REPOSITORY}\"

          ssh -o \"StrictHostKeyChecking no\" \$SSH_USER@\$SSH_HOST << EOF
            # Build source and run tests
            echo \"Building source inside staging container and performing integration tests...\"
            docker exec \$STAGING_DOCKER_CONTAINER_NAME /bin/bash -c \\
              \"cd /home/ubuntu/\$REPOSITORY; \\
               ./bin/release_cxxUnitTestsRunner.out\"
            echo \"Finished.\"
          EOF
    - run:
        name: Perform integration tests
        command: |
          # staging and production container names
          STAGING_DOCKER_CONTAINER_NAME=\"staging_\${CIRCLE_PROJECT_USERNAME}_\${REPOSITORY}\"
          PROD_DOCKER_CONTAINER_NAME=\"prod_\${CIRCLE_PROJECT_USERNAME}_\${REPOSITORY}\"

          ssh -o \"StrictHostKeyChecking no\" \$SSH_USER@\$SSH_HOST << EOF
            # Build source and run tests
            echo \"Building source inside staging container and performing integration tests...\"
            docker exec \$STAGING_DOCKER_CONTAINER_NAME /bin/bash -c \\
              \"cd /home/ubuntu/\$REPOSITORY; \\
               ./bin/release_integrationTests.out\"
            echo \"Finished.\"
          EOF
    - run:
        name: Deploy service
        command: |
          # staging and production container names
          STAGING_DOCKER_CONTAINER_NAME=\"staging_\${CIRCLE_PROJECT_USERNAME}_\${REPOSITORY}\"
          PROD_DOCKER_CONTAINER_NAME=\"prod_\${CIRCLE_PROJECT_USERNAME}_\${REPOSITORY}\"

          ssh -o \"StrictHostKeyChecking no\" \$SSH_USER@\$SSH_HOST << EOF
            if [ \$CIRCLE_REPOSITORY_URL = \"git@github.com:singnet/\$REPOSITORY.git\" ] && [ \$CIRCLE_BRANCH = \"master\" ] ; then
              echo \"This service will be deployed since it passed all tests...\"

              # stop old production containers
              echo \"Stopping and removing old production container...\"
              docker stop \$PROD_DOCKER_CONTAINER_NAME || true
              docker rm \$PROD_DOCKER_CONTAINER_NAME || true
              echo \"Finished.\"

              # stop staging container to the new producton one
              echo \"Stopping and removing staging container...\"
              docker stop \$STAGING_DOCKER_CONTAINER_NAME || true
              docker rm \$STAGING_DOCKER_CONTAINER_NAME || true 
              echo \"Finished.\"

              # running the cpp service container, service daemon and server
              echo \"Creating production container...\"
              docker run -tdi -p \$SERVICE_DAEMON_PORT:\$SERVICE_DAEMON_PORT --name \$PROD_DOCKER_CONTAINER_NAME \$BASE_CPP_IMAGE_NAME /bin/bash -c \\
                \"cd /home/ubuntu; \\
                 git clone https://github.com/\$CIRCLE_PROJECT_USERNAME/\$REPOSITORY.git; \\
                 cd \$REPOSITORY; \\
                 ./setup.sh -dr\"
              echo \"Finished.\"
              echo \"Service is up.\"
            else
              echo \"================================================================================\"
              echo \"                        Service WILL NOT be deployed\"
              echo \"================================================================================\"

              # remove staging containers since tests and build failed
              echo \"Stopping and removing staging container...\"
              docker stop \$STAGING_DOCKER_CONTAINER_NAME || true
              docker rm \$STAGING_DOCKER_CONTAINER_NAME || true
              echo \"Finished.\"
            fi
          EOF
workflows:
  version: 2
  build-and-deploy:
    jobs:
      - build"

    # try to delete old file to create a clean one
    if [ -f "./.circleci/config.yml" ]; then        
        rm ./.circleci/config.yml
    fi

    # print CI configuration into file
    echo "$CIRCLE_CI_CONFIG_STRING" >> ./.circleci/config.yml
}

buildDockerFile()
{
DOCKER_FILE="
# image to be used
FROM ubuntu:18.04

# update
RUN apt-get update

# install deps
RUN apt-get install -y libcurl4-gnutls-dev cxxtest nlohmann-json-dev build-essential autoconf libtool pkg-config \
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

# clone service repository into this dockerfile and install the DAEMON and service specific dependencies
RUN mkdir /home/ubuntu; \
    cd /home/ubuntu; \
    git clone https://github.com/Ophien/time-series-anomaly-discovery.git; \
    cd time-series-anomaly-discovery; \
    ./setup.sh -d

# run DAEMON and service
CMD [\"/bin/bash\", \"-c\", \"/home/ubuntu/time-series-anomaly-discovery/setup.sh -r\"]"

return $DOCKER_FILE
}

run()
{
    # run daemon for kovan
    snetd --config ./snetd_configs/snetd.kovan.json & 

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

callService()
{
    echo
    echo "Running a test call to this service daemon with the specified data in the 'service_conf' file."

    # change to the correct network
    snet network $NETWORK_VAR
    
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
# ----- INSTALL ALL DEPS FOR THIS SERVICE ---- #
# -------------------------------------------- #
################################################

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

    # install daemon
    mkdir snet-daemon; \
    cd snet-daemon; \
    wget -q https://github.com/singnet/snet-daemon/releases/download/v0.1.6/snet-daemon-v0.1.6-linux-amd64.tar.gz; \
    tar -xvf snet-daemon-v0.1.6-linux-amd64.tar.gz; \
    mv ./snet-daemon-v0.1.6-linux-amd64/snetd /usr/bin/snetd; \
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

################################################
# -------------------------------------------- #
# ----------- INSTALL SNET DAEMON ------------ #
# -------------------------------------------- #
################################################

if [ $DEPLOY_VAR == 1 ]; then
    # install daemon
    mkdir snet-daemon; \
    cd snet-daemon; \
    wget -q https://github.com/singnet/snet-daemon/releases/download/v0.1.6/snet-daemon-v0.1.6-linux-amd64.tar.gz; \
    tar -xvf snet-daemon-v0.1.6-linux-amd64.tar.gz; \
    mv ./snet-daemon-v0.1.6-linux-amd64/snetd /usr/bin/snetd; \
    cd ..; \
    rm -rf snet-daemon
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
# --------------- BUILD SOURCE --------------- #
# -------------------------------------------- #
################################################

# create singnet circle ci configuration file
if [ $CIRCLECI_VAR == 1 ]; then
    buildCircleCiConfig
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
    snet service metadata-add-endpoints http://$HOST_IP_ADDRESS_VAR:$SERVICE_DAEMON_PORT_VAR

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

################################################
# -------------------------------------------- #
# ----- CALL A REQUEST FOR THIS SERVICE ------ #
# -------------------------------------------- #
################################################

if [ $REQUEST_VAR == 1 ]; then
    # do a test call for this service daemon
    callService
fi