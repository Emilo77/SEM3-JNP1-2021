cmake_minimum_required(VERSION 3.18)
project(_JNP_Zad03Final)

set(CMAKE_CXX_STANDARD 20)

include_directories(.)

add_executable(_JNP_Zad03Final
        fuzzy.h
        fuzzy_example.cc
        fuzzy_test.cc
        fuzzy_test_external.cc)
