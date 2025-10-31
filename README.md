# ANT//CRUSH

A sophisticated audio distortion and saturation plugin built with JUCE framework.

## Features

- **Drive Control**: Adjustable drive amount from 1.0 to 20.0
- **Mix Control**: Wet/dry mix control from 0% to 100%
- **Saturation**: Adjustable saturation amount
- **Mid/Side Processing**: Mid/side encoding and processing
- **Dynamic Processing**: Threshold, attack, and release controls
- **Bit Crushing**: Variable bit depth reduction (1-16 bits)
- **Bit Modulation**: Dynamic bit depth modulation
- **Downsampling**: Sample rate reduction (1x to 50x)
- **Jitter**: Adds timing variations for analog character
- **Spectral Shift**: Frequency domain processing

## Technical Details

- Built with JUCE framework
- Supports VST3, AU, AAX, and Standalone formats
- Real-time audio processing with low latency
- Advanced GUI with reactive visual elements
- Parameter automation support

## Building

### Prerequisites
- JUCE framework
- Visual Studio 2022 (Windows)
- Xcode (macOS)

### Windows Build
1. Open `AntsDistSat.jucer` in Projucer
2. Generate Visual Studio project files
3. Open the solution in Visual Studio 2022
4. Build in Release configuration

### macOS Build
1. Open `AntsDistSat.jucer` in Projucer
2. Generate Xcode project files
3. Open the project in Xcode
4. Build for desired plugin formats

## Installation

Run the installer generated after building:
- Windows: `AntsDistSat_Setup_v1.0.0.exe`
- macOS: Install component and AU files manually

## License

This project is proprietary software. All rights reserved.

## Author

Created by [Your Name]

## Support

For support and bug reports, please create an issue in this repository.
