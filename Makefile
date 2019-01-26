#
# Copyright 2015 gRPC authors.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

################################################################################
#
# Look for MY_STUFF below to customize this Makefile. There's no need to change
# anything else.
#
#################################################################################

HOST_SYSTEM = $(shell uname | cut -f 1 -d_)
SYSTEM ?= $(HOST_SYSTEM)
CXX = g++
CPPFLAGS += `pkg-config --cflags protobuf grpc`
CXXFLAGS += -std=c++11
ifeq ($(SYSTEM),Darwin)
LDFLAGS += -L/usr/local/lib `pkg-config --libs protobuf grpc++ grpc`\
           -lgrpc++_reflection\
           -ldl
else
LDFLAGS += -L/usr/local/lib `pkg-config --libs protobuf grpc++ grpc`\
           -Wl,--no-as-needed -lgrpc++_reflection -Wl,--as-needed\
           -ldl
endif
PROTOC = protoc
GRPC_CPP_PLUGIN = grpc_cpp_plugin
GRPC_CPP_PLUGIN_PATH ?= `which $(GRPC_CPP_PLUGIN)`

PROTOS_PATH = src/service_spec

vpath %.proto $(PROTOS_PATH)

#################################################################################
#
# MY_STUFF - begin
#
#################################################################################

CXXFLAGS += -fpermissive
CXX_DEBUG_FLAG := -g

# project folders
SOURCE_FOLDER := ./src/cpp_service
HEADER_FOLDER := ./src/cpp_service
BIN_FOLDER := ./bin
OBJ_FOLDER := "./bin/object"

# source code by folder
SOURCE += $(SOURCE_FOLDER)/symbolic_aggregate_approximation/*.cc
SOURCE += $(SOURCE_FOLDER)/piecewise_aggregate_approximation/*.cc
SOURCE += $(SOURCE_FOLDER)/density_curve/*.cc
SOURCE += $(SOURCE_FOLDER)/sequitur/*.cc
SOURCE += $(SOURCE_FOLDER)/anomaly_discovery/*.cc
SOURCE += $(SOURCE_FOLDER)/tests/*.cc
SOURCE += $(SOURCE_FOLDER)/core/*.cc
SOURCE += $(SOURCE_FOLDER)/tests/*.cc
SOURCE += $(SOURCE_FOLDER)/session_manager/*.cc
SOURCE += $(SOURCE_FOLDER)/utils/*.cc

# grpc generated objects based on the proto file
GRPC_PROTO_OBJ := timeSeriesAnomalyDetection.pb.o
GRPC_PROTO_OBJ += timeSeriesAnomalyDetection.grpc.pb.o

SERVER_DEPS := $(OBJ_FOLDER)/ErdbAnomalyDiscovery.o
SERVER_DEPS += $(OBJ_FOLDER)/SymbolicAggregateApproximation.o
SERVER_DEPS += $(OBJ_FOLDER)/PiecewiseAggregateApproximation.o
SERVER_DEPS += $(OBJ_FOLDER)/Sequitur.o
SERVER_DEPS += $(OBJ_FOLDER)/DensityCurve.o
SERVER_DEPS += $(OBJ_FOLDER)/timeSeriesUtils.o
SERVER_DEPS += $(OBJ_FOLDER)/SessionManager.o
SERVER_DEPS += $(OBJ_FOLDER)/utils.o

all: grpcProto genObj server client test_bin

server:
	$(CXX) $^ $(LDFLAGS) -o ./bin/server\
		$(OBJ_FOLDER)/server.o\
		$(OBJ_FOLDER)/timeSeriesAnomalyDetection.pb.o\
		$(OBJ_FOLDER)/timeSeriesAnomalyDetection.grpc.pb.o\
		/usr/local/lib/libgrpc++.so\
		$(SERVER_DEPS)

client:
	$(CXX) $^ $(LDFLAGS) -o ./bin/client\
		$(OBJ_FOLDER)/client.o\
		$(OBJ_FOLDER)/timeSeriesAnomalyDetection.pb.o\
		$(OBJ_FOLDER)/timeSeriesAnomalyDetection.grpc.pb.o\
		/usr/local/lib/libgrpc++.so

test_bin:
	$(CXX) -o ./bin/deployTests $(OBJ_FOLDER)/deployTestsMain.o

grpcProto: $(GRPC_PROTO_OBJ)
	@mv timeSeriesAnomalyDetection.pb.cc\
		timeSeriesAnomalyDetection.pb.h\
		timeSeriesAnomalyDetection.grpc.pb.cc\
		timeSeriesAnomalyDetection.grpc.pb.h $(SOURCE_FOLDER)/core

genObj:
	$(CXX) $(CXXFLAGS) -I$(HEADER_FOLDER) -c $(SOURCE)
	@mv *.o $(OBJ_FOLDER)

clean:
	@rm -rf *.o\
		$(SOURCE_FOLDER)/core/*.pb.cc\
		$(SOURCE_FOLDER)/core/*.pb.h\
		$(BIN_FOLDER)/server\
		$(BIN_FOLDER)/client\
		$(BIN_FOLDER)/deployTests\
		$(OBJ_FOLDER)/*.o\
		./storage*\
		./service_metadata.json\
		./snetd.config.json

#################################################################################
#
# MY_STUFF - end
#
#################################################################################

.PRECIOUS: %.grpc.pb.cc
%.grpc.pb.cc: %.proto
	$(PROTOC) -I $(PROTOS_PATH) --grpc_out=. --plugin=protoc-gen-grpc=$(GRPC_CPP_PLUGIN_PATH) $<

.PRECIOUS: %.pb.cc
%.pb.cc: %.proto
	$(PROTOC) -I $(PROTOS_PATH) --cpp_out=. $<
