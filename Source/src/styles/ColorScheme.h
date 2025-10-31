#pragma once
#include <JuceHeader.h>

namespace ColorScheme {
    // Retro terminal color palette - pure black background
    const juce::Colour backgroundDark = juce::Colour(0, 0, 0);
    const juce::Colour backgroundLight = juce::Colour(5, 5, 5); // Very subtle for sections
    
    // Neon colors from retro terminal/DOS aesthetic
    const juce::Colour neonCyan = juce::Colour(0, 255, 255);     // #00FFFF
    const juce::Colour neonMagenta = juce::Colour(255, 0, 255); // #FF00FF
    const juce::Colour neonYellow = juce::Colour(255, 255, 0);   // #FFFF00
    const juce::Colour textColor = juce::Colours::white;
    const juce::Colour textColorDim = juce::Colour(180, 180, 180);
    
    // Glow variants (with alpha for CRT effect)
    const juce::Colour neonCyanGlow = neonCyan.withAlpha(0.3f);
    const juce::Colour neonMagentaGlow = neonMagenta.withAlpha(0.3f);
    const juce::Colour neonYellowGlow = neonYellow.withAlpha(0.3f);
    
    // Section colors (distribute neon palette)
    const juce::Colour distortionColor = neonCyan;
    const juce::Colour bitCrusherColor = neonMagenta;
    const juce::Colour dynamicsColor = neonYellow;
    
    // Get color for parameter based on name
    inline juce::Colour getColorForParameter(const juce::String& paramName) {
        if (paramName.containsIgnoreCase("Drive") || 
            paramName.containsIgnoreCase("Saturation") || 
            paramName.containsIgnoreCase("Mix") || 
            paramName.containsIgnoreCase("MidSide")) {
            return distortionColor;
        }
        else if (paramName.containsIgnoreCase("Bit") || 
                 paramName.containsIgnoreCase("Downsample") || 
                 paramName.containsIgnoreCase("Jitter")) {
            return bitCrusherColor;
        }
        else if (paramName.containsIgnoreCase("Threshold") || 
                 paramName.containsIgnoreCase("Attack") || 
                 paramName.containsIgnoreCase("Release") || 
                 paramName.containsIgnoreCase("Spectral")) {
            return dynamicsColor;
        }
        return neonCyan; // Default to cyan
    }
} 