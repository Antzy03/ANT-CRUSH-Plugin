#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

AntsDistSatAudioProcessor::AntsDistSatAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
    forwardFFT(10),  // 2^10 = 1024 points
    inverseFFT(10),
    window(1024, juce::dsp::WindowingFunction<float>::hann)
#endif
{
    // Create value tree state for parameter management (this creates and owns the parameters)
    juce::AudioProcessorValueTreeState::ParameterLayout layout;
    
    layout.add(std::make_unique<juce::AudioParameterFloat>("drive", "Drive", 1.0f, 20.0f, 5.0f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("mix", "Mix", 0.0f, 1.0f, 0.5f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("saturation", "Saturation", 0.0f, 1.0f, 0.7f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("midside", "Mid/Side", 0.0f, 1.0f, 0.5f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("threshold", "Threshold", -60.0f, 0.0f, -40.0f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("attack", "Attack", 0.1f, 100.0f, 10.0f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("release", "Release", 1.0f, 500.0f, 100.0f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("bitcrush", "Bit Crush", 1.0f, 16.0f, 16.0f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("bitmodulation", "Bit Modulation", 0.0f, 1.0f, 0.0f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("downsample", "Downsample", 1.0f, 50.0f, 1.0f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("jitter", "Jitter", 0.0f, 1.0f, 0.0f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("spectralshift", "Spectral Shift", -1.0f, 1.0f, 0.0f));
    
    valueTreeState = std::make_unique<juce::AudioProcessorValueTreeState>(*this, nullptr, "Parameters", std::move(layout));
    
    // Get pointers to ValueTreeState parameters for convenience (they're already added to processor)
    driveParam = dynamic_cast<juce::AudioParameterFloat*>(valueTreeState->getParameter("drive"));
    mixParam = dynamic_cast<juce::AudioParameterFloat*>(valueTreeState->getParameter("mix"));
    saturationParam = dynamic_cast<juce::AudioParameterFloat*>(valueTreeState->getParameter("saturation"));
    midSideParam = dynamic_cast<juce::AudioParameterFloat*>(valueTreeState->getParameter("midside"));
    thresholdParam = dynamic_cast<juce::AudioParameterFloat*>(valueTreeState->getParameter("threshold"));
    attackParam = dynamic_cast<juce::AudioParameterFloat*>(valueTreeState->getParameter("attack"));
    releaseParam = dynamic_cast<juce::AudioParameterFloat*>(valueTreeState->getParameter("release"));
    bitCrushParam = dynamic_cast<juce::AudioParameterFloat*>(valueTreeState->getParameter("bitcrush"));
    bitModulationParam = dynamic_cast<juce::AudioParameterFloat*>(valueTreeState->getParameter("bitmodulation"));
    downsampleParam = dynamic_cast<juce::AudioParameterFloat*>(valueTreeState->getParameter("downsample"));
    jitterParam = dynamic_cast<juce::AudioParameterFloat*>(valueTreeState->getParameter("jitter"));
    spectralShiftParam = dynamic_cast<juce::AudioParameterFloat*>(valueTreeState->getParameter("spectralshift"));

    spectrogramBuffer.setSize(1, spectrogramBufferSize);
    fftBuffer.setSize(1, 1024);
    fftData.resize(1024);
    bitCrushBuffer.resize(1024);
    modulationBuffer.resize(1024);
}


AntsDistSatAudioProcessor::~AntsDistSatAudioProcessor() {}


const juce::String AntsDistSatAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

void AntsDistSatAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialization that you need..
    juce::ignoreUnused(sampleRate, samplesPerBlock);
}

void AntsDistSatAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

bool AntsDistSatAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
        && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;

    return true;
}

bool AntsDistSatAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool AntsDistSatAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

bool AntsDistSatAudioProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
    return true;
#else
    return false;
#endif
}

double AntsDistSatAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int AntsDistSatAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int AntsDistSatAudioProcessor::getCurrentProgram()
{
    return 0;
}

void AntsDistSatAudioProcessor::setCurrentProgram(int index)
{
    juce::ignoreUnused(index);
}

const juce::String AntsDistSatAudioProcessor::getProgramName(int index)
{
    juce::ignoreUnused(index);
    return {};
}

void AntsDistSatAudioProcessor::changeProgramName(int index, const juce::String& newName)
{
    juce::ignoreUnused(index, newName);
}

void AntsDistSatAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ignoreUnused(midiMessages);

    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());

    // Read parameters from ValueTreeState (the ones the UI controls)
    float drive = valueTreeState->getRawParameterValue("drive")->load();
    float mix = valueTreeState->getRawParameterValue("mix")->load();
    float saturation = valueTreeState->getRawParameterValue("saturation")->load();
    float midSide = valueTreeState->getRawParameterValue("midside")->load();
    float threshold = juce::Decibels::decibelsToGain(valueTreeState->getRawParameterValue("threshold")->load());
    float attack = valueTreeState->getRawParameterValue("attack")->load();
    float release = valueTreeState->getRawParameterValue("release")->load();
    float bitCrushAmount = valueTreeState->getRawParameterValue("bitcrush")->load();
    float bitModulation = valueTreeState->getRawParameterValue("bitmodulation")->load();
    float downsample = valueTreeState->getRawParameterValue("downsample")->load();
    float jitter = valueTreeState->getRawParameterValue("jitter")->load();
    float spectralShift = valueTreeState->getRawParameterValue("spectralshift")->load();

    attackCoeff = static_cast<float>(std::exp(-1.0 / (getSampleRate() * attack * 0.001f)));
    releaseCoeff = static_cast<float>(std::exp(-1.0 / (getSampleRate() * release * 0.001f)));

    if (totalNumInputChannels >= 2)
    {
        midSideEncode(buffer);
        processMidSide(buffer, midSide);
    }

    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    { 
        auto* channelData = buffer.getWritePointer(channel);
        
        for (int i = 0; i < buffer.getNumSamples(); ++i)
        {
            float cleanSample = channelData[i];
            float currentEnvelope = envelopeFollower(std::abs(cleanSample), attack, release);
            cleanSample = noiseGate(cleanSample, threshold, currentEnvelope);

            // Get the distorted version
            float distortedSample = processSample(cleanSample, drive, saturation);
            
            // Apply bit crushing to the raw sample
            float crushedSample = bitCrush(cleanSample, bitCrushAmount, bitModulation, spectralShift, downsample, jitter);
            
            // Mix clean, distorted and bit-crushed signals
            channelData[i] = cleanSample * (1.0f - mix) + (distortedSample + crushedSample) * mix * 0.5f;
        }
    }

    if (totalNumInputChannels >= 2)
    {
        midSideDecode(buffer);
    }

    // Update spectrogram (if needed)
    if (buffer.getNumChannels() > 0)
    {
        spectrogramBuffer.clear();
        spectrogramBuffer.copyFrom(0, 0, buffer.getReadPointer(0), 
                                 std::min(buffer.getNumSamples(), spectrogramBuffer.getNumSamples()));
    }
}
void AntsDistSatAudioProcessor::midSideEncode(juce::AudioBuffer<float>& buffer)
{
    auto* leftChannel = buffer.getWritePointer(0);
    auto* rightChannel = buffer.getWritePointer(1);

    for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
    {
        float mid = (leftChannel[sample] + rightChannel[sample]) * 0.5f;
        float side = (leftChannel[sample] - rightChannel[sample]) * 0.5f;

        leftChannel[sample] = mid;
        rightChannel[sample] = side;
    }
}

void AntsDistSatAudioProcessor::midSideDecode(juce::AudioBuffer<float>& buffer)
{
    auto* midChannel = buffer.getWritePointer(0);
    auto* sideChannel = buffer.getWritePointer(1);

    for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
    {
        float left = midChannel[sample] + sideChannel[sample];
        float right = midChannel[sample] - sideChannel[sample];

        midChannel[sample] = left;
        sideChannel[sample] = right;
    }
}

void AntsDistSatAudioProcessor::processMidSide(juce::AudioBuffer<float>& buffer, float midSideRatio)
{
    auto* midChannel = buffer.getWritePointer(0);
    auto* sideChannel = buffer.getWritePointer(1);

    for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
    {
        midChannel[sample] *= (1.0f - midSideRatio);
        sideChannel[sample] *= midSideRatio;
    }
}

float AntsDistSatAudioProcessor::processSample(float sample, float drive, float saturation)
{
    // Apply drive (pre-gain stage)
    sample *= drive;
    float cleanSample = sample;

    // Oversampling to reduce aliasing (process in higher resolution)
    constexpr int oversampleFactor = 4;
    float oversampled = sample;
    for (int i = 0; i < oversampleFactor; ++i)
    {
        oversampled *= 0.5f; // soft gain control during oversampling
    }

    // Non-linear soft clipping - using a more complex curve for better character
    float clippedSample = std::tanh(oversampled * (1.0f + saturation * 4.0f)) +
                          0.3f * std::pow(oversampled, 3) - 
                          0.1f * std::pow(oversampled, 5);

    // Enhanced harmonic generation using polynomial waveshaping for harmonics
    float harmonic2 = std::sin(sample * M_PI * 2.0f) * 0.12f * saturation;
    float harmonic3 = std::sin(sample * M_PI * 3.0f) * 0.07f * saturation;
    float harmonic4 = std::sin(sample * M_PI * 4.0f) * 0.03f * saturation;

    float distortedSample = clippedSample + harmonic2 + harmonic3 + harmonic4;

    // New waveshaping function for more aggressive shaping
    distortedSample = (distortedSample * (1.0f + saturation * 0.5f)) / 
                      (1.0f + std::abs(distortedSample * saturation * 3.0f));

    // Anti-aliasing step after distortion processing (downsample)
    for (int i = 0; i < oversampleFactor; ++i)
    {
        distortedSample *= 2.0f; // undo gain control from oversampling
    }

    // Final mix of clean and distorted signal, with dynamic saturation control
    return (1.0f - saturation) * cleanSample + saturation * distortedSample;
}


float AntsDistSatAudioProcessor::envelopeFollower(float input, float /*attack*/, float /*release*/)
{
    float coeff = (input > envelope) ? attackCoeff : releaseCoeff;
    envelope = input + coeff * (envelope - input);
    return envelope;
}

float AntsDistSatAudioProcessor::noiseGate(float input, float threshold, float currentEnvelope)
{
    float gateGain = (currentEnvelope > threshold) ? 1.0f : 0.0f;
    return input * gateGain;
}

float AntsDistSatAudioProcessor::bitCrush(float sample, float bitDepth, float bitModulation, float spectralShift, float downsample, float jitter)
{
    // Preserve static variables between calls
    static float localPhase = 0.0f; // Renamed to avoid hiding class member
    static float lastSample = 0.0f;
    static int sampleCount = 0;

    // Downsampling
    sampleCount++;
    if (downsample > 1.0f)
    {
        if (sampleCount % static_cast<int>(downsample) != 0)
            return lastSample;
        lastSample = sample;
    }

    // Bit depth reduction
    float maxValue = std::pow(2.0f, bitDepth) - 1.0f;
    float crushedSample = sample;
    
    // Apply bit depth reduction
    crushedSample = std::floor(crushedSample * maxValue) / maxValue;

    // Apply modulation
    if (bitModulation > 0.0f)
    {
        float modSignal = std::sin(localPhase);
        crushedSample *= (1.0f + modSignal * bitModulation);
        localPhase += 0.1f;
        if (localPhase >= 2.0f * static_cast<float>(M_PI))
            localPhase -= 2.0f * static_cast<float>(M_PI);
    }

    // Apply spectral shift
    if (spectralShift != 0.0f)
    {
        crushedSample = spectralShift > 0.0f ? 
            crushedSample * (1.0f + spectralShift) :
            crushedSample * (1.0f + spectralShift * 0.5f);
    }

    // Apply jitter
    if (jitter > 0.0f)
    {
        float jitterAmount = (random.nextFloat() * 2.0f - 1.0f) * jitter;
        crushedSample += jitterAmount * 0.1f;
    }

    // Normalize output
    return std::clamp(crushedSample, -1.0f, 1.0f);
}


bool AntsDistSatAudioProcessor::hasEditor() const
{
    return true;
}

juce::AudioProcessorEditor* AntsDistSatAudioProcessor::createEditor()
{
    return new AntsDistSatAudioProcessorEditor(*this);
}

void AntsDistSatAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    auto state = juce::ValueTree("AntsDistSatParams");
    state.setProperty("drive", driveParam->get(), nullptr);
    state.setProperty("mix", mixParam->get(), nullptr);
    state.setProperty("saturation", saturationParam->get(), nullptr);
    state.setProperty("midSide", midSideParam->get(), nullptr);
    state.setProperty("threshold", thresholdParam->get(), nullptr);
    state.setProperty("attack", attackParam->get(), nullptr);
    state.setProperty("release", releaseParam->get(), nullptr);
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void AntsDistSatAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));

    if (xmlState.get() != nullptr)
    {
        auto state = juce::ValueTree::fromXml(*xmlState);

        *driveParam = state.getProperty("drive", driveParam->get());
        *mixParam = state.getProperty("mix", mixParam->get());
        *saturationParam = state.getProperty("saturation", saturationParam->get());
        *midSideParam = state.getProperty("midSide", midSideParam->get());
        *thresholdParam = state.getProperty("threshold", thresholdParam->get());
        *attackParam = state.getProperty("attack", attackParam->get());
        *releaseParam = state.getProperty("release", releaseParam->get());
    }
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new AntsDistSatAudioProcessor();
}