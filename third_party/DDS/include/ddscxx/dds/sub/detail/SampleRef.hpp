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

#ifndef OMG_DDS_PUB_DETAIL_SAMPLEREF_HPP_
#define OMG_DDS_PUB_DETAIL_SAMPLEREF_HPP_

/* Copyright 2010, Object Management Group, Inc.
 * Copyright 2010, PrismTech, Corp.
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


namespace dds
{
namespace sub
{
namespace detail
{
template <typename T>
class SampleRef;
}}}

#include <dds/sub/SampleInfo.hpp>
#include <org/eclipse/cyclonedds/topic/datatopic.hpp>

namespace dds
{
namespace sub
{
namespace detail
{
template <typename T>
class SampleRef
{
public:
    SampleRef()
    {
      this->data_ = nullptr;
    }

    SampleRef(ddscxx_serdata<T>* d, const dds::sub::SampleInfo& i)
    {
        this->data_ = d;
        this->info_ = i;
    }

    SampleRef(const SampleRef& other)
    {
        copy(other);
    }

    virtual ~SampleRef()
    {
      if (data_ != nullptr) {
        ddsi_serdata_unref(reinterpret_cast<ddsi_serdata *>(data_));
      }
    }

    SampleRef& operator=(const SampleRef& other)
    {
      if (this != &other)
      {
          copy(other);
      }
      return *this;
    }

public:
    const T& data() const
    {
      if (data_ == nullptr)
      {
        throw dds::core::Error("Data is Null");
      }
      return *data_->getT();
    }

    const dds::sub::SampleInfo& info() const
    {
        return info_;
    }

    dds::sub::SampleInfo& info()
    {
        return info_;
    }

    bool operator ==(const SampleRef& other) const
    {
        (void)other;
        return false;
    }

    ddscxx_serdata<T>* &data_ptr()
    {
        return this->data_;
    }


private:
    void copy(const SampleRef& other)
    {
        if (other.data_ == nullptr)
        {
            throw dds::core::Error("Other data is Null");
        }
        static_cast<void>(ddsi_serdata_ref(reinterpret_cast<ddsi_serdata* const>(other.data_)));
        this->data_ = other.data_;
        this->info_ = other.info_;
    }

    ddscxx_serdata<T>* data_;
    dds::sub::SampleInfo info_;
};

}
}
}

#endif /* OMG_DDS_PUB_DETAIL_SAMPLEREF_HPP_ */
