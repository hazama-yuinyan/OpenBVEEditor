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


#include "OpenBVECode.h"



bool operator==(const CodeDocument::Iterator& lhs, const CodeDocument::Iterator& rhs){
	return(lhs.getPosition() == rhs.getPosition());
}

bool operator!=(const CodeDocument::Iterator& lhs, const CodeDocument::Iterator& rhs){
	return !(lhs == rhs);
}

const std::pair<int, String> OpenBVECodeAssistant::KEYWORDS[] = {
	std::make_pair(0, L"UnitOfLength"),
	std::make_pair(0, L"UnitOfSpeed"),
	std::make_pair(0, L"BlockLength"),
	std::make_pair(0, L"ObjectVisiblity"),
	std::make_pair(0, L"SectionBehavior"),
	std::make_pair(0, L"CantBehavior"),
	std::make_pair(0, L"FogBehavior"),
	std::make_pair(1, L"Comment"),
	std::make_pair(1, L"Image"),
	std::make_pair(1, L"Timetable"),
	std::make_pair(1, L"Change"),
	std::make_pair(1, L"Gauge"),
	std::make_pair(1, L"Signal"),
	std::make_pair(1, L"RunInterval"),
	std::make_pair(1, L"AccelerationDueToGravity"),
	std::make_pair(1, L"Elevation"),
	std::make_pair(1, L"Temperature"),
	std::make_pair(1, L"Pressure"),
	std::make_pair(1, L"AmbientLight"),
	std::make_pair(1, L"DirectionalLight"),
	std::make_pair(1, L"LightDirection"),
	std::make_pair(2, L"Folder"),
	std::make_pair(2, L"File"),
	std::make_pair(2, L"Run"),
	std::make_pair(2, L"Rail"),
	std::make_pair(2, L"Flange"),
	std::make_pair(8, L"Day"),
	std::make_pair(8, L"Night"),
	std::make_pair(2, L"Interval"),
	std::make_pair(2, L"Velocity"),
	std::make_pair(3, L"Ground"),
	std::make_pair(3, L"Rail"),
	std::make_pair(3, L"WallL"),
	std::make_pair(3, L"WallR"),
	std::make_pair(3, L"DikeL"),
	std::make_pair(3, L"DikeR"),
	std::make_pair(3, L"FormL"),
	std::make_pair(3, L"FormR"),
	std::make_pair(3, L"FormCL"),
	std::make_pair(3, L"FormCR"),
	std::make_pair(3, L"RoofL"),
	std::make_pair(3, L"RoofR"),
	std::make_pair(3, L"RoofCL"),
	std::make_pair(3, L"RoofCR"),
	std::make_pair(3, L"CrackL"),
	std::make_pair(3, L"CrackR"),
	std::make_pair(3, L"Freeobj"),
	std::make_pair(3, L"Beacon"),
	std::make_pair(3, L"Pole"),
	std::make_pair(4, L"Background"),
	std::make_pair(9, L"X"),
	std::make_pair(9, L"Aspect"),
	std::make_pair(5, L"Ground"),
	std::make_pair(7, L"RailStart"),
	std::make_pair(7, L"Rail"),
	std::make_pair(7, L"RailType"),
	std::make_pair(7, L"RailEnd"),
	std::make_pair(7, L"Accuracy"),
	std::make_pair(7, L"Adhesion"),
	std::make_pair(7, L"Pitch"),
	std::make_pair(7, L"Curve"),
	std::make_pair(7, L"Turn"),
	std::make_pair(7, L"Height"),
	std::make_pair(7, L"FreeObj"),
	std::make_pair(7, L"Wall"),
	std::make_pair(7, L"WallEnd"),
	std::make_pair(7, L"Dike"),
	std::make_pair(7, L"DikeEnd"),
	std::make_pair(7, L"Pole"),
	std::make_pair(7, L"PoleEnd"),
	std::make_pair(7, L"Crack"),
	std::make_pair(7, L"Ground"),
	std::make_pair(7, L"Sta"),
	std::make_pair(7, L"Station"),
	std::make_pair(7, L"Stop"),
	std::make_pair(7, L"Form"),
	std::make_pair(7, L"Limit"),
	std::make_pair(7, L"Section"),
	std::make_pair(7, L"SigF"),
	std::make_pair(7, L"Sig"),
	std::make_pair(7, L"Relay"),
	std::make_pair(7, L"Transponder"),
	std::make_pair(7, L"Tr"),
	std::make_pair(7, L"AtsSn"),
	std::make_pair(7, L"AtsP"),
	std::make_pair(7, L"Pattern"),
	std::make_pair(7, L"PLimit"),
	std::make_pair(7, L"Back"),
	std::make_pair(7, L"Fog"),
	std::make_pair(7, L"Brightness"),
	std::make_pair(7, L"Marker"),
	std::make_pair(7, L"PointOfInterest"),
	std::make_pair(7, L"POI"),
	std::make_pair(7, L"PreTrain"),
	std::make_pair(7, L"Announce"),
	std::make_pair(7, L"Doppler"),
	std::make_pair(7, L"Buffer"),
	std::make_pair(9, L"Set"),
	std::make_pair(9, L"Load")
};

const String OpenBVECodeAssistant::NAMESPACES[] = {
	L"Options",
	L"Route",
	L"Train",
	L"Structure",
	L"Texture",
	L"Cycle",
	L"Signal",
	L"Track"
};

const String OpenBVECodeAssistant::TEXT_NAMESPACES[] = {
	L"Commands from this namespace provide generic options that affect the way other commands are processed. You should make use of commands from this namespace before making use of commands from other namespaces.",
	L"Commands from this namespace define general properties of the route.",
	L"Commands from this namespace define route-train associations.",
	L"The commands in the Structure namespace define which objects to use in other commands. Generally, commands like Track.Rail, Track.FreeObj and so on create objects referenced by a StructureIndex. This StructureIndex is specific to that command, so you can define a set of objects specific to Track.Rail, Track.FreeObj and so on.",
	L"Commands from this namespace define which background images to use and how they are aligned.",
	L"",
	L"Commands from this namespace define custom signals.",
	L"Commands from this namespace define the track layout. Commands from this namespace should appear after commands from any of the other namespaces, and they usually form the largest part of the route file."
};

const String OpenBVECodeAssistant::PREPROCESS_NAMES[] = {
	L"If",
	L"Else",
	L"Endif",
	L"Include",
	L"Rnd",
	L"Chr",
	L"Sub"
};

const String OpenBVECodeAssistant::PARAMETERS_PREPROCESS[] = {
	L"(Condition)",
	L"()",
	L"()",
	L"(File) or (File1; Weight1; File2; Weight2; ...)",
	L"(Start; End)",
	L"(Ascii)",
	L"(Index) = Expression"
};

const std::pair<int, String> OpenBVECodeAssistant::PARAMETERS[] = {
	std::make_pair(0, L"FactorInMeters1; FactorInMeters2; FatocrInMeters3; ...; FactorInMeters_n"),
	std::make_pair(0, L"FactorInKmph"),
	std::make_pair(0, L"Length"),
	std::make_pair(0, L"Mode"),
	std::make_pair(0, L"Mode"),
	std::make_pair(0, L"Mode"),
	std::make_pair(0, L"Mode"),
	std::make_pair(1, L"Text"),
	std::make_pair(1, L"File"),
	std::make_pair(1, L"Text"),
	std::make_pair(1, L"Mode"),
	std::make_pair(1, L"ValueInMilimeters"),
	std::make_pair(1, L"(AspectIndex) Speed"),
	std::make_pair(1, L"Interval0; Interval1; ...; Interval_n-1"),
	std::make_pair(1, L"Value"),
	std::make_pair(1, L"ValueInCelsius"),
	std::make_pair(1, L"ValueInKPa"),
	std::make_pair(1, L"RedValue; GreenValue; BlueValue"),
	std::make_pair(1, L"RedValue; GreenValue; BlueValue"),
	std::make_pair(1, L"Theta; Phi"),
	std::make_pair(2, L"FolderName"),
	std::make_pair(2, L"FolderName"),
	std::make_pair(2, L"(RailTypeIndex) RailSoundIndex"),
	std::make_pair(2, L"(RailTypeIndex) RailSoundIndex"),
	std::make_pair(2, L"(RailTypeIndex) FlangeSoundIndex"),
	std::make_pair(8, L"FileName"),
	std::make_pair(8, L"FileName"),
	std::make_pair(2, L"ValueInMilimeters"),
	std::make_pair(2, L"Interval0; Interval1; ...; Interval_n-1"),
	std::make_pair(2, L"Speed"),
	std::make_pair(3, L"(StructureIndex) FileName"),
	std::make_pair(3, L"(StructureIndex) FileName"),
	std::make_pair(3, L"(StructureIndex) FileName"),
	std::make_pair(3, L"(StructureIndex) FileName"),
	std::make_pair(3, L"(StructureIndex) FileName"),
	std::make_pair(3, L"(StructureIndex) FileName"),
	std::make_pair(3, L"(StructureIndex) FileName"),
	std::make_pair(3, L"(StructureIndex) FileName"),
	std::make_pair(3, L"(StructureIndex) FileName"),
	std::make_pair(3, L"(StructureIndex) FileName"),
	std::make_pair(3, L"(StructureIndex) FileName"),
	std::make_pair(3, L"(StructureIndex) FileName"),
	std::make_pair(3, L"(StructureIndex) FileName"),
	std::make_pair(3, L"(StructureIndex) FileName"),
	std::make_pair(3, L"(StructureIndex) FileName"),
	std::make_pair(3, L"(StructureIndex) FileName"),
	std::make_pair(3, L"(StructureIndex) FileName"),
	std::make_pair(3, L"(StructureIndex) FileName"),
	std::make_pair(3, L"(NumberOfAdditionalRails; PoleStructureIndex) FileName"),
	std::make_pair(4, L"(BackgroundTextureIndex) FileName"),
	std::make_pair(9, L"RepititionCount"),
	std::make_pair(9, L"Mode"),
	std::make_pair(5, L"(GroundStructureIndex) GroundStructureIndex0; GroundStructureIndex1; GroundStructureIndex2; ...; GroundStructureIndex_n-1"),
	std::make_pair(7, L"RailIndex; X; Y; RailType"),
	std::make_pair(7, L"RailIndex; X; Y; RailType"),
	std::make_pair(7, L"RailIndex; RailType"),
	std::make_pair(7, L"RailIndex; X; Y"),
	std::make_pair(7, L"Value"),
	std::make_pair(7, L"Rate"),
	std::make_pair(7, L"Rate"),
	std::make_pair(7, L"Radius; CantInMilimeters"),
	std::make_pair(7, L"Ratio"),
	std::make_pair(7, L"Y"),
	std::make_pair(7, L"RailIndex; FreeObjStructureIndex; X; Y; Yaw; Pitch; Roll"),
	std::make_pair(7, L"RailIndex; Direction; WallStructureIndex"),
	std::make_pair(7, L"RailIndex"),
	std::make_pair(7, L"RailIndex; Direction; DikeStructureIndex"),
	std::make_pair(7, L"RailIndex"),
	std::make_pair(7, L"RailIndex; NumberOfAdditionalRails; Location; Interval; PoleStructureIndex"),
	std::make_pair(7, L"RailIndex"),
	std::make_pair(7, L"RailIndex1; RailIndex2; CrackStructureIndex"),
	std::make_pair(7, L"CycleIndex"),
	std::make_pair(7, L"Name; ArrivalTime; DepartureTime; PassAlarm; Doors; ForcedRedSignal; System; ArrivalSound; StopDuration; PassengerRatio; DepatureSound; TimetableIndex"),
	std::make_pair(7, L"Name; ArrivalTime; DepartureTime; ForcedRedSignal; System; DepatureSound"),
	std::make_pair(7, L"Direction; BackwardTolerance; ForwardTolerance; Cars"),
	std::make_pair(7, L"RailIndex1; RailIndex2; RoofStructureIndex; FormStructureIndex"),
	std::make_pair(7, L"Speed; Post; Cource"),
	std::make_pair(7, L"a0; a1; a2; ...; a_n"),
	std::make_pair(7, L"SignalIndex; Section; X; Y; Yaw; Pitch; Roll"),
	std::make_pair(7, L"Aspects; Unused; X; Y; Yaw; Pitch; Roll"),
	std::make_pair(7, L"Aspects; Unused; X; Y; Yaw; Pitch; Roll"),
	std::make_pair(7, L"X; Y; Yaw; Pitch; Roll"),
	std::make_pair(7, L"Type; BeaconStructureIndex; Section; Data; X; Y; Yaw; Pitch; Roll"),
	std::make_pair(7, L"Type; Signal; SwitchSystem; X; Y; Yaw; Pitch; Roll"),
	std::make_pair(7, L"Type; Signal; SwitchSystem; X; Y; Yaw; Pitch; Roll"),
	std::make_pair(7, L"Nothing"),
	std::make_pair(7, L"Nothing"),
	std::make_pair(7, L"Type; Speed"),
	std::make_pair(7, L"Speed"),
	std::make_pair(7, L"BackgroundTextureIndex"),
	std::make_pair(7, L"StartingDistance; EndingDistance; RedValue; GreenValue; BlueValue"),
	std::make_pair(7, L"Value"),
	std::make_pair(7, L"FileName; Distance"),
	std::make_pair(7, L"RailIndex; X; Y; Yaw; Pitch; Roll; Text"),
	std::make_pair(7, L"RailIndex; X; Y; Yaw; Pitch; Roll; Text"),
	std::make_pair(7, L"Time"),
	std::make_pair(7, L"FileName; Speed"),
	std::make_pair(7, L"FileName; X; Y"),
	std::make_pair(7, L"Nothing"),
	std::make_pair(9, L"(SignalIndex) AnimatedObjectFile"),
	std::make_pair(9, L"(SignalIndex) SignalFileWithoutExtension; GlowFileWithoutExtension")
};

OpenBVECodeAssistant::OpenBVECodeAssistant(void) : cur_namespace_num(-1), last_returned_type(UNKNOWN), is_with_found(false)
{
}

OpenBVECodeAssistant::~OpenBVECodeAssistant(void)
{
}

void OpenBVECodeAssistant::ComputeProposals(const CodeDocument::Position& Start, const CodeDocument::Position&, Array<ContentProposal>& Results)
{
	const String LINE = Start.getLineText().fromLastOccurrenceOf(L",", false, false).trim();
	int i;
	if(is_with_found){
		CodeDocument::Position one_line_above = Start.movedByLines(-1);
		const String ABOVE_LINE = one_line_above.getLineText();
		for(i = 0; i < numElementsInArray(NAMESPACES); ++i){
			if(ABOVE_LINE.containsIgnoreCase(NAMESPACES[i])){
				cur_namespace_num = i;
				is_with_found = false;
			}
		}
	}

	returned_indeces.clear();
	if(LINE.startsWithIgnoreCase(L"With ")){
		last_returned_type = NAMESPACE;
		const String TARGET_STR = LINE.fromLastOccurrenceOf(L"With ", false, true);
		is_with_found = true;
		for(i = 0; i < numElementsInArray(NAMESPACES); ++i){
			if(NAMESPACES[i].startsWithIgnoreCase(TARGET_STR)){
				Results.add(ContentProposal(NAMESPACES[i]));
				returned_indeces.push_back(i);
			}
		}
		return;
	}else if(LINE.startsWithIgnoreCase(L"$")){
		last_returned_type = PREPROCESS;
		const String TARGET_STR = LINE.fromLastOccurrenceOf(L"$", false, true);
		for(i = 0; i < numElementsInArray(PREPROCESS_NAMES); ++i){
			if(PREPROCESS_NAMES[i].startsWithIgnoreCase(TARGET_STR)){
				Results.add(ContentProposal(PREPROCESS_NAMES[i]));
				returned_indeces.push_back(i);
			}
		}
		return;
	}else if(LINE.startsWithIgnoreCase(L".")){
		last_returned_type = BUILTIN_FUNCTION;
		const String TARGET_STR = LINE.fromLastOccurrenceOf(".", false, false);
		for(i = 0; i < numElementsInArray(KEYWORDS); ++i){
			if(KEYWORDS[i].first == cur_namespace_num && KEYWORDS[i].second.startsWithIgnoreCase(TARGET_STR)){
				Results.add(ContentProposal(KEYWORDS[i].second));
				returned_indeces.push_back(i);
			}
		}
		return;
	}else if(LINE.toLowerCase().containsAnyOf(L"ortsc")){
		last_returned_type = NAMESPACE;
		const String& TARGET_STR = LINE;
		for(i = 0; i < numElementsInArray(NAMESPACES); ++i){
			if(NAMESPACES[i].startsWithIgnoreCase(TARGET_STR)){
				Results.add(ContentProposal(NAMESPACES[i]));
				returned_indeces.push_back(i);
			}
		}
	}
}

void OpenBVECodeAssistant::ComputeContexts(const CodeDocument::Position& Start, const CodeDocument::Position&, OwnedArray<ContextInformation>& Results)
{
	std::vector<int>::const_iterator it = returned_indeces.begin(), it_end = returned_indeces.end();
	for(; it != it_end; ++it){
		if(last_returned_type == NAMESPACE){
			Results.add(new ContextInformation(TEXT_NAMESPACES[*it]));
		}else if(last_returned_type == PREPROCESS){
			Results.add(new ContextInformation(PREPROCESS_NAMES[*it] + PARAMETERS_PREPROCESS[*it]));
		}else if(last_returned_type == BUILTIN_FUNCTION){
			Results.add(new ContextInformation(KEYWORDS[*it].second + String(L" ") + PARAMETERS[*it].second));
		}
	}
}


const CodeEditorComponent::ColourScheme::TokenType OpenBVETokenizer::tokens[] = {
	{"Unknown", Colour(0, 0, 0)},		//Unknown
	{"Comment", Colour(0, 128, 0)},		//Comment
	{"Namespaces", Colour(0, 0, 128)},		//Namespaces
	{"Command", Colour(0, 0, 128)},		//Command
	{"Variable", Colour(128, 0, 0)},		//Variable
	{"Preprocessor", Colour(10, 128, 20)},	//Preprocessor
	{"Position", Colour(43, 145, 175)}	//Position
};

OpenBVETokenizer::OpenBVETokenizer(const int BufferSize) : content(), is_initialized(false), did_pos_matched(true)
{
	content.reserve(BufferSize);
	Init();
}

void OpenBVETokenizer::Init(void)
{
	using boost::xpressive::icase;
	using boost::xpressive::as_xpr;
	using boost::xpressive::set;
	using boost::xpressive::space;
	using boost::xpressive::digit;
	using boost::xpressive::bos;
	using boost::xpressive::before;
	using boost::xpressive::keep;
	using boost::xpressive::by_ref;
	using boost::xpressive::_;

	preprocess_names = (icase(L"if")| icase(L"else")| icase(L"endif")| icase(L"include")| icase(L"rnd")| icase(L"chr"));
	commands = (icase(L"unitoflength")| icase(L"unitofspeed")| icase(L"blocklength")| icase(L"objectvisiblity")| icase(L"sectionbehavior")| icase(L"cantbehavior")| icase(L"fogbehavior")
		| icase(L"comment")| icase(L"image")| icase(L"timetable")| icase(L"change")| icase(L"gauge")| icase(L"signal")| icase(L"runinterval")| icase(L"accelerationduetogravity")
		| icase(L"elevation")| icase(L"temperature")| icase(L"pressure")| icase(L"ambientlight")| icase(L"directionallight")| icase(L"lightdirection")| icase(L"folder")| icase(L"file") 
		| icase(L"run")| icase(L"rail")| icase(L"set")| icase(L"flange")| icase(L"day")| icase(L"night")| icase(L"load")| icase(L"interval")| icase(L"velocity")| icase(L"ground")
		| icase(L"freeobj")| icase(L"beacon")| icase(L"pole")| icase(L"background")| icase(L"x")| icase(L"aspect")| icase(L"railstart")| icase(L"railtype")| icase(L"railend")| icase(L"accuracy") 
		| icase(L"adhesion")| icase(L"pitch")| icase(L"curve")| icase(L"turn")| icase(L"height")| icase(L"wallend")| icase(L"dikeend")| icase(L"poleend")| icase(L"sta")| icase(L"station") 
		| icase(L"stop")| icase(L"limit")| icase(L"section")| icase(L"sigf")| icase(L"sig")| icase(L"relay")| icase(L"transponder")| icase(L"tr")| icase(L"atssn")| icase(L"atsp")
		| icase(L"pattern")| icase(L"plimit")| icase(L"back")| icase(L"fog")| icase(L"brightness")| icase(L"marker")| icase(L"pointofinterest")| icase(L"poi")| icase(L"pretrain")| icase(L"announce")| icase(L"doppler")| icase(L"buffer")
		| icase(L"wall") | icase(L"dike") | icase(L"form") | icase(L"crack"));
	namespaces = (icase(L"options")| icase(L"route")| icase(L"train")| icase(L"structure")| icase(L"texture")| icase(L"cycle")| icase(L"signal")| icase(L"track"));
	variable_names = (icase(L"run") | icase(L"rail") | icase(L"flange") | icase(L"walll")| icase(L"wallr") | icase(L"dikel")| icase(L"diker") | icase(L"forml")| icase(L"formr") | icase(L"formcl")| icase(L"formcr")
		| icase(L"roofl")| icase(L"roofr") | icase(L"roofcl")| icase(L"roofcr") | icase(L"crackl") | icase(L"crackr") | icase(L"ground") | icase(L"freeobj") | icase(L"beacon") | icase(L"pole") | icase(L"background")
		| icase(L"signal"));

	regexes[0] = L';' >> *~(set= L',');					//comment
	regexes[1] = icase(L"with") >> space >> namespaces;		//namespaces
	regexes[2] = variable_names >> L'(' >> +digit >> L')' >> *(L'.' >> icase(L"set") | icase(L"load")) >> space >> +~(set= L',');	//variable
	regexes[3] = commands >> (space | L'(') >> *~(set= L',');				//commands
	regexes[4] = L'$' >> preprocess_names >> L'(' >> (keep(+~(set=L'$')) | by_ref(regexes[4])) >> L')';	//preprocess
	regexes[5] = +digit >> *(L'.' >> +digit) >> *space;	//position

	line = *(*as_xpr(L',') >> (regexes[0] | (bos >> (regexes[1] | regexes[5])) | (!namespaces >> L'.' >> (regexes[2] | regexes[3])) | regexes[4]));
}

void OpenBVETokenizer::Tokenize(std::wstring& Str)
{
	boost::xpressive::regex_search(Str, result, line);
	result_it = begin();
	is_initialized = true;
}

int OpenBVETokenizer::ToTokenType(const boost::xpressive::wsmatch& Match)
{
	boost::xpressive::wsregex_id_filter_predicate comment_id(regexes[COMMENT-1].regex_id()), namespaces_id(regexes[NAMESPACE-1].regex_id()), namespaces_id2(namespaces.regex_id()), command_id(regexes[COMMAND-1].regex_id())
		, preprocess_id(regexes[PREPROCESSOR-1].regex_id()), variable_id(regexes[VARIABLE-1].regex_id()), position_id(regexes[POSITION-1].regex_id());

	if(comment_id(Match)){
		DBG("comment");
		return COMMENT;
	}else if(namespaces_id(Match) || namespaces_id2(Match)){
		DBG("namespaces");
		return NAMESPACE;
	}else if(command_id(Match)){
		DBG("command");
		return COMMAND;
	}else if(preprocess_id(Match)){
		DBG("preprocess");
		return PREPROCESSOR;
	}else if(variable_id(Match)){
		DBG("variable");
		return VARIABLE;
	}else if(position_id(Match)){
		DBG("position");
		return POSITION;
	}

	DBG("unknown");
	return UNKNOWN;
}

int OpenBVETokenizer::readNextToken(CodeDocument::Iterator& source)
{
	if(!is_initialized){
		CodeDocument::Iterator start(source), line_end(source);
		line_end.skip();
		line_end.skipToEndOfLine();
		line_start_pos = start.getPosition();

		while(!source.isEOF() && source != line_end){
			content.push_back(static_cast<wchar_t>(source.nextChar()));		//行の終わり、または一つのexpressionの末端までの文字列をコピーする
		}

		Tokenize(content);		//boost::xpressiveを使ってトークンの切り出しを行い、その結果のイテレーターを受け取る

		source = start;
	}

	if(result_it == end()){
		content.clear();
		is_initialized = false;
		did_pos_matched = true;
		while(!source.isEOF()){		//トークンが見つからなかったので、最後までイテレータを進める
			wchar_t c = static_cast<wchar_t>(source.nextChar());
			if(c == L'\n' || c == L'\r'){
				source.skipToEndOfLine();
				break;
			}
		}
		return UNKNOWN;
	}

	if(did_pos_matched){
		auto last_it = *result_it;
		const int DIFF = last_it.length();
		if(DIFF < 0){
			++result_it;
			return UNKNOWN;
		}
		did_pos_matched = (++result_it != end()) ? source.getPosition() == result_it->position() + line_start_pos : false;
		DBG(String(source.getLine()+1) + L"行 " + String(source.getPosition() - line_start_pos) + L"文字目:");
		for(int i = 0; i < DIFF; ++i){		//トークンの長さ分だけイテレータを進める
			source.skip();
		}
		
		return ToTokenType(last_it);		//トークンのタイプを結果のコンテナから返す
	}

	const int DIFF = result_it->position() - (source.getPosition() - line_start_pos);
	for(int i = 0; i < DIFF; ++i){		//次のトークンの先頭文字を指すまでイテレータを進める
		source.skip();
	}
	did_pos_matched = true;
	return UNKNOWN;
}

CodeEditorComponent::ColourScheme OpenBVETokenizer::getDefaultColourScheme()
{
	CodeEditorComponent::ColourScheme scheme;
	scheme.types.addArray(tokens, numElementsInArray(tokens));
	return scheme;
}
