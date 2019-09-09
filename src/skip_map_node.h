#ifndef skip_map_node_h
#define skip_map_node_h

#include <array>
#include <cassert>
#include <memory>
#include <utility>
#include "fixed_vector.hpp"

constexpr size_t MAX_SIZE{4};

/**
 * The class that represents a node in the skip list. This class provides the
 * data representation but no logic. The logic is to be implemented in the
 * iterator and container classes built on top of it.
 */
template <class Key, class T>
class skip_map_node {
 public:
  /**
   * The default constructor, only initilializes member variables
   */
  skip_map_node() : entry{Key(), T()}, links{} {}

  /**
   * Constructor, sets the the entry member using the provided values.
   */
  skip_map_node(Key key, T value) : entry{key, value}, links{} {}

  /**
   * Accessor to get the link pointer at the desired index.
   * Accessing levels out of range result in nullptr
   * @param[in] i The index of the link.
   */
  skip_map_node* link_at(size_t i) const {
    if (links.empty() || i > links.size() - 1) {
      return nullptr;
    }

    return links.at(i);
  }

  /**
   * Accessor to set the link pointer at the desired index.
   * The link vector is grown to accomodate insertions
   * @param[in] link The new value for the pointer.
   */
  void set_link(size_t i, skip_map_node* link) {
    if (links.size() < i + 1) {
      links.resize(i + 1);
    }

    links.at(i) = link;
  }

  /**
   * The value contained within the node.
   */
  std::pair<const Key, T> entry;

 private:
  /**
   * The links used to go over the list. The link at index 0 is essentially the
   * same as the "next" pointer of a classic linked list.
   */
  fixed_vector<skip_map_node*, MAX_SIZE> links;
  FRIEND_TEST(insert, case1);
};

#endif /* skip_map_node_h */
