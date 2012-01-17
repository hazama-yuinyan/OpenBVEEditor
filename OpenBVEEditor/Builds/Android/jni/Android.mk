# Automatically generated makefile, created by the Jucer
# Don't edit this file! Your changes will be overwritten when you re-save the Jucer project!

LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_CPP_EXTENSION := cpp
LOCAL_MODULE := juce_jni
LOCAL_SRC_FILES := \
  ../../../Source/VariousTools.cpp\
  ../../../Source/CodeAssistant.cpp\
  ../../../Source/SearchComponents.cpp\
  ../../../Source/Preferences.cpp\
  ../../../Source/StoredSettings.cpp\
  ../../../Source/MainComponent.cpp\
  ../../../Source/CodeEditor.cpp\
  ../../../Source/BVECode.cpp\
  ../../../Source/MainWindow.cpp\
  ../../../Source/Main.cpp\
  ../../../JuceLibraryCode/JuceLibraryCode1.cpp\
  ../../../JuceLibraryCode/JuceLibraryCode2.cpp\
  ../../../JuceLibraryCode/JuceLibraryCode3.cpp\
  ../../../JuceLibraryCode/JuceLibraryCode4.cpp\

ifeq ($(CONFIG),Debug)
  LOCAL_CPPFLAGS += -fsigned-char -fexceptions -frtti -g -O0 -D "JUCE_ANDROID=1" -D "DEBUG=1" -D "_DEBUG=1" -D "JUCER_ANDROID_7F0E4A25=1"
else
  LOCAL_CPPFLAGS += -fsigned-char -fexceptions -frtti -Os -D "JUCE_ANDROID=1" -D "NDEBUG=1" -D "JUCER_ANDROID_7F0E4A25=1"
endif

include $(BUILD_SHARED_LIBRARY)
