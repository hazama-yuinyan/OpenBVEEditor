/*
  ==============================================================================

    CodeAssistant.cpp
    Created: 22 Apr 2011 11:28:50pm

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

#include "CodeAssistant.h"
#include "CommonHeader.h"



ContextInformation::ContextInformation(const String& StrToAdd) : str(StrToAdd), layout()
{
	AttributedString attributed_str(StrToAdd);
	Font font(Font::getDefaultMonospacedFontName(), 16.0f, Font::plain);
	attributed_str.setFont(font);
	layout.createLayout(attributed_str, 300);
	setSize(300, static_cast<int>(layout.getHeight()) + 14);
	addToDesktop(ComponentPeer::windowIsTemporary);
}

void ContextInformation::paint(Graphics& g)
{
	g.fillAll(Colours::lightyellow);

	g.setColour(Colours::black);
	g.drawRect(0, 0, getWidth(), getHeight());

	layout.draw(g, juce::Rectangle<float>(0, 0, static_cast<float>(getWidth()), static_cast<float>(getHeight())));
}

AssistantContents::AssistantContents(CodeEditorComponent& Editor, CodeAssistant* Assistant) : editor(Editor), assistant_impl(Assistant), last_selected_row(0)
{
	setModel(this);
	setFocusContainer(true);
	setSize(300, 150);
}

void AssistantContents::paintListBoxItem(int rowNumber, Graphics& g, int width, int height, bool rowIsSelected)
{
	if(rowIsSelected){
		g.fillAll(Colours::lightblue.withMultipliedAlpha(0.6f));
		if(rowNumber < context_infos.size()){
			const Rectangle<int> BOUNDS = getScreenBounds();
			context_infos[rowNumber]->setTopLeftPosition(BOUNDS.getRight(), BOUNDS.getY());
			context_infos[rowNumber]->setVisible(true);
		}
	}

	ContentProposal& content = proposals.getReference(rowNumber);
	if(content.HasImage()){
		g.drawImageWithin(*content.GetImage(), 0, 0, width/10, height, RectanglePlacement(RectanglePlacement::centred));
	}
	g.setFont(height*0.8f);
	g.drawText(content.GetStringInfo(), width/10, 0, width-width/10, height, Justification(Justification::centredLeft), true);
}

void AssistantContents::Apply(const int Row)
{
	CodeDocument::Position pos = editor.getCaretPos().movedBy(-1);
	String line = editor.getCaretPos().getLineText();
	const String REPLACING_STR = proposals[Row].GetStringInfo();
	const String REPLACING_STR_IN_CAPITAL = REPLACING_STR.toUpperCase(), REPLACING_STR_IN_LOWER = REPLACING_STR.toLowerCase();
	for(; pos.getPosition() != 0 && (REPLACING_STR_IN_CAPITAL.lastIndexOfChar(pos.getCharacter()) != -1 || REPLACING_STR_IN_LOWER.lastIndexOfChar(pos.getCharacter()) != -1); pos.moveBy(-1)){ }	//get back to the first character of the word that's been inputed
	if(pos.getPosition() != 0){pos.moveBy(1);}
	editor.moveCaretTo(pos, true);
	editor.insertTextAtCaret(REPLACING_STR);
	pos.moveBy(REPLACING_STR.length());
	editor.moveCaretTo(pos, false);

	dynamic_cast<AssistantWindow*>(getParentComponent())->setVisible(false);
}

void AssistantContents::listBoxItemClicked(int row, const MouseEvent&)
{
	Apply(row);
}

void AssistantContents::returnKeyPressed(int row)
{
	Apply(row);
}

void AssistantContents::selectedRowsChanged(int lastRowSelected)
{
	if(context_infos.size() == 0 || last_selected_row >= context_infos.size()){
		return;
	}

	if(lastRowSelected >= 0){
		context_infos[last_selected_row]->setVisible(false);
		context_infos[lastRowSelected]->addToDesktop(ComponentPeer::windowIsTemporary);
		last_selected_row = lastRowSelected;
	}
}

void AssistantContents::Update(const CodeDocument::Position& affectedTextStart, const CodeDocument::Position& affectedTextEnd)
{
	proposals.clear();
	context_infos.clear();
	assistant_impl->ComputeProposals(affectedTextStart, affectedTextEnd, proposals);
	assistant_impl->ComputeContexts(affectedTextStart, affectedTextEnd, context_infos);
	jassert(proposals.size() == context_infos.size());		//These two arrays must have the same number of elements
	updateContent();
}

StringPairArray AssistantContents::GetDocumentTypes(void)
{
	//You have to add file extensions and their corresponding type names.
	StringPairArray result;
	result.set(L"Text", L".txt");
	result.set(L"CSV(for BVE route file)", L".csv");
	result.set(L"HTML", L".html");
	result.set(L"Another version of HTML", L".htm");
	result.set(L"XML", L".xml");
	result.set(L"XML(for Adventure Game Engine script file)", L".xml");

	return result;
}

AssistantWindow::AssistantWindow(AssistantContents* Assistant) : DocumentWindow("Assistant Window", Colours::snow, 0, false), content(Assistant)
{
	setContentOwned(Assistant, true);
	setTitleBarHeight(0);
	setWantsKeyboardFocus(true);
}

void AssistantWindow::codeDocumentChanged(const CodeDocument::Position& affectedTextStart, const CodeDocument::Position& affectedTextEnd)
{
	if(!content->IsUsable()){return;}		//Since we don't have any content provider, we shouldn't proceed any further.
	content->Update(affectedTextStart, affectedTextEnd);
	Rectangle<int> caret_pos = content->editor.getCaretRectangle();
	setTopLeftPosition((caret_pos.getX()+getWidth() < content->editor.getWidth()) ? caret_pos.getX() : caret_pos.getX() - getWidth()
		, (caret_pos.getY()+caret_pos.getHeight()+getHeight() < content->editor.getHeight()) ? caret_pos.getY() + caret_pos.getHeight() : caret_pos.getY() - getHeight());	//position the window in the parent window
	if(content->proposals.size() != 0){
		content->selectRow(0);
		content->grabKeyboardFocus();
		setVisible(true);
	}
}

bool AssistantWindow::keyPressed(const KeyPress& key)
{
	if(key.isCurrentlyDown()){
		if(key.isKeyCode(KeyPress::upKey)){
			content->selectRow(jmax(content->getSelectedRow()-1, 0));
			return true;
		}else if(key.isKeyCode(KeyPress::downKey)){
			content->selectRow(jmin(content->getSelectedRow()+1, content->getNumRows()-1));
			return true;
		}else if(key.isKeyCode(KeyPress::returnKey)){
			content->returnKeyPressed(content->getSelectedRow());
			return true;
		}else{
			setVisible(false);
			content->editor.keyPressed(key);
			return true;
		}
	}

	return false;
}

void AssistantWindow::getAllCommands(Array<CommandID>& commands)
{
	commands.add(CommandIDs::AUTO_COMPLETE);
}

void AssistantWindow::getCommandInfo(CommandID commandID, ApplicationCommandInfo& result)
{
	switch(commandID){
	case CommandIDs::AUTO_COMPLETE:
		result.setInfo(TRANS("Auto Complete"), TRANS("Suggests some suitable strings"), CommandCategories::EDITTING, 0);
		result.addDefaultKeypress(KeyPress::spaceKey, ModifierKeys::ctrlModifier);
		break;
	/*case default:
		break;*/
	}
}

bool AssistantWindow::perform(const InvocationInfo& info)
{
	switch(info.commandID){
	case CommandIDs::AUTO_COMPLETE:
		codeDocumentChanged(content->editor.getCaretPos(), content->editor.getCaretPos());
		return true;

	/*case default:
		break;*/
	}

	return false;
}
