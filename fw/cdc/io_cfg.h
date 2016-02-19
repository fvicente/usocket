
#ifndef IO_CFG_H
#define IO_CFG_H

/** I N C L U D E S *************************************************/
#include "autofiles\usbcfg.h"

/** T R I S *********************************************************/
#define INPUT_PIN           1
#define OUTPUT_PIN          0

/** U S B ***********************************************************/
#define tris_usb_bus_sense  TRISAbits.TRISA1    // Input

#if defined(USE_USB_BUS_SENSE_IO)
#define usb_bus_sense       PORTAbits.RA1
#else
#define usb_bus_sense       1
#endif

#define tris_self_power     TRISAbits.TRISA2    // Input

#if defined(USE_SELF_POWER_SENSE_IO)
#define self_power          PORTAbits.RA2
#else
#define self_power          1
#endif

/** OUTPUT ***********************************************************/
#define mInitOutput()      LATD = 0x00; TRISD = 0x00;

#define mLED_1              LATDbits.LATD0
#define mLED_2              LATDbits.LATD1
#define mLED_3              LATDbits.LATD2
#define mLED_4              LATDbits.LATD3
#define mLED_5              LATDbits.LATD4
#define mLED_6              LATDbits.LATD5
#define mLED_7              LATDbits.LATD6
#define mLED_8              LATDbits.LATD7

#define mLED_1_On()         mLED_1 = 1;
#define mLED_2_On()         mLED_2 = 1;
#define mLED_3_On()         mLED_3 = 1;
#define mLED_4_On()         mLED_4 = 1;
#define mLED_5_On()         mLED_5 = 1;
#define mLED_6_On()         mLED_6 = 1;
#define mLED_7_On()         mLED_7 = 1;
#define mLED_8_On()         mLED_8 = 1;

#define mLED_1_Off()        mLED_1 = 0;
#define mLED_2_Off()        mLED_2 = 0;
#define mLED_3_Off()        mLED_3 = 0;
#define mLED_4_Off()        mLED_4 = 0;
#define mLED_5_Off()        mLED_5 = 0;
#define mLED_6_Off()        mLED_6 = 0;
#define mLED_7_Off()        mLED_7 = 0;
#define mLED_8_Off()        mLED_8 = 0;

#define mLED_1_Toggle()     mLED_1 = !mLED_1;
#define mLED_2_Toggle()     mLED_2 = !mLED_2;
#define mLED_3_Toggle()     mLED_3 = !mLED_3;
#define mLED_4_Toggle()     mLED_4 = !mLED_4;
#define mLED_5_Toggle()     mLED_5 = !mLED_5;
#define mLED_6_Toggle()     mLED_6 = !mLED_6;
#define mLED_7_Toggle()     mLED_7 = !mLED_7;
#define mLED_8_Toggle()     mLED_8 = !mLED_8;


/** INPUT *****************************************************/
#define mInitInput()  TRISB=0xFF; INTCON2bits.RBPU=0;

#define InPut1              PORTBbits.RB0
#define InPut2              PORTBbits.RB1
#define InPut3              PORTBbits.RB2
#define InPut4              PORTBbits.RB3
#define InPut5              PORTBbits.RB4
#define InPut6              PORTBbits.RB5
#define InPut7              PORTBbits.RB6
#define InPut8              PORTBbits.RB7


/** ADC ***********************************************************/
// AD Clock = Fosc/4, Acq.Time = 6TAD
// Right justified
#define mInitADC()          TRISA=0x3F;TRISE=0x07;ADCON0=0x01;ADCON2=0x3C;ADCON2bits.ADFM = 1; 

#endif //IO_CFG_H
