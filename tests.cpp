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

TEST_F(SkipMapTest, all_lookup_techniques_mixed_data){
  
  auto& data =  mixed_data;
  auto map_pair = create_identical_maps(data);
  
  auto& sm = map_pair.first;
  auto& map = map_pair.second;

  // Define a vector of different techniques that can be used to extract an
  // iterator from both a skip_map and an std::map by providing the key
  std::vector<std::function<std::pair<
              skip_map<int, std::string>::iterator,
              std::map<int, std::string>::iterator>(int)>> techniques;
 
  // Add all techniques here that have to be tested in the exact same way
  techniques.emplace_back([&sm, &map](int key){return std::make_pair(sm.lower_bound(key), map.lower_bound(key));});
  techniques.emplace_back([&sm, &map](int key){return std::make_pair(sm.upper_bound(key), map.upper_bound(key));});
  techniques.emplace_back([&sm, &map](int key){return std::make_pair(sm.find(key), map.find(key));});

  // Go over each techniqu
  for( auto technique : techniques){
    //Test for each value of the data
    for(auto key_value: data){
    
      auto it_pair = technique(key_value.first);
      auto& sm_it = it_pair.first;
      auto& map_it = it_pair.second;
      
      bool sm_at_end =  it_pair.first == sm.end();
      bool map_at_end = it_pair.second == map.end();
      
      //If either values had no lower bound then they both need to have no lower
      //bound. If they have lower bounds the need to be the same
      if( sm_at_end || map_at_end ){
        ASSERT_TRUE(sm_at_end);
        ASSERT_TRUE(map_at_end);
      }
      else{
        ASSERT_EQ(sm_it->first, map_it->first);
        ASSERT_EQ(sm_it->second, map_it->second);
      }
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

TEST_F(SkipMapTest,operator){
  empty_skip_map[0];
  ASSERT_EQ(empty_skip_map.size(), 1);
  empty_skip_map[0] = "Test";
  ASSERT_EQ(empty_skip_map.size(), 1);
  ASSERT_EQ(empty_skip_map[0], "Test");
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
