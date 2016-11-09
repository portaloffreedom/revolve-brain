//
// Created by matteo on 01/11/16.
//

#include "RythmGenerationNeuron.h"
#include <sstream>
#include <cmath>

using namespace revolve::brain::cpg;



RythmGenerationNeuron::RythmGenerationNeuron(real_t weight, real_t c, real_t amplitude, real_t offset)
        : phi(1)
{
    setWeight(weight);
    setC(c);
    setAmplitude(amplitude);
    setOffset(offset);
}

RythmGenerationNeuron::~RythmGenerationNeuron()
{

}

std::vector<real_t> RythmGenerationNeuron::update(std::vector<real_t> inputs, real_t delta_time) {
    // reading neuron inputs
    if (inputs.size() != 1) {
        std::stringstream ss;
        ss << "input size should be 1, instead is " << inputs.size();
        throw invalid_input_exception(ss.str());
    }
    real_t otherPhi = inputs[0];

    // creating output value from previous phi
    real_t _output = output();

    // updating phi to the new value
    // NOTE we update phi after getting the output value because this way both
    // phi and other phi are of the same cycle.
    phi = nextPhi(otherPhi, delta_time);

    return {_output, phi};
}

real_t RythmGenerationNeuron::nextPhi(const real_t otherPhi, real_t delta_time) const {
    static const real_t PI = std::acos(-1); // define PI

    const real_t thisPhi = this->phi;

    // (2 * pi * c') + w * sin(otherPhi-thisPhi)
    const real_t deltaPhi =
            2 * PI * this->c +
            this->weight * std::sin(otherPhi-thisPhi);

    real_t new_phi = thisPhi + deltaPhi;

    //TODO compensate for phi error
//    if (new_phi > PI*2)
//        new_phi -= PI*2;
//    else if (new_phi < -PI*2)
//        new_phi += PI*2;

    //TODO consider delta_time
    return new_phi;
}

real_t RythmGenerationNeuron::output() const {
    // A * cos(phi) + o
    return (this->amplitude * std::cos(this->phi))
           + this->offset;
}

// GETTERS AND SETTERS --------------------------------------------------------

real_t RythmGenerationNeuron::getPhi() const {
    return phi;
}

real_t RythmGenerationNeuron::getWeight() const {
    return weight;
}

// the weight that determines the shape [0, 4.5]
void RythmGenerationNeuron::setWeight(real_t weight) {
    if (weight < WEIGHT_MIN || weight > WEIGHT_MAX)
        throw invalid_parameter("weight", weight, WEIGHT_MIN, WEIGHT_MAX);
    RythmGenerationNeuron::weight = weight;
}

real_t RythmGenerationNeuron::getC() const {
    return c;
}

// the phase difference [-2, 2]
void RythmGenerationNeuron::setC(real_t c) {
    if (c < C_MIN || c > C_MAX)
        throw invalid_parameter("c", c, C_MIN, C_MAX);
    RythmGenerationNeuron::c = c;
}

real_t RythmGenerationNeuron::getAmplitude() const {
    return amplitude;
}

//  the amplitude determines influence of a flexor/extensor on the final output signal [−10, 10]
void RythmGenerationNeuron::setAmplitude(real_t amplitude) {
    if (amplitude < AMPLITUDE_MIN || amplitude > AMPLITUDE_MAX)
        throw invalid_parameter("amplitude", amplitude, AMPLITUDE_MIN, AMPLITUDE_MAX);
    RythmGenerationNeuron::amplitude = amplitude;
}

real_t RythmGenerationNeuron::getOffset() const {
    return offset;
}

//  the offset dims shape of the final output signal to the starting position [-0.1, 0.1]
void RythmGenerationNeuron::setOffset(real_t offset) {
    if (offset < OFFSET_MIN || offset > OFFSET_MAX)
        throw invalid_parameter("offset", offset, OFFSET_MIN, OFFSET_MAX);
    RythmGenerationNeuron::offset = offset;
}


// FROM PERCENTAGE ------------------------------------------------------------
real_t percentage_from_range(real_t percentage, real_t range_start, real_t range_end)
{
    if (percentage > 1)
        return range_end;
    else if (percentage < 0)
        return range_start;

    real_t range = range_end - range_start;
    real_t distance = percentage * range;
    return range_start + distance;
}

real_t revolve::brain::cpg::RythmGenerationNeuron::calculateWeightFromPercentage(real_t weight) const
{
    return percentage_from_range(weight, WEIGHT_MIN, WEIGHT_MAX);
}

real_t revolve::brain::cpg::RythmGenerationNeuron::calculateCFromPercentage(real_t c) const
{
    return percentage_from_range(c, C_MIN, C_MAX);
}

real_t revolve::brain::cpg::RythmGenerationNeuron::calculateAmplitudeFromPercentage(real_t amplitude) const
{
    return percentage_from_range(amplitude, AMPLITUDE_MIN, AMPLITUDE_MAX);
}

real_t revolve::brain::cpg::RythmGenerationNeuron::calculateOffsetFromPercentage(real_t offset) const
{
    return percentage_from_range(offset, OFFSET_MIN, OFFSET_MAX);
}


// TO PERCENTAGE --------------------------------------------------------------
real_t percentage_of_range(real_t value, real_t range_start, real_t range_end)
{
    if (value > range_end)
        return 1;
    else if (value < range_start)
        return 0;

    real_t range = range_end - range_start;
    value = value - range_start;
    return value / range;
}

real_t revolve::brain::cpg::RythmGenerationNeuron::calculateWeightPercentage(real_t weight) const
{
    return percentage_of_range(weight, WEIGHT_MIN, WEIGHT_MAX);
}

real_t revolve::brain::cpg::RythmGenerationNeuron::calculateCPercentage(real_t c) const
{
    return percentage_of_range(c, C_MIN, C_MAX);
}

real_t revolve::brain::cpg::RythmGenerationNeuron::calculateAmplitudePercentage(real_t amplitude) const
{
    return percentage_of_range(amplitude, AMPLITUDE_MIN, AMPLITUDE_MAX);
}

real_t revolve::brain::cpg::RythmGenerationNeuron::calculateOffsetPercentage(real_t offset) const
{
    return percentage_of_range(offset, OFFSET_MIN, OFFSET_MAX);
}



real_t RythmGenerationNeuron::setWeightPercentage(real_t weight) {
    this->weight = calculateWeightFromPercentage(weight);
    return this->weight;
}

real_t RythmGenerationNeuron::setCPercentage(real_t c) {
    this->c = calculateCFromPercentage(c);
    return this->c;
}

real_t RythmGenerationNeuron::setAmplitudePercentage(real_t amplitude) {
    this->amplitude = calculateAmplitudeFromPercentage(amplitude);
    return this->amplitude;
}

real_t RythmGenerationNeuron::setOffsetPercentage(real_t offset) {
    this->offset = calculateOffsetFromPercentage(offset);
    return this->offset;
}
