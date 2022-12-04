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

#ifndef OMG_DDS_CORE_REF_TRAITS_H_
#define OMG_DDS_CORE_REF_TRAITS_H_

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


namespace  dds
{
namespace core
{

/** @cond
 * These traits must be provided by compliant implementations
 * to enable safe polymorphic casts.
 */

template <typename T1, typename T2>
struct is_base_of;

template <typename T1, typename T2>
struct is_same;

template <typename T>
struct smart_ptr_traits;

template <typename TO, typename FROM>
TO  polymorphic_cast(FROM& from);

/** @endcond */

}
} /* namespace dds / namespace core */


// This include should stay here as it provides implementations
// for the declaration immediately above.
#include <dds/core/detail/ref_traits.hpp>

#endif /* OMG_DDS_CORE_REF_TRAITS_H_ */
