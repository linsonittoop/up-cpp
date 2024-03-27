/*
 * Copyright (c) 2024 General Motors GTO LLC
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

#ifndef CLOUDEVENT_VALIDATOR_H
#define CLOUDEVENT_VALIDATOR_H

#include <string>
#include <vector>
#include "up-cpp/cloudevent/datamodel/spec_version.h"
#include "up-cpp/cloudevent/datamodel/service_type.h"

#define likely(x) __builtin_expect(!!(x), 1)
#define unlikely(x) __builtin_expect(!!(x), 0)

namespace uprotocol::cloudevent {

class UCloudEventValidator {
    public:
        UCloudEventValidator() = default;
        ~UCloudEventValidator() = default;

        /**
         * Validate that all mandatory parameters exists
         * @param cloudEvent
         * @return
         */
        [[nodiscard]] static auto is_valid_event(const io::cloudevents::v1::CloudEvent& cloudEvent) {
            if (unlikely(!is_empty(cloudEvent))) {
              return false;
            }

            if (unlikely(!is_type_ok(cloudEvent))) {
              return false;
            }
            if (unlikely(!is_spec_version_ok(cloudEvent))) {
              return false;
            }

            auto e1 = UCloudEventValidator::mandatory_pram_validator_m[cloudEvent.type()];
            for (auto entry : e1) {
              if (!UCloudEventValidator::exists_attr(cloudEvent, entry)) {
                return false;
              }
            }
            return true;
        }


        inline static const std::string TTL_KEY = "ttl";
        inline static const std::string SINK_KEY = "sink";
        inline static const std::string DATA_SCHEMA_KEY = "dataschema";
        inline static const std::string DATA_CONTENT_TYPE_KEY = "datacontenttype";
        inline static const std::string REQ_ID_KEY = "reqid";
        inline static const std::string DATA_KEY = "data";
        inline static const std::string HASH_KEY = "hash";
        inline static const std::string PRIORITY_KEY = "priority";

        inline static const std::string CONTENT_TYPE = "application/x-protobuf";
        inline static const std::string PROTO_CONTENT_TYPE =
            "application/cloudevents+protobuf";
        inline static const std::string PROTO_DATA_CONTENT_TYPE =
            "application/protobuf";
        inline static const std::string JSON_CONTENT_TYPE =
            "application/cloudevents+json";

      private:
        struct attr_entry {
          std::string name;
          io::cloudevents::v1::CloudEvent_CloudEventAttributeValue::AttrCase type;
        };

        [[nodiscard]] static inline bool is_empty(
            const io::cloudevents::v1::CloudEvent& cloudEvent) {
          if (unlikely(cloudEvent.id().empty() || cloudEvent.source().empty() ||
                      cloudEvent.spec_version().empty() ||
                      cloudEvent.type().empty())) {
            spdlog::info("One of the mandatory header value is missing");
            return false;  // missing base mandatory
          }
          return true;
        }

        [[nodiscard]] static inline bool is_type_ok(
            const io::cloudevents::v1::CloudEvent& cloudEvent) {
            auto& type = cloudEvent.type();
                auto service_type = ServiceType::getMessageType(type);
            if (unlikely(service_type == uprotocol::v1::UMessageType::UMESSAGE_TYPE_UNSPECIFIED)) {
                spdlog::info("Service type not supported {0}\n", type.c_str());
                return false;
            }
            return true;
        }

        [[nodiscard]] static inline bool is_spec_version_ok(
            const io::cloudevents::v1::CloudEvent& cloudEvent) {
          auto& specVersion = cloudEvent.spec_version();
          auto specversion_E = SpecVersion::getSpecVersionE(specVersion);
          if (unlikely(specversion_E == SpecVersion::SpecVersion_E::NOT_DEFINED)) {
            spdlog::info("SpecVersion is not supported {0}\n", specVersion.c_str());
            return false;
          }
          return true;
        }

        [[nodiscard]] static inline bool exists_attr(
            const io::cloudevents::v1::CloudEvent& cloudEvent, attr_entry& entry) {
          auto res = (cloudEvent.attributes()).find(entry.name);
          if (res == (cloudEvent.attributes()).end()) {
            spdlog::info(
                "Required attribute {0} of type {1} for "
                "message {2} is missing\n",
                entry.name.c_str(), attr_case_string(entry.type).c_str(),
                cloudEvent.type().c_str());
            return false;
          }
          if (res->second.attr_case() != entry.type) {
            spdlog::info(
                "Required attribute {0} of type {1} for "
                "message "
                "{2}, type is set to {3}\n",
                entry.name.c_str(), attr_case_string(entry.type).c_str(),
                cloudEvent.type().c_str(),
                attr_case_string(res->second.attr_case()).c_str());
            return false;
          }
          return true;
        }

        inline static std::unordered_map<std::string, std::vector<attr_entry>>
            mandatory_pram_validator_m{
                {ServiceType::PUBLISH_MSG_TYPE_V1, {}},
                {ServiceType::FILE_MSG_TYPE_V1, {}},
                {ServiceType::REQUEST_MSG_TYPE_V1,
                {{UCloudEventValidator::TTL_KEY,
                  CloudEvent_CloudEventAttributeValue::AttrCase::kCeInteger},
                  {UCloudEventValidator::SINK_KEY,
                  CloudEvent_CloudEventAttributeValue::AttrCase::kCeString}}},
                {ServiceType::RESPONSE_MSG_TYPE_V1,
                {{UCloudEventValidator::TTL_KEY,
                  CloudEvent_CloudEventAttributeValue::AttrCase::kCeInteger},
                  {UCloudEventValidator::SINK_KEY,
                  CloudEvent_CloudEventAttributeValue::AttrCase::kCeString},
                  {UCloudEventValidator::DATA_KEY,
                  CloudEvent_CloudEventAttributeValue::AttrCase::kCeString},
                  {UCloudEventValidator::REQ_ID_KEY,
                  CloudEvent_CloudEventAttributeValue::AttrCase::kCeString},
                  {UCloudEventValidator::DATA_SCHEMA_KEY,
                  CloudEvent_CloudEventAttributeValue::AttrCase::kCeString}}}};

        [[nodiscard]] static std::string attr_case_string(
            const CloudEvent_CloudEventAttributeValue::AttrCase type) {
          switch (type) {
            case CloudEvent_CloudEventAttributeValue::AttrCase::kCeInteger:
              return "INTEGER";
            case CloudEvent_CloudEventAttributeValue::AttrCase::kCeBoolean:
              return "BOOLEAN";
            case CloudEvent_CloudEventAttributeValue::AttrCase::kCeString:
              return "STRING";
            case CloudEvent_CloudEventAttributeValue::AttrCase::kCeBytes:
              return "BYTES";
            case CloudEvent_CloudEventAttributeValue::AttrCase::kCeUri:
              return "URI";
            case CloudEvent_CloudEventAttributeValue::AttrCase::kCeUriRef:
              return "URI_REF";
            case CloudEvent_CloudEventAttributeValue::AttrCase::kCeTimestamp:
              return "TIMESTAMP";
            case CloudEvent_CloudEventAttributeValue::AttrCase::ATTR_NOT_SET:
              return "ATTR_NOT_SET";
          }
          return "ATTR_NOT_SET";
        }
    };


} // namespace uprotocol::cloudevent

#endif // CLOUDEVENT_VALIDATOR_H
