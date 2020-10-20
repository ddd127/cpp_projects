#pragma once

#include <cstddef>

enum Status
{
    VALID,
    INVALID,
    DELETED
};

struct Node {
    size_t pos = 0;
    size_t offset = 1;
};

struct LinearProbing {
    static void next(Node & node, size_t capacity) {
        node.pos += node.offset;
        node.pos %= capacity;
    }
};

struct QuadraticProbing {
    static void next(Node & node, size_t capacity) {
        node.pos += node.offset * node.offset - (node.offset - 1) * (node.offset - 1);
        ++node.offset;
        node.pos %= capacity;
    }
};
