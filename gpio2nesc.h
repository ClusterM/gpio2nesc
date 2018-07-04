#ifndef _NES2WII_H_
#define _NES2WII_H_

#include "defines.h"

#define GLUE(a,b) a##b
#define DDR(p) GLUE(DDR,p)
#define PORT(p) GLUE(PORT,p)
#define PIN(p) GLUE(PIN,p)

#define twi_port PORT(TWI_PORT)
#define twi_ddr DDR(TWI_PORT)
#define twi_scl_pin TWI_SCL_PIN
#define twi_sda_pin TWI_SDA_PIN

#ifdef DETECT_PORT
#define dev_detect_port PORT(DETECT_PORT)
#define dev_detect_ddr DDR(DETECT_PORT)
#define dev_detect_pin DETECT_PIN
#endif

#define RED_ON PORT(RED_LED_PORT) &= ~(1<<RED_LED_PIN)
#define RED_OFF PORT(RED_LED_PORT) |= (1<<RED_LED_PIN)
#define GREEN_ON PORT(GREEN_LED_PORT) &= ~(1<<GREEN_LED_PIN)
#define GREEN_OFF PORT(GREEN_LED_PORT) |= (1<<GREEN_LED_PIN)

#ifdef BUTTON_A_PIN
	#define BTN_A (!(PIN(BUTTON_A_PORT)&(1<<BUTTON_A_PIN)))
#else
	#define BTN_A 0
#endif
#ifdef BUTTON_B_PIN
	#define BTN_B (!(PIN(BUTTON_B_PORT)&(1<<BUTTON_B_PIN)))
#else
	#define BTN_B 0
#endif
#ifdef BUTTON_X_PIN
	#define BTN_X (!(PIN(BUTTON_X_PORT)&(1<<BUTTON_X_PIN)))
#else
	#define BTN_X 0
#endif
#ifdef BUTTON_Y_PIN
	#define BTN_Y (!(PIN(BUTTON_Y_PORT)&(1<<BUTTON_Y_PIN)))
#else
	#define BTN_Y 0
#endif
#ifdef BUTTON_UP_PIN
	#define BTN_UP (!(PIN(BUTTON_UP_PORT)&(1<<BUTTON_UP_PIN)))
#else
	#define BTN_UP 0
#endif
#ifdef BUTTON_DOWN_PIN
	#define BTN_DOWN (!(PIN(BUTTON_DOWN_PORT)&(1<<BUTTON_DOWN_PIN)))
#else
	#define BTN_DOWN 0
#endif
#ifdef BUTTON_LEFT_PIN
	#define BTN_LEFT (!(PIN(BUTTON_LEFT_PORT)&(1<<BUTTON_LEFT_PIN)))
#else
	#define BTN_LEFT 0
#endif
#ifdef BUTTON_RIGHT_PIN
	#define BTN_RIGHT (!(PIN(BUTTON_RIGHT_PORT)&(1<<BUTTON_RIGHT_PIN)))
#else
	#define BTN_RIGHT 0
#endif
#ifdef BUTTON_SELECT_PIN
	#define BTN_SELECT (!(PIN(BUTTON_SELECT_PORT)&(1<<BUTTON_SELECT_PIN)))
#else
	#define BTN_SELECT 0
#endif
#ifdef BUTTON_START_PIN
	#define BTN_START (!(PIN(BUTTON_START_PORT)&(1<<BUTTON_START_PIN)))
#else
	#define BTN_START 0
#endif
#ifdef BUTTON_L_PIN
	#define BTN_L (!(PIN(BUTTON_L_PORT)&(1<<BUTTON_L_PIN)))
#else
	#define BTN_L 0
#endif
#ifdef BUTTON_R_PIN
	#define BTN_R (!(PIN(BUTTON_R_PORT)&(1<<BUTTON_R_PIN)))
#else
	#define BTN_R 0
#endif
#ifdef BUTTON_ZL_PIN
	#define BTN_ZL (!(PIN(BUTTON_ZL_PORT)&(1<<BUTTON_ZL_PIN)))
#else
	#define BTN_ZL 0
#endif
#ifdef BUTTON_ZR_PIN
	#define BTN_ZR (!(PIN(BUTTON_ZR_PORT)&(1<<BUTTON_ZR_PIN)))
#else
	#define BTN_ZR 0
#endif
#ifdef BUTTON_HOME_PIN
	#define BTN_HOME (!(PIN(BUTTON_HOME_PORT)&(1<<BUTTON_HOME_PIN)))
#else
	#define BTN_HOME 0
#endif

#endif
