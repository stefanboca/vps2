#include "VpsPluginAudioProcessorEditor.hpp"

#include "VpsPluginAudioProcessor.hpp"

//==============================================================================
VpsPluginAudioProcessorEditor::VpsPluginAudioProcessorEditor(VpsPluginAudioProcessor& p)
    : AudioProcessorEditor(&p),
      keyboardComponent(keyboardState, juce::MidiKeyboardComponent::horizontalKeyboard) {
    addAndMakeVisible(keyboardComponent);

    setSize(600, 160);
}

VpsPluginAudioProcessorEditor::~VpsPluginAudioProcessorEditor() {}

//==============================================================================
void VpsPluginAudioProcessorEditor::paint(juce::Graphics& g) { juce::ignoreUnused(g); }

void VpsPluginAudioProcessorEditor::resized() {
    keyboardComponent.setBounds(10, 10, getWidth() - 20, getHeight() - 20);
}
