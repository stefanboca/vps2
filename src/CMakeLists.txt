juce_add_plugin(VpsPlugin
    IS_SYNTH TRUE
    NEEDS_MIDI_INPUT FALSE
    NEEDS_MIDI_OUTPUT FALSE
    IS_MIDI_EFFECT FALSE
    EDITOR_WANTS_KEYBOARD_FOCUS FALSE

    PLUGIN_MANUFACTURER_CODE SDBO
    PLUGIN_CODE Vps2

    FORMATS VST3 Standalone LV2

    PRODUCT_NAME "VPS"
    LV2URI "https://github.com/stefanboca/vps2"
)

target_sources(VpsPlugin
    PRIVATE
        VpsPluginAudioProcessor.cpp
        VpsPluginAudioProcessorEditor.cpp
        dsp/processor/VpsProcessor.cpp
        dsp/synth/VpsVoice.cpp
)

target_include_directories(VpsPlugin
    PRIVATE
        include/
)

target_compile_definitions(VpsPlugin
    PUBLIC
    JUCE_WEB_BROWSER=0
    JUCE_USE_CURL=0
    JUCE_VST3_CAN_REPLACE_VST2=0
)

target_link_libraries(VpsPlugin
    PRIVATE
        juce::juce_audio_utils
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags
)
