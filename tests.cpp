#include <iostream>
#include <map>
#include <map>
#include "skip_map.h"
#include "gtest/gtest.h"

TEST(insert, empty)
{
  skip_map<int, std::string> sm;
  auto begin = sm.begin();
  auto end = sm.end();
  bool result  = begin == end;
  ASSERT_TRUE(result);
}

TEST(insert, mixed_order){
  skip_map<int, std::string> sm;
  std::map<int, std::string> map;
 
  auto insert_in_both = [&sm,&map](std::pair<int,std::string> element){
    sm.insert(element);
    map.insert(element);
  };
  
  insert_in_both({5,"fraise"});
  insert_in_both({4,"citron"});
  insert_in_both({2,"carotte"});
  insert_in_both({2,"carotte"});
  insert_in_both({1,"patate"});
  insert_in_both({3,"lime"});
  insert_in_both({6,"navet"});
  insert_in_both({0,"laitue"});

  auto sm_it = sm.begin();
  auto map_it = sm.begin();
  for(size_t i=0;i<map.size();++i){
    ASSERT_EQ(sm_it->first, map_it->first);
    ++sm_it;
    ++map_it;
  }
  
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
