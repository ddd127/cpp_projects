#pragma once

#include <random>
#include <cstddef>
#include <vector>
#include <stack>
#include <algorithm>

template <class Key>
class randomized_queue
{
public:
    class iterator {
    public:
        using value_type = Key;
        using reference = Key &;
        using pointer = Key *;
        using difference_type = int;
        using iterator_category = std::forward_iterator_tag;

        iterator(randomized_queue<Key> *ptr, size_t start_size)
                : m_sequence(ptr->gen_vector(start_size))
                , m_pos(m_sequence.begin())
                , m_queue_ptr(ptr)
        {}

        iterator &operator++() {
            m_pos++;
            return *this;
        }

        iterator operator++(int) {
            iterator result = *this;
            ++(*this);
            return result;
        }

        Key &operator*() const {
            return m_queue_ptr->m_vector[*m_pos];
        }

        Key *operator->() const {
            return m_queue_ptr->m_vector + (*m_pos);
        }

        bool operator==(const iterator &other) const {
            return m_queue_ptr == other.m_queue_ptr && (*m_pos) == *(other.m_pos);
        }

        bool operator!=(const iterator &other) const {
            return !(*this == other);
        }

    private:
        std::vector<size_t> m_sequence;
        std::vector<size_t>::iterator m_pos;
        randomized_queue<Key> *m_queue_ptr;
    };

    class const_iterator {
    public:
        using value_type = const Key;
        using reference = const Key &;
        using pointer = Key const *;
        using difference_type = int;
        using iterator_category = std::forward_iterator_tag;

        const_iterator(const randomized_queue<Key> *ptr, size_t start_size)
                : m_sequence(ptr->gen_vector(start_size))
                , m_pos(m_sequence.begin())
                , m_queue_ptr(ptr)
        { }

        const_iterator &operator++() {
            m_pos++;
            return *this;
        }

        const_iterator operator++(int) {
            const_iterator result = *this;
            ++(*this);
            return result;
        }

        const Key &operator*() const {
            return m_queue_ptr->m_vector[(*m_pos)];
        }

        const Key *operator->() const {
            return m_queue_ptr->m_vector + (*m_pos);
        }

        bool operator==(const const_iterator &other) const {
            return m_queue_ptr == other.m_queue_ptr && (*m_pos) == *other.m_pos;
        }

        bool operator!=(const const_iterator &other) const {
            return !(*this == other);
        }

    private:
        std::vector<size_t> m_sequence;
        std::vector<size_t>::iterator m_pos;
        const randomized_queue<Key> *m_queue_ptr;
    };

    randomized_queue()
            : m_vector(0)
            , m_gen(std::random_device()())
            , m_dist(0, 0)
    { }

    iterator begin() {
        return iterator(this, size());
    }

    const_iterator begin() const {
        return const_iterator(this, size());
    }

    const_iterator cbegin() const {
        return const_iterator(this, size());
    }

    iterator end() {
        return iterator(this, 0);
    }

    const_iterator end() const {
        return const_iterator(this, 0);
    }

    const_iterator cend() const {
        return const_iterator(this, 0);
    }

    void enqueue(const Key& item) {
        m_vector.emplace_back(item);
    }

    void enqueue(Key&& item) {
        m_vector.emplace_back(std::move(item));
    }

    const Key& sample() const {
        return m_vector[rand_pos()];
    }

    Key dequeue() {
        size_t pos = rand_pos();
        std::swap(m_vector[pos], m_vector[size() - 1]);
        Key result = std::move(m_vector[size() - 1]);
        m_vector.pop_back();
        return result;
    }

    size_t size() const {
        return m_vector.size();
    }

    bool empty() const {
        return m_vector.empty();
    }

private:
    std::vector<size_t> gen_vector(size_t size) const {
        std::vector<size_t> vector(size);
        for (size_t i = 0; i < size; ++i) {
            vector[i] = i;
        }
        std::shuffle(vector.begin(), vector.end(), m_gen);
        vector.emplace_back(static_cast<size_t>(-1));
        return vector;
    }

    size_t rand_pos() const {
        if (static_cast<size_t>(m_dist.max()) != size() - 1) {
            m_dist = std::uniform_int_distribution<>(0, size() - 1);
        }
        return static_cast<size_t>(m_dist(m_gen));
    }

    std::vector<Key> m_vector;
    mutable std::mt19937 m_gen;
    mutable std::uniform_int_distribution<> m_dist;
};