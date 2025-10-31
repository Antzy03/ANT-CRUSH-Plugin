#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "src/components/MainComponent.h"

class AntsDistSatLookAndFeel : public juce::LookAndFeel_V4
{
public:
    AntsDistSatLookAndFeel();
    
    void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, 
                          float sliderPos, float rotaryStartAngle, float rotaryEndAngle, 
                          juce::Slider& slider) override;
                          
    juce::Font getLabelFont(juce::Label& label) override;
    
    void drawLinearSlider(juce::Graphics& g, int x, int y, int width, int height,
                         float sliderPos, float minSliderPos, float maxSliderPos,
                         const juce::Slider::SliderStyle style, juce::Slider& slider) override;
    
private:
    // Strict black and white palette
    juce::Colour accentColor = juce::Colours::white;
    juce::Colour backgroundColor = juce::Colours::black;
    juce::Colour textColor = juce::Colours::white;
};

class AntsDistSatAudioProcessorEditor : public juce::AudioProcessorEditor,
                                        private juce::Slider::Listener,
                                        private juce::Timer
{
public:
    AntsDistSatAudioProcessorEditor(AntsDistSatAudioProcessor&);
    ~AntsDistSatAudioProcessorEditor() override;

    void paint(juce::Graphics&) override;
    void resized() override;
    
    // Add getter for MainComponent
    MainComponent* getMainComponent() { return mainComponent.get(); }

private:
    void sliderValueChanged(juce::Slider* slider) override;
    void timerCallback() override;
    void updateValueLabels();
    void drawReactiveBackground(juce::Graphics& g);

    AntsDistSatAudioProcessor& audioProcessor;

    // Reactive background data
    static constexpr int backgroundPoints = 128;
    float backgroundData[backgroundPoints] = {0};
    float backgroundAngles[backgroundPoints] = {0};

    // Keep using your existing sliders for now
    juce::Slider driveKnob;
    juce::Slider mixKnob;
    juce::Slider saturationKnob;
    juce::Slider midSideKnob;
    juce::Slider thresholdKnob;
    juce::Slider attackKnob;
    juce::Slider releaseKnob;
    juce::Slider bitCrushKnob;
    juce::Slider bitModulationKnob;
    juce::Slider spectralShiftKnob; 
    juce::Slider downsampleKnob;
    juce::Slider jitterKnob;

    juce::Label driveLabel;
    juce::Label mixLabel;
    juce::Label saturationLabel;
    juce::Label midSideLabel;
    juce::Label thresholdLabel;
    juce::Label attackLabel;
    juce::Label releaseLabel;
    juce::Label bitCrushLabel;
    juce::Label bitModulationLabel;
    juce::Label spectralShiftLabel;
    juce::Label downsampleLabel;
    juce::Label jitterLabel;

    juce::OwnedArray<juce::Label> valueLabels;
    AntsDistSatLookAndFeel customLookAndFeel;
    
    // Group knobs into functional sections
    struct KnobSection {
        juce::Slider* knob;
        juce::Label* label;
    };
    
    std::vector<KnobSection> distortionSection;
    std::vector<KnobSection> bitCrushSection;
    std::vector<KnobSection> dynamicsSection;

    // Add slider attachments for AudioProcessorValueTreeState
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> driveAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mixAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> saturationAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> midSideAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> thresholdAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> attackAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> releaseAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> bitCrushAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> bitModulationAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> spectralShiftAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> downsampleAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> jitterAttachment;

    // Main component that contains all UI elements
    std::unique_ptr<MainComponent> mainComponent;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AntsDistSatAudioProcessorEditor)
};