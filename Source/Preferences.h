/*
  ==============================================================================

    Preferences.h
    Created: 17 Apr 2011 12:37:43pm

	OpenBVEEditor - A simple and easy-to-use editor especially for the OpenBVE Route files
	Copyright (C) 2011  HAZAMA
	http://funprogramming.ojaru.jp

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

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
* The component to show various informations on the application itself. You may be able to check for an update in the future from the component.
*/
class Information : public Component
{
private:
	Label main;
	TextEditor term;

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
