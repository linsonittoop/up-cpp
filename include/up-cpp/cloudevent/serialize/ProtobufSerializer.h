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

#ifndef CLOUDEVENT_PROTOBUF_SERIALAIZER_H
#define CLOUDEVENT_PROTOBUF_SERIALAIZER_H

#include <optional>
#include <spdlog/spdlog.h>
#include <cloudevents.pb.h>
#include "up-cpp/cloudevent/serialize/UCloudEventSerializer.h"
#include "up-cpp/cloudevent/validate/UCloudEventValidator.h"

using io::cloudevents::v1::CloudEvent;

namespace uprotocol::cloudevent {

    /**
     * Class that implements the Serializer interface for protobuf format.
     */
    class ProtobufSerializer : public UCloudEventSerializer {

    public:

        ProtobufSerializer() = default;

        /**
         * @brief Serialize a CloudEvent to a byte array
         *
         * @param cloudEvent - CloudEvent to serialize
         *
         * @return the serialized CloudEvent
         */
        [[nodiscard]] std::vector<uint8_t> serialize(const CloudEvent& cloudEvent) override {

            if (!UCloudEventValidator::is_valid_event(cloudEvent)) {
                spdlog::error("Invalid input CloudEvent");
                return std::vector<uint8_t>();
            }

            std::string str;
            if (!cloudEvent.SerializeToString(&str)) {
                spdlog::error("Failed to serialize CloudEvent");
                return std::vector<uint8_t>();
            }

            std::vector<uint8_t> data(str.begin(), str.end());
            return data;
        }

        /**
         * @brief Deserialize a byte array to a CloudEvent
         *
         * @param data - byte array to deserialize
         *
         * @return the deserialized CloudEvent
         */
        [[nodiscard]] std::optional<std::unique_ptr<CloudEvent>>
                      deserialize(const std::vector<uint8_t>& data) override {

            std::string str(data.begin(), data.end());
            if (data.empty()) {
                spdlog::error("Empty serialized data");
                return std::nullopt;
            }

            auto cloudEvent = std::make_unique<CloudEvent>();
            if (!cloudEvent->ParseFromString(str)) {
                spdlog::error("Failed to parse CloudEvent");
                return std::nullopt;
            }

            if (!UCloudEventValidator::is_valid_event(*cloudEvent)) {
                spdlog::error("Invalid deserialized CloudEvent");
                return std::nullopt;
            }

            return cloudEvent;
        }

    };

}  // namespace uprotocol::cloudevent

#endif  // CLOUDEVENT_PROTOBUF_SERIALAIZER_H
