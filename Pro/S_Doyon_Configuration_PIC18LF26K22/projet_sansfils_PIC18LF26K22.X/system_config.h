/********************************************************************
 Software License Agreement:

 The software supplied herewith by Microchip Technology Incorporated
 (the "Company") for its PIC(R) Microcontroller is intended and
 supplied to you, the Company's customer, for use solely and
 exclusively on Microchip PIC Microcontroller products. The
 software is owned by the Company and/or its supplier, and is
 protected under applicable copyright laws. All rights are reserved.
 Any use in violation of the foregoing restrictions may subject the
 user to criminal sanctions under applicable laws, as well as to
 civil liability for the breach of the terms and conditions of this
 license.

 THIS SOFTWARE IS PROVIDED IN AN "AS IS" CONDITION. NO WARRANTIES,
 WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED
 TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. THE COMPANY SHALL NOT,
 IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL OR
 CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 *******************************************************************/

#ifndef _SYSTEM_CONFIG_H
    #define _SYSTEM_CONFIG_H
 
#include "miwi_config.h"        //Include miwi application layer configuration file
#include "miwi_config_pro.h"   //Include protocol layer configuration file
#include "config_24j40.h"       //Transceiver configuration file
 
   
#define SW1             1
#define SW2             2	
    
// There are three ways to use NVM to store data: External EPROM, Data EEPROM and
// programming space, with following definitions:
//      #define USE_EXTERNAL_EEPROM
//      #define USE_DATA_EEPROM
//      #define USE_PROGRAMMING_SPACE
// Each demo board has defined the method of using NVM, as
// required by Network Freezer feature.
#define USE_EXTERNAL_EEPROM

#define SUPPORT_TWO_SPI

// Define EEPROM_SHARE_SPI if external EEPROM shares the SPI
// bus with RF transceiver
//#define EEPROM_SHARE_SPI


// MRF24J40 Pin Definitions
#define RFIF                INTCONbits.INT0IF
#define RFIE                INTCONbits.INT0IE
#define PHY_CS              LATAbits.LATA5
#define PHY_CS_TRIS         TRISAbits.TRISA5
#define RF_INT_PIN          PORTBbits.RB0
#define RF_INT_TRIS         TRISBbits.TRISB0
#define PHY_WAKE            LATDbits.LATD1
#define PHY_WAKE_TRIS       TRISDbits.TRISD1
#define PHY_RESETn          LATDbits.LATD0
#define PHY_RESETn_TRIS     TRISDbits.TRISD0


// EEProm Pin Definitions
#define RF_EEnCS            LATDbits.LATD5
#define RF_EEnCS_TRIS	    TRISDbits.TRISD5

// SPI1 Pin Definitions
#define SPI_SDI             PORTCbits.RC4     // MISO 1
#define SDI_TRIS            TRISCbits.TRISC4  // MISO 1
#define SPI_SDO             LATCbits.LATC5    // MOSI 1
#define SDO_TRIS            TRISCbits.TRISC5  // MOSI 1
#define SPI_SCK             LATCbits.LATC3    // SCK  1
#define SCK_TRIS            TRISCbits.TRISC3  // SCK  1

// SPI2 Pin Definitions
#define SPI_SDI2            PORTBbits.RB2     // MISO 2 23
#define SDI2_TRIS           TRISBbits.TRISB2  // MISO 2 23
#define SPI_SDO2            LATBbits.LATB3    // MOSI 2 24
#define SDO2_TRIS           TRISBbits.TRISB3  // MOSI 2 24
#define SPI_SCK2            LATBbits.LATB1    // SCK  2 22
#define SCK2_TRIS           TRISBbits.TRISB1  // SCK  2 22

#define SPI2SSPIF           PIR3bits.SSP2IF
#define SPI2WCOL            SSP2CON1bits.WCOL
#define SPI2SSPBUF          SSP2BUF

// Switch and LED Pin Definitions
#define SW1_PORT            PORTBbits.RB1
#define SW1_TRIS            TRISBbits.TRISB1
#define SW2_PORT            PORTBbits.RB2
#define SW2_TRIS            TRISBbits.TRISB2
// (LED2 R = PIN 11 RC0) (LED0 Green = PIN 12 RC1)( LED1 Yellow = PIN 13 RC2)
#define LED0                LATCbits.LATC1
#define LED0_TRIS           TRISCbits.TRISC1
#define LED1                LATCbits.LATC2
#define LED1_TRIS           TRISCbits.TRISC2
#define LED2                LATCbits.LATC0
#define LED2_TRIS           TRISCbits.TRISC0

// External EEPROM Pin Definitions

// PIN 4 RA2
//#define EE_nCS_TRIS         TRISDbits.TRISD5
#define MAC_nCS             LATAbits.LATA2
#define TMRL                TMR0L

//External SST Serial Flash Definitions

// PIN 3 RA1
#define EE_nCS              LATAbits.LATA1
#define EE_nCS_TRIS         TRISAbits.TRISA1

// LCD Pin Definitions
#define LCD_CS_TRIS         TRISAbits.TRISA3  // CS PIN 5 RA3
#define LCD_CS              LATAbits.LATA3
#define LCD_RS_TRIS         TRISAbits.TRISA5   // RS PIN 7 RA5
#define LCD_RS              LATAbits.LATA5
#define LCD_RESET_TRIS      TRISAbits.TRISA4    // RST PIN 6 RA4
#define LCD_RESET           LATAbits.LATA4
#define LCD_BKLT_TRIS        1 // TRISEbits.TRISE1    // GROUND
#define LCD_BKLT             1//  LATEbits.LATE1
	
#endif
