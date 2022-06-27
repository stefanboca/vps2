#include "dsp/synth/VpsVoice.hpp"

#include "dsp/synth/VpsSound.hpp"

VpsVoice::VpsVoice() {}

bool VpsVoice::canPlaySound(juce::SynthesiserSound* sound) { return dynamic_cast<VpsSound*>(sound) != nullptr; }

void VpsVoice::startNote(int midiNoteNumber,
                         float velocity,
                         juce::SynthesiserSound*,
                         int /*currentPitchWheelPosition*/) {
    currentAngle = 0.0;
    level = velocity * 0.15;
    tailOff = 0.0;

    auto cyclesPerSecond = juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber);
    auto cyclesPerSample = cyclesPerSecond / getSampleRate();

    angleDelta = cyclesPerSample * 2.0 * juce::MathConstants<double>::pi;
}

void VpsVoice::stopNote(float /*velocity*/, bool allowTailOff) {
    if (allowTailOff) {
        if (tailOff == 0.0) tailOff = 1.0;
    } else {
        clearCurrentNote();
        angleDelta = 0.0;
    }
}

void VpsVoice::renderNextBlock(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples) {
    if (angleDelta != 0.0) {
        if (tailOff > 0.0) // [7]
        {
            while (--numSamples >= 0) {
                auto currentSample = (float)(std::sin(currentAngle) * level * tailOff);

                for (auto i = outputBuffer.getNumChannels(); --i >= 0;)
                    outputBuffer.addSample(i, startSample, currentSample);

                currentAngle += angleDelta;
                ++startSample;

                tailOff *= 0.99; // [8]

                if (tailOff <= 0.005) {
                    clearCurrentNote(); // [9]

                    angleDelta = 0.0;
                    break;
                }
            }
        } else {
            while (--numSamples >= 0) // [6]
            {
                auto currentSample = (float)(std::sin(currentAngle) * level);

                for (auto i = outputBuffer.getNumChannels(); --i >= 0;)
                    outputBuffer.addSample(i, startSample, currentSample);

                currentAngle += angleDelta;
                ++startSample;
            }
        }
    }
}
