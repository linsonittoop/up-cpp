/*
 * Copyright (c) 2023-2024 General Motors GTO LLC
 *
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 *
 * SPDX-FileType: SOURCE
 * SPDX-FileCopyrightText: 2023 General Motors GTO LLC
 * SPDX-License-Identifier: Apache-2.0
 */

#include <gtest/gtest.h>
#include <google/protobuf/util/message_differencer.h>
#include "up-cpp/cloudevent/serialize/ProtobufSerializer.h"
#include "up-cpp/cloudevent/validate/UCloudEventValidator.h"

using namespace io::cloudevents::v1;
using namespace uprotocol::cloudevent;

/**
 * @brief Compare two messages.
 *
 * @param msg1 - First message to be comapred.
 * @param msg2 - Second message to be compared.
 *
 * @return True if both messages are equal.
 */
static inline bool CompareMessages(const google::protobuf::Message& msg1,
                                   const google::protobuf::Message& msg2) {
    return google::protobuf::util::MessageDifferencer::Equals(msg1, msg2);
}

/**
 * @brief Create a valid CloudEvent.
 *
 * @return CloudEvent - A valid CloudEvent.
 */
CloudEvent createValidCloudEvent() {
    CloudEvent cloudEvent;
    cloudEvent.set_type("req.v1");
    cloudEvent.set_id("id 88");
    cloudEvent.set_spec_version("v1");
    cloudEvent.set_source("up://blabla");
    cloudEvent.set_text_data("hfgljhgljhghhhhhhhhhhhhhh");
    auto attr = std::make_unique<CloudEvent_CloudEventAttributeValue>();
    std::string str = "1";
    attr->set_ce_string(str);
    (*cloudEvent.mutable_attributes())["sink"] = *attr;
    auto attr2 = std::make_unique<CloudEvent_CloudEventAttributeValue>();
    attr2->set_ce_integer(88);
    (*cloudEvent.mutable_attributes())["ttl"] = *attr2;
    return cloudEvent;
}

/**
 * @brief Create invalid CloudEvent.
 *
 * @return CloudEvent - Invalid CloudEvent.
 */
CloudEvent createBadCloudEvent() {
    CloudEvent cloudEvent;
    cloudEvent.set_type("req.v1");
    cloudEvent.set_id("id 88");
    cloudEvent.set_spec_version("v1");
    cloudEvent.set_source("up://blabla");
    auto attr = std::make_unique<CloudEvent_CloudEventAttributeValue>();
    std::string str = "1";
    attr->set_ce_string(str);
    (*cloudEvent.mutable_attributes())["sink"] = *attr;
    auto attr2 = std::make_unique<CloudEvent_CloudEventAttributeValue>();
    attr2->set_ce_string("88");
    (*cloudEvent.mutable_attributes())["ttl"] = *attr2;
    return cloudEvent;
}

/**
 * @brief Test serialize an Invaild CloudEvent.
 */
TEST(ProtobufSerializerTest, SerializeInvalidCloudEvent) {
    CloudEvent cloudEvent = createBadCloudEvent();
    auto isValid = UCloudEventValidator::is_valid_event(cloudEvent);
    EXPECT_FALSE(isValid);
    std::vector<uint8_t> data = ProtobufSerializer().serialize(cloudEvent);
    EXPECT_TRUE(data.size() == 0);
}

/**
 * Test serialize an empty CloudEvent.
 */
TEST(ProtobufSerializerTest, SerializeEmptyCloudEvent) {
    CloudEvent emptyCloudEvent;
    std::vector<uint8_t> data = ProtobufSerializer().serialize(emptyCloudEvent);
    EXPECT_TRUE(data.size() == 0);
}

/**
 * @brief Test serialize a vaild CloudEvent and deserialize it back to CloudEvent.
 */
TEST(ProtobufSerializerTest, ValidCloudEvent) {
    auto serializer = ProtobufSerializer();
    CloudEvent cloudEvent = createValidCloudEvent();
    std::vector<uint8_t> data = serializer.serialize(cloudEvent);
    EXPECT_TRUE(data.size() != 0);

    auto result = serializer.deserialize(data);
    EXPECT_TRUE(result != std::nullopt);
    EXPECT_TRUE(CompareMessages(cloudEvent, *result.value()));
}

/**
 * Test deserialize an invalid data.
 */
TEST(ProtobufSerializerTest, DeserializeInvalidData) {
    auto serializer = ProtobufSerializer();
    CloudEvent cloudEvent = createValidCloudEvent();
    std::vector<uint8_t> data = serializer.serialize(cloudEvent);
    EXPECT_TRUE(data.size() != 0);

    data.push_back(0x77);
    auto result = serializer.deserialize(data);
    EXPECT_TRUE(result == std::nullopt);
}

/**
 * Test deserialize an empty data.
 */
TEST(ProtobufSerializerTest, DeserializeEmptyData) {
    std::vector<uint8_t> data;
    auto result = ProtobufSerializer().deserialize(data);
    EXPECT_TRUE(result == std::nullopt);
}

/**
 * Main routine to run the tests.
 */
int main(int argc, char** argv) {

    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}
