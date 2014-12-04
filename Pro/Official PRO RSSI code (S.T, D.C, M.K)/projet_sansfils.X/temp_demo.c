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

#include "temp_demo.h"
#include "system.h"
#include "system_config.h"
#include "miwi_api.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#define TEMP_SECOND_INTERVAL        5
#define DISPLAY_CYCLE_INTERVAL      4
#define EXIT_DEMO_TIMEOUT           6
#define NUM_TEMP_SAMPLES            4

#define EXIT_PKT                    1
#define TEMP_PKT                    3
#define RSSI_PKT                    5       //PKT definie le packet comme etant une donnée deja traité, ne pas modifier le contenue de ce packet (destiné au PAN)
#define ACK_PKT                     4
#define FIRSTHOP_PKT                6       //Mode emetteur des end device, ce packet envois des donnés innutile qui servent a mesurer le rssi (seulement destinées aux noeud directement connectés)



uint8_t CurrentNodeIndex = 0;


extern bool NetFreezerEnable;
extern uint8_t role;

//Structure de donnée sur le RSSI entre deux noeuds (à envoyer au PAN coordonateur) 
struct TempPacket
{
    uint8_t NodeAddress[2];     //Adresse qui a reçue le FIRSTHOP_PKT (qui reçoit les données bidon pour calculer le RSSI)
    uint8_t RSSIValue;          //La valeur du RSSI
    uint8_t BounceSource[2];    //Adresse source (end device qui a envoyé des données bidon qui servent à calculer le RSSI) 
}; 
struct TempPacket NodeTemp[10];

void transmit(void);

/*********************************************************************
* Function:         void RSSI(void)
*
* PreCondition:     none
*
* Input:            none
*
* Output:           none
*
* Side Effects:	    none
*
* Overview:         Calcule et envoie d'un RSSI d'un end device vers un PAN coordinator dans
*                   le but de déterminer sa position.
*
* Note:			    
**********************************************************************/
void TempDemo(void)
{
    bool Run_Demo = true;
    uint16_t VBGResult;
    double temp;
    MIWI_TICK tick1, tick2, tick3;
    uint8_t switch_val;
    
    
    /*******************************************************************/
    // Dispaly Temp Demo Splach Screen
    /*******************************************************************/	
    LCD_BacklightON();
    LCD_Display((char *)"   Microchip       Temp Demo  ", 0, true);
    LCD_BacklightOFF();
        

    /*******************************************************************/
    // Initialize RSSI Data Packet
    // NodeTemp[0] = Self
    /*******************************************************************/
    NodeTemp[0].NodeAddress[0] = myShortAddress.v[0];
    NodeTemp[0].NodeAddress[1] = myShortAddress.v[1];
    
    /*******************************************************************/
    // Read Start tickcount
    /*******************************************************************/	
    tick1 = MiWi_TickGet();
    tick3 = tick1;
    
    while(Run_Demo)
    {
        /*******************************************************************/
        // Read current tickcount
        /*******************************************************************/
		tick2 = MiWi_TickGet();
		
        /*******************************************************************/
        // Check if User wants to Exit Demo
        /*******************************************************************/
        switch_val = BUTTON_Pressed();
	if(switch_val == SW2)
        {
            /*******************************************************************/
        	// Send Exit Demo Request Packet and exit Temp Demo
        	/*******************************************************************/ 
            MiApp_FlushTx();    
            MiApp_WriteData(EXIT_PKT);
            MiApp_BroadcastPacket(false);
            LCD_BacklightON();
            LCD_Display((char *)"   Exiting....     Temp Demo  ", 0, true);
            LCD_BacklightOFF();
            
            /*******************************************************************/
            // Wait for ACK Packet or Timeout
            /*******************************************************************/
            tick1 = MiWi_TickGet();
            while(Run_Demo)
            {
                if(MiApp_MessageAvailable())
                {
                    if(rxMessage.Payload[0] == ACK_PKT)          
                        Run_Demo = false;
                        
                    MiApp_DiscardMessage();
                }
                if ((MiWi_TickGetDiff(tick2,tick1) > (ONE_SECOND * EXIT_DEMO_TIMEOUT)))
                    Run_Demo = false;
                    
                tick2 = MiWi_TickGet();
            }  
        } 
            
        else if(switch_val == SW1)  //Si on pèse sur le bouton 1
        {
			
            LED0 ^= 1;	
            MiApp_FlushTx();
            
            /*******************************************************************/
            // Envoie des données bidons pour calculer le RSSI
            /*******************************************************************/
            MiApp_WriteData(FIRSTHOP_PKT);
            MiApp_WriteData(0x05);      //Donnée bidon pour calculer le RSSI
            MiApp_WriteData(myShortAddress.v[0]);   //Mon adresse
            MiApp_WriteData(myShortAddress.v[1]);   

           
           MiApp_BroadcastPacket(false);

            /*******************************************************************/
            // Read New Start tickcount
            /*******************************************************************/
            tick1 = MiWi_TickGet();
	}

        /*******************************************************************/
        // Check for Incomming Recieve Packet.
        /*******************************************************************/
        // Function MiApp_MessageAvailable returns a boolean to indicate if 
        // a packet has been received by the transceiver. If a packet has 
        // been received, all information will be stored in the rxFrame, 
        // structure of RECEIVED_MESSAGE.
        /*******************************************************************/
        if(MiApp_MessageAvailable())        //Si on reçoit un message
        {


            uint8_t i;
            if(rxMessage.Payload[0] == FIRSTHOP_PKT)    //regarde si la donnée a été traitée (vérification de son identifiant)
            {
                 if(rxMessage.flags.bits.firsthop == 1)    //Vérifie si le message reçu n'a pas déjà été retransmis par un autre noeud
                  {
                     NodeTemp[0].RSSIValue = rxMessage.PacketRSSI;      //Prend le RSSI
                     NodeTemp[0].NodeAddress[0] = rxMessage.Payload[2]; //Prend l'adresse source du message reçu
                     NodeTemp[0].NodeAddress[1] = rxMessage.Payload[3];
                                 MiApp_FlushTx();

                                  MiApp_WriteData(RSSI_PKT);    //Retransmet le RSSI avec un idantifiant RSSI(pour dire qu'il a été traité)
                                  MiApp_WriteData(NodeTemp[0].RSSIValue);   //Envoie le RSSI
                                  MiApp_WriteData(myShortAddress.v[0]);     //Envoie mon adresse (adresse qui a recue la donnée pour calculer le RSSI)
                                  MiApp_WriteData(myShortAddress.v[1]);
                                  MiApp_WriteData(NodeTemp[0].NodeAddress[0]);  //Envoie l'adresse source du RSSI calculé (end device)
                                  MiApp_WriteData(NodeTemp[0].NodeAddress[1]);

                        MiApp_BroadcastPacket(false);

            /*******************************************************************/
            // Read New Start tickcount
            /*******************************************************************/
                      tick1 = MiWi_TickGet();

                  }
            }


            if(rxMessage.Payload[0] == RSSI_PKT)    //Regarde si la donnée a été traitée (vérification de son identifiant)
            {

                if((myShortAddress.v[0]==0) && (myShortAddress.v[1]==0))    //Si je suis le PAN, je prend les données traitées
                {
                    NodeTemp[0].RSSIValue = rxMessage.Payload[1];
                    NodeTemp[0].NodeAddress[0] = rxMessage.Payload[2];
                    NodeTemp[0].NodeAddress[1] = rxMessage.Payload[3];
                    NodeTemp[0].BounceSource[0] = rxMessage.Payload[4];
                    NodeTemp[0].BounceSource[1] = rxMessage.Payload[5];
                    transmit(); 
 //                   PrintTempLCD();       //Affiche sur l'écran si besoin
                }
            }

            MiApp_DiscardMessage();
        }
    }   
}



void PrintTempLCD(void)
{
    int temp;
    uint8_t tempF;
	
    LCD_Erase();

    //Affichage des adresses et du RSSI

    sprintf((char *)LCDText, (char*)"%02x%02x  Source",NodeTemp[0].BounceSource[1],NodeTemp[0].BounceSource[0]);

    sprintf((char *)&LCDText[16], (char*)"%02x%02x:",NodeTemp[0].NodeAddress[1],NodeTemp[0].NodeAddress[0]);


    sprintf((char *)&LCDText[21], (char*)"%d", NodeTemp[0].RSSIValue);
    
    LCD_Update();
}

void transmit(void)         //TO FIX!//
{
    TX9 = false;
    TXEN = true;
    SYNC = 0; //async
    SPEN = true;
    BRG16 = 0; //baud rate register 8BITS
    BRGH = 1;

    SPBRG = 8; //baud rate 115 200

    uint8_t RSSI_1, RSSI_2, RSSI_3 = 0;

    if(NodeTemp[0].RSSIValue >= 200)
    {
        NodeTemp[0].RSSIValue -= 200;
        RSSI_3 = NodeTemp[0].RSSIValue;
    }
    if(NodeTemp[0].RSSIValue >=100)
    {
        NodeTemp[0].RSSIValue -= 100;
        RSSI_2 = NodeTemp[0].RSSIValue;
    }
    RSSI_1 = NodeTemp[0].RSSIValue;

    while(TXIF != 1);
    TXREG = NodeTemp[0].BounceSource[1];
    while(TXIF != 1);
    TXREG = NodeTemp[0].BounceSource[0];
    while(TXIF != 1);
    TXREG = (NodeTemp[0].NodeAddress[1]);
    while(TXIF != 1);
    TXREG = (NodeTemp[0].NodeAddress[0]);
    while(TXIF != 1);
    TXREG = (RSSI_3);
    while(TXIF != 1);
    TXREG = (RSSI_2);
    while(TXIF != 1);
    TXREG = (RSSI_1);
}