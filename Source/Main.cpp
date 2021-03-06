﻿/*
  ==============================================================================

    This file was auto-generated by the Jucer!

    It contains the basic startup code for a Juce application.

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


#include "../JuceLibraryCode/JuceHeader.h"
#include "CommonHeader.h"
#include "MainWindow.h"
#include "StoredSettings.h"

#define BOOST_UTF8_BEGIN_NAMESPACE namespace utf8{
#define BOOST_UTF8_DECL
#define BOOST_UTF8_END_NAMESPACE }
#include <boost/detail/utf8_codecvt_facet.ipp>
#undef BOOST_UTF8_DECL
#undef BOOST_UTF8_BEGIN_NAMESPACE
#undef BOOST_UTF8_END_NAMESPACE

ApplicationCommandManager* command_manager = 0;


//==============================================================================
class OpenBVEEditorApplication  : public JUCEApplication
{
public:
    //==============================================================================
    OpenBVEEditorApplication()
    {
    }

    ~OpenBVEEditorApplication()
    {
    }

    //==============================================================================
    void initialise (const String&)
    {
        // Do your application's initialisation code here..
		command_manager = new ApplicationCommandManager();
		// set the locale in case of Japanese..
		std::locale loc;
		loc = std::locale(loc, new utf8::utf8_codecvt_facet());
		std::locale::global(loc);
        mainWindow = new MainAppWindow();
    }

    void shutdown()
    {
        // Do your application's shutdown code here..
        mainWindow = 0;
		StoredSettings::deleteInstance();
		deleteAndZero(command_manager);
    }

    //==============================================================================
    void systemRequestedQuit()
    {
		mainWindow->closeButtonPressed();
    }

    //==============================================================================
    const String getApplicationName()
    {
        return "OpenBVEEditor";
    }

    const String getApplicationVersion()
    {
        return ProjectInfo::versionString;
    }

    bool moreThanOneInstanceAllowed()
    {
        return false;
    }

    void anotherInstanceStarted (const String&)
    {
        
    }

	void unhandledException(const std::exception* e, const String& sourceFileName, int lineNumber)
	{
		DBG(String("an error has occured in ") + sourceFileName + String(" at line ") + String(lineNumber));
		AlertWindow::showMessageBox(AlertWindow::WarningIcon, "Error", e->what());
		systemRequestedQuit();
	}

private:
    ScopedPointer <MainAppWindow> mainWindow;
};

//==============================================================================
// This macro generates the main() routine that starts the app.
START_JUCE_APPLICATION(OpenBVEEditorApplication)
