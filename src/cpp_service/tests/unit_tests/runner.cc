/* Generated file, do not edit */

#ifndef CXXTEST_RUNNING
#define CXXTEST_RUNNING
#endif

#define _CXXTEST_HAVE_STD
#include <cxxtest/TestListener.h>
#include <cxxtest/TestTracker.h>
#include <cxxtest/TestRunner.h>
#include <cxxtest/RealDescriptions.h>
#include <cxxtest/TestMain.h>
#include <cxxtest/ErrorPrinter.h>

int main( int argc, char *argv[] ) {
 int status;
    CxxTest::ErrorPrinter tmp;
    CxxTest::RealWorldDescription::_worldName = "cxxtest";
    status = CxxTest::Main< CxxTest::ErrorPrinter >( tmp, argc, argv );
    return status;
}
bool suite_SequiturCxxUnitTest_init = false;
#include "src/cpp_service/tests/unit_tests/sequiturCxxUnitTest.h"

static SequiturCxxUnitTest suite_SequiturCxxUnitTest;

static CxxTest::List Tests_SequiturCxxUnitTest = { 0, 0 };
CxxTest::StaticSuiteDescription suiteDescription_SequiturCxxUnitTest( "./src/cpp_service/tests/unit_tests/sequiturCxxUnitTest.h", 54, "SequiturCxxUnitTest", suite_SequiturCxxUnitTest, Tests_SequiturCxxUnitTest );

static class TestDescription_suite_SequiturCxxUnitTest_test_smallAlphabetSmallInput : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_SequiturCxxUnitTest_test_smallAlphabetSmallInput() : CxxTest::RealTestDescription( Tests_SequiturCxxUnitTest, suiteDescription_SequiturCxxUnitTest, 61, "test_smallAlphabetSmallInput" ) {}
 void runTest() { suite_SequiturCxxUnitTest.test_smallAlphabetSmallInput(); }
} testDescription_suite_SequiturCxxUnitTest_test_smallAlphabetSmallInput;

static class TestDescription_suite_SequiturCxxUnitTest_test_variableAlphabetAverageInput : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_SequiturCxxUnitTest_test_variableAlphabetAverageInput() : CxxTest::RealTestDescription( Tests_SequiturCxxUnitTest, suiteDescription_SequiturCxxUnitTest, 62, "test_variableAlphabetAverageInput" ) {}
 void runTest() { suite_SequiturCxxUnitTest.test_variableAlphabetAverageInput(); }
} testDescription_suite_SequiturCxxUnitTest_test_variableAlphabetAverageInput;

static class TestDescription_suite_SequiturCxxUnitTest_test_broaderVariationAllParameters : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_SequiturCxxUnitTest_test_broaderVariationAllParameters() : CxxTest::RealTestDescription( Tests_SequiturCxxUnitTest, suiteDescription_SequiturCxxUnitTest, 63, "test_broaderVariationAllParameters" ) {}
 void runTest() { suite_SequiturCxxUnitTest.test_broaderVariationAllParameters(); }
} testDescription_suite_SequiturCxxUnitTest_test_broaderVariationAllParameters;

static class TestDescription_suite_SequiturCxxUnitTest_test_smallAlphabetHugeInput : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_SequiturCxxUnitTest_test_smallAlphabetHugeInput() : CxxTest::RealTestDescription( Tests_SequiturCxxUnitTest, suiteDescription_SequiturCxxUnitTest, 64, "test_smallAlphabetHugeInput" ) {}
 void runTest() { suite_SequiturCxxUnitTest.test_smallAlphabetHugeInput(); }
} testDescription_suite_SequiturCxxUnitTest_test_smallAlphabetHugeInput;

#include <cxxtest/Root.cpp>
const char* CxxTest::RealWorldDescription::_worldName = "cxxtest";
