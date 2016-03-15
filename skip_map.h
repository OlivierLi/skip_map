#ifndef skip_map_h
#define skip_map_h

#include <limits>
#include <array>
#include <exception>
#include <functional>
#include "skip_map_node.h"
#include "skip_map_iterator.h"



/**
 * skip_map is a sorted associative container that contains key-value pairs with 
 * unique keys. Keys are sorted by using the comparison function Compare. 
 * Search, removal, and insertion operations have expected logarithmic 
 * complexity. skip_map is implemented using a skip_list.
 */
template <class Key, class T,
          class Compare = std::less<Key>,
          class Allocator = std::allocator<skip_map_node<Key, T>> >
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
  using reverse_iterator = std::reverse_iterator<iterator>;
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;

  using node_type = skip_map_node<Key, T>;
  
  /**
   * Default constructor
   */
  skip_map():
    end_ (allocate_and_init()),
    rend_(allocate_and_init())
  {
    end_->previous = rend_;
    rend_->set_link(0, end_);
  }
  
  /**
   * Copy constructor, performs a deep copy of the data in rhs
   */
  skip_map(const skip_map& rhs):skip_map(){
    //TODO : Use range insert
    for(const auto& key_value : rhs){
      insert(key_value);
    }
  }
  
  /**
   * Move constructor, steals all pointers of rhs. Leave rhs is a state that is
   * assignable and destructible by setting its pointer to null
   */
  skip_map(skip_map&& rhs):
    rend_(rhs.rend_),
    end_(rhs.end_)
  {
    rhs.rend_ = nullptr;
    rhs.end_ = nullptr;
  }
  
  /**
   * Copy assignement operator. Uses the copy and swap idiom. rhs is received by
   * copy which takes care of performing a deep copy of the data. This also
   * saves us from defining a move assignement operator since rhs will be move 
   * constructed when the operator is invoked with an r-value.
   */
  skip_map& operator=(skip_map rhs){
    swap(rhs);
    return *this;
  }
  
  /**
   * Clears all data to relase ressources
   */
  ~skip_map(){
    clear();
    destroy_and_release(rend_);
    destroy_and_release(end_);
  }

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
   * Finds an element with key equivalent to key.
   * Uses casting to avoid duplicated code. Safe since calls of this function
   * have to come from a non-const object (Effective c++ item 3)
   */
  iterator find(const Key& key) {
    const skip_map& const_this = static_cast<const skip_map &>(*this);
    const_iterator it = const_this.find(key);
    node_type* ptr = const_cast<node_type*>(it.node);
    return iterator(ptr);
  }
  
  /**
   * const overload of find()
   */
  const_iterator find(const Key& key) const {
    auto lower_bound_it = lower_bound(key);
    if( !key_comparator_(key, lower_bound_it->first) && lower_bound_it != end()){
      return lower_bound_it;
    }
    else{
      return end();
    }
  }
  
  /**
   * Returns a reference to the value that is mapped to a key equivalent to key, 
   * performing an insertion if such key does not already exist.
   */
  T& operator[](const Key& key) {
    auto it = find(key);
    if(it!=end()){
      return it->second;
    }
    else{
      return insert(value_type{key,T()}).first->second;
    }
  }

  /**
   *
   */
  //T& operator[](Key&& key) { throw std::runtime_error("Unimplemented!"); }

  /**
   * Returns an iterator to the first element of the container.
   * If the container is empty, the returned iterator will be equal to end().
   */
  iterator begin() noexcept { return std::next(iterator(rend_)); }

  /**
   * const overload of begin()
   */
  const_iterator begin() const noexcept { return std::next(const_iterator(rend_)); }

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
   * Returns a reverse iterator to the first element of the reversed container.
   * It corresponds to the last element of the non-reversed container.
   */
  reverse_iterator rbegin() noexcept { return reverse_iterator(end()); }

  /**
   * const overload of rbegin()
   */
  const_reverse_iterator rbegin() const noexcept {
    return const_reverse_iterator(end());
  }

  /**
   * explicitally const verstion of begin()
   */
  const_reverse_iterator crbegin() const noexcept { return rbegin(); }

  /**
   * Returns a reverse iterator to the element following the last element of
   * the reversed container. It corresponds to the element preceding the first
   * element of the non-reversed container. This element acts as a placeholder,
   * attempting to access it results in undefined behavior.
   */
  reverse_iterator rend() noexcept { return reverse_iterator(begin()); }

  /**
   * const overload of rend()
   */
  const_reverse_iterator rend() const noexcept {
    return const_reverse_iterator(begin());
  }

  /**
   * explicitally const version of rend()
   */
  const_reverse_iterator crend() const noexcept { return rend(); }

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
    if(rend_ == nullptr){
      return;
    }
  
    //If we have nothing to delete
    if(empty()){
      return;
    }
 
    //Remove the nodes one by one until we are left with the end
    auto it=begin();
    while(it!=end()){
      it = erase(it);
    }
  }

  /**
   * Inserts element(s) into the container, if the container doesn't already 
   * contain an element with an equivalent key.
   */
  std::pair<iterator, bool> insert(const value_type& value) {
    
    auto current = lower_bound(value.first);
    if (!key_comparator_(value.first,current->first) && current != end()) {
      return std::make_pair(current, false);
    }

    node_type* current_p = current.node;
    node_type* previous_p = current_p->previous;
    
    // Create new node
    auto new_node = allocate_and_init(value.first, value.second);
    
    new_node->set_link(0, current_p);
    new_node->link_at(0)->previous = new_node;

    new_node->previous = previous_p;
    new_node->previous->set_link(0, new_node);

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
    //TODO : Implement with hint to avoid n*log(n) complexity
    throw std::runtime_error("Unimplemented!");
  }

  /**
   *
   */
  void insert(std::initializer_list<value_type> ilist) {
    //TODO : Implement with hint to avoid n*log(n) complexity
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
  iterator erase(iterator pos) {
    //Don't delete past the end, past the beginning nodes
    if(pos==iterator(rend_) || pos==end()){
      return end();
    }
    
    node_type* next = pos.node->link_at(0);
    node_type* previous = pos.node->previous;
    
    previous->set_link(0,next);
    next->previous =  previous;
    
    destroy_and_release(pos.node);
    
    return iterator(next);
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
    
    auto it = find(key);
    if(it==end()){
      return size();
    }
    erase(it);
    
    return size();
  }

  /**
   *
   */
  void swap(skip_map& other) {
    std::swap(rend_,other.rend_);
    std::swap(end_,other.end_);
  }

  /**
   *
   */
  size_type count(const Key& key) const {
    throw std::runtime_error("Unimplemented!");
  }

  /**
   * Returns a pair of iterators obtained with lower_bound() and upper_bound()
   */
  std::pair<iterator, iterator> equal_range(const Key& key) {
    return std::make_pair(lower_bound(key),upper_bound(key));
  }

  /**
   * const overload of equal_range()
   */
  std::pair<const_iterator, const_iterator> equal_range(const Key& key) const {
    return std::make_pair(lower_bound(key),upper_bound(key));
  }
  
  /**
   * Returns an iterator pointing to the first element that is not less than key.
   * Uses casting to avoid duplicated code. Safe since calls of this function
   * have to come from a non-const object (Effective c++ item 3)
   */
  iterator lower_bound(const Key& key) {
    const skip_map& const_this = static_cast<const skip_map &>(*this);
    const_iterator it = const_this.lower_bound(key);
    node_type* ptr = const_cast<node_type*>(it.node);
    return iterator(ptr);
  }

  /**
   * const overload of lower_bound()
   */
  const_iterator lower_bound(const Key& key) const {
    auto current  = begin();
    for (; current != end() && key_comparator_(current->first, key); ++current) {
    }
    return current;
  }

  /**
   * Returns an iterator pointing to the first element that is greater than key.   
   * Uses casting to avoid duplicated code. Safe since calls of this function
   * have to come from a non-const object (Effective c++ item 3)
   */
  iterator upper_bound(const Key& key) {
    const skip_map& const_this = static_cast<const skip_map &>(*this);
    const_iterator it = const_this.upper_bound(key);
    node_type* ptr = const_cast<node_type*>(it.node);
    return iterator(ptr);
  }

  /**
   * const overload of upper_bound()
   */
  const_iterator upper_bound(const Key& key) const {
    auto it = lower_bound(key);
  
    if(it == end()){
      return it;
    }
    
    return it+1;
  }
  
  /**
   *
   */
  Allocator get_allocator() const { return allocator_; }

 private:
  
  /**
   * Convenience function to allocate and initialize memory in the same call
   */
  template<typename... Args>
  node_type* allocate_and_init(Args&&... arguments){
    auto ptr (allocator_.allocate(sizeof(node_type)));
    allocator_.construct(ptr, std::forward<Args>(arguments)...);
    return ptr;
  }
  
  /**
   * Convenience function to destroy a node object and deallocate in the same 
   * call.
   */
  void destroy_and_release(node_type* ptr){
    if(ptr){
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
   * Pointer to the element following the last element.
   */
  node_type* end_;

  /**
   * Pointer to the element preceding the first element.
   */
  node_type* rend_;

  /**
   * Instance of Compare used to compare keys
   */
  key_compare key_comparator_;
};

/**
 * Checks if the contents of lhs and rhs are equal, that is, whether lhs.size() 
 * == rhs.size() and each element in lhs compares equal with the element in rhs 
 * at the same position.
 */
template <class Key, class T, class Compare, class Alloc>
bool operator==(const skip_map<Key, T, Compare, Alloc>& lhs,
                const skip_map<Key, T, Compare, Alloc>& rhs) {
  
  if(lhs.size() != rhs.size()){
    return false;
  }
  
  auto it1=lhs.begin();
  auto it2=rhs.begin();
  for(;it1!=lhs.end();++it1,++it2){
    if(*it1 != *it2){
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
  return !(lhs==rhs);
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
  std::swap(lhs.rend_,rhs.rend_);
  std::swap(lhs.end_, rhs.end_);
}

#endif /* skip_map_h */
