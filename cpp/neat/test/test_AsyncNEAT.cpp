#include "test_AsyncNEAT.h"

#include "neat/AsyncNEAT.h"
#include "network/cpu/cpunetwork.h"

const std::string test_name = "TestAsyncNeat";

TestAsyncNeat::TestAsyncNeat()
{

}

TestAsyncNeat::~TestAsyncNeat()
{

}

bool
TestAsyncNeat::test()
{
  if (!testXOR())
    return false;

  return true;
}

// 0 0 -> 0
// 0 1 -> 1
// 1 0 -> 1
// 1 1 -> 0
bool
TestAsyncNeat::testXOR()
{
  AsyncNeat::Init(test_name);
  AsyncNeat::SetSearchType(NEAT::GeneticSearchType::BLENDED);
  AsyncNeat::SetPopulationSize(10);
  AsyncNeat neat(2,
                 1,
                 1,
                 test_name);
  float success_margin_error = 0.0001;

  bool success = false;
  float min_error = std::numeric_limits<float>().max();
  std::vector<float> inputs0 = {0, 0, 1, 1};
  std::vector<float> inputs1 = {0, 1, 0, 1};
  std::vector<float> expectedOutputs = {0, 1, 1, 0};

  int gen;
  for (gen = 1; gen < MAX_EVALUATIONS; gen++) {
    std::shared_ptr<NeatEvaluation> eval = neat.getEvaluation();
    const NEAT::Organism *organism = eval->getOrganism();
    NEAT::CpuNetwork *net = reinterpret_cast< NEAT::CpuNetwork *> (organism->net
                                                                           .get());

    float error = 0;
    for (unsigned int test = 0; test < inputs0.size(); test++) {
      net->load_sensor(0,
                       inputs0[test]);
      net->load_sensor(1,
                       inputs1[test]);

      net->activate(1);
      NEAT::real_t *outputs = net->get_outputs();
      error += std::abs(outputs[0] - expectedOutputs[test]);
    }

    if (min_error > error) {
      min_error = error;
    }

    eval->finish(1 / error);

    if (min_error < success_margin_error) {
      std::cout << "\nAfter " << gen << " tries, a successful organism was found with an error of " << min_error
                << std::endl;
      std::cout << "The organism fitness is " << neat.getFittest()
                                                     ->getOrganism()
                                                     ->eval
                                                     .fitness << std::endl;
      success = true;
      break;
    }
  }

  neat.CleanUp();
  return success;
}


int main()
{
  TestAsyncNeat t;
  return t.test() ? 0 : 1;
}
