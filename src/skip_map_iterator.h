#ifndef skip_map_iterator_h
#define skip_map_iterator_h

#include <gtest/gtest_prod.h>

template <typename Key, typename Value, bool is_const,
          typename value_type = typename std::conditional<
              is_const, const std::pair<const Key, Value>,
              std::pair<const Key, Value>>::type>
class skip_map_iterator
    : public std::iterator<std::forward_iterator_tag, value_type> {
 public:
  using difference_type = std::ptrdiff_t;
  using node_pointer_type =
      typename std::conditional<is_const, const skip_map_node<Key, Value>*,
                                skip_map_node<Key, Value>*>::type;


  skip_map_iterator():level_(0), node(nullptr) {}

  skip_map_iterator(skip_map_node<Key, Value>* p, size_t level = 0)
      :level_(level), node(p) {}
  skip_map_iterator(const skip_map_iterator<Key, Value, false>& other)
      : level_(other.level_), node(other.node) {}

  skip_map_iterator& operator++() {
    // Do not iterate into nullptr
    if(node->link_at(level_)){
      node = node->link_at(level_);
    }
    return *this;
  }
  skip_map_iterator operator++(int) {
    skip_map_iterator tmp(*this);
    operator++();
    return tmp;
  }

  skip_map_iterator operator+(difference_type n) const {
    skip_map_iterator temp(*this);
    for (difference_type i = 0; temp.node && i < n; ++i) {
      ++temp;
    }
    return temp;
  }

  skip_map_iterator& operator+=(difference_type n) {
    skip_map_iterator temp = this->operator+(n);
    *this = temp;
    return *this;
  }

  bool operator==(const skip_map_iterator& rhs) const {
    return node == rhs.node;
  }
  bool operator!=(const skip_map_iterator& rhs) const {
    return node != rhs.node;
  }

  value_type& operator*() { return node->entry; }
  value_type* operator->() { return &node->entry; }

  void go_down() {
    --level_;
  }

  void go_up() {
    ++level_;
  }

  node_pointer_type get() const { return node; }

  // Friend classes only for unit tests
  friend class ConstructedTest;
  FRIEND_TEST(ConstructedTest, splice);

  size_t level_;
private :
  node_pointer_type node;
};

#endif /* skip_map_iterator_h */
