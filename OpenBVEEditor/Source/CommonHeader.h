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
