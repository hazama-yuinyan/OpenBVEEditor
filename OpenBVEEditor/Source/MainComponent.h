/*
  ==============================================================================

    MainComponent.h
    Created: 12 Apr 2011 8:46:18pm
    Author:  Ryouta

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


#ifndef __MAINCOMPONENT_H_3BCA74DF__
#define __MAINCOMPONENT_H_3BCA74DF__

#include "../JuceLibraryCode/JuceHeader.h"
#include "CommonHeader.h"


class CodeEditor;
class Preferences;
class SearchDialog;
class FindReplaceFunctionality;
class CodeAssistant;

/**
* This class holds the editors that are currently opened.
*/
class FlexibleDocumentComponent : public MultiDocumentPanel,
								  public ChangeBroadcaster
{
public:
	FlexibleDocumentComponent(void){};

	bool tryToCloseDocument(Component* Comp);
	void activeDocumentChanged(void);
};

/**
* The status bar. By default it shows the current caret position and a current state of some other components like The CodeEditor class, FindReplaceFunctionality class etc..
*/
class StatusBar : public Component,
				  public Timer,
				  public ChangeListener,
				  public ActionListener
{
private:
	Label caret_pos, msg;
	CodeEditorComponent* editor;
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (StatusBar);

public:
	StatusBar(CodeEditorComponent* Editor);
	~StatusBar(void);
	void timerCallback(void);
	void SetEditor(CodeEditorComponent* Editor){editor = Editor;}
	void changeListenerCallback(ChangeBroadcaster* source);
	void actionListenerCallback(const String& message);
	void paint(Graphics& g);
	void resized(void);
};

/**
* The main component that lives inside the main window.
*/
class MainComponent : public Component,
					  public ApplicationCommandTarget,
					  public MenuBarModel,
					  public Value::Listener,
					  public ActionBroadcaster
{
private:
	ScopedPointer<FlexibleDocumentComponent> doc_com;
	Toolbar tool_bar;
	TooltipWindow tool_tip;
	ScopedPointer<Preferences> preferences;
	ScopedPointer<SearchDialog> search_dialog;
	ScopedPointer<StatusBar> status_bar;
	ScopedPointer<FindReplaceFunctionality> find_func;
	Array<Value> token_colors;		//stores colors as strings
	bool is_quitting;

	class MainToolbarItemFactory : public ToolbarItemFactory
	{
	private:
		
		class CustomToolbarButton : public ToolbarButton
		{
		private:
			const int COMMAND;

		public:
			CustomToolbarButton(int ItemID, const String& LabelText, const String& Tooltip, const int CommandID, Drawable* NormalImage, Drawable* ToggledOnImage) : ToolbarButton(ItemID, LabelText, NormalImage, ToggledOnImage)
				, COMMAND(CommandID){
				setTooltip(Tooltip);
			}
			~CustomToolbarButton(void){};
			void clicked(void){
				command_manager->invokeDirectly(COMMAND, true);
			}
		};

		ToolbarButton* CreateToolBarButton(const int ItemID, const int CommandID, const String& Text, const String& Tooltip, const String& FileName)
		{
			File img_file = File::getSpecialLocation(File::currentApplicationFile).getParentDirectory().getChildFile(String("icons/")+FileName);
			Drawable* image = Drawable::createFromImageFile(img_file);
			return new CustomToolbarButton(ItemID, Text, Tooltip, CommandID, image, nullptr);
		}

		friend class MainComponent;

		class ToolbarSearchBox : public ToolbarItemComponent
		{
		private:
			friend class MainComponent;

			class SearchBox : public ComboBox,
							  public ValueTree::Listener
			{
			private:
				Array<Value> recent_searched_strings;

			public:
				SearchBox(void);
				~SearchBox(void){};

				bool keyPressed(const KeyPress& key);
				void valueChanged(Value&){recent_searched_strings.remove(0);}
				void valueTreePropertyChanged(ValueTree& treeWhosePropertyHasChanged, const Identifier& property);
				void valueTreeChildAdded(ValueTree& , ValueTree& ){};
				void valueTreeChildRemoved(ValueTree& , ValueTree& ){};
				void valueTreeChildOrderChanged(ValueTree& ){};
				void valueTreeParentChanged(ValueTree& ){};
			};

			SearchBox box;

		public:
			ToolbarSearchBox(const int ToolbarItemID) : ToolbarItemComponent(ToolbarItemID, "SearchBox", false), box()
			{
				box.setEditableText(true);
				addAndMakeVisible(&box);
				box.setTextWhenNothingSelected("Search");
			};

			bool getToolbarItemSizes(int, bool isToolbarVertical, int& preferredSize, int& minSize, int& maxSize)
			{
				if(isToolbarVertical){
					return false;
				}

				preferredSize = 200;
				minSize = 100;
				maxSize = 350;
				return true;
			}

			void paintButtonArea (Graphics&, int, int, bool, bool)
            {
            }

            void contentAreaChanged (const Rectangle<int>& contentArea)
            {
                box.setSize (contentArea.getWidth() - 2,
                                  jmin (contentArea.getHeight() - 2, 22));

                box.setCentrePosition (contentArea.getCentreX(), contentArea.getCentreY());
            }
		};

	public:
		MainToolbarItemFactory(void){};
		~MainToolbarItemFactory(void){};

		enum ToolbarItemIDs
		{
			NEW = 1, OPEN, SAVE, SAVE_AS, CUT, COPY, PASTE, SETTINGS, SEARCH_BOX
		};

		void getAllToolbarItemIds(Array<int>& ids)
		{
			ids.add(NEW);
			ids.add(OPEN);
			ids.add(SAVE);
			ids.add(SAVE_AS);
			ids.add(CUT);
			ids.add(COPY);
			ids.add(PASTE);
			ids.add(SETTINGS);
			ids.add(SEARCH_BOX);
			ids.add(separatorBarId);
			ids.add(spacerId);
		}

		void getDefaultItemSet(Array<int>& ids)
		{
			ids.add(NEW);
			ids.add(OPEN);
			ids.add(separatorBarId);
			ids.add(SAVE);
			ids.add(SAVE_AS);
			ids.add(separatorBarId);
			ids.add(CUT);
			ids.add(COPY);
			ids.add(PASTE);
			ids.add(separatorBarId);
			ids.add(spacerId);
			ids.add(SEARCH_BOX);
		}

		ToolbarItemComponent* createItem(int itemId)
		{
			switch(itemId)
			{
			case NEW:			return CreateToolBarButton(itemId, CommandIDs::CREATE_NEW_FILE, "new", TRANS("New file"), "075755-3d-glossy-blue-orb-icon-business-document5.png");
			case OPEN:			return CreateToolBarButton(itemId, CommandIDs::OPEN_FILE_VIA_DIALOG, "open", TRANS("Open"), "075693-3d-glossy-blue-orb-icon-business-box1.png");
			case SAVE:			return CreateToolBarButton(itemId, CommandIDs::SAVE_FILE, "save", TRANS("Save"), "075748-3d-glossy-blue-orb-icon-business-diskette-save.png");
			case SAVE_AS:		return CreateToolBarButton(itemId, CommandIDs::SAVE_FILE_AS, "save_as", TRANS("Save As"), "075754-3d-glossy-blue-orb-icon-business-document4.png");
			case CUT:			return CreateToolBarButton(itemId, StandardApplicationCommandIDs::cut, "cut", TRANS("Cut"), "059320-3d-glossy-blue-orb-icon-people-things-icon_003.png");
			case COPY:			return CreateToolBarButton(itemId, StandardApplicationCommandIDs::copy, "copy", TRANS("Copy"), "075845-3d-glossy-blue-orb-icon-business-paperclip.png");
			case PASTE:			return CreateToolBarButton(itemId, StandardApplicationCommandIDs::paste, "paste", TRANS("Paste"), "075714-3d-glossy-blue-orb-icon-business-clipboard2-sc1.png");
			case SETTINGS:		return CreateToolBarButton(itemId, CommandIDs::SHOW_PREFERENCE, "settings", TRANS("Settings"), "075794-3d-glossy-blue-orb-icon-business-gears1-sc44.png");
			case SEARCH_BOX:	return new ToolbarSearchBox(itemId);
			default:			break;
			}

			return nullptr;
		}
	};

	MainToolbarItemFactory factory;

	void SetProperties(const Identifier& Type, CodeTokeniser* Tokeniser);
	CodeTokeniser* CreateTokeniser(const String& FileExtension);
	CodeTokeniser* CreateTokeniser(const int SelectedIndex);
	CodeAssistant* CreateAssistant(const String& FileExtension);
	CodeAssistant* CreateAssistant(const int SelectedIndex);

public:
	MainComponent(void);
	~MainComponent(void){};

	const StringArray getMenuBarNames(void);
	const PopupMenu getMenuForIndex(int menuIndex, const String& );
	void menuItemSelected(int menuItemID, int);

	ApplicationCommandTarget* getNextCommandTarget()
    {
        // this will return the next parent component that is an ApplicationCommandTarget (in this
        // case, there probably isn't one, but it's best to use this method in your own apps).
        return findFirstTargetParentComponent();
    }
	void getAllCommands(Array<CommandID>& commands);
	void getCommandInfo(CommandID commandID, ApplicationCommandInfo& result);
	bool perform(const InvocationInfo& info);
	void valueChanged(Value& value);
	void resized(void);
	void CreateFile(void);
	void OpenFile(const File& File);
	void OpenFileViaDialog(void);
	bool CloseFile(Component* ClosingDoc);
	bool IsQuitting(void){return is_quitting;}
};



#endif  // __MAINCOMPONENT_H_3BCA74DF__
