#ifndef skip_map_h
#define skip_map_h

#include <gtest/gtest_prod.h>
#include <array>
#include <exception>
#include <functional>
#include <limits>
#include <random>
#include <stdexcept>
#include "skip_map_iterator.h"
#include "skip_map_node.h"

/**
 * skip_map is a sorted associative container that contains key-value pairs with
 * unique keys. Keys are sorted by using the comparison function Compare.
 * Search, removal, and insertion operations have expected logarithmic
 * complexity. skip_map is implemented using a skip_list.
 */
template <class Key,
          class T,
          class Compare = std::less<Key>,
          class Allocator = std::allocator<skip_map_node<Key, T>>>
class skip_map {
 public:
  using key_type = Key;
  using mapped_type = T;
  using value_type = std::pair<const Key, T>;
  using size_type = std::size_t;
  using difference_type = std::ptrdiff_t;
  using key_compare = Compare;
  using allocator_type = Allocator;
  using reference = value_type&;
  using const_reference = const value_type&;
  using pointer = typename std::allocator_traits<Allocator>::pointer;
  using const_pointer =
      typename std::allocator_traits<Allocator>::const_pointer;

  using iterator = skip_map_iterator<Key, T, false>;
  using const_iterator = skip_map_iterator<Key, T, true>;

  using splice_t = fixed_vector<iterator, MAX_SIZE>;
  using const_splice_t = fixed_vector<const_iterator, MAX_SIZE>;

  using node_type = skip_map_node<Key, T>;

  /**
   * Default constructor
   */
  skip_map()
      : rend_(allocate_and_init()), end_(allocate_and_init()), max_level_(0) {
    rend_->set_link(0, end_);
  }

  /**
   * Copy constructor, performs a deep copy of the data in rhs
   */
  skip_map(const skip_map& rhs) : skip_map() {
    // TODO : Use range insert
    for (const auto& key_value : rhs) {
      insert(key_value);
    }
    max_level_ = rhs.max_level_;
  }

  /**
   * Move constructor, steals all pointers of rhs. Leave rhs is a state that is
   * assignable and destructible by setting its pointer to null
   */
  skip_map(skip_map&& rhs)
      : rend_(rhs.rend_), end_(rhs.end_), max_level_(rhs.max_level_) {
    rhs.rend_ = nullptr;
    rhs.end_ = nullptr;
  }

  /**
   * Copy assignement operator. Uses the copy and swap idiom. rhs is received by
   * copy which takes care of performing a deep copy of the data. This also
   * saves us from defining a move assignement operator since rhs will be move
   * constructed when the operator is invoked with an r-value.
   */
  skip_map& operator=(skip_map rhs) {
    swap(rhs);
    return *this;
  }

  /**
   * Clears all data to relase ressources
   */
  ~skip_map() {
    clear();
    destroy_and_release(rend_);
    destroy_and_release(end_);
  }

  /**
   * Returns a reference to the mapped value of the element with key equivalent
   * to key.
   */
  T& at(const Key& key) {
    auto it = find(key);

    if (it != end()) {
      return it->second;
    } else {
      throw std::out_of_range("Key not found!");
    }
  }

  /**
   * const overload of at()
   */
  const T& at(const Key& key) const { return const_this().at(key); }

  const skip_map& const_this() { return static_cast<const skip_map&>(*this); }

  /**
   * Finds an element with key equivalent to key.
   * Uses casting to avoid duplicated code. Safe since calls of this function
   * have to come from a non-const object (Effective c++ item 3)
   */
  iterator find(const Key& key) {
    const_iterator it = const_this().find(key);
    node_type* ptr = const_cast<node_type*>(it.get());
    return iterator(ptr);
  }

  /**
   * const overload of find()
   */
  const_iterator find(const Key& key) const {
    auto lower_bound_it = lower_bound(key);
    if (!key_comparator_(key, lower_bound_it->first) &&
        lower_bound_it != end()) {
      return lower_bound_it;
    } else {
      return end();
    }
  }

  /**
   * Returns a reference to the value that is mapped to a key equivalent to key,
   * performing an insertion if such key does not already exist.
   */
  T& operator[](const Key& key) {
    auto it = find(key);
    if (it != end()) {
      return it->second;
    } else {
      // TODO : Would std::map avoid allocating a string and then copying?
      return insert(value_type{key, T()}).first->second;
    }
  }

  /**
   * Returns an iterator to the first element of the container.
   * If the container is empty, the returned iterator will be equal to end().
   */
  iterator begin() noexcept { return std::next(iterator(rend_)); }

  /**
   * const overload of begin()
   */
  const_iterator begin() const noexcept {
    return std::next(const_iterator(rend_));
  }

  /**
   * explicitally const version of begin()
   */
  const_iterator cbegin() const noexcept { return begin(); }

  /**
   * Returns an iterator to the element following the last element of the
   * container. This element acts as a placeholder. Attempting to access it
   * results in undefined behavior.
   */
  iterator end() noexcept { return iterator(end_); }

  /**
   * const overload of end()
   */
  const_iterator end() const noexcept { return const_iterator(end_); }

  /**
   * explicitally const verstion of end()
   */
  const_iterator cend() const noexcept { return end(); }

  /**
   * Checks if the container has no elements
   */
  bool empty() const noexcept { return begin() == end(); }

  /**
   * Returns the number of elements in the container
   */
  size_type size() const noexcept { return std::distance(begin(), end()); }

  /**
   * Returns the maximum number of elements the container is able to hold.
   */
  size_type max_size() const { return std::numeric_limits<size_type>::max(); }

  /**
   * Removes all elements from the container. This particular implementation
   * does not invalidate past-the-end iterators
   */
  void clear() noexcept {
    // If we have a null first pointer it's because the contents were moved out
    // There is no destruction or dealocating to do
    if (!rend_) {
      return;
    }

    // If we have nothing to delete
    if (empty()) {
      return;
    }

    // Remove the nodes one by one until we are left with the end
    for (auto it = begin(); it != end(); it = erase(it))
      ;
  }

  /**
   * Inserts element(s) into the container, if the container doesn't already
   * contain an element with an equivalent key.
   */
  std::pair<iterator, bool> insert(value_type value) {
    auto current = lower_bound(value.first);
    if (!key_comparator_(value.first, current->first) && current != end()) {
      return std::make_pair(current, false);
    }

    size_t node_level = gen();

    // Handle the case where the new node increase the max level
    if (node_level > max_level_) {
      // Add the necessary links to rend_
      for (size_t i = max_level_; i <= node_level; ++i) {
        rend_->set_link(i, end_);
      }

      // Increase the max level
      max_level_ = node_level;
    }

    // Create new node
    auto new_node =
        allocate_and_init(std::move(value.first), std::move(value.second));
    const auto& splice_vec = splice(value.first);
    for (auto it : splice_vec) {
      new_node->set_link(it.level_, (it + 1).get());
      it.get()->set_link(it.level_, new_node);
    }

    return std::make_pair(iterator(new_node), true);
  }

  /**
   *
   */
  iterator insert(const_iterator /*hint*/, const value_type& /*value*/) {
    throw std::runtime_error("Unimplemented!");
  }

  /**
   *
   */
  iterator insert(const_iterator /*hint*/, value_type&& /*value*/) {
    throw std::runtime_error("Unimplemented!");
  }

  /**
   *
   */
  template <class InputIt>
  void insert(InputIt /*first*/, InputIt /*last*/) {
    // TODO : Implement with hint to avoid n*log(n) complexity
    throw std::runtime_error("Unimplemented!");
  }

  /**
   *
   */
  void insert(std::initializer_list<value_type> /*ilist*/) {
    // TODO : Implement with hint to avoid n*log(n) complexity
    throw std::runtime_error("Unimplemented!");
  }

  /**
   *
   */
  template <class... Args>
  std::pair<iterator, bool> emplace(Args&&... /*args*/) {
    throw std::runtime_error("Unimplemented!");
  }

  /**
   *
   */
  template <class... Args>
  iterator emplace_hint(const_iterator /*hint*/, Args&&... /*args*/) {
    throw std::runtime_error("Unimplemented!");
  }

  /**
   * Removes specified elements from the container.
   */
  iterator erase(iterator pos) {
    // Don't delete past the end, past the beginning nodes
    if (pos == iterator(rend_) || pos == end()) {
      return end();
    }

    // Set all previous links to skip the node we are about to delete
    const auto& splice_vec = splice(pos->first);
    for (const auto& it : splice_vec) {
      it.get()->set_link(it.level_, (it + 2).get());
    }

    destroy_and_release(pos.get());

    return splice_vec.back() + 1;
  }

  /**
   *
   */
  void erase(iterator /*first*/, iterator /*last*/) {
    throw std::runtime_error("Unimplemented!");
  }

  /**
   *
   */
  size_type erase(const key_type& key) {
    auto it = find(key);
    if (it == end()) {
      return size();
    }
    erase(it);

    return size();
  }

  /**
   *
   */
  void swap(skip_map& other) {
    std::swap(rend_, other.rend_);
    std::swap(end_, other.end_);
  }

  /**
   *
   */
  size_type count(const Key& key) const {
    if (find(key) == end()) {
      return 0;
    }
    return 1;
  }

  /**
   * Returns a pair of iterators obtained with lower_bound() and upper_bound()
   */
  std::pair<iterator, iterator> equal_range(const Key& key) {
    return std::make_pair(lower_bound(key), upper_bound(key));
  }

  /**
   * const overload of equal_range()
   */
  std::pair<const_iterator, const_iterator> equal_range(const Key& key) const {
    return std::make_pair(lower_bound(key), upper_bound(key));
  }

  /**
   * Returns an iterator pointing to the first element that is not less than
   * key. Uses casting to avoid duplicated code. Safe since calls of this
   * function have to come from a non-const object (Effective c++ item 3)
   */
  iterator lower_bound(const Key& key) {
    const_iterator it = const_this().lower_bound(key);
    node_type* ptr = const_cast<node_type*>(it.get());
    return iterator(ptr);
  }

  /**
   * const overload of lower_bound()
   */
  const_iterator lower_bound(const Key& key) const {
    const auto& splice_vec = splice(key);
    return splice_vec.back() + 1;
  }

  /**
   * Returns an iterator pointing to the first element that is greater than key.
   * Uses casting to avoid duplicated code. Safe since calls of this function
   * have to come from a non-const object (Effective c++ item 3)
   */
  iterator upper_bound(const Key& key) {
    const_iterator it = const_this().upper_bound(key);
    node_type* ptr = const_cast<node_type*>(it.get());
    return iterator(ptr);
  }

  /**
   * const overload of upper_bound()
   */
  const_iterator upper_bound(const Key& key) const {
    auto it = lower_bound(key);

    if (it == end()) {
      return it;
    }

    return it + 1;
  }

  /**
   *
   */
  Allocator get_allocator() const { return allocator_; }

 private:
  /**
   * Return lower bound of each level by key.
   */
  splice_t splice(const Key& key) {
    // First get the const_iterators by invoking the const version of splice
    const const_splice_t& const_lower_bounds = const_this().splice(key);

    // Convert const iterators into iterators
    splice_t lower_bounds;
    for (const auto& const_bound : const_lower_bounds) {
      lower_bounds.emplace_back(const_cast<node_type*>(const_bound.get()),
                                const_bound.level_);
    }

    return lower_bounds;
  }

  /**
   * const overload of splice()
   */
  const_splice_t splice(const Key& key) const {
    const_splice_t lower_bounds;

    // Start at the top level and go down every level
    const_iterator start(rend_, max_level_);
    for (size_t i = 0; i <= max_level_; ++i) {
      // Access the fist non terminal node on the level
      auto temp = start;

      // Advance as far as we can without reaching the end or going over
      while (temp + 1 != end() && key_comparator_((temp + 1)->first, key)) {
        ++temp;
      }

      lower_bounds.emplace_back(temp);

      // The seach will start again from the last found node
      start = temp;

      start.go_down();
    }

    return lower_bounds;
  }

  /**
   * Convenience function to allocate and initialize memory in the same call
   */
  template <typename... Args>
  node_type* allocate_and_init(Args&&... arguments) {
    auto ptr(allocator_.allocate(sizeof(node_type)));
    allocator_.construct(ptr, std::forward<Args>(arguments)...);
    return ptr;
  }

  /**
   * Convenience function to destroy a node object and deallocate in the same
   * call.
   */
  void destroy_and_release(node_type* ptr) {
    if (ptr) {
      allocator_.destroy(ptr);
      allocator_.deallocate(ptr, sizeof(node_type));
    }
  }

  /**
   * Private instance of the Allocator type used to allocate and initialize
   * nodes
   */
  Allocator allocator_;

  /**
   * Pointer to the element preceding the first element.
   */
  node_type* rend_;

  /**
   * Pointer to the element following the last element.
   */
  node_type* end_;

  /**
   * The current highest level of any node
   */
  size_type max_level_;

  /**
   * Instance of Compare used to compare keys
   */
  key_compare key_comparator_;

  /**
   * Random number generator that determins the level of an inserted node
   */
  std::function<int()> gen =
      std::bind(std::uniform_int_distribution<>{1, MAX_SIZE - 1},
                std::default_random_engine{});

  // Define friend classes only for unit testing purposes
  friend class ConstructedTest;
  FRIEND_TEST(ConstructedTest, iterate);
  FRIEND_TEST(ConstructedTest, splice);
};

/**
 * Checks if the contents of lhs and rhs are equal, that is, whether lhs.size()
 * == rhs.size() and each element in lhs compares equal with the element in rhs
 * at the same position.
 */
template <class Key, class T, class Compare, class Alloc>
bool operator==(const skip_map<Key, T, Compare, Alloc>& lhs,
                const skip_map<Key, T, Compare, Alloc>& rhs) {
  if (lhs.size() != rhs.size()) {
    return false;
  }

  auto it1 = lhs.begin();
  auto it2 = rhs.begin();
  for (; it1 != lhs.end(); ++it1, ++it2) {
    if (*it1 != *it2) {
      return false;
    }
  }

  return true;
}

/**
 * Verifies that lhs and rhs are not equal. Uses the operartor ==
 */
template <class Key, class T, class Compare, class Alloc>
bool operator!=(const skip_map<Key, T, Compare, Alloc>& lhs,
                const skip_map<Key, T, Compare, Alloc>& rhs) {
  return !(lhs == rhs);
}

/**
 *
 */
template <class Key, class T, class Compare, class Alloc>
bool operator<(const skip_map<Key, T, Compare, Alloc>& /*lhs*/,
               const skip_map<Key, T, Compare, Alloc>& /*rhs*/) {
  throw std::runtime_error("Unimplemented!");
}

/**
 *
 */
template <class Key, class T, class Compare, class Alloc>
bool operator<=(const skip_map<Key, T, Compare, Alloc>& /*lhs*/,
                const skip_map<Key, T, Compare, Alloc>& /*rhs*/) {
  throw std::runtime_error("Unimplemented!");
}

/**
 *
 */
template <class Key, class T, class Compare, class Alloc>
bool operator>(const skip_map<Key, T, Compare, Alloc>& /*lhs*/,
               const skip_map<Key, T, Compare, Alloc>& /*rhs*/) {
  throw std::runtime_error("Unimplemented!");
}

/**
 *
 */
template <class Key, class T, class Compare, class Alloc>
bool operator>=(const skip_map<Key, T, Compare, Alloc>& /*lhs*/,
                const skip_map<Key, T, Compare, Alloc>& /*rhs*/) {
  throw std::runtime_error("Unimplemented!");
}

/**
 *
 */
template <class Key, class T, class Compare, class Alloc>
void swap(skip_map<Key, T, Compare, Alloc>& lhs,
          skip_map<Key, T, Compare, Alloc>& rhs) {
  std::swap(lhs.rend_, rhs.rend_);
  std::swap(lhs.end_, rhs.end_);
}

#endif /* skip_map_h */
