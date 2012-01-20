/*
  ==============================================================================

    SearchComponents.h
    Created: 18 Apr 2011 1:57:15am

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

#ifndef __SEARCH_H_C8050C21__
#define __SEARCH_H_C8050C21__


#include "../JuceLibraryCode/JuceHeader.h"
#undef T
#include <boost/xpressive/xpressive.hpp>


/**
* The class is doing the job and sending results to a CodeEditor or StatusBar.
*/
class FindReplaceFunctionality : public ActionBroadcaster
{
private:
	CodeEditorComponent* editor;
	boost::xpressive::wsregex_iterator cur_pos;
	std::wstring content;
	bool is_case_sensitive, use_regular_expr, is_changed;
	int cur_index, max_index;

	void Find(const String& SearchingText);
	void Replace(const String& ReplacingText);
	void RegexFind(const String& Pattern);

public:
	FindReplaceFunctionality(void);
	~FindReplaceFunctionality(void){};

	void SetEditor(CodeEditorComponent* Editor){editor = Editor;}
	void Execute(const String& Text, bool IsFind);
	void SetCaseSensitivity(bool IsCaseSensitive){is_case_sensitive = IsCaseSensitive;}
	bool GetCaseSensitivity(void) const{return is_case_sensitive;}
	void SetUseRegularExpr(bool UseRegularExpr){use_regular_expr = UseRegularExpr;}
	bool GetUseRegularExpr(void) const{return use_regular_expr;}
	void SetIsChanged(bool IsChanged){is_changed = IsChanged;}
};

/**
* Content component for SearchDialog
*/
class SearchComponent : public Component,
						public ValueTree::Listener,
						public Value::Listener
{
private:
	TabbedComponent tabbed;
	Array<Value> recently_searched, recently_replaced;
	ScopedPointer<FindReplaceFunctionality> impl;

	Array<Value>& GetRecentStrings(bool IsSearch){return (IsSearch) ? recently_searched : recently_replaced;}

	class SearchBox : public ComboBox
	{
	private:
		SearchComponent& comp;
		bool is_search_comp;

	public:
		SearchBox(const String& Name, SearchComponent& Comp, bool IsSearchComp) : ComboBox(Name), comp(Comp), is_search_comp(IsSearchComp){};
		~SearchBox(void){};

		bool keyPressed(const KeyPress& key);
	};

	class FindingComponent : public Component,
							 public ButtonListener
	{
	private:
		ScopedPointer<SearchBox> finding;
		TextButton apply_button;
		ToggleButton distinguish_button, regular_button;
		SearchComponent& comp;

	public:
		FindingComponent(SearchComponent& Comp);
		~FindingComponent(void){};
		void buttonClicked(Button* button);
		void resized(void);
	};

	class ReplacingComponent : public Component,
							   public ButtonListener
	{
	private:
		ScopedPointer<SearchBox> finding, replacing;
		TextButton find_button, apply_button, apply_all_button;
		ToggleButton distinguish_button, regular_button;
		SearchComponent& comp;

	public:
		ReplacingComponent(SearchComponent& Comp);
		~ReplacingComponent(void){};
		void buttonClicked(Button* button);
		void resized(void);
	};

public:
	SearchComponent(FindReplaceFunctionality* FindReplace);
	~SearchComponent(void){};
	void SetShowingTab(bool IsFindTab){(IsFindTab) ? tabbed.setCurrentTabIndex(0) : tabbed.setCurrentTabIndex(1);}
	FindReplaceFunctionality& GetFunctionality(void){return *impl;}
	void resized(void);
	void paint(Graphics& g);
	void valueChanged(Value& value);
	void valueTreePropertyChanged(ValueTree& treeWhosePropertyHasChanged, const Identifier& property);
	void valueTreeChildAdded(ValueTree& , ValueTree& ){};
	void valueTreeChildRemoved(ValueTree& , ValueTree& ){};
	void valueTreeChildOrderChanged(ValueTree& ){};
	void valueTreeParentChanged(ValueTree& ){};
};

class FlexibleDocumentComponent;

/**
* Just an ordinary window designed for Find or Replace Functionalities
*/
class SearchDialog : public DocumentWindow
{
private:
	FlexibleDocumentComponent& doc_comp;

public:
	SearchDialog(FlexibleDocumentComponent& DocComp, FindReplaceFunctionality* FindReplace);
	~SearchDialog(void){};
	void closeButtonPressed(void);
	void SetShowingTab(bool ShowingFind);
	void activeWindowStatusChanged(void);
};



#endif  // __SEARCH_H_C8050C21__
