/******************************************************************************
* M5Core2_Amiga
* (C)2021 M.Volkel
*
* GUI-Keyboard-Class
*******************************************************************************/

/******************************************************************************
* Header-Files
*******************************************************************************/
#include "M5Core2.h"
#include "gui_keyboard.h"

/******************************************************************************
* Global Variables
*******************************************************************************/
#define KW 26
#define KH 32
#define KSPACE 4

static const char* kKeyAlphaMapLowerCase[26] =
{
    "q", "w", "e", "r", "t", "y", "u", "i", "o", "p", //10
       "a", "s", "d", "f", "g", "h", "j", "k", "l",   //9
            "z", "x", "c", "v", "b", "n", "m",   //7
};

static const char* kKeyAlphaMapUpperCase[26] =
{
    "Q", "W", "E", "R", "T", "Y", "U", "I", "O", "P", //10
       "A", "S", "D", "F", "G", "H", "J", "K", "L",   //9
            "Z", "X", "C", "V", "B", "N", "M",   //7
};

static const char* kKeyAlphaMapNumber[26] =
{
    "1", "2", "3", "4", "5", "6", "7", "8", "9", "0", //10
       "-", "/", ":", ";", "(", ")", "$", "&", "@",   //9
            "_", "\"", ".", ",", "?", "!", "'",   //7
};

static const char* kKeyAlphaMapSymbol[26] =
{
    "[", "]", "{", "}", "#", "%", "^", "*", "+", "=", //10
       "_", "\\", "|", "~", "<", ">", " ", " ", " ",   //9
            " ", " ", " ", " ", " ", " ", " ",   //7
};

/******************************************************************************
* Functions
*******************************************************************************/

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
GUI_Keyboard::GUI_Keyboard() : GUI_Base()
{
	_string = new GUI_String("\0", "Text", WINLEFT + 8, WINTOP + 8 + 24, 320 - (2 * WINLEFT) - (2 * 8), 24);

	for (uint8_t i = 0; i < 10; i++)
		_but[i] = new GUI_Button(kKeyAlphaMapLowerCase[i], (WINLEFT + 8) + ((KW + KSPACE) * i), WINTOP + 8 + 24 + (24 + 8), KW, KH);

	for (uint8_t i = 10; i < 19; i++)
		_but[i] = new GUI_Button(kKeyAlphaMapLowerCase[i], (WINLEFT + 8) + ((KW + KSPACE) * (i - 10)) + (KW / 2), WINTOP + 8 + 24 + (24 + 8) + (KH + KSPACE), KW, KH);

	for (uint8_t i = 19; i < 26; i++)
		_but[i] = new GUI_Button(kKeyAlphaMapLowerCase[i], (WINLEFT + 8) + ((KW + KSPACE) * (i - 19)) + (KW / 2) + (KW + KSPACE), WINTOP + 8 + 24 + (24 + 8) + (2 * (KH + KSPACE)), KW, KH);

	_but[26] = new GUI_Button("<-", (WINLEFT + 8) + ((KW + KSPACE) * 7) + (KW / 2) + (KW + KSPACE), WINTOP + 8 + 24 + (24 + 8) + (2 * (KH + KSPACE)), 43, KH);
	_but[27] = new GUI_Button("Space", (WINLEFT + 8) + (KW / 2) + (2 * (KW + KSPACE)), WINTOP + 8 + 24 + (24 + 8) + (3 * (KH + KSPACE)), (4 * (KW + KSPACE) + KW), KH);
	_but[28] = new GUI_Button("Conf", (WINLEFT + 8) + (KW / 2) + (7 * (KW + KSPACE)), WINTOP + 8 + 24 + (24 + 8) + (3 * (KH + KSPACE)), 43 + (KW + KSPACE), KH);

	_sw[0] = new GUI_Switch("SH", "sh", (WINLEFT + 8), WINTOP + 8 + 24 + (24 + 8) + (2 * (KH + KSPACE)), 39, KH);
	_sw[1] = new GUI_Switch("abc", "123", (WINLEFT + 8), WINTOP + 8 + 24 + (24 + 8) + (3 * (KH + KSPACE)), 69, KH);
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
GUI_Keyboard::~GUI_Keyboard()
{
}


/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
void GUI_Keyboard::init()
{
	_event = EVENT_NONE;

	for (uint8_t i = 0; i < 29; i++)
		_but[i]->init();

	for (uint8_t i = 0; i < 2; i++)
		_sw[i]->init();
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
void GUI_Keyboard::Draw()
{
	_string->Draw();

	for (uint8_t i = 0; i < 29; i++)
		_but[i]->Draw();

	for (uint8_t i = 0; i < 2; i++)
		_sw[i]->Draw();
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
void GUI_Keyboard::Bind(int16_t event, void (*func_cb)(gui_args_vector_t&))
{
	if (event == EVENT_FINISHED)
		_finished_cb = func_cb;
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
void GUI_Keyboard::UpdateState(TouchPoint_t pos)
{
	for (uint8_t i = 0; i < 29; i++)
	{
		_but[i]->UpdateState(pos);

		if (_but[i]->isPressed())
		{
			if (i < 26)
			{
				switch (_layout)
				{
					case 0:
						_data += kKeyAlphaMapLowerCase[i];
						break;
					case 1:
						_data += kKeyAlphaMapUpperCase[i];
						break;
					case 2:
						_data += kKeyAlphaMapNumber[i];
						break;
					case 3:
						_data += kKeyAlphaMapSymbol[i];
						break;
				}
				setString();
			}
			else
			{
				switch (i)
				{
					case 26:
						_data = _data.substring(0, _data.length() - 1);
						setString();
						break;
					case 27:
						_data += " ";
						setString();
						break;
					case 28:
						_event = EVENT_FINISHED;

						if (_finished_cb != NULL)
							_finished_cb(_finished_cb_args);

						break;
				}
			}
		}
	}

	for (uint8_t i = 0; i < 2; i++)
	{
		_sw[i]->UpdateState(pos);

		if (_sw[i]->isPressed())
		{
			switch (i)
			{
				case 0:
					if ((_layout / 2) == 0)
					{
						if (_layout == 0)
							_layout = 1;
						else
							_layout = 0;
					}
					else
					{
						if (_layout == 2)
							_layout = 3;
						else
							_layout = 2;
					}

					break;
				case 1:
					if ((_layout / 2) == 0)
						_layout = 2;
					else
						_layout = 0;
					break;
			}

			for (uint8_t i = 0; i < 26; i++)
			{
				switch (_layout)
				{
					case 0:
						_but[i]->setLabel(kKeyAlphaMapLowerCase[i]);
						break;
					case 1:
						_but[i]->setLabel(kKeyAlphaMapUpperCase[i]);
						break;
					case 2:
						_but[i]->setLabel(kKeyAlphaMapNumber[i]);
						break;
					case 3:
						_but[i]->setLabel(kKeyAlphaMapSymbol[i]);
						break;
				}
			}

			for (uint8_t i = 0; i < 26; i++)
				_but[i]->Draw();
		}
	}
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
void GUI_Keyboard::UpdatePosition(TouchPoint_t pos)
{
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
void GUI_Keyboard::AddArgs(int16_t event, uint16_t n, void* arg)
{
	if (event == EVENT_FINISHED)
	{
		if (_finished_cb_args.size() > n)
			_finished_cb_args[n] = arg;
		else
			_finished_cb_args.push_back(arg);
	}
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
void GUI_Keyboard::setData(String text)
{
	_data = text;
	setString();
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
String GUI_Keyboard::getData()
{
	return _data;
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
void GUI_Keyboard::setString()
{
	_string->setValue(_data + "|");
	_string->Draw();
}
