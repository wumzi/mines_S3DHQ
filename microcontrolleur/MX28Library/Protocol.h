/* Dynamixel MX28 servo library
 * Copyright (c) 2012 Georgios Petrou, MIT License
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
 
#ifndef MX28_PROTOCOL_H
#define MX28_PROTOCOL_H

#include "mbed.h"

#define MX28_BUFFER_SIZE            0x8F

#define MX28_PROTOCOL_COMMAND_RESPONSE_TIMEOUT_MS   200

#define MX28_PROTOCOL_HEADER_0      0xFF
#define MX28_PROTOCOL_HEADER_1      0xFF

#define MX28_PROTOCOL_BROADCAST_ID  0xFE

// EEPROM Area
#define MX28_MODEL_NUMBER_L         0x00
#define MX28_MODEL_NUMBER_H         0x01
#define MX28_VERSION                0x02
#define MX28_ID                     0x03
#define MX28_BAUD_RATE              0x04
#define MX28_RETURN_DELAY_TIME      0x05
#define MX28_CW_ANGLE_LIMIT_L       0x06
#define MX28_CW_ANGLE_LIMIT_H       0x07
#define MX28_CCW_ANGLE_LIMIT_L      0x08
#define MX28_CCW_ANGLE_LIMIT_H      0x09
#define MX28_UP_LIMIT_TEMPERATURE   0x0B
#define MX28_DOWN_LIMIT_VOLTAGE     0x0C
#define MX28_UP_LIMIT_VOLTAGE       0x0D
#define MX28_MAX_TORQUE_L           0x0E
#define MX28_MAX_TORQUE_H           0x0F
#define MX28_STATUS_RETURN_LEVEL    0x10
#define MX28_ALARM_LED              0x11
#define MX28_ALARM_SHUTDOWN         0x12

// RAM Area
#define MX28_TORQUE_ENABLE          0x18
#define MX28_LED_ENABLE             0x19
#define MX28_D_GAIN                 0x1A
#define MX28_I_GAIN                 0x1B
#define MX28_P_GAIN                 0x1C
#define MX28_GOAL_POSITION_L        0x1E
#define MX28_GOAL_POSITION_H        0x1F
#define MX28_MOVING_SPEED_L         0x20
#define MX28_MOVING_SPEED_H         0x21
#define MX28_TORQUE_LIMIT_L         0x22
#define MX28_TORQUE_LIMIT_H         0x23
#define MX28_PRESENT_POSITION_L     0x24
#define MX28_PRESENT_POSITION_H     0x25
#define MX28_PRESENT_SPEED_L        0x26
#define MX28_PRESENT_SPEED_H        0x27
#define MX28_PRESENT_LOAD_L         0x28
#define MX28_PRESENT_LOAD_H         0x29
#define MX28_PRESENT_VOLTAGE        0x2A
#define MX28_PRESENT_TEMPERATURE    0x2B
#define MX28_REGISTERED_INSTRUCTION 0x2C
#define MX28_MOVING                 0x3E
#define MX28_LOCK                   0x2F
#define MX28_PUNCH_L                0x30
#define MX28_PUNCH_H                0x31

// Instruction set 
#define MX28_PING                   0x01
#define MX28_READ_DATA              0x02
#define MX28_WRITE_DATA             0x03
#define MX28_REG_WRITE              0x04
#define MX28_ACTION                 0x05
#define MX28_RESET                  0x06
#define MX28_SYNC_WRITE             0x83

// Errors
#define MX28_ERRBIT_NONE            0x00
#define MX28_ERRBIT_VOLTAGE         0x01
#define MX28_ERRBIT_ANGLE           0x02
#define MX28_ERRBIT_OVERHEAT        0x04
#define MX28_ERRBIT_RANGE           0x08
#define MX28_ERRBIT_CHECKSUM        0x10
#define MX28_ERRBIT_OVERLOAD        0x20
#define MX28_ERRBIT_INSTRUCTION     0x40

// Extra errors
#define MX28_ERRBIT_WRITE_TIMEOUT   0xFD
#define MX28_ERRBIT_READ_TIMEOUT    0xFE
#define MX28_ERRBIT_MASTER_CHECKSUM 0xFF

struct MX28_PROTOCOL_PACKET
{
    uint8_t servoId;
    uint8_t length;    
    uint8_t instructionErrorId;   
    uint8_t parameter[MX28_BUFFER_SIZE];
    uint8_t checkSum;
};

enum MX28_PROTOCOL_ENCODER_DECODER_STATE
{
    WAIT_ON_HEADER_0,
    WAIT_ON_HEADER_1,
    WAIT_ON_SERVO_ID,   
    WAIT_ON_LENGTH,
    WAIT_ON_INSTRUCTION_ERROR_ID,
    WAIT_ON_PARAMETER,
    WAIT_ON_CHECK_SUM
};

#endif // MX28_PROTOCOL_H 