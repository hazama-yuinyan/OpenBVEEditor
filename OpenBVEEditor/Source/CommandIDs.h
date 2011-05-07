


namespace CommandIDs
{
	static const int CREATE_NEW_FILE = 0x200010;
	static const int OPEN_FILE = 0x200020;
	static const int SAVE_FILE = 0x200030;
	static const int CLOSE_FILE = 0x200040;
	static const int OPEN_FILE_VIA_DIALOG = 0x200050;
	static const int SAVE_FILE_AS = 0x200060;
	static const int CLOSE_ALL_FILES = 0x200070;

	static const int SHOW_PREFERENCE = 0x200100;

	static const int FIND = 0x200200;
	static const int HANDY_SEARCH = 0x200210;
	static const int REPLACE = 0x200220;
	static const int UNDO = 0x200230;
	static const int REDO = 0x200240;
	static const int AUTO_COMPLETE = 0x200250;
};

namespace CommandCategories
{
	static const char* const GENERAL = "General";
	static const char* const SETTING = "Setting";
	static const char* const EDITTING = "Editting";
};
