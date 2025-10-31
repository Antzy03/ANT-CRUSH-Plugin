#include "PluginProcessor.h"
#include "PluginEditor.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Custom Look and Feel implementation
AntsDistSatLookAndFeel::AntsDistSatLookAndFeel()
{
    // Pure black and white color scheme
    setColour(juce::ResizableWindow::backgroundColourId, juce::Colours::black);
    setColour(juce::Slider::thumbColourId, juce::Colours::white);
    setColour(juce::Slider::rotarySliderFillColourId, juce::Colours::white);
    setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colours::white.withAlpha(0.3f));
    setColour(juce::Slider::textBoxTextColourId, juce::Colours::white);
    setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::white.withAlpha(0.3f));
    setColour(juce::Slider::textBoxBackgroundColourId, juce::Colours::black);
    setColour(juce::Label::textColourId, juce::Colours::white);
}

void AntsDistSatLookAndFeel::drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height,
                                             float sliderPos, float rotaryStartAngle, float rotaryEndAngle,
                                             juce::Slider& /*slider*/)
{
    // Modern, minimal knob design
    const float radius = juce::jmin(width, height) * 0.35f;
    const float centerX = x + width * 0.5f;
    const float centerY = y + height * 0.5f;
    const float rx = centerX - radius;
    const float ry = centerY - radius;
    const float rw = radius * 2.0f;
    
    // Draw outer ring - thin and elegant
    g.setColour(juce::Colours::white.withAlpha(0.15f));
    g.drawEllipse(rx, ry, rw, rw, 1.0f);
    
    // Calculate angle
    const float angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
    
    // Draw value arc - precise and clean
    juce::Path valueArc;
    valueArc.addArc(rx, ry, rw, rw, rotaryStartAngle, angle, true);
    g.setColour(juce::Colours::white);
    g.strokePath(valueArc, juce::PathStrokeType(1.5f, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));
    
    // Draw indicator line
    const float pointerLength = radius * 0.8f;
    juce::Path pointer;
    pointer.startNewSubPath(0, -radius * 0.2f);
    pointer.lineTo(0, -pointerLength);
    
    g.setColour(juce::Colours::white);
    g.strokePath(pointer, juce::PathStrokeType(1.0f),
                juce::AffineTransform::rotation(angle).translated(centerX, centerY));
    
    // Small center dot
    g.fillEllipse(centerX - 1.0f, centerY - 1.0f, 2.0f, 2.0f);
}

void AntsDistSatLookAndFeel::drawLinearSlider(juce::Graphics& g, int x, int y, int width, int height,
                                            float sliderPos, float minSliderPos, float maxSliderPos,
                                            const juce::Slider::SliderStyle style, juce::Slider& slider)
{
    // Use default implementation for linear sliders
    LookAndFeel_V4::drawLinearSlider(g, x, y, width, height, sliderPos, minSliderPos, maxSliderPos, style, slider);
}

juce::Font AntsDistSatLookAndFeel::getLabelFont(juce::Label& /*label*/)
{
    return juce::Font("Helvetica", 11.0f, juce::Font::bold);
}

// Editor implementation
AntsDistSatAudioProcessorEditor::AntsDistSatAudioProcessorEditor(AntsDistSatAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p)
{
    // Create main component
    mainComponent.reset(new MainComponent(audioProcessor, audioProcessor.getValueTreeState()));
    addAndMakeVisible(*mainComponent);
    
    // Calculate optimal size to fit knobs exactly
    // Layout: Header(32) + gap(4) + padding(8) + 4 rows(75 each) + 3 gaps(8 each) + padding(8) + borders(24)
    // = 32 + 4 + 8 + 300 + 24 + 8 + 24 = 400px height
    // Width: border(24) + padding(16) + max row width (4 knobs = 280 + 24 gaps = 304) = 344px, round to 400 for header
    setSize(400, 400);
    
    // Start timer for visualizer updates
    startTimerHz(25);
}

AntsDistSatAudioProcessorEditor::~AntsDistSatAudioProcessorEditor()
{
    stopTimer();
}

void AntsDistSatAudioProcessorEditor::paint(juce::Graphics& g)
{
    // Background is handled by MainComponent
    g.fillAll(juce::Colours::black);
}

void AntsDistSatAudioProcessorEditor::resized()
{
    // Main component takes up the entire window
    mainComponent->setBounds(getLocalBounds());
}

void AntsDistSatAudioProcessorEditor::timerCallback()
{
    // Update visualizer with audio buffer data
    if (mainComponent != nullptr)
    {
        mainComponent->updateVisualizer(
            audioProcessor.getAudioBufferForSpectrogram(),
            audioProcessor.getNumSpectrumSamples());
    }
}

// Keep your other existing methods

void AntsDistSatAudioProcessorEditor::sliderValueChanged(juce::Slider* /*slider*/)
{
    // This method is called when any slider's value changes
    // Since we're using AudioProcessorValueTreeState attachments,
    // we don't need to manually update parameter values here
    
    // You can add custom behavior if needed
    repaint();
}

void AntsDistSatAudioProcessorEditor::updateValueLabels()
{
    // Update value labels for all knobs
    if (valueLabels.size() >= 12)
    {
        valueLabels[0]->setText(juce::String(driveKnob.getValue(), 1), juce::dontSendNotification);
        valueLabels[1]->setText(juce::String(mixKnob.getValue(), 2), juce::dontSendNotification);
        valueLabels[2]->setText(juce::String(saturationKnob.getValue(), 2), juce::dontSendNotification);
        valueLabels[3]->setText(juce::String(midSideKnob.getValue(), 2), juce::dontSendNotification);
        valueLabels[4]->setText(juce::String(thresholdKnob.getValue(), 1) + " dB", juce::dontSendNotification);
        valueLabels[5]->setText(juce::String(attackKnob.getValue(), 1) + " ms", juce::dontSendNotification);
        valueLabels[6]->setText(juce::String(releaseKnob.getValue(), 1) + " ms", juce::dontSendNotification);
        valueLabels[7]->setText(juce::String(bitCrushKnob.getValue(), 1) + " bit", juce::dontSendNotification);
        valueLabels[8]->setText(juce::String(bitModulationKnob.getValue(), 2), juce::dontSendNotification);
        valueLabels[9]->setText(juce::String(spectralShiftKnob.getValue(), 2), juce::dontSendNotification);
        valueLabels[10]->setText(juce::String(downsampleKnob.getValue(), 1) + "x", juce::dontSendNotification);
        valueLabels[11]->setText(juce::String(jitterKnob.getValue(), 2), juce::dontSendNotification);
    }
}

void AntsDistSatAudioProcessorEditor::drawReactiveBackground(juce::Graphics& g)
{
    // Get the size of the editor
    auto bounds = getLocalBounds();
    int width = bounds.getWidth();
    int height = bounds.getHeight();
    
    // Fill the background with a dark color
    g.fillAll(juce::Colours::black);
    
    // Draw a grid pattern
    g.setColour(juce::Colours::white.withAlpha(0.1f));
    for (int x = 0; x < width; x += 20)
    {
        g.drawLine(x, 0, x, height, 1.0f);
    }
    
    for (int y = 0; y < height; y += 20)
    {
        g.drawLine(0, y, width, y, 1.0f);
    }
    
    // Draw reactive background elements based on audio data
    g.setColour(juce::Colours::white.withAlpha(0.2f));
    
    // Update background data (this would normally be done with audio data)
    for (int i = 0; i < backgroundPoints; ++i)
    {
        // Simulate some movement for visualization
        backgroundAngles[i] += 0.01f;
        if (backgroundAngles[i] > juce::MathConstants<float>::twoPi)
            backgroundAngles[i] -= juce::MathConstants<float>::twoPi;
        
        backgroundData[i] = 0.3f + 0.2f * std::sin(backgroundAngles[i]);
    }
    
    // Draw background elements
    float centerX = width * 0.5f;
    float centerY = height * 0.5f;
    
    for (int i = 0; i < backgroundPoints; ++i)
    {
        float angle = (float)i / backgroundPoints * juce::MathConstants<float>::twoPi;
        float radius = backgroundData[i] * std::min(width, height) * 0.4f;
        
        float x = centerX + std::cos(angle) * radius;
        float y = centerY + std::sin(angle) * radius;
        
        // Draw a small dot
        g.fillEllipse(x - 2.0f, y - 2.0f, 4.0f, 4.0f);
        
        // Connect to adjacent points
        if (i > 0)
        {
            float prevAngle = (float)(i - 1) / backgroundPoints * juce::MathConstants<float>::twoPi;
            float prevRadius = backgroundData[i - 1] * std::min(width, height) * 0.4f;
            
            float prevX = centerX + std::cos(prevAngle) * prevRadius;
            float prevY = centerY + std::sin(prevAngle) * prevRadius;
            
            g.drawLine(x, y, prevX, prevY, 1.0f);
        }
    }
    
    // Connect the last point to the first
    float firstAngle = 0.0f;
    float firstRadius = backgroundData[0] * std::min(width, height) * 0.4f;
    float firstX = centerX + std::cos(firstAngle) * firstRadius;
    float firstY = centerY + std::sin(firstAngle) * firstRadius;
    
    float lastAngle = (float)(backgroundPoints - 1) / backgroundPoints * juce::MathConstants<float>::twoPi;
    float lastRadius = backgroundData[backgroundPoints - 1] * std::min(width, height) * 0.4f;
    float lastX = centerX + std::cos(lastAngle) * lastRadius;
    float lastY = centerY + std::sin(lastAngle) * lastRadius;
    
    g.drawLine(lastX, lastY, firstX, firstY, 1.0f);
}