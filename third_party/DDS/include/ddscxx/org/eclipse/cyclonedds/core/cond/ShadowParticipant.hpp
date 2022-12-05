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

/**
 * @file
 */

#ifndef CYCLONEDDS_CORE_COND_SHADOW_PARTICIPANT_HPP_
#define CYCLONEDDS_CORE_COND_SHADOW_PARTICIPANT_HPP_

#include "dds/dds.h"

#include <dds/domain/DomainParticipant.hpp>
#include <org/eclipse/cyclonedds/core/config.hpp>
#include <org/eclipse/cyclonedds/core/Mutex.hpp>
#include <org/eclipse/cyclonedds/core/ScopedLock.hpp>

namespace org
{
namespace eclipse
{
namespace cyclonedds
{
namespace core
{
namespace cond
{
    class ShadowParticipant {
    public:
        static ShadowParticipant& getInstance()
        {
            org::eclipse::cyclonedds::core::ScopedMutexLock scopedLock(mutex_);

            if (!instance_) {
                // Create shadow participant that is used as parent entity for all waitsets
                dds_entity_t ddsc_part = dds_create_participant(DDS_DOMAIN_DEFAULT, NULL, NULL);
                instance_ = new ShadowParticipant(ddsc_part);
            }
            return *instance_;
        }

        dds_entity_t get_participant()
        {
            return ddsc_participant_;
        }

    private:
        ShadowParticipant(dds_entity_t ddsc_participant)
        {
            this->ddsc_participant_ = ddsc_participant;
        }

        ~ShadowParticipant()
        {
            dds_delete(this->ddsc_participant_);
        }
        ShadowParticipant(const ShadowParticipant&);
        ShadowParticipant& operator=(const ShadowParticipant&);

        static ShadowParticipant* instance_;
        static org::eclipse::cyclonedds::core::Mutex mutex_;
        dds_entity_t ddsc_participant_;
    };

}
}
}
}
}


#endif /* CYCLONEDDS_CORE_COND_SHADOW_PARTICIPANT_HPP_ */
