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

#ifndef CYCLONEDDS_CORE_TYPE_HELPERS_HPP_
#define CYCLONEDDS_CORE_TYPE_HELPERS_HPP_

#include <type_traits>

//for c++ < 14
#if __cplusplus == 201103L
#include <org/eclipse/cyclonedds/core/Missing.hpp>
#endif

//check template for whether a class is a template specification of another
template<typename Test, template<typename...> class Ref>
struct is_specialization : std::false_type {};

template<template<typename...> class Ref, typename... Args>
struct is_specialization<Ref<Args...>, Ref> : std::true_type {};

//check template for whether a class is an STL container
template<typename T, typename _ = void>
struct is_container : std::false_type {};

template<typename... Ts>
struct is_container_helper {};

template<typename T>
struct is_container<
  T,
  std::conditional_t<
  false,
  is_container_helper<
  typename T::value_type,
  typename T::size_type,
  typename T::allocator_type,
  typename T::iterator,
  typename T::const_iterator,
  decltype(std::declval<T>().size()),
  decltype(std::declval<T>().begin()),
  decltype(std::declval<T>().end()),
  decltype(std::declval<T>().cbegin()),
  decltype(std::declval<T>().cend())
  >,
  void
  >
> : public std::true_type{};

#endif
