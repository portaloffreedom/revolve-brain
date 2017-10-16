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

#ifndef REVOLVEBRAIN_BRAIN_LEARNER_CPPNNEAT_MUTATOR_H_
#define REVOLVEBRAIN_BRAIN_LEARNER_CPPNNEAT_MUTATOR_H_

#include <map>
#include <string>
#include <vector>
#include <random>
#include <utility>

#include "GeneticEncoding.h"

/// \brief class responsible for mutation
namespace cppneat
{
  class Mutator
  {
    public:
    /// \brief
    Mutator(
            std::map< Neuron::Ntype, Neuron::NeuronTypeSpec > &_specification,
            const double _tauConnectionSigma,
            const size_t _innovationNumber,
            const size_t _maxAttempts,
            std::vector< Neuron::Ntype > _addableNeurons);

    /// \brief
    static std::vector< Neuron::Ntype >
    AddableTypes(
            std::map< Neuron::Ntype, Neuron::NeuronTypeSpec > _specification);

    /// \brief
    void RegisterStartingGenotype(GeneticEncodingPtr _genotype);

    /// \brief
    void LoadRegisteredInnovations(const std::string &_yamlPath);

    /// \brief
    void InsertConnectionInnovation(
            const size_t _from,
            const size_t _to,
            const size_t _innovationNumber);

    /// \brief
    void InsertNeuronInnovation(
            const Neuron::Ntype _neuronType,
            const size_t _innovationNumber);

    /// \brief
    void RecordInnovations(const std::string &_yamlPath);

    /// \brief
    void MutateNeuronParams(
            GeneticEncodingPtr _genotype,
            const double _probability,
            const double _sigma);

    /// \brief
    void MutateWeights(
            GeneticEncodingPtr genotype,
            const double _probability,
            const double _sigma);

    /// \brief
    void MutateStructure(
            GeneticEncodingPtr _genotype,
            const double _probability);

    /// \brief
    bool AddConnectionMutation(
            GeneticEncodingPtr _genotype,
            const double _sigma);

    /// \brief
    void AddNeuronMutation(
            GeneticEncodingPtr _genotype,
            const double _sigma);

    /// \brief
    void RemoveConnectionMutation(GeneticEncodingPtr _genotype);

    /// \brief
    void RemoveNeuronMutation(GeneticEncodingPtr _genotype);

    /// \brief
    size_t AddNeuron(
            NeuronPtr _neuron,
            GeneticEncodingPtr _genotype,
            ConnectionGenePtr _split);

    /// \brief
    size_t AddConnection(
            const size_t _from,
            const size_t _to,
            const double _weight,
            GeneticEncodingPtr _genotype,
            const std::string &_socket);

    /// \brief
    std::map< Neuron::Ntype, Neuron::NeuronTypeSpec > Specification()
    {
      return this->specification_;
    };

    /// \brief
    void SetCurrentInnovationNumber(const size_t _innovationNumber)
    {
      this->innovationNumber_ = _innovationNumber;
    };

    /// \brief <mark_from, mark_to> -> innovation_number
    private:
    std::map< std::pair< size_t, size_t >, size_t > connectionInnovations_;

    /// \brief contains all connections that ever existed!
    /// <innovation_number of split connection,
    /// neuron_type of added neuron>
    ///  ->
    /// innovation_numbers of added neurons
    /// since we want the ability to have multiple neurons of the same type
    /// added between two neurons
    /// it is necessary that we check if a new neuron has already been added to
    /// the current genotype
    /// if so we add another neuron
    /// in order to store all the innovation numbers we need a vector*/
    private:
    std::map< std::pair< size_t, Neuron::Ntype >, std::vector< size_t>>
            neuronInnovations_;

    /// \brief contains only neurons that have been added by structural mutation
    private:
    std::map< Neuron::Ntype, Neuron::NeuronTypeSpec > specification_;

    /// \brief
    private:
    double tauConnectionSigma_;

    /// \brief
    private:
    size_t innovationNumber_;

    /// \brief
    private:
    size_t maxAttempts_;

    /// \brief
    private:
    std::vector< Neuron::Ntype > addableNeurons_;

    /// \brief
    private:
    std::mt19937 generator_;
  };
}

#endif  // NEAT_MUTATOR_H_
