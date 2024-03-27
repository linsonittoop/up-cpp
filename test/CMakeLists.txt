# Copyright (c) 2023 General Motors GTO LLC
#
# Licensed to the Apache Software Foundation (ASF) under one
# or more contributor license agreements.  See the NOTICE file
# distributed with this work for additional information
# regarding copyright ownership.  The ASF licenses this file
# to you under the Apache License, Version 2.0 (the
# "License"); you may not use this file except in compliance
# with the License.  You may obtain a copy of the License at
#
#   http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing,
# software distributed under the License is distributed on an
# "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
# KIND, either express or implied.  See the License for the
# specific language governing permissions and limitations
# under the License.
#
# SPDX-FileType: SOURCE
# SPDX-FileCopyrightText: 2023 General Motors GTO LLC
# SPDX-License-Identifier: Apache-2.0

enable_testing()

find_package(GTest REQUIRED)

file(GLOB_RECURSE SRC_PROTO_CORE_API "${CMAKE_CURRENT_BINARY_DIR}/up-core-api/uprotocol/*.cc")

add_executable(CloudEventAttributesTest cloudevent/UCloudEventAttributesTest.cpp)

target_link_libraries(CloudEventAttributesTest
		PUBLIC
			up-cpp::up-cpp
			up-cpp::up-core-api-protos
		PRIVATE
			GTest::gtest_main
			GTest::gmock
			pthread
)

add_test("t-01-CloudEventAttributesTest" ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/CloudEventAttributesTest)

add_executable(CloudEventProtobufSerializerTest cloudevent/ProtobufSerializerTest.cpp)

target_link_libraries(CloudEventProtobufSerializerTest
		PUBLIC
			up-cpp::up-cpp
			cloudevent_proto
			up-cpp::up-core-api-protos
		PRIVATE
			GTest::gtest_main
			GTest::gmock
			pthread
)

add_test("t-02-CloudEventProtobufSerializerTest" CloudEventProtobufSerializerTest)

add_executable(CloudEventJsonSerializerTest cloudevent/JsonSerializerTest.cpp)

target_link_libraries(CloudEventJsonSerializerTest
		PUBLIC
			up-cpp::up-cpp
			cloudevent_proto
			up-cpp::up-core-api-protos
		PRIVATE
			GTest::gtest_main
			GTest::gmock
			pthread
)

add_test("t-03-CloudEventJsonSerializerTest" CloudEventJsonSerializerTest)

add_executable(base64_test
	tools/base64_test.cpp)
target_link_libraries(base64_test
 		PUBLIC
			up-cpp::up-cpp
        PRIVATE
			GTest::gtest_main
			GTest::gmock
			pthread
)
add_test("t-06-base64-test" ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/base64_test)

#add_test("t-07-ce-factory-test" base64_test)
#add_executable(service_type_test model/serviceType_test.cpp)
#target_link_libraries(service_type_test ${CGREEN_LIBRARY} up-cpp)

#add_test("t-08-service-type-test" service_type_test)

add_executable(UAuthorityTest
	uri/builder/UAuthorityTest.cpp)
target_link_libraries(UAuthorityTest
		PUBLIC
			up-cpp::up-cpp
			spdlog::spdlog
			up-cpp::up-core-api-protos
			protobuf::protobuf
		PRIVATE
			GTest::gtest_main
			GTest::gmock
			pthread
)
add_test("t-09-UAuthorityTest" ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/UAuthorityTest)

add_executable(UEntityTest
		uri/builder/UEntityTest.cpp)
target_link_libraries(UEntityTest
		PUBLIC
			up-cpp::up-cpp
			spdlog::spdlog
			up-cpp::up-core-api-protos
			protobuf::protobuf
		PRIVATE
			GTest::gtest_main
			GTest::gmock
			pthread
)
add_test("t-10-UEntityTest" ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/UEntityTest)

add_executable(UResourceTest
	uri/builder/UResourceTest.cpp)
target_link_libraries(UResourceTest
		PUBLIC
			up-cpp::up-cpp
			spdlog::spdlog
			up-cpp::up-core-api-protos
			protobuf::protobuf
		PRIVATE
			GTest::gtest_main
			GTest::gmock
			pthread
)
add_test("t-11-UResourceTest" ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/UResourceTest)

add_executable(UUriTest
	uri/builder/UUriTest.cpp)
target_link_libraries(UUriTest
		PUBLIC
			up-cpp::up-cpp
			spdlog::spdlog
			up-cpp::up-core-api-protos
			protobuf::protobuf
		PRIVATE
			GTest::gtest_main
			GTest::gmock
			pthread
)
add_test("t-12-UUriTest" ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/UUriTest)

add_executable(LongUriSerializerTest
		uri/serializer/LongUriSerializerTest.cpp)
target_compile_options(LongUriSerializerTest PRIVATE -g -O0)
target_link_libraries(LongUriSerializerTest
		PUBLIC
			up-cpp::up-cpp
			spdlog::spdlog
			up-cpp::up-core-api-protos
			protobuf::protobuf
		PRIVATE
			GTest::gtest_main
			GTest::gmock
			pthread
)
add_test("t-13-LongUriSerializerTest" ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/LongUriSerializerTest)

add_executable(MicroUriSerializerTest
	uri/serializer/MicroUriSerializerTest.cpp)
target_compile_options(MicroUriSerializerTest PRIVATE -g -O0)
target_link_libraries(MicroUriSerializerTest
		PUBLIC
			up-cpp::up-cpp
			spdlog::spdlog
			up-cpp::up-core-api-protos
			protobuf::protobuf
		PRIVATE
			GTest::gtest_main
			GTest::gmock
			pthread
)
add_test("t-14-MicroUriSerializerTest" ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/MicroUriSerializerTest)

# include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../)
add_executable(umessagetypes_test
	utransport/umessagetypes_test.cpp)
target_link_libraries(umessagetypes_test
		PUBLIC
			up-cpp::up-cpp
		PRIVATE
			GTest::gtest_main
			GTest::gmock
			pthread
)
add_test("t-15-umessagetypes_test" ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/umessagetypes_test)

# include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../)
add_executable(upayload_test
	utransport/upayload_test.cpp)
target_link_libraries(upayload_test
		PUBLIC
			up-cpp::up-cpp
		PRIVATE
			GTest::gtest_main
			GTest::gmock
			pthread
)
add_test("t-16-upayload_test" ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/upayload_test)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../)
add_executable(upriority_test
	utransport/upriority_test.cpp)
target_link_libraries(upriority_test
		PUBLIC
			up-cpp::up-cpp
		PRIVATE
			GTest::gtest_main
			GTest::gmock
			pthread
)
add_test("t-17-upriority_test" ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/upriority_test)

# include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../)
add_executable(userializationhint_test
	utransport/userializationhint_test.cpp)
target_link_libraries(userializationhint_test
		PUBLIC
			up-cpp::up-cpp
		PRIVATE
			GTest::gtest_main
			GTest::gmock
			pthread
)
add_test("t-18-userializationhint_test" ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/userializationhint_test)

