#ifndef VECTOR_H_
#define VECTOR_H_

#include <cstddef> // std::size_t, std::ptrdiff_t
#include <iterator> // std::reverse_iterator
#include <memory> // std::allocator, std::allocator_traits

// This file must be interpreted in a "unsafe by default" mode. That is, for a
// function to be considered safe, it needs to be explicitly marked as SAFE.
// Such a function may use unsafe constructs in its implementation without
// annotation, but the compiler writer is responsible for ensuring its memory
// safety.
//
// The 'SAFE' macro indicates a function is safe
#define SAFE
#define safe(x)

namespace stdx {
  // Allocator template arguments are not pertinent to this experiment so they
  // are excluded.
  template <class T, class Allocator = std::allocator<T>> class vector
  {
  public:
    // types
    using value_type = T;
    using allocator_type = Allocator;
    using pointer = typename std::allocator_traits<Allocator>::pointer;
    using const_pointer = typename std::allocator_traits<Allocator>::const_pointer;
    using reference = value_type &;
    using const_reference = const value_type &;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using iterator = T*;
    using const_iterator = T const *;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    // construct/copy/destroy
    // SAFE constexpr vector() noexcept(noexcept(Allocator()))
    // : vector(Allocator()) {}

    // NOTE: The above implementation is incorrect. If `Allocator`'s default
    // constructor can have undefined behavior, then so can this function. We
    // need a way to inherit the safety of that operation.

    constexpr vector()
      noexcept(noexcept(Allocator()))
      safe(safe(Allocator()) && safe(vector(Allocator())))
        : vector(Allocator()) {}

    // constexpr explicit vector(const Allocator&) noexcept;
    // NOTE: The above constructor can be marked noexcept because copy- and
    // move-construction of allocators must have no throw contracts.
    constexpr explicit vector(const Allocator &) noexcept;
    constexpr explicit vector(size_type n, const Allocator& = Allocator());
    constexpr vector(size_type n, const T& value, const Allocator& = Allocator());
    template<class InputIterator>
    constexpr vector(InputIterator first, InputIterator last, const Allocator& = Allocator());
    template<container-compatible-range <T> R>
    constexpr vector(from_range_t, R&& rg, const Allocator& = Allocator());
    constexpr vector(const vector& x);
    § 24.3.11.1 931
    ©ISO/IEC N4950
    constexpr vector(vector&&) noexcept;
    constexpr vector(const vector&, const type_identity_t<Allocator>&);
    constexpr vector(vector&&, const type_identity_t<Allocator>&);
    constexpr vector(initializer_list<T>, const Allocator& = Allocator());
    constexpr ~vector();
    constexpr vector& operator=(const vector& x);
    constexpr vector& operator=(vector&& x)
    noexcept(allocator_traits<Allocator>::propagate_on_container_move_assignment::value ||
    allocator_traits<Allocator>::is_always_equal::value);
    constexpr vector& operator=(initializer_list<T>);
    template<class InputIterator>
    constexpr void assign(InputIterator first, InputIterator last);
    template<container-compatible-range <T> R>
    constexpr void assign_range(R&& rg);
    constexpr void assign(size_type n, const T& u);
    constexpr void assign(initializer_list<T>);
    constexpr allocator_type get_allocator() const noexcept;
  };
}

#endif
