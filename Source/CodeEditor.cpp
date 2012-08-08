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


#include "CodeEditor.h"
#include "CommonHeader.h"
#include "StoredSettings.h"
#include "SearchComponents.h"
#include "CodeAssistant.h"



LineNumbersDisplay::LineNumbersDisplay(CodeEditorComponent& Editor) : layout(), editor(Editor), last_start_line_num(-1)
{
	setSize(30, Editor.getHeight());
	startTimer(50);
}

LineNumbersDisplay::~LineNumbersDisplay(void)
{
	stopTimer();
}

void LineNumbersDisplay::timerCallback(void)
{
	const int START_LINE_NUM = editor.getPositionAt(1, 1).getLineNumber() + 1;
	if(START_LINE_NUM != last_start_line_num){		//If we are at different positions than before then update it.
		last_start_line_num = START_LINE_NUM;
		const int NUM_LINES = editor.getNumLinesOnScreen();
		String str;
		for(int i = 0; i < NUM_LINES; ++i){
			str += String(START_LINE_NUM + i) + String("\n");
		}

		layout.setText(str, editor.getFont());
		layout.layout(getWidth(), Justification::topRight, false);
		if(getWidth() < layout.getWidth()){
			setSize(layout.getWidth(), getHeight());
		}
		repaint();
	}
}

void LineNumbersDisplay::paint(Graphics& g)
{
	g.fillAll(Colours::antiquewhite);
	layout.drawWithin(g, 0, 0, getWidth(), getHeight(), Justification::topRight);
}

void LineNumbersDisplay::resized(void)
{
	last_start_line_num = -1;
}

CodeEditor::CodeEditor(const File& File, CodeDocument* Doc, CodeEditorComponent* Editor, CodeTokeniser* Tokeniser, AssistantWindow* Assistant) : file(File), doc(Doc), editor(Editor), tokeniser(Tokeniser), assistant(Assistant)
	, line_num(*Editor), do_auto_indent(var(false)), do_bracket_closing(var(false)), tab_width(var(4))
{
	addAndMakeVisible(editor);

	addAndMakeVisible(&line_num);

#if JUCE_MAC
    Font font (15.6f);
    font.setTypefaceName ("Menlo Regular");
#else
    Font font (15.0f);
	font.setTypefaceName (/*(StoredSettings::getInstance()->GetProperty(Identifiers::Category1).getProperty(Identifiers::Languages) == String("ja")) ? L"MS ゴシック" :*/ Font::getDefaultMonospacedFontName());
#endif
    editor->setFont (font);
	doc->addListener(this);
	addChildComponent(assistant);
	StoredSettings* settings = StoredSettings::getInstance();
	settings->RestoreProperty(Identifiers::Category1, Identifiers::BracketClosing, do_bracket_closing);
	settings->RestoreProperty(Identifiers::Category1, Identifiers::AutoIndent, do_auto_indent);
	settings->RestoreProperty(Identifiers::Category1, Identifiers::TabSize, tab_width);
	editor->setTabSize(tab_width.getValue(), true);
	layout_manager.setItemLayout(0, 20, 60, 30);
	layout_manager.setItemLayout(1, -0.9, -1.0, -0.95);
	setSize(800, 600);
}

CodeEditor::~CodeEditor()
{
}

void CodeEditor::codeDocumentChanged(const CodeDocument::Position& affectedTextStart, const CodeDocument::Position& affectedTextEnd)
{
	static const String BRACKETS[] = {String(L"("), String(L")"), String(L"{"), String(L"}"), String(L"["), String(L"]"), String(L"<"), String(L">"), String(L"'"), String(L"'"), String(L"\""), String(L"\"")};
	if(do_bracket_closing.getValue() && affectedTextEnd.getPosition() - affectedTextStart.getPosition() == 1){		//assume that the length of the affected text is 1
		//We're finding an opening character and inserting the other one. 
		const String C = String::charToString(affectedTextStart.getCharacter());
		int index = 0;
		for(; index < numElementsInArray(BRACKETS); index += 2){		//search for an existing parenthesis
			if(C == BRACKETS[index]){
				break;
			}
		}

		if(index < numElementsInArray(BRACKETS)){		//If the index is out of range, we couldn't find the first character
			editor->insertTextAtCaret(BRACKETS[index+1]);
			editor->moveCaretTo(affectedTextEnd, false);
		}
	}

	static const String LOOK_UP_CHARS[] = {String(L"{"), String(L"}")};

	if(do_auto_indent.getValue() && affectedTextStart.getLineNumber() < affectedTextEnd.getLineNumber()){		//assume that we're at the beginning of a line
		//calculate the number of tabs that will be inserted to the next line
		const String LINE_ABOVE = affectedTextStart.getLineText(), LINE = affectedTextEnd.getLineText();
		const int TAB_SIZE = editor->getTabSize();
		const int LENGTH_BEFORE = LINE_ABOVE.trimEnd().length();
		const int LENGTH_AFTER = LINE_ABOVE.trimStart().length();
		const String TARGET_STRINGS[] = {String::charToString(affectedTextStart.movedBy(-1).getCharacter()), String::charToString(affectedTextEnd.getCharacter())};
		int tab_size_to_add = (TARGET_STRINGS[0] == LOOK_UP_CHARS[0] && TARGET_STRINGS[1] != LOOK_UP_CHARS[1]) ? ((LENGTH_AFTER - LENGTH_BEFORE) / TAB_SIZE) + 1	//add another tab
							: (TARGET_STRINGS[1] == LOOK_UP_CHARS[1] && TARGET_STRINGS[0] != LOOK_UP_CHARS[0]) ? ((LENGTH_AFTER - LENGTH_BEFORE) / TAB_SIZE) - 1	//remove one tab
																											   : ((LENGTH_AFTER - LENGTH_BEFORE) / TAB_SIZE);

		for(; tab_size_to_add > 0; --tab_size_to_add){
			editor->insertTabAtCaret();
		}
	}
}

void CodeEditor::resized(void)
{
	Component* comp[] = {&line_num, editor};
	layout_manager.layOutComponents(comp, 2, 0, 0, getWidth(), getHeight(), false, true);
}

bool CodeEditor::Save(void)
{
	if(file == File::nonexistent){
		SaveAs();
	}else{
		if(!file.deleteFile() || !file.create()){		//first, delete the exisiting file
			return false;
		}

		CodeDocument& doc = editor->getDocument();
		ScopedPointer<FileOutputStream> stream = file.createOutputStream();
		if(!doc.writeToStream(*stream)){	//write out the data to a file
			return false;
		}

		doc.setSavePoint();			//set a new save point
	}
	return true;
}

bool CodeEditor::SaveAs(void)
{
	FileChooser chooser(TRANS("Specify a file to save to..."), File::getSpecialLocation(File::userDocumentsDirectory));
	if(chooser.browseForFileToSave(true)){
		file = chooser.getResult();		//set the doucument's file to the new one
		return Save();
	}

	return false;
}
