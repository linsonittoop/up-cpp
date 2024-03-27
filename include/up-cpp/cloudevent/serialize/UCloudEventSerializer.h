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

#ifndef CLOUDEVENT_SERIALIZER_H
#define CLOUDEVENT_SERIALIZER_H

#include <optional>
#include <cloudevents.pb.h>
#include <up-core-api/uattributes.pb.h>

using io::cloudevents::v1::CloudEvent;
using io::cloudevents::v1::CloudEvent_CloudEventAttributeValue;

namespace uprotocol::cloudevent {

    /**
     * Intefrace class for Serializer, to convert between CloudEvent and Byte array.
     */
    class UCloudEventSerializer {

    public:

        UCloudEventSerializer() = default;

        virtual ~UCloudEventSerializer() = default;

        /**
         * @brief Serialize a CloudEvent to a byte array
         *
         * @param cloudEvent - CloudEvent to serialize
         *
         * @return the serialized CloudEvent
         */
        [[nodiscard]] virtual std::vector<uint8_t> serialize(const CloudEvent& cloudEvent) = 0;

        /**
         * @brief Deserialize a byte array to a CloudEvent
         *
         * @param data - byte array to deserialize
         *
         * @return the deserialized CloudEvent
         */
        [[nodiscard]] virtual std::optional<std::unique_ptr<CloudEvent>>
                      deserialize(const std::vector<uint8_t>& data) = 0;

    };

}  // namespace uprotocol::cloudevent

#endif  // CLOUDEVENT_SERIALIZER_H
