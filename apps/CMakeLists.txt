foreach(app demo_fraction demo_expression demo_simplify)
add_executable(${app} ${app}.cpp)
target_link_libraries(${app} PRIVATE ratexpr)
endforeach()