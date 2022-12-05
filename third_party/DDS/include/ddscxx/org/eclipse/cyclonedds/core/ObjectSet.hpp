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

#ifndef CYCLONEDDS_CORE_OBJECT_SET_HPP_
#define CYCLONEDDS_CORE_OBJECT_SET_HPP_

#include <dds/core/InstanceHandle.hpp>

#include <org/eclipse/cyclonedds/core/ObjectDelegate.hpp>
#include <org/eclipse/cyclonedds/core/Mutex.hpp>
#include <org/eclipse/cyclonedds/core/WeakReferenceSet.hpp>

#include <vector>
#include <memory>
#include <set>


namespace org
{
namespace eclipse
{
namespace cyclonedds
{
namespace core
{

class ObjectSet
{
public:
    typedef std::set<org::eclipse::cyclonedds::core::ObjectDelegate::weak_ref_type>::iterator    setIterator;
    typedef std::vector<org::eclipse::cyclonedds::core::ObjectDelegate::weak_ref_type>::iterator vectorIterator;
    typedef std::vector<org::eclipse::cyclonedds::core::ObjectDelegate::weak_ref_type>           vector;

    /**
     *  @internal Inserts a EntityDelegate into the set.
     * @param obj The org::eclipse::cyclonedds::core::ObjectDelegate to store
     */
    void insert(org::eclipse::cyclonedds::core::ObjectDelegate& obj);

    /**
     *  @internal Erases a EntityDelegate from the set.
     * @param obj The org::eclipse::cyclonedds::core::ObjectDelegate to delete
     */
    void erase(org::eclipse::cyclonedds::core::ObjectDelegate& obj);

    /**
     *  @internal Call close() of all the Objects within the set (outside lock).
     */
    void all_close();

    /**
     *  @internal Copy internal set into a vector.
     */
    vector copy();

private:
    WeakReferenceSet<ObjectDelegate::weak_ref_type>::wset objects;
    Mutex mutex;
};

}
}
}
}

#endif /* CYCLONEDDS_CORE_OBJECT_SET_HPP_ */
