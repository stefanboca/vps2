#pragma once

#include <juce_audio_basics/juce_audio_basics.h>

struct VpsSound : public juce::SynthesiserSound {
    VpsSound() {}

    bool appliesToNote(int) override { return true; }
    bool appliesToChannel(int) override { return true; }
};
