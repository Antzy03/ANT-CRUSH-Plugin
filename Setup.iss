[Setup]
; Basic installer information
AppName=AntsDistSat
AppVersion=1.0.0
AppPublisher=yourcompany
AppPublisherURL=www.yourcompany.com
DefaultDirName={commoncf64}\VST3
DefaultGroupName=AntsDistSat
DisableProgramGroupPage=yes
OutputBaseFilename=AntsDistSat_Setup_v1.0.0
OutputDir=Output
SetupIconFile=
Compression=lzma
SolidCompression=yes
PrivilegesRequired=admin
ArchitecturesInstallIn64BitMode=x64
CreateUninstallRegKey=yes
Uninstallable=yes
WizardStyle=modern

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Files]
; Install VST3 plugin (vst3 is a folder bundle, copy all contents) - using Debug build
Source: "Builds\VisualStudio2022\x64\Debug\VST3\AntsDistSat.vst3\*"; DestDir: "{app}\AntsDistSat.vst3"; Flags: ignoreversion recursesubdirs createallsubdirs
; Install standalone application (optional - uncomment if you want to install it)
; Source: "Builds\VisualStudio2022\x64\Debug\Standalone Plugin\AntsDistSat.exe"; DestDir: "{commonpf64}\AntsDistSat"; Flags: ignoreversion

[Icons]
; Optional: Create desktop shortcut for standalone app (uncomment if installing standalone)
; Name: "{commondesktop}\AntsDistSat"; Filename: "{commonpf64}\AntsDistSat\AntsDistSat.exe"

[UninstallDelete]
; Remove VST3 plugin folder bundle
Type: filesandordirs; Name: "{app}\AntsDistSat.vst3"

