/******************************************************************************
* M5Core2_Amiga
* (C)2021 M.Volkel
*
* Wifi-Functions
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
#include <M5Core2.h>

#include "main.h"
#include "M5RTC.h"
#include "M5Settings.h"
#include "M5Wifi.h"

/******************************************************************************
* Prototypes
*******************************************************************************/
int m5wifi_compare(wifiNetwork *&a, wifiNetwork *&b);

/******************************************************************************
* Global Variables
*******************************************************************************/
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 0;
const int   daylightOffset_sec = 3600;
struct tm timeinfo;

LinkedList<wifiNetwork *> wifiNetworkList = LinkedList<wifiNetwork *>();

/******************************************************************************
* Functions
*******************************************************************************/

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
void m5wifi_initWifi(void)
{
	WiFi.disconnect();
	WiFi.softAPdisconnect(true);

	WiFi.mode(WIFI_STA);
	WiFi.begin(globalSettings->wifiSsid, globalSettings->wifiPwd);

	uint32_t _time = millis();

	while (WiFi.status() != WL_CONNECTED)
	{
		if (millis() - _time > 10000)
			break;

		Serial.print(".");
		delay(1000);
	}

	Serial.println();

	if (WiFi.status() == WL_CONNECTED)
	{
		M5.Axp.SetLed(1);
		globalSettings->isWifiConnected = true;
		Serial.print("IP: ");
		Serial.println(WiFi.localIP());
	}
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
void m5wifi_scanWifi(void)
{
	wifiNetwork *_wn;

	m5wifi_clearWifiList();
	uint8_t n = WiFi.scanNetworks();

	for (uint8_t i = 0; i < n; i++)
	{
		_wn = new wifiNetwork();
		_wn->isSaved = false;
		strncpy(_wn->ssid, WiFi.SSID(i).c_str(), sizeof(_wn->ssid));
		_wn->rssi = WiFi.RSSI(i);
		wifiNetworkList.add(_wn);
		delay(10);
	}
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
void m5wifi_printWifiList(void)
{
	wifiNetwork *_wn;

	if (wifiNetworkList.size() > 0)
	{
		for (uint8_t i = 0; i < wifiNetworkList.size(); i++)
		{
			_wn = wifiNetworkList.get(i);

			Serial.print(i + 1);
			Serial.print(": ");
			Serial.print(_wn->ssid);
			Serial.print(" (");
			Serial.print(_wn->rssi);
			Serial.print(") - ");

			if (_wn->isSaved)
				Serial.print("saved");
			else
				Serial.print("unknown");

			Serial.println();
		}
	}
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
void m5wifi_clearWifiList(void)
{
	wifiNetwork *_wn;

	if (wifiNetworkList.size() > 0)
	{
		while (wifiNetworkList.size() > 0)
		{
			_wn = wifiNetworkList.get(0);
			wifiNetworkList.remove(0);
			delete _wn;
		}
	}
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
void m5wifi_findWifi(void)
{
	wifiNetwork *_wn;
	savedWifiNetwork *_swn;

	if (wifiNetworkList.size() > 0)
	{
		for (uint8_t i = 0; i < wifiNetworkList.size(); i++)
		{
			_wn = wifiNetworkList.get(i);

			if (savedWifiNetworkList.size() > 0)
			{
				for (uint8_t j = 0; j < savedWifiNetworkList.size(); j++)
				{
					_swn = savedWifiNetworkList.get(j);
					if (strcmp(_wn->ssid, _swn->ssid) == 0)
					{
						_wn->isSaved = true;
						strncpy(_wn->pwd, _swn->pwd, sizeof(_wn->pwd));
					}
				}
			}
		}
		//wifiNetworkList.sort(m5wifi_compare);
	}
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
int m5wifi_compare(wifiNetwork *&a, wifiNetwork *&b)
{
	if (a->rssi < b->rssi)
		return -1;
	else
		return 1;
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
uint8_t m5wifi_setWifi()
{
	wifiNetwork *_wn;

	if (wifiNetworkList.size() > 0)
	{
		for (uint8_t i = 0; i < wifiNetworkList.size(); i++)
		{
			_wn = wifiNetworkList.get(i);

			if (_wn->isSaved)
			{
				strlcpy(globalSettings->wifiSsid, _wn->ssid, sizeof(globalSettings->wifiSsid));
				strlcpy(globalSettings->wifiPwd, _wn->pwd, sizeof(globalSettings->wifiPwd));
				return 1;
			}
		}
	}
	return 0;
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
void m5wifi_getNTPTime(void)
{
	if (globalSettings->isWifiConnected)
	{
		configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

		if (!getLocalTime(&timeinfo))
		{
			m5rtc_getTime();
			Serial.println("Failed to obtain time");
			return;
		}
		else
		{
			globalSettings->isNTPTime = true;
			globalSettings->hour = timeinfo.tm_hour;
			globalSettings->hour = timeinfo.tm_min;

			m5rtc_setupTime();
		}
	}
	else
		m5rtc_getTime();
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
void m5wifi_printLocalTime()
{
	struct tm timeinfo;

	if (!getLocalTime(&timeinfo))
	{
		Serial.println("Failed to obtain time");
		return;
	}

	Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
	Serial.print("Day of week: ");
	Serial.println(&timeinfo, "%A");
	Serial.print("Month: ");
	Serial.println(&timeinfo, "%B");
	Serial.print("Day of Month: ");
	Serial.println(&timeinfo, "%d");
	Serial.print("Year: ");
	Serial.println(&timeinfo, "%Y");
	Serial.print("Hour: ");
	Serial.println(&timeinfo, "%H");
	Serial.print("Hour (12 hour format): ");
	Serial.println(&timeinfo, "%I");
	Serial.print("Minute: ");
	Serial.println(&timeinfo, "%M");
	Serial.print("Second: ");
	Serial.println(&timeinfo, "%S");

	Serial.println("Time variables");
	char timeHour[3];
	strftime(timeHour,3, "%H", &timeinfo);
	Serial.println(timeHour);
	char timeWeekDay[10];
	strftime(timeWeekDay,10, "%A", &timeinfo);
	Serial.println(timeWeekDay);
	Serial.println();
}
