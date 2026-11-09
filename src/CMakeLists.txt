add_library(ratexpr STATIC fraction.cpp expression.cpp simplification.cpp)
target_include_directories(ratexpr PUBLIC ${CMAKE_SOURCE_DIR}/include)