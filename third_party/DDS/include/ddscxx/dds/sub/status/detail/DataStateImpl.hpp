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

/*
 * Copyright(c) 2006 to 2020 ADLINK Technology Limited and others
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License v. 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0, or the Eclipse Distribution License
 * v. 1.0 which is available at
 * http://www.eclipse.org/org/documents/edl-v10.php.
 *
 * SPDX-License-Identifier: EPL-2.0 OR BSD-3-Clause
 */
#ifndef CYCLONEDDS_DDS_SUB_STATUS_DATASTATE_IMPL_HPP_
#define CYCLONEDDS_DDS_SUB_STATUS_DATASTATE_IMPL_HPP_

/**
 * @file
 */

#include <dds/sub/status/DataState.hpp>

/*
 * OMG PSM class declaration
 */

// Implementation

namespace dds
{
namespace sub
{
namespace status
{


inline const SampleState SampleState::read()
{
    return SampleState(0x0001 << 0u);
}

inline const SampleState SampleState::not_read()
{
    return SampleState(0x0001 << 1u);
}

inline const SampleState SampleState::any()
{
    return SampleState(0xffff);
}

inline const ViewState ViewState::new_view()
{
    return ViewState(0x0001 << 0u);
}

inline const ViewState ViewState::not_new_view()
{
    return ViewState(0x0001 << 1u);
}

inline const ViewState ViewState::any()
{
    return ViewState(0xffff);
}

inline const InstanceState InstanceState::alive()
{
    return InstanceState(0x0001 << 0u);
}

inline const InstanceState InstanceState::not_alive_disposed()
{
    return InstanceState(0x0001 << 1u);
}

inline const InstanceState InstanceState::not_alive_no_writers()
{
    return InstanceState(0x0001 << 2u);
}

inline const InstanceState InstanceState::not_alive_mask()
{
    return not_alive_disposed() | not_alive_no_writers();
}

inline const InstanceState InstanceState::any()
{
    return InstanceState(0xffff);
}


} /* namespace status */
} /* namespace sub */
} /* namespace dds */

// End of implementation

#endif /* CYCLONEDDS_DDS_SUB_STATUS_DATASTATE_IMPL_HPP_ */
