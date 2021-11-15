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
#include "main.h"
#include "M5Settings.h"
#include "M5Wifi.h"

/******************************************************************************
* Global Variables
*******************************************************************************/
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 0;
const int   daylightOffset_sec = 3600;
struct tm timeinfo;

/******************************************************************************
* Functions
*******************************************************************************/

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
void m5wifi_initWifi(void)
{
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
	uint8_t n = WiFi.scanNetworks();

	for (uint8_t i = 0; i < n; i++)
	{
		Serial.print(i + 1);
		Serial.print(": ");
		Serial.print(WiFi.SSID(i));
		Serial.print(" (");
		Serial.print(WiFi.RSSI(i));
		Serial.print(")");
		Serial.println((WiFi.encryptionType(i) == WIFI_AUTH_OPEN) ? " " : "*");
		delay(10);
	}
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
			Serial.println("Failed to obtain time");
			return;
		}
		else
		{
			globalSettings->isNTPTime = true;
			globalSettings->hour = timeinfo.tm_hour;
			globalSettings->hour = timeinfo.tm_min;
		}

		//m5wifi_printLocalTime();
	}
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
