#pragma once

#include <array>

template <typename T, size_t S>
class fixed_vector {
 public:
  static_assert(S > 0, "Can't have an empty array");
  // TODO : Assert destructible?

  fixed_vector() : capacity_{data.size()} {}

  bool empty() const { return size_ == 0; }

  bool full() const { return !(size_ < capacity_); }

  size_t size() const { return size_; }

  size_t capacity() const { return capacity_; }

  auto begin() { return data.begin(); }

  auto begin() const { return data.begin(); }

  auto end() { return data.begin() + size_; }

  auto end() const { return data.begin() + size_; }

  auto& back() const { return data[size_ - 1]; }

  template <class... Args>
  void emplace_back(Args&&... args) {
    push_back(T{std::forward<Args>(args)...});
  }

  // TODO : Deduplicate
  const T& at(size_t i) const {
    if (i >= size()) {
      throw std::out_of_range("Index too high!");
    } else {
      return data[i];
    }
  }

  T& at(size_t i) {
    if (i >= size()) {
      throw std::out_of_range("index too high!");
    } else {
      return data[i];
    }
  }

  void resize(size_t count) { size_ = count; }

  // Handle rref
  void push_back(T value) { data[size_++] = value; }

 private:
  std::array<T, S> data;
  size_t size_{0};
  size_t capacity_;
};
