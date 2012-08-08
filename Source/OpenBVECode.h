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


#ifndef _BVE_CODE
#define _BVE_CODE


#include "CodeAssistant.h"
#undef T
#include <boost/xpressive/xpressive.hpp>
#include <boost/iterator/iterator_facade.hpp>
#include <array>


class BVECodeAssistant : public CodeAssistant
{
private:
	enum TokenTypes{UNKNOWN = -1, NAMESPACE, PREPROCESS, BUILTIN_FUNCTION};
	static const std::pair<int, String> KEYWORDS[], PARAMETERS[];
	static const String NAMESPACES[], PREPROCESS_NAMES[], TEXT_NAMESPACES[], PARAMETERS_PREPROCESS[];
	std::vector<int> returned_indeces;
	int cur_namespace_num, last_returned_type;
	bool is_with_found;

public:
	BVECodeAssistant(void);
	~BVECodeAssistant(void);
	void ComputeProposals(const CodeDocument::Position& Start, const CodeDocument::Position&, Array<ContentProposal>& Results);
	void ComputeContexts(const CodeDocument::Position& Start, const CodeDocument::Position&, OwnedArray<ContextInformation>& Results);
};

class BVETokenizer : public CodeTokeniser
{
private:
	typedef boost::xpressive::wsmatch::nested_results_type::const_iterator RegexIterator;

	enum TokenTypes{UNKNOWN, COMMENT, NAMESPACE, VARIABLE, COMMAND, PREPROCESSOR, POSITION};
	boost::xpressive::wsmatch result;
	RegexIterator begin(void){return result.nested_results().begin();}
	RegexIterator end(void){return result.nested_results().end();}

	static const Colour colors[];
	bool is_initialized, did_pos_matched;
	int line_start_pos;
	std::wstring content;
	RegexIterator result_it;
	std::tr1::array<boost::xpressive::wsregex, POSITION> regexes;
	boost::xpressive::wsregex preprocess_names, commands, namespaces, variable_names, line;

	void Tokenize(std::wstring& Str);
	void Init(void);
	int ToTokenType(const boost::xpressive::wsmatch& Match);

public:
	explicit BVETokenizer(const int BufferSize);
	BVETokenizer& operator=(const BVETokenizer& Src){
		result_it = Src.result_it;
		return *this;
	}
	int readNextToken(CodeDocument::Iterator& source);
	StringArray getTokenTypes(void);
	const Colour getDefaultColour(int tokenType);
};

#endif
