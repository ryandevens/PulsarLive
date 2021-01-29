/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
PulsarAudioProcessor::PulsarAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
pulsarTrain(pulsaretArray, pulsaretArray2, mFundFreq, fundRange, mFundRand, fundIsSpread, mFormFreq, formRange, mFormRand, formIsSpread, mFormFreq2, formRange2, mFormRand2, formIsSpread2, panL, panR, panRange, panRand, panIsSpread, mAmp, ampRange, mAmpRand, ampIsSpread, intermittance, waveType, waveRange, waveRand, waveIsSpread),
e(*this, nullptr, "Parameters", createParams())
#endif
{
    e.state.addListener (this);
    
    for (int i = 0; i < 12; ++i)
    {
        pulsaretArray.add(new OwnedPulsaret(pulsaretArray, pulsaretTable)); // up to 8 pulsarets available to overlap with each other
        pulsaretArray2.add(new OwnedPulsaret(pulsaretArray2, pulsaretTable));
    }
    
    
}

PulsarAudioProcessor::~PulsarAudioProcessor()
{
}



//==============================================================================
const juce::String PulsarAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool PulsarAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool PulsarAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool PulsarAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double PulsarAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int PulsarAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int PulsarAudioProcessor::getCurrentProgram()
{
    return 0;
}

void PulsarAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String PulsarAudioProcessor::getProgramName (int index)
{
    return {};
}

void PulsarAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void PulsarAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    for (int i = 0; i < pulsaretArray.size(); ++i)
    {
        pulsaretArray[i]->prepare(sampleRate);
        pulsaretArray2[i]->prepare(sampleRate);
    }
    pulsarTrain.prepare(sampleRate);
    
    fundRange.setStart(1);
    fundRange.setEnd(1000);
    formRange.setStart(100);
    formRange.setEnd(16000);
    formRange2.setStart(100);
    formRange2.setEnd(16000);
    panRange.setStart(0);
    panRange.setStart(100);
    waveRange.setStart(0);
    waveRange.setEnd(100);
    ampRange.setStart(0);
    ampRange.setEnd(100);
    
    update();

    pulsarSynth.prepare(sampleRate, samplesPerBlock);
    messageCollector.reset(sampleRate);
}

void PulsarAudioProcessor::releaseResources()
{
    
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool PulsarAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void PulsarAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    if (mustUpdateProcessing)
    {
        update();

    }

    pulsarTrain.generateNextBlock(buffer);
    
    messageCollector.removeNextBlockOfMessages (midiMessages, buffer.getNumSamples());
    pulsarSynth.renderNextBlock (buffer, midiMessages, 0, buffer.getNumSamples());

}

juce::AudioBuffer<float>& PulsarAudioProcessor::getEnvBuffer()
{
    return pulsaretArray[0]->getEnv();
}
//==============================================================================
bool PulsarAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* PulsarAudioProcessor::createEditor()
{
    return new PulsarAudioProcessorEditor (*this);
}


//==============================================================================
void PulsarAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    juce::ValueTree copyState = e.copyState();
    std::unique_ptr<juce::XmlElement> xml = copyState.createXml();
    copyXmlToBinary (*xml.get(), destData);
}

void PulsarAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xml = getXmlFromBinary (data, sizeInBytes);
    juce::ValueTree copyState = juce::ValueTree::fromXml (*xml.get());
    e.replaceState (copyState);
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new PulsarAudioProcessor();
}

void PulsarAudioProcessor::setTrainRunning()
{

    pulsarTrain.triggerEnv();
    trainIsRunning = true;
}

void PulsarAudioProcessor::setTrainStopped()
{
    pulsarTrain.triggerRelease();
    trainIsRunning = false;
}

void PulsarAudioProcessor::update()
{
    mustUpdateProcessing = false;
    updateFund();
    updateForm();
    updateForm2();
    updateWave();
    updatePan();
    updateAmp();
}



void PulsarAudioProcessor::updateFund()
{
    mFundFreq = *e.getRawParameterValue("Fundamental Freq");
    mFundSpread = *e.getRawParameterValue("Fundamental Spread");
    mFundRand = *e.getRawParameterValue("Fundamental Random");
    
    width = *e.getRawParameterValue("Width");

    auto atk = e.getRawParameterValue("Attack");
    auto dec = e.getRawParameterValue("Decay");
    auto sus = e.getRawParameterValue("Sustain Level");
    auto rel = e.getRawParameterValue("Release");
    
    pulsarTrain.setEnv(*atk, *dec, *sus, *rel);
    
    if(mFundSpread.get() != 1)
    {
        fundIsSpread = true;
        auto start = jlimit(1.f, 999.f, mFundFreq.get() / mFundSpread.get());
        auto end = jlimit(2.f, 1000.f, mFundFreq.get() * mFundSpread.get());
        
        fundRange.setStart(start);
        fundRange.setEnd(end);
    }
    else
    {
        fundIsSpread = false;
    }
    
    pulsarTrain.setFrequencies(mFundFreq.get(), mFormFreq2.get(), mFormFreq.get());
    pulsarTrain.setPulsaretWidth(width.get());
    pulsarTrain.updateFundamental(); // this really exists just to change the period length
}

void PulsarAudioProcessor::updateForm()
{
    mFormFreq = *e.getRawParameterValue("Formant Freq");
    mFormSpread = *e.getRawParameterValue("Formant Spread");
    mFormRand = *e.getRawParameterValue("Formant Random");

    
    if (mFormSpread.get() != 1)
    {
        formIsSpread = true;
        auto start = jlimit(100.f, 15999.f, mFormFreq.get() / mFormSpread.get());
        auto end = jlimit(101.f, 16000.f, mFormFreq.get() * mFormSpread.get());

        formRange.setStart(start);
        formRange.setEnd(end);
    }
    else
    {
        formIsSpread = false;
    }

    pulsarTrain.setFrequencies(mFundFreq.get(), mFormFreq2.get(), mFormFreq.get());
}


void PulsarAudioProcessor::updateForm2()
{
    mFormFreq2 = *e.getRawParameterValue("Formant Freq2");
    mFormSpread2 = *e.getRawParameterValue("Formant Spread2");
    mFormRand2 = *e.getRawParameterValue("Formant Random2");
    
    if (mFormSpread2.get() != 1)
    {
        formIsSpread2 = true;
        auto start = jlimit(100.f, 15999.f, mFormFreq2.get() / mFormSpread2.get() );
        auto end = jlimit(101.f, 16000.f, mFormFreq2.get() * mFormSpread2.get() );
        formRange2.setStart(start);
        formRange2.setEnd(end);
    }
    else
    {
        formIsSpread2 = false;
    }
    

    pulsarTrain.setFrequencies(mFundFreq.get(), mFormFreq2.get(), mFormFreq.get());
    //pulsarTrain.updateFormant();
}


void PulsarAudioProcessor::updatePan()
{
    panR = *e.getRawParameterValue("Pan");
    panL = 100.f - panR.get();
    panSpread = *e.getRawParameterValue("Pan Spread");
    panRand = *e.getRawParameterValue("Pan Random");
    
    if(panSpread.get() != 0)
    {
        panIsSpread = true;
        auto start = jlimit(0.f, 99.f, panR.get() - panSpread.get()/2);
        auto end = jlimit(1.f, 100.f, panR.get() + panSpread.get()/2);
        
        panRange.setStart(start);
        panRange.setEnd(end);
    }
    else
    {
        panIsSpread = false;
    }

}



void PulsarAudioProcessor::updateWave()
{
    waveType = *e.getRawParameterValue("Wave Type");
    waveSpread = *e.getRawParameterValue("Wave Spread");
    waveRand = *e.getRawParameterValue("Wave Random");
    
    if(waveSpread.get() != 0)
    {
        waveIsSpread = true;
        auto start = jlimit(0.f, 99.f, waveType.get() - waveSpread.get()/2);
        auto end = jlimit(1.f, 100.f, waveType.get() + waveSpread.get()/2);
        waveRange.setStart(start);
        waveRange.setEnd(end);
        
    }
    else
    {
        waveIsSpread = false;
    }
    
    pulsaretTable.setTable(waveType.get() / 100.f);
}

void PulsarAudioProcessor::updateAmp()
{
    mAmp = *e.getRawParameterValue("Amp");
    mAmpSpread = *e.getRawParameterValue("Amp Spread");
    mAmpRand = *e.getRawParameterValue("Amp Random");
    intermittance = *e.getRawParameterValue("Intermittance");
    trainIsRunning = *e.getRawParameterValue("Trigger");
    if(trainIsRunning.get())
    {
        setTrainRunning();
    }
    else if (!trainIsRunning.get())
    {
        setTrainStopped();
    }
    
    auto glideTime = e.getRawParameterValue("Glide Time");
    pulsarTrain.setGlideTime(*glideTime);
    auto on = e.getRawParameterValue("Trigger On");
    auto off = e.getRawParameterValue("Trigger Off");

    pulsarTrain.setTrigger(*on, *off);
    if (mAmpSpread.get() != 0)
    {
        ampIsSpread = true;
        auto start = jlimit(0.f, 99.f, mAmp.get() - mAmpSpread.get()/2);
        auto end = jlimit(1.f, 100.f, mAmp.get() + mAmpSpread.get()/2);
        ampRange.setStart(start);
        ampRange.setEnd(end);
    }
    else
    {
        ampIsSpread = false;
    }
    
}
        
/*=============================================================================================*/
/*=============================================================================================*/
void PulsarAudioProcessor::valueTreePropertyChanged(ValueTree& tree, const Identifier& property)
{
    mustUpdateProcessing = true;
}

PulsaretTable& PulsarAudioProcessor::getPulsaretTable()
{
    return pulsaretTable;
}

/*=============================================================================================*/
/*===================================== FUNDAMENTAL ===========================================*/
juce::AudioProcessorValueTreeState::ParameterLayout PulsarAudioProcessor::createParams()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> parameters;
    
    parameters.push_back (std::make_unique<AudioParameterFloat>("Fundamental Freq", "Fundamental Freq",
                                                                NormalisableRange<float> (1.f, 1000.f, 0.1f, 0.5f), 200.f));
    
    parameters.push_back (std::make_unique<AudioParameterFloat>("Fundamental Spread", "Fundamental Spread",
                                                                NormalisableRange<float> (1.f, 16.f, 0.01f, 0.5f), 1.f));
    
    parameters.push_back (std::make_unique<AudioParameterFloat>("Fundamental Random", "Fundamental Random",
                                                                NormalisableRange<float> (0.f, 1.f, 0.01f, 1.f), 0.f));
    
    
    
    parameters.push_back (std::make_unique<AudioParameterFloat>("Formant Freq", "Formant Freq",
                                                                NormalisableRange<float> (100.f, 16000.f, 0.1f, 0.25f), 600.f));
    
    parameters.push_back (std::make_unique<AudioParameterFloat>("Formant Spread", "Formant Spread",
                                                                NormalisableRange<float> (1.f, 16.f, 0.01f, 0.5f), 1.f));
    
    parameters.push_back (std::make_unique<AudioParameterFloat>("Formant Random", "Formant Random",
                                                                NormalisableRange<float> (0.f, 1.f, 0.001f, 1.f), 0.f));
    
    
    
    parameters.push_back (std::make_unique<AudioParameterFloat>("Formant Freq2", "Formant Freq2",
                                                                NormalisableRange<float> (100.f, 16000.f, 0.1f, 0.25f), 600.f));
    
    parameters.push_back (std::make_unique<AudioParameterFloat>("Formant Spread2", "Formant Spread2",
                                                                NormalisableRange<float> (1.f, 16.f, 0.01f, 0.5f), 1.f));
    
    parameters.push_back (std::make_unique<AudioParameterFloat>("Formant Random2", "Formant Random2",
                                                                NormalisableRange<float> (0.f, 1.f, 0.001f, 1.f), 0.f));
    
    
    
    parameters.push_back (std::make_unique<AudioParameterFloat>("Pan", "Pan",
                                                                NormalisableRange<float> (0.0f, 100.f, 0.1f, 1.f), 50.f));
    
    parameters.push_back (std::make_unique<AudioParameterFloat>("Pan Spread", "Pan Spread",
                                                                NormalisableRange<float> (0.f, 100.f, 0.01f, 1.f), 1.f));
    
    parameters.push_back (std::make_unique<AudioParameterFloat>("Pan Random", "Pan Random",
                                                                NormalisableRange<float> (0.f, 1.f, 0.01f, 1.f), 0.f));
    
    
    
    parameters.push_back (std::make_unique<AudioParameterFloat>("Wave Type", "Wave Type",
                                                                NormalisableRange<float> (0.f, 100.f, 0.001f, 1.f), 50.f));
    
    parameters.push_back (std::make_unique<AudioParameterFloat>("Wave Spread", "Wave Spread",
                                                                NormalisableRange<float> (0.f, 100.f, 0.1f, 1.f), 0.f));
    
    parameters.push_back (std::make_unique<AudioParameterFloat>("Wave Random", "Wave Random",
                                                                NormalisableRange<float> (0.f, 1.f, 0.001f, 1.f), 0.f));
    
    
    
    parameters.push_back (std::make_unique<AudioParameterFloat>("Amp", "Amp",
                                                                NormalisableRange<float> (0.0f, 100.f, 0.1f, 1.f), 50.f));
    
    parameters.push_back (std::make_unique<AudioParameterFloat>("Amp Spread", "Amp Spread",
                                                                NormalisableRange<float> (0.f, 100.f, 0.1f, 1.f), 0.f));
    
    parameters.push_back (std::make_unique<AudioParameterFloat>("Amp Random", "Amp Random",
                                                                NormalisableRange<float> (0.f, 1.f, 0.001f, 1.f), 0.00f));
    
    parameters.push_back (std::make_unique<AudioParameterFloat>("Intermittance", "Intermittance",
                                                                NormalisableRange<float> (0.f, 1.f, 0.001f, 1.f), 1.f));
    
    parameters.push_back (std::make_unique<AudioParameterInt>("Trigger On", "Trigger On", 1, 10, 1));
    
    parameters.push_back (std::make_unique<AudioParameterInt>("Trigger Off", "Trigger Off", 0, 10, 1));
    
    parameters.push_back (std::make_unique<AudioParameterBool>("Trigger", "Trigger", false));
    
    parameters.push_back (std::make_unique<AudioParameterInt>("Glide Time", "Glide Time", 0, 10000, 6000));
    
    
    parameters.push_back (std::make_unique<AudioParameterFloat>("Width", "Width",
                                                                NormalisableRange<float> (0.f, 1.f, 0.001f, 1.f), 0.01f));
    
    
    parameters.push_back (std::make_unique<AudioParameterFloat>("Attack", "Attack",
                                                                NormalisableRange<float> (0.f, 10000.f, 0.001f, 0.5f), 100.f));
    parameters.push_back (std::make_unique<AudioParameterFloat>("Decay", "Decay",
                                                                NormalisableRange<float> (0.f, 5000.f, 0.001f, 0.5f), 50.f));
    parameters.push_back (std::make_unique<AudioParameterFloat>("Sustain Level", "Sustain Level",
                                                                NormalisableRange<float> (0.f, 1.f, 0.001f, 1.f), 0.01f));
    parameters.push_back (std::make_unique<AudioParameterFloat>("Release", "Release",
                                                                NormalisableRange<float> (0.f, 10000.f, 0.001f, 0.5f), 200.f));
    
    
    return { parameters.begin(), parameters.end() };
}
