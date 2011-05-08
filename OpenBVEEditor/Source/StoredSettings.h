/*
  ==============================================================================

    StoredSettings.h
    Created: 13 Apr 2011 4:48:07pm
    Author:  Ryouta

	OpenBVEEditor - A simple and easy-to-use editor especially for the OpenBVE Route files
	Copyright (C) 2011  Ryouta Ozaki

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
