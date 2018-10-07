// Copyright (c) 2017 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef QUIC_CONTAINERS_H_
#define QUIC_CONTAINERS_H_
#include<unordered_set>
#include<unordered_map>
#include<utility>
// TODO(mpw): s/std::unordered_map/gtl::node_hash_map/ once node_hash_map is
//   PG3-compatible.
template <typename Key,
          typename Value,
          typename Hash = typename std::unordered_map<Key, Value>::hasher,
          typename Eq = typename std::unordered_map<Key, Value>::key_equal,
          typename Alloc =
              typename std::unordered_map<Key, Value>::allocator_type>
using QuicUnorderedMapImpl = std::unordered_map<Key, Value, Hash, Eq, Alloc>;

// TODO(mpw): s/std::unordered_set/gtl::node_hash_set/ once node_hash_set is
//   PG3-compatible.
template <typename Key,
          typename Hash = typename std::unordered_set<Key>::hasher,
          typename Eq = typename std::unordered_set<Key>::key_equal,
          typename Alloc = typename std::unordered_set<Key>::allocator_type>
using QuicUnorderedSetImpl = std::unordered_set<Key, Hash, Eq, Alloc>;
// A general-purpose unordered map.
template <typename Key,
          typename Value,
          typename Hash = typename QuicUnorderedMapImpl<Key, Value>::hasher,
          typename Eq = typename QuicUnorderedMapImpl<Key, Value>::key_equal,
          typename Alloc =
              typename QuicUnorderedMapImpl<Key, Value>::allocator_type>
using QuicUnorderedMap = QuicUnorderedMapImpl<Key, Value, Hash, Eq, Alloc>;

// A general-purpose unordered set.
template <typename Key,
          typename Hash = typename QuicUnorderedSetImpl<Key>::hasher,
          typename Eq = typename QuicUnorderedSetImpl<Key>::key_equal,
          typename Alloc = typename QuicUnorderedSetImpl<Key>::allocator_type>
using QuicUnorderedSet = QuicUnorderedSetImpl<Key, Hash, Eq, Alloc>;

#endif
