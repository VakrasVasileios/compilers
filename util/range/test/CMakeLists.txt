add_executable(range_test range_test.cpp)
target_link_libraries(range_test PRIVATE gtest)
target_compile_definitions(range_test PRIVATE TESTING=1)