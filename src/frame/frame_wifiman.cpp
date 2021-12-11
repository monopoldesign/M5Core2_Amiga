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
	GUI_Slider *_slid = (GUI_Slider *)(args[1]);

	_li->init();
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

	_li->setSelectedMin(0);
	_li->setSelectedItem(0);
	_li->Draw();

	// configure Slider
	_slid->setMin(0);
	_slid->setMax(_li->getSize());
	_slid->setLevel(0);
	_slid->setSelectedMin(0);
	_slid->calculate();
	_slid->init();
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
void but_arrowUp(gui_args_vector_t &args)
{
	GUI_List *_li = (GUI_List *)(args[0]);
	GUI_Slider *_slid = (GUI_Slider *)(args[1]);
	GUI_String *_str = (GUI_String *)(args[2]);

	_li->selectPrevItem();

	_str->setValue(_li->getSelectedItem());

	uint8_t min = _li->getSelectedMin();
	_slid->setSelectedMin(min);
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
void but_arrowDown(gui_args_vector_t &args)
{
	GUI_List *_li = (GUI_List *)(args[0]);
	GUI_Slider *_slid = (GUI_Slider *)(args[1]);
	GUI_String *_str = (GUI_String *)(args[2]);

	_li->selectNextItem();

	_str->setValue(_li->getSelectedItem());

	uint8_t min = _li->getSelectedMin();
	_slid->setSelectedMin(min);
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
void sliderMove(gui_args_vector_t &args)
{
	GUI_Slider *_slid = (GUI_Slider *)(args[0]);
	GUI_List *_li = (GUI_List *)(args[1]);

	uint8_t smin = _slid->getSelectedMin();
	uint8_t lmin = _li->getSelectedMin();

	if (smin != lmin)
	{
		_li->setSelectedMin(smin);
		_li->Draw();
	}
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
void list_press(gui_args_vector_t &args)
{
	GUI_List *_li = (GUI_List *)(args[0]);
	GUI_String *_str = (GUI_String *)(args[1]);

	_str->setValue(_li->getSelectedItem());
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
Frame_WifiMan::Frame_WifiMan(void)
{
	_frame_name = "Frame_WifiMan";

	_string = new GUI_String("\0", "\0", 10, 8 + (2 * 24) + 148 + 4, 200 + 28, 24);

	_list = new GUI_List(10, 8 + (2 * 24), 200, 148);
	_list->AddArgs(EVENT_RELEASED, 0, _list);
	_list->AddArgs(EVENT_RELEASED, 1, _string);
	_list->Bind(EVENT_RELEASED, &list_press);

	_but[0] = new GUI_Button(BUT_ARROWUP, 10 + 200, 8 + (2 * 24) + 148 - 64, 28, 32);
	_but[0]->AddArgs(EVENT_RELEASED, 0, _list);
	_but[0]->AddArgs(EVENT_RELEASED, 1, _slider);
	_but[0]->AddArgs(EVENT_RELEASED, 2, _string);
	_but[0]->Bind(EVENT_RELEASED, &but_arrowUp);

	_but[1] = new GUI_Button(BUT_ARROWDOWN, 10 + 200, 8 + (2 * 24) + 148 - 32, 28, 32);
	_but[1]->AddArgs(EVENT_RELEASED, 0, _list);
	_but[1]->AddArgs(EVENT_RELEASED, 1, _slider);
	_but[1]->AddArgs(EVENT_RELEASED, 2, _string);
	_but[1]->Bind(EVENT_RELEASED, &but_arrowDown);

	_slider = new GUI_Slider(10 + 200, 8 + (2 * 24), 28, 148);
	_slider->AddArgs(EVENT_MOVED, 0, _slider);
	_slider->AddArgs(EVENT_MOVED, 1, _list);
	_slider->Bind(EVENT_MOVED, &sliderMove);

	_but[2] = new GUI_Button("Ok", 10 + 200 + 28 + 8, 8 + (2 * 24) + (0 * 32), 64, 32);
	_but[2]->AddArgs(EVENT_RELEASED, 0, _list);
	_but[2]->AddArgs(EVENT_RELEASED, 1, _slider);
	_but[2]->Bind(EVENT_RELEASED, &but_rescan);

	_but[3] = new GUI_Button("Exit", 10 + 200 + 28 + 8, 8 + (2 * 24) + (1 * 32) + (1 * 8), 64, 32);
	_but[3]->AddArgs(EVENT_RELEASED, 0, (void *)(&_is_run));
	_but[3]->Bind(EVENT_RELEASED, &Frame_Base::exit_cb);

	_cb = new GUI_Checkbox("Ok?", 10 + 200 + 28 + 8 + 32, 8 + (2 * 24) + (2 * 32) + (2 * 8), PM_LEFT);

	exitbtn();
	_key_exit->AddArgs(EVENT_RELEASED, 0, (void *)(&_is_run));
	_key_exit->Bind(EVENT_RELEASED, &Frame_Base::exit_cb);
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

	/*
	if (wifiNetworkList.size() > 0)
	{
		for (uint8_t i = 0; i < wifiNetworkList.size(); i++)
		{
			_wn = wifiNetworkList.get(i);
			String s = String(_wn->ssid);
			_list->addItem(s);
		}
	}
	*/
	for (uint8_t i = 0; i < 20; i++)
	{
		String s = "Item" + String(i + 1);
		_list->addItem(s);
	}


	// configure Slider
	_slider->setMin(0);
	//_slider->setMax(_list->getSize());
	_slider->setMax(20);
	_slider->setLevel(0);
	_slider->setSelectedMin(0);
	_slider->calculate();

	GUI_AddObject(_slider);
	GUI_AddObject_Move(_slider);
	_slider->init();

	for (uint8_t i = 0; i < 4; i++)
	{
		GUI_AddObject(_but[i]);
		_but[i]->init();
	}

	GUI_AddObject(_string);
	_string->init();

	GUI_AddObject(_cb);
	_cb->init();

	return 3;
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
int Frame_WifiMan::run()
{
	Frame_Base::run();
	return 1;
}
