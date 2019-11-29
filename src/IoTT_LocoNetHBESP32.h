/*
LocoNetESPSerial.h

Based on SoftwareSerial.cpp - Implementation of the Arduino software serial for ESP8266.
Copyright (c) 2015-2016 Peter Lerup. All rights reserved.

Adaptation to LocoNet (half-duplex network with DCMA) by Hans Tanner. 
See Digitrax LocoNet PE documentation for more information

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

*/

#ifndef LocoNetESP32_h
#define LocoNetESP32_h

#include <inttypes.h>
#include <IoTTCommDef.h>
#include <IoTT_LocoNetHybrid.h>
#include <HardwareSerial.h>
#include <ArduinoJson.h>


// This class is compatible with the corresponding AVR one,
// the constructor however has an optional rx buffer size.
// Speed up to 115200 can be used.

#define lnBusy 0
#define lnAwaitBackoff 1
#define lnNetAvailable 2

#define rxBufferSize 64
#define txBufferSize 64
#define verBufferSize 48

#define queBufferSize 50 //messages that can be written in one burst before buffer overflow

class LocoNetESPSerial : public HardwareSerial
{
public:
	LocoNetESPSerial(int receivePin, int transmitPin, bool inverse_logic = true, int uartNr = 2, unsigned int buffSize = 64);
	~LocoNetESPSerial();
	void begin();
	void processLoop();
	void setBusyLED(int8_t ledNr);
	uint16_t lnWriteMsg(lnTransmitMsg txData);
	uint16_t lnWriteMsg(lnReceiveBuffer txData);
	void setLNCallback(cbFct newCB);
	int cdBackoff();
	bool carrierOK();
	void loadLNCfgJSON(DynamicJsonDocument doc);
   
private:
   
   // Member functions
   void handleLNIn(uint8_t inData, uint8_t inFlags = 0);
   void processLNMsg(lnReceiveBuffer * recData);
   void processLNReceive();
   void processLNTransmit();

   void sendBreakSequence();
   uint8_t getXORCheck(uint8_t * msgData, uint8_t * msgLen);

   
   // Member variables
   lnTransmitMsg transmitQueue[queBufferSize];
   uint8_t que_rdPos, que_wrPos;
   lnReceiveBuffer lnInBuffer, lnEchoBuffer;
   int m_rxPin, m_txPin;
   bool m_invert = true;
   uint8_t m_uart = 2;
   uint8_t m_buffsize = 64;
   bool receiveMode;
   int8_t busyLED = -1;
   uint8_t m_bitTime = 60;
   bool m_highSpeed = true;

//   TaskHandle_t txTask;
//   static void transmitTask(void * parameter);

   uint32_t m_StartCD;
   unsigned int rx_rdPos, rx_wrPos;
   unsigned int tx_rdPos, tx_wrPos;
   unsigned int ver_rdPos, ver_wrPos;
   uint16_t rx_buffer[rxBufferSize];
   uint8_t tx_buffer[txBufferSize];
   uint8_t ver_buffer[verBufferSize];

   uint8_t    bitRecStatus = 0;    //0: waiting for OpCode; 1: waiting for package data
   uint8_t    lnBufferPtr = 0; //index of next msg buffer location to read
   uint8_t    lnExpLen = 0;

   uint8_t transmitStatus;
   uint32_t transmitTime;
   uint8_t numWrite, numRead;
   
   uint32_t respTime; //store here the micros for each message that goes out
   uint8_t  respOpCode; //store OpCode of every message that is sent so we remember if it has a reply
   uint16_t respID; //store here the reqID of each message that goes out

};

//this is the callback function. Provide a function of this name and parameter in your application and it will be called when a new message is received
extern void onLocoNetMessage(lnReceiveBuffer * recData) __attribute__ ((weak));

#endif
