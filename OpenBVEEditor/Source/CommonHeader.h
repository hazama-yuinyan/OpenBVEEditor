/*
  ==============================================================================

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


#ifndef _COMMON_HEADER
#define _COMMON_HEADER

#include "CommandIDs.h"
#include "../JuceLibraryCode/JuceHeader.h"


extern ApplicationCommandManager* command_manager;


template<class T>
Rectangle<T> operator+(const Rectangle<T>& lhs, const Rectangle<T>& rhs){
	return Rectangle<T>(lhs.getX()+rhs.getX(), lhs.getY()+rhs.getY(), lhs.getWidth()+rhs.getWidth(), lhs.getHeight()+rhs.getHeight());
}


namespace Identifiers
{
	static const Identifier Category1("General");
	static const Identifier RecentFiles("RecentFiles");
	static const Identifier KeyMappings("KeyMappings");
	static const Identifier Languages("Languages");
	static const Identifier RecentSearches("RecentSearches");
	static const Identifier RecentReplaces("RecentReplaces");
	static const Identifier Value("Value");			//for common value properties
	static const Identifier AutoComplete("AutoComplete");
	static const Identifier BracketClosing("BracketClosing");
	static const Identifier AutoIndent("AutoIndent");
	static const Identifier TabSize("TabSize");
};

#endif
