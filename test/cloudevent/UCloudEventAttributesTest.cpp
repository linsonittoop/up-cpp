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
#include "up-cpp/cloudevent/datamodel/UCloudEventAttributes.h"

using namespace uprotocol::cloudevent;
using namespace uprotocol::v1;
using uprotocol::v1::UPriority;

/**
 * Test methods of UCloudEventAttributes::Builder.
 */
TEST(UCloudEventAttributesTest, BuilderMethods) {
    UCloudEventAttributes attributes = UCloudEventAttributes::Builder().build();
    EXPECT_TRUE(attributes.isEmpty());

    attributes = UCloudEventAttributes::Builder().withHash("hash")
                                                 .withPriority(UPriority::UPRIORITY_CS1)
                                                 .withTtl(100)
                                                 .withToken("token")
                                                 .withTraceparent("traceparent")
                                                 .build();
    EXPECT_FALSE(attributes.isEmpty());
}

/**
 * Test isEmpty() of UCloudEventAttributes.
 */
TEST(UCloudEventAttributesTest, isEmpty) {
    UCloudEventAttributes attributes1 = UCloudEventAttributes::Builder().build();
    EXPECT_TRUE(attributes1.isEmpty());
    EXPECT_TRUE(attributes1.hash().empty());
    EXPECT_EQ(attributes1.priority(), UPriority::UPRIORITY_UNSPECIFIED);
    EXPECT_EQ(attributes1.ttl(), 0);
    EXPECT_TRUE(attributes1.token().empty());
    EXPECT_TRUE(attributes1.traceparent().empty());

    UCloudEventAttributes attributes2;
    EXPECT_TRUE(attributes1 == attributes2);
}

/**
 * Test the accessors of UCloudEventAttributes.
 */
TEST(UCloudEventAttributesTest, Accessors) {
    UCloudEventAttributes attributes = UCloudEventAttributes::Builder().withHash("hash")
                                                                       .withPriority(UPriority::UPRIORITY_CS1)
                                                                       .withTtl(100)
                                                                       .withToken("token")
                                                                       .withTraceparent("traceparent")
                                                                       .build();
    EXPECT_EQ(attributes.hash(), "hash");
    EXPECT_EQ(attributes.priority(), UPriority::UPRIORITY_CS1);
    EXPECT_EQ(attributes.ttl(), 100);
    EXPECT_EQ(attributes.token(), "token");
    EXPECT_EQ(attributes.traceparent(), "traceparent");
}

/**
 * Test the equality of UCloudEventAttributes.
 */
TEST(UCloudEventAttributesTest, Equality) {
    UCloudEventAttributes attributes1 = UCloudEventAttributes::Builder().withHash("hash")
                                                                        .withPriority(UPriority::UPRIORITY_CS1)
                                                                        .withTtl(100)
                                                                        .build();
    // Compare with Empty attributes.
    UCloudEventAttributes::Builder builder = UCloudEventAttributes::Builder();
    UCloudEventAttributes attributes2 = builder.build();
    EXPECT_FALSE(attributes1 == attributes2);

    // Compare different attributes.
    attributes2 = builder.withHash("hash")
                         .withPriority(UPriority::UPRIORITY_CS1)
                         .withToken("token")
                         .build();
    EXPECT_FALSE(attributes1 == attributes2);

    // Compare same attributes.
    attributes2 = builder.withTtl(100)
                         .withToken("")
                         .build();
    EXPECT_TRUE(attributes1 == attributes2);

    // Compare again after modifying one of them.
    attributes2 = builder.withHash("different hash")
                         .build();
    EXPECT_FALSE(attributes1 == attributes2);
}

/**
 * Test the string representation of UCloudEventAttributes.
 */
TEST(UCloudEventAttributesTest, ToString) {
    UCloudEventAttributes attributes = UCloudEventAttributes::Builder().withHash("hash")
                                                                       .withPriority(UPriority::UPRIORITY_CS1)
                                                                       .withTtl(100)
                                                                       .withToken("value1")
                                                                       .withTraceparent("value2")
                                                                       .build();
    std::string expected = "UCloudEventAttributes{hash=hash, priority=2, ttl=100, token=value1, traceparent=value2}";
    EXPECT_EQ(attributes.toString(), expected);
}

/**
 * Main routine for execution of tests
*/
int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
