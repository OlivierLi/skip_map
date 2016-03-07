#ifndef skip_map_h
#define skip_map_h

#include <limits>
#include <array>
#include <exception>
#include <functional>
#include "skip_map_node.h"
#include "skip_map_iterator.h"

/**
 *
 */
template <class Key, class T, class Compare = std::less<Key>,
          class Allocator = std::allocator<std::pair<const Key, T>>>
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
  using iterator = skip_map_iterator<Key, T,false>;
  using const_iterator = skip_map_iterator<Key, T, true>;
  using reverse_iterator = std::reverse_iterator<iterator>;
  using const_reverse_iterator = std::reverse_iterator<reverse_iterator>;

  /**
   *
   */
  skip_map():
    end_(new skip_map_node<Key, T>),
    rend_(new skip_map_node<Key, T>)
  {
    end_->previous = rend_;
    rend_->set_link(0,end_);
  }
  
  ~skip_map() = default;
  
  /**
   *
   */
  Allocator get_allocator() const { return allocator; }
  
  /**
   *
   */
  T& at(const Key& key) { throw std::runtime_error("Unimplemented!"); }
  
  /**
   *
   */
  const T& at(const Key& key) const {
    throw std::runtime_error("Unimplemented!");
  }
  
  /**
   *
   */
  T& operator[](const Key& key) { throw std::runtime_error("Unimplemented!"); }
  
  /**
   *
   */
  T& operator[](Key&& key) { throw std::runtime_error("Unimplemented!"); }

  /**
   * Returns an iterator to the first element of the container.
   * If the container is empty, the returned iterator will be equal to end().
   */
  iterator begin() noexcept {
    return iterator(rend_)+1;
  }
  
  /**
   * const overload of begin()
   */
  const_iterator begin() const noexcept {
    return const_iterator(rend_)+1;
  }
  
  /**
   * explicitally const version of begin()
   */
  const_iterator cbegin() const noexcept {
    return begin();
  }

  /**
   * Returns an iterator to the element following the last element of the
   * container. This element acts as a placeholder. Attempting to access it
   * results in undefined behavior.
   */
  iterator end() noexcept {
    return iterator(end_);
  }
  
  /**
   * const overload of end()
   */
  const_iterator end() const noexcept {
    return const_iterator(end_);
  }
  
  /**
   * explicitally const verstion of end()
   */
  const_iterator cend() const noexcept {
    return end();
  }

  /**
   *
   */
  reverse_iterator rbegin() noexcept {
    return std::reverse_iterator<iterator>(end());
  }
  
  /**
   *
   */
  const_reverse_iterator rbegin() const noexcept {
    throw std::runtime_error("Unimplemented!");
  }
  
  /**
   *
   */
  const_reverse_iterator crbegin() const noexcept {
    throw std::runtime_error("Unimplemented!");
  }

  /**
   *
   */
  reverse_iterator rend() noexcept {
    return std::reverse_iterator<iterator>(begin());
  }
  
  /**
   *
   */
  const_reverse_iterator rend() const noexcept {
    throw std::runtime_error("Unimplemented!");
  }
  
  /**
   *
   */
  const_reverse_iterator crend() const noexcept {
    throw std::runtime_error("Unimplemented!");
  }

  /**
   *
   */
  bool empty() const noexcept { return begin() == end(); }
  
  /**
   *
   */
  size_type size() const noexcept { return std::distance(begin(), end()); }
  
  /**
   *
   */
  size_type max_size() const { return std::numeric_limits<size_type>::max(); }

  /**
   *
   */
  void clear() noexcept { throw std::runtime_error("Unimplemented!"); }

  /**
   *
   */
  std::pair<iterator, bool> insert(const value_type& value) {
    // Create new node
    auto new_node = new skip_map_node<Key, T>(value.first,value.second);
    
    auto previous = iterator(rend_);
    auto current = previous+1;
    for(;current!=end() && key_comparator(current->first,value.first);++previous,++current){
      if(current->first == value.first){
        return std::make_pair(current,false);
      }
    }
    if(current->first == value.first && current!=end()){
        return std::make_pair(current,false);
    }
    
    skip_map_node<Key, T>* previous_p = previous.node;
    skip_map_node<Key, T>* current_p = current.node;
   
    new_node->set_link(0,current_p);
    new_node->link_at(0)->previous = new_node;
  
    new_node->previous = previous_p;
    new_node->previous->set_link(0,new_node);
    
    return std::make_pair(iterator(new_node), true);
  }

  /*template <class P>
  std::pair<iterator, bool> insert(P&& value) {
    throw std::runtime_error("Unimplemented!");
  }
  */
  
  /**
   *
   */
  iterator insert(const_iterator hint, const value_type& value) {
    throw std::runtime_error("Unimplemented!");
  }
  
  /**
   *
   */
  iterator insert(const_iterator hint, value_type&& value) {
    throw std::runtime_error("Unimplemented!");
  }
  
  /**
   *
   */
  template <class InputIt>
  void insert(InputIt first, InputIt last) {
    throw std::runtime_error("Unimplemented!");
  }
  
  /**
   *
   */
  void insert(std::initializer_list<value_type> ilist) {
    throw std::runtime_error("Unimplemented!");
  }
  
  /**
   *
   */
  template <class... Args>
  std::pair<iterator, bool> emplace(Args&&... args) {
    throw std::runtime_error("Unimplemented!");
  }
  
  /**
   *
   */
  template <class... Args>
  iterator emplace_hint(const_iterator hint, Args&&... args) {
    throw std::runtime_error("Unimplemented!");
  }

  /**
   *
   */
  iterator erase(const_iterator pos) {
    throw std::runtime_error("Unimplemented!");
  }
  
  /**
   *
   */
  void erase(iterator first, iterator last) {
    throw std::runtime_error("Unimplemented!");
  }
  
  /**
   *
   */
  size_type erase(const key_type& key) {
    throw std::runtime_error("Unimplemented!");
  }

  /**
   *
   */
  void swap(skip_map& other) { throw std::runtime_error("Unimplemented!"); }

  /**
   *
   */
  size_type count(const Key& key) const {
    throw std::runtime_error("Unimplemented!");
  }
  
  /**
   *
   */
  template <class K>
  size_type count(const K& x) const {
    throw std::runtime_error("Unimplemented!");
  }
  
  /**
   *
   */
  iterator find(const Key& key) { throw std::runtime_error("Unimplemented!"); }
  const_iterator find(const Key& key) const {
    throw std::runtime_error("Unimplemented!");
  }
  
  /**
   *
   */
  template <class K>
  iterator find(const K& x) {
    throw std::runtime_error("Unimplemented!");
  }
  
  /**
   *
   */
  template <class K>
  const_iterator find(const K& x) const {
    throw std::runtime_error("Unimplemented!");
  }

  /**
   *
   */
  std::pair<iterator, iterator> equal_range(const Key& key) {
    throw std::runtime_error("Unimplemented!");
  }
  
  /**
   *
   */
  std::pair<const_iterator, const_iterator> equal_range(const Key& key) const {
    throw std::runtime_error("Unimplemented!");
  }
  
  /**
   *
   */
  template <class K>
  std::pair<iterator, iterator> equal_range(const K& x) {
    throw std::runtime_error("Unimplemented!");
  }
  
  /**
   *
   */
  template <class K>
  std::pair<const_iterator, const_iterator> equal_range(const K& x) const {
    throw std::runtime_error("Unimplemented!");
  }

  /**
   *
   */
  iterator lower_bound(const Key& key) {
    throw std::runtime_error("Unimplemented!");
  }
  
  /**
   *
   */
  const_iterator lower_bound(const Key& key) const {
    throw std::runtime_error("Unimplemented!");
  }
  
  /**
   *
   */
  template <class K>
  iterator lower_bound(const K& x) {
    throw std::runtime_error("Unimplemented!");
  }
  
  /**
   *
   */
  template <class K>
  const_iterator lower_bound(const K& x) const {
    throw std::runtime_error("Unimplemented!");
  }

  /**
   *
   */
  iterator upper_bound(const Key& key) {
    throw std::runtime_error("Unimplemented!");
  }
  
  /**
   *
   */
  const_iterator upper_bound(const Key& key) const {
    throw std::runtime_error("Unimplemented!");
  }
  
  /**
   *
   */
  template <class K>
  iterator upper_bound(const K& x) {
    throw std::runtime_error("Unimplemented!");
  }
  
  /**
   *
   */
  template <class K>
  const_iterator upper_bound(const K& x) const {
    throw std::runtime_error("Unimplemented!");
  }

 private:
  /**
   *
   */
  Allocator allocator;
  
  /**
   *
   */
  skip_map_node<Key, T>* end_;
  
  /**
   *
   */
  skip_map_node<Key, T>* rend_;
  
  /**
   *
   */
  key_compare key_comparator;
};

/**
 *
 */
template <class Key, class T, class Compare, class Alloc>
bool operator==(const skip_map<Key, T, Compare, Alloc>& lhs,
                const skip_map<Key, T, Compare, Alloc>& rhs) {
  throw std::runtime_error("Unimplemented!");
}

/**
 *
 */
template <class Key, class T, class Compare, class Alloc>
bool operator!=(const skip_map<Key, T, Compare, Alloc>& lhs,
                const skip_map<Key, T, Compare, Alloc>& rhs) {
  throw std::runtime_error("Unimplemented!");
}

/**
 *
 */
template <class Key, class T, class Compare, class Alloc>
bool operator<(const skip_map<Key, T, Compare, Alloc>& lhs,
               const skip_map<Key, T, Compare, Alloc>& rhs) {
  throw std::runtime_error("Unimplemented!");
}

/**
 *
 */
template <class Key, class T, class Compare, class Alloc>
bool operator<=(const skip_map<Key, T, Compare, Alloc>& lhs,
                const skip_map<Key, T, Compare, Alloc>& rhs) {
  throw std::runtime_error("Unimplemented!");
}

/**
 *
 */
template <class Key, class T, class Compare, class Alloc>
bool operator>(const skip_map<Key, T, Compare, Alloc>& lhs,
               const skip_map<Key, T, Compare, Alloc>& rhs) {
  throw std::runtime_error("Unimplemented!");
}

/**
 *
 */
template <class Key, class T, class Compare, class Alloc>
bool operator>=(const skip_map<Key, T, Compare, Alloc>& lhs,
                const skip_map<Key, T, Compare, Alloc>& rhs) {
  throw std::runtime_error("Unimplemented!");
}

/**
 *
 */
template <class Key, class T, class Compare, class Alloc>
void swap(skip_map<Key, T, Compare, Alloc>& lhs,
          skip_map<Key, T, Compare, Alloc>& rhs) {
  throw std::runtime_error("Unimplemented!");
}

#endif /* skip_map_h */
