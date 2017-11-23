#include "test_NEATLoadSave.h"

#include "neat/AsyncNEAT.h"
#include "network/cpu/cpunetwork.h"

#include <fstream>
#include <innovgenome/innovgenome.h>
#include <network/network.h>

const std::string test_name = "TestNEATLoadSave";

TestNEATLoadSave::TestNEATLoadSave()
{

}

TestNEATLoadSave::~TestNEATLoadSave()
{

}

#define ASSERT(field1, field2) {if (field1 != field2) {std::clog << "Assertion "#field1" == "#field2 << std::endl << field1 << " != " << field2 << std::endl; assert(false);}}

bool
TestNEATLoadSave::test()
{
  if (not testXOR())
    return false;

  return true;
}

// 0 0 -> 0
// 0 1 -> 1
// 1 0 -> 1
// 1 1 -> 0
bool
TestNEATLoadSave::testXOR()
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
  std::vector<NEAT::real_t> inputs0 = {0, 0, 1, 1};
  std::vector<NEAT::real_t> inputs1 = {0, 1, 0, 1};
  std::vector<NEAT::real_t> expectedOutputs = {0, 1, 1, 0};

  int gen;
  for (gen = 1; gen < MAX_EVALUATIONS; gen++) {
    std::shared_ptr<NeatEvaluation> eval = neat.getEvaluation();
    const NEAT::Organism *organism = eval->getOrganism();
    auto *net = reinterpret_cast< NEAT::CpuNetwork *> (organism->net
                                                                           .get());

    NEAT::real_t error = 0;
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

  if (success)
  {
    const NEAT::Organism *organism = neat.getFittest()->getOrganism();

    // get original network
    auto *net = reinterpret_cast< NEAT::CpuNetwork *> ((*organism).net.get());

    // save and load
    std::stringstream genome_save = testSave(organism);
    std::unique_ptr<NEAT::Organism> loaded_organism = testLoad(genome_save);

#define TEST_EQUAL(field) ASSERT(((*(*organism).net).get_dims().field),((*(*loaded_organism).net).get_dims().field))
      TEST_EQUAL(nnodes.all);
      TEST_EQUAL(nnodes.bias);
      TEST_EQUAL(nnodes.sensor);
      TEST_EQUAL(nnodes.input);
      TEST_EQUAL(nnodes.noninput);
      TEST_EQUAL(nnodes.output);
      TEST_EQUAL(nlinks);
#undef test_equal

    // get loaded network
    auto *loaded_net = reinterpret_cast< NEAT::CpuNetwork *> ((*loaded_organism).net.get());

    // compare two outputs
    NEAT::real_t error = 0;
    NEAT::real_t error_target = 0;
    for (unsigned int test = 0; test < inputs0.size(); test++) {
      net->load_sensor(0, inputs0[test]);
      net->load_sensor(1, inputs1[test]);
      loaded_net->load_sensor(0, inputs0[test]);
      loaded_net->load_sensor(1, inputs1[test]);

      net->activate(1);
      loaded_net->activate(1);

      NEAT::real_t *outputs = net->get_outputs();
      NEAT::real_t *outputs_loaded = loaded_net->get_outputs();

      std::cout << "1 input {" << inputs0[test] << ',' << inputs1[test] << "} output: " << outputs[0] << " \toutput_loaded: " << outputs_loaded[0] << std::endl;
      error += std::abs(outputs[0] - outputs_loaded[0]);
      error_target += std::abs(outputs_loaded[0] - expectedOutputs[test]);
      //error += std::abs(outputs[0] - expectedOutputs[test]);
    }

    std::cout << "Error in network load: " << error << std::endl;
    if (error > success_margin_error)
          success = false;
    std::cout << "Error in network load (distance from original target): " << error << std::endl;
    if (error_target > success_margin_error)
          success = false;
  }

  AsyncNeat::CleanUp();
  return success;
}

std::stringstream TestNEATLoadSave::testSave(const NEAT::Organism *organism) {

  std::stringstream genome_save;

  (*organism->genome).save(genome_save);
  genome_save.flush();
  genome_save.seekg(0);

  return genome_save;
}

std::unique_ptr<NEAT::Organism> TestNEATLoadSave::testLoad(std::stringstream &genome_save) {
  const std::string robot_name = "loaded_controller";

  std::unique_ptr<NEAT::Genome> genome = NEAT::env->genome_manager->make_default();
  (*genome).load(genome_save);

  std::unique_ptr<NEAT::Organism> organism(new NEAT::Organism(*genome));

  (*genome).init_phenotype(*(*organism).net);
  return organism;
}


int main()
{
  TestNEATLoadSave t;
  return t.test() ? 0 : 1;
}
