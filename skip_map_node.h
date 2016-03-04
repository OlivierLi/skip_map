#ifndef skip_map_node_h
#define skip_map_node_h

#include <memory>
#include <array>

const size_t skip_list_size_k = 5;

template <class Key, class T>
class skip_map_node {
public:
  skip_map_node():links{nullptr}, previous{nullptr}, entry{0,""}{
  }
  skip_map_node(Key key, T value):skip_map_node(){
    entry = std::make_pair(key,value);
  }
 
  skip_map_node* link_at(size_t i) const{
    return links[i];
  }
  
  void set_link(size_t i, skip_map_node* link){
    links[i] = link;
  }
  
  skip_map_node* previous;
  std::pair<Key, T> entry;
private:
  std::array<skip_map_node*, skip_list_size_k> links;
};

#endif /* skip_map_node_h */
