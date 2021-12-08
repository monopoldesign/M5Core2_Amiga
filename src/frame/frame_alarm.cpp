/******************************************************************************
* M5Core2_Amiga
* (C)2021 M.Volkel
*
* Frame: Alarm-Application
*******************************************************************************/

/******************************************************************************
* Header-Files
*******************************************************************************/
#include <M5Core2.h>

#include "main.h"
#include "frame_alarm.h"

/******************************************************************************
* Global Variables
*******************************************************************************/
RTC_TimeTypeDef RTCTimeNow;

/******************************************************************************
* Functions
*******************************************************************************/

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
void falrm_activate(gui_args_vector_t &args)
{
	M5.Rtc.GetTime(&RTCTimeNow);

	Serial.print("Time now: ");
	Serial.print(RTCTimeNow.Hours);
	Serial.print(":");
	Serial.print(RTCTimeNow.Minutes);
	Serial.print(":");
	Serial.print(RTCTimeNow.Seconds);
	Serial.println();

	RTCTimeNow.Minutes += 1;

	Serial.print("Wake up at: ");
	Serial.print(RTCTimeNow.Hours);
	Serial.print(":");
	Serial.print(RTCTimeNow.Minutes);
	Serial.print(":");
	Serial.print(RTCTimeNow.Seconds);
	Serial.println();

	M5.shutdown(RTCTimeNow);
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
Frame_Alarm::Frame_Alarm(void)
{
	_frame_name = "Frame_Alarm";

	_but = new GUI_Button("Activate Alarm", 4 + 8, (2 * 24) + (1 * 8), 320 - (2 * (4 + 8)), 32);
	_but->Bind(EVENT_RELEASED, &falrm_activate);

	exitbtn();
	_key_exit->AddArgs(EVENT_RELEASED, 0, (void *)(&_is_run));
	_key_exit->Bind(EVENT_RELEASED, &exit_cb);
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
Frame_Alarm::~Frame_Alarm(void)
{
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
int Frame_Alarm::init(gui_args_vector_t &args)
{
	_is_run = 1;

	// Window
	openWindow(0, 24, 320, 216, _frame_name);

	// Button
	GUI_AddObject(_but);
	_but->init();

	return 3;
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
int Frame_Alarm::run()
{
	Frame_Base::run();
	return 1;
}
