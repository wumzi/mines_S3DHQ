/* Dynamixel MX28 servo library
 * Copyright (c) 2012-2013 Georgios Petrou, MIT License
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software 
 * and associated documentation files (the "Software"), to deal in the Software without restriction, 
 * including without limitation the rights to use, copy, modify, merge, publish, distribute, 
 * sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is 
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or 
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING 
 * BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND 
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, 
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, 
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */


 /*
 #include "mbed.h"
 #include "MX28.h"
 #define MX28_DEBUG
 
 Serial pc(USBTX, USBRX);
 MX28 mx28(p9, p10, 57600);

 int main() 
 {
    pc.baud(115200);   
    
    pc.getc();
    pc.printf("======================================================\r\n"); 
    
   uint8_t servoId = 0x1C;
    
    uint16_t modelNumber;        
    mx28.GetModelNumber(servoId, &modelNumber);
    pc.printf("%d", modelNumber);
    
    uint8_t firmwareVersion;        
    mx28.GetFirmwareVersion(servoId, &firmwareVersion);
    
    uint8_t id;        
    mx28.GetId(servoId, &id);               
    mx28.SetId(servoId, servoId);
    
    int32_t baudRate;
    mx28.GetBaudRate(servoId, &baudRate);    
    mx28.SetBaudRate(servoId, 57600); 
    
    uint8_t returnDelayTime;
    mx28.GetReturnDelayTime(servoId, &returnDelayTime);     
    mx28.SetReturnDelayTime(servoId, 0xFA); 
    
    uint16_t cwAngleLimit;
    mx28.GetCWAngleLimit(servoId, &cwAngleLimit);
    mx28.SetCWAngleLimit(servoId, 0x0000);
    
    uint16_t ccwAngleLimit;
    mx28.GetCCWAngleLimit(servoId, &ccwAngleLimit);    
    mx28.SetCCWAngleLimit(servoId, 0x0FFF);
    
    uint8_t highestTemperatureLimit;
    mx28.GetHighestTemperatureLimit(servoId, &highestTemperatureLimit);       
    mx28.SetHighestTemperatureLimit(servoId, 0x50);
    
    uint8_t downLimitVoltage;
    mx28.GetLowestVoltageLimit(servoId, &downLimitVoltage);       
    mx28.SetLowestVoltageLimit(servoId, 0x3C);
   
    uint8_t upLimitVoltage;
    mx28.GetHighestVoltageLimit(servoId, &upLimitVoltage);       
    mx28.SetHighestVoltageLimit(servoId, 0xA0);
    
    uint16_t maxTorque;
    mx28.GetMaxTorque(servoId, &maxTorque);    
    mx28.SetMaxTorque(servoId, 0x03FF);
    
    uint8_t statusReturnLevel;        
    mx28.GetStatusReturnLevel(servoId, &statusReturnLevel);
    mx28.SetStatusReturnLevel(servoId, 0x02);
    
    uint8_t alarmLED;        
    mx28.GetAlarmLED(servoId, &alarmLED);           
    mx28.SetAlarmLED(servoId, 0x24);
    
    uint8_t alarmShutdown;        
    mx28.GetAlarmShutdown(servoId, &alarmShutdown);           
    mx28.SetAlarmShutdown(servoId, 0x24); 
            
    uint8_t enableTorque;   
    mx28.GetEnableTorque(servoId, &enableTorque);    
    mx28.SetEnableTorque(servoId, 0x00);
    
    uint8_t enableLED;   
    mx28.GetEnableLED(servoId, &enableLED);    
    mx28.SetEnableLED(servoId, 0x00);
    
    uint8_t pGain;        
    mx28.GetPGain(servoId, &pGain);           
    mx28.SetPGain(servoId, 0x20);
    
    uint8_t iGain;        
    mx28.GetIGain(servoId, &iGain);           
    mx28.SetIGain(servoId, 0x00);
    
    uint8_t dGain;        
    mx28.GetDGain(servoId, &dGain);           
    mx28.SetDGain(servoId, 0x00);
    
    uint16_t goalPosition;
    mx28.GetGoalPosition(servoId, &goalPosition);     
    mx28.SetGoalPosition(servoId, 0x0800);
    
    uint16_t movingSpeed;
    mx28.GetMovingSpeed(servoId, &movingSpeed);     
    mx28.SetMovingSpeed(servoId, 0x00FF);
    
    uint16_t torqueLimit;
    mx28.GetTorqueLimit(servoId, &torqueLimit);     
    mx28.SetTorqueLimit(servoId, 0x03FF);
    
    uint16_t presentPosition;
    mx28.GetPresentPosition(servoId, &presentPosition);
    
    uint16_t presentSpeed;
    mx28.GetPresentSpeed(servoId, &presentSpeed); 

    uint16_t presentLoad;
    mx28.GetPresentLoad(servoId, &presentLoad);     
    
    uint8_t presentVoltage;        
    mx28.GetPresentVoltage(servoId, &presentVoltage);
    
    uint8_t presentTemperature;        
    mx28.GetPresentTemperature(servoId, &presentTemperature);
    
    uint8_t isRegistered;   

    mx28.GetIsRegistered(servoId, &isRegistered);
    
    uint8_t isMoving;   
    mx28.GetIsMoving(servoId, &isMoving);
    
    uint8_t lock;        
    mx28.GetIsLocked(servoId, &lock);           
    mx28.SetIsLocked(servoId, 0x00); 
    
    uint16_t punch;
    mx28.GetPunch(servoId, &punch);     
    mx28.SetPunch(servoId, 0x0020);
    
    mx28.Ping(servoId);
    
    mx28.Reset(servoId);
    
    uint8_t servo1Id = 0x1C;
    uint8_t servo2Id = 0x02;
    uint8_t servo3Id = 0x03;
    
    uint16_t servo1GoalPosition = 0x0800;
    uint16_t servo2GoalPosition = 0x0800;
    uint16_t servo3GoalPosition = 0x0800;
    
    MX28_PROTOCOL_PACKET packet;
    packet.servoId = MX28_PROTOCOL_BROADCAST_ID;
    // (Data length + 1) * Number of servos + 4
    packet.length = (2+ 1) * 3 + 4;             
    packet.instructionErrorId = MX28_SYNC_WRITE;
    packet.parameter[0] = MX28_GOAL_POSITION_L;
    packet.parameter[1] = 0x06;
    packet.parameter[2] = servo1Id;
    Utilities::ConvertUInt16ToUInt8Array(servo1GoalPosition, (uint8_t*)&(packet.parameter[3]));    
    packet.parameter[9] = servo2Id;
    Utilities::ConvertUInt16ToUInt8Array(servo2GoalPosition, (uint8_t*)&(packet.parameter[10]));
    packet.parameter[16] = servo3Id;
    Utilities::ConvertUInt16ToUInt8Array(servo3GoalPosition, (uint8_t*)&(packet.parameter[17]));
   
    pc.printf("Set servos goal positions: %hu %hu %hu\r\n", servo1GoalPosition, servo2GoalPosition, servo3GoalPosition);
    
    mx28.CommunicatePacket(&packet);
          
    packet.servoId = servoId;
    packet.length = 4;
    packet.instructionErrorId = MX28_READ_DATA;
    packet.parameter[0] = MX28_PRESENT_POSITION_L;
    packet.parameter[1] = 0x08;
 
    mx28.CommunicatePacket(&packet);
    
    presentPosition = Utilities::ConvertUInt8ArrayToUInt16(packet.parameter);
    presentSpeed = Utilities::ConvertUInt8ArrayToUInt16((uint8_t*)&(packet.parameter[2]));   
    presentLoad = Utilities::ConvertUInt8ArrayToUInt16((uint8_t*)&(packet.parameter[4]));    
    presentVoltage = packet.parameter[6];
    presentTemperature = packet.parameter[7];
        
    pc.printf("Present position: %hu\r\n", presentPosition);
    pc.printf("Present speed: %hu\r\n", presentSpeed);
    pc.printf("Present load: %hu\r\n", presentLoad);
    pc.printf("Present voltage: 0x%02X\r\n", presentVoltage);
    pc.printf("Present temperature: 0x%02X\r\n", presentTemperature);             
        
    uint8_t status = mx28.GetModelNumber(servoId, &modelNumber);
   
    if(status == MX28_ERRBIT_WRITE_TIMEOUT)
        pc.printf("Error: Write timeout\r\n");
    else if(status == MX28_ERRBIT_READ_TIMEOUT) 
        pc.printf("Error: Read timeout\r\n");
    else if(status == MX28_ERRBIT_MASTER_CHECKSUM) 
        pc.printf("Error: Master checksum error\r\n");
    else
    {
        if(status & MX28_ERRBIT_VOLTAGE)
            pc.printf("Error: Input voltage error\r\n");
        if(status & MX28_ERRBIT_ANGLE)
            pc.printf("Error: Angle limit error\r\n");
        if(status & MX28_ERRBIT_OVERHEAT)
            pc.printf("Error: Overheat error\r\n");
        if(status & MX28_ERRBIT_RANGE)
            pc.printf("Error: Out of range error\r\n");
        if(status & MX28_ERRBIT_CHECKSUM)
            pc.printf("Error: Checksum error\r\n");
        if(status & MX28_ERRBIT_OVERLOAD)
            pc.printf("Error: Overload error\r\n");
        if(status & MX28_ERRBIT_INSTRUCTION)
            pc.printf("Error: Instruction code error\r\n");
    }
    
    pc.printf("======================================================\r\n"); 
 
    return 0;           
 }
*/

#ifndef MX28_H
#define MX28_H

#include "mbed.h"
#include "SerialHalfDuplex.h"
#include "Protocol.h"
#include "Utilities.h"

#define MX28_DEBUG


class MX28
{
    private:  
        /** PC serial connection used in debug mode.
        */
        Serial *pc;    
                          
        /** Servo serial half duplex connection.
        */
        SerialHalfDuplex *servoSerialHalfDuplex;  
        
    public: 
        /** Send the MX28 packet over the serial half duplex connection.
        *        
        * @param packet The MX28 packet.
        * @return MX28_ERRBIT_NONE if succeeded, error code otherwise.
        */
        uint8_t CommunicatePacket(MX28_PROTOCOL_PACKET *packet);
        
        /** Get the servo model number.
        *        
        * @param servoId The servo id.
        * @param modelNumber The variable to store the model number.
        * @return MX28_ERRBIT_NONE if succeeded, error code otherwise.
        */
        uint8_t GetModelNumber(uint8_t servoId, uint16_t *modelNumber);
        
        /** Get the servo firmware version.
        *        
        * @param servoId The servo id.
        * @param firmwareVersion The variable to store the model number.
        * @return MX28_ERRBIT_NONE if succeeded, error code otherwise.
        */        
        uint8_t GetFirmwareVersion(uint8_t servoId, uint8_t *firmwareVersion);
        
        /** Get the servo id.
        *        
        * @param servoId The servo id.
        * @param id The variable to store the id.
        * @return MX28_ERRBIT_NONE if succeeded, error code otherwise.
        */    
        uint8_t GetId(uint8_t servoId, uint8_t *id);
        
        /** Set the servo id.       
        *      
        * @param servoId The servo id.
        * @param newId The new servo id.
        * @param isRegWrite If the command will be registered.
        * @return MX28_ERRBIT_NONE if succeeded, error code otherwise.
        */
        uint8_t SetId(uint8_t servoId, uint8_t newId, bool isRegWrite = false); 
        
        /** Get the servo baudrate.
        *        
        * @param servoId The servo id.
        * @param baudRate The variable to store the baudrate.
        * @return MX28_ERRBIT_NONE if succeeded, error code otherwise.
        */  
        uint8_t GetBaudRate(uint8_t servoId, int32_t *baudRate);
        
        /** Set the servo baudrate.       
        *      
        * @param servoId The servo id.
        * @param baudRate The servo baudrate.
        * @param isRegWrite If the command will be registered.
        * @return MX28_ERRBIT_NONE if succeeded, error code otherwise.
        */      
        uint8_t SetBaudRate(uint8_t servoId, int baudRate, bool isRegWrite = false);
        
        /** Get the servo return delay time.
        *        
        * @param servoId The servo id.
        * @param returnDelayTime The variable to store the return delay time.
        * @return MX28_ERRBIT_NONE if succeeded, error code otherwise.
        */  
        uint8_t GetReturnDelayTime(uint8_t servoId, uint8_t *returnDelayTime);
        
        /** Set the servo delay time.       
        *      
        * @param servoId The servo id.
        * @param returnDelayTime The servo return delay time.
        * @param isRegWrite If the command will be registered.
        * @return MX28_ERRBIT_NONE if succeeded, error code otherwise.
        */   
        uint8_t SetReturnDelayTime(uint8_t servoId, uint8_t returnDelayTime, bool isRegWrite = false); 
        
        /** Get the servo clockwise angle limit.
        *        
        * @param servoId The servo id.
        * @param cwAngleLimit The variable to store the clockwise angle limit.
        * @return MX28_ERRBIT_NONE if succeeded, error code otherwise.
        */  
        uint8_t GetCWAngleLimit(uint8_t servoId, uint16_t *cwAngleLimit); 
        
        /** Set the servo clockwise angle limit.     
        *      
        * @param servoId The servo id.
        * @param cwAngleLimit The servo clockwise angle limit.
        * @param isRegWrite If the command will be registered.
        * @return MX28_ERRBIT_NONE if succeeded, error code otherwise.
        */     
        uint8_t SetCWAngleLimit(uint8_t servoId, uint16_t cwAngleLimit, bool isRegWrite = false); 
        
        /** Get the servo counterclockwise angle limit.
        *        
        * @param servoId The servo id.
        * @param ccwAngleLimit The variable to store the counterclockwise angle limit.
        * @return MX28_ERRBIT_NONE if succeeded, error code otherwise.
        */  
        uint8_t GetCCWAngleLimit(uint8_t servoId, uint16_t *ccwAngleLimit); 
        
        /** Set the servo counterclockwise angle limit.     
        *      
        * @param servoId The servo id.
        * @param ccwAngleLimit The servo counterclockwise angle limit.
        * @param isRegWrite If the command will be registered.
        * @return MX28_ERRBIT_NONE if succeeded, error code otherwise.
        */         
        uint8_t SetCCWAngleLimit(uint8_t servoId, uint16_t ccwAngleLimit, bool isRegWrite = false); 
        
        /** Get the servo up temperature limit.
        *        
        * @param servoId The servo id.
        * @param highestTemperatureLimit The variable to store the highest temperature limit.
        * @return MX28_ERRBIT_NONE if succeeded, error code otherwise.
        */
        uint8_t GetHighestTemperatureLimit(uint8_t servoId, uint8_t *highestTemperatureLimit); 
        
        /** Set the servo highest temperature limit.     
        *      
        * @param servoId The servo id.
        * @param highestTemperatureLimit The servo highest temperature limit.
        * @param isRegWrite If the command will be registered.
        * @return MX28_ERRBIT_NONE if succeeded, error code otherwise.
        */  
        uint8_t SetHighestTemperatureLimit(uint8_t servoId, uint8_t highestTemperatureLimit, bool isRegWrite = false);   
        
        /** Get the servo lowest voltage limit.
        *        
        * @param servoId The servo id.
        * @param lowestVoltageLimit The variable to store the lowest voltage limit.
        * @return MX28_ERRBIT_NONE if succeeded, error code otherwise.
        */
        uint8_t GetLowestVoltageLimit(uint8_t servoId, uint8_t *lowestVoltageLimit);
        
        /** Set the servo lowest voltage limit.     
        *      
        * @param servoId The servo id.
        * @param lowestVoltageLimit The servo lowest voltage limit.
        * @param isRegWrite If the command will be registered.
        * @return MX28_ERRBIT_NONE if succeeded, error code otherwise.
        */  
        uint8_t SetLowestVoltageLimit(uint8_t servoId, uint8_t lowestVoltageLimit, bool isRegWrite = false);      
        
        /** Get the servo highest voltage limit.
        *        
        * @param servoId The servo id.
        * @param highestVoltageLimit The variable to store the highest voltage limit.
        * @return MX28_ERRBIT_NONE if succeeded, error code otherwise.
        */
        uint8_t GetHighestVoltageLimit(uint8_t servoId, uint8_t *highestVoltageLimit);
        
        /** Set the servo highest voltage limit.   
        *      
        * @param servoId The servo id.
        * @param highestVoltageLimit The servo highest voltage limit.
        * @param isRegWrite If the command will be registered.
        * @return MX28_ERRBIT_NONE if succeeded, error code otherwise.
        */  
        uint8_t SetHighestVoltageLimit(uint8_t servoId, uint8_t highestVoltageLimit, bool isRegWrite = false);      
        
        /** Get the servo max torque.
        *        
        * @param servoId The servo id.
        * @param maxTorque The variable to store the max torque.
        * @return MX28_ERRBIT_NONE if succeeded, error code otherwise.
        */        
        uint8_t GetMaxTorque(uint8_t servoId, uint16_t *maxTorque);
        
        /** Set the servo max torque.   
        *      
        * @param servoId The servo id.
        * @param maxTorque The servo max torque.
        * @param isRegWrite If the command will be registered.
        * @return MX28_ERRBIT_NONE if succeeded, error code otherwise.
        */  
        uint8_t SetMaxTorque(uint8_t servoId, uint16_t maxTorque, bool isRegWrite = false);     
        
        /** Get the servo status return level.
        *        
        * @param servoId The servo id.
        * @param statusReturnLevel The variable to store the status return level.
        * @return MX28_ERRBIT_NONE if succeeded, error code otherwise.
        */
        uint8_t GetStatusReturnLevel(uint8_t servoId, uint8_t *statusReturnLevel);
           
        /** Set the servo status return level.   
        *      
        * @param servoId The servo id.
        * @param statusReturnLevel The servo status return level.
        * @param isRegWrite If the command will be registered.
        * @return MX28_ERRBIT_NONE if succeeded, error code otherwise.
        */       
        uint8_t SetStatusReturnLevel(uint8_t servoId, uint8_t statusReturnLevel, bool isRegWrite = false);     
        
        /** Get the servo alarm LED.
        *        
        * @param servoId The servo id.
        * @param alarmLED The variable to store the alarm LED.
        * @return MX28_ERRBIT_NONE if succeeded, error code otherwise.
        */
        uint8_t GetAlarmLED(uint8_t servoId, uint8_t *alarmLED);
        
        /** Set the servo alarm LED.   
        *      
        * @param servoId The servo id.
        * @param alarmLED The servo alarm LED.
        * @param isRegWrite If the command will be registered.
        * @return MX28_ERRBIT_NONE if succeeded, error code otherwise.
        */
        uint8_t SetAlarmLED(uint8_t servoId, uint8_t alarmLED, bool isRegWrite = false); 
        
        /** Get the servo alarm shutdown.
        *        
        * @param servoId The servo id.
        * @param alarmShutdown The variable to store the alarm shutdown.
        * @return MX28_ERRBIT_NONE if succeeded, error code otherwise.
        */
        uint8_t GetAlarmShutdown(uint8_t servoId, uint8_t *alarmShutdown);
        
        /** Set the servo alarm shutdown.   
        *      
        * @param servoId The servo id.
        * @param alarmShutdown The servo alarm shutdown.
        * @param isRegWrite If the command will be registered.
        * @return MX28_ERRBIT_NONE if succeeded, error code otherwise.
        */
        uint8_t SetAlarmShutdown(uint8_t servoId, uint8_t alarmShutdown, bool isRegWrite = false);  
        
        /** Get the servo enable torque.
        *        
        * @param servoId The servo id.
        * @param enableTorque The variable to store the enable torque.
        * @return MX28_ERRBIT_NONE if succeeded, error code otherwise.
        */
        uint8_t GetEnableTorque(uint8_t servoId, uint8_t *enableTorque);
        
        /** Set the servo enable torque.   
        *      
        * @param servoId The servo id.
        * @param enableTorque The servo enable torque.
        * @param isRegWrite If the command will be registered.
        * @return MX28_ERRBIT_NONE if succeeded, error code otherwise.
        */
        uint8_t SetEnableTorque(uint8_t servoId, uint8_t enableTorque, bool isRegWrite = false); 
        
        /** Get the servo enable LED.
        *        
        * @param servoId The servo id.
        * @param enableLED The variable to store the enable LED.
        * @return MX28_ERRBIT_NONE if succeeded, error code otherwise.
        */
        uint8_t GetEnableLED(uint8_t servoId, uint8_t *enableLED);
        
        /** Set the servo enable LED.  
        *      
        * @param servoId The servo id.
        * @param enableLED The servo enable LED.
        * @param isRegWrite If the command will be registered.
        * @return MX28_ERRBIT_NONE if succeeded, error code otherwise.
        */
        uint8_t SetEnableLED(uint8_t servoId, uint8_t enableLED, bool isRegWrite = false);    
        
        /** Get the servo P gain.
        *        
        * @param servoId The servo id.
        * @param pGain The variable to store the P gain.
        * @return MX28_ERRBIT_NONE if succeeded, error code otherwise.
        */
        uint8_t GetPGain(uint8_t servoId, uint8_t *pGain);
        
        /** Set the servo P gain.  
        *      
        * @param servoId The servo id.
        * @param pGain The servo P gain.
        * @param isRegWrite If the command will be registered.
        * @return MX28_ERRBIT_NONE if succeeded, error code otherwise.
        */
        uint8_t SetPGain(uint8_t servoId, uint8_t pGain, bool isRegWrite = false);
        
        /** Get the servo I gain.
        *        
        * @param servoId The servo id.
        * @param iGain The variable to store the I gain.
        * @return MX28_ERRBIT_NONE if succeeded, error code otherwise.
        */
        uint8_t GetIGain(uint8_t servoId, uint8_t *iGain);
         
        /** Set the servo I gain.  
        *      
        * @param servoId The servo id.
        * @param iGain The servo I gain.
        * @param isRegWrite If the command will be registered.
        * @return MX28_ERRBIT_NONE if succeeded, error code otherwise.
        */
        uint8_t SetIGain(uint8_t servoId, uint8_t iGain, bool isRegWrite = false); 
         
        /** Get the servo D gain.
        *        
        * @param servoId The servo id.
        * @param dGain The variable to store the D gain.
        * @return MX28_ERRBIT_NONE if succeeded, error code otherwise.
        */
        uint8_t GetDGain(uint8_t servoId, uint8_t *dGain);
         
        /** Set the servo D gain.  
        *      
        * @param servoId The servo id.
        * @param dGain The servo D gain.
        * @param isRegWrite If the command will be registered.
        * @return MX28_ERRBIT_NONE if succeeded, error code otherwise.
        */
        uint8_t SetDGain(uint8_t servoId, uint8_t dGain, bool isRegWrite = false);        
        
        /** Get the servo goal position.
        *        
        * @param servoId The servo id.
        * @param goalPosition The variable to store the goal position.
        * @return MX28_ERRBIT_NONE if succeeded, error code otherwise.
        */
        uint8_t GetGoalPosition(uint8_t servoId, uint16_t *goalPosition); 
        
        /** Set the servo goal position.
        *      
        * @param servoId The servo id.
        * @param goalPosition The servo goal position.
        * @param isRegWrite If the command will be registered.
        * @return MX28_ERRBIT_NONE if succeeded, error code otherwise.
        */
        uint8_t SetGoalPosition(uint8_t servoId, uint16_t goalPosition, bool isRegWrite = false);       
        
        /** Get the servo moving speed.
        *        
        * @param servoId The servo id.
        * @param movingSpeed The variable to store the moving speed.
        * @return MX28_ERRBIT_NONE if succeeded, error code otherwise.
        */
        uint8_t GetMovingSpeed(uint8_t servoId, uint16_t *movingSpeed); 
        
        /** Set the servo moving speed.
        *      
        * @param servoId The servo id.
        * @param movingSpeed The servo moving speed.
        * @param isRegWrite If the command will be registered.
        * @return MX28_ERRBIT_NONE if succeeded, error code otherwise.
        */
        uint8_t SetMovingSpeed(uint8_t servoId, uint16_t movingSpeed, bool isRegWrite = false);        
        
        /** Get the servo torque limit.
        * 0 to 1023 (0x3FF) is available, and the unit is about 0.1%.  
        * For example, if the value is 512, it is about 50%; that means only 50% of the maximum torque will be used.
        * If the power is turned on, the value of Max Torque (Address 14, 15) is used as the initial value.       
        *
        * @param servoId The servo id.
        * @param torqueLimit The variable to store the torque limit.
        * @return MX28_ERRBIT_NONE if succeeded, error code otherwise.
        */
        uint8_t GetTorqueLimit(uint8_t servoId, uint16_t *torqueLimit); 
        
        /** Set the servo torque limit.
        *      
        * @param servoId The servo id.
        * @param torqueLimit The servo torque limit.
        * @param isRegWrite If the command will be registered.
        * @return MX28_ERRBIT_NONE if succeeded, error code otherwise.
        */
        uint8_t SetTorqueLimit(uint8_t servoId, uint16_t torqueLimit, bool isRegWrite = false);       
        
        /** Get the servo present position.
        * The range of the value is 0~4095 (0xFFF), and the unit is 0.088 degree.        
        *
        * @param servoId The servo id.
        * @param presentPosition The variable to store the present position.
        * @return MX28_ERRBIT_NONE if succeeded, error code otherwise.
        */
        uint8_t GetPresentPosition(uint8_t servoId, uint16_t *presentPosition); 
        
        /** Get the servo present speed.
        * 0~2047 (0x000~0X7FF) can be used.
        * If a value is in the rage of 0~1023 then the motor rotates to the CCW direction.
        * If a value is in the rage of 1024~2047 then the motor rotates to the CW direction.
        * The 10th bit becomes the direction bit to control the direction; 0 and 1024 are equal.
        * The value unit is about 0.11rpm.
        * For example, if it is set to 300 then the motor is moving to the CCW direction at a rate of about 34.33rpm.        
        *
        * @param servoId The servo id.
        * @param presentSpeed The variable to store the present speed.
        * @return MX28_ERRBIT_NONE if succeeded, error code otherwise.
        */
        uint8_t GetPresentSpeed(uint8_t servoId, uint16_t *presentSpeed);
        
        /** Get the servo present load.
        * The range of the value is 0~2047, and the unit is about 0.1%.
        * If the value is 0~1023, it means the load works to the CCW direction.
        * If the value is 1024~2047, it means the load works to the CW direction.
        * That is, the 10th bit becomes the direction bit to control the direction, and 1024 is equal to 0.
        * For example, the value is 512, it means the load is detected in the direction of CCW about 50% of the maximum torque.        
        *
        * @param servoId The servo id.
        * @param presentLoad The variable to store the present load.
        * @return MX28_ERRBIT_NONE if succeeded, error code otherwise.
        */
        uint8_t GetPresentLoad(uint8_t servoId, uint16_t *presentLoad);
        
        /** Get the servo present voltage.
        * This value is 10 times larger than the actual voltage. 
        * For example, when 10V is supplied, the data value is 100 (0x64)
        *        
        * @param servoId The servo id.
        * @param presentVoltage The variable to store the present voltage.
        * @return MX28_ERRBIT_NONE if succeeded, error code otherwise.
        */
        uint8_t GetPresentVoltage(uint8_t servoId, uint8_t *presentVoltage);
        
        /** Get the servo present temperature.
        * It is the internal temperature of Dynamixel in Celsius.
        * Data value is identical to the actual temperature in Celsius. 
        * For example, if the data value is 85 (0x55), the current internal temperature is 85℃.
        *        
        * @param servoId The servo id.
        * @param presentTemperature The variable to store the present temperature.
        * @return MX28_ERRBIT_NONE if succeeded, error code otherwise.
        */
        uint8_t GetPresentTemperature(uint8_t servoId, uint8_t *presentTemperature);
        
        /** Get if there are commands transmitted by MX28_REG_WRITE.
        * 0 There are no commands transmitted by REG_WRITE.
        * 1 There are commands transmitted by REG_WRITE. 
        *
        * @param servoId The servo id.
        * @param isRegistered The variable to store if it is registered.
        * @return MX28_ERRBIT_NONE if succeeded, error code otherwise.
        */
        uint8_t GetIsRegistered(uint8_t servoId, uint8_t *isRegistered);
        
        /** Get if the servo is moving.
        * 0 Goal position command execution is completed.
        * 1 Goal position command execution is in progress.
        *
        * @param servoId The servo id.
        * @param isMoving The variable to store if the servo is moving.
        * @return MX28_ERRBIT_NONE if succeeded, error code otherwise.
        */
        uint8_t GetIsMoving(uint8_t servoId, uint8_t *isMoving);
        
        /** Get if the servo EEPROM is locked.
        * 0 EEPROM area can be modified.
        * 1 EEPROM area cannot be modified.
        *        
        * @param servoId The servo id.
        * @param isLock The variable to store if the servo EEPROM is locked.
        * @return MX28_ERRBIT_NONE if succeeded, error code otherwise.
        */
        uint8_t GetIsLocked(uint8_t servoId, uint8_t *isLocked);
        
        /** Set if the servo EEPROM is locked.
        *      
        * @param servoId The servo id.
        * @param isLocked The variable to store if the servo EEPROM is locked.
        * @param isRegWrite If the command will be registered.
        * @return MX28_ERRBIT_NONE if succeeded, error code otherwise.
        */
        uint8_t SetIsLocked(uint8_t servoId, uint8_t isLocked, bool isRegWrite = false);    
        
        /** Get the servo punch.
        * Can choose vales from 0x00 to 0x3FF.
        *      
        * @param servoId The servo id.
        * @param punch The variable to store the servo punch. 
        * @return MX28_ERRBIT_NONE if succeeded, error code otherwise.
        */
        uint8_t GetPunch(uint8_t servoId, uint16_t *punch);
        
        /** Set the servo punch.     
        *      
        * @param servoId The servo id.
        * @param punch The servo punch value.
        * @param isRegWrite If the command will be registered.
        * @return MX28_ERRBIT_NONE if succeeded, error code otherwise.
        */    
        uint8_t SetPunch(uint8_t servoId, uint16_t punch, bool isRegWrite = false);
        
        /** Ping the servo.
        * No action. Used for obtaining a Status Packet.
        *      
        * @param servoId The servo id.
        * @return MX28_ERRBIT_NONE if succeeded, error code otherwise.
        */
        uint8_t Ping(uint8_t servoId);
        
        /** Reset the servo.
        * Changes the control table values of the Dynamixel actuator
        * to the Factory Default Value settings     
        *
        * @param servoId The servo id.
        * @return MX28_ERRBIT_NONE if succeeded, error code otherwise.
        */
        uint8_t Reset(uint8_t servoId);
       
        /** Trigger the servo.
        * Triggers the action registered by the REG_WRITE instruction
        *
        * @param servoId The servo id.
        * @return MX28_ERRBIT_NONE if succeeded, error code otherwise.
        */
        uint8_t Action(uint8_t servoId);
              
        /** Create an MX28 servo object connected to the specified serial half duplex pins,
        *   with the specified baudrate.
        *
        * @param tx Send pin.
        * @param rx Receive pin. 
        * @param baudrate The bus speed. 
        */
        MX28(PinName tx, PinName rx, int baudRate);
               
        /** Destroy an MX28 servo object
        */
        ~MX28(); 
};

#endif // MX28_H