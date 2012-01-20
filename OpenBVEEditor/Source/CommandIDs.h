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
