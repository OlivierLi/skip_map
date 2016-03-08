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
  bool result  = (begin == end);
  ASSERT_TRUE(result);
}

TEST(insert, duplicates)
{
  skip_map<int, std::string> sm;
  ASSERT_TRUE(sm.insert({0,""}).second);
  ASSERT_FALSE(sm.insert({0,""}).second);
}

TEST(iterate, mixed_order){
  skip_map<int, std::string> sm;
  std::map<int, std::string> map;
 
  auto insert_in_both = [&sm,&map](std::pair<int,std::string> element){
    sm.insert(element);
    map.insert(element);
  };
  
  std::vector<std::pair<int,std::string>> data{
    {5,"fraise"},
    {4,"citron"},
    {2,"carotte"},
    {2,"carotte"},
    {1,"patate"},
    {3,"lime"},
    {6,"navet"},
    {0,"laitue"}};
 
  for(auto key_value : data){
    insert_in_both(key_value);
  }
 
  ASSERT_EQ(sm.size(), map.size());
  
  auto sm_it = sm.begin();
  auto map_it = map.begin();
  for(;sm_it!=sm.end();++sm_it,++map_it){
    ASSERT_EQ(sm_it->first, map_it->first);
    ASSERT_EQ(sm_it->second, map_it->second);
  }
 
  auto sm_rit = sm.rbegin();
  auto map_rit = map.rbegin();
  for(;sm_rit!=sm.rend();++sm_rit,++map_rit){
    ASSERT_EQ(sm_rit->first, map_rit->first);
    ASSERT_EQ(sm_rit->second, map_rit->second);
  }
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
