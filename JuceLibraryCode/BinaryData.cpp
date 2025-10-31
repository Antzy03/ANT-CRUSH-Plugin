/* ==================================== JUCER_BINARY_RESOURCE ====================================

   This is an auto-generated file: Any edits you make may be overwritten!

*/

#include <cstring>

namespace BinaryData
{

//================== AntDistSat.xml ==================
static const unsigned char temp_binary_data_0[] =
"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n"
"\r\n"
"<magic>\r\n"
"  <Styles>\r\n"
"    <Style name=\"default\">\r\n"
"      <Nodes/>\r\n"
"      <Classes>\r\n"
"        <plot-view border=\"2\" background-color=\"black\"/>\r\n"
"        <slider border=\"0\" text=\"white\"/>\r\n"
"        <slider-track background=\"black\"/>\r\n"
"        <slider-thumb background=\"green\"/>\r\n"
"      </Classes>\r\n"
"      <Types>\r\n"
"        <Slider>\r\n"
"          <backgroundColour value=\"FF595C5A\" />\r\n"
"          <textColourId value=\"FFFFFFFF\" />\r\n"
"        </Slider>\r\n"
"        <ToggleButton>\r\n"
"          <backgroundColour value=\"FF595C5A\" />\r\n"
"          <textColourId value=\"FFFFFFFF\" />\r\n"
"        </ToggleButton>\r\n"
"      </Types>\r\n"
"      <Palettes>\r\n"
"        <default/>\r\n"
"      </Palettes>\r\n"
"    </Style>\r\n"
"  </Styles>\r\n"
"  <View flex-direction=\"column\">\r\n"
"    <View flex-grow=\"0.5\">\r\n"
"      <Spectrogram source=\"spectrogram\" plot-color=\"white\" background-color=\"black\"/>\r\n"
"    </View>\r\n"
"    <View flex-direction=\"row\" flex-wrap=\"wrap\">\r\n"
"      <Slider caption=\"Drive\" parameter=\"drive\"/>\r\n"
"      <Slider caption=\"Mix\" parameter=\"mix\"/>\r\n"
"      <Slider caption=\"Saturation\" parameter=\"saturation\"/>\r\n"
"      <Slider caption=\"Mid/Side\" parameter=\"midside\"/>\r\n"
"      <Slider caption=\"Threshold\" parameter=\"threshold\"/>\r\n"
"      <Slider caption=\"Attack\" parameter=\"attack\"/>\r\n"
"      <Slider caption=\"Release\" parameter=\"release\"/>\r\n"
"      <Slider caption=\"Bit Crush\" parameter=\"bitcrush\"/>\r\n"
"      <Slider caption=\"Bit Modulation\" parameter=\"bitmodulation\"/>\r\n"
"      <Slider caption=\"Downsample\" parameter=\"downsample\"/>\r\n"
"      <Slider caption=\"Jitter\" parameter=\"jitter\"/>\r\n"
"      <Slider caption=\"Spectral Shift\" parameter=\"spectralshift\"/>\r\n"
"    </View>\r\n"
"  </View>\r\n"
"</magic>";

const char* AntDistSat_xml = (const char*) temp_binary_data_0;


const char* getNamedResource (const char* resourceNameUTF8, int& numBytes);
const char* getNamedResource (const char* resourceNameUTF8, int& numBytes)
{
    unsigned int hash = 0;

    if (resourceNameUTF8 != nullptr)
        while (*resourceNameUTF8 != 0)
            hash = 31 * hash + (unsigned int) *resourceNameUTF8++;

    switch (hash)
    {
        case 0x907bef71:  numBytes = 1654; return AntDistSat_xml;
        default: break;
    }

    numBytes = 0;
    return nullptr;
}

const char* namedResourceList[] =
{
    "AntDistSat_xml"
};

const char* originalFilenames[] =
{
    "AntDistSat.xml"
};

const char* getNamedResourceOriginalFilename (const char* resourceNameUTF8);
const char* getNamedResourceOriginalFilename (const char* resourceNameUTF8)
{
    for (unsigned int i = 0; i < (sizeof (namedResourceList) / sizeof (namedResourceList[0])); ++i)
        if (strcmp (namedResourceList[i], resourceNameUTF8) == 0)
            return originalFilenames[i];

    return nullptr;
}

}
