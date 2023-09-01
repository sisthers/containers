#ifndef S21_VECTOR_H_
#define S21_VECTOR_H_

#include <iostream>

namespace s21 {

template <class T, class Allocator = std::allocator<T>>
class vector {
 private:
  using allocator_traits = std::allocator_traits<Allocator>;

  template <bool IsConst>
  class common_iterator {
   public:
    using difference_type = ptrdiff_t;
    using pointer =
        std::conditional_t<IsConst, typename allocator_traits::const_pointer,
                           typename allocator_traits::pointer>;
    using reference = std::conditional_t<IsConst, const T &, T &>;
    using value_type = T;
    using iterator_category = std::random_access_iterator_tag;

    template <bool IsConstFriend>
    friend class common_iterator;

    explicit common_iterator(pointer ptr) noexcept;
    common_iterator() noexcept;

    operator common_iterator<true>() const noexcept;

    reference operator*() const;
    pointer operator->() const;
    reference operator[](difference_type n) const;

    common_iterator<IsConst> &operator++();
    common_iterator<IsConst> &operator--();
    common_iterator<IsConst> operator++(int);
    common_iterator<IsConst> operator--(int);
    common_iterator<IsConst> operator+(difference_type n) const;
    common_iterator<IsConst> operator-(difference_type n) const;
    common_iterator<IsConst> &operator+=(difference_type n);
    common_iterator<IsConst> &operator-=(difference_type n);
    difference_type operator-(const common_iterator<true> &other) const;

    inline bool operator==(const common_iterator<true> &other) const noexcept;
    inline bool operator!=(const common_iterator<true> &other) const noexcept;
    inline bool operator<(const common_iterator<true> &other) const noexcept;
    inline bool operator>(const common_iterator<true> &other) const noexcept;
    inline bool operator<=(const common_iterator<true> &other) const noexcept;
    inline bool operator>=(const common_iterator<true> &other) const noexcept;

   private:
    pointer _ptr;
  };

 public:
  using value_type = T;
  using allocator_type = Allocator;
  using size_type = size_t;
  using difference_type = ptrdiff_t;
  using reference = value_type &;
  using const_reference = const value_type &;
  using pointer = typename allocator_traits::pointer;
  using const_pointer = typename allocator_traits::const_pointer;
  using iterator = common_iterator<false>;
  using const_iterator = common_iterator<true>;
  using reverse_iterator = std::reverse_iterator<iterator>;
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;

  vector() noexcept(noexcept(Allocator()));
  explicit vector(const Allocator &alloc) noexcept;
  vector(size_type count, const T &value, const Allocator &alloc = Allocator());
  explicit vector(size_type count, const Allocator &alloc = Allocator());
  template <class InputIt,
            std::enable_if_t<!std::is_integral<InputIt>::value, bool> = true>
  vector(InputIt first, InputIt last, const Allocator &alloc = Allocator());
  vector(const vector &other);
  vector(const vector &other, const Allocator &alloc);
  vector(vector &&other) noexcept;
  vector(vector &&other, const Allocator &alloc);
  vector(std::initializer_list<T> init, const Allocator &alloc = Allocator());
  ~vector();
  vector &operator=(const vector &other);
  vector &operator=(vector &&other) noexcept(noexcept(
      allocator_traits::propagate_on_container_move_assignment::value ||
      allocator_traits::is_always_equal::value));
  vector &operator=(std::initializer_list<T> ilist);
  void assign(size_type count, const T &value);
  template <class InputIt,
            std::enable_if_t<!std::is_integral<InputIt>::value, bool> = true>
  void assign(InputIt first, InputIt last);
  void assign(std::initializer_list<T> ilist);

  allocator_type get_allocator() const noexcept;
  reference at(size_type pos);
  const_reference at(size_type pos) const;
  reference operator[](size_type pos);
  const_reference operator[](size_type pos) const;
  reference front();
  const_reference front() const;
  reference back();
  const_reference back() const;
  T *data() noexcept;
  const T *data() const noexcept;

  iterator begin() noexcept;
  const_iterator begin() const noexcept;
  const_iterator cbegin() const noexcept;
  iterator end() noexcept;
  const_iterator end() const noexcept;
  const_iterator cend() const noexcept;
  reverse_iterator rbegin() noexcept;
  const_reverse_iterator rbegin() const noexcept;
  const_reverse_iterator crbegin() const noexcept;
  reverse_iterator rend() noexcept;
  const_reverse_iterator rend() const noexcept;
  const_reverse_iterator crend() const noexcept;

  bool empty() const noexcept;
  size_type size() const noexcept;
  size_type max_size() const noexcept;
  void reserve(size_type new_cap);
  size_type capacity() const noexcept;
  void shrink_to_fit();

  void clear() noexcept;
  iterator insert(const_iterator pos, const T &value);
  iterator insert(const_iterator pos, T &&value);
  iterator insert(const_iterator pos, size_type count, const T &value);
  template <class InputIt,
            std::enable_if_t<!std::is_integral<InputIt>::value, bool> = true>
  iterator insert(const_iterator pos, InputIt first, InputIt last);
  iterator insert(const_iterator pos, std::initializer_list<T> ilist);
  void erase(const_iterator pos);
  void erase(const_iterator first, const_iterator last);
  template <class... Args>
  iterator emplace(const_iterator pos, Args &&...args);
  void push_back(const T &value);
  void push_back(T &&value);
  template <class... Args>
  void emplace_back(Args &&...args);
  void pop_back();
  void resize(size_type count);
  void resize(size_type count, const value_type &value);
  void swap(vector &other) noexcept(
      noexcept(allocator_traits::propagate_on_container_swap::value ||
               allocator_traits::is_always_equal::value));

 private:
  void move_to_new_arr(T *new_arr, size_type pos, size_type count,
                       size_type shift, size_type capacity_to_deallocate);
  void move_to_new_arr(T *new_arr, size_type pos,
                       size_type capacity_to_deallocate, T &&value);
  void copy_to_new_arr(T *new_arr, size_type pos, size_type count,
                       size_type capacity_to_deallocate, const T &value);
  template <class InputIt>
  void copy_to_new_arr(T *new_arr, size_type pos, InputIt first, InputIt last,
                       size_type capacity_to_deallocate);
  void shift_elements(const_iterator pos, size_type shift, bool to_right);
  size_type calculate_capacity(size_type count);
  void deallocate_old_arr();

  size_type _size;
  size_type _capacity;
  pointer _arr;
  Allocator _allocator;
};

template <class T, class Allocator>
vector<T, Allocator>::vector() noexcept(noexcept(Allocator()))
    : _size(0), _capacity(0), _arr(nullptr), _allocator(Allocator()) {}

template <class T, class Allocator>
vector<T, Allocator>::vector(const Allocator &alloc) noexcept
    : _size(0), _capacity(0), _arr(nullptr), _allocator(alloc) {}

template <class T, class Allocator>
vector<T, Allocator>::vector(size_type count, const T &value,
                             const Allocator &alloc)
    : _size(0), _capacity(0), _arr(nullptr), _allocator(alloc) {
  reserve(count);
  while (_size != count) push_back(value);
}

template <class T, class Allocator>
vector<T, Allocator>::vector(size_type count, const Allocator &alloc)
    : _size(0), _capacity(0), _arr(nullptr), _allocator(alloc) {
  reserve(count);
  while (_size != count) push_back(T());
}

template <class T, class Allocator>
template <class InputIt,
          std::enable_if_t<!std::is_integral<InputIt>::value, bool>>
vector<T, Allocator>::vector(InputIt first, InputIt last,
                             const Allocator &alloc)
    : _size(0), _capacity(0), _arr(nullptr), _allocator(alloc) {
  reserve(last - first);
  for (; first != last; ++first) push_back(*first);
}

template <class T, class Allocator>
vector<T, Allocator>::vector(const vector &other)
    : _size(0),
      _capacity(0),
      _arr(nullptr),
      _allocator(allocator_traits::select_on_container_copy_construction(
          other.get_allocator())) {
  reserve(other._capacity);
  for (auto it = other.begin(); it != other.end(); ++it) push_back(*it);
}

template <class T, class Allocator>
vector<T, Allocator>::vector(const vector &other, const Allocator &alloc)
    : _size(0), _capacity(0), _arr(nullptr), _allocator(alloc) {
  reserve(other._capacity);
  for (auto it = other.begin(); it != other.end(); ++it) push_back(*it);
}

template <class T, class Allocator>
vector<T, Allocator>::vector(vector &&other) noexcept
    : _size(other._size),
      _capacity(other._capacity),
      _arr(other._arr),
      _allocator(std::move(other._allocator)) {
  other._size = 0;
  other._capacity = 0;
  other._arr = nullptr;
}

template <class T, class Allocator>
vector<T, Allocator>::vector(vector &&other, const Allocator &alloc)
    : _size(0), _capacity(0), _arr(nullptr), _allocator(alloc) {
  if (_allocator == other.get_allocator()) {
    reserve(other._capacity);
    for (auto it = other.begin(); it != other.end(); ++it) {
      push_back(std::move(*it));
    }
    other.clear();
    other.shrink_to_fit();
  } else {
    other._size = 0;
    other._capacity = 0;
    _arr = other._arr;
  }
  other._arr = nullptr;
}

template <class T, class Allocator>
vector<T, Allocator>::vector(std::initializer_list<T> init,
                             const Allocator &alloc)
    : _size(0), _capacity(0), _arr(nullptr), _allocator(alloc) {
  reserve(init.size());
  for (auto it = init.begin(); it != init.end(); ++it) push_back(*it);
}

template <class T, class Allocator>
vector<T, Allocator>::~vector() {
  clear();
  shrink_to_fit();
}

template <class T, class Allocator>
vector<T, Allocator> &vector<T, Allocator>::operator=(const vector &other) {
  if (this == &other) return *this;
  clear();
  if (allocator_traits::propagate_on_container_copy_assignment::value) {
    shrink_to_fit();
    _allocator = other._allocator;
  }
  reserve(other._capacity);
  for (auto it = other.begin(); it != other.end(); ++it) push_back(*it);
  return *this;
}

template <class T, class Allocator>
vector<T, Allocator> &vector<T, Allocator>::operator=(vector &&other) noexcept(
    noexcept(allocator_traits::propagate_on_container_move_assignment::value ||
             allocator_traits::is_always_equal::value)) {
  clear();
  shrink_to_fit();
  if (allocator_traits::propagate_on_container_move_assignment::value ||
      _allocator == other._allocator) {
    _size = other._size;
    _capacity = other._capacity;
    _arr = other._arr;
    other._arr = nullptr;
    other._capacity = 0;
    other._size = 0;
    if (allocator_traits::propagate_on_container_move_assignment::value)
      _allocator = other._allocator;
  } else {
    for (auto it = other.begin(); it != other.end(); ++it)
      push_back(std::move(*it));
    other.clear();
    other.shrink_to_fit();
  }
  return *this;
}

template <class T, class Allocator>
vector<T, Allocator> &vector<T, Allocator>::operator=(
    std::initializer_list<T> ilist) {
  clear();
  reserve(ilist.size());
  for (auto it = ilist.begin(); it != ilist.end(); ++it) push_back(*it);
  return *this;
}

template <class T, class Allocator>
void vector<T, Allocator>::assign(size_type count, const T &value) {
  clear();
  reserve(count);
  while (_size != count) push_back(value);
}

template <class T, class Allocator>
template <class InputIt,
          std::enable_if_t<!std::is_integral<InputIt>::value, bool>>
void vector<T, Allocator>::assign(InputIt first, InputIt last) {
  clear();
  reserve(last - first);
  for (; first != last; ++first) push_back(*first);
}

template <class T, class Allocator>
void vector<T, Allocator>::assign(std::initializer_list<T> ilist) {
  clear();
  reserve(ilist.size());
  for (auto it = ilist.begin(); it != ilist.end(); ++it) push_back(*it);
}

template <class T, class Allocator>
typename vector<T, Allocator>::allocator_type
vector<T, Allocator>::get_allocator() const noexcept {
  return _allocator;
}

template <class T, class Allocator>
typename vector<T, Allocator>::reference vector<T, Allocator>::at(
    size_type pos) {
  if (pos >= _size) throw std::out_of_range("Index out of range");
  return *(_arr + pos);
}

template <class T, class Allocator>
typename vector<T, Allocator>::const_reference vector<T, Allocator>::at(
    size_type pos) const {
  if (pos >= _size) throw std::out_of_range("Index out of range");
  return *(_arr + pos);
}

template <class T, class Allocator>
typename vector<T, Allocator>::reference vector<T, Allocator>::operator[](
    size_type pos) {
  return *(_arr + pos);
}

template <class T, class Allocator>
typename vector<T, Allocator>::const_reference vector<T, Allocator>::operator[](
    size_type pos) const {
  return *(_arr + pos);
}

template <class T, class Allocator>
typename vector<T, Allocator>::reference vector<T, Allocator>::front() {
  return *_arr;
}

template <class T, class Allocator>
typename vector<T, Allocator>::const_reference vector<T, Allocator>::front()
    const {
  return *_arr;
}

template <class T, class Allocator>
typename vector<T, Allocator>::reference vector<T, Allocator>::back() {
  return *(_arr + _size - 1);
}

template <class T, class Allocator>
typename vector<T, Allocator>::const_reference vector<T, Allocator>::back()
    const {
  return *(_arr + _size - 1);
}

template <class T, class Allocator>
T *vector<T, Allocator>::data() noexcept {
  return _arr;
}

template <class T, class Allocator>
const T *vector<T, Allocator>::data() const noexcept {
  return _arr;
}

template <class T, class Allocator>
typename vector<T, Allocator>::iterator vector<T, Allocator>::begin() noexcept {
  iterator iter(_arr);
  return iter;
}

template <class T, class Allocator>
typename vector<T, Allocator>::const_iterator vector<T, Allocator>::begin()
    const noexcept {
  const_iterator iter(_arr);
  return iter;
}

template <class T, class Allocator>
typename vector<T, Allocator>::const_iterator vector<T, Allocator>::cbegin()
    const noexcept {
  const_iterator iter(_arr);
  return iter;
}

template <class T, class Allocator>
typename vector<T, Allocator>::iterator vector<T, Allocator>::end() noexcept {
  iterator iter(_arr + _size);
  return iter;
}

template <class T, class Allocator>
typename vector<T, Allocator>::const_iterator vector<T, Allocator>::end()
    const noexcept {
  const_iterator iter(_arr + _size);
  return iter;
}

template <class T, class Allocator>
typename vector<T, Allocator>::const_iterator vector<T, Allocator>::cend()
    const noexcept {
  const_iterator iter(_arr + _size);
  return iter;
}

template <class T, class Allocator>
typename vector<T, Allocator>::reverse_iterator
vector<T, Allocator>::rbegin() noexcept {
  return reverse_iterator(end());
}

template <class T, class Allocator>
typename vector<T, Allocator>::const_reverse_iterator
vector<T, Allocator>::rbegin() const noexcept {
  return const_reverse_iterator(end());
}

template <class T, class Allocator>
typename vector<T, Allocator>::const_reverse_iterator
vector<T, Allocator>::crbegin() const noexcept {
  return const_reverse_iterator(cend());
}

template <class T, class Allocator>
typename vector<T, Allocator>::reverse_iterator
vector<T, Allocator>::rend() noexcept {
  return reverse_iterator(begin());
}

template <class T, class Allocator>
typename vector<T, Allocator>::const_reverse_iterator
vector<T, Allocator>::rend() const noexcept {
  return const_reverse_iterator(begin());
}

template <class T, class Allocator>
typename vector<T, Allocator>::const_reverse_iterator
vector<T, Allocator>::crend() const noexcept {
  return const_reverse_iterator(cbegin());
}

template <class T, class Allocator>
bool vector<T, Allocator>::empty() const noexcept {
  return _size == 0;
}

template <class T, class Allocator>
typename vector<T, Allocator>::size_type vector<T, Allocator>::size()
    const noexcept {
  return _size;
}

template <class T, class Allocator>
typename vector<T, Allocator>::size_type vector<T, Allocator>::max_size()
    const noexcept {
  return allocator_traits::max_size(_allocator);
}

template <class T, class Allocator>
void vector<T, Allocator>::reserve(size_type new_cap) {
  if (new_cap > max_size()) {
    throw std::length_error("Max size is bigger then new capacity");
  }
  if (new_cap <= _capacity) {
    return;
  }
  T *new_arr = allocator_traits::allocate(_allocator, new_cap);
  move_to_new_arr(new_arr, 0, _size, 0, new_cap);
  deallocate_old_arr();
  _arr = new_arr;
  _capacity = new_cap;
}

template <class T, class Allocator>
typename vector<T, Allocator>::size_type vector<T, Allocator>::capacity()
    const noexcept {
  return _capacity;
}

template <class T, class Allocator>
void vector<T, Allocator>::shrink_to_fit() {
  if (_capacity == _size) {
    return;
  }
  if (_size != 0) {
    T *new_arr = allocator_traits::allocate(_allocator, _size);
    move_to_new_arr(new_arr, 0, _size, 0, _size);
    deallocate_old_arr();
    _arr = new_arr;
  } else {
    deallocate_old_arr();
  }
  _capacity = _size;
}

template <class T, class Allocator>
void vector<T, Allocator>::clear() noexcept {
  while (!empty()) pop_back();
}

template <class T, class Allocator>
typename vector<T, Allocator>::iterator vector<T, Allocator>::insert(
    const_iterator pos, const T &value) {
  size_type insert_pos = pos - cbegin();
  if (_size == _capacity) {
    size_type new_cap = calculate_capacity(1);
    T *new_arr = allocator_traits::allocate(_allocator, new_cap);
    move_to_new_arr(new_arr, 0, insert_pos, 0, new_cap);
    copy_to_new_arr(new_arr, insert_pos, 1, new_cap, value);
    move_to_new_arr(new_arr, insert_pos + 1, _size - insert_pos, -1, new_cap);
    deallocate_old_arr();
    _arr = new_arr;
    _capacity = new_cap;
  } else {
    shift_elements(pos, 1, true);
    allocator_traits::construct(_allocator, _arr + insert_pos, value);
  }
  ++_size;
  iterator iter = begin() + insert_pos;
  return iter;
}

template <class T, class Allocator>
typename vector<T, Allocator>::iterator vector<T, Allocator>::insert(
    const_iterator pos, T &&value) {
  size_type insert_pos = pos - cbegin();
  if (_size == _capacity) {
    size_type new_cap = calculate_capacity(1);
    T *new_arr = allocator_traits::allocate(_allocator, new_cap);
    move_to_new_arr(new_arr, 0, insert_pos, 0, new_cap);
    move_to_new_arr(new_arr, insert_pos, new_cap, std::move(value));
    move_to_new_arr(new_arr, insert_pos + 1, _size - insert_pos, -1, new_cap);
    deallocate_old_arr();
    _arr = new_arr;
    _capacity = new_cap;
  } else {
    shift_elements(pos, 1, true);
    allocator_traits::construct(_allocator, _arr + insert_pos,
                                std::move_if_noexcept(value));
  }
  ++_size;
  iterator iter = begin() + insert_pos;
  return iter;
}

template <class T, class Allocator>
typename vector<T, Allocator>::iterator vector<T, Allocator>::insert(
    const_iterator pos, size_type count, const T &value) {
  size_type insert_pos = pos - cbegin();
  if (_size + count > _capacity) {
    size_type new_cap = calculate_capacity(count);
    T *new_arr = allocator_traits::allocate(_allocator, new_cap);
    move_to_new_arr(new_arr, 0, insert_pos, 0, new_cap);
    copy_to_new_arr(new_arr, insert_pos, count, new_cap, value);
    move_to_new_arr(new_arr, insert_pos + count, _size - insert_pos, -count,
                    new_cap);
    deallocate_old_arr();
    _arr = new_arr;
    _capacity = new_cap;
  } else {
    shift_elements(pos, count, true);
    for (size_type j = insert_pos; j < insert_pos + count; ++j)
      allocator_traits::construct(_allocator, _arr + j, value);
  }
  _size += count;
  iterator iter = begin() + insert_pos;
  return iter;
}

template <class T, class Allocator>
template <class InputIt,
          std::enable_if_t<!std::is_integral<InputIt>::value, bool>>
typename vector<T, Allocator>::iterator vector<T, Allocator>::insert(
    const_iterator pos, InputIt first, InputIt last) {
  size_type count = last - first;
  size_type insert_pos = pos - cbegin();
  if (_size + count > _capacity) {
    size_type new_cap = calculate_capacity(count);
    T *new_arr = allocator_traits::allocate(_allocator, new_cap);
    move_to_new_arr(new_arr, 0, insert_pos, 0, new_cap);
    copy_to_new_arr(new_arr, insert_pos, first, last, new_cap);
    move_to_new_arr(new_arr, insert_pos + count, _size - insert_pos, -count,
                    new_cap);
    deallocate_old_arr();
    _arr = new_arr;
    _capacity = new_cap;
  } else {
    shift_elements(pos, count, true);
    for (size_type j = insert_pos; first != last; ++first, ++j)
      allocator_traits::construct(_allocator, _arr + j, *first);
  }
  _size += count;
  iterator iter = begin() + insert_pos;
  return iter;
}

template <class T, class Allocator>
typename vector<T, Allocator>::iterator vector<T, Allocator>::insert(
    const_iterator pos, std::initializer_list<T> ilist) {
  size_type count = ilist.size();
  size_type insert_pos = pos - cbegin();
  if (_size + count > _capacity) {
    size_type new_cap = calculate_capacity(count);
    T *new_arr = allocator_traits::allocate(_allocator, new_cap);
    move_to_new_arr(new_arr, 0, insert_pos, 0, new_cap);
    copy_to_new_arr(new_arr, insert_pos, ilist.begin(), ilist.end(), new_cap);
    move_to_new_arr(new_arr, insert_pos + count, _size - insert_pos, -count,
                    new_cap);
    deallocate_old_arr();
    _arr = new_arr;
    _capacity = new_cap;
  } else {
    shift_elements(pos, count, true);
    auto it = ilist.begin();
    for (size_type j = insert_pos; it != ilist.end(); ++it, ++j)
      allocator_traits::construct(_allocator, _arr + j, *it);
  }
  _size += count;
  iterator iter = begin() + insert_pos;
  return iter;
}

template <class T, class Allocator>
void vector<T, Allocator>::erase(const_iterator pos) {
  allocator_traits::destroy(_allocator, _arr + (pos - cbegin()));
  shift_elements(pos, 1, false);
  --_size;
}

template <class T, class Allocator>
void vector<T, Allocator>::erase(const_iterator first, const_iterator last) {
  for (auto it = first; it != last; ++it)
    allocator_traits::destroy(_allocator, _arr + (it - cbegin()));
  shift_elements(first, last - first, false);
  _size -= (last - first);
}
template <class T, class Allocator>
template <class... Args>
typename vector<T, Allocator>::iterator vector<T, Allocator>::emplace(
    const_iterator pos, Args &&...args) {
  size_type emplace_pos = pos - cbegin();
  if (_size == _capacity) {
    size_type new_cap = calculate_capacity(1);
    T *new_arr = allocator_traits::allocate(_allocator, new_cap);
    move_to_new_arr(new_arr, 0, emplace_pos, 0, new_cap);
    try {
      allocator_traits::construct(_allocator, new_arr + emplace_pos,
                                  std::forward<Args>(args)...);
    } catch (...) {
      allocator_traits::deallocate(_allocator, new_arr, new_cap);
      throw;
    }
    move_to_new_arr(new_arr, emplace_pos + 1, _size - emplace_pos, -1, new_cap);
    deallocate_old_arr();
    _arr = new_arr;
    _capacity = new_cap;
  } else {
    shift_elements(pos, 1, true);
    allocator_traits::construct(_allocator, _arr + emplace_pos,
                                std::forward<Args>(args)...);
  }
  ++_size;
  iterator iter = begin() + emplace_pos;
  return iter;
}

template <class T, class Allocator>
void vector<T, Allocator>::push_back(const T &value) {
  emplace_back(value);
}

template <class T, class Allocator>
void vector<T, Allocator>::push_back(T &&value) {
  emplace_back(std::move(value));
}

template <class T, class Allocator>
template <class... Args>
void vector<T, Allocator>::emplace_back(Args &&...args) {
  if (_capacity == 0)
    reserve(1);
  else if (_size == _capacity)
    reserve(_capacity * 2);
  try {
    allocator_traits::construct(_allocator, _arr + _size,
                                std::forward<Args>(args)...);
  } catch (...) {
    shrink_to_fit();
    throw;
  }
  ++_size;
}

template <class T, class Allocator>
void vector<T, Allocator>::pop_back() {
  allocator_traits::destroy(_allocator, _arr + _size - 1);
  --_size;
}

template <class T, class Allocator>
void vector<T, Allocator>::resize(size_type count) {
  if (count <= _size) {
    while (count != _size) pop_back();
  } else {
    reserve(count);
    while (count != _size) push_back(T());
  }
}

template <class T, class Allocator>
void vector<T, Allocator>::resize(size_type count, const value_type &value) {
  if (count <= _size) {
    while (count != _size) pop_back();
  } else {
    reserve(count);
    while (count != _size) push_back(value);
  }
}

template <class T, class Allocator>
void vector<T, Allocator>::swap(vector &other) noexcept(
    noexcept(allocator_traits::propagate_on_container_swap::value ||
             allocator_traits::is_always_equal::value)) {
  std::swap(other._size, _size);
  std::swap(other._capacity, _capacity);
  std::swap(other._arr, _arr);
  if (allocator_traits::propagate_on_container_swap::value)
    std::swap(other._allocator, _allocator);
}

template <class T, class Allocator>
void vector<T, Allocator>::move_to_new_arr(T *new_arr, size_type pos,
                                           size_type count, size_type shift,
                                           size_type capacity_to_deallocate) {
  size_type i = pos;
  try {
    for (; i < pos + count; ++i) {
      allocator_traits::construct(_allocator, new_arr + i,
                                  std::move_if_noexcept(_arr[i + shift]));
    }
  } catch (...) {
    for (size_type j = 0; j < i; ++j) {
      allocator_traits::destroy(_allocator, new_arr + j);
    }
    allocator_traits::deallocate(_allocator, new_arr, capacity_to_deallocate);
    throw;
  }
}

template <class T, class Allocator>
void vector<T, Allocator>::move_to_new_arr(T *new_arr, size_type pos,
                                           size_type capacity_to_deallocate,
                                           T &&value) {
  try {
    allocator_traits::construct(_allocator, new_arr + pos,
                                std::move_if_noexcept(value));
  } catch (...) {
    allocator_traits::deallocate(_allocator, new_arr, capacity_to_deallocate);
    throw;
  }
}

template <class T, class Allocator>
void vector<T, Allocator>::copy_to_new_arr(T *new_arr, size_type pos,
                                           size_type count,
                                           size_type capacity_to_deallocate,
                                           const T &value) {
  size_type i = pos;
  try {
    for (; i < pos + count; ++i) {
      allocator_traits::construct(_allocator, new_arr + i, value);
    }
  } catch (...) {
    for (size_type j = 0; j < i; ++j) {
      allocator_traits::destroy(_allocator, new_arr + j);
    }
    allocator_traits::deallocate(_allocator, new_arr, capacity_to_deallocate);
    throw;
  }
}

template <class T, class Allocator>
template <class InputIt>
void vector<T, Allocator>::copy_to_new_arr(T *new_arr, size_type pos,
                                           InputIt first, InputIt last,
                                           size_type capacity_to_deallocate) {
  size_type i = pos;
  try {
    for (; first != last; ++first, ++i) {
      allocator_traits::construct(_allocator, new_arr + i, *first);
    }
  } catch (...) {
    for (size_type j = 0; j < i; ++j) {
      allocator_traits::destroy(_allocator, new_arr + j);
    }
    allocator_traits::deallocate(_allocator, new_arr, capacity_to_deallocate);
    throw;
  }
}

template <class T, class Allocator>
void vector<T, Allocator>::shift_elements(const_iterator pos, size_type shift,
                                          bool to_right) {
  if (to_right) {
    size_type i = _size + shift - 1;
    const_reverse_iterator end_it(pos);
    for (auto it = rbegin(); it != end_it; ++it, --i) {
      allocator_traits::construct(_allocator, _arr + i,
                                  std::move_if_noexcept(*it));
      allocator_traits::destroy(_allocator, _arr + i - shift);
    }
  } else {
    size_type i = pos - cbegin();
    for (iterator it(_arr + i); it != end() - shift; ++it, ++i) {
      allocator_traits::construct(_allocator, _arr + i,
                                  std::move_if_noexcept(*(it + shift)));
      allocator_traits::destroy(_allocator, _arr + i + shift);
    }
  }
}

template <class T, class Allocator>
void vector<T, Allocator>::deallocate_old_arr() {
  if (_arr != nullptr) {
    size_type temp_size = _size;
    if (std::is_nothrow_move_constructible<T>::value == false) {
      clear();
      _size = temp_size;
    }
    allocator_traits::deallocate(_allocator, _arr, _capacity);
  }
  _arr = nullptr;
}

template <class T, class Allocator>
typename vector<T, Allocator>::size_type
vector<T, Allocator>::calculate_capacity(size_type count) {
  size_type result = _capacity * 2;
  if (result < _size + count) result = _size + count;
  return result;
}

template <class T, class Allocator>
template <bool IsConst>
vector<T, Allocator>::common_iterator<IsConst>::common_iterator(
    pointer ptr) noexcept
    : _ptr(ptr) {}

template <class T, class Allocator>
template <bool IsConst>
vector<T, Allocator>::common_iterator<IsConst>::common_iterator() noexcept
    : _ptr(nullptr) {}

template <class T, class Allocator>
template <bool IsConst>
vector<T, Allocator>::common_iterator<IsConst>::operator common_iterator<true>()
    const noexcept {
  common_iterator<true> iter(_ptr);
  return iter;
}

template <class T, class Allocator>
template <bool IsConst>
typename vector<T, Allocator>::template common_iterator<IsConst>::reference
vector<T, Allocator>::common_iterator<IsConst>::operator*() const {
  return *_ptr;
}

template <class T, class Allocator>
template <bool IsConst>
typename vector<T, Allocator>::template common_iterator<IsConst>::pointer
vector<T, Allocator>::common_iterator<IsConst>::operator->() const {
  return _ptr;
}

template <class T, class Allocator>
template <bool IsConst>
typename vector<T, Allocator>::template common_iterator<IsConst>::reference
vector<T, Allocator>::common_iterator<IsConst>::operator[](
    difference_type n) const {
  return *(_ptr + n);
}

template <class T, class Allocator>
template <bool IsConst>
typename vector<T, Allocator>::template common_iterator<IsConst>
    &vector<T, Allocator>::common_iterator<IsConst>::operator++() {
  ++_ptr;
  return *this;
}

template <class T, class Allocator>
template <bool IsConst>
typename vector<T, Allocator>::template common_iterator<IsConst>
    &vector<T, Allocator>::common_iterator<IsConst>::operator--() {
  --_ptr;
  return *this;
}

template <class T, class Allocator>
template <bool IsConst>
typename vector<T, Allocator>::template common_iterator<IsConst>
vector<T, Allocator>::common_iterator<IsConst>::operator++(int) {
  auto tmp = *this;
  ++_ptr;
  return tmp;
}

template <class T, class Allocator>
template <bool IsConst>
typename vector<T, Allocator>::template common_iterator<IsConst>
vector<T, Allocator>::common_iterator<IsConst>::operator--(int) {
  auto tmp = *this;
  --_ptr;
  return tmp;
}

template <class T, class Allocator>
template <bool IsConst>
typename vector<T, Allocator>::template common_iterator<IsConst>
vector<T, Allocator>::common_iterator<IsConst>::operator+(
    difference_type n) const {
  auto tmp = *this;
  tmp._ptr += n;
  return tmp;
}

template <class T, class Allocator>
template <bool IsConst>
typename vector<T, Allocator>::template common_iterator<IsConst>
vector<T, Allocator>::common_iterator<IsConst>::operator-(
    difference_type n) const {
  auto tmp = *this;
  tmp._ptr -= n;
  return tmp;
}

template <class T, class Allocator>
template <bool IsConst>
typename vector<T, Allocator>::template common_iterator<IsConst> &
vector<T, Allocator>::common_iterator<IsConst>::operator+=(difference_type n) {
  _ptr += n;
  return *this;
}

template <class T, class Allocator>
template <bool IsConst>
typename vector<T, Allocator>::template common_iterator<IsConst> &
vector<T, Allocator>::common_iterator<IsConst>::operator-=(difference_type n) {
  _ptr -= n;
  return *this;
}

template <class T, class Allocator>
template <bool IsConst>
typename vector<T,
                Allocator>::template common_iterator<IsConst>::difference_type
vector<T, Allocator>::common_iterator<IsConst>::operator-(
    const common_iterator<true> &other) const {
  return _ptr - other._ptr;
}

template <class T, class Allocator>
template <bool IsConst>
inline bool vector<T, Allocator>::common_iterator<IsConst>::operator==(
    const common_iterator<true> &other) const noexcept {
  return _ptr == other._ptr;
}

template <class T, class Allocator>
template <bool IsConst>
inline bool vector<T, Allocator>::common_iterator<IsConst>::operator!=(
    const common_iterator<true> &other) const noexcept {
  return _ptr != other._ptr;
}

template <class T, class Allocator>
template <bool IsConst>
inline bool vector<T, Allocator>::common_iterator<IsConst>::operator<(
    const common_iterator<true> &other) const noexcept {
  return _ptr < other._ptr;
}

template <class T, class Allocator>
template <bool IsConst>
inline bool vector<T, Allocator>::common_iterator<IsConst>::operator<=(
    const common_iterator<true> &other) const noexcept {
  return _ptr <= other._ptr;
}

template <class T, class Allocator>
template <bool IsConst>
inline bool vector<T, Allocator>::common_iterator<IsConst>::operator>(
    const common_iterator<true> &other) const noexcept {
  return _ptr > other._ptr;
}

template <class T, class Allocator>
template <bool IsConst>
inline bool vector<T, Allocator>::common_iterator<IsConst>::operator>=(
    const common_iterator<true> &other) const noexcept {
  return _ptr >= other._ptr;
}

}  // namespace s21

#endif  // S21_VECTOR_H_
