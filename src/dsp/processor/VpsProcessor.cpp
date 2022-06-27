#include "dsp/processor/VpsProcessor.hpp"

#include "dsp/synth/VpsSound.hpp"
#include "dsp/synth/VpsVoice.hpp"

#include <juce_dsp/juce_dsp.h>

VpsProcessor::VpsProcessor() {
    for (int i = 0; i < 4; ++i) { synthesiser.addVoice(new VpsVoice()); }

    synthesiser.addSound(new VpsSound);
}

void VpsProcessor::prepareToPlay(double sampleRate, int samplesPerBlock) {
    synthesiser.setCurrentPlaybackSampleRate(sampleRate);
    juce::ignoreUnused(samplesPerBlock);
}

void VpsProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) {
    synthesiser.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());
}
