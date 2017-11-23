#ifndef REVOLVE_NEAT_TEST_NEATLOADSAVE_H_
#define REVOLVE_NEAT_TEST_NEATLOADSAVE_H_

#include <boost/exception/detail/shared_ptr.hpp>
#include <neat/NEATEvaluation.h>

/**
 * Test class for NEAT Load and Save functionalities
 */
class TestNEATLoadSave
{
public:
    TestNEATLoadSave();

    ~TestNEATLoadSave();

    /**
     * Runs all tests. Returns false if one of the tests fails.
     */
    bool
    test();

private:
    /**
     * test if the algorithm is able to resolve the XOR problem
     */
    bool
    testXOR();
    /**
     * test if saving the organism works
     */
    std::stringstream
    testSave(const NEAT::Organism *organism);
    /**
     * test if loading the organism works
     */
    std::unique_ptr<NEAT::Organism>
    testLoad(std::stringstream& genome_save);

    const int MAX_EVALUATIONS = 9999;
};

#endif // REVOLVE_NEAT_TEST_NEATLOADSAVE_H_
