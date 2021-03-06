/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
PulsarAudioProcessorEditor::PulsarAudioProcessorEditor (PulsarAudioProcessor& p)
: AudioProcessorEditor (&p), audioProcessor (p), pulsaretVisualizer(p.getPulsaretTable())
{

    setSize(530, 415);
    startTimerHz(60);
    
    frame = juce::Rectangle<int>(15, 50, getWidth() - 30, getHeight() - 65);
    
    pulsaretVisualizer.setBounds(frame);
    addAndMakeVisible(pulsaretVisualizer);

    nameLabel = std::make_unique<Label>("","P U L S A R");
    addAndMakeVisible(nameLabel.get());
    nameLabel->setBounds(175, 15, 200, 25);
    nameLabel->setFont(Font ("Consolas", "Regular", 20.f));
    nameLabel->setLookAndFeel(&nameFeel);
    nameLabel->setColour(juce::Label::textColourId, juce::Colours::white);
    
    
    setLookAndFeel(&mixFeel);
    
    triggerButton = std::make_unique<TextButton>();
    triggerButton->setBounds(frame);
    triggerButton->addListener(this);
    addAndMakeVisible(triggerButton.get());
    
    
    
    attackSlider = std::make_unique<Slider>(Slider::SliderStyle::LinearBarVertical, Slider::TextBoxBelow);
    attackSlider->setBounds(195, getHeight()-77, 30, 55);
    attackSlider->addListener(this);
    attackSlider->setLookAndFeel(&mixFeel);
    addAndMakeVisible (attackSlider.get());
    
    decaySlider = std::make_unique<Slider>(Slider::SliderStyle::LinearBarVertical, Slider::TextBoxBelow);
    decaySlider->setBounds(235, getHeight()-77, 30, 55);
    decaySlider->addListener(this);
    decaySlider->setLookAndFeel(&mixFeel);
    addAndMakeVisible (decaySlider.get());
    
    sustainSlider = std::make_unique<Slider>(Slider::SliderStyle::LinearBarVertical, Slider::TextBoxBelow);
    sustainSlider->setBounds(275, getHeight()-77, 30, 55);
    sustainSlider->addListener(this);
    sustainSlider->setLookAndFeel(&mixFeel);
    addAndMakeVisible (sustainSlider.get());
    
    releaseSlider = std::make_unique<Slider>(Slider::SliderStyle::LinearBarVertical, Slider::TextBoxBelow);
    releaseSlider->setBounds(315, getHeight()-77, 30, 55);
    releaseSlider->addListener(this);
    releaseSlider->setLookAndFeel(&mixFeel);
    addAndMakeVisible (releaseSlider.get());
    
    glideSlider = std::make_unique<Slider>(Slider::SliderStyle::LinearBarVertical, Slider::TextBoxBelow);
    glideSlider->setBounds(455, getHeight()-77, 40, 55);
    glideSlider->addListener(this);
    glideSlider->setLookAndFeel(&mixFeel);
    addAndMakeVisible (glideSlider.get());
    
    widthSlider = std::make_unique<Slider>(Slider::SliderStyle::LinearBarVertical, Slider::TextBoxBelow);
    widthSlider->setBounds(35, getHeight()-77, 40, 55);
    widthSlider->addListener(this);
    widthSlider->setLookAndFeel(&mixFeel);
    addAndMakeVisible (widthSlider.get());
    /*==================================================================================================*/
    /*========================================== FUNDAMENTAL ===========================================*/
    fundMultiSlider = std::make_unique<Slider>(Slider::SliderStyle::ThreeValueVertical, Slider::NoTextBox);
    fundMultiSlider->setBounds(35, 80, 40, 160);
    fundMultiSlider->addListener(this);
    fundMultiSlider->setLookAndFeel(&mixFeel);
    addAndMakeVisible (fundMultiSlider.get());
    
    fundSlider = std::make_unique<Slider>(Slider::SliderStyle::LinearBarVertical, Slider::TextBoxBelow);
    fundSlider->setBounds(35, 80, 40, 160);
    fundSlider->addListener(this);
    fundSlider->setLookAndFeel(&mixFeel);
    addAndMakeVisible (fundSlider.get());
    
    
    
    fundRandSlider = std::make_unique<Slider>(Slider::SliderStyle::LinearVertical, Slider::NoTextBox);
    fundRandSlider->setBounds(80, 80, 10, 160);
    fundRandSlider->addListener(this);
    fundRandSlider->setLookAndFeel(&mixFeel);
    addAndMakeVisible (fundRandSlider.get());
    
    fundSpreadSlider = std::make_unique<Slider>(Slider::SliderStyle::LinearHorizontal, Slider::NoTextBox);
    fundSpreadSlider->setBounds(35, 245, 40, 10);
    fundSpreadSlider->addListener(this);
    fundSpreadSlider->setLookAndFeel(&mixFeel);
    addAndMakeVisible (fundSpreadSlider.get());
    
    fundLabel = std::make_unique<Label>("", "Fund");
    addAndMakeVisible (fundLabel.get());
    fundLabel->attachToComponent (fundSlider.get(), false);
    fundLabel->setJustificationType (Justification::centred);
    fundLabel->setLookAndFeel(&mixFeel);

//
    /*==================================================================================================*/
    /*============================================= FORM 1 =============================================*/
    formMultiSlider = std::make_unique<Slider>(Slider::SliderStyle::ThreeValueVertical, Slider::NoTextBox);
    formMultiSlider->setBounds(115, 80, 40, 160);
    formMultiSlider->addListener(this);
    formMultiSlider->setLookAndFeel(&mixFeel);
    addAndMakeVisible (formMultiSlider.get());
    
    formFreqSlider = std::make_unique<Slider>(Slider::SliderStyle::LinearBarVertical, Slider::TextBoxBelow);
    formFreqSlider->setBounds(115, 80, 40, 160);
    formFreqSlider->addListener(this);
    formFreqSlider->setLookAndFeel(&mixFeel);
    addAndMakeVisible (formFreqSlider.get());
    
    formRandSlider = std::make_unique<Slider>(Slider::SliderStyle::LinearVertical, Slider::NoTextBox);
    formRandSlider->setBounds(160, 80, 10, 160);
    formRandSlider->addListener(this);
    formRandSlider->setLookAndFeel(&mixFeel);
    addAndMakeVisible (formRandSlider.get());
    
    formSpreadSlider = std::make_unique<Slider>(Slider::SliderStyle::LinearHorizontal, Slider::NoTextBox);
    formSpreadSlider->setBounds(115, 245, 40, 10);
    formSpreadSlider->addListener(this);
    formSpreadSlider->setLookAndFeel(&mixFeel);
    addAndMakeVisible (formSpreadSlider.get());
    
    formFreqLabel = std::make_unique<Label>("", "Form");
    addAndMakeVisible (formFreqLabel.get());
    formFreqLabel->attachToComponent (formFreqSlider.get(), false);
    formFreqLabel->setJustificationType (Justification::centred);
    formFreqLabel->setLookAndFeel(&mixFeel);
    
    /*==================================================================================================*/
    /*============================================= FORM 2 =============================================*/
    formMultiSlider2 = std::make_unique<Slider>(Slider::SliderStyle::ThreeValueVertical, Slider::NoTextBox);
    formMultiSlider2->setBounds(195, 80, 40, 160);
    formMultiSlider2->addListener(this);
    formMultiSlider2->setLookAndFeel(&mixFeel);
    addAndMakeVisible (formMultiSlider2.get());
    
    formFreqSlider2 = std::make_unique<Slider>(Slider::SliderStyle::LinearBarVertical, Slider::TextBoxBelow);
    formFreqSlider2->setBounds(195, 80, 40, 160);
    formFreqSlider2->addListener(this);
    formFreqSlider2->setLookAndFeel(&mixFeel);
    addAndMakeVisible (formFreqSlider2.get());
    
    formRandSlider2 = std::make_unique<Slider>(Slider::SliderStyle::LinearVertical, Slider::NoTextBox);
    formRandSlider2->setBounds(240, 80, 10, 160);
    formRandSlider2->addListener(this);
    formRandSlider2->setLookAndFeel(&mixFeel);
    addAndMakeVisible (formRandSlider2.get());
    
    formSpreadSlider2 = std::make_unique<Slider>(Slider::SliderStyle::LinearHorizontal, Slider::NoTextBox);
    formSpreadSlider2->setBounds(195, 245, 40, 10);
    formSpreadSlider2->addListener(this);
    formSpreadSlider2->setLookAndFeel(&mixFeel);
    addAndMakeVisible (formSpreadSlider2.get());
    
    formFreqLabel2 = std::make_unique<Label>("", "Form 2");
    addAndMakeVisible (formFreqLabel2.get());
    formFreqLabel2->attachToComponent (formFreqSlider2.get(), false);
    formFreqLabel2->setJustificationType (Justification::centred);
    formFreqLabel2->setLookAndFeel(&mixFeel);
    
    /*==================================================================================================*/
    /*============================================= WAVE ===============================================*/
    waveMultiSlider = std::make_unique<Slider>(Slider::SliderStyle::ThreeValueVertical, Slider::NoTextBox);
    waveMultiSlider->setBounds(295, 80, 40, 160);
    waveMultiSlider->addListener(this);
    waveMultiSlider->setLookAndFeel(&mixFeel);
    addAndMakeVisible (waveMultiSlider.get());
    
    waveSlider = std::make_unique<Slider>(Slider::SliderStyle::LinearBarVertical, Slider::TextBoxBelow);
    waveSlider->setBounds(295, 80, 40, 160);
    waveSlider->addListener(this);
    waveSlider->setLookAndFeel(&mixFeel);
    addAndMakeVisible (waveSlider.get());
    
    waveSpreadSlider = std::make_unique<Slider>(Slider::SliderStyle::LinearHorizontal, Slider::NoTextBox);
    waveSpreadSlider->setBounds(295, 245, 40, 10);
    waveSpreadSlider->addListener(this);
    waveSpreadSlider->setLookAndFeel(&mixFeel);
    addAndMakeVisible (waveSpreadSlider.get());
    
    waveRandSlider = std::make_unique<Slider>(Slider::SliderStyle::LinearVertical, Slider::NoTextBox);
    waveRandSlider->setBounds(340, 80, 10, 160);
    waveRandSlider->addListener(this);
    waveRandSlider->setLookAndFeel(&mixFeel);
    addAndMakeVisible (waveRandSlider.get());

    /*==================================================================================================*/
    /*============================================= INTER ==============================================*/
    interSlider = std::make_unique<Slider>(Slider::SliderStyle::LinearVertical, Slider::NoTextBox);
    interSlider->setBounds(496, 80, 10, 160);
    interSlider->addListener(this);
    interSlider->setLookAndFeel(&mixFeel);
    addAndMakeVisible (interSlider.get());
    
    triggerOnSlider = std::make_unique<Slider>(Slider::SliderStyle::LinearBarVertical, Slider::TextBoxBelow);
    triggerOnSlider->setBounds(447, 80, 22, 160);
    triggerOnSlider->addListener(this);
    triggerOnSlider->setLookAndFeel(&mixFeel);
    addAndMakeVisible (triggerOnSlider.get());
    
    triggerOffSlider = std::make_unique<Slider>(Slider::SliderStyle::LinearBarVertical, Slider::TextBoxBelow);
    triggerOffSlider->setBounds(470, 80, 22, 160);
    triggerOffSlider->addListener(this);
    triggerOffSlider->setLookAndFeel(&mixFeel);
    addAndMakeVisible (triggerOffSlider.get());

    
    /*==================================================================================================*/
    /*============================================= AMP ================================================*/
    ampMultiSlider = std::make_unique<Slider>(Slider::SliderStyle::ThreeValueVertical, Slider::NoTextBox);
    ampMultiSlider->setBounds(375, 80, 40, 160);
    ampMultiSlider->addListener(this);
    ampMultiSlider->setLookAndFeel(&mixFeel);
    addAndMakeVisible (ampMultiSlider.get());
    
    ampSlider = std::make_unique<Slider>(Slider::SliderStyle::LinearBarVertical, Slider::TextBoxBelow);
    ampSlider->setBounds(375, 80, 40, 160);
    ampSlider->addListener(this);
    ampSlider->setLookAndFeel(&mixFeel);
    addAndMakeVisible (ampSlider.get());
    
    ampSpreadSlider = std::make_unique<Slider>(Slider::SliderStyle::LinearHorizontal, Slider::NoTextBox);
    ampSpreadSlider->setBounds(375, 245, 40, 10);
    ampSpreadSlider->addListener(this);
    ampSpreadSlider->setLookAndFeel(&mixFeel);
    addAndMakeVisible (ampSpreadSlider.get());
    
    ampRandSlider = std::make_unique<Slider>(Slider::SliderStyle::LinearVertical, Slider::NoTextBox);
    ampRandSlider->setBounds(420, 80, 10, 160);
    ampRandSlider->addListener(this);
    ampRandSlider->setLookAndFeel(&mixFeel);
    addAndMakeVisible (ampRandSlider.get());
    
    /*==================================================================================================*/
    /*============================================= PAN ================================================*/
    panMultiSlider = std::make_unique<Slider>(Slider::SliderStyle::ThreeValueHorizontal, Slider::NoTextBox);
    panMultiSlider->setBounds(125, 275, 290, 20);
    panMultiSlider->addListener(this);
    panMultiSlider->setLookAndFeel(&mixFeel);
    addAndMakeVisible (panMultiSlider.get());
    
    panSlider = std::make_unique<Slider>(Slider::SliderStyle::LinearHorizontal, Slider::NoTextBox);
    panSlider->setBounds(125, 275, 290, 20);
    panSlider->addListener(this);
    panSlider->setLookAndFeel(&mixFeel);
    addAndMakeVisible (panSlider.get());
    
    panSpreadSlider = std::make_unique<Slider>(Slider::SliderStyle::LinearVertical, Slider::NoTextBox);
    panSpreadSlider->setBounds(110, 275, 10, 40);
    panSpreadSlider->addListener(this);
    panSpreadSlider->setLookAndFeel(&mixFeel);
    addAndMakeVisible (panSpreadSlider.get());

    panRandSlider = std::make_unique<Slider>(Slider::SliderStyle::LinearHorizontal, Slider::NoTextBox);
    panRandSlider->setBounds(125, 305, 290, 10);
    panRandSlider->addListener(this);
    panRandSlider->setLookAndFeel(&mixFeel);
    addAndMakeVisible (panRandSlider.get());


    waveLabel = std::make_unique<Label>("", "Wave");
    addAndMakeVisible (waveLabel.get());
    waveLabel->attachToComponent (waveSlider.get(), false);
    waveLabel->setJustificationType (Justification::centred);
    waveLabel->setLookAndFeel(&mixFeel);

    interLabel = std::make_unique<Label>("", "Inter");
    addAndMakeVisible (interLabel.get());
    interLabel->attachToComponent (interSlider.get(), false);
    interLabel->setJustificationType (Justification::centred);
    interLabel->setLookAndFeel(&mixFeel);
    
    ampLabel = std::make_unique<Label>("", "Amp");
    addAndMakeVisible (ampLabel.get());
    ampLabel->attachToComponent (ampSlider.get(), false);
    ampLabel->setJustificationType (Justification::centred);
    ampLabel->setLookAndFeel(&mixFeel);
   
    panLabel = std::make_unique<Label>("", "Pan");
    addAndMakeVisible (panLabel.get());
    panLabel->attachToComponent (panSlider.get(), false);
    panLabel->setJustificationType (Justification::centred);
    panLabel->setLookAndFeel(&mixFeel);
    
    widthLabel = std::make_unique<Label>("", "Width");
    addAndMakeVisible (widthLabel.get());
    widthLabel->attachToComponent (widthSlider.get(), false);
    widthLabel->setJustificationType (Justification::centred);
    widthLabel->setLookAndFeel(&mixFeel);
    
    glideLabel = std::make_unique<Label>("", "Glide");
    addAndMakeVisible (glideLabel.get());
    glideLabel->attachToComponent (glideSlider.get(), false);
    glideLabel->setJustificationType (Justification::centred);
    glideLabel->setLookAndFeel(&mixFeel);
    
    attackLabel = std::make_unique<Label>("", "A");
    addAndMakeVisible (attackLabel.get());
    attackLabel->attachToComponent(attackSlider.get(), false);
    attackLabel->setJustificationType (Justification::centred);
    attackLabel->setLookAndFeel(&mixFeel);

    decayLabel = std::make_unique<Label>("", "D");
    decayLabel->attachToComponent(decaySlider.get(), false);
    decayLabel->setJustificationType (Justification::centred);
    decayLabel->setLookAndFeel(&mixFeel);

    sustainLabel = std::make_unique<Label>("", "S");
    sustainLabel->attachToComponent(sustainSlider.get(), false);
    sustainLabel->setJustificationType (Justification::centred);
    sustainLabel->setLookAndFeel(&mixFeel);

    releaseLabel = std::make_unique<Label>("", "R");
    releaseLabel->attachToComponent(releaseSlider.get(), false);
    releaseLabel->setJustificationType (Justification::centred);
    releaseLabel->setLookAndFeel(&mixFeel);
    
    onLabel = std::make_unique<Label>("", "On");
    onLabel->attachToComponent(triggerOnSlider.get(), false);
    onLabel->setJustificationType (Justification::centred);
    onLabel->setLookAndFeel(&mixFeel);
    
    offLabel = std::make_unique<Label>("", "Off");
    offLabel->attachToComponent(triggerOffSlider.get(), false);
    offLabel->setJustificationType (Justification::centred);
    offLabel->setLookAndFeel(&mixFeel);
    
    
    

    
    triggerAttachment = std::make_unique<AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.e, "Trigger", *triggerButton);
    
    fundAttachment = std::make_unique<Attachment>(audioProcessor.e, "Fundamental Freq", *fundSlider);
    fundSpreadAttachment = std::make_unique<Attachment>(audioProcessor.e, "Fundamental Spread", *fundSpreadSlider);
    fundRandAttachment = std::make_unique<Attachment>(audioProcessor.e, "Fundamental Random", *fundRandSlider);
    
    formFreqAttachment = std::make_unique<Attachment>(audioProcessor.e, "Formant Freq", *formFreqSlider);
    formSpreadAttachment = std::make_unique<Attachment>(audioProcessor.e, "Formant Spread", *formSpreadSlider);
    formRandAttachment = std::make_unique<Attachment>(audioProcessor.e, "Formant Random", *formRandSlider);
    
    formFreqAttachment2 = std::make_unique<Attachment>(audioProcessor.e, "Formant Freq2", *formFreqSlider2);
    formSpreadAttachment2 = std::make_unique<Attachment>(audioProcessor.e, "Formant Spread2", *formSpreadSlider2);
    formRandAttachment2 = std::make_unique<Attachment>(audioProcessor.e, "Formant Random2", *formRandSlider2);
    
    waveAttachment = std::make_unique<Attachment>(audioProcessor.e, "Wave Type", *waveSlider);
    waveSpreadAttachment = std::make_unique<Attachment>(audioProcessor.e, "Wave Spread", *waveSpreadSlider);
    waveRandAttachment = std::make_unique<Attachment>(audioProcessor.e, "Wave Random", *waveRandSlider);
    
    panAttachment = std::make_unique<Attachment>(audioProcessor.e, "Pan", *panSlider);
    panSpreadAttachment = std::make_unique<Attachment>(audioProcessor.e, "Pan Spread", *panSpreadSlider);
    panRandAttachment = std::make_unique<Attachment>(audioProcessor.e, "Pan Random", *panRandSlider);
    
    ampAttachment = std::make_unique<Attachment>(audioProcessor.e, "Amp", *ampSlider);
    ampSpreadAttachment = std::make_unique<Attachment>(audioProcessor.e, "Amp Spread", *ampSpreadSlider);
    ampRandAttachment = std::make_unique<Attachment>(audioProcessor.e, "Amp Random", *ampRandSlider);
    interAttachment = std::make_unique<Attachment>(audioProcessor.e, "Intermittance", *interSlider);
    triggerOnAttachment = std::make_unique<Attachment>(audioProcessor.e, "Trigger On", *triggerOnSlider);
    triggerOffAttachment = std::make_unique<Attachment>(audioProcessor.e, "Trigger Off", *triggerOffSlider);
    
    widthAttachment = std::make_unique<Attachment>(audioProcessor.e, "Width", *widthSlider);
    
    attackAttachment = std::make_unique<Attachment>(audioProcessor.e, "Attack", *attackSlider);
    decayAttachment = std::make_unique<Attachment>(audioProcessor.e, "Decay", *decaySlider);
    sustainAttachment = std::make_unique<Attachment>(audioProcessor.e, "Sustain Level", *sustainSlider);
    releaseAttachment = std::make_unique<Attachment>(audioProcessor.e, "Release", *releaseSlider);
    
    glideAttachment = std::make_unique<Attachment>(audioProcessor.e, "Glide Time", *glideSlider);
    
    
    ampSpreadSlider->setValue(0);
    waveSpreadSlider->setValue(0);
    waveMultiSlider->setMinValue(50);
    waveMultiSlider->setMaxValue(50);
    waveMultiSlider->setValue(50);
    panSpreadSlider->setValue(0);
    ampSlider->setValue(50);
    waveSlider->setValue(50);
    
    

}

PulsarAudioProcessorEditor::~PulsarAudioProcessorEditor()
{
}

//==============================================================================
void PulsarAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.setColour(juce::Colours::black);
    g.fillAll();
    
    
    g.setColour(juce::Colours::yellow);
    //juce::Rectangle<int> frame(15, 50, getWidth() - 30, getHeight() - 65);  // 25px from sides, 30 from top, 5 below nameLabel, 5 above bottom
    g.drawRoundedRectangle(frame.toFloat(), 5.f, 2.f);
    
    g.setColour(juce::Colours::grey);
    g.drawRoundedRectangle(getLocalBounds().reduced(5).toFloat(), 5.f, 2.f);
    
   
    
    
    
}

void PulsarAudioProcessorEditor::resized()
{
    
}

void PulsarAudioProcessorEditor::buttonClicked(Button* button)
{

}

void PulsarAudioProcessorEditor::buttonStateChanged(Button* button)
{
    //wtVisualizer.setBuffer(audioProcessor.getEnvBuffer());
    if(button == triggerButton.get())
    {
        //triggerButton->repaint();
        if(button->isDown())
        {
            audioProcessor.triggerPulsarTrain();
            
        }
        else
        {
            audioProcessor.releasePulsarTrain();
        }
    }
}

void PulsarAudioProcessorEditor::sliderValueChanged(Slider* s)
{
   
    if(s == fundSpreadSlider.get() || s == fundSlider.get())
    {
        double spread = fundSpreadSlider->getValue();
        double freq = fundSlider->getValue();
        
        
        fundMultiSlider->setNormalisableRange(juce::NormalisableRange<double>(1, 1000, 1, 0.5));
        if (spread < 15)
        {
            fundMultiSlider->setMinValue(freq/spread);
            fundMultiSlider->setValue(freq);
            fundMultiSlider->setMaxValue(freq*spread);
        }
        else
        {
            fundMultiSlider->setMinValue(1);
            fundMultiSlider->setMaxValue(1000);
        }
        fundMultiSlider->repaint();
    }
    
    if(s == formSpreadSlider.get() || s == formFreqSlider.get())
    {
        double spread = formSpreadSlider->getValue();
        double freq = formFreqSlider->getValue();
        
        
        formMultiSlider->setNormalisableRange(juce::NormalisableRange<double>(100, 16000, 1, 0.25));
        if (spread < 15)
        {
            formMultiSlider->setMinValue(freq/spread);
            formMultiSlider->setValue(freq);
            formMultiSlider->setMaxValue(freq*spread);
        }
        else
        {
            formMultiSlider->setMinValue(100);
            formMultiSlider->setMaxValue(16000);
        }
        
        formMultiSlider->repaint();
    }
    
    if(s == formSpreadSlider2.get() || s == formFreqSlider2.get())
    {
        double spread = formSpreadSlider2->getValue();
        double freq = formFreqSlider2->getValue();
        
        formMultiSlider2->setNormalisableRange(juce::NormalisableRange<double>(100, 16000, 1, 0.25));
        if (spread < 15)
        {
            formMultiSlider2->setMinValue(freq/spread);
            formMultiSlider2->setValue(freq);
            formMultiSlider2->setMaxValue(freq*spread);
        }
        else
        {
            formMultiSlider2->setMinValue(100);
            formMultiSlider2->setMaxValue(16000);
        }
        
        formMultiSlider2->repaint();
    }
    
    if(s == waveSpreadSlider.get() || s == waveSlider.get())
    {
        double spread = waveSpreadSlider->getValue();
        double selection = waveSlider->getValue();
        
        waveMultiSlider->setNormalisableRange(juce::NormalisableRange<double>(0, 100, 0.1, 1));
        if (spread < 98 && spread > 2)
        {
            waveMultiSlider->setMinValue(selection - spread/2);
            waveMultiSlider->setValue(selection);
            waveMultiSlider->setMaxValue(selection + spread/2);
        }
        else if (spread <= 2)
        {
            waveMultiSlider->setMinValue(selection);
            waveMultiSlider->setValue(selection);
            waveMultiSlider->setMaxValue(selection);
        }
        else if (spread >= 98)
        {
            waveMultiSlider->setMinValue(0);
            waveMultiSlider->setMaxValue(100);
        }
        
        waveMultiSlider->repaint();
    }
    
    if(s == ampSpreadSlider.get() || s == ampSlider.get())
    {
        double spread = ampSpreadSlider->getValue();
        double selection = ampSlider->getValue();
        //float val = NormalisableRange<float>(0, 100, 0.1, 1).convertTo0to1(selection);

        
        ampMultiSlider->setNormalisableRange(juce::NormalisableRange<double>(0, 100, 0.1, 1));
        if (spread <= 98 && spread >= 2)
        {
            ampMultiSlider->setMinValue(selection - spread/2);
            ampMultiSlider->setValue(selection);
            ampMultiSlider->setMaxValue(selection + spread/2);
        }
        else if (spread <= 2)
        {
            ampMultiSlider->setMinValue(selection);
            ampMultiSlider->setValue(selection);
            ampMultiSlider->setMaxValue(selection);
        }
        else if (spread >= 98)
        {
            ampMultiSlider->setMinValue(0);
            ampMultiSlider->setMaxValue(100);
        }
        
        ampMultiSlider->repaint();
    }
    
    if(s == panSpreadSlider.get() || s == panSlider.get())
    {
        double spread = panSpreadSlider->getValue();
        double pan = panSlider->getValue();
        panMultiSlider->setNormalisableRange(juce::NormalisableRange<double>(0, 100, 0.1, 1));
        
        if(spread <= 98 && spread >= 2)
        {
            panMultiSlider->setMinValue(pan - spread/2);
            panMultiSlider->setValue(pan);
            panMultiSlider->setMaxValue(pan + spread/2);
        }
        else if (spread <= 2)
        {
            panMultiSlider->setMinValue(pan);
            panMultiSlider->setValue(pan);
            panMultiSlider->setMaxValue(pan);
        }
        else
        {
            panMultiSlider->setMinValue(0);
            panMultiSlider->setValue(pan);
            panMultiSlider->setMaxValue(100);
        }
        
        panMultiSlider->repaint();
    }

}

void PulsarAudioProcessorEditor::repaintPulsaret()
{
    if(audioProcessor.isTrainRunning())
    {
        DBG("test");
        if(audioProcessor.isFlashing())
        {
            pulsaretVisualizer.setNewWaveColour(juce::Colours::yellow);
           
            pulsaretVisualizer.setAmp(audioProcessor.getFlashCoef());
            pulsaretVisualizer.repaint();
        }
        else
        {
            pulsaretVisualizer.setNewWaveColour(juce::Colours::black);
            pulsaretVisualizer.setAmp(audioProcessor.getFlashCoef());
            pulsaretVisualizer.repaint();
        }
    }
    else
    {
        pulsaretVisualizer.setNewWaveColour(juce::Colours::black);
        pulsaretVisualizer.repaint();
    }
    
}

void PulsarAudioProcessorEditor::timerCallback()
{
    repaintPulsaret();

}

