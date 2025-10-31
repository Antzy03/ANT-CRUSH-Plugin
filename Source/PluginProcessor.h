#pragma once

#include <JuceHeader.h>


class AntsDistSatAudioProcessor : public juce::AudioProcessor
{
public:
    AntsDistSatAudioProcessor();
    ~AntsDistSatAudioProcessor() override;

    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    bool isBusesLayoutSupported(const BusesLayout& layouts) const override;

    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;
   
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram(int index) override;
    const juce::String getProgramName(int index) override;
    void changeProgramName(int index, const juce::String& newName) override;

    void getStateInformation(juce::MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;

    // Public getters for the parameters
    juce::AudioParameterFloat* getDriveParam() const { return driveParam; }
    juce::AudioParameterFloat* getMixParam() const { return mixParam; }
    juce::AudioParameterFloat* getSaturationParam() const { return saturationParam; }
    juce::AudioParameterFloat* getMidSideParam() const { return midSideParam; }
    juce::AudioParameterFloat* getThresholdParam() const { return thresholdParam; }
    juce::AudioParameterFloat* getAttackParam() const { return attackParam; }
    juce::AudioParameterFloat* getReleaseParam() const { return releaseParam; }
    juce::AudioParameterFloat* getBitCrushParam() const { return bitCrushParam; }
    juce::AudioParameterFloat* getBitModulationParam() const { return bitModulationParam; }
    juce::AudioParameterFloat* getSpectralShiftParam() const { return spectralShiftParam; }
    juce::AudioParameterFloat* getDownsampleParam() const { return downsampleParam; }
    juce::AudioParameterFloat* getJitterParam() const { return jitterParam; }

    // Methods for spectrogram
    const float* getAudioBufferForSpectrogram() const { return spectrogramBuffer.getReadPointer(0); }
    int getNumSpectrumSamples() const { return spectrogramBuffer.getNumSamples(); }
    
    // Value tree state for parameter management
    juce::AudioProcessorValueTreeState& getValueTreeState() { return *valueTreeState; }

private:

    static constexpr int fftSize = 1024;
    static constexpr int fftOrder = 10; // 2^10 = 1024
    static constexpr int spectrogramBufferSize = 1024; 

    juce::AudioParameterFloat* driveParam;
    juce::AudioParameterFloat* mixParam;
    juce::AudioParameterFloat* saturationParam;
    juce::AudioParameterFloat* midSideParam;
    juce::AudioParameterFloat* thresholdParam;
    juce::AudioParameterFloat* attackParam;
    juce::AudioParameterFloat* releaseParam;
    juce::AudioParameterFloat* bitCrushParam;
    juce::AudioParameterFloat* bitModulationParam;
    juce::AudioParameterFloat* spectralShiftParam;
    juce::AudioParameterFloat* downsampleParam;
    juce::AudioParameterFloat* jitterParam;
    juce::AudioBuffer<float> spectrogramBuffer;
    
    // Value tree state for parameter management
    std::unique_ptr<juce::AudioProcessorValueTreeState> valueTreeState;

    float processSample(float sample, float drive, float saturation);

    // Mid-Side processing methods
    void midSideEncode(juce::AudioBuffer<float>& buffer);
    void midSideDecode(juce::AudioBuffer<float>& buffer);
    void processMidSide(juce::AudioBuffer<float>& buffer, float midSideRatio);

    // Noise gate methods and variables
    float envelopeFollower(float input, float attack, float release);
    float noiseGate(float input, float threshold, float envelope);
    float envelope = 0.0f;
    float attackCoeff = 0.0f;
    float releaseCoeff = 0.0f;

    // Bitcrushing methods and variables
    float bitCrush(float sample, float bitDepth, float bitModulation, float spectralShift, float downsample, float jitter);
    juce::dsp::FFT forwardFFT;
    juce::dsp::FFT inverseFFT;
    juce::AudioBuffer<float> fftBuffer;
    juce::dsp::WindowingFunction<float> window;

    std::vector<float> fftData;
    std::vector<float> bitCrushBuffer;
    std::vector<float> modulationBuffer;

    juce::Random random;
    float phase = 0.0f;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AntsDistSatAudioProcessor)
};
