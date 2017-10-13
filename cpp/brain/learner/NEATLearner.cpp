/*
* Copyright (C) 2017 Vrije Universiteit Amsterdam
*
* Licensed under the Apache License, Version 2.0 (the "License");
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*
* Description: TODO: <Add brief description about file purpose>
* Author: Rafael Kiesel
*
*/

#include <algorithm>
#include <iostream>
#include <fstream>
#include <map>
#include <vector>

#include <yaml-cpp/yaml.h>

#include "brain/learner/cppneat/CPPNCrossover.h"

#include "NEATLearner.h"

namespace cppneat
{
  /////////////////////////////////////////////////
  NEATLearner::NEATLearner(
          MutatorPtr _mutator,
          const std::string &_mutatorPath,
          LearningConfiguration &_configuration
  )
          : activeBrain_(nullptr)
          , numGeneration(0)
          , numEvaluatedBrains(0)
          , mutator_(_mutator)
          , mutatorPath_(_mutatorPath)
          , isAsexual_(_configuration.asexual)
          , initStructuralMutations_(_configuration.initial_structural_mutations)
          , numChildren_(_configuration.num_children)
          , populationSize_(_configuration.pop_size)
          , tournamentSize_(_configuration.tournament_size)
          , weightMutationProbability_(_configuration.weight_mutation_probability)
          , weightMutationSigma_(_configuration.weight_mutation_sigma)
          , paramMutationProbability_(_configuration.param_mutation_probability)
          , paramMutationSigma_(_configuration.param_mutation_sigma)
          , augmentationProbability_(_configuration.structural_augmentation_probability)
          , removalProbability_(_configuration.structural_removal_probability)
          , maxGenerations_(_configuration.max_generations)
          , speciationThreshold_(_configuration.speciation_threshold)
          , repeatEvaluation_(_configuration.repeat_evaluations)
          , startFrom_(_configuration.start_from)
          , interspeciesMateProbability_(_configuration.interspecies_mate_probability)
  {
    std::random_device rd;
    generator.seed(rd());
    if (populationSize_ < 2)
    {
      populationSize_ = 2;
    }
    if (tournamentSize_ > populationSize_)
    {
      tournamentSize_ = populationSize_;
    }
    if (tournamentSize_ < 2)
    {
      tournamentSize_ = 2;
    }
//    if (mutator_path != "none")
//    {
//      mutator->load_known_innovations(mutator_path);
//    }
    if (startFrom_ != nullptr)
    {
      std::cout
              << "generating inital population from starting network"
              << std::endl;
      Initialise(GeneticEncodingPtrs());
    }
    else
    {
      std::cout
              << "no starting network given, initialise has to be called"
              << std::endl;
    }
    this->mutator_->RegisteStartingGenotype(startFrom_);
    std::cout
            << "\033[1;33m"
            << "-------------------------------------------------"
                    "----------------------------------------------------------"
            << "\033[0m"
            << std::endl;
    std::cout.width(35);
    std::cout
            << std::right
            << "\033[1;31m"
            << "Starting NEAT learner with the following parameters"
            << "\033[0m"
            << std::endl;
    std::cout.width(100);
    std::cout
            << std::right
            << "Asexual reproduction: "
            << "\033[1;36m"
            << isAsexual_
            << "\033[0m"
            << std::endl;
    std::cout.width(100);
    std::cout
            << std::right
            << "Population size: "
            << "\033[1;36m"
            << populationSize_
            << "\033[0m"
            << std::endl;
    std::cout.width(100);
    std::cout
            << std::right
            << "Tournament size: (currently not applicable, always 2) "
            << "\033[1;36m"
            << tournamentSize_
            << "\033[0m"
            << std::endl;
    std::cout.width(100);
    std::cout
            << std::right
            << "Number of children: (rest of new individuals are elite of "
                    "previous generation) "
            << "\033[1;36m"
            << numChildren_
            << "\033[0m"
            << std::endl;
    std::cout.width(100);
    std::cout
            << std::right
            << "Weight mutation probability: "
            << "\033[1;36m"
            << weightMutationProbability_
            << "\033[0m"
            << std::endl;
    std::cout.width(100);
    std::cout
            << std::right
            << "Weight mutation sigma: "
            << "\033[1;36m"
            << weightMutationSigma_
            << "\033[0m"
            << std::endl;
    std::cout.width(100);
    std::cout
            << std::right
            << "Param mutation probability: "
            << "\033[1;36m"
            << paramMutationProbability_
            << "\033[0m"
            << std::endl;
    std::cout.width(100);
    std::cout << std::right << "Param mutation sigma: "
              << "\033[1;36m" << paramMutationSigma_ << "\033[0m" << std::endl;
    std::cout.width(100);
    std::cout
            << std::right
            << "Probability of structural additions: "
            << "\033[1;36m"
            << augmentationProbability_
            << "\033[0m"
            << std::endl;
    std::cout.width(100);
    std::cout
            << std::right
            << "Probability of structural removal: "
            << "\033[1;36m"
            << removalProbability_
            << "\033[0m"
            << std::endl;
    std::cout.width(100);
    std::cout << std::right << "Number of generations: "
              << "\033[1;36m" << maxGenerations_ << "\033[0m" << std::endl;
    std::cout.width(100);
    std::cout
            << std::right
            << "Speciation threshold: "
                    "(maximum dissimilarity to be in same species) "
            << "\033[1;36m"
            << speciationThreshold_
            << "\033[0m"
            << std::endl;
    std::cout.width(100);
    std::cout
            << std::right
            << "How often do we evaluate before we take the average fitness: "
            << "\033[1;36m"
            << repeatEvaluation_
            << "\033[0m"
            << std::endl;
    std::cout.width(100);
    std::cout
            << std::right
            << "How many initial structural mutations do we apply in case a "
                    "starting genome was given: "
            << "\033[1;36m"
            << initStructuralMutations_
            << "\033[0m"
            << std::endl;
    std::cout.width(100);
    std::cout
            << std::right
            << "Probability to mate outside of species while there is more than"
                    " one member inside the species: "
            << "\033[1;36m"
            << interspeciesMateProbability_
            << "\033[0m"
            << std::endl;
    std::cout
            << "\033[1;33m"
            << "-------------------------------------------------"
                    "----------------------------------------------------------"
            << "\033[0m"
            << std::endl;
  }

  /////////////////////////////////////////////////
  void NEATLearner::Initialise(GeneticEncodingPtrs _genotypes)
  {
    if (_genotypes.empty())
    {
      this->brainPpopulation_ = this->InitBrains();
    }
    else
    {
      std::cout << "initialised with starting population" << std::endl;
      std::cout << "overwriting current population if present" << std::endl;
      this->brainPpopulation_ = _genotypes;
    }
    this->evaluationQueue_.clear();
    for (GeneticEncodingPtr brain : this->brainPpopulation_)
    {
      this->evaluationQueue_.push_back(brain);
    }
    this->activeBrain_ = this->evaluationQueue_.back();
    this->evaluationQueue_.pop_back();
  }

  /////////////////////////////////////////////////
  GeneticEncodingPtrs NEATLearner::YamlBrains(
          const std::string &_yamlPath,
          const int _offset)
  {
    if (_offset != -1)
    {
      int innovation_counter = _offset;
      YAML::Node yaml_file = YAML::LoadFile(_yamlPath);
      if (yaml_file.IsNull())
      {
        std::cout << "Failed to load the yaml file." << std::endl;
        return GeneticEncodingPtrs();
      }
      GeneticEncodingPtrs genotypes;
      for (unsigned int first = 0; first < yaml_file.size(); first++)
      {
        std::map< int, int > old_to_new;
        GeneticEncodingPtr newGenome(new GeneticEncoding(true));
        for (unsigned int counter = 0;
             counter < yaml_file[first]["brain"]["layers"].size(); counter++)
        {
          YAML::Node layer = yaml_file[first]["brain"]["layers"][counter];
          layer = layer["layer_" + std::to_string(counter + 1)];
          bool is_new_layer = true;
          for (unsigned int i = 0; i < layer.size(); i++)
          {
            YAML::Node neuron_node = layer[i];
            std::string neuron_id = neuron_node["nid"].as< std::string >();
            Neuron::Ntype neuron_type =
                    static_cast<Neuron::Ntype>(neuron_node["ntype"].as< int >());
            Neuron::Layer neuron_layer =
                    static_cast<Neuron::Layer>(neuron_node["nlayer"].as< int >());
            int innov_numb;
            if (old_to_new.find(neuron_node["in_no"].as< int >())
                == old_to_new.end())
            {
              innov_numb = innovation_counter++;
              old_to_new[neuron_node["in_no"].as< int >()] = innov_numb;
            }
            else
            {
              innov_numb = old_to_new[neuron_node["in_no"].as< int >()];
            }
            std::map< std::string, double > neuron_params;
            std::vector< std::string > params;
            for (std::pair< Neuron::Ntype, Neuron::NeuronTypeSpec >
                      spec_pair : mutator_->Specification())
            {
              for (Neuron::ParamSpec param_spec : spec_pair.second.parameters)
              {
                if (std::find(params.begin(),
                              params.end(),
                              param_spec.name) == params.end())
                {
                  params.push_back(param_spec.name);
                }
              }
            }
            YAML::Node params_node = neuron_node["params"];
            for (std::string param_name : params)
            {
              YAML::Node param_node = params_node[param_name];
              if (param_node.IsDefined())
              {
                neuron_params[param_name] = param_node.as< double >();
              }
            }
            NeuronPtr new_neuron(new Neuron(neuron_id,
                                            neuron_layer,
                                            neuron_type,
                                            neuron_params));
            NeuronGenePtr new_neuron_gene(new NeuronGene(new_neuron,
                                                         innov_numb,
                                                         true));
            newGenome->AddNeuron(new_neuron_gene,
                                 counter,
                                 is_new_layer);
            is_new_layer = false;
          }
        }
        for (unsigned int i = 0;
             yaml_file[first]["brain"]["connection_genes"].size(); i++)
        {
          YAML::Node connection =
                  yaml_file[first]["brain"]["connection_genes"][i]["con_1"];
          int mark_to = old_to_new[connection["to"].as< int >()];
          int mark_from = old_to_new[connection["from"].as< int >()];
          double weight = connection["weight"].as< double >();
          int innov_numb;
          if (old_to_new.find(connection["in_no"].as< int >())
              == old_to_new.end())
          {
            innov_numb = innovation_counter++;
            old_to_new[connection["in_no"].as< int >()] = innov_numb;
          }
          else
          {
            innov_numb = old_to_new[connection["in_no"].as< int >()];
          }
          ConnectionGenePtr newConnection(
                  new ConnectionGene(mark_to,
                                     mark_from,
                                     weight,
                                     innov_numb,
                                     true,
                                     ""));
          newGenome->AddConnection(newConnection);
        }
        genotypes.push_back(newGenome);
      }
      return genotypes;
    }
    else
    {
      YAML::Node yaml_file = YAML::LoadFile(_yamlPath);
      if (yaml_file.IsNull())
      {
        std::cout << "Failed to load the yaml file." << std::endl;
        return GeneticEncodingPtrs();
      }
      GeneticEncodingPtrs genotypes;
      for (unsigned int first = 0; first < yaml_file.size(); first++)
      {
        GeneticEncodingPtr newGenome(new GeneticEncoding(true));
        for (unsigned int counter = 0;
             counter < yaml_file[first]["brain"]["layers"].size(); counter++)
        {
          YAML::Node layer = yaml_file[first]["brain"]["layers"][counter];
          layer = layer["layer_" + std::to_string(counter + 1)];
          bool is_new_layer = true;
          for (unsigned int i = 0; i < layer.size(); i++)
          {
            // Load connections
            for (size_t i = 0;
                 i < yaml_file[first]["brain"]["connection_genes"].size(); i++)
            {
              YAML::Node connection =
                      yaml_file[first]["brain"]["connection_genes"][i]["con_1"];
              int mark_to = connection["to"].as< int >();
              int mark_from = connection["from"].as< int >();
              double weight = connection["weight"].as< double >();
              int innov_numb = connection["in_no"].as< int >();

              mutator_->InsertConnectionInnovation(mark_from,
                                                   mark_to,
                                                   innov_numb);

              ConnectionGenePtr newConnection(
                      new ConnectionGene(mark_to,
                                         mark_from,
                                         weight,
                                         innov_numb,
                                         true,
                                         _yamlPath,
                                         first,
                                         ""));
              newGenome->AddConnection(newConnection);
            }

            // Load neurons
            YAML::Node neuron_node = layer[i];
            std::string neuron_id = neuron_node["nid"].as< std::string >();
            Neuron::Ntype neuron_type =
                    static_cast<Neuron::Ntype>(neuron_node["ntype"].as< int >());
            Neuron::Layer neuron_layer =
                    static_cast<Neuron::Layer>(neuron_node["nlayer"].as< int >());
            int innov_numb = neuron_node["in_no"].as< int >();
            std::map< std::string, double > neuron_params;
            std::vector< std::string > params;
            for (std::pair< Neuron::Ntype, Neuron::NeuronTypeSpec >
                      spec_pair : mutator_->Specification())
            {
              for (Neuron::ParamSpec param_spec : spec_pair.second.parameters)
              {
                if (std::find(params.begin(),
                              params.end(),
                              param_spec.name) == params.end())
                {
                  params.push_back(param_spec.name);
                }
              }
            }
            YAML::Node params_node = neuron_node["params"];
            for (std::string param_name : params)
            {
              YAML::Node param_node = params_node[param_name];
              if (param_node.IsDefined())
              {
                neuron_params[param_name] = param_node.as< double >();
              }
            }
            NeuronPtr new_neuron(
                    new Neuron(neuron_id,
                               neuron_layer,
                               neuron_type,
                               neuron_params));
            NeuronGenePtr new_neuron_gene(
                    new NeuronGene(new_neuron,
                                   innov_numb,
                                   true,
                                   _yamlPath,
                                   first));

            mutator_->InsertNeuronInnovation(neuron_type, innov_numb);

            newGenome->AddNeuron(new_neuron_gene,
                                 counter,
                                 is_new_layer);
            is_new_layer = false;
          }
        }

        genotypes.push_back(newGenome);
      }
      return genotypes;
    }
  }

  /////////////////////////////////////////////////
  GeneticEncodingPtrs NEATLearner::InitBrains()
  {
    GeneticEncodingPtrs initialPopulation;
    for (size_t i = 0; i < populationSize_; ++i)
    {
      GeneticEncodingPtr genome = this->startFrom_->Copy();
#ifdef CPPNEAT_DEBUG
      if (not genome->is_valid())
      {
          std::cerr << "copying caused invalid genotype" << std::endl;
      }
#endif
      for (size_t j = 0; j < initStructuralMutations_
                         and initStructuralMutations_ > 0; j++)
      {
        this->ApplyStructuralMutation(genome);
      }

      mutator_->MutateWeights(genome, 1, weightMutationSigma_);
      mutator_->MutateNeuronParams(genome, 1, paramMutationSigma_);

      initialPopulation.push_back(genome);
    }
    return initialPopulation;
  }

  /////////////////////////////////////////////////
  void NEATLearner::reportFitness(
          const std::string &_id,
          GeneticEncodingPtr _genotype,
          const double _fitness)
  {
    std::cout << "Evalutation over\n"
              << "Evaluated " << ++numEvaluatedBrains << " brains \n"
              << "Last fitness: " << _fitness << std::endl;
    this->RecordGenome(_id, _genotype);

    fitnessBuffer_.push_back(_fitness);
    if (fitnessBuffer_.size() == this->repeatEvaluation_)
    {
      double sum = 0;
      for (double fitness : fitnessBuffer_)
      {
        sum += fitness;
      }
      double avgFitness = sum / this->repeatEvaluation_;
      this->brainFitness_[this->activeBrain_] = avgFitness;
      this->brainVelocity_[this->activeBrain_] = avgFitness;

      if (this->evaluationQueue_.size() == 0)
      {
        this->ShareFitness();
        this->generatePopulation();
        std::reverse(evaluationQueue_.begin(), evaluationQueue_.end());
        this->numGeneration++;
      }
      this->activeBrain_ = this->evaluationQueue_.back();
      this->evaluationQueue_.pop_back();
      this->fitnessBuffer_.clear();
      if (this->numGeneration >= this->maxGenerations_)
      {
        std::cout << "Maximum number of generations reached" << std::endl;
        std::exit(0);
      }
    }
  }

  /////////////////////////////////////////////////
  GeneticEncodingPtr NEATLearner::currentGenotype()
  {
    return this->activeBrain_;
  }

  /////////////////////////////////////////////////
  void NEATLearner::RecordGenome(
          const std::string &_robotName,
          GeneticEncodingPtr _genome)
  {
    std::ofstream outputFile;
    outputFile.open(_robotName + ".policy",
                    std::ios::app | std::ios::out | std::ios::ate);
    outputFile << "- evaluation: " << numEvaluatedBrains << std::endl;
    outputFile << "  brain:" << std::endl;
    outputFile << "    connection_genes:" << std::endl;
    auto connection_genes = _genome->connectionGenes_;
    int n_cons = 1;
    for (auto conGene : connection_genes)
    {
      auto connection = conGene.get();
      outputFile << "      - con_" << n_cons << ":" << std::endl;
      outputFile << "            in_no: "
                 << connection->InnovationNumber() << std::endl;
      outputFile << "            from: " << connection->from_ << std::endl;
      outputFile << "            to: " << connection->to_ << std::endl;
      outputFile << "            weight: " << connection->weight_ << std::endl;
      outputFile << "            parent_name: "
                 << connection->ParentsName() << std::endl;
      outputFile << "            parent_index: "
                 << connection->ParentsIndex() << std::endl;
    }
    outputFile << "    layers:" << std::endl;
    auto layers = _genome->layers_;
    int n_layer = 1;
    for (auto it = layers.begin(); it != layers.end(); it++)
    {
      outputFile << "      - layer_" << n_layer << ":" << std::endl;
      for (auto it2 = it->begin(); it2 != it->end(); it2++)
      {
        auto neuron = it2->get()->neuron_;
        auto neuron_params = neuron->parameters_;
        outputFile << "          - nid: " << neuron->neuronId_ << std::endl;
        outputFile << "            ntype: " << neuron->neuronType_ << std::endl;
        outputFile << "            nlayer: " << neuron->layer_ << std::endl;
        outputFile << "            in_no: "
                   << it2->get()->InnovationNumber() << std::endl;
        outputFile << "            parent_name: "
                   << it2->get()->ParentsName() << std::endl;
        outputFile << "            parent_index: "
                   << it2->get()->ParentsIndex() << std::endl;
        outputFile << "            params:" << std::endl;
        for (auto np = neuron_params.begin(); np != neuron_params.end(); np++)
        {
          outputFile << "              " << np->first
                     << ": " << np->second << std::endl;
        }
      }
      n_layer++;
    }
    outputFile.close();
  }

  /////////////////////////////////////////////////
  void NEATLearner::ShareFitness()
  {
    // speciate
    std::map< GeneticEncodingPtr, GeneticEncodingPtrs > oldSpecies = species_;
    species_.clear();
    // choose representative from previous generation
    // (or do nothing for first run)
    for (auto spPair : oldSpecies)
    {
      std::uniform_int_distribution< int > choose(0, spPair.second.size() - 1);
      GeneticEncodingPtr representative = spPair.second[choose(generator)];
      species_.insert({representative, GeneticEncodingPtrs()});
    }

    for (auto currentBrain : brainVelocity_)
    {
      bool added = false;
      // search for matching species
      for (std::pair< GeneticEncodingPtr, std::vector< GeneticEncodingPtr>>
                spPair
              : species_)
      {
        // TODO: coefficients
//        std::cout << "<<<<<<<<<< NEATLearner::shareFitness:632 " << std::endl;
        double dissimilarity = GeneticEncoding::Dissimilarity(
                spPair.first,
                currentBrain.first,
                1,
                1,
                0.4);
//        std::cout << "<<<<<<<<<< NEATLearner::shareFitness:639 " << std::endl;

        if (dissimilarity < speciationThreshold_)
        {
          added = true;
          species_[spPair.first].push_back(currentBrain.first);
          break;
        }
      }
      // add new species in case of no matches
      if (not added)
      {
        species_.insert(std::make_pair(
                currentBrain.first,
                GeneticEncodingPtrs(1, currentBrain.first)));
      }
    }
    oldSpecies = species_;
    species_.clear();
    // only keep species which are not empty
    for (auto sppair : oldSpecies)
    {
      if (not sppair.second.empty())
      {
        species_.insert(sppair);
      }
    }
    // actual sharing
    std::map< GeneticEncodingPtr, double > new_fitness;
    for (auto sppair : species_)
    {
      for (GeneticEncodingPtr brain : sppair.second)
      {
        new_fitness[brain] = brainVelocity_[brain] / sppair.second.size();
      }
    }
    brainFitness_ = new_fitness;
  }

  /////////////////////////////////////////////////
  void NEATLearner::generatePopulation()
  {
    // calculate number of children for each species
    std::map< GeneticEncodingPtr, size_t > offsprings =
            this->NumChildrenPerSpecie();

    // reproduce
    ///////////////////////////////////////////////
    this->Reproduce(offsprings);

    // elitism
    FitnessPairs velocityPairs;
    for (auto brain : this->brainVelocity_)
    {
      velocityPairs.push_back({brain.first, brain.second});
    }
    std::sort(velocityPairs.begin(),
              velocityPairs.end(),
              [](FitnessPair a, FitnessPair b) {
                return a.second > b.second;
              });

    for (size_t i = 0; i < (populationSize_ - numChildren_); ++i)
    {
      this->evaluationQueue_.push_back(velocityPairs[i].first);
    }

    // remove old fitness
    this->brainFitness_.clear();
    this->brainVelocity_.clear();
    // debug
    std::cout << "Produced new generation with: " << std::endl;
    std::cout << "* " << this->species_.size()
              << " species with sizes: " << std::endl;
    for (auto specie : this->species_)
    {
      std::cout << "** " << specie.second.size() << std::endl;
    }
    std::cout << "* overall number of individuals in queue: "
              << evaluationQueue_.size()
              << std::endl;
  }

  /////////////////////////////////////////////////
  void
  NEATLearner::Reproduce(std::map< GeneticEncodingPtr, size_t > _offsprings)
  {
    std::uniform_real_distribution< double > uniform(0, 1);
    for (auto spPair : this->species_)
    {
      FitnessPairs fitnessPairs;
      for (auto brain : spPair.second)
      {
        fitnessPairs.push_back(FitnessPair(
                brain,
                this->brainFitness_[brain]));
      }
      std::sort(fitnessPairs.begin(),
                fitnessPairs.end(),
                [](FitnessPair a, FitnessPair b) {
                  return a.second > b.second;
                });

      ParentPairs parentPairs;
      for (size_t top = 0; top < _offsprings[spPair.first]; ++top)
      {
        if (spPair.second.size() == 1
            or (uniform(generator) < this->interspeciesMateProbability_))
        {
          // if there is only one individual in species or we are below
          // threshold probability we want inter-species mating
          std::uniform_int_distribution< size_t > gen(
                  0,
                  spPair.second.size() - 1);
          GeneticEncodingPtr mom = spPair.second[gen(generator)];

          std::uniform_int_distribution< size_t > gen1(
                  0,
                  this->species_.size() - 1);
          size_t until = gen1(generator);
          auto species_iterator = this->species_.begin();
          for (size_t k = 0; k < until; k++)
          {
            ++species_iterator;
          }

          FitnessPairs toSort;
          GeneticEncodingPtrs bachelors = (species_iterator)->second;
          for (auto brain : bachelors)
          {
            toSort.push_back(FitnessPair(
                    brain,
                    this->brainFitness_[brain]));
          }
          std::sort(
                  toSort.begin(),
                  toSort.end(),
                  [](FitnessPair a, FitnessPair b) {
                    return a.second > b.second;
                  });

          GeneticEncodingPtr dad = toSort.front().first;
          parentPairs.push_back(
                  brainFitness_[mom] > brainFitness_[dad] ?
                  std::make_pair(mom, dad) :
                  std::make_pair(dad, mom));
//          parentPairs.push_back({mom, mom});
        }
        else
        {
          auto selected = TournamentSelection(fitnessPairs, 2);
          parentPairs.push_back(selected);
        }
      }

      for (auto parents : parentPairs)
      {
        GeneticEncodingPtr offspring = ProduceChild(
                parents.first,
                parents.second);
        this->evaluationQueue_.push_back(offspring);
      }
    }
  }

  /////////////////////////////////////////////////
  std::map< GeneticEncodingPtr, size_t > NEATLearner::NumChildrenPerSpecie()
  {
    double totalFitness = 0;
    std::map< GeneticEncodingPtr, double > fitnesses;
    for (auto spPair : this->species_)
    {
      double currentSum = 0;
      for (auto brain : spPair.second)
      {
        currentSum += this->brainFitness_[brain];
      }
      fitnesses[spPair.first] = currentSum;
      totalFitness += currentSum;
    }

    std::map< GeneticEncodingPtr, size_t > offsprings;
    size_t numOffsprings = 0;
    double threshold = 0;
    double avgFitness = totalFitness / this->numChildren_;
    GeneticEncodingPtr bestBrain = fitnesses.begin()->first;
    for (auto fitness : fitnesses)
    {
      while (threshold + fitness.second >= (numOffsprings + 1) * avgFitness)
      {
        offsprings[fitness.first]++;
        numOffsprings++;
      }
      threshold += fitness.second;
      if (fitnesses[bestBrain] < fitness.second)
      {
        bestBrain = fitness.first;
      }
    }

    // should not happen, but might (>= for double flawed)
    if (numOffsprings < this->numChildren_)
    {
      assert(numOffsprings == this->numChildren_ - 1);
      offsprings[bestBrain]++;
    }

    return offsprings;
  }

  /////////////////////////////////////////////////
  GeneticEncodingPtr NEATLearner::ProduceChild(
          GeneticEncodingPtr _parent1,
          GeneticEncodingPtr _parent2)
  {
    GeneticEncodingPtr offspring;
    if (this->isAsexual_)
    {
      offspring = _parent1->Copy();
    }
    else
    {
      offspring = Crossover::crossover(_parent1, _parent2);
    }

    mutator_->MutateWeights(
            offspring,
            weightMutationProbability_,
            weightMutationSigma_);

    mutator_->MutateNeuronParams(
            offspring,
            paramMutationProbability_,
            paramMutationSigma_);

    this->ApplyStructuralMutation(offspring);

    return offspring;
  }

  /////////////////////////////////////////////////
  void NEATLearner::ApplyStructuralMutation(GeneticEncodingPtr _genotype)
  {
    std::uniform_real_distribution< double > uniform(0, 1);
    mutator_->MutateStructure(_genotype, augmentationProbability_);

    if (uniform(generator) < this->removalProbability_)
    {
      if (uniform(generator) < 0.5)
      {
        mutator_->RemoveConnectionMutation(_genotype);
      }
      else
      {
        mutator_->RemoveNeuronMutation(_genotype);
      }
    }
  }

  /////////////////////////////////////////////////
  ParentPair NEATLearner::TournamentSelection(
          FitnessPairs _candidates,
          size_t _tournamentSize)
  {
    std::shuffle(_candidates.begin(), _candidates.end(), generator);
    _candidates = FitnessPairs(_candidates.begin(),
                               _candidates.begin() + _tournamentSize);
    std::sort(
            _candidates.begin(),
            _candidates.end(),
            [](FitnessPair a, FitnessPair b) {
              return a.second > b.second;
            });

    return {_candidates.at(0).first, _candidates.at(1).first};
  }

  /////////////////////////////////////////////////
  const bool NEATLearner::ASEXUAL = false;

  /////////////////////////////////////////////////
  const int NEATLearner::POP_SIZE = 50;

  /////////////////////////////////////////////////
  const int NEATLearner::TOURNAMENT_SIZE = 40;

  /////////////////////////////////////////////////
  const int NEATLearner::NUM_CHILDREN = 45;

  /////////////////////////////////////////////////
  const double NEATLearner::WEIGHT_MUTATION_PROBABILITY = 0.8;

  /////////////////////////////////////////////////
  const double NEATLearner::WEIGHT_MUTATION_SIGMA = 5.0;

  /////////////////////////////////////////////////
  const double NEATLearner::PARAM_MUTATION_PROBABILITY = 0.8;

  /////////////////////////////////////////////////
  const double NEATLearner::PARAM_MUTATION_SIGMA = 0.25;

  /////////////////////////////////////////////////
  const double NEATLearner::STRUCTURAL_AUGMENTATION_PROBABILITY = 0.8;

  /////////////////////////////////////////////////
  const double NEATLearner::STRUCTURAL_REMOVAL_PROBABILITY = 0;

  /////////////////////////////////////////////////
  const int NEATLearner::MAX_GENERATIONS = 20;

  /////////////////////////////////////////////////
  const double NEATLearner::SPECIATION_TRESHOLD = 0.03;

  /////////////////////////////////////////////////
  const int NEATLearner::REPEAT_EVALUATIONS = 1;

  /////////////////////////////////////////////////
  const int NEATLearner::INITIAL_STRUCTURAL_MUTATIONS = 1;

  /////////////////////////////////////////////////
  const double NEATLearner::INTERSPECIES_MATE_PROBABILITY = 0.001;
}
