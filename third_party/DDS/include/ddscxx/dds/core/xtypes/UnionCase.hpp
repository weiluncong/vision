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
/*
 * TUnionCase.hpp
 *
 *  Created on: Oct 24, 2012
 *      Author: angelo
 */

#ifndef  OMG_DDS_CORE_XTYPES_UNION_CASE_HPP_
#define  OMG_DDS_CORE_XTYPES_UNION_CASE_HPP_

#include <dds/core/Reference.hpp>
#include <dds/core/xtypes/MemberType.hpp>
#include <dds/core/xtypes/detail/UnionCase.hpp>

/** @cond */
namespace dds
{
namespace core
{
namespace xtypes
{
template <typename T, template <typename Q> class DELEGATE = detail::UnionCase>
class UnionCase;
}
}
}
/** @endcond */

template <typename T, template <typename Q> class DELEGATE>
class dds::core::xtypes::UnionCase : public dds::core::Reference< DELEGATE<T> >
{
public:
    // Nil case
    UnionCase();
public:
    UnionCase(T discriminator, const MemberType& member);
public:
    T discriminator();
    const MemberType& member();
};

#endif /* OMG_DDS_CORE_XTYPES_T_UNION_CASE_HPP_ */
