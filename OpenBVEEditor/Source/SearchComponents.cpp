/*
  ==============================================================================

    SearchComponents.cpp
    Created: 18 Apr 2011 1:57:15am
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


#include "SearchComponents.h"
#include "StoredSettings.h"
#include "CommonHeader.h"
#include "MainComponent.h"
#include "CodeEditor.h"


FindReplaceFunctionality::FindReplaceFunctionality(void) : editor(nullptr), is_case_sensitive(false), use_regular_expr(false), is_changed(false), cur_index(1)
{
}

void FindReplaceFunctionality::Find(const String& SearchingText)
{
	if(is_changed){
		CodeDocument& doc = editor->getDocument();
		const String CONTENT = doc.getAllContent();
		content = std::wstring(CONTENT.toWideCharPointer());
		boost::xpressive::wsregex text = boost::xpressive::wsregex::compile(SearchingText.toWideCharPointer());
		cur_pos = boost::xpressive::wsregex_iterator(content.begin(), content.end(), text, boost::xpressive::regex_constants::format_literal);
		max_index = (cur_pos != boost::xpressive::wsregex_iterator()) ? cur_pos->size() : 0;
		is_changed = false;
	}

	boost::xpressive::wsregex_iterator end;
	if(cur_pos == end){
		if(cur_index == 1){
			//signal the StatusBar that there aren't such strings
			//����������Ȃ������̂ŁA�X�e�[�^�X�o�[�ɕ\�����镶����𑗐M����
			sendActionMessage(TRANS("No matches found."));
		}
		//Because we've reached the end, set the component to search at the next time this method is called.
		//�Ō�ɓ��B�����̂ŁA�ēx�������s���悤�Ƀt���O�𗧂ĂĂ���
		is_changed = true;
		cur_index = 1;
		return;
	}

	CodeDocument::Position current_pos(&editor->getDocument(), cur_pos->position());
	editor->moveCaretTo(current_pos, false);
	editor->moveCaretTo(current_pos.movedBy(cur_pos->length()), true);
	editor->repaint();
	sendActionMessage(String(cur_index)+TRANS("out of")+String(max_index)+TRANS("matches"));
	++cur_pos; ++cur_index;
}

void FindReplaceFunctionality::Replace(const String& ReplacingText)
{
	editor->insertTextAtCaret(ReplacingText);
}

void FindReplaceFunctionality::RegexFind(const String& Pattern)
{
	if(is_changed){
		CodeDocument& doc = editor->getDocument();
		const String CONTENT = doc.getAllContent();
		content = std::wstring(CONTENT.toWideCharPointer());
		boost::xpressive::wsregex regex = boost::xpressive::wsregex::compile(Pattern.toWideCharPointer());
		cur_pos = boost::xpressive::wsregex_iterator(content.begin(), content.end(), regex);
		max_index = (cur_pos != boost::xpressive::wsregex_iterator()) ? cur_pos->size() : 0;
		is_changed = false;
	}

	boost::xpressive::wsregex_iterator end;
	if(cur_pos == end){
		if(cur_index == 1){
			//signal the StatusBar that there aren't such strings
			//����������Ȃ������̂ŁA�X�e�[�^�X�o�[�ɕ\�����镶����𑗐M����
			sendActionMessage(TRANS("No matches found."));
		}
		//Because we've reached the end, set the component to search at the next time this method is called.
		//�Ō�ɓ��B�����̂ŁA�ēx�������s���悤�Ƀt���O�𗧂ĂĂ���
		is_changed = true;
		cur_index = 1;
		return;
	}

	CodeDocument::Position current_pos(&editor->getDocument(), cur_pos->position());
	editor->moveCaretTo(current_pos, false);
	editor->moveCaretTo(current_pos.movedBy(cur_pos->length()), true);
	editor->repaint();
	sendActionMessage(String(cur_index)+TRANS("out of")+String(max_index)+TRANS("matches"));
	++cur_pos; ++cur_index;
}

void FindReplaceFunctionality::Execute(const String& Text, bool IsFind)
{
	(!IsFind) ? Replace(Text) :
	(use_regular_expr) ? RegexFind(Text) : Find(Text);
}

SearchComponent::SearchComponent(FindReplaceFunctionality* FindReplace) : tabbed(TabbedButtonBar::TabsAtTop), impl(FindReplace)
{
	StoredSettings* settings = StoredSettings::getInstance();
	settings->GetProperty(Identifiers::Category1).addListener(this);
	tabbed.addTab(TRANS("Find"), Colours::aqua, new FindingComponent(*this), true);
	tabbed.addTab(TRANS("Replace"), Colours::limegreen, new ReplacingComponent(*this), true);
	addAndMakeVisible(&tabbed);
	setSize(600, 400);
}

void SearchComponent::resized(void)
{
	tabbed.setBoundsInset(BorderSize<int>(0));
}

void SearchComponent::paint(Graphics& g)
{
	g.fillAll(Colours::lightgrey);
}

SearchComponent::FindingComponent::FindingComponent(SearchComponent& Comp) : finding("finding_text", Comp, true), apply_button(TRANS("Find"), TRANS("Find the string")), comp(Comp), distinguish_button(TRANS("Case sensitive"))
	, regular_button(TRANS("Regular expressions"))
{
	finding.setEditableText(true);
	addAndMakeVisible(&finding);
	finding.showEditor();

	apply_button.addListener(this);

	distinguish_button.setToggleState(comp.impl->GetCaseSensitivity(), false);
	distinguish_button.addListener(this);

	regular_button.setToggleState(comp.impl->GetUseRegularExpr(), false);
	regular_button.addListener(this);

	addAndMakeVisible(&apply_button);
	addAndMakeVisible(&distinguish_button);
	addAndMakeVisible(&regular_button);

	setSize(600, 400);
}

void SearchComponent::FindingComponent::buttonClicked(Button* button)
{
	if(button == &apply_button){
		//After adding the text in the ComboBox to the input history, it'll do the work.
		//�e�L�X�g�{�b�N�X�̃e�L�X�g����͗����ɒǉ����Ă���A���ۂɌ������s��
		const String TEXT = finding.getText();
		StoredSettings::getInstance()->AddRecentString(Identifiers::RecentSearches, TEXT);
		comp.impl->SetIsChanged(true);
		comp.impl->Execute(TEXT, true);
	}else if(button == &distinguish_button){
		comp.impl->SetCaseSensitivity(distinguish_button.getToggleState());
	}else if(button == &regular_button){
		comp.impl->SetUseRegularExpr(regular_button.getToggleState());
	}
}

void SearchComponent::FindingComponent::resized(void)
{
	finding.setBoundsRelative(0.05f, 0.05f, 0.8f, 0.1f);
	distinguish_button.setBoundsRelative(0.05f, 0.3f, 0.5f, 0.05f);
	regular_button.setBoundsRelative(0.05f, 0.4f, 0.5f, 0.05f);
	apply_button.setBoundsRelative(0.8f, 0.8f, 0.15f, 0.1f);
}

SearchComponent::ReplacingComponent::ReplacingComponent(SearchComponent& Comp) : finding("finding_text", Comp, true), replacing("replacing_text", Comp, false), find_button(TRANS("Find"), TRANS("Find the string"))
	, apply_button(TRANS("Replace"), TRANS("Replace the string")), apply_all_button(TRANS("Apply to all"), TRANS("Replace each string")), comp(Comp)
	, distinguish_button(TRANS("Case sensitive")), regular_button(TRANS("Regular expressions"))
{
	finding.setEditableText(true);
	addAndMakeVisible(&finding);
	replacing.setEditableText(true);
	addAndMakeVisible(&replacing);
	finding.showEditor();

	find_button.addListener(this);

	apply_button.addListener(this);

	apply_all_button.addListener(this);

	addAndMakeVisible(&find_button);
	addAndMakeVisible(&apply_button);
	addAndMakeVisible(&apply_all_button);

	distinguish_button.setToggleState(comp.impl->GetCaseSensitivity(), false);
	distinguish_button.addListener(this);

	regular_button.setToggleState(comp.impl->GetUseRegularExpr(), false);
	regular_button.addListener(this);

	addAndMakeVisible(&distinguish_button);
	addAndMakeVisible(&regular_button);

	setSize(600, 600);
}

void SearchComponent::ReplacingComponent::buttonClicked(Button* button)
{
	if(button == &find_button){
		//After adding the text in the ComboBox to the input histories, it'll do the work.
		//�e�L�X�g�{�b�N�X�̃e�L�X�g����͗����ɒǉ����Ă���A���ۂɌ������s��
		const String TEXT = finding.getText();
		StoredSettings::getInstance()->AddRecentString(Identifiers::RecentSearches, TEXT);
		comp.impl->SetIsChanged(true);
		comp.impl->Execute(TEXT, true);
	}else if(button == &apply_button){
		//After adding the text in the ComboBox to the input histories, it'll do the work.
		//�e�L�X�g�{�b�N�X�̃e�L�X�g����͗����ɒǉ����Ă���A���ۂɒu�����s��
		const String TEXT = finding.getText();
		StoredSettings::getInstance()->AddRecentString(Identifiers::RecentReplaces, TEXT);
		comp.impl->SetIsChanged(true);
		comp.impl->Execute(TEXT, false);
	}else if(button == &apply_all_button){
	}else if(button == &distinguish_button){
		comp.impl->SetCaseSensitivity(distinguish_button.getToggleState());
	}else if(button == &regular_button){
		comp.impl->SetUseRegularExpr(regular_button.getToggleState());
	}
}

void SearchComponent::ReplacingComponent::resized(void)
{
	finding.setBoundsRelative(0.05f, 0.05f, 0.8f, 0.1f);
	replacing.setBoundsRelative(0.05f, 0.2f, 0.8f, 0.1f);
	distinguish_button.setBoundsRelative(0.05f, 0.3f, 0.5f, 0.05f);
	regular_button.setBoundsRelative(0.05f, 0.4f, 0.5f, 0.05f);
	find_button.setBoundsRelative(0.6f, 0.7f, 0.15f, 0.1f);
	apply_button.setBoundsRelative(0.8f, 0.7f, 0.15f, 0.1f);
	apply_all_button.setBoundsRelative(0.8f, 0.85f, 0.15f, 0.1f);
}

void SearchComponent::valueChanged(Value& value)
{
	recently_searched.removeValue(value);
	recently_replaced.removeValue(value);
}

void SearchComponent::valueTreePropertyChanged(ValueTree& treeWhosePropertyHasChanged, const Identifier& property)
{
	if(treeWhosePropertyHasChanged.hasType(Identifiers::RecentSearches)){
		Value tmp = treeWhosePropertyHasChanged.getPropertyAsValue(property, nullptr);
		tmp.addListener(this);
		recently_searched.add(tmp);
	}else if(treeWhosePropertyHasChanged.hasType(Identifiers::RecentReplaces)){
		Value tmp = treeWhosePropertyHasChanged.getPropertyAsValue(property, nullptr);
		tmp.addListener(this);
		recently_replaced.add(tmp);
	}
}

SearchDialog::SearchDialog(FlexibleDocumentComponent& DocComp, FindReplaceFunctionality* FindReplace) : DocumentWindow(TRANS("Find or Replace"), Colours::lightgrey, DocumentWindow::allButtons), doc_comp(DocComp)
{
	setContentOwned(new SearchComponent(FindReplace), true);
	setSize(600, 400);
}

void SearchDialog::closeButtonPressed(void)
{
	//Because we'll reuse the class later, we're simply setting it invisible.
	setVisible(false);
}

void SearchDialog::SetShowingTab(bool ShowingFind)
{
	dynamic_cast<SearchComponent*>(getContentComponent())->SetShowingTab(ShowingFind);
}

void SearchDialog::activeWindowStatusChanged(void)
{
	SearchComponent* comp = dynamic_cast<SearchComponent*>(getContentComponent());
	comp->GetFunctionality().SetEditor(dynamic_cast<CodeEditor*>(doc_comp.getActiveDocument())->GetEditorComponent());
	comp->GetFunctionality().SetIsChanged(true);
}
