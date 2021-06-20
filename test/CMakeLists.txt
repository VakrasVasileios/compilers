add_executable(avm_test avm_test.cpp)
target_link_libraries(avm_test PRIVATE gtest)
target_link_libraries(avm_test PRIVATE exec)
target_compile_definitions(avm_test PRIVATE TESTING=1)