/*
 * File:   finalProjLib.c
 * Author: hassanali
 *
 * Created on May 3, 2018, 2:56 PM
 */


#include <p24Fxxxx.h>
#include <xc.h>
#include "adam_lab4_asm.h"
#include "finalProjLib.h"
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


void OSCILLATOR_Initialize(void)
{
    // NOSC FRCDIV; SOSCEN enabled; OSWEN Switch is Complete; 
    __builtin_write_OSCCONL((uint8_t) (0x0702 & 0x00FF));

}


struct tm {
    uint8_t tm_year ;
    uint8_t tm_mon ;
    uint8_t tm_mday;
    uint8_t tm_wday;
    uint8_t tm_hour;
    uint8_t tm_min;
    uint8_t tm_sec;
};

struct al {
    uint8_t al_mon ;
    uint8_t al_mday;
    uint8_t al_wday;
    uint8_t al_hour;
    uint8_t al_min;
    uint8_t al_sec;
};

void RTCC_TimeSet(struct tm *initialTime)
{
   // Set the RTCWREN bit
   __builtin_write_RTCWEN();

   RCFGCALbits.RTCEN = 0;
   
   IFS3bits.RTCIF = 0;
   IEC3bits.RTCIE = 0;

   // set RTCC initial time
   RCFGCALbits.RTCPTR = 3;                               // start the sequence
   RTCVAL =  ConvertHexToBCD(initialTime->tm_year);                        // YEAR
   RTCVAL = (ConvertHexToBCD(initialTime->tm_mon) << 8) | ConvertHexToBCD(initialTime->tm_mday);  // MONTH-1/DAY-1
   RTCVAL = (ConvertHexToBCD(initialTime->tm_wday) << 8) | ConvertHexToBCD(initialTime->tm_hour); // WEEKDAY/HOURS
   RTCVAL = (ConvertHexToBCD(initialTime->tm_min) << 8) | ConvertHexToBCD(initialTime->tm_sec);   // MINUTES/SECONDS
             
   // Enable RTCC, clear RTCWREN         
   RCFGCALbits.RTCEN = 1;  
   RCFGCALbits.RTCWREN = 0;
   
   IEC3bits.RTCIE = 1;

}

bool RTCC_TimeGet(struct tm *currentTime)
{
    uint16_t register_value;
    if(RCFGCALbits.RTCSYNC){
        return false;
    }

   // Set the RTCWREN bit
   __builtin_write_RTCWEN();

    RCFGCALbits.RTCPTR = 3;
    register_value = RTCVAL;
    currentTime->tm_year = ConvertBCDToHex(register_value & 0x00FF);
    RCFGCALbits.RTCPTR = 2;
    register_value = RTCVAL;
    currentTime->tm_mon = ConvertBCDToHex((register_value & 0xFF00) >> 8);
    currentTime->tm_mday = ConvertBCDToHex(register_value & 0x00FF);
    RCFGCALbits.RTCPTR = 1;
    register_value = RTCVAL;
    currentTime->tm_wday = ConvertBCDToHex((register_value & 0xFF00) >> 8);
    currentTime->tm_hour = ConvertBCDToHex(register_value & 0x00FF);
    RCFGCALbits.RTCPTR = 0;
    register_value = RTCVAL;
    currentTime->tm_min = ConvertBCDToHex((register_value & 0xFF00) >> 8);
    currentTime->tm_sec = ConvertBCDToHex(register_value & 0x00FF);

    RCFGCALbits.RTCWREN = 0;

    return true;
}

void RTCC_AlarmSet(struct al *initialAlarm)
{
   // Set the RTCWREN bit
   __builtin_write_RTCWEN();

  ALCFGRPTbits.ALRMEN = 0;
   
   IFS3bits.RTCIF = 0;
   IEC3bits.RTCIE = 0;

   // set RTCC initial time
  ALCFGRPTbits.ALRMPTR = 2;                              // start the sequence
   
   ALRMVAL = (ConvertHexToBCD(initialAlarm->al_mon) << 8) | ConvertHexToBCD(initialAlarm->al_mday);  // MONTH-1/DAY-1
   ALRMVAL = (ConvertHexToBCD(initialAlarm->al_wday) << 8) | ConvertHexToBCD(initialAlarm->al_hour); // WEEKDAY/HOURS
   ALRMVAL = (ConvertHexToBCD(initialAlarm->al_min) << 8) | ConvertHexToBCD(initialAlarm->al_sec);   // MINUTES/SECONDS
             
   // Enable RTCC, clear RTCWREN         
   ALCFGRPTbits.ALRMEN = 1;
   RCFGCALbits.RTCWREN = 0;
   
   IEC3bits.RTCIE = 1;

}


bool RTCC_alarmGet(struct al *currentAlarm)
{
    uint16_t register_value;
    if(RCFGCALbits.RTCSYNC){
        return false;
    }

   // Set the RTCWREN bit
   __builtin_write_RTCWEN();

    
    ALCFGRPTbits.ALRMPTR = 2;
    register_value = ALRMVAL;
    currentAlarm->al_mon = ConvertBCDToHex((register_value & 0xFF00) >> 8);
    currentAlarm->al_mday = ConvertBCDToHex(register_value & 0x00FF);
    ALCFGRPTbits.ALRMPTR = 1;
    register_value = ALRMVAL;
    currentAlarm->al_wday = ConvertBCDToHex((register_value & 0xFF00) >> 8);
    currentAlarm->al_hour = ConvertBCDToHex(register_value & 0x00FF);
    ALCFGRPTbits.ALRMPTR = 0;
    register_value = ALRMVAL;
    currentAlarm->al_min = ConvertBCDToHex((register_value & 0xFF00) >> 8);
    currentAlarm->al_sec = ConvertBCDToHex(register_value & 0x00FF);

    RCFGCALbits.RTCWREN = 0;

    return true;
}
static uint8_t ConvertHexToBCD(uint8_t hexvalue)
{
    uint8_t bcdvalue;
    bcdvalue = (hexvalue / 10) << 4;
    bcdvalue = bcdvalue | (hexvalue % 10);
    return (bcdvalue);
}

static uint8_t ConvertBCDToHex(uint8_t bcdvalue)
{
    uint8_t hexvalue;
    hexvalue = (((bcdvalue & 0xF0) >> 4)* 10) + (bcdvalue & 0x0F);
    return hexvalue;
}

// END OF ALI's OSCILLATOR FUNCTIONS

void __attribute__((__interrupt__,__auto_psv__)) _U1RXInterrupt(void)
{
    IFS0bits.U1RXIF = 0;
    while(U1STAbits.URXDA)
        {
        buffer[front++] = U1RXREG;
   
        front &= 63;
        }
    
    buttonMap(front);    
}


void setup(void){
   CLKDIVbits.RCDIV = 0;
    
   AD1PCFG = 0xffff;
   lcd_init();
}

void UARTsetup(void){
    
    _TRISB6 = 0;  // U1TX output
    _TRISB5 = 1; // U1RX input
    _TRISB7 = 1; // U1RX input
    LATBbits.LATB7=0;

    U1MODE = 0;  // UEN<1:0> bits control the pins
//     U1BRG = 34; // 115200 baud,
//     U1MODEbits.BRGH = 1;
    U1MODEbits.BRGH = 0;
    U1BRG = 103; // 38400 baud (check the calculation with the datasheet)
    U1MODEbits.UEN = 0;
    U1MODEbits.UARTEN = 1;
    U1STAbits.UTXEN = 1;
    U1STAbits.URXISEL=3;   // Interrupt flag bit is set when receive buffer is full (i.e., has 4 data characters)

    // Peripheral Pin Select 
    __builtin_write_OSCCONL(OSCCON & 0xbf); // unlock PPS
    _RP6R = 0x0003;   //RB6->UART1:U1TX; See Table 10-3 on P109 of the datasheet
    _U1RXR = 5;   //RB5->UART1:U1RX;
    __builtin_write_OSCCONL(OSCCON | 0x40); // lock   PPS

    IFS0bits.U1RXIF = 0;
    IEC0bits.U1RXIE = 1;
 
    
}

void RTCC_Setup(void)
{

    OSCILLATOR_Initialize();
    
   // Set the RTCWREN bit
   __builtin_write_RTCWEN();

   RCFGCALbits.RTCEN = 0;


   // set RTCC time 2018-04-28 06-39-59
   RCFGCALbits.RTCPTR = 3;        // start the sequence
   RTCVAL = 0x18;    // YEAR
   RTCVAL = 0x428;    // MONTH-1/DAY-1
   RTCVAL = 0x623;    // WEEKDAY/HOURS
   RTCVAL = 0x5850;    // MINUTES/SECONDS
  

   // set Alarm time 2018-04-28 07-01-59
   ALCFGRPTbits.ALRMEN = 0;
   ALCFGRPTbits.ALRMPTR = 2;
   ALRMVAL = 0x428;
   ALRMVAL = 0x607;
   ALRMVAL = 0x0159;

   // ALRMPTR MIN_SEC; AMASK Every Day; ARPT 0; CHIME disabled; ALRMEN enabled; 
   ALCFGRPT = 0x9800;
   RCFGCALbits.RTCOE = 1; // Enable RTCC output

   // RTSECSEL Alarm Pulse; 
   PADCFG1 = 0x0000;
           
   // Enable RTCC, clear RTCWREN
   RCFGCALbits.RTCEN = 1;
   RCFGCALbits.RTCWREN = 0;

   //Enable RTCC interrupt
   IEC3bits.RTCIE = 1;
}


void buttonMap(unsigned char front)
{
          if((buffer[front - 4] == ':' && buffer[front - 1] == '1') || (buffer[front - 2] == '0' && buffer[front - 1] == ';'))
    {
      blu = '1';
    }
    
          if((buffer[front - 4] == '9' && buffer[front - 1] == '2') || (buffer[front - 2] == '0' && buffer[front - 1] == ':'))
    {
      blu = '2';
    }
          if((buffer[front - 4] == '8' && buffer[front - 1] == '3') || (buffer[front - 2] == '0' && buffer[front - 1] == '9'))
    {
      blu = '3';
    }
          if((buffer[front - 4] == '7' && buffer[front - 1] == '4') || (buffer[front - 2] == '0' && buffer[front - 1] == '8'))
    {
      blu = '4';
    }
          if((buffer[front - 4] == '6' && buffer[front - 1] == '5') || (buffer[front - 2] == '0' && buffer[front - 1] == '7'))
    {
      blu = '5';
    }
          if((buffer[front - 4] == '5' && buffer[front - 1] == '6') || (buffer[front - 2] == '0' && buffer[front - 1] == '6'))
    {
      blu = '6';
    }          
          if((buffer[front - 4] == '4' && buffer[front - 1] == '7') || (buffer[front - 2] == '0' && buffer[front - 1] == '5'))
    {
      blu = '7';
    }  
          if((buffer[front - 4] == '3' && buffer[front - 1] == '8') || (buffer[front - 2] == '0' && buffer[front - 1] == '4'))
    {
      blu = '8';
    }          
}

void lcd_cmd(char command){
    
    I2C2CONbits.SEN = 1;
    
    while(I2C2CONbits.SEN == 1);
    
    IFS3bits.MI2C2IF = 0;
    
    I2C2TRN = 0b01111100;
    
    while(IFS3bits.MI2C2IF == 0);
    IFS3bits.MI2C2IF = 0;
    
    I2C2TRN = 0b00000000; 
    
    
    while(IFS3bits.MI2C2IF == 0);
    IFS3bits.MI2C2IF = 0;
    
    I2C2TRN = command;
    
    while(IFS3bits.MI2C2IF == 0);
    IFS3bits.MI2C2IF = 0;
    
    I2C2CONbits.PEN = 1;
    
    while(I2C2CONbits.PEN == 1);

    
}






void lcd_init(void){
    
    
    TRISBbits.TRISB3 = 1;
    
    I2C2CONbits.I2CEN = 0;
    
    //Sets the Baud Rate to 100 kHz
    I2C2BRG = 157; 
    I2C2CONbits.I2CEN = 1;
    IFS3bits.MI2C2IF = 0; 
    
    int i = 0;
    
    for(;i < 50; i++)
    {
        wait1ms();
    }
    
    lcd_cmd(0b00111000);    //function set, normal instruction mode
    wait50us();
    lcd_cmd(0b00101100); // extra function set
    wait50us();
    lcd_cmd(0b00001100);    //Display On
    wait50us();
    lcd_cmd(0b00000001);    //Clear Display
    
    i = 0;
    
    for(; i < 200; i++)
    {
       wait1ms();
    }

    lcd_cmd(0b00000110);    // Entry Mode Set
    

}

    
void lcd_setCursor(char x, char y)
{   
    
    char address = 0b10000000;
  
    address += ((0x40)*x) + y;
    
    lcd_cmd(address);
    wait50us();
}

void lcd_printChar(char myChar)
{
    
    I2C2CONbits.SEN = 1;
    
    while(I2C2CONbits.SEN == 1);
    
    IFS3bits.MI2C2IF = 0;
    
    I2C2TRN = 0b01111100;
             
    while(IFS3bits.MI2C2IF == 0);
    IFS3bits.MI2C2IF = 0;
    
    I2C2TRN = 0b01000000;
    
    while(IFS3bits.MI2C2IF == 0);
    
    IFS3bits.MI2C2IF = 0;
    
    I2C2TRN = myChar;
    
    while(IFS3bits.MI2C2IF == 0);
    IFS3bits.MI2C2IF = 0;
    
    I2C2CONbits.PEN = 1;
    
    while(I2C2CONbits.PEN == 1);
}

void lcd_CharNext(char myChar)
{
    
    I2C2TRN = 0b11000000;
    
    while(IFS3bits.MI2C2IF == 0);
    
    IFS3bits.MI2C2IF = 0;
    
    I2C2TRN = myChar;
    
    while(IFS3bits.MI2C2IF == 0);
    IFS3bits.MI2C2IF = 0;
    
    
}

void lcd_CharEnd(char myChar)
{
    I2C2TRN = 0b01000000;
    
    while(IFS3bits.MI2C2IF == 0);
    
    IFS3bits.MI2C2IF = 0;
    
    I2C2TRN = myChar;
    
    while(IFS3bits.MI2C2IF == 0);
    IFS3bits.MI2C2IF = 0;
    
     I2C2CONbits.PEN = 1;
    
    while(I2C2CONbits.PEN == 1);
    
}

void lcd_printStr(volatile char *s)
{
    int index = 0;
    
    I2C2CONbits.SEN = 1;
    
    while(I2C2CONbits.SEN == 1);
    
    IFS3bits.MI2C2IF = 0;
    
    I2C2TRN = 0b01111100;
             
    while(IFS3bits.MI2C2IF == 0);
    
    IFS3bits.MI2C2IF = 0;
    
    while(s[index + 1] != 0)    // checks if its the last char
    {
        lcd_CharNext(s[index]);
        index++;
    }
    lcd_CharEnd(s[index]);
        
    
}

//char moveCursor(void)
//{
//    static char num = 0;
//    lcd_setCursor(0, num++);
//}

void delay(int delay){
    int i;
        for(i = 0; i < delay; i++)
        {
            wait1ms();
        }
}

void readKeyPress(void){
    switch (blu){
            case '1':
                        //lcd_cmd(0b00011000); // shift display
                        wait50us();
                        display_clock();
                        break;
            case '2':
                        lcd_cmd(0b00010100); // cursor shift
                        wait50us();
                        blu = 0;
                        break;
            case '3':
                        lcd_setCursor(row, col);
                        lcd_cmd(0b00001111);
                        display_temp();
                        break;     
            case '4':
                        lcd_cmd(0b00001100);
                        display_clock();
                        break;
            case '5':
                if(row == 1)
                {
                    if(col == 5)
                    {   
                        if((h1+1) == 51)
                        {
                            h1 = 48;
                        }
                        else
                        h1++;
                        if(h1 == 50 && h2 > 51)
                        h2 = 48;
                    }
                    if(col == 6)
                    {   
                        if(h1 != 50)
                        {
                        if((h2+1) == 58)
                        {
                            h2 = 48;
                        }
                        else
                        h2++;
                        }
                        else
                        {
                         if((h2+1) == 53)
                        {
                            h2 = 48;
                        }
                        else
                        h2++;
                        }
                            
                    }
                    if(col == 8)
                    {   
                        if((m1+1) == 54)
                        {
                            m1 = 48;
                        }
                        else
                        m1++;
                    }
                    if(col == 9)
                    {
                        if((m2+1) == 58)
                        {
                            m2 = 48;
                        }
                        else
                        m2++;
                    }
                    if(col == 11)
                    {
                        if((s1+1) == 54)
                        {
                            s1 = 48;
                        }
                        else
                        s1++;
                    }
                    if(col == 12)
                    {
                        if((s2+1) == 58)
                        {
                            s2 = 48;
                        }
                        else
                        s2++;
                    }
                    
                    if(col == 0 || col == 1 || col == 2)
                    {
                        w_day = (w_day + 1) % 7;
                    }
                    
                    
                }
                
                if(row == 0)
                {
                    if(col == 3)
                    {   
                        if((month1+1) == 50)
                        {
                            month1 = 48;
                        }
                        else
                        month1++;
                        if(month1 == 49)
                        month2 = 48;
                    }
                    if(col == 4)
                    {   
                        if(month1 != 49)
                        {
                        if((month2+1) == 58)
                        {
                            month2 = 48;
                        }
                        else
                        month2++;
                        }
                        else
                        {
                         if((month2+1) == 51)
                        {
                            month2 = 48;
                        }
                        else
                        month2++;
                        }
                    }
                    if(col == 6)
                    {   
                        if((day1+1) == 52)
                        {
                            day1 = 48;
                        }
                        else
                        day1++;
                        if(day1 == 51)
                            day2 = 48;
                    }
                    if(col == 7)
                    {
                        if(day1 != 51)
                        {
                            if((day2+1) == 58)
                            {
                                day2 = 48;
                            }
                            else
                            day2++;                          
                        }
                        else
                        {
                            if((day2+1) == 50)
                            {
                                day2 = 48;
                            }
                            else
                            day2++; 
                        }
                        

                    }
                    if(col == 11)
                    {
                        if((year1+1) == 58)
                        {
                            year1 = 48;
                        }
                        else
                        year1++;
                    }
                    if(col == 12)
                    {
                        if((year2+1) == 58)
                        {
                            year2 = 48;
                        }
                        else
                        year2++;
                    }  

                }
                        
                        wait50us();
                        blu = '3';
                        break;
                
            case '6':
                        lcd_cmd(0b00001100);
                        display_clock();
                        break;             
            case '7':
                
                        col -= 1;
                        if(col < 0)
                        {
                            col = 15;
                            row ^= 1;
                        }
                        if(col >= 16)
                        {
                            col = 0;
                            row ^= 1;
                        }

                        lcd_setCursor(row,col);
                        lcd_cmd(0b00001111); 
                        wait50us();
                        
                        blu = '3';
                        break; 
            case '8':
                
                        col += 1;
                        if(col < 0)
                        {
                            col = 15;
                            row ^= 1;
                        }
                        if(col >= 16)
                        {
                            col = 0;
                            row ^= 1;
                        }
                        
                        lcd_setCursor(row,col);
                        lcd_cmd(0b00001111); 
                        wait50us();
                        blu = '3';
                        break;
                        
            default:
                        display_clock();
                        break;
        }
}


void display_clock(){

    h1 = hour / 10;
    h2 = hour % 10;
    h1+=48;
    h2+=48;   

    m1 = min / 10;
    m2 = min % 10;
    m1+=48;
    m2+=48;  
   
    s1 = sec / 10;
    s2 = sec % 10;
    s1+=48;
    s2+=48;
    
    
    day1 = day / 10;
    day2 = day % 10;
    day1+=48;
    day2+=48;   

    month1 = month / 10;
    month2 = month % 10;
    month1+=48;
    month2+=48;  
   
    year1 = year / 10;
    year2 = year % 10;
    year1+=48;
    year2+=48;
    
    w_day = wday % 10;
    
    if(w_day == 6)
    {
        week_day1[0] = 'S';
        week_day1[1] = 'A';
        week_day1[2] = 'T';
    }
    else if(w_day == 0)
    {
        week_day1[0] = 'S';
        week_day1[1] = 'U';
        week_day1[2] = 'N';
    }
        else if(w_day == 1)
    {
        week_day1[0] = 'M';
        week_day1[1] = 'O';
        week_day1[2] = 'N';
    }
        else if(w_day == 2)
    {
        week_day1[0] = 'T';
        week_day1[1] = 'U';
        week_day1[2] = 'E';
    }
        else if(w_day == 3)
    {
        week_day1[0] = 'W';
        week_day1[1] = 'E';
        week_day1[2] = 'D';
    }
        else if(w_day == 4)
    {
        week_day1[0] = 'T';
        week_day1[1] = 'H';
        week_day1[2] = 'U';
    }
        else
    {
        week_day1[0] = 'F';
        week_day1[1] = 'R';
        week_day1[2] = 'I';
    }

    
    time1[0] = h1;
    time1[1] = h2;
    time1[3] = m1;
    time1[4] = m2;
    time1[6] = s1;
    time1[7] = s2;
    
    
    calendar1[0] = month1;
    calendar1[1] = month2;
    calendar1[3] = day1;
    calendar1[4] = day2;
    calendar1[8] = year1;
    calendar1[9] = year2;
    
    
    
    delay(150);
        lcd_setCursor(0, 3);
        lcd_printStr(calendar1);
        
        lcd_setCursor(1,0);
        lcd_printStr(week_day1);
        
       lcd_setCursor(1, 5);
       lcd_printStr(time1);
}
void display_temp(){

    uint8_t hour_temp,min_temp,sec_temp;
    uint8_t month_temp,day_temp,year_temp;
    uint8_t wday_temp;
    
    hour_temp = ((h1-48) * 10) + (h2-48);   
    min_temp = ((m1-48) * 10) + (m2-48);
    sec_temp = ((s1-48) * 10) + (s2-48);
    
    month_temp = ((month1-48) * 10) + (month2-48);   
    day_temp = ((day1-48) * 10) + (day2-48);
    year_temp = ((year1-48) * 10) + (year2-48);
    
    wday_temp = w_day;
    
    
   struct tm temp_time = {year_temp,month_temp,day_temp,wday_temp,hour_temp,min_temp,sec_temp};
   
    RTCC_TimeSet(&temp_time);
    
    time1_temp[0] = h1;
    time1_temp[1] = h2;
    time1_temp[3] = m1;
    time1_temp[4] = m2;
    time1_temp[6] = s1;
    time1_temp[7] = s2;
    
    calendar1_temp[0] = month1;
    calendar1_temp[1] = month2;
    calendar1_temp[3] = day1;
    calendar1_temp[4] = day2;
    calendar1_temp[8] = year1;
    calendar1_temp[9] = year2;
    
    
        if(w_day == 6)
    {
        week_day1_temp[0] = 'S';
        week_day1_temp[1] = 'A';
        week_day1_temp[2] = 'T';
    }
    else if(w_day == 0)
    {
        week_day1_temp[0] = 'S';
        week_day1_temp[1] = 'U';
        week_day1_temp[2] = 'N';
    }
        else if(w_day == 1)
    {
        week_day1_temp[0] = 'M';
        week_day1_temp[1] = 'O';
        week_day1_temp[2] = 'N';
    }
        else if(w_day == 2)
    {
        week_day1_temp[0] = 'T';
        week_day1_temp[1] = 'U';
        week_day1_temp[2] = 'E';
    }
        else if(w_day == 3)
    {
        week_day1_temp[0] = 'W';
        week_day1_temp[1] = 'E';
        week_day1_temp[2] = 'D';
    }
        else if(w_day == 4)
    {
        week_day1_temp[0] = 'T';
        week_day1_temp[1] = 'H';
        week_day1_temp[2] = 'U';
    }
        else
    {
        week_day1_temp[0] = 'F';
        week_day1_temp[1] = 'R';
        week_day1_temp[2] = 'I';
    }
    
    
    
    
    
    

    delay(150);
        lcd_setCursor(0, 3);
        lcd_printStr(calendar1_temp);
        
        lcd_setCursor(1,0);
        lcd_printStr(week_day1_temp);
        
       lcd_setCursor(1, 5);
       lcd_printStr(time1_temp);
}