#ifndef skip_map_iterator_h
#define skip_map_iterator_h

template<class Key, class T>
class skip_map_iterator : public std::iterator<std::bidirectional_iterator_tag,std::pair<Key, T>>{
public:
  using difference_type = typename std::iterator<std::bidirectional_iterator_tag,T>::difference_type;
  using value_type = std::pair<Key, T>;

  skip_map_iterator(skip_map_node<Key, T>* p) :node(p) {}
  skip_map_iterator(const skip_map_iterator& other) : node(other.node) {}

  skip_map_iterator& operator++() {
    node = node->links[0];
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
  
  value_type& operator*() {
    return node->entry;
  }
  value_type* operator->() {
    return &node->entry;
  }

private:
  skip_map_node<Key, T>* node;
};

#endif /* skip_map_iterator_h */
