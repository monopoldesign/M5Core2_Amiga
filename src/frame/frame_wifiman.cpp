/******************************************************************************
* M5Core2_Amiga
* (C)2021 M.Volkel
*
* Frame: Wifi-Manager
*******************************************************************************/

/******************************************************************************
* Header-Files
*******************************************************************************/
#include <M5Core2.h>

#include "main.h"
#include "M5Wifi.h"
#include "frame_wifiman.h"

/******************************************************************************
* Global Variables
*******************************************************************************/

/******************************************************************************
* Functions
*******************************************************************************/

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
void but_rescan(gui_args_vector_t &args)
{
	wifiNetwork *_wn;
	
	GUI_List *_li = (GUI_List *)(args[0]);
	_li->clearItemList();

	m5wifi_scanWifi();

	if (wifiNetworkList.size() > 0)
	{
		for (uint8_t i = 0; i < wifiNetworkList.size(); i++)
		{
			_wn = wifiNetworkList.get(i);
			String s = String(_wn->ssid);
			_li->addItem(s);
		}
	}

	_li->Draw();
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
void but_arrowUp(gui_args_vector_t &args)
{
	GUI_List *_li = (GUI_List *)(args[0]);
	_li->selectPrevItem();
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
void but_arrowDown(gui_args_vector_t &args)
{
	GUI_List *_li = (GUI_List *)(args[0]);
	_li->selectNextItem();
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
Frame_WifiMan::Frame_WifiMan(void)
{
	_frame_name = "Frame_WifiMan";

	_list = new GUI_List(10, 8 + (2 * 24), 200, 148);
	_but[0] = new GUI_Button(BUT_ARROWUP, 10 + 200, 8 + (2 * 24) + 148 - 64, 28, 32);
	_but[1] = new GUI_Button(BUT_ARROWDOWN, 10 + 200, 8 + (2 * 24) + 148 - 32, 28, 32);
	_slider = new GUI_Slider(10 + 200, 8 + (2 * 24), 28, 148 - (2 * 32), 0, 10, 0);
	_string = new GUI_String("\0", "\0", 10, 8 + (2 * 24) + 148 + 4, 200 + 28, 24);

	_but[2] = new GUI_Button("Ok", 10 + 200 + 28 + 8, 8 + (2 * 24) + (0 * 32), 64, 32);
	_but[3] = new GUI_Button("Exit", 10 + 200 + 28 + 8, 8 + (2 * 24) + (1 * 32) + 8, 64, 32);

	_but[0]->AddArgs(GUI_Button::EVENT_RELEASED, 0, _list);
	_but[0]->Bind(GUI_Button::EVENT_RELEASED, &but_arrowUp);

	_but[1]->AddArgs(GUI_Button::EVENT_RELEASED, 0, _list);
	_but[1]->Bind(GUI_Button::EVENT_RELEASED, &but_arrowDown);

	_but[2]->AddArgs(GUI_Button::EVENT_RELEASED, 0, _list);
	_but[2]->Bind(GUI_Button::EVENT_RELEASED, &but_rescan);

	_but[3]->AddArgs(GUI_Button::EVENT_RELEASED, 0, (void *)(&_is_run));
	_but[3]->Bind(GUI_Button::EVENT_RELEASED, &Frame_Base::exit_cb);

	exitbtn();
	_key_exit->AddArgs(GUI_Button::EVENT_RELEASED, 0, (void *)(&_is_run));
	_key_exit->Bind(GUI_Button::EVENT_RELEASED, &Frame_Base::exit_cb);
}
	
/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
Frame_WifiMan::~Frame_WifiMan(void)
{
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
int Frame_WifiMan::init(gui_args_vector_t &args)
{
	wifiNetwork *_wn;

	_is_run = 1;

	// Window
	openWindow(0, 24, 320, 216, _frame_name);

	GUI_AddObject(_list);
	_list->init();

	for (uint8_t i = 0; i < 4; i++)
	{
		GUI_AddObject(_but[i]);
		_but[i]->init();
	}

	GUI_AddObject(_slider);
	_slider->init();

	GUI_AddObject(_string);
	_string->init();

	if (wifiNetworkList.size() > 0)
	{
		for (uint8_t i = 0; i < wifiNetworkList.size(); i++)
		{
			_wn = wifiNetworkList.get(i);
			String s = String(_wn->ssid);
			_list->addItem(s);
		}
	}

	return 3;
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
int Frame_WifiMan::run()
{
	Frame_Base::run();

	if (_list->isSelected())
		_string->setValue(_list->getSelectedItem());

	return 1;
}
