/*
  ==============================================================================

    VariousTools.h
    Created: 17 Nov 2011 12:42:34am

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

#ifndef __VARIOUSTOOLS_H_C58E79A1__
#define __VARIOUSTOOLS_H_C58E79A1__

/**
* This tool recorrect the positions in BVE route files.
*/
class CorrectPositioner
{
private:
	CodeDocument& target_doc;

public:
	CorrectPositioner(void);

	void SetDocument(CodeDocument& Doc){target_doc = Doc;}
};

/**
* CorrectPositioner shows its contents inside this window.
*/
class CorrectPositionerWindow : public DocumentWindow
{
public:
	CorrectPositionerWindow(void);
};



#endif  // __VARIOUSTOOLS_H_C58E79A1__
