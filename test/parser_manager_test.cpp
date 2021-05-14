#include <gtest/gtest.h>
#include "../include/parser_manager.h"
#include <sstream>

TEST(ParserManager, init_library_functions) {
    InitLibraryFunctions();
    std::string expected = "-----------     Scope #0     -----------\n"
                    "[library function] \"print\" (line 0) (scope 0)\n"
                    "[library function] \"input\" (line 0) (scope 0)\n"
                    "[library function] \"objectmemberkeys\" (line 0) (scope 0)\n"
                    "[library function] \"objecttotalmembers\" (line 0) (scope 0)\n"
                    "[library function] \"objectcopy\" (line 0) (scope 0)\n"
                    "[library function] \"totalarguments\" (line 0) (scope 0)\n"
                    "[library function] \"argument\" (line 0) (scope 0)\n"
                    "[library function] \"typeof\" (line 0) (scope 0)\n"
                    "[library function] \"strtonum\" (line 0) (scope 0)\n"
                    "[library function] \"sqrt\" (line 0) (scope 0)\n"
                    "[library function] \"cos\" (line 0) (scope 0)\n"
                    "[library function] \"sin\" (line 0) (scope 0)\n";

    std::stringstream actual;
    LogSymbolTable(actual);   
    GTEST_ASSERT_EQ(expected, actual.str());     
}

#ifdef TESTING
int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
#endif