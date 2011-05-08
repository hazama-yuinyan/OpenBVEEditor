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
	struct RegexIterator : public boost::iterator_adaptor<RegexIterator, std::vector<boost::xpressive::wsregex_iterator>::iterator, boost::use_default, boost::forward_traversal_tag>
	{
	public:
		typedef boost::iterator_adaptor<RegexIterator, std::vector<boost::xpressive::wsregex_iterator>::iterator, boost::use_default, boost::forward_traversal_tag> base_type;
		RegexIterator(void) : base_type(){};
		RegexIterator(const std::vector<boost::xpressive::wsregex_iterator>::iterator& It) : base_type(It){};
		RegexIterator(const RegexIterator& Other) : base_type(Other.base()){};
		RegexIterator& operator=(const RegexIterator& Other){
			base_reference() = Other.base();
			return *this;
		}
		void increment(void){
			++(*base_reference());

			boost::xpressive::wsregex_iterator end;
			if(*base() == end){
				++base_reference();
			}
		}
	};

	struct RegexCompare : public std::binary_function<boost::xpressive::wsregex_iterator, boost::xpressive::wsregex_iterator, bool>
	{
		bool operator()(const boost::xpressive::wsregex_iterator& lhs, const boost::xpressive::wsregex_iterator& rhs) const{
			return(lhs->position() < rhs->position());
		}
	};

	class MakeRegexIterator
	{
	private:
		const std::wstring &STR;

	public:
		MakeRegexIterator(const std::wstring& Str) : STR(Str){};
		boost::xpressive::wsregex_iterator operator()(boost::xpressive::wsregex& Regex){
			return boost::xpressive::wsregex_iterator(STR.begin(), STR.end(), Regex);
		}
	};

	struct IsInvalid
	{
	public:
		bool operator()(const boost::xpressive::wsregex_iterator& It) const{
			return(It == boost::xpressive::wsregex_iterator());
		}
	};

	enum TokenTypes{UNKNOWN, COMMENT, NAMESPACE, COMMAND, VARIABLE, PREPROCESSOR, POSITION};
	std::vector<boost::xpressive::wsregex_iterator> results;
	RegexIterator begin(void){return RegexIterator(results.begin());}
	RegexIterator end(void){return RegexIterator(results.end());}
	static const Colour colors[];
	bool is_initialized, did_pos_matched;
	int line_start_pos;
	std::wstring content;
	RegexIterator result_it;
	std::tr1::array<boost::xpressive::wsregex, POSITION> regexes;
	boost::xpressive::wsregex preprocess_names, commands, namespaces, variable_names;
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
	const StringArray getTokenTypes(void);
	const Colour getDefaultColour(int tokenType);
};

#endif
