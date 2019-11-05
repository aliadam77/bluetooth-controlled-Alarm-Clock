#ifndef FINALPROJLIB_H
#define	FINALPROJLIB_H
#include <xc.h> // include processor files - each processor file is guarded. 
#include "adam_lab4_asm.h"
#include "finalProjLib.h"


#ifdef	__cplusplus


#endif /* __cplusplus */


volatile unsigned char buffer[64];
volatile unsigned char front = 0;
volatile unsigned char back = 0;
volatile unsigned char blu;


volatile signed char row = 0;
volatile signed char col = 0;

volatile  unsigned char h1 = '0'; // (1,5)
volatile  unsigned char h2 = '0'; // (1,6)
volatile  unsigned char m1 = '0'; // (1,8)
volatile  unsigned char m2 = '0'; // (1,9)
volatile  unsigned char s1 = '0'; // (1,11)
volatile  unsigned char s2 = '0'; // (1,12)
volatile char time1[9] = {'0','0',':','0','0',':','0','0', NULL};
volatile char time1_temp[9] = {'0','0',':','0','0',':','0','0', NULL};

volatile  unsigned char month1 = '0'; // (0,3)
volatile  unsigned char month2 = '0'; // (0,4)
volatile  unsigned char day1 = '0'; // (0,6)
volatile  unsigned char day2 = '0'; // (0,7)
volatile  unsigned char year1 = '0'; // (0,9)
volatile  unsigned char year2 = '0'; // (0,10)
volatile char calendar1[11] = {'0','0','/','0','0','/','2','0','0','0', NULL};
volatile char calendar1_temp[11] = {'0','0','/','0','0','/','2','0','0','0', NULL};


volatile unsigned char w_day = '0';
volatile unsigned char w_day1 = '0'; // (1,0)
volatile unsigned char w_day2 = '0'; // (1,1)
volatile unsigned char w_day3 = '0'; // (1,2)
volatile char week_day1[4] = {'0','0','0', NULL};
volatile char week_day1_temp[4] = {'0','0','0', NULL};

void OSCILLATOR_Initialize(void);
void RTCC_TimeSet(struct tm *initialTime);
bool RTCC_TimeGet(struct tm *currentTime);
void RTCC_AlarmSet(struct al *initialAlarm);
bool RTCC_alarmGet(struct al *currentAlarm);
static uint8_t ConvertHexToBCD(uint8_t hexvalue);
static uint8_t ConvertBCDToHex(uint8_t bcdvalue);

void __attribute__((__interrupt__,__auto_psv__)) _U1RXInterrupt(void);
void UARTsetup(void);
void setup(void);
void RTCC_Setup(void);
void buttonMap(unsigned char front);

void lcd_cmd(char command);
void lcd_init(void);
void lcd_setCursor(char x, char y);
void lcd_printChar(char myChar);
void lcd_CharNext(char myChar);
void lcd_CharEnd(char myChar);
void lcd_printStr(volatile char *s);

void delay(int delay);
void readKeyPress(void);
void display_clock();
void display_temp();

// ALI'S OSCILLATOR FUNCTIONS
static uint8_t ConvertHexToBCD(uint8_t hexvalue);
static uint8_t ConvertBCDToHex(uint8_t bcdvalue);
uint8_t sec, min, hour,day,month,year,wday;
struct tm time ;


    

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* XC_HEADER_TEMPLATE_H */
