/* OpenSprinkler Unified (AVR/RPI/BBB/LINUX/ESP8266) Firmware
 * Copyright (C) 2015 by Ray Wang (ray@opensprinkler.com)
 *
 * OpenSprinkler macro defines and hardware pin assignments
 * Feb 2015 @ OpenSprinkler.com
 *
 * This file is part of the OpenSprinkler library
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see
 * <http://www.gnu.org/licenses/>.
 */

#ifndef _DEFINES_H
#define _DEFINES_H

//#define ENABLE_DEBUG  // enable serial debug

typedef unsigned char byte;
typedef unsigned long ulong;
  
#define TMP_BUFFER_SIZE      255   // scratch buffer size

/** Firmware version, hardware version, and maximal values */
#define OS_FW_VERSION  219  // Firmware version: 219 means 2.1.9
                            // if this number is different from the one stored in non-volatile memory
                            // a device reset will be automatically triggered

#define OS_FW_MINOR      3  // Firmware minor version

/** Hardware version base numbers */
#define OS_HW_VERSION_BASE   0x00
#define OSPI_HW_VERSION_BASE 0x40
#define OSBO_HW_VERSION_BASE 0x80
#define SIM_HW_VERSION_BASE  0xC0

/** Hardware type macro defines */
#define HW_TYPE_AC           0xAC   // standard 24VAC for 24VAC solenoids only, with triacs
#define HW_TYPE_DC           0xDC   // DC powered, for both DC and 24VAC solenoids, with boost converter and MOSFETs
#define HW_TYPE_LATCH        0x1A   // DC powered, for DC latching solenoids only, with boost converter and H-bridges
#define HW_TYPE_UNKNOWN      0xFF

/** Data file names */
#define IOPTS_FILENAME        "iopts.dat"   // integer options data file
#define SOPTS_FILENAME        "sopts.dat"   // string options data file
#define STATIONS_FILENAME     "stns.dat"    // stations data file
#define NVCON_FILENAME        "nvcon.dat"   // non-volatile controller data file, see OpenSprinkler.h --> struct NVConData
#define PROG_FILENAME         "prog.dat"    // program data file
#define DONE_FILENAME         "done.dat"    // used to indicate the completion of all files

/** Station macro defines */
#define STN_TYPE_STANDARD    0x00
#define STN_TYPE_RF          0x01	// Radio Frequency (RF) station
#define STN_TYPE_REMOTE      0x02	// Remote OpenSprinkler station
#define STN_TYPE_GPIO        0x03	// direct GPIO station
#define STN_TYPE_HTTP        0x04	// HTTP station
#define STN_TYPE_OTHER       0xFF

/** IFTTT macro defines */
#define IFTTT_PROGRAM_SCHED   0x01
#define IFTTT_SENSOR1         0x02
#define IFTTT_FLOWSENSOR      0x04
#define IFTTT_WEATHER_UPDATE  0x08
#define IFTTT_REBOOT          0x10
#define IFTTT_STATION_RUN     0x20
#define IFTTT_SENSOR2         0x40
#define IFTTT_RAINDELAY				0x80

/** HTTP request macro defines */
#define HTTP_RQT_SUCCESS			 0
#define HTTP_RQT_NOT_RECEIVED -1
#define HTTP_RQT_CONNECT_ERR	-2
#define HTTP_RQT_TIMEOUT			-3
#define HTTP_RQT_EMPTY_RETURN	-4

/** Sensor macro defines */
#define SENSOR_TYPE_NONE    0x00
#define SENSOR_TYPE_RAIN    0x01	// rain sensor
#define SENSOR_TYPE_FLOW    0x02	// flow sensor
#define SENSOR_TYPE_SOIL    0x03  // soil moisture sensor
#define SENSOR_TYPE_PSWITCH 0xF0	// program switch sensor
#define SENSOR_TYPE_OTHER   0xFF

#define FLOWCOUNT_RT_WINDOW   30    // flow count window (for computing real-time flow rate), 30 seconds

/** Reboot cause */
#define REBOOT_CAUSE_NONE   0
#define REBOOT_CAUSE_RESET  1
#define REBOOT_CAUSE_BUTTON 2
#define REBOOT_CAUSE_RSTAP  3
#define REBOOT_CAUSE_TIMER  4
#define REBOOT_CAUSE_WEB    5
#define REBOOT_CAUSE_WIFIDONE     6
#define REBOOT_CAUSE_FWUPDATE     7
#define REBOOT_CAUSE_WEATHER_FAIL 8
#define REBOOT_CAUSE_NETWORK_FAIL 9
#define REBOOT_CAUSE_NTP          10
#define REBOOT_CAUSE_POWERON      99


/** WiFi defines */
#define WIFI_MODE_AP       0xA9
#define WIFI_MODE_STA      0x2A

#define OS_STATE_INITIAL        0
#define OS_STATE_CONNECTING     1
#define OS_STATE_CONNECTED      2
#define OS_STATE_TRY_CONNECT    3

#define LED_FAST_BLINK 100
#define LED_SLOW_BLINK 500

/** Storage / zone expander defines */
#define MAX_EXT_BOARDS    4  // maximum number of 8-zone expanders (each 16-zone expander counts as 2)

#define MAX_NUM_STATIONS  (8+(MAX_EXT_BOARDS*8))  // maximum number of stations (onboard stations + external stations)
#define STATION_NAME_SIZE 32    // maximum number of characters in each station name
#define MAX_SOPTS_SIZE    160   // maximum string option size

#define STATION_SPECIAL_DATA_SIZE  (TMP_BUFFER_SIZE - STATION_NAME_SIZE - 12)

/** Default string option values */
#define DEFAULT_PASSWORD          "a6d82bced638de3def1e9bbb4983225c"  // md5 of 'opendoor'
#define DEFAULT_LOCATION          "42.36,-71.06"	// Boston,MA
#define DEFAULT_JAVASCRIPT_URL    "https://ui.opensprinkler.com/js"
#define DEFAULT_WEATHER_URL       "weather.opensprinkler.com"
#define DEFAULT_IFTTT_URL         "maker.ifttt.com"
#define DEFAULT_EMPTY_STRING      ""

/** Macro define of each option
  * Refer to OpenSprinkler.cpp for details on each option
  */
enum {
	IOPT_FW_VERSION=0,// read-only (ro)
	IOPT_TIMEZONE,
	IOPT_USE_NTP,
	IOPT_USE_DHCP,
	IOPT_STATIC_IP1,
	IOPT_STATIC_IP2,
	IOPT_STATIC_IP3,
	IOPT_STATIC_IP4,
	IOPT_GATEWAY_IP1,
	IOPT_GATEWAY_IP2,
	IOPT_GATEWAY_IP3,
	IOPT_GATEWAY_IP4,
	IOPT_HTTPPORT_0,
	IOPT_HTTPPORT_1,
	IOPT_HW_VERSION, //ro
	IOPT_EXT_BOARDS,
	IOPT_SEQUENTIAL_RETIRED, //ro
	IOPT_STATION_DELAY_TIME,
	IOPT_MASTER_STATION,
	IOPT_MASTER_ON_ADJ,
	IOPT_MASTER_OFF_ADJ,
	IOPT_URS_RETIRED, // ro
	IOPT_RSO_RETIRED, // ro
	IOPT_WATER_PERCENTAGE,
	IOPT_DEVICE_ENABLE, // editable through jc
	IOPT_IGNORE_PASSWORD,
	IOPT_DEVICE_ID,
	IOPT_LCD_CONTRAST,
	IOPT_LCD_BACKLIGHT,
	IOPT_LCD_DIMMING,
	IOPT_BOOST_TIME,
	IOPT_USE_WEATHER,
	IOPT_NTP_IP1,
	IOPT_NTP_IP2,
	IOPT_NTP_IP3,
	IOPT_NTP_IP4,
	IOPT_ENABLE_LOGGING,
	IOPT_MASTER_STATION_2,
	IOPT_MASTER_ON_ADJ_2,
	IOPT_MASTER_OFF_ADJ_2,
	IOPT_FW_MINOR, //ro
	IOPT_PULSE_RATE_0,
	IOPT_PULSE_RATE_1,
	IOPT_REMOTE_EXT_MODE, // editable through jc
	IOPT_DNS_IP1,
	IOPT_DNS_IP2,
	IOPT_DNS_IP3,
	IOPT_DNS_IP4,
	IOPT_SPE_AUTO_REFRESH,
	IOPT_IFTTT_ENABLE,
	IOPT_SENSOR1_TYPE,
	IOPT_SENSOR1_OPTION,	
	IOPT_SENSOR2_TYPE,
	IOPT_SENSOR2_OPTION,
	IOPT_SENSOR1_ON_DELAY,
	IOPT_SENSOR1_OFF_DELAY,
	IOPT_SENSOR2_ON_DELAY,
	IOPT_SENSOR2_OFF_DELAY,
	IOPT_SUBNET_MASK1,
	IOPT_SUBNET_MASK2,
	IOPT_SUBNET_MASK3,
	IOPT_SUBNET_MASK4,
	IOPT_WIFI_MODE, //ro
	IOPT_RESET,     //ro
	NUM_IOPTS // total number of integer options
};

enum {
	SOPT_PASSWORD=0,
	SOPT_LOCATION,
	SOPT_JAVASCRIPTURL,
	SOPT_WEATHERURL,
	SOPT_WEATHER_OPTS,
	SOPT_IFTTT_KEY,
	SOPT_STA_SSID,
	SOPT_STA_PASS,
	//SOPT_WEATHER_KEY,
	//SOPT_AP_PASS,
	//SOPT_MQTT_IP,
	NUM_SOPTS	// total number of string options
};

/** Log Data Type */
#define LOGDATA_STATION    0x00
#define LOGDATA_SENSOR1    0x01
#define LOGDATA_RAINDELAY  0x02
#define LOGDATA_WATERLEVEL 0x03
#define LOGDATA_FLOWSENSE  0x04
#define LOGDATA_SENSOR2    0x05
#define LOGDATA_CURRENT    0x80

#undef OS_HW_VERSION

/** Hardware defines */

	#define OS_HW_VERSION    (OS_HW_VERSION_BASE+30)
	#define EXP_I2CADDR_BASE 0x20	//base addres for expansion board
	#define LCD_I2CADDR      0x3C // 128x64 OLED display I2C address

	#define PIN_CURR_SENSE    A0
	#define PIN_FREE_LIST     {} // no free GPIO pin at the moment
	#define ETHER_BUFFER_SIZE   8192

	/* To accommodate different OS30 versions, we use software defines pins */ 
	extern byte PIN_BUTTON_1;
	extern byte PIN_BUTTON_2;
	extern byte PIN_BUTTON_3;
	extern byte PIN_BOOST;
	extern byte PIN_BOOST_EN;
	extern byte PIN_SENSOR1;
	extern byte PIN_SENSOR2;
	extern byte PIN_IOEXP_CS;

	/* OS30 revision 1 pin defines */
	#define V1_PIN_BUTTON_1      16 // button 1
	#define V1_PIN_BUTTON_2      14 // button 2
	#define V1_PIN_BUTTON_3      12 // button 3
	#define V1_PIN_BOOST         255
	#define V1_PIN_BOOST_EN      255
	#define V1_PIN_SENSOR1       13 // sensor 1
	//#define V1_PIN_SENSOR2       10 // sensor 2

//#define ENABLE_DEBUG
#if defined(ENABLE_DEBUG) /** Serial debug functions */
	#define DEBUG_BEGIN(x)   {Serial.begin(x);}
	#define DEBUG_PRINT(x)   {Serial.print(x);}
	#define DEBUG_PRINTLN(x) {Serial.println(x);}
#else
	#define DEBUG_BEGIN(x)   {}
	#define DEBUG_PRINT(x)   {}
	#define DEBUG_PRINTLN(x) {}
#endif

/** Other defines */
// button values
#define BUTTON_1            0x01
#define BUTTON_2            0x02
#define BUTTON_3            0x04

// button status values
#define BUTTON_NONE         0x00  // no button pressed
#define BUTTON_MASK         0x0F  // button status mask
#define BUTTON_FLAG_HOLD    0x80  // long hold flag
#define BUTTON_FLAG_DOWN    0x40  // down flag
#define BUTTON_FLAG_UP      0x20  // up flag

// button timing values
#define BUTTON_DELAY_MS        1  // short delay (milliseconds)
#define BUTTON_HOLD_MS      1000  // long hold expiration time (milliseconds)

// button mode values
#define BUTTON_WAIT_NONE       0  // do not wait, return value immediately
#define BUTTON_WAIT_RELEASE    1  // wait until button is release
#define BUTTON_WAIT_HOLD       2  // wait until button hold time expires

#define DISPLAY_MSG_MS      2000  // message display time (milliseconds)

#endif  // _DEFINES_H