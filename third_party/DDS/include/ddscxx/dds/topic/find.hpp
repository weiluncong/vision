/*
 *  Copyright(c) 2021 to 2023 AutoCore Technology (Nanjing) Co., Ltd. All rights reserved.
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this list of
 *    conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list
 *    of conditions and the following disclaimer in the documentation and/or other materials
 *    provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its contributors may be used
 *    to endorse or promote products derived from this software without specific prior written
 *    permission.
 */

#ifndef OMG_DDS_TOPIC_FIND_HPP_
#define OMG_DDS_TOPIC_FIND_HPP_

/* Copyright 2010, Object Management Group, Inc.
 * Copyright 2010, PrismTech, Inc.
 * Copyright 2010, Real-Time Innovations, Inc.
 * All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <string>

#include <dds/topic/detail/find.hpp>

namespace dds
{
namespace topic
{

/**
 * This operation gives access to a locally-created Topic, ContentFilteredTopic,
 * MultiTopic, AnyTopic and TopicDescription with a matching name.
 *
 * dds::topic::find will never create a Topic but returns a reference to an
 * Topic proxy that already exists locally. When no local proxy exists, the
 * returned Topic will be a dds::core::null object. The operation never blocks.
 *
 * For discovering globally available Topics, look
 * @link dds::topic::discover(const dds::domain::DomainParticipant&, const std::string&, const dds::core::Duration&)
 * here.@endlink
 *
 * @param dp the DomainParticipant to find the topic on
 * @param topic_name the topic name to find
 */
template <typename TOPIC>
TOPIC
find(const dds::domain::DomainParticipant& dp, const std::string& topic_name);

}
}

#endif /* OMG_DDS_TOPIC_FIND_HPP_ */
