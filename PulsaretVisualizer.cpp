/*
  ==============================================================================

    PulsaretVisualizer.cpp
    Created: 6 Nov 2020 4:34:21pm
    Author:  Billie (Govnah) Jean

  ==============================================================================
*/

#include "PulsaretVisualizer.h"

//==============================================================================
PulsaretVisualizer::PulsaretVisualizer(PulsaretTable& t) : pulsaretTable(t)
{
    setSize(400, 150);
    
}

PulsaretVisualizer::~PulsaretVisualizer()
{
}

void PulsaretVisualizer::paint (juce::Graphics& g)
{
    auto w = getLocalBounds().getWidth();
    auto h = getLocalBounds().getHeight();
    auto frameHalf = h / 2;
    float waveIncrement = (float) w / (float) pulsaretTable.getTable().getNumSamples();
    
    juce::Rectangle<int> waveFrame = {0,0, w, h};

    g.fillRoundedRectangle(waveFrame.toFloat(), 5.f);
    
    auto hue = 1.f - amp;
    g.setColour(waveColour.withHue(hue).brighter(amp));

    juce::Path wavePath;
    wavePath.startNewSubPath(0, frameHalf);

    auto* buffRead1 = pulsaretTable.getTable().getReadPointer(0);
    auto* buffRead2 = pulsaretTable.getNextTable().getReadPointer(0);

    for(int i = 0; i <= pulsaretTable.getTable().getNumSamples(); ++i)
    {
        
        auto x = i * waveIncrement;
        auto value1 = buffRead1[i] * (1.f - pulsaretTable.getWaveFraction());
        auto value2 = buffRead2[i] * (pulsaretTable.getWaveFraction());
        waveValue = value1 + value2;
        auto y = frameHalf - (waveValue * frameHalf * 0.9f); // 0.9 meant to keep the wave from ever touching edge of frame
        

        wavePath.lineTo(x, y);
    }
    wavePath.lineTo(w, frameHalf);

   
    //float lineThickness = juce::NormalisableRange<float>(0.f, 1.f, 1.f, 7.f).convertFrom0to1(ampGlow);
    PathStrokeType stroke(3.f, juce::PathStrokeType::curved);
    g.strokePath(wavePath, stroke);
    
    if (isDrawing)
    {
        wavePath.quadraticTo(lastPosition, mousePosition);
        lastPosition.setXY(mousePosition.getX(), mousePosition.getY());
    }
    juce::PathStrokeType stroke2(1.75f, juce::PathStrokeType::curved);
    g.strokePath(wavePath, stroke2);
    

}

void PulsaretVisualizer::resized()
{
    
}

void PulsaretVisualizer::setValue()
{
    //valueLabel->setText(String(pulsaretTable.getWaveIndex()), dontSendNotification);
}


