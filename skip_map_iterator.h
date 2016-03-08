#ifndef skip_map_iterator_h
#define skip_map_iterator_h

template<class Key, class T, bool is_const>
class skip_map_iterator : public std::iterator<std::bidirectional_iterator_tag,std::pair<const Key, T>>{
public:
  using difference_type = typename std::iterator<std::bidirectional_iterator_tag,T>::difference_type;
  using value_type = std::pair<const Key, T>;
  
  using ref_type = typename std::conditional<is_const,const value_type&, value_type&>::type;
  using pointer_type = typename std::conditional<is_const,const value_type*, value_type*>::type;
  using node_pointer_type = typename std::conditional<is_const,const skip_map_node<Key, T>*, skip_map_node<Key, T>*>::type;
  
  skip_map_iterator(skip_map_node<Key, T>* p) :node(p) {}
  skip_map_iterator(const skip_map_iterator<Key,T, false>& other) : node(other.node) {}
  
  skip_map_iterator& operator++() {
    node = node->link_at(0);
    return *this;
  }
  skip_map_iterator operator++(int){
    skip_map_iterator tmp(*this);
    operator++();
    return tmp;
  }

  skip_map_iterator& operator--(){
    node = node->previous;
    return *this;
  }
  skip_map_iterator operator--(int){
    skip_map_iterator tmp(*this);
    operator--();
    return tmp;
  }
  
  skip_map_iterator operator+(difference_type n) const{
    skip_map_iterator temp(*this);
    for(size_t i=0;temp.node && i<n;++i){
      ++temp;
    }
    return temp;
  }
  
  skip_map_iterator operator-(difference_type n) const{
    skip_map_iterator temp(*this);
    for(difference_type i=0;temp.node && i<n;++i){
      --temp;
    }
    return temp;
  }
  
  skip_map_iterator& operator+=(difference_type n){
    skip_map_iterator temp = this->operator+(n);
    *this = temp;
    return *this;
  }
  
  skip_map_iterator& operator-=(difference_type n){
    skip_map_iterator temp = this->operator-(n);
    *this = temp;
    return *this;
  }

  bool operator==(const skip_map_iterator& rhs){
    return node==rhs.node;
  }
  bool operator!=(const skip_map_iterator& rhs){
    return node!=rhs.node;
  }
  
  ref_type operator*() {
    return node->entry;
  }
  pointer_type operator->() {
    return &node->entry;
  }

  
//private:
  node_pointer_type node;
};

#endif /* skip_map_iterator_h */
