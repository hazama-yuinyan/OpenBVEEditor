/*
  ==============================================================================

    StoredSettings.h
    Created: 13 Apr 2011 4:48:07pm
    Author:  Ryouta

  ==============================================================================
*/

#ifndef __STOREDSETTINGS_H_F46ED0DF__
#define __STOREDSETTINGS_H_F46ED0DF__

#include "../JuceLibraryCode/JuceHeader.h"


/*
*	This class loads/saves various settings from/to a file and holds them. Because it is a singleton, you can access it from anywhere, at any time.
*/
class StoredSettings
{
private:
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (StoredSettings);
	File settings_file;
	ValueTree tree_root;
	const int MAX_RECENT_STRINGS;

public:
	StoredSettings();
	~StoredSettings();

	RecentlyOpenedFilesList list;
	
	ValueTree GetProperty(const Identifier& CategoryName);
	void RestoreProperty(const Identifier& CategoryName, const Identifier& PropertyName, Value& Val);
	void AddRecentString(const Identifier& Type, const String& Str);
	void Apply(void);

	juce_DeclareSingleton(StoredSettings, false);
};



#endif  // __STOREDSETTINGS_H_F46ED0DF__
