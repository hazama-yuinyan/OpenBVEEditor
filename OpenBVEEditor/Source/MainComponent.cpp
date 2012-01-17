/*
  ==============================================================================

    MainComponent.cpp
    Created: 12 Apr 2011 8:46:19pm
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


#include "MainComponent.h"
#include "CodeEditor.h"
#include "BVECode.h"
#include "CommonHeader.h"
#include "StoredSettings.h"
#include "Preferences.h"
#include "SearchComponents.h"


bool FlexibleDocumentComponent::tryToCloseDocument(Component* Comp)
{
	CodeEditor* editor = dynamic_cast<CodeEditor*>(Comp);
	return editor->IsSaved();
}

void FlexibleDocumentComponent::activeDocumentChanged(void)
{
	//inform the StatusBar that active document has changed
	sendChangeMessage();
}

StatusBar::StatusBar(CodeEditorComponent* Editor) : caret_pos("caret_pos", TRANS("line:")+String(1)+TRANS(" pos:")+String(1)+TRANS(" total lines:")), editor(Editor)
{
	addAndMakeVisible(&caret_pos);
	addAndMakeVisible(&msg);
	startTimer(25);
}

StatusBar::~StatusBar(void)
{
	stopTimer();
}

void StatusBar::timerCallback(void)
{
	if(editor->hasKeyboardFocus(true)){
		CodeDocument::Position pos = editor->getCaretPos();
		caret_pos.setText(TRANS("line:")+String(pos.getLineNumber()+1)+TRANS(" pos:")+String(pos.getIndexInLine()+1)+TRANS(" total lines:")+String(editor->getDocument().getNumLines()), false);
		repaint();
	}
}

void StatusBar::paint(Graphics& g)
{
	g.fillAll(Colours::antiquewhite);
	g.setColour(Colours::grey);
	const float X = static_cast<float>(caret_pos.getWidth());
	g.drawLine(X, 0.0f, X, static_cast<float>(getHeight()), 3.0f);
}

void StatusBar::resized(void)
{
	caret_pos.setBoundsRelative(0.0f, 0.0f, 0.3f, 1.0f);
	msg.setBoundsRelative(0.3f, 0.0f, 0.7f, 1.0f);
}

void StatusBar::changeListenerCallback(ChangeBroadcaster* source)
{
	auto comp = dynamic_cast<CodeEditor*>(dynamic_cast<FlexibleDocumentComponent*>(source)->getActiveDocument());
	if(comp != nullptr){
		editor = comp->GetEditorComponent();
	}
}

void StatusBar::actionListenerCallback(const String& message)
{
	msg.setText(message, false);
	repaint();
}

MainComponent::MainToolbarItemFactory::ToolbarSearchBox::SearchBox::SearchBox(void): ComboBox("search_box")
{
	StoredSettings::getInstance()->GetProperty(Identifiers::Category1).addListener(this);
}

bool MainComponent::MainToolbarItemFactory::ToolbarSearchBox::SearchBox::keyPressed(const KeyPress& key)
{
	if(key.isCurrentlyDown() && key.getModifiers() == ModifierKeys()){
		if(key.isKeyCode(KeyPress::returnKey)){
			//After adding the text in the ComboBox to the input history, it'll do the work.
			//テキストボックスのテキストを入力履歴に追加してから、実際に検索を行う
			const String TEXT = getText();
			StoredSettings::getInstance()->AddRecentString(Identifiers::RecentSearches, TEXT);
			ApplicationCommandTarget::InvocationInfo info(CommandIDs::HANDY_SEARCH);
			info.invocationMethod = ApplicationCommandTarget::InvocationInfo::direct;
			info.originatingComponent = this;
			command_manager->invoke(info, true);
		}else{
			//searches the input history for texts similar to that in the text box and adds it to the ComboBox
			//入力履歴から、テキストボックスにすでに入力されている文字列と合致するものをコンボボックスに追加する
			String new_text = getText();
			int size = recent_searched_strings.size();
			for(; size > 0; --size){
				if(recent_searched_strings[size-1].toString().startsWithIgnoreCase(new_text)){
					addItem(recent_searched_strings[size-1].toString(), getNumItems());
				}
			}
			if(getNumItems() > 0){
				showPopup();
			}
		}

		return true;
	}

	return false;
}

void MainComponent::MainToolbarItemFactory::ToolbarSearchBox::SearchBox::valueTreePropertyChanged(ValueTree& treeWhosePropertyHasChanged, const Identifier& property)
{
	if(treeWhosePropertyHasChanged.hasType(Identifiers::RecentSearches)){
		Value tmp = treeWhosePropertyHasChanged.getPropertyAsValue(property, nullptr);
		tmp.addListener(this);
		recent_searched_strings.add(tmp);
	}
}

MainComponent::MainComponent(void) : doc_com(new FlexibleDocumentComponent()), tool_bar(), preferences(new Preferences()), find_func(new FindReplaceFunctionality())
{
	doc_com->setLayoutMode(MultiDocumentPanel::MaximisedWindowsWithTabs);
	addAndMakeVisible(&tool_bar);
	tool_bar.addDefaultItems(factory);

	addAndMakeVisible(doc_com);

	const String COMMAND_LINE = JUCEApplication::getInstance()->getCommandLineParameters();
	if(COMMAND_LINE.isNotEmpty()){
		String tmp = COMMAND_LINE.fromLastOccurrenceOf("/", false, false);
		OpenFile(File::getSpecialLocation(File::currentApplicationFile).getParentDirectory().getChildFile(tmp));
	}else{
		CreateFile();
	}

	status_bar = new StatusBar(dynamic_cast<CodeEditor*>(doc_com->getActiveDocument())->GetEditorComponent());
	status_bar->setSize(getWidth(), 40);
	doc_com->addChangeListener(status_bar);
	addAndMakeVisible(status_bar);

	auto find_replace = new FindReplaceFunctionality();
	find_replace->addActionListener(status_bar);
	search_dialog = new SearchDialog(*doc_com, find_replace);

	find_func->addActionListener(status_bar);

	setSize(1024, 768);
}

void MainComponent::SetProperties(const Identifier& Type, CodeTokeniser* Tokeniser)
{
	const StringArray TOKEN_TYPES = Tokeniser->getTokenTypes();
	StoredSettings* settings = StoredSettings::getInstance();
	for(int i = 0; i < TOKEN_TYPES.size(); ++i){
		Value tmp(var(Tokeniser->getDefaultColour(i).toString()));
		tmp.addListener(this);
		token_colors.add(tmp);
		settings->RestoreProperty(Type, Identifier(TOKEN_TYPES[i]), tmp);
	}
}

const StringArray MainComponent::getMenuBarNames(void)
{
	StringArray names;
	names.add(TRANS("File"));
	names.add(TRANS("Edit"));
	names.add(TRANS("Search"));
	names.add(TRANS("View"));
	
	return names;
}

const PopupMenu MainComponent::getMenuForIndex(int menuIndex, const String& )
{
	PopupMenu menu;
	if(menuIndex == 0){
		menu.addCommandItem(command_manager, CommandIDs::CREATE_NEW_FILE, TRANS("New file")+String("..."));
		menu.addCommandItem(command_manager, CommandIDs::OPEN_FILE_VIA_DIALOG, TRANS("Open")+String("..."));
		menu.addSeparator();

		menu.addCommandItem(command_manager, CommandIDs::CLOSE_FILE, TRANS("Close"));
		menu.addSeparator();

		menu.addCommandItem(command_manager, CommandIDs::SAVE_FILE, TRANS("Save"));
		menu.addCommandItem(command_manager, CommandIDs::SAVE_FILE_AS, TRANS("Save As")+String("..."));
		menu.addSeparator();

		PopupMenu sub_menu;
		StoredSettings::getInstance()->list.createPopupMenuItems(sub_menu, 1, false, true);
		menu.addSubMenu(TRANS("Open recent file")+String("..."), sub_menu);
		menu.addSeparator();

		menu.addCommandItem(command_manager, StandardApplicationCommandIDs::quit, TRANS("Quit"));
	}else if(menuIndex == 1){
		menu.addCommandItem(command_manager, CommandIDs::REDO, TRANS("Redo"));
		menu.addCommandItem(command_manager, CommandIDs::UNDO, TRANS("Undo"));
		menu.addSeparator();

		menu.addCommandItem(command_manager, StandardApplicationCommandIDs::cut, TRANS("Cut"));
		menu.addCommandItem(command_manager, StandardApplicationCommandIDs::copy, TRANS("Copy"));
		menu.addCommandItem(command_manager, StandardApplicationCommandIDs::paste, TRANS("Paste"));
		menu.addCommandItem(command_manager, StandardApplicationCommandIDs::del, TRANS("Delete"));
		menu.addSeparator();

		menu.addCommandItem(command_manager, StandardApplicationCommandIDs::selectAll, TRANS("Select All"));
		menu.addSeparator();

		menu.addCommandItem(command_manager, CommandIDs::AUTO_COMPLETE, TRANS("Auto Complete"));
	}else if(menuIndex == 2){
		menu.addCommandItem(command_manager, CommandIDs::FIND, TRANS("Find")+String("..."));
		menu.addSeparator();

		menu.addCommandItem(command_manager, CommandIDs::REPLACE, TRANS("Replace")+String("..."));
	}else if(menuIndex == 3){
		menu.addCommandItem(command_manager, CommandIDs::SHOW_PREFERENCE, TRANS("Settings")+String("..."));
	}

	return menu;
}

void MainComponent::menuItemSelected(int menuItemID, int)
{
	if(1 <= menuItemID && menuItemID < StoredSettings::getInstance()->list.getMaxNumberOfItems()){
		OpenFile(StoredSettings::getInstance()->list.getFile(menuItemID-1));
	}
}

void MainComponent::getAllCommands(Array<CommandID>& commands)
{
	const CommandID ids[] = {
		CommandIDs::CREATE_NEW_FILE,
		CommandIDs::CLOSE_FILE,
		CommandIDs::CLOSE_ALL_FILES,
		CommandIDs::OPEN_FILE_VIA_DIALOG,
		CommandIDs::SAVE_FILE,
		CommandIDs::SAVE_FILE_AS,
		CommandIDs::SHOW_PREFERENCE,
		CommandIDs::FIND,
		CommandIDs::HANDY_SEARCH,
		CommandIDs::REPLACE,
		StandardApplicationCommandIDs::cut,
		StandardApplicationCommandIDs::copy,
		StandardApplicationCommandIDs::paste,
		StandardApplicationCommandIDs::selectAll,
		CommandIDs::REDO,
		CommandIDs::UNDO
	};

	commands.addArray(ids, numElementsInArray(ids));
}

void MainComponent::getCommandInfo(CommandID commandID, ApplicationCommandInfo& result)
{
	switch(commandID){
	case CommandIDs::CREATE_NEW_FILE:
		result.setInfo(TRANS("New file"), TRANS("Creates a new file"), CommandCategories::GENERAL, 0);
		result.addDefaultKeypress('N', ModifierKeys::commandModifier);
		break;
	case CommandIDs::CLOSE_FILE:
		result.setInfo(TRANS("Close"), TRANS("Closes the current file"), CommandCategories::GENERAL, 0);
		result.addDefaultKeypress('W', ModifierKeys::commandModifier);
		break;
	case CommandIDs::CLOSE_ALL_FILES:
		result.setInfo(TRANS("Close all"), TRANS("Closes all the documents"), CommandCategories::GENERAL, ApplicationCommandInfo::hiddenFromKeyEditor);
		break;
	case CommandIDs::OPEN_FILE_VIA_DIALOG:
		result.setInfo(TRANS("Open"), TRANS("Opens a file with a dialog"), CommandCategories::GENERAL, 0);
		result.addDefaultKeypress('O', ModifierKeys::commandModifier);
		break;
	case CommandIDs::SAVE_FILE:
		result.setInfo(TRANS("Save"), TRANS("Saves the current document to the same file"), CommandCategories::GENERAL, 0);
		result.addDefaultKeypress('S', ModifierKeys::commandModifier);
		break;
	case CommandIDs::SAVE_FILE_AS:
		result.setInfo(TRANS("Save As"), TRANS("Creates a new file and saves the current document to that file"), CommandCategories::GENERAL, 0);
		break;
	case CommandIDs::SHOW_PREFERENCE:
		result.setInfo(TRANS("Settings"), TRANS("Pops up the settings window"), CommandCategories::GENERAL, 0);
		break;
	case CommandIDs::FIND:
		result.setInfo(TRANS("Find"), TRANS("Finds a certain string"), CommandCategories::GENERAL, 0);
		result.addDefaultKeypress('F', ModifierKeys::commandModifier);
		break;
	case CommandIDs::HANDY_SEARCH:
		result.setInfo("Handy Search", TRANS("Finds a certain string"), CommandCategories::GENERAL, ApplicationCommandInfo::hiddenFromKeyEditor);
		break;
	case CommandIDs::REPLACE:
		result.setInfo(TRANS("Replace"), TRANS("Replaces a certain string with another one"), CommandCategories::EDITTING, 0);
		result.addDefaultKeypress('P', ModifierKeys::commandModifier);
		break;
	case StandardApplicationCommandIDs::cut:
		result.setInfo(TRANS("Cut"), TRANS("Copys and cuts some strings"), CommandCategories::EDITTING, 0);
		result.addDefaultKeypress('X', ModifierKeys::commandModifier);
		break;
	case StandardApplicationCommandIDs::copy:
		result.setInfo(TRANS("Copy"), TRANS("Copys some strings"), CommandCategories::EDITTING, 0);
		result.addDefaultKeypress('C', ModifierKeys::commandModifier);
		break;
	case StandardApplicationCommandIDs::paste:
		result.setInfo(TRANS("Paste"), TRANS("Pastes the strings that you copied or cut before"), CommandCategories::EDITTING, 0);
		result.addDefaultKeypress('V', ModifierKeys::commandModifier);
		break;
	case StandardApplicationCommandIDs::selectAll:
		result.setInfo(TRANS("Select All"), TRANS("Selects all the strings in the document"), CommandCategories::GENERAL, 0);
		result.addDefaultKeypress('A', ModifierKeys::commandModifier);
		break;
	case CommandIDs::UNDO:
		result.setInfo(TRANS("Undo"), TRANS("Undo the last operation"), CommandCategories::EDITTING, 0);
		result.addDefaultKeypress('Z', ModifierKeys::commandModifier);
		break;
	case CommandIDs::REDO:
		result.setInfo(TRANS("Redo"), TRANS("Redo the last operation"), CommandCategories::EDITTING, 0);
		result.addDefaultKeypress('Y', ModifierKeys::commandModifier);
		break;
	default:
		break;
	}
}

bool MainComponent::perform(const InvocationInfo& info)
{
	CodeEditor* editor = dynamic_cast<CodeEditor*>(doc_com->getActiveDocument());
	CodeEditorComponent* editor_comp = editor->GetEditorComponent();

	switch(info.commandID){
	case CommandIDs::CREATE_NEW_FILE:
		CreateFile();
		return true;

	case CommandIDs::CLOSE_FILE:
		CloseFile(doc_com->getActiveDocument());
		return true;

	case CommandIDs::CLOSE_ALL_FILES:
		{
			int num_docs_opened = doc_com->getNumDocuments();
			for(; num_docs_opened > 0; --num_docs_opened){
				if(!CloseFile(doc_com->getDocument(num_docs_opened - 1))){
					is_quitting = false;
					return true;
				}
			}
			is_quitting = true;
			return true;
		}

	case CommandIDs::OPEN_FILE_VIA_DIALOG:
		OpenFileViaDialog();
		return true;

	case CommandIDs::SAVE_FILE:
		editor->Save();
		return true;

	case CommandIDs::SAVE_FILE_AS:
		editor->SaveAs();
		return true;

	case CommandIDs::SHOW_PREFERENCE:
		preferences->ShowInDialogBox(600, 400, Colours::lightgrey);
		return true;

	case CommandIDs::FIND:
		if(!search_dialog->isVisible()){
			const Rectangle<int> DESKTOP_SIZE = Desktop::getInstance().getMainMonitorArea();
			search_dialog->setCentrePosition(DESKTOP_SIZE.getWidth()*3/4, DESKTOP_SIZE.getHeight()/3);
			search_dialog->setVisible(true);
		}
		search_dialog->SetShowingTab(true);
		return true;

	case CommandIDs::REPLACE:
		if(!search_dialog->isVisible()){
			const Rectangle<int> DESKTOP_SIZE = Desktop::getInstance().getMainMonitorArea();
			search_dialog->setCentrePosition(DESKTOP_SIZE.getWidth()*3/4, DESKTOP_SIZE.getHeight()/3);
			search_dialog->setVisible(true);
		}
		search_dialog->SetShowingTab(false);
		return true;

	case CommandIDs::HANDY_SEARCH:
		{
			typedef MainComponent::MainToolbarItemFactory::ToolbarSearchBox::SearchBox ToolBarBox;
			ToolBarBox* box = dynamic_cast<ToolBarBox*>(info.originatingComponent);
			if(box){
				const String TEXT = box->getText();
				find_func->SetIsChanged(true);
				find_func->Execute(TEXT, true);
			}
			return true;
		}

	case StandardApplicationCommandIDs::cut:
		editor_comp->cutToClipboard();
		return true;

	case StandardApplicationCommandIDs::copy:
		editor_comp->copyToClipboard();
		return true;

	case StandardApplicationCommandIDs::paste:
		editor_comp->pasteFromClipboard();
		return true;

	case StandardApplicationCommandIDs::selectAll:
		editor_comp->selectAll();
		return true;

	case CommandIDs::UNDO:
		editor_comp->undo();
		return true;

	case CommandIDs::REDO:
		editor_comp->redo();
		return true;

	default:
		break;
	}

	return false;
}

void MainComponent::valueChanged(Value& value)
{
	//change one of the token colors
	int size = token_colors.size();
	for(; size > 0 && token_colors[size-1].refersToSameSourceAs(value); --size){ }
	CodeEditor* editor = dynamic_cast<CodeEditor*>(doc_com->getActiveDocument());
	editor->GetEditorComponent()->setColourForTokenType(size-1, Colour::fromString(value.toString()));
}

void MainComponent::resized(void)
{
	tool_bar.setBoundsRelative(0.0f, 0.0f, 1.0f, 0.1f);
	doc_com->setBoundsRelative(0.0f, 0.1f, 1.0f, 0.85f);
	status_bar->setBoundsRelative(0.0f, 0.95f, 1.0f, 0.05f);
}

CodeTokeniser* MainComponent::CreateTokeniser(const String& FileExtension)
{
	//create a tokenizer depending on the extension of the file
	return (FileExtension == String(".csv")) ? new BVETokenizer(1000) : nullptr;
}

CodeTokeniser* MainComponent::CreateTokeniser(const int SelectedIndex)
{
	//create a tokenizer depeding on the selected item index of the new file wizard
	return (SelectedIndex == 1) ? new BVETokenizer(1000) : nullptr;
}

CodeAssistant* MainComponent::CreateAssistant(const String& FileExtension)
{
	//create a CodeAssistant depending on the extension of the file
	return (FileExtension == String(".csv")) ? new BVECodeAssistant() : nullptr;
}

CodeAssistant* MainComponent::CreateAssistant(const int SelectedIndex)
{
	//create a CodeAssistant depending on the selected item index of the new file wizard
	return (SelectedIndex == 1) ? new BVECodeAssistant() : nullptr;
}

void MainComponent::CreateFile(void)
{
	/**
	* With this class, you can specify the type of the doc when creating a new file from scratch
	*/
	class DocumentTypeChooser : public ListBox,
								public ListBoxModel
	{
	public:
		StringArray doc_types;

		DocumentTypeChooser(const StringArray& DocTypes) : doc_types(DocTypes), ListBox("DocumentTypeChooser")
		{
			setModel(this);
			selectRow(0);
			setMouseMoveSelectsRows(true);
			setSize(500, 400);
		};
		~DocumentTypeChooser(void){};

		int getNumRows(void){return doc_types.size();}
		void paintListBoxItem(int rowNumber, Graphics& g, int width, int height, bool rowIsSelected)
		{
			const String& TEXT = doc_types.getReference(rowNumber);
			if(rowIsSelected){
				g.fillAll(Colours::antiquewhite);
			}

			g.setFont(Font(Font::getDefaultSansSerifFontName(), height*0.8f, Font::bold));
			g.setColour(Colours::black);
			g.drawText(TEXT, 0, 0, width, height, Justification::centredLeft, true);
		};
		void mouseDoubleClick(const MouseEvent& e)
		{
			const Point<int> DOWN_POINT = e.getEventRelativeTo(this).getMouseDownPosition();
			const int SELECTED_INDEX = getRowContainingPosition(DOWN_POINT.getX(), DOWN_POINT.getY());
			DialogWindow* dw = findParentComponentOfClass((DialogWindow*)0);
			if(dw){
				dw->exitModalState(SELECTED_INDEX);
			}
		};
	};

	//ask the user what type he or she wants the doc to be of
	StringPairArray types = AssistantContents::GetDocumentTypes();
	DocumentTypeChooser chooser(types.getAllKeys());
	const int RESULT = DialogWindow::showModalDialog(TRANS("Double click on an item and the new doc is of that type"), &chooser, nullptr, Colours::brown, false);
	if(RESULT < 0){
		sendActionMessage(TRANS("You have dismissed the creation of a new file..."));
		return;
	}

	CodeTokeniser* tokeniser = CreateTokeniser(RESULT);
	CodeDocument* doc = new CodeDocument();
	CodeEditorComponent* editor_comp = new CodeEditorComponent(*doc, tokeniser);
	editor_comp->loadContent("\0xfffx");
	AssistantWindow* assistant = new AssistantWindow(new AssistantContents(*editor_comp, CreateAssistant(RESULT)));
	doc->addListener(assistant);
	CodeEditor* editor = new CodeEditor(File(), doc, editor_comp, tokeniser, assistant);
	editor->setName(String("New File")+types.getAllValues()[RESULT]);
	doc_com->addDocument(editor, Colour(255, 255, 255), true);
}

void MainComponent::OpenFile(const File& File)
{
	CodeDocument* doc = new CodeDocument();
	const String CONTENT = File.loadFileAsString();
	CodeTokeniser* tokeniser = CreateTokeniser(File.getFileExtension());
	CodeEditorComponent* editor_comp =  new CodeEditorComponent(*doc, tokeniser);
	editor_comp->loadContent(CONTENT);
	AssistantWindow* assistant = new AssistantWindow(new AssistantContents(*editor_comp, CreateAssistant(File.getFileExtension())));
	doc->addListener(assistant);
	CodeEditor* editor = new CodeEditor(File, doc, editor_comp, tokeniser, assistant);
	editor->setName(File.getFileName());
	doc_com->addDocument(editor, Colour(255, 255, 255), true);
	StoredSettings::getInstance()->list.addFile(File);
}

void MainComponent::OpenFileViaDialog(void)
{
	//open a file with a dialog box
	const String EXTS("*.csv;*.txt;*.htm;*.html");
	FileChooser chooser(TRANS("Choose a file to open..."), File::getSpecialLocation(File::userDocumentsDirectory), EXTS);
	if(chooser.browseForFileToOpen()){
		File file = chooser.getResult();
		OpenFile(file);
	}
}

bool MainComponent::CloseFile(Component* ClosingDoc)
{
	if(!doc_com->tryToCloseDocument(ClosingDoc)){				//If the document has been changed then ask the user first whether we should save it or not.
		const int RESULT = AlertWindow::showYesNoCancelBox(AlertWindow::QuestionIcon, TRANS("About to close...")
			, TRANS("You are about to close the document.\n Would you like to save it?"));

		switch(RESULT){
		case 0:
			return false;		//With cancel button clicked, we will return immediately
		case 1:
			dynamic_cast<CodeEditor*>(ClosingDoc)->Save();		//The user agrees and we'll save it
			break;
		case 2:
		default:
			break;				//The user discards it and continue..
		}
	}

	doc_com->closeDocument(ClosingDoc, false);		//finally close the document
	return true;
}
