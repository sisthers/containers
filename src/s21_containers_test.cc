#include "s21_containers.h"

#include <gtest/gtest.h>

#include <array>
#include <vector>

// array tests

class tester_class {
 public:
  tester_class() {
    m = new int[10];
    for (int i = 0; i < 10; i++) m[i] = i;
  }

  tester_class(int *_m, int _n) : m(_m), n(_n) {}

  tester_class(const tester_class &other) {
    m = new int[10];
    for (int i = 0; i < 10; i++) m[i] = other.m[i];
  }

  tester_class(tester_class &&other) noexcept {
    m = other.m;
    other.m = nullptr;
  }

  tester_class &operator=(const tester_class &other) {
    for (int i = 0; i < 10; i++) m[i] = other.m[i];
    return *this;
  }

  tester_class &operator=(tester_class &&other) {
    delete[] m;
    m = other.m;
    other.m = nullptr;
    return *this;
  }

  bool operator!=(const tester_class &other) const { return other.n != 10; }

  bool operator==(const tester_class &other) const { return other.n == 10; }

  ~tester_class() { delete[] m; }

  int *m;
  int n = 10;
};

class throw_tester_class {
 public:
  throw_tester_class() {
    m = new int[10];
    for (int i = 0; i < 10; i++) m[i] = i;
  }
  throw_tester_class(const throw_tester_class &other) {
    if (other.n == 1) {
      throw std::out_of_range("qwe");
    }
    m = new int[10];
    for (int i = 0; i < 10; i++) m[i] = other.m[i];
    n = other.n;
  }
  throw_tester_class(throw_tester_class &&other) {
    m = other.m;
    n = other.n;
    other.n = 0;
    other.m = nullptr;
  }
  throw_tester_class &operator=(const throw_tester_class &other) {
    if (n == 1) {
      delete[] m;
      throw;
    }
    for (int i = 0; i < 10; i++) m[i] = other.m[i];

    n = other.n;
    return *this;
  }
  throw_tester_class &operator=(throw_tester_class &&other) {
    delete[] m;
    n = other.n;
    m = other.m;
    other.m = nullptr;
    return *this;
  }

  bool operator!=(const throw_tester_class &other) const {
    return other.n != 10;
  }

  ~throw_tester_class() {
    if (m != nullptr) delete[] m;
  }

  int *m = nullptr;
  int n = 10;
};

template <class T, class Allocator>
bool compare_to_std(const std::vector<T, Allocator> &std_vec,
                    const s21::vector<T, Allocator> &s21_vec,
                    bool compare_elements) {
  if (std_vec.size() == s21_vec.size() &&
      std_vec.capacity() == s21_vec.capacity() &&
      std_vec.empty() == s21_vec.empty() &&
      std_vec.max_size() == s21_vec.max_size() &&
      std_vec.get_allocator() == s21_vec.get_allocator()) {
    if (compare_elements)
      for (size_t i = 0; i < std_vec.size(); i++)
        if (std_vec[i] != s21_vec[i]) return false;
    return true;
  }
  return false;
}

template <class T, std::size_t N>
bool compare_to_std(const std::array<T, N> &std_arr,
                    const s21::array<T, N> &s21_arr, bool compare_elements) {
  if (std_arr.size() == s21_arr.size() && std_arr.empty() == s21_arr.empty() &&
      std_arr.max_size() == s21_arr.max_size() &&
      std_arr.front() == s21_arr.front() && std_arr.back() == s21_arr.back()) {
    if (compare_elements)
      for (size_t i = 0; i < s21_arr.size(); i++)
        if (std_arr[i] != s21_arr[i]) return false;
    return true;
  }
  return false;
}

TEST(array, init) {
  std::array<double, 5> stdarr1 = {1, 2, 3, 4, 5};
  s21::array<double, 5> s21arr1 = {1, 2, 3, 4, 5};
  EXPECT_EQ(compare_to_std(stdarr1, s21arr1, true), true);

  std::array<tester_class, 5> stdarr2 = {tester_class()};
  s21::array<tester_class, 5> s21arr2 = {tester_class()};
  EXPECT_EQ(compare_to_std(stdarr2, s21arr2, false), true);

  std::array<double, 5> stdarr3({});
  s21::array<double, 5> s21arr3({});
  EXPECT_EQ(compare_to_std(stdarr3, s21arr3, true), true);
}

TEST(array, iterators) {
  int a = 0;
  std::array<int, 10> stdarr1 = {1, 2, 3, 4, 5, 6, 7, 8, 9, 0};
  for (auto it = stdarr1.begin(); it != stdarr1.cend(); ++it) *it = a++;
  a = 0;
  s21::array<int, 10> s21arr1 = {1, 2, 3, 4, 5, 6, 7, 8, 9, 0};
  for (auto it = s21arr1.begin(); it != s21arr1.cend(); ++it) *it = a++;
  EXPECT_EQ(compare_to_std(stdarr1, s21arr1, true), true);

  std::array<int, 10>::const_iterator stditer1 = stdarr1.begin() + 1;
  s21::array<int, 10>::const_iterator s21iter1 = s21arr1.begin() + 1;
  EXPECT_EQ(*stditer1, *s21iter1);

  stditer1++;
  s21iter1++;
  EXPECT_EQ(*stditer1, *s21iter1);

  EXPECT_EQ(*++stditer1, *++s21iter1);

  EXPECT_EQ(*stditer1--, *s21iter1--);

  --stditer1;
  --s21iter1;

  EXPECT_EQ(*(stditer1 + 2), *(s21iter1 + 2));

  EXPECT_EQ(*(stditer1 - 1), *(s21iter1 - 1));

  EXPECT_EQ(stditer1[2], s21iter1[2]);

  EXPECT_EQ(*(stditer1 += 3), *(s21iter1 += 3));

  EXPECT_EQ(*(stditer1 -= 2), *(s21iter1 -= 2));

  EXPECT_EQ(s21iter1 - s21arr1.begin(), stditer1 - stdarr1.begin());

  std::array<int, 10>::iterator stditer2(stdarr1.data());
  s21::array<int, 10>::iterator s21iter2(s21arr1.data());

  EXPECT_EQ(stditer2 == stdarr1.cbegin(), s21iter2 == s21arr1.cbegin());

  EXPECT_EQ(stditer2 != stdarr1.cend(), s21iter2 != s21arr1.cend());

  EXPECT_EQ(stditer2 <= stdarr1.cend(), s21iter2 <= s21arr1.cend());
  EXPECT_EQ(stditer2 <= stdarr1.cbegin(), s21iter2 <= s21arr1.cbegin());

  EXPECT_EQ(stditer2 < stdarr1.cend(), s21iter2 < s21arr1.cend());
  EXPECT_EQ(stditer2 < stdarr1.cbegin(), s21iter2 < s21arr1.cbegin());

  EXPECT_EQ(stditer2 >= stdarr1.cend(), s21iter2 >= s21arr1.cend());
  EXPECT_EQ(stditer2 >= stdarr1.cbegin(), s21iter2 >= s21arr1.cbegin());

  EXPECT_EQ(stditer2 > stdarr1.cend(), s21iter2 > s21arr1.cend());
  EXPECT_EQ(stditer2 > stdarr1.cbegin(), s21iter2 > s21arr1.cbegin());

  std::array<tester_class, 1> stdarr2({tester_class()});
  s21::array<tester_class, 1> s21arr2({tester_class()});

  std::array<int, 10>::reverse_iterator stdreviter1 = stdarr1.rbegin();
  std::array<int, 10>::const_reverse_iterator stdreviter2 = stdarr1.rbegin();
  std::array<int, 10>::const_reverse_iterator stdreviter3 = stdarr1.crbegin();

  s21::array<int, 10>::reverse_iterator s21reviter1 = s21arr1.rbegin();
  s21::array<int, 10>::const_reverse_iterator s21reviter2 = s21arr1.rbegin();
  s21::array<int, 10>::const_reverse_iterator s21reviter3 = s21arr1.crbegin();

  EXPECT_EQ(*stdreviter1, *s21reviter1);
  for (; s21reviter2 != s21arr1.rend(); ++stdreviter2, ++s21reviter2)
    EXPECT_EQ(*stdreviter2, *s21reviter2);

  EXPECT_EQ(*stdreviter3, *s21reviter3);

  EXPECT_EQ(stdarr2.begin()->n, s21arr2.begin()->n);

  const std::array<int, 10>::const_reverse_iterator stdreviter4 =
      stdarr1.crend() - 1;
  const s21::array<int, 10>::const_reverse_iterator s21reviter4 =
      s21arr1.crend() - 1;
  EXPECT_EQ(*stdreviter4, *s21reviter4);
}

TEST(array, accessers) {
  std::array<int, 10> stdarr1({1, 2, 3, 4, 5, 6, 7, 8, 9, 0});
  s21::array<int, 10> s21arr1({1, 2, 3, 4, 5, 6, 7, 8, 9, 0});

  for (int i = 0; i < 10; ++i) {
    EXPECT_EQ(stdarr1.at(i), s21arr1.at(i));
    EXPECT_EQ(stdarr1[i], s21arr1[i]);
  }

  EXPECT_EQ(stdarr1.front(), s21arr1.front());

  EXPECT_EQ(stdarr1.back(), s21arr1.back());

  EXPECT_EQ(*(stdarr1.data() + 2), *(s21arr1.data() + 2));
}

TEST(array, modifiers) {
  std::array<int, 5> stdarr1({1, 2, 3});
  s21::array<int, 5> s21arr1({1, 2, 3});

  EXPECT_EQ(compare_to_std(stdarr1, s21arr1, true), true);

  std::array<int, 5> stdarr2({0, 1, 2, 3, 4});
  s21::array<int, 5> s21arr2({0, 1, 2, 3, 4});

  EXPECT_EQ(compare_to_std(stdarr2, s21arr2, true), true);

  stdarr1.swap(stdarr2);
  s21arr1.swap(s21arr2);

  EXPECT_EQ(compare_to_std(stdarr1, s21arr1, true), true);
  EXPECT_EQ(compare_to_std(stdarr2, s21arr2, true), true);

  stdarr1.fill(100);
  s21arr1.fill(100);

  EXPECT_EQ(compare_to_std(stdarr1, s21arr1, true), true);
}

// vector tests

TEST(vector, constructors) {
  std::vector<double> stdvec_inner(50, 5.0);
  s21::vector<double> s21vec_inner(50, 5.0);

  std::vector<std::vector<double>> stdvec1;
  s21::vector<std::vector<double>> s21vec1;
  EXPECT_EQ(compare_to_std(stdvec1, s21vec1, false), true);

  s21::vector<int> s21vec2(std::allocator<int>);

  std::vector<std::vector<double>> stdvec3(10, stdvec_inner);
  s21::vector<std::vector<double>> s21vec3(10, stdvec_inner);
  EXPECT_EQ(compare_to_std(stdvec3, s21vec3, false), true);
  for (int i = 0; i < 10; ++i)
    for (int j = 0; j < 50; ++j) EXPECT_EQ(s21vec3[i][j], 5.0);

  std::vector<double> stdvec4(stdvec_inner.begin(), stdvec_inner.end());
  s21::vector<double> s21vec4(stdvec_inner.begin(), stdvec_inner.end());
  EXPECT_EQ(compare_to_std(stdvec4, s21vec4, true), true);

  std::vector<std::vector<double>> stdvec5(stdvec3);
  s21::vector<std::vector<double>> s21vec5(s21vec3);

  std::vector<double> stdvec6(stdvec_inner);
  s21::vector<double> s21vec6(s21vec_inner);
  EXPECT_EQ(compare_to_std(stdvec6, s21vec6, true), true);

  std::vector<double> stdvec8(std::move(stdvec_inner));
  s21::vector<double> s21vec8(std::move(s21vec_inner));
  EXPECT_EQ(compare_to_std(stdvec8, s21vec8, true), true);
  EXPECT_EQ(compare_to_std(stdvec_inner, s21vec_inner, true), true);

  std::vector<double> stdvec10({1, 2, 3, 4, 5, 6, 7});
  s21::vector<double> s21vec10({1, 2, 3, 4, 5, 6, 7});

  EXPECT_EQ(compare_to_std(stdvec10, s21vec10, true), true);
}

TEST(vector, assignments) {
  std::vector<double> stdvec_inner({1, 2, 3, 4, 5, 6, 7});
  s21::vector<double> s21vec_inner({1, 2, 3, 4, 5, 6, 7});

  std::vector<tester_class> stdvec1(3);
  s21::vector<tester_class> s21vec1(3);

  std::vector<tester_class> stdvec2(4);
  stdvec2 = stdvec1;

  s21::vector<tester_class> s21vec2(4);
  s21vec2 = s21vec1;

  EXPECT_EQ(compare_to_std(stdvec2, s21vec2, false), true);

  std::vector<tester_class> stdvec3;
  s21::vector<tester_class> s21vec3;

  stdvec3 = std::move(stdvec2);
  s21vec3 = std::move(s21vec2);

  EXPECT_EQ(compare_to_std(stdvec3, s21vec3, false), true);

  std::vector<double> stdvec4;
  s21::vector<double> s21vec4;

  stdvec4 = {1, 2, 3, 4, 5};
  s21vec4 = {1, 2, 3, 4, 5};

  EXPECT_EQ(compare_to_std(stdvec4, s21vec4, true), true);

  stdvec4.assign(15, 20.5);
  s21vec4.assign(15, 20.5);

  EXPECT_EQ(compare_to_std(stdvec4, s21vec4, true), true);

  stdvec4.assign(3, 20.5);
  s21vec4.assign(3, 20.5);

  EXPECT_EQ(compare_to_std(stdvec4, s21vec4, true), true);

  stdvec4.assign(stdvec_inner.begin(), stdvec_inner.end());
  s21vec4.assign(s21vec_inner.begin(), s21vec_inner.end());

  EXPECT_EQ(compare_to_std(stdvec4, s21vec4, true), true);

  stdvec4.assign({0, 2, 0, 4});
  s21vec4.assign({0, 2, 0, 4});

  EXPECT_EQ(compare_to_std(stdvec4, s21vec4, true), true);
}

TEST(vector, accessers) {
  std::vector<int> stdvec1({1, 2, 3, 4, 5, 6, 7, 8, 9, 0});
  s21::vector<int> s21vec1({1, 2, 3, 4, 5, 6, 7, 8, 9, 0});

  EXPECT_EQ(stdvec1.get_allocator(), s21vec1.get_allocator());
  for (int i = 0; i < 10; ++i) {
    EXPECT_EQ(stdvec1.at(i), s21vec1.at(i));
    EXPECT_EQ(stdvec1[i], s21vec1[i]);
  }

  EXPECT_EQ(stdvec1.front(), s21vec1.front());

  EXPECT_EQ(stdvec1.back(), s21vec1.back());

  EXPECT_EQ(*(stdvec1.data() + 2), *(s21vec1.data() + 2));
}

TEST(vector, iterators) {
  int a = 0;
  std::vector<int> stdvec1(10);
  for (auto it = stdvec1.begin(); it != stdvec1.cend(); ++it) *it = a++;
  a = 0;
  s21::vector<int> s21vec1(10);
  for (auto it = s21vec1.begin(); it != s21vec1.cend(); ++it) *it = a++;
  EXPECT_EQ(compare_to_std(stdvec1, s21vec1, true), true);

  std::vector<int>::const_iterator stditer1 = stdvec1.begin() + 1;
  s21::vector<int>::const_iterator s21iter1 = s21vec1.begin() + 1;
  EXPECT_EQ(*stditer1, *s21iter1);

  stditer1++;
  s21iter1++;
  EXPECT_EQ(*stditer1, *s21iter1);

  EXPECT_EQ(*++stditer1, *++s21iter1);

  EXPECT_EQ(*stditer1--, *s21iter1--);

  --stditer1;
  --s21iter1;

  EXPECT_EQ(*(stditer1 + 2), *(s21iter1 + 2));

  EXPECT_EQ(*(stditer1 - 1), *(s21iter1 - 1));

  EXPECT_EQ(stditer1[2], s21iter1[2]);

  EXPECT_EQ(*(stditer1 += 3), *(s21iter1 += 3));

  EXPECT_EQ(*(stditer1 -= 2), *(s21iter1 -= 2));

  EXPECT_EQ(s21iter1 - s21vec1.begin(), stditer1 - stdvec1.begin());

  std::vector<int>::iterator stditer2{stdvec1.begin()};
  s21::vector<int>::iterator s21iter2{s21vec1.begin()};

  EXPECT_EQ(stditer2 == stdvec1.cbegin(), s21iter2 == s21vec1.cbegin());

  EXPECT_EQ(stditer2 != stdvec1.cend(), s21iter2 != s21vec1.cend());

  EXPECT_EQ(stditer2 <= stdvec1.cend(), s21iter2 <= s21vec1.cend());
  EXPECT_EQ(stditer2 <= stdvec1.cbegin(), s21iter2 <= s21vec1.cbegin());

  EXPECT_EQ(stditer2 < stdvec1.cend(), s21iter2 < s21vec1.cend());
  EXPECT_EQ(stditer2 < stdvec1.cbegin(), s21iter2 < s21vec1.cbegin());

  EXPECT_EQ(stditer2 >= stdvec1.cend(), s21iter2 >= s21vec1.cend());
  EXPECT_EQ(stditer2 >= stdvec1.cbegin(), s21iter2 >= s21vec1.cbegin());

  EXPECT_EQ(stditer2 > stdvec1.cend(), s21iter2 > s21vec1.cend());
  EXPECT_EQ(stditer2 > stdvec1.cbegin(), s21iter2 > s21vec1.cbegin());

  std::vector<tester_class> stdvec2(1);
  s21::vector<tester_class> s21vec2(1);

  std::vector<int>::reverse_iterator stdreviter1 = stdvec1.rbegin();
  std::vector<int>::const_reverse_iterator stdreviter2 = stdvec1.rbegin();
  std::vector<int>::const_reverse_iterator stdreviter3 = stdvec1.crbegin();

  s21::vector<int>::reverse_iterator s21reviter1 = s21vec1.rbegin();
  s21::vector<int>::const_reverse_iterator s21reviter2 = s21vec1.rbegin();
  s21::vector<int>::const_reverse_iterator s21reviter3 = s21vec1.crbegin();

  EXPECT_EQ(*stdreviter1, *s21reviter1);
  for (; s21reviter2 != s21vec1.rend(); ++stdreviter2, ++s21reviter2)
    EXPECT_EQ(*stdreviter2, *s21reviter2);

  EXPECT_EQ(*stdreviter3, *s21reviter3);

  EXPECT_EQ(stdvec2.begin()->n, s21vec2.begin()->n);

  s21::vector<int>::iterator s21iter0;

  const std::vector<int>::const_reverse_iterator stdreviter4 =
      stdvec1.crend() - 1;
  const s21::vector<int>::const_reverse_iterator s21reviter4 =
      s21vec1.crend() - 1;
  EXPECT_EQ(*stdreviter4, *s21reviter4);
}

TEST(vector, capacity) {
  std::vector<int> stdvec_inner(100);

  std::vector<std::vector<int>> stdvec1(100, stdvec_inner);
  s21::vector<std::vector<int>> s21vec1(100, stdvec_inner);
  EXPECT_EQ(compare_to_std(stdvec1, s21vec1, false), true);

  stdvec1.reserve(100);
  s21vec1.reserve(100);
  EXPECT_EQ(compare_to_std(stdvec1, s21vec1, false), true);

  stdvec1.reserve(120);
  s21vec1.reserve(120);
  EXPECT_EQ(compare_to_std(stdvec1, s21vec1, false), true);

  stdvec1.shrink_to_fit();
  s21vec1.shrink_to_fit();
  EXPECT_EQ(compare_to_std(stdvec1, s21vec1, false), true);

  stdvec1.shrink_to_fit();
  s21vec1.shrink_to_fit();
  EXPECT_EQ(compare_to_std(stdvec1, s21vec1, false), true);

  stdvec1.reserve(1);
  s21vec1.reserve(1);
  EXPECT_EQ(compare_to_std(stdvec1, s21vec1, false), true);

  stdvec1.clear();
  s21vec1.clear();
  EXPECT_EQ(compare_to_std(stdvec1, s21vec1, false), true);
}

TEST(vector, modifiers) {
  std::vector<tester_class> stdvec1(10);
  s21::vector<tester_class> s21vec1(10);
  EXPECT_EQ(compare_to_std(stdvec1, s21vec1, false), true);

  stdvec1.clear();
  s21vec1.clear();
  EXPECT_EQ(compare_to_std(stdvec1, s21vec1, false), true);

  stdvec1.clear();
  s21vec1.clear();
  EXPECT_EQ(compare_to_std(stdvec1, s21vec1, false), true);

  stdvec1.insert(stdvec1.end(), tester_class());
  s21vec1.insert(s21vec1.end(), tester_class());
  EXPECT_EQ(compare_to_std(stdvec1, s21vec1, false), true);

  std::vector<double> stdvec2(10);
  s21::vector<double> s21vec2(10);

  stdvec2.insert(stdvec2.begin() + 2, 3.4);
  s21vec2.insert(s21vec2.begin() + 2, 3.4);

  EXPECT_EQ(compare_to_std(stdvec2, s21vec2, true), true);

  stdvec2.insert(stdvec2.begin() + 2, 3.4);
  s21vec2.insert(s21vec2.begin() + 2, 3.4);

  EXPECT_EQ(compare_to_std(stdvec2, s21vec2, true), true);

  stdvec2.insert(stdvec2.begin() + 2, 100, 3.4);
  s21vec2.insert(s21vec2.begin() + 2, 100, 3.4);

  EXPECT_EQ(compare_to_std(stdvec2, s21vec2, true), true);

  stdvec2.insert(stdvec2.begin() + 2, {1, 2, 3, 4, 5});
  s21vec2.insert(s21vec2.begin() + 2, {1, 2, 3, 4, 5});

  EXPECT_EQ(compare_to_std(stdvec2, s21vec2, true), true);

  stdvec2.insert(stdvec2.begin() + 10, s21vec2.begin(), s21vec2.begin() + 2);
  s21vec2.insert(s21vec2.begin() + 10, stdvec2.begin(), stdvec2.begin() + 2);

  EXPECT_EQ(compare_to_std(stdvec2, s21vec2, true), true);

  tester_class tester_class1;

  std::vector<tester_class> stdvec3;
  s21::vector<tester_class> s21vec3;

  stdvec3.insert(stdvec3.end(), tester_class1);
  s21vec3.insert(s21vec3.end(), tester_class1);

  stdvec3.insert(stdvec3.end(), tester_class1);
  s21vec3.insert(s21vec3.end(), tester_class1);

  EXPECT_EQ(compare_to_std(stdvec3, s21vec3, false), true);

  stdvec3.reserve(10);
  s21vec3.reserve(10);

  stdvec3.insert(stdvec3.end(), tester_class1);
  s21vec3.insert(s21vec3.end(), tester_class1);

  EXPECT_EQ(compare_to_std(stdvec3, s21vec3, false), true);

  stdvec3.insert(stdvec3.end(), 3, tester_class1);
  s21vec3.insert(s21vec3.end(), 3, tester_class1);

  EXPECT_EQ(compare_to_std(stdvec3, s21vec3, false), true);

  stdvec3.shrink_to_fit();
  s21vec3.shrink_to_fit();

  stdvec3.insert(stdvec3.end(), s21vec3.begin() + 1, s21vec3.begin() + 2);
  s21vec3.insert(s21vec3.end(), stdvec3.begin() + 1, stdvec3.begin() + 2);

  EXPECT_EQ(compare_to_std(stdvec3, s21vec3, false), true);

  stdvec3.insert(stdvec3.end(), {tester_class1});
  s21vec3.insert(s21vec3.end(), {tester_class1});

  EXPECT_EQ(compare_to_std(stdvec3, s21vec3, false), true);

  stdvec2.erase(stdvec2.begin() + 3);
  s21vec2.erase(s21vec2.begin() + 3);

  EXPECT_EQ(compare_to_std(stdvec2, s21vec2, true), true);

  stdvec2.erase(stdvec2.begin() + 3, stdvec2.end());
  s21vec2.erase(s21vec2.begin() + 3, s21vec2.end());

  EXPECT_EQ(compare_to_std(stdvec2, s21vec2, true), true);

  stdvec3.erase(stdvec3.begin(), stdvec3.begin() + 2);
  s21vec3.erase(s21vec3.begin(), s21vec3.begin() + 2);

  EXPECT_EQ(compare_to_std(stdvec3, s21vec3, false), true);

  stdvec3.clear();
  s21vec3.clear();

  stdvec3.shrink_to_fit();
  s21vec3.shrink_to_fit();

  stdvec3.push_back(tester_class1);
  s21vec3.push_back(tester_class1);

  EXPECT_EQ(compare_to_std(stdvec3, s21vec3, false), true);

  stdvec3.push_back(tester_class1);
  s21vec3.push_back(tester_class1);

  EXPECT_EQ(compare_to_std(stdvec3, s21vec3, false), true);

  stdvec2.clear();
  s21vec2.clear();

  stdvec2.shrink_to_fit();
  s21vec2.shrink_to_fit();

  stdvec2.push_back(100);
  s21vec2.push_back(100);

  EXPECT_EQ(compare_to_std(stdvec2, s21vec2, true), true);

  stdvec2.push_back(100);
  s21vec2.push_back(100);

  EXPECT_EQ(compare_to_std(stdvec2, s21vec2, true), true);

  stdvec2.pop_back();
  s21vec2.pop_back();

  EXPECT_EQ(compare_to_std(stdvec2, s21vec2, true), true);

  stdvec2.resize(100);
  s21vec2.resize(100);

  EXPECT_EQ(compare_to_std(stdvec2, s21vec2, true), true);

  stdvec2.resize(2);
  s21vec2.resize(2);
  EXPECT_EQ(compare_to_std(stdvec2, s21vec2, true), true);

  stdvec2.resize(3, 10);
  s21vec2.resize(3, 10);

  EXPECT_EQ(compare_to_std(stdvec2, s21vec2, true), true);

  stdvec2.resize(1, 10);
  s21vec2.resize(1, 10);

  EXPECT_EQ(compare_to_std(stdvec2, s21vec2, true), true);

  std::vector<tester_class> stdvec4(15);
  std::vector<tester_class> stdvec5(4);
  stdvec4.reserve(100);

  s21::vector<tester_class> s21vec4(15);
  s21::vector<tester_class> s21vec5(4);
  s21vec4.reserve(100);

  s21vec4.swap(s21vec5);
  s21vec4.swap(s21vec5);

  EXPECT_EQ(compare_to_std(stdvec4, s21vec4, false), true);
  EXPECT_EQ(compare_to_std(stdvec5, s21vec5, false), true);

  int *m1 = new int[10];
  int *m2 = new int[10];

  s21vec3.emplace_back(m1, 12);
  stdvec3.emplace_back(m2, 12);

  s21vec3.emplace_back(tester_class());
  stdvec3.emplace_back(tester_class());

  int *m3 = new int[10];
  int *m4 = new int[10];

  s21vec3.emplace(s21vec3.begin() + 1, m3, 10);
  stdvec3.emplace(stdvec3.begin() + 1, m4, 10);

  EXPECT_EQ(compare_to_std(stdvec3, s21vec3, false), true);
  EXPECT_EQ(s21vec3.back().n, stdvec3.back().n);
}

TEST(vector, throws) {
  bool catched = false;
  s21::vector<int> s21vec1(100);
  try {
    s21vec1.reserve(s21vec1.max_size() + 1);
  } catch (...) {
    catched = true;
  }
  EXPECT_EQ(catched, true);
  catched = false;

  try {
    s21vec1.at(101) = 10;
  } catch (...) {
    catched = true;
  }
  EXPECT_EQ(catched, true);
  catched = false;

  s21::vector<throw_tester_class> s21vec2(15);
  s21::vector<throw_tester_class> s21vec3(15);

  std::vector<throw_tester_class> stdvec2(15);
  std::vector<throw_tester_class> stdvec3(15);

  s21vec3[1].n = 1;
  stdvec3[1].n = 1;

  throw_tester_class tester_class1;
  throw_tester_class tester_class2;
  throw_tester_class tester_class3;
  tester_class1.n = 1;
  tester_class2.n = 1;
  tester_class3.n = 1;
  try {
    s21vec2.push_back(tester_class1);
  } catch (...) {
    catched = true;
  }
  EXPECT_EQ(catched, true);
  catched = false;

  try {
    stdvec2.push_back(tester_class1);
  } catch (...) {
    catched = true;
  }
  EXPECT_EQ(catched, true);
  EXPECT_EQ(compare_to_std(stdvec2, s21vec2, false), true);
  catched = false;

  try {
    s21vec2.insert(s21vec2.begin() + 2, tester_class1);
  } catch (...) {
    catched = true;
  }
  EXPECT_EQ(catched, true);
  catched = false;

  try {
    stdvec2.insert(stdvec2.begin() + 2, tester_class1);
  } catch (...) {
    catched = true;
  }
  EXPECT_EQ(catched, true);
  EXPECT_EQ(compare_to_std(stdvec2, s21vec2, false), true);
  catched = false;

  try {
    s21vec2.insert(s21vec2.begin() + 2, 5, tester_class1);
  } catch (...) {
    catched = true;
  }
  EXPECT_EQ(catched, true);
  catched = false;

  try {
    stdvec2.insert(stdvec2.begin() + 2, 5, tester_class1);
  } catch (...) {
    catched = true;
  }
  EXPECT_EQ(catched, true);
  EXPECT_EQ(compare_to_std(stdvec2, s21vec2, false), true);
  catched = false;

  try {
    s21vec2.insert(s21vec2.begin() + 2, s21vec3.begin(), s21vec3.end());
  } catch (...) {
    catched = true;
  }
  EXPECT_EQ(catched, true);
  catched = false;

  try {
    stdvec2.insert(stdvec2.begin() + 2, stdvec3.begin(), stdvec3.end());
  } catch (...) {
    catched = true;
  }
  EXPECT_EQ(catched, true);
  EXPECT_EQ(compare_to_std(stdvec2, s21vec2, false), true);
  catched = false;

  for (int i = 1; i < 15; ++i) {
    s21vec2[i].n = 1;
    stdvec2[i].n = 1;
  }

  try {
    s21vec2.reserve(100);
  } catch (...) {
    catched = true;
  }
  EXPECT_EQ(catched, true);
  catched = false;

  try {
    s21vec2.reserve(100);
  } catch (...) {
    catched = true;
  }
  EXPECT_EQ(catched, true);
  EXPECT_EQ(compare_to_std(stdvec2, s21vec2, false), true);
  catched = false;
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
