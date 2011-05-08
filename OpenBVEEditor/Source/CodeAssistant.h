/*
  ==============================================================================

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


#ifndef _CODE_ASSISTANT
#define _CODE_ASSISTANT


#include "../JuceLibraryCode/JuceHeader.h"


/**
* This class is used to store strings and images relating to them and to suggest them to the user. 
*/
class ContentProposal
{
private:
	String str;
	ScopedPointer<Image> img;

public:
	ContentProposal(void) : str(String::empty), img(){};
	ContentProposal(const String& StrToAdd, Image* const Img = nullptr) : str(StrToAdd), img(Img){};
	ContentProposal(const ContentProposal& Other) : str(Other.str), img(Other.img){};
	void Apply(CodeDocument& Doc);
	const String GetStringInfo(void) const{return str;}
	const Image* GetImage(void) const{return img;}
	bool HasImage(void) const{return img != nullptr;}
};

/**
* Used to show some informations about the suggested string besides it.
*/
class ContextInformation : public Component
{
private:
	String str;
	TextLayout layout;

public:
	ContextInformation(void) : str(String::empty), layout(){};
	explicit ContextInformation(const String& StrToAdd);
	ContextInformation(const ContextInformation& Other) : str(Other.str), layout(Other.layout){};
	void paint(Graphics& g);
};

/**
* The base class for processing suitable strings to the contexts.
*/
class CodeAssistant
{
public:
	CodeAssistant(void){};
	virtual ~CodeAssistant(void){};
	virtual void ComputeProposals(const CodeDocument::Position& Start, const CodeDocument::Position& End, Array<ContentProposal>& Results) = 0;
	virtual void ComputeContexts(const CodeDocument::Position& Start, const CodeDocument::Position& End, OwnedArray<ContextInformation>& Results) = 0;
};

/**
* It will collect informations and display them at appropriate times.
*/
class AssistantContents : public ListBoxModel,
						  public ListBox
{
private:
	Array<ContentProposal> proposals;
	OwnedArray<ContextInformation> context_infos;
	ValueTree tree;
	ScopedPointer<CodeAssistant> assistant_impl;
	CodeEditorComponent& editor;
	int last_selected_row;

	friend class AssistantWindow;

	void Apply(const int Row);

public:
	AssistantContents(CodeEditorComponent& Editor, CodeAssistant* Assistant);
	~AssistantContents(void){};
	int getNumRows(void){return proposals.size();}
	void paintListBoxItem(int rowNumber, Graphics& g, int width, int height, bool rowIsSelected);
	void listBoxItemClicked(int row, const MouseEvent&);
	void returnKeyPressed(int lastRowSelected);
	void selectedRowsChanged(int lastRowSelected);
	void Update(const CodeDocument::Position& affectedTextStart, const CodeDocument::Position& afferctedTextEnd);
	static void GetDocumentTypes(StringPairArray& Types);
	bool IsUsable(void){return assistant_impl != nullptr;}
};

/**
* AssistantContents will show its contents inside this window.
*/
class AssistantWindow : public DocumentWindow,
						public CodeDocument::Listener,
						public ApplicationCommandTarget
{
private:
	AssistantContents* content;

public:
	explicit AssistantWindow(AssistantContents* Assistant);
	void codeDocumentChanged(const CodeDocument::Position& affectedTextStart, const CodeDocument::Position& affertedTextEnd);
	bool keyPressed(const KeyPress& key);
	ApplicationCommandTarget* getNextCommandTarget()
    {
        // this will return the next parent component that is an ApplicationCommandTarget (in this
        // case, there probably isn't one, but it's best to use this method in your own apps).
        return findFirstTargetParentComponent();
    }
	void getAllCommands(Array<CommandID>& commands);
	void getCommandInfo(CommandID commandID, ApplicationCommandInfo& result);
	bool perform(const InvocationInfo& info);
};

#endif
