#include "AsyncNEAT.h"

#include "species/speciesorganism.h"
#include <limits>
#include <fstream>
#include <sstream>
#include <boost/filesystem.hpp>

#define DEFAULT_RNG_SEED 1

AsyncNeat::AsyncNeat(unsigned int n_inputs,
                     unsigned int n_outputs,
                     int rng_seed, const std::string &robot_name)
    : n_inputs(n_inputs),
      n_outputs(n_outputs),
      generation(1),
      best_fitness_counter(0),
      rng_seed(rng_seed),
      fittest(nullptr),
      fittest_fitness(std::numeric_limits<float>().min()),
      robot_name(robot_name)
{
  if (NEAT::env->genome_manager == nullptr) {
    throw std::invalid_argument("genome manager not initialized, "
                                    "please run AsyncNeat::Init() function to initialize the genome manager"
                                    " [remember also to call AsyncNeat::CleanUp() after finised using all"
                                    " AsyncNEAT objects]");
  }

  NEAT::rng_t rng{rng_seed};
  NEAT::rng_t rng_exp(rng.integer());
  std::vector<std::unique_ptr<NEAT::Genome>> genomes =
      NEAT::env->genome_manager->create_seed_generation(NEAT::env->pop_size,
                                                        rng_exp,
                                                        1,
                                                        n_inputs,
                                                        n_outputs,
                                                        n_inputs,
                                                        this->robot_name);
  //Spawn the Population
  population = NEAT::Population::create(rng_exp,
                                        genomes);
  refill_evaluation_queue();
}

AsyncNeat::~AsyncNeat()
{
  delete population;
}

std::shared_ptr<NeatEvaluation>
AsyncNeat::getEvaluation()
{
  if (this->evaluatingQueue
          .empty()) {
    std::cerr << "Neat::getEvaluation() evaluation queue is empty" << std::endl;
    return nullptr;
  }

  std::shared_ptr<NeatEvaluation> new_evaluation = this->evaluatingQueue.front();
  new_evaluation->add_finished_callback([this, new_evaluation](float fitness) {
      this->evaluatingList.remove(new_evaluation);
      this->singleEvalutionFinished(new_evaluation,
                                    fitness);
  });

  this->evaluatingQueue.pop_front();
  this->evaluatingList.push_back(new_evaluation);

  return new_evaluation;
}

void
AsyncNeat::next_generation()
{
  generation++;
  population->next_generation();
  refill_evaluation_queue();
}

void
AsyncNeat::refill_evaluation_queue()
{
  size_t n_organism = population->size();
  for (size_t i = 0; i < n_organism; i++) {
    evaluatingQueue.push_back(
        std::make_shared<NeatEvaluation>(
            population->get(i)
        )
    );
  }
}


void
AsyncNeat::singleEvalutionFinished(std::shared_ptr<NeatEvaluation> evaluation,
                                   float fitness)
{
  if (fitness > this->fittest_fitness) {
    this->setFittest(evaluation, fitness);
  }

  // need to wait for all generational evaluations to finish before creating a new generation
  // (because of spieces shared fitness)
  if (evaluatingQueue.empty() && evaluatingList.empty()) {
    next_generation();
  }

}

void
AsyncNeat::setFittest(std::shared_ptr<NeatEvaluation> new_fittest,
                      float new_fitness)
{
  this->fittest = new_fittest;
  this->fittest_fitness = new_fitness;
  this->best_fitness_counter++;

  size_t evaluation = ((generation - 1) * NEAT::env->pop_size) + fittest->getOrganism()->population_index + 1;
    
  size_t start = this->robot_name.rfind('/')+1;
  size_t end = this->robot_name.find('-');
  std::string test_n = this->robot_name.substr(end+1, robot_name.length() - end+1);
  std::string robot_name = this->robot_name.substr(start, end-start);
  
  std::ostringstream filename;
  
  filename << "./results/" << robot_name << '/' << test_n;
  boost::filesystem::create_directories(filename.str());
  filename << "/genome_n-" << this->best_fitness_counter << "_eval-" << evaluation << "_gen-" << generation << ".yaml";
  
  std::fstream genome_save;
  genome_save.open(filename.str(), std::ios::out);

  std::cout << "New best fitness! " << new_fitness << " saved at \"" << filename.str() << '\"' << std::endl;



  fittest->getOrganism()->genome->save(genome_save);
  genome_save
          << "\nfitness:"
          << "\n  value: " << new_fitness
          << "\n  evaluation: " << evaluation
          << "\n  generation: " << generation
          << std::endl;
  genome_save.close();
}
