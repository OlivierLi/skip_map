#include <iostream>
#include <map>
#include "gtest/gtest.h"
#include "skip_map.h"

class SkipMapTest : public ::testing::Test {
 protected:
  SkipMapTest() = default;
  virtual ~SkipMapTest() = default;

  auto create_identical_maps(
      const std::vector<std::pair<int, std::string>>& data) {
    skip_map<int, std::string> sm;
    std::map<int, std::string> map;

    for (auto key_value : data) {
      if (sm.insert(key_value).second != map.insert(key_value).second) {
        std::cerr << "One of the insert successes mismatched! ["
                  << key_value.first << ":" << key_value.second << "]"
                  << std::endl;
      }
    }

    return std::make_pair(sm, map);
  }

  skip_map<int, std::string> empty_skip_map;
  std::map<int, std::string> empty_map;

  std::vector<std::pair<int, std::string>> mixed_data{
      {5, "fraise"}, {4, "citron"}, {2, "carotte"}, {2, "carotte"},
      {1, "patate"}, {3, "lime"},   {6, "navet"},   {0, "laitue"}};
};

TEST_F(SkipMapTest, size_when_empty) {
  ASSERT_EQ(empty_skip_map.size(), empty_map.size());
  ASSERT_EQ(empty_skip_map.empty(), empty_map.empty());
}

TEST_F(SkipMapTest, lower_bound_empty_map) {
  ASSERT_EQ(*empty_skip_map.lower_bound(1), *empty_skip_map.end());
}

TEST_F(SkipMapTest, all_lookup_techniques_mixed_data) {
  auto map_pair = create_identical_maps(mixed_data);
  auto& sm = map_pair.first;
  auto& map = map_pair.second;

  // Define a vector of different techniques that can be used to extract an
  // iterator from both a skip_map and an std::map by providing the key
  std::vector<
      std::function<std::pair<skip_map<int, std::string>::iterator,
                              std::map<int, std::string>::iterator>(int)>>
      techniques;

  // Add all techniques here that have to be tested in the exact same way
  techniques.emplace_back([&sm, &map](int key) {
    return std::make_pair(sm.lower_bound(key), map.lower_bound(key));
  });
  techniques.emplace_back([&sm, &map](int key) {
    return std::make_pair(sm.upper_bound(key), map.upper_bound(key));
  });
  techniques.emplace_back([&sm, &map](int key) {
    return std::make_pair(sm.find(key), map.find(key));
  });
  // TODO : How to add at?

  // Go over each technique
  for (auto technique : techniques) {
    // Test for each value of the data
    for (auto key_value : mixed_data) {
      auto it_pair = technique(key_value.first);
      auto& sm_it = it_pair.first;
      auto& map_it = it_pair.second;

      bool sm_at_end = it_pair.first == sm.end();
      bool map_at_end = it_pair.second == map.end();

      // If either values had no lower bound then they both need to have no
      // lower bound. If they have lower bounds the need to be the same
      if (sm_at_end || map_at_end) {
        ASSERT_TRUE(sm_at_end);
        ASSERT_TRUE(map_at_end);
      } else {
        ASSERT_EQ(sm_it->first, map_it->first);
        ASSERT_EQ(sm_it->second, map_it->second);
      }
    }
  }
}

TEST_F(SkipMapTest, iterate) {
  auto map_pair = create_identical_maps(mixed_data);
  auto& sm = map_pair.first;
  auto& map = map_pair.second;

  ASSERT_EQ(sm.size(), map.size());

  auto sm_it = sm.cbegin();
  auto map_it = map.cbegin();
  for (; sm_it != sm.cend(); ++sm_it, ++map_it) {
    ASSERT_EQ(sm_it->first, map_it->first);
    ASSERT_EQ(sm_it->second, map_it->second);
  }
}

TEST_F(SkipMapTest, insert_with_operator) {
  empty_skip_map[0];
  ASSERT_EQ(empty_skip_map.size(), size_t(1));
  empty_skip_map[0] = "Test";
  ASSERT_EQ(empty_skip_map.size(), size_t(1));
  ASSERT_EQ(empty_skip_map[0], "Test");
}

TEST(insert, duplicates) {
  skip_map<int, std::string> sm;
  ASSERT_TRUE(sm.insert({0, ""}).second);
  ASSERT_FALSE(sm.insert({0, ""}).second);
}

TEST(insert, case1) {
  int increasing_key = 0;
  skip_map<int, std::string> sm;

  std::vector<size_t> level_sequence{0,1,2,3};
  //std::vector<size_t> level_sequence{1,1,0,3};
  //std::vector<size_t> level_sequence{3,2,1,0};

  for(size_t level : level_sequence){
    sm.set_gen_for_testing([level](){return level;});
    ASSERT_TRUE(sm.insert({++increasing_key, ""}).second);

    auto it = sm.find(increasing_key);
    ASSERT_EQ(it.node->links.size(), level);
  }

  EXPECT_EQ(sm.size(), level_sequence.size());
  for(const auto& k_v : sm){
    std::cout << k_v.first << ":" << k_v.second << std::endl;
  }
}

TEST(static_case, constness) {
  skip_map<int, std::string> sm;

  static_assert(!std::is_const<decltype(sm.begin())::value_type>::value,
                "Value type can't be const for non const iterators");

  static_assert(std::is_const<decltype(sm.cbegin())::value_type>::value,
                "Value type has to be const for const iterators");
}

TEST(operators, equal) {
  skip_map<int, std::string> sm1;
  skip_map<int, std::string> sm2;
  ASSERT_EQ(sm1, sm2);

  std::pair<int, std::string> key_value{1, "A"};
  std::pair<int, std::string> key_value_2{2, "B"};
  sm1.insert(key_value);
  sm2.insert(key_value);
  ASSERT_EQ(sm1, sm2);

  sm1.insert(key_value_2);
  ASSERT_NE(sm1, sm2);

  sm2.insert(key_value_2);
  ASSERT_EQ(sm1, sm2);

  sm1[1] = "Not equal anymore";
  ASSERT_NE(sm1, sm2);
}

TEST_F(SkipMapTest, copying) {
  skip_map<int, std::string> sm1;
  skip_map<int, std::string> sm2;

  for (const auto& pair : mixed_data) {
    sm1.insert(pair);
  }

  // Copy assign into sm2
  sm2 = sm1;
  ASSERT_EQ(sm1, sm2);

  // Copy construct into sm3
  skip_map<int, std::string> sm3 = sm1;
  ASSERT_EQ(sm1, sm3);

  // Verify that we have deep copies
  sm1.begin()->second = sm1.begin()->second + "Not equal anymore!";
  ASSERT_NE(sm1, sm2);
  ASSERT_NE(sm1, sm3);
}

TEST_F(SkipMapTest, move_construction) {
  skip_map<int, std::string> sm1;

  for (const auto& pair : mixed_data) {
    sm1.insert(pair);
  }

  // move assign into sm2
  skip_map<int, std::string> bak = sm1;
  skip_map<int, std::string> sm2(std::move(sm1));
  ASSERT_EQ(sm2, bak);
}

TEST_F(SkipMapTest, move_assignment) {
  skip_map<int, std::string> sm1;

  for (const auto& pair : mixed_data) {
    sm1.insert(pair);
  }

  // move assign into sm2
  skip_map<int, std::string> bak = sm1;
  skip_map<int, std::string> sm2;
  sm2 = std::move(sm1);
  ASSERT_EQ(sm2, bak);
}

TEST_F(SkipMapTest, clear) {
  skip_map<int, std::string> sm;

  for (const auto& pair : mixed_data) {
    sm.insert(pair);
  }

  sm.clear();

  ASSERT_TRUE(sm.empty());
  ASSERT_EQ(sm.size(), size_t(0));
}

TEST_F(SkipMapTest, at) {
  ASSERT_THROW(empty_skip_map.at(1), std::out_of_range);

  auto map_pair = create_identical_maps(mixed_data);
  auto& sm = map_pair.first;
  auto& map = map_pair.second;

  ASSERT_EQ(sm.at(1), map.at(1));
}

TEST_F(SkipMapTest, erase) {
  auto map_pair = create_identical_maps(mixed_data);

  auto& sm = map_pair.first;
  auto& map = map_pair.second;

  for (const auto& key_value : mixed_data) {
    sm.erase(key_value.first);
    map.erase(key_value.first);

    ASSERT_EQ(sm.size(), map.size());
    auto it1 = sm.begin();
    auto it2 = map.begin();
    for (; it1 != sm.end(); ++it1, ++it2) {
      ASSERT_EQ(*it1, *it2);
    }
  }
}

//-----------------------------------------------------------------------------
// array tests------------------------------------------------------------------
//-----------------------------------------------------------------------------

class FixedVectorTest : public ::testing::Test {
 protected:
  FixedVectorTest() = default;
  virtual ~FixedVectorTest() = default;
  fixed_vector<int, 10> data;
};

TEST_F(FixedVectorTest, empty) {
  ASSERT_TRUE(data.empty());
  ASSERT_EQ(data.size(), 0);
  ASSERT_THROW(data.at(1), std::out_of_range);
  ASSERT_EQ(std::distance(data.begin(), data.end()), 0);
}

TEST_F(FixedVectorTest, push_back) {
  size_t i = 0;
  for (; i < data.capacity(); ++i) {
    ASSERT_FALSE(data.full());
    ASSERT_EQ(data.size(), i);
    ASSERT_EQ(std::distance(data.begin(), data.end()), i);
    data.push_back(i);
    ASSERT_EQ(data.at(i), i);
    ASSERT_EQ(data.size(), i + 1);
    ASSERT_EQ(data.back(), i);
    ASSERT_EQ(std::distance(data.begin(), data.end()), i + 1);
  }

  // Done filling, no place anymore
  ASSERT_THROW(data.at(i), std::out_of_range);
  ASSERT_TRUE(data.full());
  ASSERT_EQ(std::distance(data.begin(), data.end()), 10);

  // Let's try resizing
  data.resize(10);  // No effect
  ASSERT_EQ(data.size(), 10);
  ASSERT_EQ(data.at(9), 9);

  data.resize(5);
  ASSERT_EQ(data.size(), 5);
  ASSERT_FALSE(data.full());
}

int main(int argc, char** argv) {
  std::string filter("*");
  ::testing::GTEST_FLAG(filter) = filter;
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
