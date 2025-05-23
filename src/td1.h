#pragma once
#include "wheel.h"
#include "bluetooth.h"

class Potentiometer                                 //Begin Potentiometer class definition
{
private:                                            //Private data member declaration
    AnalogIn inputSignal;                           //Declaration of AnalogIn object
    float VDD, currentSampleNorm, currentSampleVolts; //Float variables to speficy the value of VDD and most recent samples

public:                                             // Public declarations
    Potentiometer(PinName pin, float v) : inputSignal(pin), VDD(v) {}   //Constructor - user provided pin name assigned to AnalogIn...
                                                                        //VDD is also provided to determine maximum measurable voltage
    float amplitudeVolts(void)                      //Public member function to measure the amplitude in volts
    {
        return (inputSignal.read()*VDD);            //Scales the 0.0-1.0 value by VDD to read the input in volts
    }
    
    float amplitudeNorm(void)                       //Public member function to measure the normalised amplitude
    {
        return inputSignal.read();                  //Returns the ADC value normalised to range 0.0 - 1.0
    }
    
    void sample(void)                               //Public member function to sample an analogue voltage
    {
        currentSampleNorm = inputSignal.read();       //Stores the current ADC value to the class's data member for normalised values (0.0 - 1.0)
        currentSampleVolts = currentSampleNorm * VDD; //Converts the normalised value to the equivalent voltage (0.0 - 3.3 V) and stores this information
    }
    
    float getCurrentSampleVolts(void)               //Public member function to return the most recent sample from the potentiometer (in volts)
    {
        return currentSampleVolts;                  //Return the contents of the data member currentSampleVolts
    }
    
    float getCurrentSampleNorm(void)                //Public member function to return the most recent sample from the potentiometer (normalised)
    {
        return currentSampleNorm;                   //Return the contents of the data member currentSampleNorm  
    }

};


class SamplingPotentiometer : public Potentiometer { 
 
private:                                                                 
    float samplingFrequency, samplingPeriod; 
    Ticker sampler;
    float currentValue;

    void sampleAndMap() {
        this->sample();
        float normValue = getCurrentSampleNorm(); // get norm value (0.0 - 1.0)
        currentValue = normValue * 100; 
    }                                                      
 
public:                                                                  
    SamplingPotentiometer(PinName pin, float v, float fs)
        : Potentiometer(pin, v), samplingFrequency(fs) {
        samplingPeriod = 1.0f / samplingFrequency;
        sampler.attach(callback(this, &SamplingPotentiometer::sampleAndMap), samplingPeriod);
    }

    // get minutes
    int getPercentage() {
        return static_cast<int>(currentValue);
    }
 
};

extern C12832 lcd;

extern SamplingPotentiometer potLeft;
extern SamplingPotentiometer potRight;

void task2();
void task3();
void task4();
