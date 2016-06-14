//===----------------------------------------------------------------------===//
//
//       Peloton
//
// map.h
//
// Identification: src/include/common/map.h
//
// Copyright (c) 2015-16, Carnegie Mellon University Database Group
//
//===----------------------------------------------------------------------===//

#pragma once

#include <type_traits>

#include "libcds/cds/init.h"
#include "libcds/cds/urcu/general_instant.h"
#include "libcds/cds/sync/pool_monitor.h"
#include "libcds/cds/memory/vyukov_queue_pool.h"
#include "libcds/cds/container/bronson_avltree_map_rcu.h"

namespace peloton {

// TREE_MAP_TEMPLATE_ARGUMENTS
#define TREE_MAP_TEMPLATE_ARGUMENTS template <typename KeyType, \
    typename ValueType>

// TREE_MAP_TYPE
#define TREE_MAP_TYPE TreeMap<KeyType, ValueType>

TREE_MAP_TEMPLATE_ARGUMENTS
class TreeMap {
 public:

  // Stores pointers to values
  typedef ValueType* MappedType;

  // rcu implementation
  typedef cds::urcu::general_instant<> RCUImpl;

  // rcu type
  typedef cds::urcu::gc<RCUImpl> RCUType;

  // find func mapped type
  typedef typename std::remove_pointer<MappedType>::type FuncMappedType;

  TreeMap();
  ~TreeMap();

  // Inserts a item into priority map.
  bool Insert(const KeyType &key, MappedType pVal);

  // Extracts item with high priority
  std::pair<bool, bool> Update(const KeyType &key, MappedType pVal, bool bInsert=true);

  // Delete key from the map
  bool Erase(const KeyType &key);

  // Extracts the corresponding value
  bool Find(const KeyType &key, ValueType& value);

  // Checks whether the map contains key
  bool Contains(const KeyType &key);

  // Clears the tree (thread safe, not atomic)
  void Clear();

  // Returns item count in the map
  size_t GetSize() const;

  // Checks if the map is empty
  bool IsEmpty() const;

  // Checks internal consistency (not atomic, not thread-safe)
  bool CheckConsistency() const;

 private:

  // concurrent AVL tree algorithm
  // http://libcds.sourceforge.net/doc/cds-api/
  typedef cds::container::BronsonAVLTreeMap<RCUType, KeyType, MappedType> avl_tree_t;

  avl_tree_t avl_tree;
};

}  // namespace peloton