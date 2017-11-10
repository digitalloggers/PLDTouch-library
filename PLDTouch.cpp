#include <SPI.h>
#include "PLDTouch.h"
#include "calibration.h"
word ctl(byte command);


PLDTouch::PLDTouch(byte tcs, byte irq, long dispw, long disph)
  : rotation(PLDTOUCH_NO_ROTATION),
    display_width(dispw),
    display_height(disph),
    T_CS(tcs), T_IRQ(irq)
{
}


void PLDTouch::init(byte rotation)
{
	pinMode(T_IRQ, INPUT); digitalWrite(T_IRQ, HIGH);
    pinMode(T_CS, OUTPUT); digitalWrite(T_CS, HIGH);
	this->rotation = rotation;
}


Point PLDTouch::read (int precision)
{
	unsigned long tmpx=0, tmpy=0;
	int datacount = 0;
	
	SPI.begin();
	SPI.setClockDivider(SPI_CLOCK_DIV32);
	digitalWrite(T_CS, LOW);
	for(int i=0; datacount<precision && i<precision*2; i++)
	{
		unsigned long x = ctl(0x90);
		unsigned long y = ctl(0xD0);
		if (x!=0 && y!=0)
		{
			tmpx += x;
			tmpy += y;
			datacount++;
		}
	}
	digitalWrite(T_CS, HIGH);
	SPI.end();

	if (datacount==0) return Point();

	const long touch_x = tmpx/datacount - PLDTOUCH_CAL_LEFT;
	const long touch_y = tmpy/datacount - PLDTOUCH_CAL_TOP;
	const long touch_width = PLDTOUCH_CAL_RIGHT - PLDTOUCH_CAL_LEFT;
	const long touch_height = PLDTOUCH_CAL_BOTTOM - PLDTOUCH_CAL_TOP;
	const long display_x = touch_x * display_width / touch_width;
	const long display_y = touch_y * display_height / touch_height;

	switch(rotation)
	{
		case PLDTOUCH_NO_ROTATION:
			return Point(display_x, display_y);
			break;
		case PLDTOUCH_ROTATE_90:
			return Point(display_y, display_width - display_x);
			break;
		case PLDTOUCH_ROTATE_180:
			return Point(display_width - display_x, display_height - display_y);
			break;
		case PLDTOUCH_ROTATE_270:
			return Point(display_height - display_y, display_x);
			break;
	}
}


bool PLDTouch::dataAvailable()
{
	return !digitalRead(T_IRQ);
}


Point PLDTouch::readRaw()
{
	SPI.begin();
	digitalWrite(T_CS, LOW);
	unsigned long tx=ctl(0x90);
	unsigned long ty=ctl(0xD0);
	digitalWrite(T_CS, HIGH);
	SPI.end();
	return Point(tx, ty);
}


word ctl(byte command)
{
	SPI.transfer(command);
	byte f1 = SPI.transfer(0x00);
	byte f2 = SPI.transfer(0x00);
	return word(f1, f2) >> 3;
}
