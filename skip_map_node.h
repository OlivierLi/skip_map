#ifndef skip_map_node_h
#define skip_map_node_h

#include <memory>
#include <utility>
#include <array>
#include <cassert>

const size_t skip_list_size_k = 5;

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
  skip_map_node() : links{nullptr}, previous{nullptr}, entry{0, ""} {}

  /**
   * Constructor, sets the the entry member using the provided values.
   */
  skip_map_node(Key key, T value)
      : links{nullptr}, previous{nullptr}, entry{key, value} {}

  /**
   * Accessor to get the link pointer at the desired index.
   * @param[in] i The index of the link. Must be between 0 and
   * skip_list_size_k-1.
   */
  skip_map_node* link_at(size_t i) const {
    assert(i < skip_list_size_k - 1);
    return links[i];
  }

  /**
   * Accessor to set the link pointer at the desired index.
   * @param[in] i The index of the link. Must be between 0 and
   * skip_list_size_k-1.
   * @param[in] link The new value for the pointer.
   */
  void set_link(size_t i, skip_map_node* link) {
    assert(i < skip_list_size_k - 1);
    links[i] = link;
  }

  /**
   * A link to the previous node in the list. Provided so that the data
   * structures built on top of these nodes can be iterated over in reverse
   * order.
   */
  skip_map_node* previous;

  /**
   * The value contained within the node.
   */
  std::pair<const Key, T> entry;

 private:
  /**
   * The links used to go over the list. The link at index 0 is essentially the
   * same as the "next" pointer of a classic linked list.
   */
  std::array<skip_map_node*, skip_list_size_k> links;
};

#endif /* skip_map_node_h */
