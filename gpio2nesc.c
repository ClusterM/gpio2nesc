#include "gpio2nesc.h"
#include "defines.h"
#include <util/delay.h>
#include <avr/eeprom.h>
#include <inttypes.h>
#include "wiimote.h"
#include <avr/wdt.h>

// classic controller id
const unsigned char classic_controller_id[6] = {0x00, 0x00, 0xA4, 0x20, 0x01, 0x01};

volatile int jx = 0, jy = 0, rx = 0, ry = 0, tl = 0, tr = 0;
volatile uint8_t left = 0, right = 0, up = 0, down = 0, a = 0, b = 0, x = 0, y = 0, 
				select = 0, start = 0, home = 0, l = 0, r = 0, zl = 0, zr = 0;

// calibration data
const unsigned char cal_data[32] = {
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00
};

void wiimote_query()
{
	wdt_reset();
	RED_ON;
	unsigned char but_dat[8]; // struct containing button data
	if (twi_reg[0xFE] == 1) // data format
	{
		but_dat[0] = 0b00000000; // RX<4:3>	LX<5:0>
		but_dat[1] = 0b00000000; // RX<2:1>	LY<5:0>
		but_dat[2] = 0b00000000; // RX<0>	LT<4:3>	RY<4:0>
		but_dat[3] = 0b00000000; // LT<2:0>	RT<4:0>
		but_dat[4] = 0b11111111; // BDR	BDD	BLT	B-	BH	B+	BRT	 1
		but_dat[5] = 0b11111111; // BZL	BB	BY	BA	BX	BZR	BDL	BDU
		but_dat[6] = 0;
		but_dat[7] = 0;
		
		but_dat[0] |= (jx / 4 + 0x20) & 0x3F;
		but_dat[1] |= (jy / -4 + 0x20) & 0x3F;
		but_dat[0] |= ((rx / 8 + 0x10) & 0x18) << 3;
		but_dat[1] |= ((rx / 8 + 0x10) & 0x06) << 5;
		but_dat[2] |= ((rx / 8 + 0x10) & 0x01) << 7;
		but_dat[2] |= (ry / -8 + 0x10) & 0x1F;
		but_dat[2] |= ((tl / 8) & 0x18) << 2;
		but_dat[3] |= ((tl / 8) & 0x07) << 5;
		but_dat[3] |= (tr / 8) & 0x1F;

		if (right) but_dat[4] &= ~(1<<7);
		if (down) but_dat[4] &= ~(1<<6);
		if (l) but_dat[4] &= ~(1<<5);
		if (select) but_dat[4] &= ~(1<<4);
		if (home) but_dat[4] &= ~(1<<3);
		if (start) but_dat[4] &= ~(1<<2);
		if (r) but_dat[4] &= ~(1<<1);
		
		if (zl) but_dat[5] &= ~(1<<7);
		if (b) but_dat[5] &= ~(1<<6);
		if (y) but_dat[5] &= ~(1<<5);
		if (a) but_dat[5] &= ~(1<<4);
		if (x) but_dat[5] &= ~(1<<3);
		if (zr) but_dat[5] &= ~(1<<2);
		if (left) but_dat[5] &= ~(1<<1);
		if (up) but_dat[5] &= ~(1<<0);
		
		but_dat[6] = 0;
		but_dat[7] = 0;
		
		if (((but_dat[4] & 0xFE) == 0xFE) && (but_dat[5] == 0xFF))
			GREEN_OFF;
		else
			GREEN_ON;
		
		wm_newaction(but_dat);
	} 
	else if (twi_reg[0xFE] == 3) // data format
	{
		but_dat[0] = jx + 0x80;
		but_dat[1] = rx + 0x80;
		but_dat[2] = 0x7fl - jy;
		but_dat[3] = 0x7fl - ry;
		but_dat[4] = tl;
		but_dat[5] = tr;
			
		but_dat[6] = 0b11111111; // BDR	BDD	BLT	B-	BH	B+	BRT	 1
		but_dat[7] = 0b11111111; // BZL	BB	BY	BA	BX	BZR	BDL	BDU
			
		if (right) but_dat[6] &= ~(1<<7);
		if (down) but_dat[6] &= ~(1<<6);
		if (l) but_dat[6] &= ~(1<<5);
		if (select) but_dat[6] &= ~(1<<4);
		if (home) but_dat[6] &= ~(1<<3);
		if (start) but_dat[6] &= ~(1<<2);
		if (r) but_dat[6] &= ~(1<<1);

		if (zl) but_dat[7] &= ~(1<<7);
		if (b) but_dat[7] &= ~(1<<6);
		if (y) but_dat[7] &= ~(1<<5);
		if (a) but_dat[7] &= ~(1<<4);
		if (x) but_dat[7] &= ~(1<<3);
		if (zr) but_dat[7] &= ~(1<<2);
		if (left) but_dat[7] &= ~(1<<1);
		if (up) but_dat[7] &= ~(1<<0);

		if (((but_dat[6] & 0xFE) == 0xFE) && (but_dat[7] == 0xFF))
			GREEN_OFF;
		else
			GREEN_ON;

		wm_newaction(but_dat);
	} 
}

int main()
{
	DDR(RED_LED_PORT) |= (1<<RED_LED_PIN); // Red led, output
	DDR(GREEN_LED_PORT) |= (1<<GREEN_LED_PIN); // Red led, output
	RED_ON;
	GREEN_ON;
	_delay_ms(100);
	RED_OFF;
	GREEN_OFF;
#ifdef BUTTON_A_PIN
	DDR(BUTTON_A_PORT) &= ~(1<<BUTTON_A_PIN);
	PORT(BUTTON_A_PORT) |= 1<<BUTTON_A_PIN;
#endif
#ifdef BUTTON_B_PIN
	DDR(BUTTON_B_PORT) &= ~(1<<BUTTON_B_PIN);
	PORT(BUTTON_B_PORT) |= 1<<BUTTON_B_PIN;
#endif
#ifdef BUTTON_X_PIN
	DDR(BUTTON_X_PORT) &= ~(1<<BUTTON_X_PIN);
	PORT(BUTTON_X_PORT) |= 1<<BUTTON_X_PIN;
#endif
#ifdef BUTTON_Y_PIN
	DDR(BUTTON_Y_PORT) &= ~(1<<BUTTON_Y_PIN);
	PORT(BUTTON_Y_PORT) |= 1<<BUTTON_Y_PIN;
#endif
#ifdef BUTTON_UP_PIN
	DDR(BUTTON_UP_PORT) &= ~(1<<BUTTON_UP_PIN);
	PORT(BUTTON_UP_PORT) |= 1<<BUTTON_UP_PIN;
#endif
#ifdef BUTTON_DOWN_PIN
	DDR(BUTTON_DOWN_PORT) &= ~(1<<BUTTON_DOWN_PIN);
	PORT(BUTTON_DOWN_PORT) |= 1<<BUTTON_DOWN_PIN;
#endif
#ifdef BUTTON_LEFT_PIN
	DDR(BUTTON_LEFT_PORT) &= ~(1<<BUTTON_LEFT_PIN);
	PORT(BUTTON_LEFT_PORT) |= 1<<BUTTON_LEFT_PIN;
#endif
#ifdef BUTTON_RIGHT_PIN
	DDR(BUTTON_RIGHT_PORT) &= ~(1<<BUTTON_RIGHT_PIN);
	PORT(BUTTON_RIGHT_PORT) |= 1<<BUTTON_RIGHT_PIN;
#endif
#ifdef BUTTON_SELECT_PIN
	DDR(BUTTON_SELECT_PORT) &= ~(1<<BUTTON_SELECT_PIN);
	PORT(BUTTON_SELECT_PORT) |= 1<<BUTTON_SELECT_PIN;
#endif
#ifdef BUTTON_START_PIN
	DDR(BUTTON_START_PORT) &= ~(1<<BUTTON_START_PIN);
	PORT(BUTTON_START_PORT) |= 1<<BUTTON_START_PIN;
#endif
#ifdef BUTTON_L_PIN
	DDR(BUTTON_L_PORT) &= ~(1<<BUTTON_L_PIN);
	PORT(BUTTON_L_PORT) |= 1<<BUTTON_L_PIN;
#endif
#ifdef BUTTON_R_PIN
	DDR(BUTTON_R_PORT) &= ~(1<<BUTTON_R_PIN);
	PORT(BUTTON_R_PORT) |= 1<<BUTTON_R_PIN;
#endif
#ifdef BUTTON_ZL_PIN
	DDR(BUTTON_ZL_PORT) &= ~(1<<BUTTON_ZL_PIN);
	PORT(BUTTON_ZL_PORT) |= 1<<BUTTON_ZL_PIN;
#endif
#ifdef BUTTON_ZR_PIN
	DDR(BUTTON_ZR_PORT) &= ~(1<<BUTTON_ZR_PIN);
	PORT(BUTTON_ZR_PORT) |= 1<<BUTTON_ZR_PIN;
#endif
#ifdef BUTTON_HOME_PIN
	DDR(BUTTON_HOME_PORT) &= ~(1<<BUTTON_HOME_PIN);
	PORT(BUTTON_HOME_PORT) |= 1<<BUTTON_HOME_PIN;
#endif
	uint8_t connected = 0;

	while(1)
	{
		if (!connected)
		{
			wm_init((void*)classic_controller_id, (void*)cal_data, wiimote_query);
			connected = 1;
			wdt_enable(WDTO_2S);
		}

		a = BTN_A;
		b = BTN_B;
		x = BTN_X;
		y = BTN_Y;
		select = BTN_SELECT;
		start = BTN_START;
		up = BTN_UP;
		down = BTN_DOWN;
		left = BTN_LEFT;
		right = BTN_RIGHT;
		l = BTN_L;
		r = BTN_R;
		zl = BTN_ZL;
		zr = BTN_ZR;
		home = BTN_HOME;
		_delay_ms(1);
	}
	return 0;
}
