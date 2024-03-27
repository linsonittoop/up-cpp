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

#ifndef CLOUDEVENT_JSON_SERIALIZER_H
#define CLOUDEVENT_JSON_SERIALIZER_H

#include <optional>
#include <spdlog/spdlog.h>
#include <cloudevents.pb.h>
#include <rapidjson/document.h>
#include "up-cpp/cloudevent/serialize/UCloudEventSerializer.h"
#include "up-cpp/cloudevent/validate/UCloudEventValidator.h"

using io::cloudevents::v1::CloudEvent;

namespace uprotocol::cloudevent {

    /**
     * Class that implements the Serializer interface for Json format.
     */
    class JsonSerializer : public UCloudEventSerializer {

    public:

        JsonSerializer() = default;

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

            //TODO: Missing serializer implementation
            return std::vector<uint8_t>();
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
            rapidjson::Document doc;
            doc.Parse(str.c_str());

            auto cloudEvent = std::make_unique<CloudEvent>();
            for (auto const& m : doc.GetObject()) {
                auto name = m.name.GetString();
                auto type = m.value.GetType();
                if (!(type == rapidjson::kStringType || type == rapidjson::kNumberType)) {
                    spdlog::error("Unsupported json type %s", ToString(type).c_str());
                    return std::nullopt;
                }

                if (strcmp("id", name) == 0) {
                    (*cloudEvent).set_id(m.value.GetString());
                } else if (strcmp("source", name) == 0) {
                    (*cloudEvent).set_source(m.value.GetString());
                } else if (strcmp("specversion", name) == 0) {
                    (*cloudEvent).set_spec_version(m.value.GetString());
                } else if (strcmp("type", name) == 0) {
                    (*cloudEvent).set_type(m.value.GetString());
                } else if (strcmp("data", name) == 0) {
                    (*cloudEvent).set_text_data(m.value.GetString());
                } else {  // all are attributes
                    if (type == rapidjson::kStringType) {
                        auto attr = std::make_unique<CloudEvent_CloudEventAttributeValue>();
                        auto str = std::string(m.value.GetString());
                        attr->set_ce_string(str);
                        (*cloudEvent->mutable_attributes())[name] = *attr;
                        attr.release();
                    } else {  // numeric
                        auto attr = std::make_unique<CloudEvent_CloudEventAttributeValue>();
                        attr->set_ce_integer(m.value.GetInt());
                        (*(*cloudEvent).mutable_attributes())[name] = *attr;
                        attr.release();
                    }
                }
            }

            if (!UCloudEventValidator::is_valid_event(*cloudEvent)) {
                spdlog::error("Invalid deserialized CloudEvent");
                return std::nullopt;
            }

            return cloudEvent;
        }

    private:

        static std::string ToString(rapidjson::Type type) {
            switch (type) {
                case rapidjson::kNullType: {
                    return "NULL-TYPE";
                }
                case rapidjson::kFalseType: {
                    return "False";
                }
                case rapidjson::kTrueType: {
                    return "True";
                }
                case rapidjson::kObjectType: {
                    return "Object";
                }
                case rapidjson::kArrayType: {
                    return "Array";
                }
                case rapidjson::kStringType: {
                    return "String";
                }
                case rapidjson::kNumberType: {
                    return "Number";
                }
                default: {
                    return "Not defined";
                }
            }
        }
    };

}  // namespace uprotocol::cloudevent

#endif  // CLOUDEVENT_JSON_SERIALIZER_H
