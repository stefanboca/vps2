#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_audio_utils/juce_audio_utils.h>

class VpsPluginAudioProcessor;

//==============================================================================
class VpsPluginAudioProcessorEditor : public juce::AudioProcessorEditor {
public:
    explicit VpsPluginAudioProcessorEditor(VpsPluginAudioProcessor&);
    ~VpsPluginAudioProcessorEditor() override;

    //==============================================================================
    void paint(juce::Graphics&) override;
    void resized() override;

    juce::MidiKeyboardState& getKeyboardState() { return keyboardState; }

private:
    juce::MidiKeyboardState keyboardState;
    juce::MidiKeyboardComponent keyboardComponent;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(VpsPluginAudioProcessorEditor)
};
