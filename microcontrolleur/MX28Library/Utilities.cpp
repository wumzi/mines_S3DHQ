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
 
#include "Utilities.h"
 
uint8_t Utilities::GetCheckSum(const uint8_t *data, uint8_t length)
{
    uint8_t checkSum = 0;

    for(int i = 0; i < length; i++)
        checkSum += data[i];

    checkSum = ~checkSum;
    
    return checkSum;
}

void Utilities::ConvertUInt16ToUInt8Array(uint16_t value, uint8_t* data)
{
    data[0] = value;
    data[1] = value >> 8;
}        
      
void Utilities::ConvertInt16ToUInt8Array(int16_t value, uint8_t* data)
{
    data[0] = value;
    data[1] = value >> 8;
}           
  
void Utilities::ConvertUInt32ToUInt8Array(uint32_t value, uint8_t* data)
{
    data[0] = value;
    data[1] = value >> 8;
    data[2] = value >> 16;
    data[3] = value >> 24;
} 
      
void Utilities::ConvertInt32ToUInt8Array(int32_t value, uint8_t* data)
{
    data[0] = value;
    data[1] = value >> 8;
    data[2] = value >> 16;
    data[3] = value >> 24;
} 

uint16_t Utilities::ConvertUInt8ArrayToUInt16(uint8_t* data)
{
    return (((uint16_t)data[1]) << 8) | ((uint16_t)data[0]);
}

int16_t Utilities::ConvertUInt8ArrayToInt16(uint8_t* data)
{
    return (((int16_t)data[1]) << 8) | ((int16_t)data[0]);
}

int32_t Utilities::ConvertUInt8ArrayToInt32(uint8_t* data)
{
    return  (((int32_t)data[3]) << 24) | (((int32_t)data[2]) << 16) | (((int32_t)data[1]) << 8) | ((int32_t)data[0]);
}
 