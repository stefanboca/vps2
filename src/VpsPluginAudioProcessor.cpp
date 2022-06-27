#include "VpsPluginAudioProcessor.hpp"

#include "VpsPluginAudioProcessorEditor.hpp"
#include "dsp/processor/VpsProcessor.hpp"

VpsPluginAudioProcessor::VpsPluginAudioProcessor()
    : AudioProcessor(BusesProperties().withOutput("Output", juce::AudioChannelSet::stereo(), true)),
      processorGraph(new juce::AudioProcessorGraph()) {}

VpsPluginAudioProcessor::~VpsPluginAudioProcessor() {}

//==============================================================================
const juce::String VpsPluginAudioProcessor::getName() const { return JucePlugin_Name; }

bool VpsPluginAudioProcessor::acceptsMidi() const { return true; }

bool VpsPluginAudioProcessor::producesMidi() const { return false; }

bool VpsPluginAudioProcessor::isMidiEffect() const { return false; }

double VpsPluginAudioProcessor::getTailLengthSeconds() const { return 0.0; }

int VpsPluginAudioProcessor::getNumPrograms() {
    return 1; // NB: some hosts don't cope very well if you tell them there are 0 programs,
              // so this should be at least 1, even if you're not really implementing programs.
}

int VpsPluginAudioProcessor::getCurrentProgram() { return 0; }

void VpsPluginAudioProcessor::setCurrentProgram(int index) { juce::ignoreUnused(index); }

const juce::String VpsPluginAudioProcessor::getProgramName(int index) {
    juce::ignoreUnused(index);
    return {};
}

void VpsPluginAudioProcessor::changeProgramName(int index, const juce::String& newName) {
    juce::ignoreUnused(index, newName);
}

//==============================================================================
void VpsPluginAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock) {
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    processorGraph->setPlayConfigDetails(0, getMainBusNumOutputChannels(), sampleRate, samplesPerBlock);
    processorGraph->prepareToPlay(sampleRate, samplesPerBlock);

    initialiseGraph();
}

void VpsPluginAudioProcessor::releaseResources() { processorGraph->releaseResources(); }

bool VpsPluginAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const {
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono() &&
        layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
    return true;
}

void VpsPluginAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) {
    juce::ScopedNoDenormals noDenormals;

    for (auto i = getTotalNumInputChannels(); i < getTotalNumOutputChannels(); ++i)
        buffer.clear(i, 0, buffer.getNumSamples());

    updateGraph();

    if (getActiveEditor() != nullptr) {
        VpsPluginAudioProcessorEditor* editor = dynamic_cast<VpsPluginAudioProcessorEditor*>(getActiveEditor());
        editor->getKeyboardState().processNextMidiBuffer(midiMessages, 0, buffer.getNumSamples(), true); // [4]
    }

    processorGraph->processBlock(buffer, midiMessages);
}

//==============================================================================
bool VpsPluginAudioProcessor::hasEditor() const { return true; }

juce::AudioProcessorEditor* VpsPluginAudioProcessor::createEditor() { return new VpsPluginAudioProcessorEditor(*this); }

//==============================================================================
void VpsPluginAudioProcessor::getStateInformation(juce::MemoryBlock& destData) { juce::ignoreUnused(destData); }

void VpsPluginAudioProcessor::setStateInformation(const void* data, int sizeInBytes) {
    juce::ignoreUnused(data, sizeInBytes);
}

//==============================================================================
void VpsPluginAudioProcessor::initialiseGraph() {
    processorGraph->clear();

    audioOutputNode = processorGraph->addNode(std::make_unique<juce::AudioProcessorGraph::AudioGraphIOProcessor>(
        juce::AudioProcessorGraph::AudioGraphIOProcessor::audioOutputNode));
    midiInputNode = processorGraph->addNode(std::make_unique<juce::AudioProcessorGraph::AudioGraphIOProcessor>(
        juce::AudioProcessorGraph::AudioGraphIOProcessor::midiInputNode));

    vpsProcessor = processorGraph->addNode(std::make_unique<VpsProcessor>());

    for (int channel = 0; channel < getTotalNumOutputChannels(); ++channel)
        processorGraph->addConnection({{vpsProcessor->nodeID, channel}, {audioOutputNode->nodeID, channel}});

    processorGraph->addConnection({{midiInputNode->nodeID, juce::AudioProcessorGraph::midiChannelIndex},
                                   {vpsProcessor->nodeID, juce::AudioProcessorGraph::midiChannelIndex}});
}

void VpsPluginAudioProcessor::updateGraph() {}

//==============================================================================
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter() { return new VpsPluginAudioProcessor(); }
