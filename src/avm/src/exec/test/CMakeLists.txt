add_executable(exec_test exec_test.cpp)
target_link_libraries(exec_test PRIVATE gtest)
target_link_libraries(exec_test PRIVATE execute)
target_compile_definitions(exec_test PRIVATE TESTING=1)