#ifndef S21_ARRAY_H_
#define S21_ARRAY_H_

#include <iostream>

namespace s21 {

template <class T, std::size_t N>
struct array {
  using value_type = T;
  using size_type = std::size_t;
  using difference_type = std::ptrdiff_t;
  using reference = value_type &;
  using const_reference = const value_type &;
  using pointer = value_type *;
  using const_pointer = const value_type *;
  using iterator = value_type *;
  using const_iterator = const value_type *;
  using reverse_iterator = std::reverse_iterator<iterator>;
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;

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

  void fill(const T &other);
  void swap(array &other) noexcept(std::is_nothrow_swappable_v<T>);

  T _array[N ? N : 1];
};

template <class T, std::size_t N>
typename array<T, N>::reference array<T, N>::at(size_type pos) {
  if (pos >= N) throw std::out_of_range("Index out of range");
  return _array[pos];
}

template <class T, std::size_t N>
typename array<T, N>::const_reference array<T, N>::at(size_type pos) const {
  if (pos >= N) throw std::out_of_range("Index out of range");
  return _array[pos];
}

template <class T, std::size_t N>
typename array<T, N>::reference array<T, N>::operator[](size_type pos) {
  return _array[pos];
}

template <class T, std::size_t N>
typename array<T, N>::const_reference array<T, N>::operator[](
    size_type pos) const {
  return _array[pos];
}

template <class T, std::size_t N>
typename array<T, N>::reference array<T, N>::front() {
  return *begin();
}

template <class T, std::size_t N>
typename array<T, N>::const_reference array<T, N>::front() const {
  return *cbegin();
}

template <class T, std::size_t N>
typename array<T, N>::reference array<T, N>::back() {
  return *(end() - 1);
}

template <class T, std::size_t N>
typename array<T, N>::const_reference array<T, N>::back() const {
  return *(cend() - 1);
}

template <class T, std::size_t N>
T *array<T, N>::data() noexcept {
  return std::addressof(_array[0]);
}

template <class T, std::size_t N>
const T *array<T, N>::data() const noexcept {
  return std::addressof(_array[0]);
}

template <class T, std::size_t N>
typename array<T, N>::iterator array<T, N>::begin() noexcept {
  return iterator(std::addressof(_array[0]));
}

template <class T, std::size_t N>
typename array<T, N>::const_iterator array<T, N>::begin() const noexcept {
  return const_iterator(std::addressof(_array[0]));
}

template <class T, std::size_t N>
typename array<T, N>::const_iterator array<T, N>::cbegin() const noexcept {
  return const_iterator(std::addressof(_array[0]));
}

template <class T, std::size_t N>
typename array<T, N>::iterator array<T, N>::end() noexcept {
  return iterator(std::addressof(_array[N]));
}

template <class T, std::size_t N>
typename array<T, N>::const_iterator array<T, N>::end() const noexcept {
  return const_iterator(std::addressof(_array[N]));
}

template <class T, std::size_t N>
typename array<T, N>::const_iterator array<T, N>::cend() const noexcept {
  return const_iterator(std::addressof(_array[N]));
}

template <class T, std::size_t N>
typename array<T, N>::reverse_iterator array<T, N>::rbegin() noexcept {
  return reverse_iterator(end());
}

template <class T, std::size_t N>
typename array<T, N>::const_reverse_iterator array<T, N>::rbegin()
    const noexcept {
  return const_reverse_iterator(cend());
}

template <class T, std::size_t N>
typename array<T, N>::const_reverse_iterator array<T, N>::crbegin()
    const noexcept {
  return const_reverse_iterator(cend());
}

template <class T, std::size_t N>
typename array<T, N>::reverse_iterator array<T, N>::rend() noexcept {
  return reverse_iterator(begin());
}

template <class T, std::size_t N>
typename array<T, N>::const_reverse_iterator array<T, N>::rend()
    const noexcept {
  return const_reverse_iterator(cbegin());
}

template <class T, std::size_t N>
typename array<T, N>::const_reverse_iterator array<T, N>::crend()
    const noexcept {
  return const_reverse_iterator(cbegin());
}

template <class T, std::size_t N>
bool array<T, N>::empty() const noexcept {
  return N == 0;
}

template <class T, std::size_t N>
typename array<T, N>::size_type array<T, N>::size() const noexcept {
  return N;
}

template <class T, std::size_t N>
typename array<T, N>::size_type array<T, N>::max_size() const noexcept {
  return N;
}

template <class T, std::size_t N>
void array<T, N>::fill(const T &other) {
  std::fill_n(begin(), N, other);
}

template <class T, std::size_t N>
void array<T, N>::swap(array &other) noexcept(std::is_nothrow_swappable_v<T>) {
  std::swap_ranges(begin(), end(), other.begin());
}
}  // namespace s21

#endif  // S21_ARRAY_H_
