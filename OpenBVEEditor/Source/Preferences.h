/*
  ==============================================================================

    Preferences.h
    Created: 17 Apr 2011 12:37:43pm
    Author:  Ryouta

  ==============================================================================
*/

#ifndef __PREFERENCES_H_42DB53D3__
#define __PREFERENCES_H_42DB53D3__


#include "../JuceLibraryCode/JuceHeader.h"



/**
* This class allows the user to specify the colors here and there.
*/
class ColoringDisplay : public PropertyComponent,
						public ChangeListener
{
private:
	Value color;		//stores a string representing a color

public:
	ColoringDisplay(Value& Val, const String& PropertyName);
	~ColoringDisplay(void){};

	void paint(Graphics& g);
	void mouseDown(const MouseEvent& e);
	void changeListenerCallback(ChangeBroadcaster* source);
};

/**
* The component to show various informations on the application itself. You may be able to check for an update in the future.
*/
class Information : public Component
{
private:
	Label main;

public:
	Information(void);
	~Information(void){};

	void paint(Graphics& g);
	void resized(void);
};

/**
* The Preferences Panel
*/
class MyPreferencesPanel : public PreferencesPanel
{
private:
	PropertyPanel* CreateGeneralSettings(void);

public:
	MyPreferencesPanel(void){};
	~MyPreferencesPanel(void){};
	Component* createComponentForPage(const String& pageName);
};

/**
* Content component for the Preference
*/
class Preferences : public Component,
					public ButtonListener
{
private:
	TextButton apply_button;
	MyPreferencesPanel panel;

public:
	Preferences(void);
	~Preferences(void){};

	void buttonClicked(Button*);
	void ShowInDialogBox(const int Width, const int Height, const Colour& BackgroundColor);
	void resized(void);
	JUCE_LEAK_DETECTOR(Preferences);
};


#endif  // __PREFERENCES_H_42DB53D3__
