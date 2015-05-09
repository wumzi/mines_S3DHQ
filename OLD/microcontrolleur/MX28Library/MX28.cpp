/* Copyright (c) 2012 Georgios Petrou, MIT License
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
 
#include "MX28.h"

uint8_t MX28::CommunicatePacket(MX28_PROTOCOL_PACKET *packet)
{    
    uint8_t currentParameter = 0;
    bool isWholePacket = false;    
    uint8_t encoderState = WAIT_ON_HEADER_0;
            
    packet->checkSum = Utilities::GetCheckSum((uint8_t*)&(packet->servoId), packet->length + 1);

    Timer timer;
    timer.start();
    
    while((timer.read_ms() < MX28_PROTOCOL_COMMAND_RESPONSE_TIMEOUT_MS) && (!isWholePacket))
    { 
        if(servoSerialHalfDuplex->writeable()) 
        {
            switch(encoderState)
            {
                case WAIT_ON_HEADER_0:
                {   
                    #ifdef MX28_DEBUG
                        pc->printf("Write: 0x%02X ", MX28_PROTOCOL_HEADER_0);
                    #endif
                    
                    servoSerialHalfDuplex->putc(MX28_PROTOCOL_HEADER_0);
   
                    encoderState = WAIT_ON_HEADER_1;                
                    
                    break;
                }
                case WAIT_ON_HEADER_1:
                {   
                    #ifdef MX28_DEBUG
                        pc->printf("0x%02X ", MX28_PROTOCOL_HEADER_1);
                    #endif
                          
                    servoSerialHalfDuplex->putc(MX28_PROTOCOL_HEADER_1);
                    
                    encoderState = WAIT_ON_SERVO_ID;
                    
                    break;   
                } 
                case WAIT_ON_SERVO_ID:
                {   
                    #ifdef MX28_DEBUG
                        pc->printf("0x%02X ", packet->servoId);
                    #endif
                    
                    servoSerialHalfDuplex->putc(packet->servoId);
                    
                    encoderState = WAIT_ON_LENGTH;
                   
                    break;
                } 
                case WAIT_ON_LENGTH:
                {
                    #ifdef MX28_DEBUG
                        pc->printf("0x%02X ", packet->length);
                    #endif
                    
                    servoSerialHalfDuplex->putc(packet->length);
                    
                    encoderState = WAIT_ON_INSTRUCTION_ERROR_ID;
                    
                    break; 
                }
                case WAIT_ON_INSTRUCTION_ERROR_ID:
                {
                    #ifdef MX28_DEBUG
                        pc->printf("0x%02X ", packet->instructionErrorId);
                    #endif
                    
                    servoSerialHalfDuplex->putc(packet->instructionErrorId);
                    
                    if(packet->length > 2)
                        encoderState = WAIT_ON_PARAMETER;                    
                    else                   
                        encoderState = WAIT_ON_CHECK_SUM;
                    
                    break;    
                }
                case WAIT_ON_PARAMETER:
                {                      
                    #ifdef MX28_DEBUG
                        pc->printf("0x%02X ", packet->parameter[currentParameter]);
                    #endif
        
                    servoSerialHalfDuplex->putc(packet->parameter[currentParameter]);
                                       
                    if(++currentParameter == packet->length - 2)                    
                        encoderState = WAIT_ON_CHECK_SUM;
                    
                    break;
                }
                case WAIT_ON_CHECK_SUM:
                {      
                    #ifdef MX28_DEBUG
                        pc->printf("0x%02X\r\n", packet->checkSum);
                    #endif
                    
                    servoSerialHalfDuplex->putc(packet->checkSum);              
                    
                    encoderState = WAIT_ON_HEADER_0; 
                    isWholePacket = true;
                    
                    break;
                }                
            }
        }    
    }
    
    #ifdef MX28_DEBUG
        pc->printf("Timer: %d ms\r\n", timer.read_ms());
    #endif    
    
    timer.stop();
    
    if(!isWholePacket)
    {
        #ifdef MX28_DEBUG
            pc->printf("Error: Write response timeout.\r\n");
        #endif
       
        return MX28_ERRBIT_WRITE_TIMEOUT;
    }
    
    if( packet->servoId == MX28_PROTOCOL_BROADCAST_ID || 
        packet->instructionErrorId == MX28_ACTION || 
        packet->instructionErrorId == MX28_SYNC_WRITE)
        
        return MX28_ERRBIT_NONE;   
    
    currentParameter = 0; 
    isWholePacket = false;    
    uint8_t decoderState = WAIT_ON_HEADER_0;
        
    timer.reset();
    timer.start();
    
    while((timer.read_ms() < MX28_PROTOCOL_COMMAND_RESPONSE_TIMEOUT_MS) && (!isWholePacket))
    {  
        if(servoSerialHalfDuplex->readable()) 
        {                        
            switch(decoderState)
            {
                case WAIT_ON_HEADER_0:
                {   
                    uint8_t mx28ProtocolHeader0 = servoSerialHalfDuplex->getc(); 
                    
                    #ifdef MX28_DEBUG
                        pc->printf("Read: 0x%02X ", mx28ProtocolHeader0);
                    #endif
    
                    decoderState = WAIT_ON_HEADER_1;                
                    
                    break;
                }
                case WAIT_ON_HEADER_1:
                {   
                    uint8_t mx28ProtocolHeader1 = servoSerialHalfDuplex->getc(); 
                    
                    #ifdef MX28_DEBUG
                        pc->printf("0x%02X ", mx28ProtocolHeader1);
                    #endif
                    
                    decoderState = WAIT_ON_SERVO_ID;
                    
                    break;   
                }
                case WAIT_ON_SERVO_ID:
                {   
                    packet->servoId = servoSerialHalfDuplex->getc(); 
                    
                    #ifdef MX28_DEBUG
                        pc->printf("0x%02X ", packet->servoId);
                    #endif
                    
                    decoderState = WAIT_ON_LENGTH;
                    
                    break;
                }                
                case WAIT_ON_LENGTH:
                {
                    packet->length = servoSerialHalfDuplex->getc();
                    
                    #ifdef MX28_DEBUG
                        pc->printf("0x%02X ", packet->length);
                    #endif
                                       
                    decoderState = WAIT_ON_INSTRUCTION_ERROR_ID;  
                                  
                    break;
                } 
                case WAIT_ON_INSTRUCTION_ERROR_ID:
                {
                    packet->instructionErrorId = servoSerialHalfDuplex->getc();
                    
                    #ifdef MX28_DEBUG
                        pc->printf("0x%02X ", packet->instructionErrorId);
                    #endif
                                   
                    if(packet->length > 2)
                        decoderState = WAIT_ON_PARAMETER;                    
                    else                   
                        decoderState = WAIT_ON_CHECK_SUM;
                    
                    break;    
                } 
                case WAIT_ON_PARAMETER:
                {    
                    uint8_t parameter = servoSerialHalfDuplex->getc();               
                    packet->parameter[currentParameter] = parameter;
                    
                    #ifdef MX28_DEBUG
                        pc->printf("0x%02X ", parameter);
                    #endif
                    
                    if(++currentParameter == packet->length - 2)                    
                        decoderState = WAIT_ON_CHECK_SUM;
                   
                    break;
                }
                case WAIT_ON_CHECK_SUM:
                {
                    packet->checkSum = servoSerialHalfDuplex->getc();
                    
                    #ifdef MX28_DEBUG
                        pc->printf("0x%02X\r\n", packet->checkSum);
                    #endif
                    
                    decoderState = WAIT_ON_HEADER_0; 
                    isWholePacket = true;
                    
                    break;
                }
            }
        }    
    }
    
    #ifdef MX28_DEBUG
        pc->printf("Timer: %d ms\r\n", timer.read_ms());
    #endif
    
    timer.stop();
    
    if(!isWholePacket)
    {
        #ifdef MX28_DEBUG
            pc->printf("Error: Read response timeout\r\n");
        #endif
               
        return MX28_ERRBIT_READ_TIMEOUT;
    }   
   
    if(packet->instructionErrorId != MX28_ERRBIT_NONE)
    {
        #ifdef MX28_DEBUG
            if(packet->instructionErrorId & MX28_ERRBIT_VOLTAGE)
                pc->printf("Error: Input voltage error\r\n");
            if(packet->instructionErrorId & MX28_ERRBIT_ANGLE)
                pc->printf("Error: Angle limit error\r\n");
            if(packet->instructionErrorId & MX28_ERRBIT_OVERHEAT)
                pc->printf("Error: Overheat error\r\n");
            if(packet->instructionErrorId & MX28_ERRBIT_RANGE)
                pc->printf("Error: Out of range error\r\n");
            if(packet->instructionErrorId & MX28_ERRBIT_CHECKSUM)
                pc->printf("Error: Checksum error\r\n");
            if(packet->instructionErrorId & MX28_ERRBIT_OVERLOAD)
                pc->printf("Error: Overload error\r\n");
            if(packet->instructionErrorId & MX28_ERRBIT_INSTRUCTION)
                pc->printf("Error: Instruction code error\r\n");            
        #endif         
        
        return packet->instructionErrorId;
    }
   
    if(packet->checkSum != Utilities::GetCheckSum((uint8_t*)&(packet->servoId), packet->length + 1))
    {
        #ifdef MX28_DEBUG
            pc->printf("Error: Master got wrong checksum\r\n");
        #endif   
            
        return MX28_ERRBIT_MASTER_CHECKSUM;
    }   
  
    return MX28_ERRBIT_NONE;        
}

uint8_t MX28::GetModelNumber(uint8_t servoId, uint16_t *modelNumber)
{
    MX28_PROTOCOL_PACKET packet;
    
    packet.servoId = servoId;
    packet.length = 4;
    packet.instructionErrorId = MX28_READ_DATA;
    packet.parameter[0] = MX28_MODEL_NUMBER_L;
    packet.parameter[1] = 0x02;
    
    uint8_t status = CommunicatePacket(&packet);
    
    if(status == MX28_ERRBIT_NONE)
    {
        *modelNumber = Utilities::ConvertUInt8ArrayToUInt16(packet.parameter);
        
        #ifdef MX28_DEBUG
            pc->printf("Get model number: %hu\r\n", *modelNumber);
        #endif
    }
    
    return status;
}

uint8_t MX28::GetFirmwareVersion(uint8_t servoId, uint8_t *firmwareVersion)
{
    MX28_PROTOCOL_PACKET packet;
    
    packet.servoId = servoId;
    packet.length = 4;
    packet.instructionErrorId = MX28_READ_DATA;
    packet.parameter[0] = MX28_VERSION;
    packet.parameter[1] = 0x01;
 
    uint8_t status = CommunicatePacket(&packet);
        
    if(status == MX28_ERRBIT_NONE)
    {
       *firmwareVersion = packet.parameter[0];        
        
        #ifdef MX28_DEBUG
            pc->printf("Get firmware version: 0x%02X\r\n", *firmwareVersion);
        #endif
     }
     
     return status;
}

uint8_t MX28::GetId(uint8_t servoId, uint8_t *id)
{
    MX28_PROTOCOL_PACKET packet;
    
    packet.servoId = servoId;
    packet.length = 4;
    packet.instructionErrorId = MX28_READ_DATA;
    packet.parameter[0] = MX28_ID;
    packet.parameter[1] = 0x01;
 
    uint8_t status = CommunicatePacket(&packet);
        
    if(status == MX28_ERRBIT_NONE)
    {
        *id = packet.parameter[0];        
        
        #ifdef MX28_DEBUG
            pc->printf("Get id: 0x%02X\r\n", *id);
        #endif        
    }
    
    return status;
}

uint8_t MX28::SetId(uint8_t servoId, uint8_t id, bool isRegWrite) 
{
    MX28_PROTOCOL_PACKET packet;

    packet.servoId = servoId;
    packet.length = 4;
    packet.instructionErrorId = (isRegWrite == true) ? MX28_REG_WRITE : MX28_WRITE_DATA;
    packet.parameter[0] = MX28_ID;
    packet.parameter[1] = id;
    
    #ifdef MX28_DEBUG
        pc->printf("Set id: 0x%02X\r\n", id);
    #endif

    return CommunicatePacket(&packet);  
}

uint8_t MX28::GetBaudRate(uint8_t servoId, int32_t *baudRate)
{
    MX28_PROTOCOL_PACKET packet;
    
    packet.servoId = servoId;
    packet.length = 4;
    packet.instructionErrorId = MX28_READ_DATA;
    packet.parameter[0] = MX28_BAUD_RATE;
    packet.parameter[1] = 1;
 
    uint8_t status = CommunicatePacket(&packet);
        
    if(status == MX28_ERRBIT_NONE)
    {
        if(packet.parameter[0] < 0xFA)
            *baudRate = 2000000.0 / (double)(packet.parameter[0] + 1);
        else if(packet.parameter[0] == 0xFA)
            *baudRate = 2250000;
        else if(packet.parameter[0] == 0xFB)
            *baudRate = 2500000;
        else
            *baudRate = 3000000;          
       
        #ifdef MX28_DEBUG
            pc->printf("Get baud rate: %d\r\n", *baudRate);
        #endif
    } 
     
    return status;
}

uint8_t MX28::SetBaudRate(uint8_t servoId, int32_t baudRate, bool isRegWrite) 
{
    MX28_PROTOCOL_PACKET packet;
    
    uint8_t divisor = 0x00;
    
    if(baudRate < 2250000)
        divisor = (uint8_t)(2000000.0 / (double)baudRate) - 1;
    else if(baudRate == 2250000)
        divisor = 0xFA;
    else if(baudRate == 2500000)
        divisor = 0xFB; 
    else          
        divisor = 0xFC; 
        
    packet.servoId = servoId;
    packet.length = 4;
    packet.instructionErrorId = (isRegWrite == true) ? MX28_REG_WRITE : MX28_WRITE_DATA;
    packet.parameter[0] = MX28_BAUD_RATE;
    packet.parameter[1] = divisor;
    
    #ifdef MX28_DEBUG
        pc->printf("Set baudrate: 0x%02X\r\n", divisor);
    #endif    
    
    return CommunicatePacket(&packet);  
}

uint8_t MX28::GetReturnDelayTime(uint8_t servoId, uint8_t *returnDelayTime)
{
    MX28_PROTOCOL_PACKET packet;
    
    packet.servoId = servoId;
    packet.length = 4;
    packet.instructionErrorId = MX28_READ_DATA;
    packet.parameter[0] = MX28_RETURN_DELAY_TIME;
    packet.parameter[1] = 0x01;
 
    uint8_t status = CommunicatePacket(&packet);
        
    if(status == MX28_ERRBIT_NONE)
    {
        *returnDelayTime = packet.parameter[0];        
        
        #ifdef MX28_DEBUG
            pc->printf("Get return delay time: 0x%02X\r\n", *returnDelayTime);
        #endif
    }
    
    return status;
}

uint8_t MX28::SetReturnDelayTime(uint8_t servoId, uint8_t returnDelayTime, bool isRegWrite)
{
    MX28_PROTOCOL_PACKET packet;
       
    packet.servoId = servoId;
    packet.length = 4;
    packet.instructionErrorId = (isRegWrite == true) ? MX28_REG_WRITE : MX28_WRITE_DATA;
    packet.parameter[0] = MX28_RETURN_DELAY_TIME;
    packet.parameter[1] = returnDelayTime;
    
    #ifdef MX28_DEBUG
        pc->printf("Set return delay time: 0x%02X\r\n", returnDelayTime);
    #endif

    return CommunicatePacket(&packet);  
}

uint8_t MX28::GetCWAngleLimit(uint8_t servoId, uint16_t *cwAngleLimit)
{
    MX28_PROTOCOL_PACKET packet;
    
    packet.servoId = servoId;
    packet.length = 4;
    packet.instructionErrorId = MX28_READ_DATA;
    packet.parameter[0] = MX28_CW_ANGLE_LIMIT_L;
    packet.parameter[1] = 0x02;
 
    uint8_t status = CommunicatePacket(&packet);
        
    if(status == MX28_ERRBIT_NONE)
    {
        *cwAngleLimit = Utilities::ConvertUInt8ArrayToUInt16(packet.parameter);       
        
        #ifdef MX28_DEBUG
            pc->printf("Set CW angle limit: %hu\r\n", *cwAngleLimit);
        #endif
    }
    
    return status;
}

uint8_t MX28::SetCWAngleLimit(uint8_t servoId, uint16_t cwAngleLimit, bool isRegWrite)
{
    MX28_PROTOCOL_PACKET packet;
       
    packet.servoId = servoId;
    packet.length = 5;
    packet.instructionErrorId = (isRegWrite == true) ? MX28_REG_WRITE : MX28_WRITE_DATA;
    packet.parameter[0] = MX28_CW_ANGLE_LIMIT_L;
    Utilities::ConvertInt16ToUInt8Array(cwAngleLimit, (uint8_t*)&(packet.parameter[1]));
    
    #ifdef MX28_DEBUG
        pc->printf("Set Cw angle limit: %hu\r\n", cwAngleLimit);
    #endif

    return CommunicatePacket(&packet);  
}

uint8_t MX28::GetCCWAngleLimit(uint8_t servoId, uint16_t *ccwAngleLimit)
{
    MX28_PROTOCOL_PACKET packet;
    
    packet.servoId = servoId;
    packet.length = 4;
    packet.instructionErrorId = MX28_READ_DATA;
    packet.parameter[0] = MX28_CCW_ANGLE_LIMIT_L;
    packet.parameter[1] = 0x02;
 
    uint8_t status = CommunicatePacket(&packet);
        
    if(status == MX28_ERRBIT_NONE)
    {
        *ccwAngleLimit = Utilities::ConvertUInt8ArrayToUInt16(packet.parameter);       
        
        #ifdef MX28_DEBUG
            pc->printf("Get CCW angle limit: %hu\r\n", *ccwAngleLimit);
        #endif
    }
    
    return status;
}

uint8_t MX28::SetCCWAngleLimit(uint8_t servoId, uint16_t ccwAngleLimit, bool isRegWrite)
{
    MX28_PROTOCOL_PACKET packet;
       
    packet.servoId = servoId;
    packet.length = 5;
    packet.instructionErrorId = (isRegWrite == true) ? MX28_REG_WRITE : MX28_WRITE_DATA;
    packet.parameter[0] = MX28_CCW_ANGLE_LIMIT_L;
    Utilities::ConvertUInt16ToUInt8Array(ccwAngleLimit, (uint8_t*)&(packet.parameter[1]));
    
    #ifdef MX28_DEBUG
        pc->printf("Set CCW angle limit: %hu\r\n", ccwAngleLimit);
    #endif

    return CommunicatePacket(&packet);  
}

uint8_t MX28::GetHighestTemperatureLimit(uint8_t servoId, uint8_t *highestTemperatureLimit)
{
    MX28_PROTOCOL_PACKET packet;
    
    packet.servoId = servoId;
    packet.length = 4;
    packet.instructionErrorId = MX28_READ_DATA;
    packet.parameter[0] = MX28_UP_LIMIT_TEMPERATURE;
    packet.parameter[1] = 0x01;
 
    uint8_t status = CommunicatePacket(&packet);
        
    if(status == MX28_ERRBIT_NONE)
    {
        *highestTemperatureLimit = packet.parameter[0];       
        
        #ifdef MX28_DEBUG
            pc->printf("Get highest temperature limit: 0x%02X\r\n", *highestTemperatureLimit);
        #endif
    }
    
    return status;
}

uint8_t MX28::SetHighestTemperatureLimit(uint8_t servoId, uint8_t highestTemperatureLimit, bool isRegWrite)
{
    MX28_PROTOCOL_PACKET packet;
       
    packet.servoId = servoId;
    packet.length = 4;
    packet.instructionErrorId = (isRegWrite == true) ? MX28_REG_WRITE : MX28_WRITE_DATA;
    packet.parameter[0] = MX28_UP_LIMIT_TEMPERATURE;
    packet.parameter[1] = highestTemperatureLimit;
    
    #ifdef MX28_DEBUG
        pc->printf("Set highest temperature limit: 0x%02X\r\n", highestTemperatureLimit);
    #endif

    return CommunicatePacket(&packet);
}

uint8_t MX28::GetLowestVoltageLimit(uint8_t servoId, uint8_t *lowestVoltageLimit)
{
    MX28_PROTOCOL_PACKET packet;
    
    packet.servoId = servoId;
    packet.length = 4;
    packet.instructionErrorId = MX28_READ_DATA;
    packet.parameter[0] = MX28_DOWN_LIMIT_VOLTAGE;
    packet.parameter[1] = 0x01;
 
    uint8_t status = CommunicatePacket(&packet);
        
    if(status == MX28_ERRBIT_NONE)
    {
        *lowestVoltageLimit = packet.parameter[0];       
        
        #ifdef MX28_DEBUG
            pc->printf("Get lowest voltage limit: 0x%02X\r\n", *lowestVoltageLimit);
        #endif
    }
    
    return status;
}

uint8_t MX28::SetLowestVoltageLimit(uint8_t servoId, uint8_t lowestVoltageLimit, bool isRegWrite)
{
    MX28_PROTOCOL_PACKET packet;
       
    packet.servoId = servoId;
    packet.length = 4;
    packet.instructionErrorId = (isRegWrite == true) ? MX28_REG_WRITE : MX28_WRITE_DATA;
    packet.parameter[0] = MX28_DOWN_LIMIT_VOLTAGE;
    packet.parameter[1] = lowestVoltageLimit;
    
    #ifdef MX28_DEBUG
        pc->printf("Set lowest voltage limit: 0x%02X\r\n", lowestVoltageLimit);
    #endif

    return CommunicatePacket(&packet);
}

uint8_t MX28::GetHighestVoltageLimit(uint8_t servoId, uint8_t *highestVoltageLimit)
{
    MX28_PROTOCOL_PACKET packet;
    
    packet.servoId = servoId;
    packet.length = 4;
    packet.instructionErrorId = MX28_READ_DATA;
    packet.parameter[0] = MX28_UP_LIMIT_VOLTAGE;
    packet.parameter[1] = 0x01;
 
    uint8_t status = CommunicatePacket(&packet);
        
    if(status == MX28_ERRBIT_NONE)
    {
        *highestVoltageLimit = packet.parameter[0];       
        
        #ifdef MX28_DEBUG
            pc->printf("Get highest voltage limit: 0x%02X\r\n", *highestVoltageLimit);
        #endif
    }
    
    return status;
} 

uint8_t MX28::SetHighestVoltageLimit(uint8_t servoId, uint8_t highestVoltageLimit, bool isRegWrite)
{
    MX28_PROTOCOL_PACKET packet;
       
    packet.servoId = servoId;
    packet.length = 4;
    packet.instructionErrorId = (isRegWrite == true) ? MX28_REG_WRITE : MX28_WRITE_DATA;
    packet.parameter[0] = MX28_UP_LIMIT_VOLTAGE;
    packet.parameter[1] = highestVoltageLimit;
    
    #ifdef MX28_DEBUG
        pc->printf("Set highest voltage limit: 0x%02X\r\n", highestVoltageLimit);
    #endif

    return CommunicatePacket(&packet);
}

uint8_t MX28::GetMaxTorque(uint8_t servoId, uint16_t *maxTorque)
{
    MX28_PROTOCOL_PACKET packet;
    
    packet.servoId = servoId;
    packet.length = 4;
    packet.instructionErrorId = MX28_READ_DATA;
    packet.parameter[0] = MX28_MAX_TORQUE_L;
    packet.parameter[1] = 0x02;
 
    uint8_t status = CommunicatePacket(&packet);
        
    if(status == MX28_ERRBIT_NONE)
    {
        *maxTorque = Utilities::ConvertUInt8ArrayToUInt16(packet.parameter);       
        
        #ifdef MX28_DEBUG
            pc->printf("Get max torque: %hu\r\n", *maxTorque);
        #endif
    }
    
    return status;
} 

uint8_t MX28::SetMaxTorque(uint8_t servoId, uint16_t maxTorque, bool isRegWrite)
{
    MX28_PROTOCOL_PACKET packet;
       
    packet.servoId = servoId;
    packet.length = 5;
    packet.instructionErrorId = (isRegWrite == true) ? MX28_REG_WRITE : MX28_WRITE_DATA;
    packet.parameter[0] = MX28_MAX_TORQUE_L;
    Utilities::ConvertUInt16ToUInt8Array(maxTorque, (uint8_t*)&(packet.parameter[1]));
    
    #ifdef MX28_DEBUG
        pc->printf("Set max torque: %hu\r\n", maxTorque);
    #endif
   
    return CommunicatePacket(&packet);  
}

uint8_t MX28::GetStatusReturnLevel(uint8_t servoId, uint8_t *statusReturnLevel)
{
    MX28_PROTOCOL_PACKET packet;
    
    packet.servoId = servoId;
    packet.length = 4;
    packet.instructionErrorId = MX28_READ_DATA;
    packet.parameter[0] = MX28_STATUS_RETURN_LEVEL;
    packet.parameter[1] = 0x01;
 
    uint8_t status = CommunicatePacket(&packet);
        
    if(status == MX28_ERRBIT_NONE)
    {
        *statusReturnLevel = packet.parameter[0];        
        
        #ifdef MX28_DEBUG
            pc->printf("Get status return level: 0x%02X\r\n", *statusReturnLevel);
        #endif
    }
    
    return status;
}

uint8_t MX28::SetStatusReturnLevel(uint8_t servoId, uint8_t statusReturnLevel, bool isRegWrite)
{
    MX28_PROTOCOL_PACKET packet;

    packet.servoId = servoId;
    packet.length = 4;
    packet.instructionErrorId = (isRegWrite == true) ? MX28_REG_WRITE : MX28_WRITE_DATA;
    packet.parameter[0] = MX28_STATUS_RETURN_LEVEL;
    packet.parameter[1] = statusReturnLevel;
    
    #ifdef MX28_DEBUG
        pc->printf("Set status return level: 0x%02X\r\n", statusReturnLevel);
    #endif

    return CommunicatePacket(&packet);  
}

uint8_t MX28::GetAlarmLED(uint8_t servoId, uint8_t *alarmLED)
{
    MX28_PROTOCOL_PACKET packet;
    
    packet.servoId = servoId;
    packet.length = 4;
    packet.instructionErrorId = MX28_READ_DATA;
    packet.parameter[0] = MX28_ALARM_LED;
    packet.parameter[1] = 0x01;
 
    uint8_t status = CommunicatePacket(&packet);
        
    if(status == MX28_ERRBIT_NONE)
    {
        *alarmLED = packet.parameter[0];        
        
        #ifdef MX28_DEBUG
            pc->printf("Get alarm LED: 0x%02X\r\n", *alarmLED);
        #endif
    }
    
    return status;
}

uint8_t MX28::SetAlarmLED(uint8_t servoId, uint8_t alarmLED, bool isRegWrite)
{
    MX28_PROTOCOL_PACKET packet;

    packet.servoId = servoId;
    packet.length = 4;
    packet.instructionErrorId = (isRegWrite == true) ? MX28_REG_WRITE : MX28_WRITE_DATA;
    packet.parameter[0] = MX28_ALARM_LED;
    packet.parameter[1] = alarmLED;
    
    #ifdef MX28_DEBUG
        pc->printf("Set alarm LED: 0x%02X\r\n", alarmLED);
    #endif

    return CommunicatePacket(&packet);  
}

uint8_t MX28::GetAlarmShutdown(uint8_t servoId, uint8_t *alarmShutdown)
{
    MX28_PROTOCOL_PACKET packet;
    
    packet.servoId = servoId;
    packet.length = 4;
    packet.instructionErrorId = MX28_READ_DATA;
    packet.parameter[0] = MX28_ALARM_SHUTDOWN;
    packet.parameter[1] = 0x01;
 
    uint8_t status = CommunicatePacket(&packet);
        
    if(status == MX28_ERRBIT_NONE)
    {
        *alarmShutdown = packet.parameter[0];        
        
        #ifdef MX28_DEBUG
            pc->printf("Get alarm shutdown: 0x%02X\r\n", *alarmShutdown);
        #endif
    }
    
    return status;
}

uint8_t MX28::SetAlarmShutdown(uint8_t servoId, uint8_t alarmShutdown, bool isRegWrite)
{
    MX28_PROTOCOL_PACKET packet;

    packet.servoId = servoId;
    packet.length = 4;
    packet.instructionErrorId = (isRegWrite == true) ? MX28_REG_WRITE : MX28_WRITE_DATA;
    packet.parameter[0] = MX28_ALARM_SHUTDOWN;
    packet.parameter[1] = alarmShutdown;
    
    #ifdef MX28_DEBUG
        pc->printf("Set alarm shutdown: 0x%02X\r\n", alarmShutdown);
    #endif

    return CommunicatePacket(&packet);  
}

uint8_t MX28::GetEnableTorque(uint8_t servoId, uint8_t *enableTorque)
{
    MX28_PROTOCOL_PACKET packet;
    
    packet.servoId = servoId;
    packet.length = 4;
    packet.instructionErrorId = MX28_READ_DATA;
    packet.parameter[0] = MX28_TORQUE_ENABLE;
    packet.parameter[1] = 0x01;
 
    uint8_t status = CommunicatePacket(&packet);
        
    if(status == MX28_ERRBIT_NONE)
    {
        *enableTorque = packet.parameter[0];        
        
        #ifdef MX28_DEBUG
            pc->printf("Get enable torque: 0x%02X\r\n", *enableTorque);
        #endif
    }
    
    return status;
}

uint8_t MX28::SetEnableTorque(uint8_t servoId, uint8_t enableTorque, bool isRegWrite)
{
    MX28_PROTOCOL_PACKET packet;

    packet.servoId = servoId;
    packet.length = 4;
    packet.instructionErrorId = (isRegWrite == true) ? MX28_REG_WRITE : MX28_WRITE_DATA;
    packet.parameter[0] = MX28_TORQUE_ENABLE;
    packet.parameter[1] = enableTorque;
    
    #ifdef MX28_DEBUG
        pc->printf("Set enable torque: 0x%02X\r\n", enableTorque);
    #endif

    return CommunicatePacket(&packet);  
}

uint8_t MX28::GetEnableLED(uint8_t servoId, uint8_t *enableLED)
{
    MX28_PROTOCOL_PACKET packet;
    
    packet.servoId = servoId;
    packet.length = 4;
    packet.instructionErrorId = MX28_READ_DATA;
    packet.parameter[0] = MX28_LED_ENABLE;
    packet.parameter[1] = 0x01;
 
    uint8_t status = CommunicatePacket(&packet);
        
    if(status == MX28_ERRBIT_NONE)
    {
        *enableLED = packet.parameter[0];        
        
        #ifdef MX28_DEBUG
            pc->printf("Get enable LED: 0x%02X\r\n", *enableLED);
        #endif
    }
    
    return status;
}

uint8_t MX28::SetEnableLED(uint8_t servoId, uint8_t enableLED, bool isRegWrite)
{
    MX28_PROTOCOL_PACKET packet;

    packet.servoId = servoId;
    packet.length = 4;
    packet.instructionErrorId = (isRegWrite == true) ? MX28_REG_WRITE : MX28_WRITE_DATA;
    packet.parameter[0] = MX28_LED_ENABLE;
    packet.parameter[1] = enableLED;
    
    #ifdef MX28_DEBUG
        pc->printf("Set enable LED: 0x%02X\r\n", enableLED);
    #endif

    return CommunicatePacket(&packet);  
}

uint8_t MX28::GetPGain(uint8_t servoId, uint8_t *pGain)
{
    MX28_PROTOCOL_PACKET packet;
    
    packet.servoId = servoId;
    packet.length = 4;
    packet.instructionErrorId = MX28_READ_DATA;
    packet.parameter[0] = MX28_P_GAIN;
    packet.parameter[1] = 0x01;
 
    uint8_t status = CommunicatePacket(&packet);
        
    if(status == MX28_ERRBIT_NONE)
    {
        *pGain = packet.parameter[0];        
        
        #ifdef MX28_DEBUG
            pc->printf("Get P gain: 0x%02X\r\n", *pGain);
        #endif
    }
    
    return status;
}

uint8_t MX28::SetPGain(uint8_t servoId, uint8_t pGain, bool isRegWrite)
{
    MX28_PROTOCOL_PACKET packet;

    packet.servoId = servoId;
    packet.length = 4;
    packet.instructionErrorId = (isRegWrite == true) ? MX28_REG_WRITE : MX28_WRITE_DATA;
    packet.parameter[0] = MX28_P_GAIN;
    packet.parameter[1] = pGain;
    
    #ifdef MX28_DEBUG
        pc->printf("Set P gain: 0x%02X\r\n", pGain);
    #endif

    return CommunicatePacket(&packet);  
}

uint8_t MX28::GetIGain(uint8_t servoId, uint8_t *iGain)
{
    MX28_PROTOCOL_PACKET packet;
    
    packet.servoId = servoId;
    packet.length = 4;
    packet.instructionErrorId = MX28_READ_DATA;
    packet.parameter[0] = MX28_I_GAIN;
    packet.parameter[1] = 0x01;
 
    uint8_t status = CommunicatePacket(&packet);
        
    if(status == MX28_ERRBIT_NONE)
    {
        *iGain = packet.parameter[0];        
        
        #ifdef MX28_DEBUG
            pc->printf("Get I gain: 0x%02X\r\n", *iGain);
        #endif
    }
    
    return status;
}

uint8_t MX28::SetIGain(uint8_t servoId, uint8_t iGain, bool isRegWrite)
{
    MX28_PROTOCOL_PACKET packet;

    packet.servoId = servoId;
    packet.length = 4;
    packet.instructionErrorId = (isRegWrite == true) ? MX28_REG_WRITE : MX28_WRITE_DATA;
    packet.parameter[0] = MX28_I_GAIN;
    packet.parameter[1] = iGain;
    
    #ifdef MX28_DEBUG
        pc->printf("Set I gain: 0x%02X\r\n", iGain);
    #endif

    return CommunicatePacket(&packet);  
}

uint8_t MX28::GetDGain(uint8_t servoId, uint8_t *dGain)
{
    MX28_PROTOCOL_PACKET packet;
    
    packet.servoId = servoId;
    packet.length = 4;
    packet.instructionErrorId = MX28_READ_DATA;
    packet.parameter[0] = MX28_D_GAIN;
    packet.parameter[1] = 0x01;
 
    uint8_t status = CommunicatePacket(&packet);
        
    if(status == MX28_ERRBIT_NONE)
    {
        *dGain = packet.parameter[0];        
        
        #ifdef MX28_DEBUG
            pc->printf("Get D gain: 0x%02X\r\n", *dGain);
        #endif
    }
    
    return status;
}

uint8_t MX28::SetDGain(uint8_t servoId, uint8_t dGain, bool isRegWrite)
{
    MX28_PROTOCOL_PACKET packet;

    packet.servoId = servoId;
    packet.length = 4;
    packet.instructionErrorId = (isRegWrite == true) ? MX28_REG_WRITE : MX28_WRITE_DATA;
    packet.parameter[0] = MX28_D_GAIN;
    packet.parameter[1] = dGain;
    
    #ifdef MX28_DEBUG
        pc->printf("Set D gain: 0x%02X\r\n", dGain);
    #endif

    return CommunicatePacket(&packet);  
}

uint8_t MX28::GetGoalPosition(uint8_t servoId, uint16_t *goalPosition)
{
    MX28_PROTOCOL_PACKET packet;
    
    packet.servoId = servoId;
    packet.length = 4;
    packet.instructionErrorId = MX28_READ_DATA;
    packet.parameter[0] = MX28_GOAL_POSITION_L;
    packet.parameter[1] = 0x02;
 
    uint8_t status = CommunicatePacket(&packet);
        
    if(status == MX28_ERRBIT_NONE)
    {
        *goalPosition = Utilities::ConvertUInt8ArrayToUInt16(packet.parameter);       
        
        #ifdef MX28_DEBUG
            pc->printf("Get goal position: %hu\r\n", *goalPosition);
        #endif
    }
    
    return status;
} 

uint8_t MX28::SetGoalPosition(uint8_t servoId, uint16_t goalPosition, bool isRegWrite)
{
    MX28_PROTOCOL_PACKET packet;
       
    packet.servoId = servoId;
    packet.length = 5;
    packet.instructionErrorId = (isRegWrite == true) ? MX28_REG_WRITE : MX28_WRITE_DATA;
    packet.parameter[0] = MX28_GOAL_POSITION_L;
    Utilities::ConvertUInt16ToUInt8Array(goalPosition, (uint8_t*)&(packet.parameter[1]));
    
    #ifdef MX28_DEBUG
        pc->printf("Set goal position: %hu\r\n", goalPosition);
    #endif

    return CommunicatePacket(&packet);  
}

uint8_t MX28::GetMovingSpeed(uint8_t servoId, uint16_t *movingSpeed)
{
    MX28_PROTOCOL_PACKET packet;
    
    packet.servoId = servoId;
    packet.length = 4;
    packet.instructionErrorId = MX28_READ_DATA;
    packet.parameter[0] = MX28_MOVING_SPEED_L;
    packet.parameter[1] = 0x02;
 
    uint8_t status = CommunicatePacket(&packet);
        
    if(status == MX28_ERRBIT_NONE)
    {
        *movingSpeed = Utilities::ConvertUInt8ArrayToUInt16(packet.parameter);       
        
        #ifdef MX28_DEBUG
            pc->printf("Get moving speed: %hu\r\n", *movingSpeed);
        #endif
    }
    
    return status;
} 

uint8_t MX28::SetMovingSpeed(uint8_t servoId, uint16_t movingSpeed, bool isRegWrite)
{
    MX28_PROTOCOL_PACKET packet;
       
    packet.servoId = servoId;
    packet.length = 5;
    packet.instructionErrorId = (isRegWrite == true) ? MX28_REG_WRITE : MX28_WRITE_DATA;
    packet.parameter[0] = MX28_MOVING_SPEED_L;
    Utilities::ConvertUInt16ToUInt8Array(movingSpeed, (uint8_t*)&(packet.parameter[1]));
    
    #ifdef MX28_DEBUG
        pc->printf("Set moving speed: %hu\r\n", movingSpeed);
    #endif

    return CommunicatePacket(&packet);  
}

uint8_t MX28::GetTorqueLimit(uint8_t servoId, uint16_t *torqueLimit)
{
    MX28_PROTOCOL_PACKET packet;
    
    packet.servoId = servoId;
    packet.length = 4;
    packet.instructionErrorId = MX28_READ_DATA;
    packet.parameter[0] = MX28_TORQUE_LIMIT_L;
    packet.parameter[1] = 0x02;
 
    uint8_t status = CommunicatePacket(&packet);
        
    if(status == MX28_ERRBIT_NONE)
    {
        *torqueLimit = Utilities::ConvertUInt8ArrayToUInt16(packet.parameter);       
        
        #ifdef MX28_DEBUG
            pc->printf("Torque limit: %hu\r\n", *torqueLimit);
        #endif
    }
    
    return status;
} 

uint8_t MX28::SetTorqueLimit(uint8_t servoId, uint16_t torqueLimit, bool isRegWrite)
{
    MX28_PROTOCOL_PACKET packet;
       
    packet.servoId = servoId;
    packet.length = 5;
    packet.instructionErrorId = (isRegWrite == true) ? MX28_REG_WRITE : MX28_WRITE_DATA;
    packet.parameter[0] = MX28_TORQUE_LIMIT_L;
    Utilities::ConvertUInt16ToUInt8Array(torqueLimit, (uint8_t*)&(packet.parameter[1]));
    
    #ifdef MX28_DEBUG
        pc->printf("Set torque limit: %hu\r\n", torqueLimit);
    #endif

    return CommunicatePacket(&packet);  
}

uint8_t MX28::GetPresentPosition(uint8_t servoId, uint16_t *presentPosition)
{
    MX28_PROTOCOL_PACKET packet;
    
    packet.servoId = servoId;
    packet.length = 4;
    packet.instructionErrorId = MX28_READ_DATA;
    packet.parameter[0] = MX28_PRESENT_POSITION_L;
    packet.parameter[1] = 0x02;
 
    uint8_t status = CommunicatePacket(&packet);
        
    if(status == MX28_ERRBIT_NONE)
    {
        *presentPosition = Utilities::ConvertUInt8ArrayToUInt16(packet.parameter);       
        
        #ifdef MX28_DEBUG
            pc->printf("Get present position: %hu\r\n", *presentPosition);
        #endif
    }
    
    return status;
} 

uint8_t MX28::GetPresentSpeed(uint8_t servoId, uint16_t *presentSpeed)
{
    MX28_PROTOCOL_PACKET packet;
    
    packet.servoId = servoId;
    packet.length = 4;
    packet.instructionErrorId = MX28_READ_DATA;
    packet.parameter[0] = MX28_PRESENT_SPEED_L;
    packet.parameter[1] = 0x02;
 
    uint8_t status = CommunicatePacket(&packet);
        
    if(status == MX28_ERRBIT_NONE)
    {
        *presentSpeed = Utilities::ConvertUInt8ArrayToUInt16(packet.parameter);       
        
        #ifdef MX28_DEBUG
            pc->printf("Get present speed: %hu\r\n", *presentSpeed);
        #endif
    }
    
    return status;
} 

uint8_t MX28::GetPresentLoad(uint8_t servoId, uint16_t *presentLoad)
{
    MX28_PROTOCOL_PACKET packet;
    
    packet.servoId = servoId;
    packet.length = 4;
    packet.instructionErrorId = MX28_READ_DATA;
    packet.parameter[0] = MX28_PRESENT_LOAD_L;
    packet.parameter[1] = 0x02;
 
    uint8_t status = CommunicatePacket(&packet);
        
    if(status == MX28_ERRBIT_NONE)
    {
        *presentLoad = Utilities::ConvertUInt8ArrayToUInt16(packet.parameter);       
        
        #ifdef MX28_DEBUG
            pc->printf("Get present load: %hu\r\n", *presentLoad);
        #endif
    }
    
    return status;
} 

uint8_t MX28::GetPresentVoltage(uint8_t servoId, uint8_t *presentVoltage)
{
    MX28_PROTOCOL_PACKET packet;
    
    packet.servoId = servoId;
    packet.length = 4;
    packet.instructionErrorId = MX28_READ_DATA;
    packet.parameter[0] = MX28_PRESENT_VOLTAGE;
    packet.parameter[1] = 0x01;
 
    uint8_t status = CommunicatePacket(&packet);
        
    if(status == MX28_ERRBIT_NONE)
    {
        *presentVoltage = packet.parameter[0];        
        
        #ifdef MX28_DEBUG
            pc->printf("Get present voltage: 0x%02X\r\n", *presentVoltage);
        #endif
    }
    
    return status;
}

uint8_t MX28::GetPresentTemperature(uint8_t servoId, uint8_t *presentTemperature)
{
    MX28_PROTOCOL_PACKET packet;
    
    packet.servoId = servoId;
    packet.length = 4;
    packet.instructionErrorId = MX28_READ_DATA;
    packet.parameter[0] = MX28_PRESENT_TEMPERATURE;
    packet.parameter[1] = 0x01;
 
    uint8_t status = CommunicatePacket(&packet);
        
    if(status == MX28_ERRBIT_NONE)
    {
        *presentTemperature = packet.parameter[0];        
        
        #ifdef MX28_DEBUG
            pc->printf("Get present temperature: 0x%02X\r\n", *presentTemperature);
        #endif
    }
    
    return status;
}

uint8_t MX28::GetIsRegistered(uint8_t servoId, uint8_t *registered)
{
    MX28_PROTOCOL_PACKET packet;
    
    packet.servoId = servoId;
    packet.length = 4;
    packet.instructionErrorId = MX28_READ_DATA;
    packet.parameter[0] = MX28_PRESENT_TEMPERATURE;
    packet.parameter[1] = 0x01;
 
    uint8_t status = CommunicatePacket(&packet);
        
    if(status == MX28_ERRBIT_NONE)
    {
        *registered = packet.parameter[0];        
        
        #ifdef MX28_DEBUG
            pc->printf("Get is registered: 0x%02X\r\n", *registered);
        #endif
    }
    
    return status;
}

uint8_t MX28::GetIsMoving(uint8_t servoId, uint8_t *moving)
{
    MX28_PROTOCOL_PACKET packet;
    
    packet.servoId = servoId;
    packet.length = 4;
    packet.instructionErrorId = MX28_READ_DATA;
    packet.parameter[0] = MX28_PRESENT_TEMPERATURE;
    packet.parameter[1] = 0x01;
 
    uint8_t status = CommunicatePacket(&packet);
        
    if(status == MX28_ERRBIT_NONE)
    {
        *moving = packet.parameter[0];        
        
        #ifdef MX28_DEBUG
            pc->printf("Get is moving: 0x%02X\r\n", *moving);
        #endif
    }
    
    return status;
}

uint8_t MX28::GetIsLocked(uint8_t servoId, uint8_t *isLocked)
{
    MX28_PROTOCOL_PACKET packet;
    
    packet.servoId = servoId;
    packet.length = 4;
    packet.instructionErrorId = MX28_READ_DATA;
    packet.parameter[0] = MX28_LOCK;
    packet.parameter[1] = 0x01;
 
    uint8_t status = CommunicatePacket(&packet);
        
    if(status == MX28_ERRBIT_NONE)
    {
        *isLocked = packet.parameter[0];        
        
        #ifdef MX28_DEBUG
            pc->printf("Get is locked: 0x%02X\r\n", *isLocked);
        #endif
    }
    
    return status;
}

uint8_t MX28::SetIsLocked(uint8_t servoId, uint8_t isLocked, bool isRegWrite)
{
    MX28_PROTOCOL_PACKET packet;

    packet.servoId = servoId;
    packet.length = 4;
    packet.instructionErrorId = (isRegWrite == true) ? MX28_REG_WRITE : MX28_WRITE_DATA;
    packet.parameter[0] = MX28_LOCK;
    packet.parameter[1] = isLocked;
    
    #ifdef MX28_DEBUG
        pc->printf("Set is locked: 0x%02X\r\n", isLocked);
    #endif

    return CommunicatePacket(&packet);  
}

uint8_t MX28::GetPunch(uint8_t servoId, uint16_t *punch)
{
    MX28_PROTOCOL_PACKET packet;
    
    packet.servoId = servoId;
    packet.length = 4;
    packet.instructionErrorId = MX28_READ_DATA;
    packet.parameter[0] = MX28_PUNCH_L;
    packet.parameter[1] = 0x02;
 
    uint8_t status = CommunicatePacket(&packet);
        
    if(status == MX28_ERRBIT_NONE)
    {
        *punch = Utilities::ConvertUInt8ArrayToUInt16(packet.parameter);       
        
        #ifdef MX28_DEBUG
            pc->printf("Get punch: %hu\r\n", *punch);
        #endif
    }
    
    return status;
}

uint8_t MX28::SetPunch(uint8_t servoId, uint16_t punch, bool isRegWrite)
{
    MX28_PROTOCOL_PACKET packet;
       
    packet.servoId = servoId;
    packet.length = 5;
    packet.instructionErrorId = (isRegWrite == true) ? MX28_REG_WRITE : MX28_WRITE_DATA;
    packet.parameter[0] = MX28_PUNCH_L;
    Utilities::ConvertUInt16ToUInt8Array(punch, (uint8_t*)&(packet.parameter[1]));
    
    #ifdef MX28_DEBUG
        pc->printf("Set punch: %hu\r\n", punch);
    #endif

    return CommunicatePacket(&packet);  
}

uint8_t MX28::Ping(uint8_t servoId)
{
    MX28_PROTOCOL_PACKET packet;
       
    packet.servoId = servoId;
    packet.length = 2;
    packet.instructionErrorId = MX28_PING;
        
    #ifdef MX28_DEBUG
        pc->printf("Ping\r\n");
    #endif

    return CommunicatePacket(&packet);  
}

uint8_t MX28::Reset(uint8_t servoId)
{
    MX28_PROTOCOL_PACKET packet;
       
    packet.servoId = servoId;
    packet.length = 2;
    packet.instructionErrorId = MX28_RESET;
        
    #ifdef MX28_DEBUG
        pc->printf("Reset\r\n");
    #endif

    return CommunicatePacket(&packet);  
}

uint8_t MX28::Action(uint8_t servoId)
{
    MX28_PROTOCOL_PACKET packet;
       
    packet.servoId = servoId;
    packet.length = 2;
    packet.instructionErrorId =  MX28_ACTION;
        
    #ifdef MX28_DEBUG
        pc->printf("Action\r\n");
    #endif

    return CommunicatePacket(&packet);  
}

MX28::MX28(PinName tx, PinName rx, int baudRate)
{
    #ifdef MX28_DEBUG
        pc = new Serial(USBTX, USBRX);
        pc->baud(115200);
        pc->printf("\033[2J");
    #endif
    
    servoSerialHalfDuplex = new SerialHalfDuplex(tx, rx);
    servoSerialHalfDuplex->baud(baudRate);    
}

MX28::~MX28()
{
    #ifdef MX28_DEBUG
        if(pc != NULL)           
            delete pc;        
    #endif
    
    if(servoSerialHalfDuplex != NULL)
        delete servoSerialHalfDuplex;
}  
