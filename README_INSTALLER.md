# Windows Installer Setup Guide

## Building the Installer

To create a Windows installer for AntsDistSat:

### Prerequisites

1. **Inno Setup** (free, open-source installer creator for Windows)
   - Download from: https://jrsoftware.org/isdl.php
   - Install Inno Setup (version 6.0 or later recommended)

2. **Build the Debug version** of the plugin:
   - Open `Builds/VisualStudio2022/AntsDistSat.sln` in Visual Studio
   - Select "Debug" and "x64" configurations
   - Build the solution (Build → Build Solution)
   - The VST3 plugin will be created at: `Builds/VisualStudio2022/x64/Debug/VST3/AntsDistSat.vst3`
   - Or run `build_release.bat` to build automatically

### Creating the Installer

1. **Open Inno Setup Compiler**
   - Launch Inno Setup from Start Menu

2. **Open the Setup Script**
   - File → Open
   - Navigate to your project folder and open `Setup.iss`

3. **Build the Installer**
   - Build → Compile (or press F9)
   - The installer `.exe` file will be created in the `Output` folder in your project directory
   - Or run `create_installer.bat` to create the installer automatically

### What the Installer Does

- Installs the VST3 plugin to: `C:\Program Files\Common Files\VST3\AntsDistSat.vst3`
- This is the standard Windows location where most DAWs look for VST3 plugins
- Requires administrator privileges to install (standard for VST3 plugins)
- Creates an uninstaller that can be accessed through Windows Settings → Apps

### Customization

You can customize the installer by editing `Setup.iss`:

- **AppVersion**: Change the version number
- **OutputBaseFilename**: Change the installer filename
- **AppPublisher**: Add your company name
- **Standalone App**: Uncomment the lines marked with `;` to also install the standalone application

### Testing the Installer

1. Run the generated installer (`.exe` file)
2. Follow the installation wizard
3. Restart your DAW and check if the plugin appears in your plugin list
4. To uninstall: Windows Settings → Apps → AntsDistSat → Uninstall

### Troubleshooting

**Plugin doesn't appear in DAW:**
- Make sure you installed to the correct location (`C:\Program Files\Common Files\VST3\`)
- Some DAWs need to be restarted or have their plugin cache rescanned
- Check your DAW's plugin preferences/settings

**Installer fails:**
- Make sure you've built the Release version first
- Check that `AntsDistSat.vst3` exists in the expected location
- Run Visual Studio as administrator if you have permission issues

