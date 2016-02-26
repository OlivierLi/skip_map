#ifndef skip_map_h
#define skip_map_h

#include <limits>
#include <array>
#include <exception>
#include <functional>

const size_t skip_list_size_k = 5;

template <class Key, class T, class Compare = std::less<Key>,
          class Allocator = std::allocator<std::pair<const Key, T>>>
class skip_map {
 public:
  using key_type = Key;
  using mapped_type = T;
  // TODO : Keys SHOULD BE CONST, implement proxy object
  using value_type = std::pair<Key, T>;

 private:
  class skip_map_node {
   public:
    std::array<skip_map_node*, skip_list_size_k> links;
    skip_map_node* previous;
    value_type entry;
  };

 public:
  class skip_map_iterator {
   public:
    skip_map_iterator(skip_map_node* node) : node_(node) {}
    value_type& operator*() { return node_->entry; }
    value_type& operator++() {
      value_type& temp = operator*();
      node_ = node_->links[0];
      return temp;
    }
    value_type& operator--() {
      value_type& temp = operator*();
      node_ = node_->previous;
      return temp;
    }
    bool operator==(const skip_map_iterator& rhs) { return node_ == rhs.node_; }
    bool operator!=(const skip_map_iterator& rhs) { return node_ != rhs.node_; }

   private:
    skip_map_node* node_;
  };

  using size_type = std::size_t;
  using difference_type = std::ptrdiff_t;
  using key_compare = Compare;
  using allocator_type = Allocator;
  using reference = value_type&;
  using const_reference = const value_type&;
  using pointer = typename std::allocator_traits<Allocator>::pointer;
  using const_pointer =
      typename std::allocator_traits<Allocator>::const_pointer;
  using iterator = skip_map_iterator;
  using const_iterator = const iterator;
  using reverse_iterator = std::reverse_iterator<iterator>;
  using const_reverse_iterator = std::reverse_iterator<const iterator>;

  skip_map() : head_(nullptr) {}
  ~skip_map() { throw std::runtime_error("Unimplemented!"); }

  Allocator get_allocator() const { return allocator; }

  T& at(const Key& key) { throw std::runtime_error("Unimplemented!"); }
  const T& at(const Key& key) const {
    throw std::runtime_error("Unimplemented!");
  }

  T& operator[](const Key& key) { throw std::runtime_error("Unimplemented!"); }
  T& operator[](Key&& key) { throw std::runtime_error("Unimplemented!"); }

  iterator begin() noexcept { return skip_map_iterator(head_); }
  const_iterator begin() const noexcept {
    throw std::runtime_error("Unimplemented!");
  }
  const_iterator cbegin() const noexcept {
    throw std::runtime_error("Unimplemented!");
  }

  iterator end() noexcept { return skip_map_iterator(nullptr); }
  const_iterator end() const noexcept {
    throw std::runtime_error("Unimplemented!");
  }
  const_iterator cend() const noexcept {
    throw std::runtime_error("Unimplemented!");
  }

  reverse_iterator rbegin() noexcept {
    throw std::runtime_error("Unimplemented!");
  }
  const_reverse_iterator rbegin() const noexcept {
    throw std::runtime_error("Unimplemented!");
  }
  const_reverse_iterator crbegin() const noexcept {
    throw std::runtime_error("Unimplemented!");
  }

  reverse_iterator rend() noexcept {
    throw std::runtime_error("Unimplemented!");
  }
  const_reverse_iterator rend() const noexcept {
    throw std::runtime_error("Unimplemented!");
  }
  const_reverse_iterator crend() const noexcept {
    throw std::runtime_error("Unimplemented!");
  }

  bool empty() const noexcept { return begin() == end(); }
  bool size() const noexcept { return std::distance(begin(), end()); }
  size_type max_size() const { return std::numeric_limits<size_type>::max(); }

  void clear() noexcept { throw std::runtime_error("Unimplemented!"); }

  //------------------------------------------------------------------------------------------------------------------------
  std::pair<iterator, bool> insert(const value_type& value) {
    // Create new node
    skip_map_node* new_node = new skip_map_node();
    new_node->entry = value;
   
    skip_map_node* temp = head_;
    //If the list is already we insert directly
    if (!head_) {
      head_ = new_node;
    }
    else {
      // Find where to insert it
      // Insert either when we reached the end, or the values get bigger
      while (temp->links[0] && !key_comparator(new_node->entry.first, temp->links[0]->entry.first)) {
        //The value is already in the map, don't insert it
        if (temp->entry.first == value.first) {
          return std::make_pair(skip_map_iterator(temp), true);
        }
        temp = temp->links[0];
      }
      if (!temp->links[0]) {
        temp->links[0] = new_node;
      } else {
        new_node->links[0] = temp->links[0];
        temp->links[0] = new_node;
      }
    }
    return std::make_pair(skip_map_iterator(temp), true);
  }
  
  //------------------------------------------------------------------------------------------------------------------------
  template <class P>
  std::pair<iterator, bool> insert(P&& value) {
    throw std::runtime_error("Unimplemented!");
  }
  iterator insert(const_iterator hint, const value_type& value) {
    throw std::runtime_error("Unimplemented!");
  }
  iterator insert(const_iterator hint, value_type&& value) {
    throw std::runtime_error("Unimplemented!");
  }
  template <class InputIt>
  void insert(InputIt first, InputIt last) {
    throw std::runtime_error("Unimplemented!");
  }
  void insert(std::initializer_list<value_type> ilist) {
    throw std::runtime_error("Unimplemented!");
  }
  template <class... Args>
  std::pair<iterator, bool> emplace(Args&&... args) {
    throw std::runtime_error("Unimplemented!");
  }
  template <class... Args>
  iterator emplace_hint(const_iterator hint, Args&&... args) {
    throw std::runtime_error("Unimplemented!");
  }

  iterator erase(const_iterator pos) {
    throw std::runtime_error("Unimplemented!");
  }
  void erase(iterator first, iterator last) {
    throw std::runtime_error("Unimplemented!");
  }
  size_type erase(const key_type& key) {
    throw std::runtime_error("Unimplemented!");
  }

  void swap(skip_map& other) { throw std::runtime_error("Unimplemented!"); }

  size_type count(const Key& key) const {
    throw std::runtime_error("Unimplemented!");
  }
  template <class K>
  size_type count(const K& x) const {
    throw std::runtime_error("Unimplemented!");
  }
  iterator find(const Key& key) { throw std::runtime_error("Unimplemented!"); }
  const_iterator find(const Key& key) const {
    throw std::runtime_error("Unimplemented!");
  }
  template <class K>
  iterator find(const K& x) {
    throw std::runtime_error("Unimplemented!");
  }
  template <class K>
  const_iterator find(const K& x) const {
    throw std::runtime_error("Unimplemented!");
  }

  std::pair<iterator, iterator> equal_range(const Key& key) {
    throw std::runtime_error("Unimplemented!");
  }
  std::pair<const_iterator, const_iterator> equal_range(const Key& key) const {
    throw std::runtime_error("Unimplemented!");
  }
  template <class K>
  std::pair<iterator, iterator> equal_range(const K& x) {
    throw std::runtime_error("Unimplemented!");
  }
  template <class K>
  std::pair<const_iterator, const_iterator> equal_range(const K& x) const {
    throw std::runtime_error("Unimplemented!");
  }

  iterator lower_bound(const Key& key) {
    throw std::runtime_error("Unimplemented!");
  }
  const_iterator lower_bound(const Key& key) const {
    throw std::runtime_error("Unimplemented!");
  }
  template <class K>
  iterator lower_bound(const K& x) {
    throw std::runtime_error("Unimplemented!");
  }
  template <class K>
  const_iterator lower_bound(const K& x) const {
    throw std::runtime_error("Unimplemented!");
  }

  iterator upper_bound(const Key& key) {
    throw std::runtime_error("Unimplemented!");
  }
  const_iterator upper_bound(const Key& key) const {
    throw std::runtime_error("Unimplemented!");
  }
  template <class K>
  iterator upper_bound(const K& x) {
    throw std::runtime_error("Unimplemented!");
  }
  template <class K>
  const_iterator upper_bound(const K& x) const {
    throw std::runtime_error("Unimplemented!");
  }

 private:
  Allocator allocator;
  skip_map_node* head_;
  key_compare key_comparator;
};

template <class Key, class T, class Compare, class Alloc>
bool operator==(const skip_map<Key, T, Compare, Alloc>& lhs,
                const skip_map<Key, T, Compare, Alloc>& rhs) {
  throw std::runtime_error("Unimplemented!");
}
template <class Key, class T, class Compare, class Alloc>
bool operator!=(const skip_map<Key, T, Compare, Alloc>& lhs,
                const skip_map<Key, T, Compare, Alloc>& rhs) {
  throw std::runtime_error("Unimplemented!");
}
template <class Key, class T, class Compare, class Alloc>
bool operator<(const skip_map<Key, T, Compare, Alloc>& lhs,
               const skip_map<Key, T, Compare, Alloc>& rhs) {
  throw std::runtime_error("Unimplemented!");
}
template <class Key, class T, class Compare, class Alloc>
bool operator<=(const skip_map<Key, T, Compare, Alloc>& lhs,
                const skip_map<Key, T, Compare, Alloc>& rhs) {
  throw std::runtime_error("Unimplemented!");
}
template <class Key, class T, class Compare, class Alloc>
bool operator>(const skip_map<Key, T, Compare, Alloc>& lhs,
               const skip_map<Key, T, Compare, Alloc>& rhs) {
  throw std::runtime_error("Unimplemented!");
}
template <class Key, class T, class Compare, class Alloc>
bool operator>=(const skip_map<Key, T, Compare, Alloc>& lhs,
                const skip_map<Key, T, Compare, Alloc>& rhs) {
  throw std::runtime_error("Unimplemented!");
}

template <class Key, class T, class Compare, class Alloc>
void swap(skip_map<Key, T, Compare, Alloc>& lhs,
          skip_map<Key, T, Compare, Alloc>& rhs) {
  throw std::runtime_error("Unimplemented!");
}

#endif /* skip_map_h */
