#ifndef REVOLVEBRAIN_BRAIN_CONTROLLER_EXTENDEDANN_H_
#define REVOLVEBRAIN_BRAIN_CONTROLLER_EXTENDEDANN_H_

#include "Controller.h"
#include "brain/Evaluator.h"
#include "brain/controller/extnn/ENeuron.h"
#include "brain/controller/extnn/NeuralConnection.h"
#include <map>
#include <vector>
#include <string>

#include "brain/Types.h"
#include "brain/controller/extnn/LinearNeuron.h"
#include "brain/controller/extnn/SigmoidNeuron.h"
#include "brain/controller/extnn/OscillatorNeuron.h"
#include "brain/controller/extnn/VOscillator.h"
#include "brain/controller/extnn/XOscillator.h"
#include "brain/controller/extnn/LeakyIntegrator.h"
#include "brain/controller/extnn/BiasNeuron.h"
#include "brain/controller/extnn/DifferentialCPG.h"
#include "brain/controller/extnn/InputNeuron.h"
#include "brain/controller/extnn/InputDependentOscillatorNeuron.h"

namespace revolve {
namespace brain {

struct CPPNConfig
{
    std::vector<NeuronPtr> allNeurons_; //vector containing all neurons
    std::vector<NeuronPtr> inputNeurons_; //vector containing the input neurons
    std::vector<NeuronPtr> outputNeurons_; //vector containing the output neurons
    std::vector<NeuronPtr> hiddenNeurons_; //vector containing the hidden neurons

    std::map<NeuronPtr, int> outputPositionMap_;    // positions for indexing into the outputs_ buffer for each output neuron
    std::map<NeuronPtr, int> inputPositionMap_;    // positions for indexing into the inputs_ buffer for each input neuron

    std::map<std::string, NeuronPtr> idToNeuron_;    // Map neuron id strings to Neuron objects

    std::vector<NeuralConnectionPtr> connections_; //vector of all the neural connections
};

//extended neural network controller usable with standard neat or hyper neat (use different conversion methods)
class RafCPGController : public Controller<CPPNConfigPtr>
{
  public:
  /**
   * Constructor for a neural network including neurons that are of a different type than the usual ones.
   * @param model_name: name of the model
   * @param _config: configuration file
   * @param evaluator: pointer to the evaluator that is used
   * @param actuators: vector list of robot's actuators
   * @param sensors: vector list of robot's sensors
   * @return pointer to the neural network
   */
  RafCPGController(std::string model_name,
                 CPPNConfigPtr _config,
                 const std::vector<ActuatorPtr> &actuators,
                 const std::vector<SensorPtr> &sensors);

  virtual ~RafCPGController();

  /**
  * Method for updating sensors readings, actuators positions
  * @param actuators: vector list of robot's actuators
  * @param sensors: vector list of robot's sensors
  * @param t: current time
  * @param step:
  */
  virtual void update(const std::vector<ActuatorPtr> &actuators,
                      const std::vector<SensorPtr> &sensors,
                      double t,
                      double step);

  /**
   * Gets the weight of all the connections
   * @return weights of all neural connections
   */
  virtual CPPNConfigPtr getPhenotype();

  /**
   * Changes the weights of the neural connections
   * @param weights: new weights to be assigned
   */
  virtual void setPhenotype(CPPNConfigPtr _config);

  void writeNetwork(std::ofstream &write_to);

  protected:

  std::string modelName_; //name of the robot

  double *inputs_;    // buffer of input values from the sensors
  double *outputs_;     // buffer of output values for the actuators

  std::vector<NeuronPtr> allNeurons_; //vector containing all neurons
  std::vector<NeuronPtr> inputNeurons_; //vector containing the input neurons
  std::vector<NeuronPtr> outputNeurons_; //vector containing the output neurons
  std::vector<NeuronPtr> hiddenNeurons_; //vector containing the hidden neurons

  std::map<NeuronPtr, int> outputPositionMap_;    // positions for indexing into the outputs_ buffer for each output neuron
  std::map<NeuronPtr, int> inputPositionMap_;    // positions for indexing into the inputs_ buffer for each input neuron

  std::map<std::string, NeuronPtr> idToNeuron_;    // Map neuron id strings to Neuron objects

  std::vector<NeuralConnectionPtr> connections_; //vector of all the neural connections

};


}
}

#endif // REVOLVEBRAIN_BRAIN_CONTROLLER_EXTENDEDANN_H_