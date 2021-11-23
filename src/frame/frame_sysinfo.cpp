/******************************************************************************
* M5Core2_Amiga
* (C)2021 M.Volkel
*
* Frame: SysInfo
*******************************************************************************/

/******************************************************************************
* Header-Files
*******************************************************************************/
#include <M5Core2.h>

#include "main.h"
#include "frame_sysinfo.h"

#define STRINGSIZE	100

/******************************************************************************
* Global Variables
*******************************************************************************/

/******************************************************************************
* Functions
*******************************************************************************/

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
Frame_SysInfo::Frame_SysInfo(void)
{
	_frame_name = "Frame_SysInfo";

	_string[0] = new GUI_String("Flash-Size:", ESP.getFlashChipSize(), 320 - STRINGSIZE - 16, (0 * (24 + 4)) + 56 + 4, STRINGSIZE, 24);
	_string[1] = new GUI_String("Flash-Speed (MHz):", ESP.getFlashChipSpeed() / 1000000, 320 - STRINGSIZE - 16, (1 * (24 + 4)) + 56 + 4, STRINGSIZE, 24);
	_string[2] = new GUI_String("Heap:", ESP.getHeapSize(), 320 - STRINGSIZE - 16, (2 * (24 + 4)) + 56 + 4, STRINGSIZE, 24);
	_string[3] = new GUI_String("Free Heap:", ESP.getFreeHeap(), 320 - STRINGSIZE - 16, (3 * (24 + 4)) + 56 + 4, STRINGSIZE, 24);
	_string[4] = new GUI_String("Free PSRAM:", ESP.getFreePsram(), 320 - STRINGSIZE - 16, (4 * (24 + 4)) + 56 + 4, STRINGSIZE, 24);
	_string[5] = new GUI_String("Min.Heap:", esp_get_minimum_free_heap_size(), 320 - STRINGSIZE - 16, (5 * (24 + 4)) + 56 + 4, STRINGSIZE, 24);

	exitbtn();
	_key_exit->AddArgs(EVENT_RELEASED, 0, (void *)(&_is_run));
	_key_exit->Bind(EVENT_RELEASED, &Frame_Base::exit_cb);
}
	
/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
Frame_SysInfo::~Frame_SysInfo(void)
{
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
int Frame_SysInfo::init(gui_args_vector_t &args)
{
	_is_run = 1;

	// Window
	openWindow(0, 24, 320, 216, _frame_name);

	// Bevelbox
	bevel(8, 56, 320 - (2 * 8), 240 - (2 * 24) - (2 * 8), BT_RECESSED);

	for (uint8_t i = 0; i < 6; i++)
	{
		GUI_AddObject(_string[i]);
		_string[i]->init();
	}

	return 3;
}
