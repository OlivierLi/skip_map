/// This executable is intended as a simple way to execute precisely the code we
/// want to inspect using Linux Perf.

#include <iostream>
#include <list>
#include "skip_map.h"
#include "test_facilities.hpp"

void iterate(skip_map<Key, Value>& sm) {
  for (auto it = sm.begin(); it != sm.end(); ++it)
    ;
}

int main(int, char**) {
  // Create a list.
  std::list<KeyValue> list(default_size, {1, long_string});

  // Create an equivalent skip_map.
  skip_map<Key, Value> sm;
  sm.set_gen_for_testing([]() { return 0; });
  fill(sm);

  for (auto it = sm.begin(); it != sm.end(); ++it)
    ;
  for (auto it = list.begin(); it != list.end(); ++it)
    ;

  return list.size();
}
