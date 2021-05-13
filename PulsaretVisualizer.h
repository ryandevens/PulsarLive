/*
  ==============================================================================

    PulsaretVisualizer.h
    Created: 6 Nov 2020 4:34:21pm
    Author:  Billie (Govnah) Jean

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PulsaretTable.h"


//==============================================================================
/*
*/
class PulsaretVisualizer : public juce::Component //, juce::Timer
{
public:
    PulsaretVisualizer(PulsaretTable& t);
    
    ~PulsaretVisualizer() override;

    void paint (juce::Graphics& g) override;
    void resized() override;
    void setValue();
    
    void setTable(PulsaretTable& table);
    
//    void timerCallback() override
//    {
//
//        repaint();
//    }
    
    void setNewWaveColour(juce::Colour newColour)
    {
        waveColour = newColour;
    }
    
    void setAmp(float a)
    {
        amp = std::abs(a);
    }
    
    
private:
    PulsaretTable& pulsaretTable;
    bool isDrawing = false;
    
    std::unique_ptr<juce::Label> valueLabel;
    juce::String value = "value";
    
    juce::Point<float> mousePosition;
    juce::Point<float> lastPosition;

    juce::Path wavePath;
    
    float waveValue = 0.f;
    
    juce::Colour    waveColour;
    juce::Colour    bgColour;
    float waveSat;
    float waveHue;
    float waveVal;
    float bgSat;
    float bgHue = 0.5f;
    float bgVal;
    float amp = 1.f;

    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PulsaretVisualizer)
};
