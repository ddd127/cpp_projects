cmake_minimum_required(VERSION 3.17)
project(c___projects)

set(CMAKE_CXX_STANDARD 17)

include_directories(data_structures)

add_executable(c___projects main.cpp data_structures/randomized_queue.h data_structures/hash_set.h data_structures/policy.h useful_things/olymp_template.cpp)
