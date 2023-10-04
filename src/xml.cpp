#include "defs.h"

void xml_GetSetting(TiXmlNode *Parent, char *SettingName, int *Value)
{
	TiXmlElement *Element;
	int tmpint = 0;

	Element = Parent->FirstChildElement(SettingName);
	if (!Element)
	{
		console_Print("XML: Couldn't find value attribute");
		return;
	}

	Element->Attribute("Value", &tmpint);

	*Value = tmpint;
}

void xml_GetSetting(TiXmlNode *Parent, char *SettingName, float *Value)
{
	TiXmlElement *Element;
	double tmpdouble = 0.0f;

	Element = Parent->FirstChildElement(SettingName);
	if (!Element)
	{
		console_Print("XML: Couldn't find value attribute");
		return;
	}

	Element->Attribute("Value", &tmpdouble);

	*Value = tmpdouble;
}

void xml_GetSetting(TiXmlNode *Parent, char *SettingName, char *Value)
{
	TiXmlElement *Element;
	char *tmpchar = 0;

	Element = Parent->FirstChildElement(SettingName);
	if (!Element)
	{
		console_Print("XML: Couldn't find value attribute");
		return;
	}

	strcpy(Value, Element->Attribute("Value"));
}

void xml_SaveSetting(TiXmlNode *Parent, char *SettingName, int Value)
{
	TiXmlElement *Element;
	Element = new TiXmlElement(SettingName);
	Element->SetAttribute("Value", Value);
	Parent->InsertEndChild((TiXmlNode &)*Element);
	delete Element;
}

void xml_SaveSetting(TiXmlNode *Parent, char *SettingName, float Value)
{
	TiXmlElement *Element;
	Element = new TiXmlElement(SettingName);
	Element->SetAttribute("Value", Value);
	Parent->InsertEndChild((TiXmlNode &)*Element);
	delete Element;
}

void xml_SaveSetting(TiXmlNode *Parent, char *SettingName, char *Value)
{
	TiXmlElement *Element;
	Element = new TiXmlElement(SettingName);
	Element->SetAttribute("Value", Value);
	Parent->InsertEndChild((TiXmlNode &)*Element);
	delete Element;
}
