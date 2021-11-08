#ifndef _GUI_BASE_H
#define _GUI_BASE_H

#include <Arduino.h>
#include <M5Core2.h>
#include <vector>

typedef std::vector<void *> gui_args_vector_t;

class GUI_Base
{
	public:
		GUI_Base(int16_t x, int16_t y, int16_t w, int16_t h);
		GUI_Base();
		virtual ~GUI_Base() {};
		virtual void Draw() = 0;
		virtual void UpdateState(TouchPoint_t pos) = 0;
		void SetID(uint32_t id) { _id = id; }

		inline int16_t getX() { return _x; }
		inline int16_t getY() { return _y; }
		inline int16_t getW() { return _w; }
		inline int16_t getH() { return _h; }

	protected:
		int16_t _x, _y, _w, _h;
		uint32_t _id;
		bool _ishide = false;
};

#endif
