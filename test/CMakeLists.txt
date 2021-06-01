link_directories(${compilers_SOURCE_DIR}/libs)
add_subdirectory(${compilers_SOURCE_DIR}/util/exec test)

add_executable(syntax_analysis_test syntax_analysis_test.cpp)
target_link_libraries(syntax_analysis_test PRIVATE gtest)
target_link_libraries(syntax_analysis_test PRIVATE exec)
target_compile_definitions(syntax_analysis_test PRIVATE TESTING=1)

add_executable(intermediate_code_test intermediate_code_test.cpp)
target_link_libraries(intermediate_code_test PRIVATE gtest)
target_link_libraries(intermediate_code_test PRIVATE exec)
target_compile_definitions(intermediate_code_test PRIVATE TESTING=1)

