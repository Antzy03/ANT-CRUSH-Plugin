; Inno Setup Script for AntsDistSat VST3 Plugin
; This script creates a Windows installer for the plugin

#define MyAppName "AntsDistSat"
#define MyAppVersion "1.0.0"
#define MyAppPublisher "Anant Chaudhary"
#define MyAppURL "https://github.com/anomalyco/AntsDistSat"

[Setup]
AppId={{B8F3A2C1-D4E5-6789-ABCD-EF1234567890}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
AppPublisher={#MyAppPublisher}
AppPublisherURL={#MyAppURL}
AppSupportURL={#MyAppURL}
AppUpdatesURL={#MyAppURL}
DefaultDirName={autopf}\{#MyAppName}
DefaultGroupName={#MyAppName}
DisableProgramGroupPage=yes
LicenseFile=LICENSE.txt
OutputDir=Output
OutputBaseFilename=AntsDistSat_Setup_v{#MyAppVersion}
Compression=lzma
SolidCompression=yes
WizardStyle=modern
ArchitecturesInstallIn64BitMode=x64os
ArchitecturesAllowed=x64os
PrivilegesRequired=admin

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Files]
; VST3 Plugin Bundle (64-bit) - Install the full .vst3 bundle directory
Source: "Builds\VisualStudio2022\x64\Release\VST3\AntsDistSat.vst3\Contents\x86_64-win\AntsDistSat.vst3"; DestDir: "{commonpf}\Common Files\VST3\AntsDistSat.vst3\Contents\x86_64-win"; Flags: ignoreversion 64bit

; Standalone Application
Source: "Builds\VisualStudio2022\x64\Release\Standalone Plugin\AntsDistSat.exe"; DestDir: "{app}"; Flags: ignoreversion 64bit

; Documentation
Source: "README.md"; DestDir: "{app}"; Flags: ignoreversion
Source: "LICENSE.txt"; DestDir: "{app}"; Flags: ignoreversion

[Icons]
Name: "{group}\{#MyAppName}"; Filename: "{app}\AntsDistSat.exe"
Name: "{group}\Uninstall {#MyAppName}"; Filename: "{uninstallexe}"

[UninstallDelete]
; Clean up the VST3 bundle directory on uninstall
Type: filesandordirs; Name: "{commonpf}\Common Files\VST3\AntsDistSat.vst3"

[Messages]
WelcomeLabel2=This will install [name/ver] on your computer.%n%nThe VST3 plugin will be installed to:%n{commonpf}\Common Files\VST3\%n%nThe standalone application will be installed to the chosen directory.%n%nClick Next to continue.

[Code]
procedure CurStepChanged(CurStep: TSetupStep);
begin
  if CurStep = ssPostInstall then
  begin
    MsgBox('Installation complete!' + #13#10 + #13#10 +
           'VST3 plugin installed to:' + #13#10 +
           ExpandConstant('{commonpf}\Common Files\VST3\AntsDistSat.vst3\') + #13#10 + #13#10 +
           'Standalone app installed to:' + #13#10 +
           ExpandConstant('{app}\AntsDistSat.exe') + #13#10 + #13#10 +
           'Please restart your DAW and rescan plugins if necessary.', mbInformation, MB_OK);
  end;
end;
