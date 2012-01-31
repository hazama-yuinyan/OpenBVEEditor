﻿/*
  ==============================================================================

    This file was auto-generated by the Jucer!

    It contains the basic outline for a simple desktop window.

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

#include "CommonHeader.h"
#include "MainWindow.h"
#include "MainComponent.h"
#include "StoredSettings.h"


//==============================================================================
MainAppWindow::MainAppWindow()
    : DocumentWindow (JUCEApplication::getInstance()->getApplicationName(),
                      Colours::lightgrey,
                      DocumentWindow::allButtons)
{
	StoredSettings::getInstance();												//call the constructor of StoredSettings to initialize it
	MainComponent* const comp = new MainComponent();
	command_manager->registerAllCommandsForTarget(comp);
	command_manager->registerAllCommandsForTarget(JUCEApplication::getInstance());
	setResizable(true, true);
	setContentOwned(comp, true);
	setMenuBar(comp);
	const Rectangle<int> display_size = Desktop::getInstance().getMainMonitorArea();
	setSize(display_size.getWidth()/2, display_size.getHeight()/2);
    centreWithSize (getWidth(), getHeight());
	addKeyListener(command_manager->getKeyMappings());
    setVisible (true);
}

MainAppWindow::~MainAppWindow()
{
	setMenuBar(nullptr);
}

void MainAppWindow::closeButtonPressed()
{
	command_manager->invokeDirectly(CommandIDs::CLOSE_ALL_FILES, false);
	if(dynamic_cast<MainComponent*>(getContentComponent())->IsQuitting()){
		JUCEApplication::getInstance()->quit();
	}
}
