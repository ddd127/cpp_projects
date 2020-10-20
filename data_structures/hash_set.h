#pragma once

#include <functional>
#include <memory>
#include "policy.h"

template <
        class Key,
        class CollisionPolicy = LinearProbing,
        class Hash = std::hash<Key>,
        class Equal = std::equal_to<Key>
>
class HashSet
{
public:
    // types
    using key_type = Key;
    using value_type = Key;
    using size_type = size_t;
    using difference_type = ptrdiff_t;
    using hasher = Hash;
    using key_equal = Equal;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;

    class iterator
    {
    public:
        using value_type = key_type;
        using reference = key_type&;
        using pointer = key_type*;
        using difference_type = int;
        using iterator_category = std::forward_iterator_tag;

        explicit iterator(const HashSet<key_type, CollisionPolicy, Hash, Equal> &hashSet,
                          const int &pos = 0)
                : hashSet_(&hashSet),
                  pos_(pos)
        {
            for (; pos_ != hashSet_->m_array.size() && hashSet_->m_is_valid[pos_] != VALID; ++pos_);
        }

        iterator& operator=(const iterator& other) = default;

        iterator& operator++() { // ++i
            for (++pos_; pos_ != hashSet_->m_array.size() && hashSet_->m_is_valid[pos_] != VALID; ++pos_);
            return *this;
        }

        iterator operator++(int) { // i++
            iterator ret = *this;
            ++(*this);
            return ret;
        }

        const value_type& operator*() const { return *(hashSet_->m_array[pos_]); }
        pointer operator->() const { return hashSet_->m_array[pos_]; }
        bool operator==(const iterator &other) const { return hashSet_ == other.hashSet_ && pos_ == other.pos_; }
        bool operator!=(const iterator &other) const { return !(*this == other); }

    private:
        const HashSet<key_type, CollisionPolicy, Hash, Equal> *hashSet_;
        size_type pos_;
    };

    using iterator = iterator;
    using const_iterator = iterator;

    explicit HashSet(size_type expected_max_size = START_SIZE,
                     const hasher & hash = hasher(),
                     const key_equal & equal = key_equal())
            : m_size(0)
            , m_deleted(0)
            , m_hash(hash)
            , m_equal(equal)
            , m_array(expected_max_size > START_SIZE ? expected_max_size : START_SIZE)
            , m_is_valid(expected_max_size > START_SIZE ? expected_max_size : START_SIZE, INVALID)
    { }

    template <class InputIt>
    HashSet(InputIt first, InputIt last,
            size_type expected_max_size = START_SIZE,
            const hasher & hash = hasher(),
            const key_equal & equal = key_equal())
            : HashSet(expected_max_size, hash, equal)
    {
        for (; first != last; ++first) {
            insert(*first);
        }
    }

    HashSet(const HashSet &other)
            : HashSet(other.begin(), other.end(), other.size())
    { }

    HashSet(HashSet && other) noexcept
            : m_size(other.m_size)
            , m_deleted(other.m_deleted)
            , m_hash(std::move(other.m_hash))
            , m_equal(std::move(other.m_equal))
            , m_array(std::move(other.m_array))
            , m_is_valid(std::move(other.m_is_valid))
    { }

    HashSet(std::initializer_list<key_type> init,
            size_type expected_max_size = START_SIZE,
            const hasher & hash = hasher(),
            const key_equal & equal = key_equal())
            : HashSet(init.begin(), init.end(), expected_max_size, hash, equal)
    { }

    HashSet & operator = (const HashSet & other) = default;

    HashSet & operator = (HashSet && other) noexcept = default;

    HashSet & operator = (std::initializer_list<key_type> init)
    {
        HashSet<key_type>(init).swap(*this);
    }

    iterator begin() noexcept
    {
        return iterator(*this, 0);
    }
    const_iterator begin() const noexcept
    {
        return iterator(*this, 0);
    }
    const_iterator cbegin() const noexcept
    {
        return iterator(*this, 0);
    }

    iterator end() noexcept
    {
        return iterator(*this, m_array.size());
    }
    const_iterator end() const noexcept
    {
        return const_iterator(*this, m_array.size());
    }
    const_iterator cend() const noexcept
    {
        return const_iterator(*this, m_array.size());
    }

    bool empty() const
    {
        return m_size == 0u;
    }
    size_type size() const
    {
        return m_size;
    }
    size_type max_size() const
    {
        return m_array.size() / 4;
    }

    void clear()
    {
        for (auto it = begin(); it != end(); ++it) {
            erase(it);
        }
        rehash(0);
    }
    std::pair<iterator, bool> insert(const key_type & key)
    {
        check_size();
        size_t pos = find_place(key);
        if (m_is_valid[pos] == VALID) {
            return std::make_pair(const_iterator(*this, pos), false);
        }
        return insert_new(pos, std::move(std::unique_ptr<key_type>(new key_type(key))));
    }
    std::pair<iterator, bool> insert(key_type && key)
    {
        check_size();
        size_t pos = find_place(key);
        if (m_is_valid[pos] == VALID) {
            return std::make_pair(const_iterator(*this, pos), false);
        }
        return insert_new(pos, std::move(std::unique_ptr<key_type>(new key_type(std::move(key)))));
    }
    iterator insert(const_iterator, const key_type & key)
    {
        return insert(key).first;
    }
    iterator insert(const_iterator, key_type && key)
    {
        return insert(key).first;
    }
    template <class InputIt>
    void insert(InputIt first, InputIt last)
    {
        for (; first != last; ++first) {
            insert(*first);
        }
    }
    void insert(std::initializer_list<key_type> init)
    {
        insert(init.begin(), init.end());
    }

    // construct element in-place, no copy or move operations are performed;
    // element's constructor is called with exact same arguments as `emplace` method
    // (using `std::forward<Args>(args)...`)
    template <class... Args>
    std::pair<iterator, bool> emplace(Args &&... args)
    {
        check_size();
        auto p = std::unique_ptr<key_type>(new key_type(std::forward<Args>(args)...));
        size_t pos = find_place(*p);
        if (m_is_valid[pos] == VALID) {
            return std::make_pair(const_iterator(*this, pos), false);
        }
        return insert_new(pos, std::move(p));
    }
    template <class... Args>
    iterator emplace_hint(const_iterator, Args &&... args)
    {
        return emplace(std::forward<Args>(args)...).first;
    }

    iterator erase(const_iterator pos)
    {
        erase(*pos);
        return ++pos;
    }
    iterator erase(const_iterator first, const_iterator last)
    {
        for (; first != last; ++first) {
            erase(*first);
        }
        return last;
    }
    size_type erase(const key_type & key)
    {
        size_t pos = find_place(key);
        if (m_is_valid[pos] != VALID) {
            return 0u;
        }
        --m_size;
        ++m_deleted;
        m_array[pos] = nullptr;
        m_is_valid[pos] = DELETED;
        return 1u;
    }

    // exchanges the contents of the container with those of other;
    // does not invoke any move, copy, or swap operations on individual elements
    void swap(HashSet && other) noexcept
    {
        std::swap(m_size, other.m_size);
        std::swap(m_deleted, other.m_deleted);
        std::swap(m_hash, other.m_hash);
        std::swap(m_equal, other.m_equal);
        std::swap(m_array, other.m_array);
        std::swap(m_is_valid, other.m_is_valid);
    }

    size_type count(const key_type & key) const
    {
        return find(key) == end() ? 0 : 1;
    }
    iterator find(const key_type & key)
    {
        size_t pos = find_place(key);
        return (m_is_valid[pos] == VALID) ? const_iterator (*this, pos) : end();
    }
    const_iterator find(const key_type & key) const
    {
        size_t pos = find_place(key);
        return (m_is_valid[pos] == VALID) ? const_iterator (*this, pos) : end();
    }
    bool contains(const key_type & key) const
    {
        return m_is_valid[find_place(key)] == VALID;
    }
    std::pair<iterator, iterator> equal_range(const key_type & key)
    {
        const_iterator first = find(key);
        const_iterator second = find(key);
        return std::make_pair(first, second == end() ? second : ++second);
    }
    std::pair<const_iterator, const_iterator> equal_range(const key_type & key) const
    {
        const_iterator first = find(key);
        const_iterator second = find(key);
        return std::make_pair(first, second == end() ? second : ++second);
    }

    size_type bucket_count() const
    {
        return m_array.size();
    }
    size_type max_bucket_count() const
    {
        return m_array.size();
    }
    size_type bucket_size(const size_type) const
    {
        return 1;
    }
    size_type bucket(const key_type &) const
    {
        return 0;
    }

    float load_factor() const
    {
        return static_cast<double>(m_size) / static_cast<double>(m_array.size());
    }
    float max_load_factor() const
    {
        return 1;
    }
    void rehash(const size_type)
    {
        reserve(0);
    }
    void reserve(size_type count)
    {
        std::vector<std::unique_ptr<key_type>> items;
        for (size_t i = 0; i < m_array.size(); ++i) {
            if (m_is_valid[i] == VALID) {
                items.emplace_back(std::move(m_array[i]));
            }
        }
        m_array = std::vector<std::unique_ptr<key_type>>(std::max(m_array.size(), 4 * (m_size + count)));
        m_is_valid = std::vector<Status>(m_array.size(), INVALID);
        m_deleted = 0;
        for (auto& it : items) {
            size_t pos = find_place(*it);
            m_array[pos] = std::move(it);
            m_is_valid[pos] = VALID;
        }
    }

    // compare two containers contents
    friend bool operator == (const HashSet & lhs, const HashSet & rhs)
    {
        for (auto it_l = lhs.begin(), it_r = rhs.begin();
             it_l != lhs.end() || it_r != rhs.end();
             ++it_l, ++it_r) {
            if (it_l == lhs.end() || it_r == rhs.end() || !(*it_l == *it_r)) {
                return false;
            }
        }
        return true;
    }
    friend bool operator != (const HashSet & lhs, const HashSet & rhs)
    {
        return !(lhs == rhs);
    }

private:
    static constexpr size_t START_SIZE = 100;
    size_type m_size;
    size_type m_deleted;
    hasher m_hash;
    key_equal m_equal;
    std::vector<std::unique_ptr<key_type>> m_array;
    std::vector<Status> m_is_valid;

    std::pair<iterator, bool> insert_new(size_t pos, std::unique_ptr<key_type> && ptr) {
        m_array[pos] = std::move(ptr);
        m_is_valid[pos] = VALID;
        ++m_size;
        return std::make_pair(iterator(*this, pos), true);
    }

    size_t find_place(const key_type& key) const {
        Node ptr;
        ptr.pos = m_hash(key) % m_array.size();
        while (m_is_valid[ptr.pos] != INVALID && (m_is_valid[ptr.pos] != VALID || !m_equal(*(m_array[ptr.pos]), key))) {
            CollisionPolicy::next(ptr, m_array.size());
        }
        return ptr.pos;
    }

    void check_size() {
        if (m_array.size() == 0) {
            reserve(100);
        }
        if (2 * (m_size + m_deleted) > m_array.size()) {
            reserve(m_size);
        }
    }
};
