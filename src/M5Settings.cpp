/******************************************************************************
* M5Core2_Amiga
* (C)2021 M.Volkel
*
* Settings-Functions
*******************************************************************************/

// Comment templates

/******************************************************************************
*
*******************************************************************************/

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/

/******************************************************************************/

/******************************************************************************
* Header-Files
*******************************************************************************/
#include "M5Settings.h"

/******************************************************************************
* Global Variables
*******************************************************************************/
struct Settings *globalSettings;
LinkedList<savedWifiNetwork *> savedWifiNetworkList = LinkedList<savedWifiNetwork *>();

/******************************************************************************
* Functions
*******************************************************************************/

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
void m5set_initSettings(void)
{
	globalSettings = (struct Settings *)calloc(1, sizeof(struct Settings));
	globalSettings->isWifiConnected = false;
	globalSettings->isNTPTime = false;
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
void m5set_createSettings(void)
{
	savedWifiNetwork *_swn;

	_swn = new savedWifiNetwork();
	sprintf(_swn->ssid, "Size");
	savedWifiNetworkList.add(_swn);

	_swn = new savedWifiNetwork();
	sprintf(_swn->ssid, "NSA-Surveillance");
	sprintf(_swn->pwd, "laganas.2017");
	savedWifiNetworkList.add(_swn);

	_swn = new savedWifiNetwork();
	sprintf(_swn->ssid, "Mario's iOS");
	sprintf(_swn->pwd, "xtesta.0815");
	savedWifiNetworkList.add(_swn);

	_swn = savedWifiNetworkList.get(0);
	sprintf(_swn->ssid, "%d", savedWifiNetworkList.size());
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
void m5set_saveSettings(void)
{
	String fileName = "/config.txt";
	DynamicJsonDocument doc(1024);
	savedWifiNetwork *_swn;

    File dataFile = SPIFFS.open(fileName, "w");

	// create JSON-Document
	for (uint8_t i = 0; i < savedWifiNetworkList.size(); i++)
	{
		_swn = savedWifiNetworkList.get(i);
		doc[i]["ssid"] = _swn->ssid;
		doc[i]["pwd"] = _swn->pwd;
	}

	// serialize JSON
	if (serializeJson(doc, dataFile) == 0)
		Serial.println(F("Failed to write to file"));

	dataFile.close();
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
void m5set_loadSettings(void)
{
	String fileName = "/config.txt";
	DynamicJsonDocument doc(1024);
	savedWifiNetwork *_swn;

	if (SPIFFS.exists(fileName))
	{
		File dataFile = SPIFFS.open(fileName, "r");

		DeserializationError error = deserializeJson(doc, dataFile);

		if (error)
		{
			Serial.println("Failed to read file!");
			return;
		}

		dataFile.close();

		uint8_t _size = atoi(doc[0]["ssid"]);
		m5set_clearSettings();

		_swn = new savedWifiNetwork();
		sprintf(_swn->ssid, "%d", _size);
		savedWifiNetworkList.add(_swn);

		for (uint8_t i = 1; i < _size; i++)
		{
			_swn = new savedWifiNetwork();
			strlcpy(_swn->ssid, doc[i]["ssid"], sizeof(_swn->ssid));
			strlcpy(_swn->pwd, doc[i]["pwd"], sizeof(_swn->pwd));
			savedWifiNetworkList.add(_swn);
		}

		//m5set_printSavedWifiList();
	}
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
void m5set_clearSettings(void)
{
	savedWifiNetwork *_swn;

	if (savedWifiNetworkList.size() > 0)
	{
		while (savedWifiNetworkList.size() > 0)
		{
			_swn = savedWifiNetworkList.get(0);
			savedWifiNetworkList.remove(0);
			delete _swn;
		}
	}
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
void m5set_printSavedWifiList(void)
{
	savedWifiNetwork *_swn;

	for (uint8_t i = 0; i < savedWifiNetworkList.size(); i++)
	{
		_swn = savedWifiNetworkList.get(i);
		Serial.print("Network ");
		Serial.print(i);
		Serial.print(": ");
		Serial.print("SSID: ");
		Serial.print(_swn->ssid);
		Serial.print(", PWD: ");
		Serial.println(_swn->pwd);
	}
}
