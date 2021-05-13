/*
  ==============================================================================

    PulsarTrain.cpp
    Created: 5 Nov 2020 11:24:59am
    Author:  Billie (Govnah) Jean

  ==============================================================================
*/

#include "PulsarTrain.h"

PulsarTrain::PulsarTrain()
{
    setTrigger(3, 2);
}

PulsarTrain::~PulsarTrain()
{
    
    
}

void PulsarTrain::prepare(double sampleRate)
{
    pulsaret1.prepare(sampleRate);
    pulsaret2.prepare(sampleRate);
    mSampleRate = sampleRate;
    smoothFund.reset(mSampleRate, glideTime.get());
    smoothForm.reset(mSampleRate, glideTime.get());
    smoothForm2.reset(mSampleRate, glideTime.get());

    env.setSampleRate(sampleRate);
    
    fundRange.setStart(1);
    fundRange.setEnd(500);
    formRange.setStart(100);
    formRange.setEnd(12000);
    formRange2.setStart(100);
    formRange2.setEnd(12000);
    panRange.setStart(0);
    panRange.setStart(100);
    waveRange.setStart(0);
    waveRange.setEnd(100);
    ampRange.setStart(0);
    ampRange.setEnd(100);
}


void PulsarTrain::startTrain()
{
    pulsarPeriod = (1 / mFundFreq.get()) * mSampleRate;
    samplesRemainingInPeriod = pulsarPeriod;
}

void PulsarTrain::setFrequencies(float fundFreq, float form1, float form2)
{
    smoothFund.setTargetValue(fundFreq);
    smoothForm.setTargetValue(form1);
    smoothForm2.setTargetValue(form2);
}

//========================================================================*/
/*
    Seems small but this little function drives the whole apparatus
*/
int PulsarTrain::getPeriod()
{

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
/*========================================================================*/

/*========================================================================*/


/*
    This only sets 1 pulsaret running in each array, we iterate to find the first one that isn't already running
    Calculations are not split into sub equations because I am not going to organize all params into an array of 2
    Show me a way that I can use one function but called at different times based on if pulsaret1.isFree / pulsaret2.isFree
    One might be free and the other not (overlapping width)  I could iterate in this function and pass the iteration value at the
    function call but then all the params would need to be associated with an index of their own and making an array for each pair of params
    form/form2, wave/wave2, 
*/
void PulsarTrain::triggerPulsaret()
{

    pulsaret1.setAsHit();
    pulsaret1.isSingleCycle(isSingleCycle);
    pulsaret1.setLengthInSamples(pulsaretSamples1, samplesRemainingInPeriod);
    if (pulsaret1.isFree)
    {
        float freq;
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
        
        if(waveIsSpread.get())
        {
            juce::Random randWave;
            if(randWave.nextFloat() < waveRand.get())
            {
                auto waveNum = rand.nextInt(waveRange);
                pulsaret1.pulsaretTable.setTable(waveNum / 100.f);
            }
        }
        else
        {
            pulsaret1.pulsaretTable.setTable(waveType.get() / 100.f);
        }
        pulsaret1.setFrequency(freq); // get pulsaret ready to read
        pulsaret1.setRunning();
        
    }
    

    pulsaret2.setAsHit();
    pulsaret2.isSingleCycle(isSingleCycle);
    pulsaret2.setLengthInSamples(pulsaretSamples2, samplesRemainingInPeriod);
    
    if (pulsaret2.isFree) // find first free pulsaret in array
    {
        float freq;
        
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
        
        if(waveIsSpread.get())
        {
            juce::Random randWave;
            if(randWave.nextFloat() < waveRand.get())
            {
                auto waveNum = rand.nextInt(waveRange);
                pulsaret2.pulsaretTable.setTable(waveNum / 100.f);
            }
        }
        else
        {
            pulsaret2.pulsaretTable.setTable(waveType.get() / 100.f);
        }
        pulsaret2.setFrequency(freq);
        pulsaret2.setRunning();
    }
    
}

void PulsarTrain::triggerPulsaretWithNoAmp()
{
    pulsaret1.setAsMiss();
    pulsaret2.setAsMiss();
}

void PulsarTrain::updateFundamental()
{
    pulsarPeriod = (1 / mFundFreq.get()) * mSampleRate;
    samplesRemainingInPeriod = pulsarPeriod;
    
    smoothFund.setTargetValue(mFundFreq.get());
}


void PulsarTrain::setPulsaretWidth(float width)
{
        isSingleCycle = false;
    
        float dutyCycle = getPeriod() * width;
        pulsaretSamples1 = dutyCycle;
    
        float dutyCycle2 = getPeriod() * width;
        pulsaretSamples2 = dutyCycle2;

    if(width < 0.02)
    {

        isSingleCycle = true;
        isContinuous = false;
        pulsaretSamples1 = (1/mFormFreq.get()) * mSampleRate;
        pulsaretSamples2 = (1/mFormFreq2.get()) * mSampleRate;

    }
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
        
        if (!pulsaret1.isFree) // if the pulsaret is still running i.e. NOT free (affected by width)
        {
            sample += pulsaret1.getNextSample();
        }
        if (!pulsaret2.isFree)
        {
            sample += pulsaret2.getNextSample();
        }

        auto pLeft = panL.get() / 100.f;
        auto pRight = panR.get() / 100.f;
        auto amp = mAmp.get() / 100.f;

        auto envGain = env.getNextSample();
        if (envGain < 0.01f)
            isRunning = false;
        flashCoef = envGain;
        
        if (buffer.getNumChannels() == 1)
        {
            buffWrite[0][i] = sample * amp * envGain;
        }
        else if (buffer.getNumChannels() > 1)
        {
            buffWrite[0][i] = sample * amp * pLeft * envGain ;
            buffWrite[1][i] = sample * amp * pRight * envGain;
        }
        samplesRemainingInPeriod--;
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
        
        // start counting down the "off" counts once we have counted all the onCounts
        if(onCount <= 0)
        {
            if(offCount >= 0)
            {
                //triggerPulsaretWithNoAmp();
                isFlashing = false;
                offCount--;
            }
            
        }
        
        if(onCount <= 0 && offCount < 0)
        {
            onCount = triggerOn;
            offCount = triggerOff;
        }
    }

    if (!isTriggerPattern) // we are NOT in a triggerPattern but still might need randomized values
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
            //flipFlashState(); // every other
        }
        else
        {
            isFlashing = false;
        }
    }
}


void PulsarTrain::checkStatus()
{

}


void PulsarTrain::setTrigger(int on, int off)
{
    if(off == 0)
    {
        isTriggerPattern = false;
    }
    if(off > 0)
    {
        isTriggerPattern = true;
        
        triggerOn = on;
        triggerOff = off;
    }
}

//void PulsarTrain::setPulsaretParamsAndTrigger();

void PulsarTrain::setGlideTime(float glideTime)
{
    
    auto seconds = glideTime / 1000;
    smoothFund.reset(mSampleRate, seconds);
    smoothForm.reset(mSampleRate, seconds);
    smoothForm2.reset(mSampleRate, seconds);
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
    setEnv();
    setGlideTime(glideTime.get());
    env.noteOn();
}

void PulsarTrain::triggerRelease()
{
    env.noteOff();
}

void PulsarTrain::setEnv()
{
    envParam.attack = attack / 1000.f;
    envParam.decay = decay / 1000.f;
    envParam.sustain = sustain;
    envParam.release = release / 1000.f;
    env.setParameters(envParam);
}

void PulsarTrain::updateParams(float f, float fSpread, float fRand,
                                float fo, float foSpread, float foRand,
                                float fo2, float foSpread2, float foRand2,
                                float p, float pSpread, float pRand,
                                float a, float aSpread, float aRand,
                               float inter, float atk, float dec, float sus, float rel, float width,
                               float gTime, int trigOn, int trigOff, bool trig, float w, float wSpread, float wRand)
{
    setFrequencies(f, fo, fo2);
    mFundSpread = fSpread;
    mFundRand = fRand;

    mFormSpread = foSpread;
    mFormRand = foRand;
    
    mFormSpread2 = foSpread2;
    mFormRand2 = foRand2;
    
    panR = p;
    panL = 100.f - p;
    panSpread = pSpread;
    panRand = pRand;
    
    mAmp = a;
    mAmpSpread = aSpread;
    mAmpRand = aRand;
    
    intermittance = inter;
    
    attack = atk;
    decay = dec;
    sustain = sus;
    release = rel;
    
    glideTime = gTime;
//    setGlideTime(gTime);
    
    waveType = w;
    waveSpread = wSpread;
    waveRand = wRand;
    
    setTrigger(trigOn, trigOff); // poor name, for a trigger pattern will change
    calculateRanges();
    setPulsaretWidth(width);
}


void PulsarTrain::calculateRanges()
{
    if(mFundSpread.get() != 1)
    {
        fundIsSpread = true;
        auto start = jlimit(1.f, 499.f, mFundFreq.get() / mFundSpread.get());
        auto end = jlimit(2.f, 500.f, mFundFreq.get() * mFundSpread.get());
        
        fundRange.setStart(start);
        fundRange.setEnd(end);
    }
    else if (mFundSpread.get() == 1)
    {
        fundIsSpread = false;
    }
    
    if (mFormSpread.get() != 1)
    {
        formIsSpread = true;
        auto start = jlimit(100.f, 11999.f, mFormFreq.get() / mFormSpread.get());
        auto end = jlimit(101.f, 12000.f, mFormFreq.get() * mFormSpread.get());
        
        formRange.setStart(start);
        formRange.setEnd(end);
    }
    else if (mFormSpread.get() == 1)
    {
        formIsSpread = false;
    }
    
    
    
    if (mFormSpread2.get() != 1)
    {
        formIsSpread2 = true;
        auto start = jlimit(100.f, 11999.f, mFormFreq2.get() / mFormSpread2.get() );
        auto end = jlimit(101.f, 12000.f, mFormFreq2.get() * mFormSpread2.get() );
        formRange2.setStart(start);
        formRange2.setEnd(end);
    }
    else if (mFormSpread2.get() == 1)
    {
        formIsSpread2 = false;
    }
    
    
    if(panSpread.get() != 0)
    {
        panIsSpread = true;
        auto start = jlimit(0.f, 99.f, panR.get() - panSpread.get()/2);
        auto end = jlimit(1.f, 100.f, panR.get() + panSpread.get()/2);
        
        panRange.setStart(start);
        panRange.setEnd(end);
    }
    else if(panSpread.get() == 0)
    {
        panIsSpread = false;
    }
    
    
    
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
    

}


PulsaretTable& PulsarTrain::getPulsaretTable()
{
    return pulsaret1.getPulsaretTable();
}

