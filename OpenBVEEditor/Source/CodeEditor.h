#ifndef _CODE_EDITOR
#define _CODE_EDITOR

#include "../JuceLibraryCode/JuceHeader.h"


class AssistantWindow;


/**
* This class shows the line number at next to it.
*/
class LineNumbersDisplay : public Component,
						   public Timer
{
private:
	TextLayout layout;
	CodeEditorComponent& editor;
	int last_start_line_num;

public:
	LineNumbersDisplay(CodeEditorComponent& Editor);
	~LineNumbersDisplay(void);

	void timerCallback(void);
	void paint(Graphics& g);
	void resized(void);
};

/**
* The editor. It also has an instance of the file that the document refers to, and several other objects related to the editor.
*/
class CodeEditor : public Component,
				   public CodeDocument::Listener
{
private:
	File file;
	ScopedPointer<CodeDocument> doc;
	ScopedPointer<CodeEditorComponent> editor;
	ScopedPointer<CodeTokeniser> tokeniser;
	ScopedPointer<AssistantWindow> assistant;
	LineNumbersDisplay line_num;
	StretchableLayoutManager layout_manager;
	Value do_auto_indent, do_bracket_closing, tab_width;	//the real types: bool, bool and int respectively

public:
	CodeEditor(const File& File, CodeDocument* Doc, CodeEditorComponent* Editor, CodeTokeniser* Tokeniser, AssistantWindow *Assistant);
	~CodeEditor(void);

	void codeDocumentChanged(const CodeDocument::Position& affectedTextStart, const CodeDocument::Position& affectedTextEnd);
	void resized(void);
	bool IsSaved(void) const{return !doc->hasChangedSinceSavePoint();}
	bool Save(void);
	bool SaveAs(void);
	CodeEditorComponent* GetEditorComponent(void){return editor;}
};

#endif
