/*
  ==============================================================================

    Preferences.cpp
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

#include "Preferences.h"
#include "StoredSettings.h"
#include "CommonHeader.h"



ColoringDisplay::ColoringDisplay(Value& Val, const String& PropertyName) : PropertyComponent(PropertyName)
{
	color.referTo(Val);
}

void ColoringDisplay::paint(Graphics& g)
{
	PropertyComponent::paint(g);
	const Rectangle<int> COLOR_DISPLAY_AREA = getLookAndFeel().getPropertyComponentContentPosition(*this);
	g.setColour(Colour::fromString(color.toString()));
	g.fillRect(COLOR_DISPLAY_AREA);
}

void ColoringDisplay::mouseDown(const MouseEvent& e)
{
	const Point<int> DOWN_POINT = e.getMouseDownPosition();
	const Rectangle<int> COLOR_DISPLAY_AREA = getLookAndFeel().getPropertyComponentContentPosition(*this);
	if(COLOR_DISPLAY_AREA.contains(DOWN_POINT)){		//Only when you click inside the color display, a color selector shows up
		ColourSelector selector;
		selector.setName("color selector");
		selector.setCurrentColour(Colour::fromString(color.toString()));
		selector.addChangeListener(this);
		selector.setColour(ColourSelector::backgroundColourId, Colours::transparentBlack);
		selector.setSize(300, 400);

		CallOutBox callout(selector, *this, 0);
		callout.runModalLoop();
	}
}

void ColoringDisplay::changeListenerCallback(ChangeBroadcaster* source)
{
	ColourSelector* cs = dynamic_cast<ColourSelector*>(source);
	color.setValue(var(cs->getCurrentColour().toString()));
}

Information::Information(void) : main("informations", JUCEApplication::getInstance()->getApplicationName() + JUCEApplication::getInstance()->getApplicationVersion()+String("\n-A simple and easy-to-use editor"))
	, term("term")
{
	Font font = main.getFont();
	font.setHeight(40.0f);
	main.setFont(font);
	addAndMakeVisible(&main);

	Font font2 = term.getFont();
	font2.setHeight(16.0f);
	term.setFont(font2);
	term.setMultiLine(true);
	term.setReadOnly(true);
	term.setText(
		"    Copyright (C) 2011  HAZAMA\n\n"
		"    This program is free software; you can redistribute it and/or modify\n"
		"    it under the terms of the GNU General Public License as published by\n"
		"    the Free Software Foundation; either version 2 of the License, or\n"
		"    (at your option) any later version.\n\n"
		"    This program is distributed in the hope that it will be useful,\n"
		"    but WITHOUT ANY WARRANTY; without even the implied warranty of\n"
		"    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n"
		"    GNU General Public License for more details.\n\n"
		"    You should have received a copy of the GNU General Public License\n"
		"    along with this program; if not, write to the Free Software\n"
		"    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA"
		);
	addAndMakeVisible(&term);
}

void Information::paint(Graphics& g)
{
	g.fillAll(Colours::aquamarine);
}

void Information::resized(void)
{
	term.setBoundsRelative(0.1f, 0.1f, 0.8f, 0.5f);
	main.setBoundsRelative(0.05f, 0.7f, 0.9f, 0.25f);
}

PropertyPanel* MyPreferencesPanel::CreateGeneralSettings(void)
{
	PropertyPanel* tmp = new PropertyPanel();
	Array<PropertyComponent*> properties;
	StoredSettings* settings = StoredSettings::getInstance();

	/*Language Chooser*/
	Array<var> others;
	StringArray choices;
	Array<File> lang_files;
	File::getSpecialLocation(File::currentApplicationFile).getParentDirectory().getChildFile("languages/").findChildFiles(lang_files, File::findFiles, false, String("*.txt"));
	choices.add("en");
	others.add(var("en"));
	for(int i = 0; i < lang_files.size(); ++i){
		choices.add(lang_files[i].getFileNameWithoutExtension());
		others.add(var(lang_files[i].getFileNameWithoutExtension()));
	}
	ChoicePropertyComponent* choice_property = new ChoicePropertyComponent(settings->GetProperty(Identifiers::Category1).getPropertyAsValue(Identifiers::Languages, nullptr), TRANS("Language"), choices, others);
	choice_property->setTooltip(TRANS("Choose a preferred language"));
	properties.add(choice_property);

	/*Auto Bracket Closing*/
	properties.add(new BooleanPropertyComponent(settings->GetProperty(Identifiers::Category1).getPropertyAsValue(Identifiers::BracketClosing, nullptr), TRANS("Auto Bracket Closing"), TRANS("Inserts a closing parenthesis.")));

	/*Auto Indent*/
	properties.add(new BooleanPropertyComponent(settings->GetProperty(Identifiers::Category1).getPropertyAsValue(Identifiers::AutoIndent, nullptr), TRANS("Auto Identation"), TRANS("Indents automatically.")));

	/*Tab Width*/
	SliderPropertyComponent* slider = new SliderPropertyComponent(settings->GetProperty(Identifiers::Category1).getPropertyAsValue(Identifiers::TabSize, nullptr), TRANS("Tab Width"), 1.0, 100.0, 1.0);
	slider->setTooltip(TRANS("Changes the width of a TAB"));
	properties.add(slider);

	tmp->addProperties(properties);
	return tmp;
}

Component* MyPreferencesPanel::createComponentForPage(const String& pageName)
{
	if(pageName == "GeneralSettings"){
		return CreateGeneralSettings();
	}else if(pageName == "KeyMappings"){
		return new KeyMappingEditorComponent(*command_manager->getKeyMappings(), true);
	}else if(pageName == "Informations"){
		return new Information();
	}

	return nullptr;
}

Preferences::Preferences(void) : panel(), apply_button("Apply Button", TRANS("Apply the changes"))
{
	ScopedPointer<Drawable> general_img(Drawable::createFromImageFile(File::getSpecialLocation(File::currentApplicationFile).getParentDirectory().getChildFile("icons/078784-blue-metallic-orb-icon-business-gear11.png")));
	ScopedPointer<Drawable> keymap_img(Drawable::createFromImageFile(File::getSpecialLocation(File::currentApplicationFile).getParentDirectory().getChildFile("icons/069692-blue-metallic-orb-icon-alphanumeric-letter-tt.png")));
	ScopedPointer<Drawable> info_img(Drawable::createFromImageFile(File::getSpecialLocation(File::currentApplicationFile).getParentDirectory().getChildFile("icons/069649-blue-metallic-orb-icon-alphanumeric-information2-ps.png")));
	panel.addSettingsPage("GeneralSettings", general_img, nullptr, nullptr);
	panel.addSettingsPage("KeyMappings", keymap_img, nullptr, nullptr);
	panel.addSettingsPage("Informations", info_img, nullptr, nullptr);
	addAndMakeVisible(&panel);

	apply_button.setButtonText(TRANS("Apply"));
	apply_button.addListener(this);
	addAndMakeVisible(&apply_button);
}

void Preferences::buttonClicked(Button*)
{
	StoredSettings::getInstance()->Apply();
}

void Preferences::resized(void)
{
	panel.setBoundsRelative(0.0f, 0.0f, 1.0f, 0.95f);
	apply_button.setBoundsRelative(0.8f, 0.95f, 0.15f, 0.05f);
}

void Preferences::ShowInDialogBox(const int Width, const int Height, const Colour& BackgroundColor)
{
	setSize(Width, Height);
	DialogWindow::showDialog(TRANS("Preferences"), this, nullptr, BackgroundColor, true);
}
