# Automatically generated makefile, created by the Introjucer
# Don't edit this file! Your changes will be overwritten when you re-save the Introjucer project!

LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := juce_jni
LOCAL_SRC_FILES := \
  ../../../Source/BVECode.cpp\
  ../../../Source/VariousTools.cpp\
  ../../../Source/CodeAssistant.cpp\
  ../../../Source/SearchComponents.cpp\
  ../../../Source/Preferences.cpp\
  ../../../Source/StoredSettings.cpp\
  ../../../Source/MainComponent.cpp\
  ../../../Source/CodeEditor.cpp\
  ../../../Source/OpenBVECode.cpp\
  ../../../Source/MainWindow.cpp\
  ../../../Source/Main.cpp\
  ../../../../../juce/modules/juce_core/juce_core.cpp\
  ../../../../../juce/modules/juce_cryptography/juce_cryptography.cpp\
  ../../../../../juce/modules/juce_data_structures/juce_data_structures.cpp\
  ../../../../../juce/modules/juce_events/juce_events.cpp\
  ../../../../../juce/modules/juce_graphics/juce_graphics.cpp\
  ../../../../../juce/modules/juce_gui_basics/juce_gui_basics.cpp\
  ../../../../../juce/modules/juce_gui_extra/juce_gui_extra.cpp\

ifeq ($(CONFIG),Debug)
  LOCAL_CPPFLAGS += -fsigned-char -fexceptions -frtti -g -I "../../JuceLibraryCode" -O0 -D "JUCE_ANDROID=1" -D "JUCE_ANDROID_API_VERSION=8" -D "JUCE_ANDROID_ACTIVITY_CLASSNAME=com_yourcompany_openbveeditor_OpenBVEEditor" -D JUCE_ANDROID_ACTIVITY_CLASSPATH=\"com/yourcompany/openbveeditor/OpenBVEEditor\" -D "DEBUG=1" -D "_DEBUG=1" -D "JUCER_ANDROID_7F0E4A25=1"
  LOCAL_LDLIBS := -llog -lGLESv2
else
  LOCAL_CPPFLAGS += -fsigned-char -fexceptions -frtti -I "../../JuceLibraryCode" -Os -D "JUCE_ANDROID=1" -D "JUCE_ANDROID_API_VERSION=8" -D "JUCE_ANDROID_ACTIVITY_CLASSNAME=com_yourcompany_openbveeditor_OpenBVEEditor" -D JUCE_ANDROID_ACTIVITY_CLASSPATH=\"com/yourcompany/openbveeditor/OpenBVEEditor\" -D "NDEBUG=1" -D "JUCER_ANDROID_7F0E4A25=1"
  LOCAL_LDLIBS := -llog -lGLESv2
endif

include $(BUILD_SHARED_LIBRARY)
