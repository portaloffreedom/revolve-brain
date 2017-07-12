#include "Conversion.h"

#include <fstream>

//dbg_plot

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graphviz.hpp>

namespace revolve {
namespace brain {

std::map<boost::shared_ptr<CPPNConfig>, CPPNEAT::GeneticEncodingPtr> known_genotypes;

std::map<CPPNEAT::Neuron::Ntype, CPPNEAT::Neuron::NeuronTypeSpec> brain_spec;

std::map<int, size_t > input_map;

std::map<int, size_t > output_map;

void set_param_spec(CPPNEAT::Neuron::ParamSpec &spec,
                    std::string name,
                    double eps,
                    double min_value,
                    double max_value,
                    bool min_inclusive,
                    bool max_inclusive)
{
  spec.name = name;
  spec.epsilon = eps;
  spec.max_inclusive = max_inclusive;
  spec.min_inclusive = min_inclusive;
  spec.min_value = min_value;
  spec.max_value = max_value;
}

void
set_brain_spec(bool hyperneat)
{
  double eps = 1e-9;
  bool max_inclusive = false;
  bool min_inclusive = false;
  CPPNEAT::Neuron::NeuronTypeSpec input;
  input.possible_layers
       .push_back(CPPNEAT::Neuron::INPUT_LAYER);

  brain_spec[CPPNEAT::Neuron::INPUT] = input;


  CPPNEAT::Neuron::NeuronTypeSpec sigmoid;
  CPPNEAT::Neuron::ParamSpec bias_spec;
  set_param_spec(bias_spec,
                 "rv:bias",
                 eps,
                 -1,
                 1,
                 max_inclusive,
                 min_inclusive);
  CPPNEAT::Neuron::ParamSpec gain_spec;
  set_param_spec(gain_spec,
                 "rv:gain",
                 eps,
                 0,
                 1,
                 max_inclusive,
                 min_inclusive);
  sigmoid.param_specs
         .push_back(bias_spec);
  sigmoid.param_specs
         .push_back(gain_spec);
  sigmoid.possible_layers
         .push_back(CPPNEAT::Neuron::HIDDEN_LAYER);
  sigmoid.possible_layers
         .push_back(CPPNEAT::Neuron::OUTPUT_LAYER);

  brain_spec[CPPNEAT::Neuron::SIGMOID] = sigmoid;


  CPPNEAT::Neuron::NeuronTypeSpec simple;
  simple.param_specs
        .push_back(bias_spec);
  simple.param_specs
        .push_back(gain_spec);
  simple.possible_layers
        .push_back(CPPNEAT::Neuron::HIDDEN_LAYER);
  simple.possible_layers
        .push_back(CPPNEAT::Neuron::OUTPUT_LAYER);

  brain_spec[CPPNEAT::Neuron::SIMPLE] = simple;


  CPPNEAT::Neuron::NeuronTypeSpec bias;
  bias.param_specs
      .push_back(bias_spec);
  bias.possible_layers
      .push_back(CPPNEAT::Neuron::INPUT_LAYER);


  brain_spec[CPPNEAT::Neuron::BIAS] = bias;

  if (!hyperneat) {
    CPPNEAT::Neuron::NeuronTypeSpec oscillator;
    CPPNEAT::Neuron::ParamSpec period_spec;
    set_param_spec(period_spec,
                   "rv:period",
                   eps,
                   0,
                   10,
                   max_inclusive,
                   min_inclusive);
    CPPNEAT::Neuron::ParamSpec phase_offset_spec;
    set_param_spec(phase_offset_spec,
                   "rv:phase_offset",
                   eps,
                   0,
                   3.14,
                   max_inclusive,
                   min_inclusive);
    CPPNEAT::Neuron::ParamSpec amplitude_spec;
    set_param_spec(amplitude_spec,
                   "rv:amplitude",
                   eps,
                   0,
                   10000,
                   max_inclusive,
                   min_inclusive);
    oscillator.param_specs
              .push_back(period_spec);
    oscillator.param_specs
              .push_back(phase_offset_spec);
    oscillator.param_specs
              .push_back(amplitude_spec);
    oscillator.possible_layers
              .push_back(CPPNEAT::Neuron::INPUT_LAYER);

    brain_spec[CPPNEAT::Neuron::OSCILLATOR] = oscillator;
  } else {
    CPPNEAT::Neuron::NeuronTypeSpec input_oscillator;
    CPPNEAT::Neuron::ParamSpec period_spec;
    set_param_spec(period_spec,
                   "rv:period",
                   eps,
                   0,
                   10,
                   max_inclusive,
                   min_inclusive);
    CPPNEAT::Neuron::ParamSpec phase_offset_spec;
    set_param_spec(phase_offset_spec,
                   "rv:phase_offset",
                   eps,
                   0,
                   3.14,
                   max_inclusive,
                   min_inclusive);
    CPPNEAT::Neuron::ParamSpec amplitude_spec;
    set_param_spec(amplitude_spec,
                   "rv:amplitude",
                   eps,
                   0,
                   10000,
                   max_inclusive,
                   min_inclusive);
    input_oscillator.param_specs
                    .push_back(period_spec);
    input_oscillator.param_specs
                    .push_back(phase_offset_spec);
    input_oscillator.param_specs
                    .push_back(amplitude_spec);
    input_oscillator.possible_layers
                    .push_back(CPPNEAT::Neuron::HIDDEN_LAYER);
    input_oscillator.possible_layers
                    .push_back(CPPNEAT::Neuron::OUTPUT_LAYER);


    brain_spec[CPPNEAT::Neuron::INPUT_OSCILLATOR] = input_oscillator;
  }

  if (!hyperneat) {
    CPPNEAT::Neuron::NeuronTypeSpec differential_neuron;
    differential_neuron.param_specs
        .push_back(bias_spec);
    differential_neuron.possible_layers
        .push_back(CPPNEAT::Neuron::HIDDEN_LAYER);
    differential_neuron.possible_layers
        .push_back(CPPNEAT::Neuron::OUTPUT_LAYER);


    brain_spec[CPPNEAT::Neuron::DIFFERENTIAL_CPG] = differential_neuron;
  }
}


boost::shared_ptr<CPPNConfig>
convertForController(CPPNEAT::GeneticEncodingPtr genotype)
{
  assert(!genotype->is_layered_);
  std::vector<CPPNEAT::NeuronGenePtr> neuron_genes = genotype->neuron_genes_;
  std::vector<CPPNEAT::ConnectionGenePtr> connection_genes = genotype->connection_genes_;

  std::map<int, NeuronPtr> innov_number_to_neuron;

  boost::shared_ptr<CPPNConfig> config(new CPPNConfig());
  for (CPPNEAT::NeuronGenePtr neuron_gene : neuron_genes) {
    NeuronPtr newNeuron;
    std::string neuronId = neuron_gene->neuron->neuron_id;
    std::map<std::string, double> neuron_params = neuron_gene->neuron->neuron_params;

    switch (neuron_gene->neuron->layer) {
      case CPPNEAT::Neuron::INPUT_LAYER: {
        newNeuron.reset(new InputNeuron(neuronId, neuron_params));
        config->inputNeurons_.push_back(newNeuron);
        config->inputPositionMap_[newNeuron] = input_map[neuron_gene->getInnovNumber()];
        break;
      }
      case CPPNEAT::Neuron::HIDDEN_LAYER: {
        switch (neuron_gene->neuron->neuron_type) {
          case CPPNEAT::Neuron::INPUT:
          case CPPNEAT::Neuron::SIMPLE: {
            newNeuron.reset(new LinearNeuron(neuronId, neuron_params));
            break;
          }
          case CPPNEAT::Neuron::SIGMOID: {
            newNeuron.reset(new SigmoidNeuron(neuronId, neuron_params));
            break;
          }
          case CPPNEAT::Neuron::DIFFERENTIAL_CPG: {
            newNeuron.reset(new DifferentialCPG(neuronId, neuron_params));
            break;
          }
          case CPPNEAT::Neuron::BIAS: {
            newNeuron.reset(new BiasNeuron(neuronId, neuron_params));
            break;
          }
          case CPPNEAT::Neuron::OSCILLATOR: {
            newNeuron.reset(new OscillatorNeuron(neuronId, neuron_params));
            break;
          }
          case CPPNEAT::Neuron::INPUT_OSCILLATOR: {
            newNeuron.reset(new InputDependentOscillatorNeuron(neuronId, neuron_params));
            break;
          }
          default: {
            throw std::runtime_error("Unkown neuron type to be converted");
          }

        }
        config->hiddenNeurons_.push_back(newNeuron);
        break;
      }
      case CPPNEAT::Neuron::OUTPUT_LAYER: {
        switch (neuron_gene->neuron->neuron_type) {
          case CPPNEAT::Neuron::INPUT:
          case CPPNEAT::Neuron::SIMPLE: {
            newNeuron.reset(new LinearNeuron(neuronId, neuron_params));
            break;
          }
          case CPPNEAT::Neuron::SIGMOID: {
            newNeuron.reset(new SigmoidNeuron(neuronId, neuron_params));
            break;
          }
          case CPPNEAT::Neuron::DIFFERENTIAL_CPG: {
            newNeuron.reset(new DifferentialCPG(neuronId, neuron_params));
            break;
          }
          case CPPNEAT::Neuron::BIAS: {
            newNeuron.reset(new BiasNeuron(neuronId, neuron_params));
            break;
          }
          case CPPNEAT::Neuron::OSCILLATOR: {
            newNeuron.reset(new OscillatorNeuron(neuronId, neuron_params));
            break;
          }
          case CPPNEAT::Neuron::INPUT_OSCILLATOR: {
            newNeuron.reset(new InputDependentOscillatorNeuron(neuronId, neuron_params));
            break;
          }
          default: {
            throw std::runtime_error("Unknown neuron type to be converted");
          }

        }
        config->outputNeurons_.push_back(newNeuron);
        config->outputPositionMap_[newNeuron] = output_map[neuron_gene->getInnovNumber()];
        break;
      }
      default: {
        throw std::runtime_error("Robot brain error");
      }
    }
    config->allNeurons_.push_back(newNeuron);
    config->idToNeuron_[neuronId] = newNeuron;
    innov_number_to_neuron[neuron_gene->getInnovNumber()] = newNeuron;
  }
  for (CPPNEAT::ConnectionGenePtr connection_gene : connection_genes) {
    NeuronPtr dst = innov_number_to_neuron[connection_gene->mark_to];
    NeuralConnectionPtr newConnection(new NeuralConnection(innov_number_to_neuron[connection_gene->mark_from],
                                                           dst,
                                                           connection_gene->weight));
    dst->AddIncomingConnection(dst->GetSocketId(), newConnection);
    config->connections_.push_back(newConnection);
  }
  known_genotypes[config] = genotype;
  return config;

}

CPPNEAT::GeneticEncodingPtr
convertForLearner(boost::shared_ptr<CPPNConfig> config)
{
  return known_genotypes[config];
}




///////////////////////////////////////////////////////////////////////////////
/// RLPower_CPG~RLPower_CPPN
///////////////////////////////////////////////////////////////////////////////
std::vector<double> convertPolicyToDouble(PolicyPtr genotype)
{
  return (*genotype)[0];
}

PolicyPtr convertDoubleToNull(std::vector<double> phenotype)
{
  return nullptr; //input is not taken into account
}
///////////////////////////////////////////////////////////////////////////////


boost::shared_ptr<CPPNConfig> cpg_network;

std::map<std::string, std::tuple<int, int, int>> neuron_coordinates;

CPPNEAT::GeneticEncodingPtr last_genotype_;

boost::shared_ptr<LayeredExtNNConfig>
convertForLayeredExtNN(CPPNEAT::GeneticEncodingPtr genotype)
{
  assert(genotype->is_layered_);
  std::vector<std::vector<CPPNEAT::NeuronGenePtr>> layers = genotype->layers_;
  std::vector<CPPNEAT::ConnectionGenePtr> connection_genes = genotype->connection_genes_;

  std::map<int, NeuronPtr> neuron_inovation_numbers;

  boost::shared_ptr<LayeredExtNNConfig> cppn(new LayeredExtNNConfig());
  cppn->layers_ = std::vector<std::vector<NeuronPtr>>(layers.size(), std::vector<NeuronPtr>());

  for (size_t i = 0; i < layers.size(); i++) {
    for (CPPNEAT::NeuronGenePtr neuron_gene : layers[i]) {
      NeuronPtr new_neuron;
      std::string neuronId = neuron_gene->neuron->neuron_id;
      std::map<std::string, double> neuron_params = neuron_gene->neuron->neuron_params;

      switch (neuron_gene->neuron->layer) {
        case CPPNEAT::Neuron::INPUT_LAYER: {
          new_neuron.reset(new InputNeuron(neuronId, neuron_params));
          cppn->layers_[i].push_back(new_neuron);
          cppn->inputPositionMap_[new_neuron] = input_map[neuron_gene->getInnovNumber()];
          break;
        }
        case CPPNEAT::Neuron::HIDDEN_LAYER: {
          switch (neuron_gene->neuron->neuron_type) {
            case CPPNEAT::Neuron::INPUT:
            case CPPNEAT::Neuron::SIMPLE: {
              new_neuron.reset(new LinearNeuron(neuronId, neuron_params));
              break;
            }
            case CPPNEAT::Neuron::SIGMOID: {
              new_neuron.reset(new SigmoidNeuron(neuronId, neuron_params));
              break;
            }
            case CPPNEAT::Neuron::DIFFERENTIAL_CPG: {
              new_neuron.reset(new DifferentialCPG(neuronId, neuron_params));
              break;
            }
            case CPPNEAT::Neuron::BIAS: {
              new_neuron.reset(new BiasNeuron(neuronId, neuron_params));
              break;
            }
            case CPPNEAT::Neuron::OSCILLATOR: {
              new_neuron.reset(new OscillatorNeuron(neuronId, neuron_params));
              break;
            }
            case CPPNEAT::Neuron::INPUT_OSCILLATOR: {
              new_neuron.reset(new InputDependentOscillatorNeuron(neuronId, neuron_params));
              break;
            }
            default: {
              throw std::runtime_error("Unkown neuron type to be converted");
            }
          }
          cppn->layers_[i].push_back(new_neuron);
          break;
        }
        case CPPNEAT::Neuron::OUTPUT_LAYER: {
          switch (neuron_gene->neuron->neuron_type) {
            case CPPNEAT::Neuron::INPUT:
            case CPPNEAT::Neuron::SIMPLE: {
              new_neuron.reset(new LinearNeuron(neuronId, neuron_params));
              break;
            }
            case CPPNEAT::Neuron::SIGMOID: {
              new_neuron.reset(new SigmoidNeuron(neuronId, neuron_params));
              break;
            }
            case CPPNEAT::Neuron::DIFFERENTIAL_CPG: {
              new_neuron.reset(new DifferentialCPG(neuronId, neuron_params));
              break;
            }
            case CPPNEAT::Neuron::BIAS: {
              new_neuron.reset(new BiasNeuron(neuronId, neuron_params));
              break;
            }
            case CPPNEAT::Neuron::OSCILLATOR: {
              new_neuron.reset(new OscillatorNeuron(neuronId, neuron_params));
              break;
            }
            case CPPNEAT::Neuron::INPUT_OSCILLATOR: {
              new_neuron.reset(new InputDependentOscillatorNeuron(neuronId, neuron_params));
              break;
            }
            default: {
              throw std::runtime_error("Unknown neuron type to be converted");
            }

          }
          cppn->layers_[i].push_back(new_neuron);
          cppn->outputPositionMap_[new_neuron] = output_map[neuron_gene->getInnovNumber()];
          break;
        }
        default: {
          throw std::runtime_error("Robot brain error");
        }
      }
      cppn->idToNeuron_[neuronId] = new_neuron;
      neuron_inovation_numbers[neuron_gene->getInnovNumber()] = new_neuron;
    }
  }
  for (CPPNEAT::ConnectionGenePtr connection_gene : connection_genes) {
    NeuronPtr destination_neuron = neuron_inovation_numbers[connection_gene->mark_to];
    NeuralConnectionPtr newConnection(new NeuralConnection(neuron_inovation_numbers[connection_gene->mark_from],
                                                           destination_neuron,
                                                           connection_gene->weight));
    destination_neuron->AddIncomingConnection(destination_neuron->GetSocketId(), newConnection);
    cppn->connections_.push_back(newConnection);
  }
  return cppn;
}

void write_debugplot(boost::shared_ptr<CPPNConfig> conf,
                     bool include_coordinates)
{
  std::ofstream write_to("debug_plot_extnn.dot");
  boost::adjacency_list<> graph(conf->allNeurons_.size());
  for (size_t i = 0; i < conf->allNeurons_.size(); i++) {
    std::vector<std::pair<std::string, NeuralConnectionPtr>> connectionsToAdd =
            conf->allNeurons_[i]->getIncomingConnections();

    for (std::pair<std::string, NeuralConnectionPtr> connectionToAdd : connectionsToAdd) {
      NeuronPtr input = connectionToAdd.second->GetInputNeuron();
      long indexInput = std::find(conf->allNeurons_.begin(),
                                 conf->allNeurons_.end(),
                                 input) - conf->allNeurons_.begin();
      boost::add_edge(indexInput, i, graph);
    }
  }

  std::string *names = new std::string[conf->allNeurons_.size()];
  for (size_t i = 0; i < conf->allNeurons_.size(); i++) {
    std::stringstream nodeName;
    nodeName << conf->allNeurons_[i]->Id() + " of type: " + conf->allNeurons_[i]->getType() << std::endl;
    for (std::pair<std::string, double> param : conf->allNeurons_[i]->getNeuronParameters()) {
      nodeName << param.first << ": " << param.second << std::endl;
    }
    if (include_coordinates) {
      std::tuple<int, int, int> coord = neuron_coordinates[conf->allNeurons_[i]->Id()];
      nodeName << "(x,y,z) = (" << std::get<0>(coord)
               << "," << std::get<1>(coord)
               << "," << std::get<2>(coord)
               << ")" << std::endl;
    }
    names[i] = nodeName.str();
  }
  boost::write_graphviz(write_to, graph, boost::make_label_writer(names));
  delete[] names;
}

//////////////////////////////////////////////////////////////////////////////
/// HyperNEAT_CPG
///////////////////////////////////////////////////////////////////////////////
boost::shared_ptr<CPPNConfig>
convertGeneticEncodingToCPPNConfig(CPPNEAT::GeneticEncodingPtr genotype)
{
  boost::shared_ptr<LayeredExtNNConfig> cppn = convertForLayeredExtNN(genotype);
  for (NeuralConnectionPtr connection : cpg_network->connections_) {
    NeuronPtr src_neuron = connection->GetInputNeuron();
    NeuronPtr dst_neuron = connection->GetOutputNeuron();
    std::tuple<int, int, int> coord_src = neuron_coordinates[src_neuron->Id()];
    std::tuple<int, int, int> coord_dst = neuron_coordinates[dst_neuron->Id()];
    for (NeuronPtr neuron : cppn->layers_[0]) {
      //could be faster by neuron->Id()[6] but less easy to read
      if (neuron->Id() == "Input-0") {
        neuron->SetInput(std::get<0>(coord_src));
      } else if (neuron->Id() == "Input-1") {
        neuron->SetInput(std::get<1>(coord_src));
      } else if (neuron->Id() == "Input-2") {
        neuron->SetInput(std::get<2>(coord_src));
      } else if (neuron->Id() == "Input-3") {
        neuron->SetInput(std::get<0>(coord_dst));
      } else if (neuron->Id() == "Input-4") {
        neuron->SetInput(std::get<1>(coord_dst));
      } else if (neuron->Id() == "Input-5") {
        neuron->SetInput(std::get<2>(coord_dst));
      }
    }

    for (std::vector<NeuronPtr> layer : cppn->layers_) {
      for (NeuronPtr neuron : layer) {
        neuron->Update(0);
      }
      for (NeuronPtr neuron : layer) {
        neuron->FlipState();
      }
    }

    for (auto it = cppn->layers_[cppn->layers_.size() - 1].begin();
         it != cppn->layers_[cppn->layers_.size() - 1].end(); ++it) {
      auto outNeuron = *it;
      if (outNeuron->Id() == "weight") {
        connection->SetWeight(outNeuron->GetOutput());
        break;
      }
    }
  }
  for (NeuronPtr neuron : cpg_network->allNeurons_) {

    // Retrieve coordinates of source and destination neuron
    std::tuple<int, int, int> coord_src = neuron_coordinates[neuron->Id()];
    std::tuple<int, int, int> coord_dst = std::make_tuple(0, 0, 0);
    for (NeuronPtr input_neuron : cppn->layers_[0]) {
      //could be faster by input_neuron->Id()[6] but less easy to read
      if (input_neuron->Id() == "Input-0") {
        input_neuron->SetInput(std::get<0>(coord_src));
      } else if (input_neuron->Id() == "Input-1") {
        input_neuron->SetInput(std::get<1>(coord_src));
      } else if (input_neuron->Id() == "Input-2") {
        input_neuron->SetInput(std::get<2>(coord_src));
      } else if (input_neuron->Id() == "Input-3") {
        input_neuron->SetInput(std::get<0>(coord_dst));
      } else if (input_neuron->Id() == "Input-4") {
        input_neuron->SetInput(std::get<1>(coord_dst));
      } else if (input_neuron->Id() == "Input-5") {
        input_neuron->SetInput(std::get<2>(coord_dst));
      }
    }

    // Feed CPPN with input coordinates
    for (std::vector<NeuronPtr> layer : cppn->layers_) {
      for (NeuronPtr hidden_neuron : layer) {
        hidden_neuron->Update(0);
      }
      for (NeuronPtr hidden_neuron : layer) {
        hidden_neuron->FlipState();
      }
    }


    std::map<std::string, double> params;
    for (auto it = cppn->layers_[cppn->layers_.size() - 1].begin();
         it != cppn->layers_[cppn->layers_.size() - 1].end(); ++it) {
      auto outNeuron = *it;
      params[outNeuron->Id()] = outNeuron->GetOutput();
    }
    neuron->setNeuronParameters(params);
  }
  last_genotype_ = genotype;
  // write_debugplot(cpg_network, true);
  return cpg_network;
}

CPPNEAT::GeneticEncodingPtr
convertCPPNConfigToGeneticEncoding(boost::shared_ptr<CPPNConfig> config)
{
  return last_genotype_;
}
///////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////
/// HyperNEAT_Splines
///////////////////////////////////////////////////////////////////////////////
std::vector<std::pair<int, int>> sorted_coordinates;

PolicyPtr policy;

size_t spline_size;
size_t update_rate;
size_t cur_step = 0;

CPPNEAT::GeneticEncodingPtr get_hyper_neat_net_splines()
{
  int innov_number = 1;
  CPPNEAT::GeneticEncodingPtr ret(new CPPNEAT::GeneticEncoding(true));

  // Add input layer (x, y, z) for splines
  std::map<std::string, double> initial_neuron_params;
  for (int i = 0; i < 3; i++) {
    CPPNEAT::NeuronPtr neuron(new CPPNEAT::Neuron(
            //better names (like input x1 etc) might help
            "Input-" + std::to_string(i),
            CPPNEAT::Neuron::INPUT_LAYER,
            CPPNEAT::Neuron::INPUT,
            initial_neuron_params));

    // Increment innovation number
    CPPNEAT::NeuronGenePtr neuron_gene(new CPPNEAT::NeuronGene(neuron, innov_number++, true));
    ret->add_neuron_gene(neuron_gene, 0, i == 0);
  }

  // Add output layer
  initial_neuron_params["rv:bias"] = 0;
  initial_neuron_params["rv:gain"] = 0;
  CPPNEAT::NeuronPtr weight_neuron(new CPPNEAT::Neuron("weight",
                                                       CPPNEAT::Neuron::OUTPUT_LAYER,
                                                       CPPNEAT::Neuron::SIMPLE,
                                                       initial_neuron_params));
  CPPNEAT::NeuronGenePtr weight_neuron_gene(new CPPNEAT::NeuronGene(weight_neuron,
                                                                    innov_number++,
                                                                    true));
  ret->add_neuron_gene(weight_neuron_gene, 1, true);

  // Connect every input with every output
  for (size_t i = 0; i < 3; i++) {
    CPPNEAT::ConnectionGenePtr connection_to_weight(
            new CPPNEAT::ConnectionGene(weight_neuron_gene->getInnovNumber(), i + 1, 0, innov_number++, true, ""));
    ret->add_connection_gene(connection_to_weight);
  }
  return ret;
}

PolicyPtr convertForSplinesFromHyper(CPPNEAT::GeneticEncodingPtr genotype)
{
  //TODO::fix update rate
  boost::shared_ptr<LayeredExtNNConfig> cppn = convertForLayeredExtNN(genotype);
  if (policy == nullptr) {
    policy = PolicyPtr(new Policy(sorted_coordinates.size(), Spline(spline_size, 0)));
  }
  if (++cur_step >= update_rate) {
    spline_size++;
    cur_step = 0;
    policy = PolicyPtr(new Policy(sorted_coordinates.size(), Spline(spline_size, 0)));
  }
  for (size_t j = 0; j < sorted_coordinates.size(); j++) {
    for (size_t i = 0; i < spline_size; i++) {
      std::tuple<double, double, double> coord(sorted_coordinates[j].first,
                                               sorted_coordinates[j].second,
                                               i / ((double)spline_size));
      for (NeuronPtr neuron : cppn->layers_[0]) {
        //could be faster by neuron->Id()[6] but less easy to read
        if (neuron->Id() == "Input-0") {
          neuron->SetInput(std::get<0>(coord));
        } else if (neuron->Id() == "Input-1") {
          neuron->SetInput(std::get<1>(coord));
        } else if (neuron->Id() == "Input-2") {
          neuron->SetInput(std::get<2>(coord));
        }
      }

      for (std::vector<NeuronPtr> layer : cppn->layers_) {
        for (NeuronPtr neuron : layer) {
          neuron->Update(0);
        }
        for (NeuronPtr neuron : layer) {
          neuron->FlipState();
        }
      }

      // Retrieve the output value
      for (auto it = cppn->layers_[cppn->layers_.size() - 1].begin();
           it != cppn->layers_[cppn->layers_.size() - 1].end();
           ++it) {
        auto outNeuron = *it;
        if (outNeuron->Id() == "weight") {
          (*policy)[j][i] = outNeuron->GetOutput();
          break;
        }
      }
    }
  }
  last_genotype_ = genotype;
  return policy;
}

CPPNEAT::GeneticEncodingPtr
convertForHyperFromSplines(PolicyPtr policy)
{
  return last_genotype_;
}


}
}