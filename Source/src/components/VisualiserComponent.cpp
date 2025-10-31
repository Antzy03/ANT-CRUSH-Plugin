#include "VisualiserComponent.h"
#include "TextureManager.h"

VisualiserComponent::VisualiserComponent()
{
    // Initialize visualization data
    visualData.resize(512, 0.0f);
    targetData.resize(512, 0.0f);
    waveformPoints.resize(512, 0.0f);
    
    // Start the timer for animation (smooth vibration)
    startTimerHz(24); // 24 fps for better performance
}

VisualiserComponent::~VisualiserComponent()
{
    stopTimer();
}

void VisualiserComponent::paint(juce::Graphics& g)
{
    juce::Rectangle<float> bounds = getLocalBounds().toFloat();
    g.fillAll(ColorScheme::backgroundDark);

    if (visualData.empty())
        return;

    static const juce::String asciiRamp = " .':-=+*#%@"; // low -> high density (more gradation)
    const int cellW = 10;   // Cell width for vertical columns (smaller for better coverage)
    const int cellH = 16;   // Character cell height
    // Ensure we fill the entire width by calculating exact columns needed
    const int cols = juce::jmax(1, (int)std::ceil(bounds.getWidth() / cellW)); // Fill entire width
    const int rows = juce::jmax(1, (int)bounds.getHeight() / cellH);

    // Prepare font and color
    g.setFont(juce::Font("Consolas", (float)cellH - 1.0f, juce::Font::plain));
    
    // Vertical visualizer: columns from left to right, amplitude from bottom to top
    // Nicely mirrored: symmetric reflection with center peak
    const float maxAmp = (float)rows * 0.9f; // Max amplitude in rows (90% of height)

    // Map visualData into column amplitudes (vertical spectrum style)
    // Render mirrored across full width with proper center filling
    for (int cx = 0; cx < cols; ++cx)
    {
        // Calculate mirrored position: 0.0->0.5 maps to 0->1, 0.5->1.0 mirrors back
        const float normalizedPos = (float)cx / (float)juce::jmax(1, cols - 1); // 0.0 to 1.0
        float mirroredPos;
        
        if (normalizedPos <= 0.5f)
        {
            // Left side: map 0.0->0.5 linearly to 0.0->1.0
            mirroredPos = normalizedPos * 2.0f;
        }
        else
        {
            // Right side: map 0.5->1.0 to mirror back 1.0->0.0
            mirroredPos = (1.0f - normalizedPos) * 2.0f;
        }
        
        // Map mirrored position to data index (0 to max)
        const int si = (int)juce::jlimit(0, (int)visualData.size() - 1, (int)std::round(mirroredPos * (visualData.size() - 1)));
        const float v = juce::jlimit(-1.0f, 1.0f, visualData[si]);
        const float amp = std::abs(v); // Amplitude [0..1]
        const float ampRows = amp * maxAmp; // Convert to row units

        // Render column from bottom to top
        for (int ry = 0; ry < rows; ++ry)
        {
            const float rowFromBottom = (float)(rows - ry - 1); // 0 = bottom, rows-1 = top
            const bool withinColumn = rowFromBottom < ampRows;

            // Compute density index based on distance from bottom (peak at top of column)
            float proximity = 0.0f;
            if (withinColumn && ampRows > 0.0f)
                proximity = juce::jlimit(0.0f, 1.0f, rowFromBottom / (ampRows + 1e-3f));

            int idx = 0;
            juce::String displayChar;
            
            // Add some "crazy" ant characters randomly scattered (reduced frequency for performance)
            const float antChance = 0.08f; // 8% chance for ant character
            const bool showAnt = withinColumn && (random.nextFloat() < antChance);
            
            if (showAnt)
            {
                // Random ant-related characters - more variety!
                const juce::String antChars = "A<>^v[]{}";
                const int antIdx = (int)(random.nextFloat() * antChars.length());
                displayChar = juce::String::charToString(antChars[antIdx]);
                idx = asciiRamp.length() - 1; // Use brightest character for ants
            }
            else if (withinColumn)
            {
                idx = (int)juce::jlimit(0, asciiRamp.length() - 1, (int)std::round(proximity * (asciiRamp.length() - 1)));
                displayChar = juce::String::charToString(asciiRamp[idx]);
            }
            else
            {
                displayChar = juce::String::charToString(asciiRamp[idx]);
            }

            // Neon colors with stronger glow effect - cyan/magenta/yellow gradient
            float alpha = withinColumn ? (0.5f + 0.5f * (1.0f - proximity)) : 0.05f; // Brighter at peak
            if (showAnt) alpha = 1.0f; // Ants are always bright
            
            // Color mapping: cyan at bottom, magenta in middle, yellow at peaks (top of column)
            juce::Colour charColor;
            if (withinColumn)
            {
                // Create color gradient based on mirrored position for symmetry
                float colorPhase = normalizedPos + (float)juce::Time::getMillisecondCounter() / 10000.0f;
                float colorMix = std::sin(colorPhase * juce::MathConstants<float>::twoPi) * 0.5f + 0.5f;
                
                if (proximity > 0.7f) // Peak areas - yellow
                {
                    charColor = ColorScheme::neonYellow.withAlpha(alpha);
                }
                else if (proximity > 0.4f) // Mid areas - magenta
                {
                    charColor = ColorScheme::neonMagenta.withAlpha(alpha);
                }
                else // Base areas - cyan
                {
                    charColor = ColorScheme::neonCyan.withAlpha(alpha);
                }
            }
            else
            {
                charColor = juce::Colours::white.withAlpha(alpha);
            }
            
            // Calculate x position - scale to fill entire width
            const float xPos = bounds.getX() + (float)cx / (float)juce::jmax(1, cols - 1) * bounds.getWidth();
            const int px = (int)xPos;
            const int py = (int)bounds.getY() + ry * cellH;
            
            // Simplified glow effect for better performance (single subtle glow)
            if ((withinColumn || showAnt) && alpha > 0.1f)
            {
                // Single subtle glow layer
                g.setColour(charColor.withAlpha(alpha * 0.4f));
                g.drawText(displayChar, px - 1, py - 1, cellW + 2, cellH + 2, juce::Justification::centred);
            }
            
            // Main character
            if (withinColumn || showAnt || alpha > 0.05f)
            {
                g.setColour(charColor);
                g.drawText(displayChar, px, py, cellW, cellH, juce::Justification::centred);
            }
        }
    }
}

void VisualiserComponent::resized()
{
    // Nothing specific needed here
}

void VisualiserComponent::setAudioBuffer(const float* buffer, int size)
{
    if (buffer != nullptr && size > 0)
    {
        // Resize audio buffer if needed
        if (audioBuffer.getNumSamples() != size)
        {
            audioBuffer.setSize(1, size);
        }
        
        // Copy buffer data
        audioBuffer.copyFrom(0, 0, buffer, size);
        
        // Update target data for visualization
        // Focus on bass/mid frequencies by emphasizing lower frequencies
        const int numPoints = juce::jmin(size, (int)targetData.size());
        const float* samples = audioBuffer.getReadPointer(0);
        
        // Apply low-pass emphasis: weight lower frequencies more heavily
        // Cut off visualization beyond bass/mids
        for (int i = 0; i < numPoints; ++i)
        {
            const float freqPosition = (float)i / (float)juce::jmax(1, numPoints - 1);
            // Emphasize bass/mids: weight decreases as frequency increases
            // Cut off frequencies beyond ~60% range
            const float bassMidWeight = (freqPosition > 0.6f) ? 0.0f : (1.0f - (freqPosition / 0.6f) * 0.5f);
            targetData[i] = samples[i] * bassMidWeight;
        }
    }
}

void VisualiserComponent::setActiveColor(juce::Colour newColor)
{
    activeColor = newColor;
    repaint();
}

void VisualiserComponent::setIntensity(float newIntensity)
{
    intensity = juce::jlimit(0.0f, 1.0f, newIntensity);
    repaint();
}

void VisualiserComponent::timerCallback()
{
    updateVisualisation();
    repaint();
}

void VisualiserComponent::updateVisualisation()
{
    // Smooth transition with faster response for vibration effect
    for (int i = 0; i < visualData.size(); ++i)
    {
        visualData[i] = visualData[i] * (1.0f - smoothingFactor) + targetData[i] * smoothingFactor;
        // Add subtle randomization for vibration effect
        if (audioBuffer.getNumSamples() > 0)
        {
            visualData[i] += (random.nextFloat() - 0.5f) * 0.05f * std::abs(targetData[i]);
        }
    }
    
    // If no audio data is available, create some demo animation (more vibrant)
    if (audioBuffer.getNumSamples() == 0)
    {
        const float time = (float)juce::Time::getMillisecondCounter() / 1000.0f;
        
        for (int i = 0; i < targetData.size(); ++i)
        {
            float phase = (float)i / targetData.size() * juce::MathConstants<float>::twoPi * 4.0f;
            float wave1 = std::sin(phase + time * 2.5f);
            float wave2 = std::sin(phase * 1.3f + time * 3.2f) * 0.6f;
            targetData[i] = (wave1 + wave2) * 0.7f;
        }
    }
}

// Rest of the implementation... 