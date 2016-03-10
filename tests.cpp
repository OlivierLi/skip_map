#include <iostream>
#include <map>
#include <map>
#include "skip_map.h"
#include "gtest/gtest.h"

class SkipMapTest : public ::testing::Test {
protected:

  SkipMapTest(){
  }
  
  virtual ~SkipMapTest(){
  }
  
  std::pair<skip_map<int, std::string>,std::map<int, std::string>>
  create_identical_maps(std::vector<std::pair<int,std::string>> data){
    
    skip_map<int, std::string> sm;
    std::map<int, std::string> map;
    
    auto insert_in_both = [&sm, &map](std::pair<int, std::string> element) {
      //Insert into both maps, insuring the the insertions return the
      //same success flag
      ASSERT_EQ(sm.insert(element).second, map.insert(element).second);
    };
    
    for (auto key_value : data) {
      insert_in_both(key_value);
    }
    
    return std::make_pair(sm, map);
  }
  
  skip_map<int, std::string> empty_skip_map;
  std::map<int, std::string> empty_map;
  
  std::vector<std::pair<int, std::string>> mixed_data{{5, "fraise"},
                                                      {4, "citron"},
                                                      {2, "carotte"},
                                                      {2, "carotte"},
                                                      {1, "patate"},
                                                      {3, "lime"},
                                                      {6, "navet"},
                                                      {0, "laitue"}};
};

TEST_F(SkipMapTest, size_when_empty){
  ASSERT_EQ(empty_skip_map.size(), empty_map.size());
  ASSERT_EQ(empty_skip_map.empty(), empty_map.empty());
}

TEST_F(SkipMapTest, lower_bound_empty_map){
  ASSERT_EQ(*empty_skip_map.lower_bound(1), *empty_skip_map.end());
}

TEST_F(SkipMapTest, lower_bound_full_data){
  
  auto& data =  mixed_data;
  auto map_pair = create_identical_maps(data);
  
  auto& sm = map_pair.first;
  auto& map = map_pair.second;
  
  for(auto key_value: data){
   
    auto lb_it_sm =  sm.lower_bound(key_value.first);
    auto lb_it_map = map.lower_bound(key_value.first);
    
    bool sm_at_end =  lb_it_sm == sm.end();
    bool map_at_end = lb_it_map == map.end();
    
    //If either values had no lower bound then they both need to have no lower
    //bound. If they have lower bounds the need to be the same
    if( sm_at_end || map_at_end ){
      ASSERT_TRUE(sm_at_end);
      ASSERT_TRUE(map_at_end);
    }
    else{
      ASSERT_EQ(lb_it_sm->first, lb_it_map->first);
    }
  }
}


TEST_F(SkipMapTest, upper_bound_full_data){
  
  auto& data =  mixed_data;
  auto map_pair = create_identical_maps(data);
  
  auto& sm = map_pair.first;
  auto& map = map_pair.second;
  
  for(auto key_value: data){
    
    auto lb_it_sm =  sm.upper_bound(key_value.first);
    auto lb_it_map = map.upper_bound(key_value.first);
    
    bool sm_at_end =  lb_it_sm == sm.end();
    bool map_at_end = lb_it_map == map.end();
    
    //If either values had no upper bound then they both need to have no upper
    //bound. If they have upper bounds the need to be the same
    if( sm_at_end || map_at_end ){
      ASSERT_TRUE(sm_at_end);
      ASSERT_TRUE(map_at_end);
    }
    else{
      ASSERT_EQ(lb_it_sm->first, lb_it_map->first);
    }
  }
}

TEST_F(SkipMapTest, iterate) {
  auto map_pair = create_identical_maps(mixed_data);
  auto sm = map_pair.first;
  auto map = map_pair.second;
  
  ASSERT_EQ(sm.size(), map.size());

  auto sm_it = sm.cbegin();
  auto map_it = map.cbegin();
  for (; sm_it != sm.cend(); ++sm_it, ++map_it) {
    ASSERT_EQ(sm_it->first, map_it->first);
    ASSERT_EQ(sm_it->second, map_it->second);
  }

  auto sm_rit = sm.crbegin();
  auto map_rit = map.crbegin();
  for (; sm_rit != sm.crend(); ++sm_rit, ++map_rit) {
    ASSERT_EQ(sm_rit->first, map_rit->first);
    ASSERT_EQ(sm_rit->second, map_rit->second);
  }
}

TEST(insert,operator){
  skip_map<int, std::string> sm;
  sm[0];
  ASSERT_EQ(sm.size(), 1);
  sm[0] = 10;
  ASSERT_EQ(sm.size(), 1);
}

TEST(insert, duplicates) {
  skip_map<int, std::string> sm;
  ASSERT_TRUE(sm.insert({0, ""}).second);
  ASSERT_FALSE(sm.insert({0, ""}).second);
}

TEST(static_case, constness) {
  skip_map<int, std::string> sm;

  static_assert(!std::is_const<decltype(sm.begin())::value_type>::value,
                "Value type can't be const for non const iterators");

  static_assert(std::is_const<decltype(sm.cbegin())::value_type>::value,
                "Value type has to be const for const iterators");
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
