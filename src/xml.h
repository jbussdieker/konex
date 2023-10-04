void xml_GetSetting(TiXmlNode *Parent, char *SettingName, int *Value);
void xml_GetSetting(TiXmlNode *Parent, char *SettingName, float *Value);
void xml_GetSetting(TiXmlNode *Parent, char *SettingName, char *Value);

void xml_SaveSetting(TiXmlNode *Parent, char *SettingName, int Value);
void xml_SaveSetting(TiXmlNode *Parent, char *SettingName, float Value);
void xml_SaveSetting(TiXmlNode *Parent, char *SettingName, char *Value);
