#include <cxxtest/TestSuite.h>
#include <random>
#include <cstdio>
#include <cmath>
#include <string>

#include "../../sequitur/Sequitur.h"

using namespace timeSeries;
using namespace std;

static void callSequitur(
    const unsigned int &rInAlphabetSize,
    const unsigned int &rInInputSize)
{
    // create a sequitur algorithm instance
    Sequitur sequitur;

    // random device
    std::random_device random_device;
    std::mt19937 *rs = new std::mt19937(random_device());

    // buffers for word generation
    char lineBuffer[2];
    lineBuffer[1] = '\0';
    std::string line;

    // original generated inputstring
    std::string inputString = "";

    // random distribution to generate words
    std::uniform_int_distribution<> *randomDist = new std::uniform_int_distribution<>(0, rInAlphabetSize - 1);

    for (unsigned int j = 0; j < rInInputSize; j++)
    {
        // build random word
        int rnd = (*randomDist)(*rs);
        lineBuffer[0] = 'a' + (char)rnd;
        line.assign(lineBuffer);
        inputString += line;

        // insert word into the sequitur algorithm
        sequitur.insertSymbol(line.c_str());
    }

    //  test through the cxxtest api
    std::string generated_string = "";
    sequitur.expandGrammar("~0", generated_string);

    // test results
    TS_ASSERT(generated_string == inputString);
}

class SequiturCxxUnitTest : public CxxTest::TestSuite
{
  public:
    SequiturCxxUnitTest();
    ~SequiturCxxUnitTest();

    // TODO XXX Add a test to check the stage output
    void test_smallAlphabetSmallInput();
    void test_variableAlphabetAverageInput();
    void test_broaderVariationAllParameters();
    void test_smallAlphabetHugeInput();
};

SequiturCxxUnitTest::SequiturCxxUnitTest()
{
}

SequiturCxxUnitTest::~SequiturCxxUnitTest()
{
}

void SequiturCxxUnitTest::test_smallAlphabetSmallInput()
{
    TS_TRACE("Starting smallAlphabetSmallInput test");

    unsigned int N = 100;
    unsigned int inputSize = 20;
    unsigned int minAlphabetSize = 4;
    unsigned int maxAlphabetSize = 4;

    for (unsigned int i = 0; i < N; i++)
    {
        for (unsigned int alphabetSize = minAlphabetSize; alphabetSize <= maxAlphabetSize; alphabetSize++)
        {
            callSequitur(alphabetSize, inputSize);
        }
    }

    TS_TRACE("Finishing smallAlphabetSmallInput test");
}

void SequiturCxxUnitTest::test_variableAlphabetAverageInput()
{
    TS_TRACE("Starting variableAlphabetAverageInput test");

    unsigned int N = 100;
    unsigned int inputSize = 50;
    unsigned int minAlphabetSize = 2;
    unsigned int maxAlphabetSize = 10;
    char lineBuffer[2];
    lineBuffer[1] = '\0';
    std::string line;

    for (unsigned int i = 0; i < N; i++)
    {
        for (unsigned int alphabetSize = minAlphabetSize; alphabetSize <= maxAlphabetSize; alphabetSize++)
        {
            callSequitur(alphabetSize, inputSize);
        }
    }

    TS_TRACE("Finishing variableAlphabetAverageInput test");
}

void SequiturCxxUnitTest::test_broaderVariationAllParameters()
{
    TS_TRACE("Starting broaderVariationAllParameters test");

    unsigned int N = 100;
    unsigned int minInputSize = 10;
    unsigned int maxInputSize = 1000;
    unsigned int minAlphabetSize = 2;
    unsigned int maxAlphabetSize = 10;
    char lineBuffer[2];
    lineBuffer[1] = '\0';
    std::string line;

    for (unsigned int i = 0; i < N; i++)
    {
        for (unsigned int alphabetSize = minAlphabetSize; alphabetSize <= maxAlphabetSize; alphabetSize++)
        {
            for (unsigned int inputSize = minInputSize; inputSize <= maxInputSize; inputSize += 10)
            {
                //callSequitur(alphabetSize, inputSize);
            }
        }
    }

    TS_TRACE("Finishing broaderVariationAllParameters test");
}

void SequiturCxxUnitTest::test_smallAlphabetHugeInput()
{
    TS_TRACE("Starting smallAlphabetHugeInput test");

    unsigned int N = 1;
    unsigned int inputSize = 1000000;
    unsigned int minAlphabetSize = 5;
    unsigned int maxAlphabetSize = 5;
    char lineBuffer[2];
    lineBuffer[1] = '\0';
    std::string line;

    for (unsigned int i = 0; i < N; i++)
    {
        for (unsigned int alphabetSize = minAlphabetSize; alphabetSize <= maxAlphabetSize; alphabetSize++)
        {
            callSequitur(alphabetSize, inputSize);
        }
    }

    TS_TRACE("Finishing smallAlphabetHugeInput test");
}
