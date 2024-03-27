/*
 * Copyright (c) 2023 General Motors GTO LLC
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

#ifndef COULDEVENT_ATTRIBUTES_H
#define COULDEVENT_ATTRIBUTES_H

#include <iostream>
#include <optional>
#include <utility>
#include <up-core-api/uattributes.pb.h>

using namespace uprotocol::v1;

namespace uprotocol::cloudevent {

    class UCloudEventAttributes {

    public:

        /**
         * @brief Default constructor for UCloudEventAttributes.
         */
        UCloudEventAttributes() {};

        /**
         * @brief Builder for constructing UCloudEventAttributes.
         */
        class Builder {

            friend class UCloudEventAttributes;

        public:

            /**
             * @brief Constructor for Builder.
             */
            Builder() {}

            /**
             * @brief Set hash in the attributes.
             *
             * @param hash - The hash to set.
             *
             * @return The Builder configured with hash.
             */
            Builder& withHash(const std::string_view& hash) {
                hash_ = hash;
                return *this;
            }

            /**
             * @brief Set priority in the attributes.
             *
             * @param priority - The priority to set.
             *
             * @return The Builder configured with priority.
             */
            Builder& withPriority(const UPriority priority) {
                priority_ = priority;
                return *this;
            }

            /**
             * @brief Set ttl in the attributes.
             *
             * @param ttl - The ttl to set.
             *
             * @return The Builder configured with ttl.
             */
            Builder& withTtl(const uint32_t ttl) {
                ttl_ = ttl;
                return *this;
            }

            /**
             * @brief Set token in the attributes.
             *
             * @param token - The token to set.
             *
             * @return The Builder configured with token.
             */
            Builder& withToken(const std::string_view token) {
                token_ = token;
                return *this;
            }

            /**
             * @brief Set traceparent in the attributes.
             *
             * @param traceparent - The traceparent to set.
             *
             * @return The Builder configured with traceparent.
             */
            Builder& withTraceparent(const std::string_view traceparent) {
                traceparent_ = traceparent;
                return *this;
            }

            /**
             * @brief Build the UCloudEventAttributes.
             *
             * @return The UCloudEventAttributes.
             */
            [[nodiscard]] UCloudEventAttributes build() {
                return UCloudEventAttributes(*this);
            }

        private:

            std::string hash_ = {""};
            UPriority priority_ = {UPriority::UPRIORITY_UNSPECIFIED};
            uint32_t ttl_ = {0};
            std::string token_ = {""};
            std::string traceparent_ = {""};

        };

        /**
         * @brief Check if the UCloudEventAttributes is empty.
         *
         * @return True if the UCloudEventAttributes is empty, false otherwise.
         */
        [[nodiscard]] bool isEmpty() const {
            if (hash_.empty() && (priority_ == UPriority::UPRIORITY_UNSPECIFIED) &&
                ttl_ == 0 && token_.empty() && traceparent_.empty()) {
                return true;
            }
            return false;
        }

        /**
         * @brief Get the hash.
         *
         * @return The hash.
         */
        [[nodiscard]] std::string hash() const { return hash_; }

        /**
         * @brief Get the priority.
         *
         * @return The priority.
         */
        [[nodiscard]] UPriority priority() const { return priority_; }

        /**
         * @brief Get the ttl.
         *
         * @return The ttl.
         */
        [[nodiscard]] uint32_t ttl() const { return ttl_; }

        /**
         * @brief Get the token.
         *
         * @return The token.
         */
        [[nodiscard]] std::string token() const { return token_; }

        /**
         * @brief Get the traceparent.
         *
         * @return The traceparent.
         */
        [[nodiscard]] std::string traceparent() const { return traceparent_; }

        /**
         * @brief Override the equality operator.
         *
         * @param rhs - The right hand side of the operator.
         *
         * @return True if the objects are equal, false otherwise.
         */
        [[nodiscard]] bool operator==(const UCloudEventAttributes& rhs) const {
            if (this == &rhs) {
                return true;
            }

            return (hash_ == rhs.hash_) &&
                   (priority_ == rhs.priority_) &&
                   (ttl_ == rhs.ttl_) &&
                   (token_ == rhs.token_) &&
                   (traceparent_ == rhs.traceparent_);
        }

        /**
         * @brief String representation of the UCloudEventAttributes object.
         *
         * @return The string representation.
         */
        [[nodiscard]] std::string toString() const {
            return "UCloudEventAttributes{hash=" + hash_ +
                   ", priority=" + std::to_string(priority_) +
                   ", ttl=" + std::to_string(ttl_) +
                   ", token=" + token_ +
                   ", traceparent=" + traceparent_ + "}";
        }


    private:

        /**
         * @brief Constructor with Builder.
         *
         * @param builder - The Builder to use.
         *
         * @return The UCloudEventAttributes.
         */
        explicit UCloudEventAttributes(const UCloudEventAttributes::Builder& builder) :
            hash_(builder.hash_),
            priority_(builder.priority_),
            ttl_(builder.ttl_),
            token_(builder.token_),
            traceparent_(builder.traceparent_) {}

        /**
         * @brief hash attribute of CloudEvent
         */
        std::string hash_ = {""};
        /**
         * @brief priority attribute of CloudEvent
         */
        UPriority priority_ = {UPriority::UPRIORITY_UNSPECIFIED};
        /**
         * @brief ttl attribute of CloudEvent
         */
        uint32_t ttl_ = {0};
        /**
         * @brief token attribute of CloudEvent
         */
        std::string token_ = {""};
        /**
         * @brief traceparent attribute of CloudEvent
         */
        std::string traceparent_ = {""};

    };

}   // namespace uprotocol::cloudevent

#endif  // COULDEVENT_ATTRIBUTES_H
