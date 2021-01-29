/*
  ==============================================================================

    PulsarTrain.cpp
    Created: 5 Nov 2020 11:24:59am
    Author:  Billie (Govnah) Jean

  ==============================================================================
*/

#include "PulsarTrain.h"

PulsarTrain::PulsarTrain(juce::OwnedArray<OwnedPulsaret>& array, juce::OwnedArray<OwnedPulsaret>& array2,
                         Atomic<float>& fund, juce::Range<int>& fuRange, Atomic<float>& fundRand, Atomic<bool>& fuIsSpread,
                         Atomic<float>& form, juce::Range<int>& fRange, Atomic<float>& formRand, Atomic<bool>& fIsSpread,
                         Atomic<float>& form2, juce::Range<int>& fRange2, Atomic<float>& formRand2, Atomic<bool>& fIsSpread2,
                         Atomic<float>& pL, Atomic<float>& pR, juce::Range<int>& pRange, Atomic<float>& pRand, Atomic<bool>& pIsSpread,
                         Atomic<float>& amp, juce::Range<int>& aRange, Atomic<float>& ampRand, Atomic<bool>& aIsSpread,
                         Atomic<float>& inter, Atomic<float>& wType, juce::Range<int>& wRange, Atomic<float>& wRand, Atomic<bool>& wIsSpread) : pulsaretArray(array), pulsaretArray2(array2),
                         mFundFreq(fund), fundRange(fuRange), mFundRand(fundRand), fundIsSpread(fuIsSpread),
                         mFormFreq(form), formRange(fRange), mFormRand(formRand), formIsSpread(fIsSpread),
                         mFormFreq2(form2), formRange2(fRange2), mFormRand2(formRand2), formIsSpread2(fIsSpread2),
panL(pL), panR(pR), panRange(pRange), panRand(pRand), panIsSpread(pIsSpread), mAmp(amp), ampRange(aRange), mAmpRand(ampRand), ampIsSpread(aIsSpread), intermittance(inter), waveType(wType), waveRange(wRange), waveRand(wRand), waveIsSpread(wIsSpread)

{
    setTrigger(3, 2);
    
}

PulsarTrain::~PulsarTrain()
{
    env.setMode(pedal::CurvedEnvelope::Mode::ADSR);
    env.setAttackTime(1000.f);
    env.setDecayTime(1000.f);
    env.setSustainLevel(0.4f);
    env.setReleaseTime(2000.f);
    
}

void PulsarTrain::prepare(double sampleRate)
{
    mSampleRate = sampleRate;
    smoothFund.reset(mSampleRate, 0.5);
    smoothForm.reset(mSampleRate, 0.5);
    smoothForm2.reset(mSampleRate, 0.5);
}


void PulsarTrain::startTrain()
{
    pulsarPeriod = (1 / mFundFreq.get()) * mSampleRate;
    samplesRemainingInPeriod = pulsarPeriod;
}

void PulsarTrain::setFrequencies(float fundFreq, float form1, float form2)
{
    
    if(inHarmonicMode)
    {
        
        setHarmonicFormants(fundFreq, form1, form2);
        smoothFund.setTargetValue(fundFreq);
    }
    else
    {
        smoothFund.setTargetValue(fundFreq);
        smoothForm.setTargetValue(form1);
        smoothForm2.setTargetValue(form2);
    }
    //DBG(smoothFund.getNextValue());

}

void PulsarTrain::setHarmonicFormants(float fund, float form1, float form2)
{
    if(fund <= form1)
    {
        int ratio1 = jlimit(1.f, 12.f, form1 / fund);
        smoothForm.setTargetValue(fund * ratio1);
    }
    else
    {
        int ratio1 = jlimit(1.f, 12.f, fund / form1);
        smoothForm.setTargetValue(fund / ratio1);
    }
    
    if(fund <= form2)
    {
        int ratio2 = jlimit(1.f, 12.f, form2 / fund);
        smoothForm2.setTargetValue(fund * ratio2);
    }
    else
    {
        int ratio2 = jlimit(1.f, 12.f, fund / form2);
        smoothForm2.setTargetValue(fund / ratio2);
    }
}

int PulsarTrain::getPeriod()
{
    //DBG(mFundFreq.get());
//    smoothFund.setTargetValue(mFundFreq.get());
    auto freq = mFundFreq.get();
    
    if(fundIsSpread.get())
    {
        juce::Random randFund;
        if(randFund.nextFloat() <= mFundRand.get())
        {
            freq = rand.nextInt(fundRange);
        }
    }
    
    pulsarPeriod = (1 / freq) * mSampleRate;
    return pulsarPeriod;
}

/*
    This only sets 1 pulsaret running in each array, we iterate to find the first one that isn't already running
*/
void PulsarTrain::triggerPulsaret()
{
    
    
    if(waveIsSpread.get())
    {
        juce::Random randWave;
        if(randWave.nextFloat() < waveRand.get())
        {
            waveType = rand.nextInt(waveRange);
            pulsaretArray[0]->pulsaretTable.setTable(waveType.get() / 100.f);
        }
    }
    
    for (int i = 0; i < pulsaretArray.size(); ++i)
    {
        if (pulsaretArray[i]->isFree) // find first free pulsaret in array
        {
            int freq;

            if(formIsSpread.get())
            {
                juce::Random rand1;
                if(rand1.nextFloat() < mFormRand.get()) // As the value approaches 1.0, we approach a 100% chance we get a random
                {
                    
                    freq = rand1.nextInt(formRange);
                }
                else
                {
                    freq = mFormFreq.get();
                }
                
            }
            else
            {
                freq = mFormFreq.get();
            }

            pulsaretArray[i]->setCycles(pulsaretCycles1);
            pulsaretArray[i]->setFrequency(freq); // get pulsaret ready to read
            //pulsaretArray[i]->triggerEnv();
            pulsaretArray[i]->setRunning();
//
            break;
        }
    }
    
    for (int i = 0; i < pulsaretArray2.size(); ++i)
    {
        if (pulsaretArray2[i]->isFree) // find first free pulsaret in array
        {
            int freq;
            
            
            if(formIsSpread2.get())
            {
                juce::Random rand2;
                if(rand2.nextFloat() < mFormRand2.get()) // As the value approaches 1.0, we approach a 100% chance we get a random
                {
                    freq = rand2.nextInt(formRange2);
                }
                else
                {
                    freq = mFormFreq2.get();
                }
            }
            else
            {
                freq = mFormFreq2.get();
            }

            pulsaretArray2[i]->setCycles(pulsaretCycles2);
            pulsaretArray2[i]->setFrequency(freq); // get pulsaret ready to read
            //pulsaretArray2[i]->triggerEnv();
            pulsaretArray2[i]->setRunning();
            //
            break;
        }
    }
}
void PulsarTrain::updateFundamental()
{
    pulsarPeriod = (1 / mFundFreq.get()) * mSampleRate;
    samplesRemainingInPeriod = pulsarPeriod;
    
    smoothFund.setTargetValue(mFundFreq.get());
}

void PulsarTrain::setPulsaretWidth(float width)
{
    pulsaretWidth = width;
    
    float period = (1 / mFormFreq.get()) * mSampleRate;
    float dutyCycle = getPeriod() * width;
    pulsaretCycles1 = (int)jmax<float>(dutyCycle / period, 1.f);

    float period2 = (1 / mFormFreq2.get()) * mSampleRate;
    float dutyCycle2 = getPeriod() * width;
    pulsaretCycles2 = (int)jmax<float>(dutyCycle2 / period2, 1.f);

}



void PulsarTrain::generateNextBlock(juce::AudioBuffer<float>& buffer)
{
    auto buffWrite = buffer.getArrayOfWritePointers();
    
    
    for (int i = 0; i < buffer.getNumSamples(); ++i)
    {
        mFundFreq = smoothFund.getNextValue();
        mFormFreq = smoothForm.getNextValue();
        mFormFreq2 = smoothForm2.getNextValue();
        
        if (samplesRemainingInPeriod == 0) // we've gone through a whole pulsar period
        {
            setPulsaretParamsAndTrigger();
            samplesRemainingInPeriod = getPeriod();
        }
        
        
        float sample = 0.f;
        
        for (int pulsaretIndex = 0; pulsaretIndex < pulsaretArray.size(); ++pulsaretIndex)
        {
            if (pulsaretArray[pulsaretIndex]->isFree)
                break;
            sample += pulsaretArray[pulsaretIndex]->getNextSample() + pulsaretArray2[pulsaretIndex]->getNextSample();
        }
        for (int pulsaretIndex = 0; pulsaretIndex < pulsaretArray2.size(); ++pulsaretIndex)
        {
            if (pulsaretArray2[pulsaretIndex]->isFree)
                break;
            sample += pulsaretArray2[pulsaretIndex]->getNextSample();
        }
        
        /*
            Doing this because rand object can only generate within a range if it is in integer
            Putting all my ' / 100 ' normalization RIGHT HERE AND NOWHERE ELSE!!!
        */
        auto pLeft = panL.get() / 100.f;
        auto pRight = panR.get() / 100.f;
        auto amp = mAmp.get() / 100.f;
        
                //DBG(amp);
       // DBG(env.generateSample());
        buffWrite[0][i] = sample * amp * pLeft * env.generateSample();
        buffWrite[1][i] = sample * amp * pRight * env.generateSample();
        
        
        samplesRemainingInPeriod--;
        
        if(isFlashing.get())
            flashState = (float)samplesRemainingInPeriod / (float)getPeriod();
        else
            flashState = 0.f;
    }
}

/*=====================================================================================*/
/*========================== setPulsaretParamsAndTrigger() ============================*/
/*-------------------------------------------------------------------------------------*/

void PulsarTrain::setPulsaretParamsAndTrigger()
{
    if(isTriggerPattern) // we are in a triggerPattern
    {
        if(onCount > 0)
        {
            juce::Random rand0;
            if(rand0.nextFloat() <= intermittance.get()) // trigger if not a "miss" and a part of the triggerOn queue
            {
                if(ampIsSpread.get())
                {
                    juce::Random rand1;
                    if(rand1.nextFloat() <= mAmpRand.get())
                    {
                        float amp = rand.nextInt(ampRange); // amp in range of 0 - 100
                        mAmp = amp;
                    }
                }
                
                if(panIsSpread.get())
                {
                    juce::Random rand2;
                    if(rand2.nextFloat() <= panRand.get())
                    {
                        panR = rand.nextInt(panRange); // pan in range of 0 - 100
                        panL = 100.f - panR.get();
                    }
                }
                
                onCount--;
                triggerPulsaret();
                isFlashing = true;
                //flipFlashState(); // every other
            }
            
        }
        else if(offCount > 0)
        {
            offCount--;
            isFlashing = false;
            if(onCount == 0 && offCount == 0)
            {
                onCount = triggerOn;
                offCount = triggerOff;
            }
        }
    }
    else // we are NOT in a triggerPattern but still might need randomized values
    {
        juce::Random rand0;
        if(rand0.nextFloat() <= intermittance.get()) // trigger if not a "miss" and a part of the triggerOn queue
        {
            if(ampIsSpread.get())
            {
                juce::Random rand1;
                if(rand1.nextFloat() <= mAmpRand.get())
                {
                    float amp = rand.nextInt(ampRange); // amp in range of 0 - 100
                    mAmp = amp;
                }
            }
            
            if(panIsSpread.get())
            {
                juce::Random rand2;
                if(rand2.nextFloat() <= panRand.get())
                {
                    panR = rand.nextInt(panRange); // pan in range of 0 - 100
                    panL = 100.f - panR.get();
                }
            }
            
            isFlashing = true;
            triggerPulsaret();
            flipFlashState(); // every other
        }
        else
        {
            isFlashing = false;
        }
    }
}


void PulsarTrain::checkStatus()
{
    for (int i = 0; i < pulsaretArray.size(); ++i)
    {
        if (!pulsaretArray[i]->isFree) // 
        {
            needsCalculation = true;
        }
    }
}

float PulsarTrain::getFlashState()
{
    return flashState.get();
}

void PulsarTrain::flipFlashState()
{
    if(flashState.get())
    {
        flashState = false;
    }
    else
    {
        flashState = true;
    }
}

void PulsarTrain::setTrigger(int on, int off)
{
    
    if(off == 0)
    {
        isTriggerPattern = false;
    }
    else
    {
        isTriggerPattern = true;
        
        triggerOn = on;
        triggerOff = off;
        onCount = on;
        offCount = off;
    }
}

//void PulsarTrain::setPulsaretParamsAndTrigger();

void PulsarTrain::setGlideTime(float glideTime)
{
    auto seconds = glideTime / 1000;
}

bool PulsarTrain::getHarmonicModeStatus()
{
    return inHarmonicMode;
}

float PulsarTrain::getFormFreq1()
{
    return smoothForm.getTargetValue();
}
float PulsarTrain::getFormFreq2()
{
    return smoothForm2.getTargetValue();
}


void PulsarTrain::triggerEnv()
{
    env.setTrigger(true);
}

void PulsarTrain::triggerRelease()
{
    env.setTrigger(false);
}

void PulsarTrain::setEnv(float atk, float dec, float sus, float rel)
{
    env.setAttackTime(atk);
    env.setDecayTime(dec);
    env.setSustainLevel(sus);
    env.setReleaseTime(rel);
}


