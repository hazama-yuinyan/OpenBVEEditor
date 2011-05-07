/*
  ==============================================================================

    StoredSettings.cpp
    Created: 13 Apr 2011 4:48:07pm
    Author:  Ryouta

  ==============================================================================
*/

#include "StoredSettings.h"
#include "CommonHeader.h"


StoredSettings::StoredSettings(void) : tree_root(Identifiers::Category1)
	, settings_file(PropertiesFile::getDefaultAppSettingsFile(JUCEApplication::getInstance()->getApplicationName(), "settings", JUCEApplication::getInstance()->getApplicationName(), false))
	, MAX_RECENT_STRINGS(20)
{
	ScopedPointer<XmlElement> elem = XmlDocument::parse(settings_file);
	if(elem){
		tree_root = ValueTree::fromXml(*elem->getChildElement(0));
		command_manager->getKeyMappings()->restoreFromXml(*elem->getChildElement(1));
	}
	list.setMaxNumberOfItems(10);
	Apply();
}

StoredSettings::~StoredSettings(void)
{
	ScopedPointer<XmlElement> root_elem(new XmlElement("PROPERTIES"));
	XmlElement* settings_elem = tree_root.createXml();
	XmlElement* keys_elem = command_manager->getKeyMappings()->createXml(true);
	root_elem->addChildElement(settings_elem);
	root_elem->addChildElement(keys_elem);
	if(!settings_file.deleteFile() || !settings_file.create() || !root_elem->writeToFile(settings_file, String::empty)){
		AlertWindow::showMessageBox(AlertWindow::WarningIcon, "Caution!", TRANS("Couldn't save the settings!"));
	}
	clearSingletonInstance();
}

void StoredSettings::Apply(void)
{
	if(tree_root.getNumProperties() == 0){
		//We'll set various settings at the first start-up.
		//初回起動時にプロパティーを設定する
		tree_root.setProperty(Identifiers::RecentFiles, var(list.toString()), nullptr);

		tree_root.setProperty(Identifiers::Languages, var("en"), nullptr);
		tree_root.addChild(ValueTree(Identifiers::RecentSearches), -1, nullptr);
		tree_root.addChild(ValueTree(Identifiers::RecentReplaces), -1, nullptr);
	}

	list.restoreFromString(tree_root.getProperty(Identifiers::RecentFiles));
	list.removeNonExistentFiles();

	const String LANG_NAME = tree_root.getProperty(Identifiers::Languages);
	if(LANG_NAME != String("en")){
		File lang_file = File::getSpecialLocation(File::currentApplicationFile).getParentDirectory().getChildFile(String("languages/") + LANG_NAME + String(".txt"));
		LocalisedStrings::setCurrentMappings(new LocalisedStrings(lang_file));
		if(LANG_NAME == String("ja")){
			// UI表示フォントを日本語表示可能なものに変更する
			LookAndFeel::getDefaultLookAndFeel().setDefaultSansSerifTypefaceName(L"メイリオ");
		}
	}
	Desktop::getInstance().getComponent(0)->repaint();
}

ValueTree StoredSettings::GetProperty(const Identifier& CategoryName)
{
	return (CategoryName == Identifiers::Category1) ? tree_root : tree_root.getOrCreateChildWithName(CategoryName, nullptr);
}

void StoredSettings::RestoreProperty(const Identifier& CategoryName, const Identifier& PropertyName, Value& Val)
{
	ValueTree tree = (CategoryName == Identifiers::Category1) ? tree_root : tree_root.getOrCreateChildWithName(CategoryName, nullptr);
	if(!tree.hasProperty(PropertyName)){
		//If there isn't one then set a new one.
		//プロパティーが存在しなければ、設定する
		tree.setProperty(PropertyName, Val, nullptr);
	}else{
		//If the property already exists then just return it.
		//存在していれば、そのプロパティーを参照する
		Val.referTo(tree.getPropertyAsValue(PropertyName, nullptr));
	}
}

void StoredSettings::AddRecentString(const Identifier& Type, const String& Str)
{
	ValueTree tree = GetProperty(Type);
	if(tree.getNumProperties() >= MAX_RECENT_STRINGS){		//If we have more strings in the history over the range, remove the oldest one from it.
		tree.removeProperty(tree.getPropertyName(0), nullptr);
	}

	Identifier tmp(String(Str.hashCode64()));
	tree.setProperty(tmp, var(Str), nullptr);
}

juce_ImplementSingleton(StoredSettings);
