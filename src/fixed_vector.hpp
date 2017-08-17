#include <array>
#include <cassert>

template<typename T, size_t S>
class fixed_vector{
public:

  static_assert( S>0, "Can't have an empty array");
  // TODO : Assert destructible?

  fixed_vector(): capacity_{data.size()} {}

  bool empty(){
    return size_ == 0;
  }

  bool full(){
    return !(size_ < capacity_);
  }

  size_t size(){
    return size_;
  }

  size_t capacity(){
    return capacity_;
  }

  T& at(size_t i){
    if(i>=size()){
      throw std::out_of_range("Index too high!"); 
    }
    else{
      return data[i];
    }
  }
  
  // Handle rref
  void push_back( const T& value ){
    assert(!full());
    data[size_++] = value;
  }

private:
  std::array<T, S> data;
  size_t size_{0};
  size_t capacity_;
};
