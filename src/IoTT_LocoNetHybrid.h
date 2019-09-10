/*
IoTT_LocoNetUART.h

Target CPU: ESP32
UART Emulation with special features for LocoNet (half-duplex network with DCMA) by Hans Tanner. 
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

#ifndef IoTT_LocoNetHybrid_h
#define IoTT_LocoNetHybrid_h

#include <inttypes.h>
#include <arduino.h>
#include <IoTTCommDef.h>

#define lnBusy 0
#define lnAwaitBackoff 1
#define lnNetAvailable 2

#define lnMaxMsgSize 48

#define carrierLossNotification 333333 //notify every 5000 ms if carrier is missing

#define lnRespTimeout  500000 //microseconds

enum uartModeType : uint8_t {uart_idle=0, uart_transmit=1, uart_collision=2};
//enum uartRxStateType : uint8_t {waitStartBit=0, waitDataBit=1, waitStopBit=2};
enum uartTxStateType : uint8_t {sendStartBit=0, sendDataBit=1, sendStopBit=2};

void hybrid_begin(uint8_t pinRxNum, uint8_t pinTxNum, bool invLogic);
void hybrid_end();
void hybrid_highSpeed(bool goFast);
void hybrid_setBusyLED(int8_t ledNr);
bool hybrid_availableForWrite();
uint16_t hybrid_available();
bool hybrid_carrierOK();
uint16_t hybrid_read();
uint8_t hybrid_write(uint8_t * dataByte, uint8_t numBytes=1);
void hybrid_flush();
uint8_t  hybrid_LocoNetAvailable();
//void hybrid_driver(); //timer interrupt occurs every 15 micros.

#endif
