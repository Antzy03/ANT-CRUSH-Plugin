#include "CustomLookAndFeel.h"
#include "TextureManager.h"
#include "../styles/ColorScheme.h"

CustomLookAndFeel::CustomLookAndFeel()
{
    // Set default colors for standard components
    setColour(juce::ResizableWindow::backgroundColourId, ColorScheme::backgroundDark);
    setColour(juce::Slider::thumbColourId, ColorScheme::neonCyan);
    setColour(juce::Slider::rotarySliderFillColourId, ColorScheme::neonCyan);
    setColour(juce::Slider::rotarySliderOutlineColourId, ColorScheme::neonCyan.withAlpha(0.3f));
    setColour(juce::Slider::textBoxTextColourId, ColorScheme::textColor);
    setColour(juce::Slider::textBoxOutlineColourId, ColorScheme::neonCyan.withAlpha(0.3f));
    setColour(juce::Slider::textBoxBackgroundColourId, ColorScheme::backgroundDark);
    setColour(juce::Label::textColourId, ColorScheme::textColor);
}

void CustomLookAndFeel::drawBackground(juce::Graphics& g, juce::Rectangle<int> /*bounds*/)
{
    // Simple black background (visualizer draws the animated pattern)
    g.fillAll(ColorScheme::backgroundDark);
}

void CustomLookAndFeel::drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height,
                                       float sliderPos, float rotaryStartAngle, float rotaryEndAngle,
                                       juce::Slider& slider)
{
    // Get color based on parameter name (use neon colors)
    juce::Colour knobColor = ColorScheme::getColorForParameter(slider.getName());
    
    // Calculate dimensions
    const float radius = juce::jmin(width, height) * 0.4f;
    const float centerX = x + width * 0.5f;
    const float centerY = y + height * 0.5f;
    const float rx = centerX - radius;
    const float ry = centerY - radius;
    const float rw = radius * 2.0f;
    
    // Draw outer ring with glow
    juce::Colour glowColor = knobColor.withAlpha(0.15f);
    g.setColour(glowColor);
    g.drawEllipse(rx - 1.0f, ry - 1.0f, rw + 2.0f, rw + 2.0f, 2.0f);
    g.setColour(knobColor.withAlpha(0.25f));
    g.drawEllipse(rx, ry, rw, rw, 1.5f);
    
    // Calculate angle
    const float angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
    
    // Draw value arc with neon color and glow
    juce::Path valueArc;
    valueArc.addArc(rx, ry, rw, rw, rotaryStartAngle, angle, true);
    g.setColour(knobColor);
    g.strokePath(valueArc, juce::PathStrokeType(2.5f, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));
    
    // Add glow to value arc
    g.setColour(knobColor.withAlpha(0.4f));
    g.strokePath(valueArc, juce::PathStrokeType(4.0f, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));
    
    // More detailed ASCII tick glyphs for better ASCII aesthetic
    g.setFont(juce::Font("Consolas", juce::jmax(8.0f, radius * 0.16f), juce::Font::plain));
    const int numTicks = 48; // More ticks for finer ASCII look
    const float tickR = radius * 0.92f;
    for (int i = 0; i <= numTicks; ++i)
    {
        const float t = (float)i / (float)numTicks;
        const float a = rotaryStartAngle + t * (rotaryEndAngle - rotaryStartAngle);
        const float tx = centerX + std::cos(a) * tickR;
        const float ty = centerY + std::sin(a) * tickR;
        const bool filled = (a <= angle);
        
        // Use different ASCII characters for more variety
        juce::String tickChar;
        if (i % 8 == 0)
        {
            // Major ticks every 8
            tickChar = filled ? "+" : ".";
        }
        else if (i % 4 == 0)
        {
            // Medium ticks every 4
            tickChar = filled ? ":" : "-";
        }
        else
        {
            // Minor ticks
            tickChar = filled ? "|" : ".";
        }
        
        g.setColour(filled ? knobColor : knobColor.withAlpha(0.25f));
        g.drawFittedText(tickChar, juce::Rectangle<int>((int)tx - 4, (int)ty - 4, 8, 8), juce::Justification::centred, 1);
    }
    
    // ASCII circle made of characters instead of @ symbol
    g.setFont(juce::Font("Consolas", juce::jmax(6.0f, radius * 0.12f), juce::Font::plain));
    const float innerR = radius * 0.25f;
    const int circlePoints = 16;
    for (int i = 0; i < circlePoints; ++i)
    {
        const float angle = (float)i / (float)circlePoints * juce::MathConstants<float>::twoPi;
        const float cx = centerX + std::cos(angle) * innerR;
        const float cy = centerY + std::sin(angle) * innerR;
        g.setColour(knobColor.withAlpha(0.6f));
        g.drawFittedText("o", juce::Rectangle<int>((int)cx - 3, (int)cy - 3, 6, 6), juce::Justification::centred, 1);
    }
    
    // Center point
    g.setColour(knobColor);
    g.drawFittedText("+", juce::Rectangle<int>((int)centerX - 4, (int)centerY - 4, 8, 8), juce::Justification::centred, 1);
}

juce::Font CustomLookAndFeel::getLabelFont(juce::Label& /*label*/)
{
    // Monospace for ASCII aesthetic
    return juce::Font("Consolas", 12.0f, juce::Font::bold);
}

void CustomLookAndFeel::drawLinearSlider(juce::Graphics& g, int x, int y, int width, int height,
                                     float sliderPos, float minSliderPos, float maxSliderPos,
                                     const juce::Slider::SliderStyle style, juce::Slider& slider)
{
    // ASCII-style linear sliders with neon colors
    juce::Colour sliderColor = ColorScheme::getColorForParameter(slider.getName());

    if (style == juce::Slider::LinearHorizontal)
    {
        // Compute normalized position [0..1]
        const float norm = (maxSliderPos > minSliderPos) ? (sliderPos - minSliderPos) / (maxSliderPos - minSliderPos) : 0.0f;

        // Character grid
        const int cellW = 10;
        const int cellH = juce::jmax(12, height - 6);
        const int left = x;
        const int top = y + (height - cellH) / 2;
        const int cols = juce::jmax(1, width / cellW);
        const int filled = juce::jlimit(0, cols, (int)std::round(norm * cols));

        g.setFont(juce::Font("Consolas", (float)cellH - 2.0f, juce::Font::plain));

        for (int i = 0; i < cols; ++i)
        {
            const int px = left + i * cellW;
            const bool isFilled = (i < filled);
            if (isFilled)
            {
                // Glow effect for filled cells
                g.setColour(sliderColor.withAlpha(0.3f));
                g.drawText("#", px - 1, top - 1, cellW + 2, cellH + 2, juce::Justification::centred);
                g.setColour(sliderColor);
            }
            else
            {
                g.setColour(sliderColor.withAlpha(0.15f));
            }
            const juce::String ch = isFilled ? juce::String("#") : juce::String("-");
            g.drawText(ch, px, top, cellW, cellH, juce::Justification::centred);
        }
    }
    else if (style == juce::Slider::LinearVertical)
    {
        // Compute normalized position [0..1]
        const float norm = (maxSliderPos > minSliderPos) ? (sliderPos - minSliderPos) / (maxSliderPos - minSliderPos) : 0.0f;

        // Character grid
        const int cellH = 12;
        const int cellW = juce::jmax(12, width - 6);
        const int rows = juce::jmax(1, height / cellH);
        const int filled = juce::jlimit(0, rows, (int)std::round(norm * rows));

        g.setFont(juce::Font("Consolas", (float)cellH - 1.0f, juce::Font::plain));

        for (int r = 0; r < rows; ++r)
        {
            const int py = y + height - (r + 1) * cellH;
            const bool isFilled = (r < filled);
            if (isFilled)
            {
                g.setColour(sliderColor.withAlpha(0.3f));
                g.drawText("#", x + (width - cellW) / 2 - 1, py - 1, cellW + 2, cellH + 2, juce::Justification::centred);
                g.setColour(sliderColor);
            }
            else
            {
                g.setColour(sliderColor.withAlpha(0.15f));
            }
            const juce::String ch = isFilled ? juce::String("#") : juce::String("-");
            g.drawText(ch, x + (width - cellW) / 2, py, cellW, cellH, juce::Justification::centred);
        }
    }
    else
    {
        LookAndFeel_V4::drawLinearSlider(g, x, y, width, height, sliderPos, minSliderPos, maxSliderPos, style, slider);
    }
}

// ASCII box-drawing border (using Unicode box-drawing characters)
void CustomLookAndFeel::drawASCIIBorder(juce::Graphics& g, juce::Rectangle<int> bounds, juce::Colour borderColor)
{
    g.setFont(juce::Font("Consolas", 12.0f, juce::Font::plain));
    g.setColour(borderColor);
    
    // Use Unicode characters for box drawing (double-line)
    const juce::String topLeft = juce::CharPointer_UTF8("\xe2\x95\x94");     // ╔
    const juce::String topRight = juce::CharPointer_UTF8("\xe2\x95\x97");    // ╗
    const juce::String bottomLeft = juce::CharPointer_UTF8("\xe2\x95\x9a");  // ╚
    const juce::String bottomRight = juce::CharPointer_UTF8("\xe2\x95\x9d");  // ╝
    const juce::String horizontal = juce::CharPointer_UTF8("\xe2\x95\x90");   // ═
    const juce::String vertical = juce::CharPointer_UTF8("\xe2\x95\x91");     // ║
    
    // Top border
    g.drawText(topLeft, bounds.getX(), bounds.getY(), 12, 12, juce::Justification::centredLeft);
    g.drawText(topRight, bounds.getRight() - 12, bounds.getY(), 12, 12, juce::Justification::centredLeft);
    for (int x = bounds.getX() + 12; x < bounds.getRight() - 12; x += 12)
    {
        g.drawText(horizontal, x, bounds.getY(), 12, 12, juce::Justification::centredLeft);
    }
    
    // Bottom border
    g.drawText(bottomLeft, bounds.getX(), bounds.getBottom() - 12, 12, 12, juce::Justification::centredLeft);
    g.drawText(bottomRight, bounds.getRight() - 12, bounds.getBottom() - 12, 12, 12, juce::Justification::centredLeft);
    for (int x = bounds.getX() + 12; x < bounds.getRight() - 12; x += 12)
    {
        g.drawText(horizontal, x, bounds.getBottom() - 12, 12, 12, juce::Justification::centredLeft);
    }
    
    // Left and right borders
    for (int y = bounds.getY() + 12; y < bounds.getBottom() - 12; y += 12)
    {
        g.drawText(vertical, bounds.getX(), y, 12, 12, juce::Justification::centredLeft);
        g.drawText(vertical, bounds.getRight() - 12, y, 12, 12, juce::Justification::centredLeft);
    }
}

// CRT screen glow effect - disabled (pure black background)
void CustomLookAndFeel::drawCRTGlow(juce::Graphics& /*g*/, juce::Rectangle<int> /*bounds*/)
{
    // Glow effect removed - pure black background only
} 