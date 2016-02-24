#ifndef skip_map_h
#define skip_map_h

#include <limits>
#include <array>
#include <exception>

template<class T>
class skip_map_iterator{
  T& operator*(){
    throw std::runtime_error("Unimplemented!");
  }
  T& operator++(){
    throw std::runtime_error("Unimplemented!");
  }
  T& operator--(){
    throw std::runtime_error("Unimplemented!");
  }
};

template<class T,size_t size>
class skip_map_node{
  std::array<skip_map_node,size> links;
  T value;
};

template<class Key, class T, class Compare = std::less<Key>, class Allocator = std::allocator<std::pair<const Key, T>>>
class skip_map{
public:
  using key_type = Key;
  using mapped_type =	T;
  using value_type = std::pair<const Key, T>;
  using size_type =	std::size_t;
  using difference_type = std::ptrdiff_t;
  using key_compare	= Compare;
  using allocator_type = Allocator;
  using reference =	value_type&;
  using const_reference	= const value_type&;
  using pointer = typename std::allocator_traits<Allocator>::pointer;
  using const_pointer = typename std::allocator_traits<Allocator>::const_pointer;
  using iterator = skip_map_iterator<T>;
  using const_iterator  = const skip_map_iterator<T>;
  using reverse_iterator = std::reverse_iterator<iterator>;
  using const_reverse_iterator = std::reverse_iterator<const iterator>;
  
  Allocator get_allocator() const{
    return allocator;
  }
  
  T& at( const Key& key ){
    throw std::runtime_error("Unimplemented!");
  }
  const T& at( const Key& key ) const{
    throw std::runtime_error("Unimplemented!");
  }
  
  T& operator[]( const Key& key ){
    throw std::runtime_error("Unimplemented!");
  }
  T& operator[]( Key&& key ){
    throw std::runtime_error("Unimplemented!");
  }
  
  iterator begin() noexcept{
    throw std::runtime_error("Unimplemented!");
  }
  const_iterator begin() const noexcept{
    throw std::runtime_error("Unimplemented!");
  }
  const_iterator cbegin() const noexcept{
    throw std::runtime_error("Unimplemented!");
  }
 
  iterator end() noexcept{
    throw std::runtime_error("Unimplemented!");
  }
  const_iterator end() const noexcept{
    throw std::runtime_error("Unimplemented!");
  }
  const_iterator cend() const noexcept{
    throw std::runtime_error("Unimplemented!");
  }
  
  reverse_iterator rbegin() noexcept{
    throw std::runtime_error("Unimplemented!");
  }
  const_reverse_iterator rbegin() const noexcept{
    throw std::runtime_error("Unimplemented!");
  }
  const_reverse_iterator crbegin() const noexcept{
    throw std::runtime_error("Unimplemented!");
  }
  
  reverse_iterator rend() noexcept{
    throw std::runtime_error("Unimplemented!");
  }
  const_reverse_iterator rend() const noexcept{
    throw std::runtime_error("Unimplemented!");
  }
  const_reverse_iterator crend() const noexcept{
    throw std::runtime_error("Unimplemented!");
  }
  
  bool empty() const noexcept{
    return begin() == end();
  }
  bool size() const noexcept{
    return std::distance(begin(),end());
  }
  size_type max_size() const{
    return std::numeric_limits<size_type>::max();
  }

  void clear() noexcept{
    throw std::runtime_error("Unimplemented!");
  }
  std::pair<iterator,bool> insert( const value_type& value ){
    throw std::runtime_error("Unimplemented!");
  }
  template< class P >
  std::pair<iterator,bool> insert( P&& value ){
    throw std::runtime_error("Unimplemented!");
  }
  iterator insert( const_iterator hint, const value_type& value ){
    throw std::runtime_error("Unimplemented!");
  }
  iterator insert( const_iterator hint, value_type&& value ){
    throw std::runtime_error("Unimplemented!");
  }
  template< class InputIt >
  void insert( InputIt first, InputIt last ){
    throw std::runtime_error("Unimplemented!");
  }
  void insert( std::initializer_list<value_type> ilist ){
    throw std::runtime_error("Unimplemented!");
  }
  template< class... Args >
  std::pair<iterator,bool> emplace( Args&&... args ){
    throw std::runtime_error("Unimplemented!");
  }
  template <class... Args>
  iterator emplace_hint( const_iterator hint, Args&&... args ){
    throw std::runtime_error("Unimplemented!");
  }

  iterator erase( const_iterator pos ){
    throw std::runtime_error("Unimplemented!");
  }
  void erase( iterator first, iterator last ){
    throw std::runtime_error("Unimplemented!");
  }
  size_type erase( const key_type& key ){
    throw std::runtime_error("Unimplemented!");
  }

  void swap( skip_map& other ){
    throw std::runtime_error("Unimplemented!");
  }

  size_type count( const Key& key ) const{
    throw std::runtime_error("Unimplemented!");
  }
  template< class K >
  size_type count( const K& x ) const{
    throw std::runtime_error("Unimplemented!");

  }
  iterator find( const Key& key ){
    throw std::runtime_error("Unimplemented!");

  }
  const_iterator find( const Key& key ) const{
    throw std::runtime_error("Unimplemented!");

  }
  template< class K > iterator find( const K& x ){
    throw std::runtime_error("Unimplemented!");

  }
  template< class K > const_iterator find( const K& x ) const{
    throw std::runtime_error("Unimplemented!");

  }

  std::pair<iterator,iterator> equal_range( const Key& key ){
    throw std::runtime_error("Unimplemented!");
  }
  std::pair<const_iterator,const_iterator> equal_range( const Key& key ) const{
    throw std::runtime_error("Unimplemented!");
  }
  template< class K >
  std::pair<iterator,iterator> equal_range( const K& x ){
    throw std::runtime_error("Unimplemented!");
  }
  template< class K >
  std::pair<const_iterator,const_iterator> equal_range( const K& x ) const{
    throw std::runtime_error("Unimplemented!");

  }

  iterator lower_bound( const Key& key ){
    throw std::runtime_error("Unimplemented!");
  }
  const_iterator lower_bound( const Key& key ) const{
    throw std::runtime_error("Unimplemented!");
  }
  template< class K >
  iterator lower_bound(const K& x){
    throw std::runtime_error("Unimplemented!");
  }
  template< class K >
  const_iterator lower_bound(const K& x) const{
    throw std::runtime_error("Unimplemented!");
  }

  iterator upper_bound( const Key& key ){
    throw std::runtime_error("Unimplemented!");
  }
  const_iterator upper_bound( const Key& key ) const{
    throw std::runtime_error("Unimplemented!");
  }
  template< class K >
  iterator upper_bound( const K& x ){
    throw std::runtime_error("Unimplemented!");
  }
  template< class K >
  const_iterator upper_bound( const K& x ) const{
    throw std::runtime_error("Unimplemented!");
  }

private:
  Allocator allocator;
};

#endif /* skip_map_h */
