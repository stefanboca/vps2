#pragma once

#include "dsp/processor/ProcessorBase.hpp"

#include <juce_audio_processors/juce_audio_processors.h>

class VpsProcessor : public ProcessorBase {
public:
    VpsProcessor();

    const juce::String getName() const override { return "VPS Core"; }
    bool acceptsMidi() const override { return true; }

    void prepareToPlay(double sampleRate, int samplesPerBlock) override;

    void processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer&) override;

    void reset() override {}

private:
    juce::Synthesiser synthesiser;
};
