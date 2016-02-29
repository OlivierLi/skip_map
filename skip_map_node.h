#ifndef skip_map_node_h
#define skip_map_node_h

const size_t skip_list_size_k = 5;

template <class Key, class T>
struct skip_map_node {
public:
  skip_map_node():links{nullptr},previous{nullptr}{
  }
  skip_map_node(Key key, T value):entry(std::pair<Key, T>(key,value)),links{nullptr},previous{nullptr}{
  }
  std::array<skip_map_node*, skip_list_size_k> links;
  skip_map_node* previous;
  std::pair<Key, T> entry;
};

#endif /* skip_map_node_h */
