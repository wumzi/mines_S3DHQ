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
 
#ifndef MX28_UTILITIES_H
#define MX28_UTILITIES_H

#include "mbed.h"

/** MX28 utility functions class
*/
class Utilities
{
    public:
        /** Get the checksum for a packet.
        *        
        * @param data The array of bytes.
        * @param length The size of the array.
        * @return checksum
        */
        static uint8_t GetCheckSum(const uint8_t *data, uint8_t length);
        
        /** Convert a UInt16 number to array of chars.
        *        
        * @param value The number value.
        * @param data The array to store the conversion.
        */
        static void ConvertUInt16ToUInt8Array(uint16_t value, uint8_t* data);

        /** Convert an Int16 number to array of chars.
        *        
        * @param value The number value.
        * @param data The array to store the conversion.
        */
        static void ConvertInt16ToUInt8Array(int16_t value, uint8_t* data);
        
        /** Convert a UInt32 number to array of chars.
        *        
        * @param value The number value.
        * @param data The array to store the conversion.
        */
        static void ConvertUInt32ToUInt8Array(uint32_t value, uint8_t* data);

        /** Convert an Int32 number to array of chars.
        *        
        * @param value The number value.
        * @param data The array to store the conversion.
        */
        static void ConvertInt32ToUInt8Array(int32_t value, uint8_t* data);
        
        /** Convert an array of char to UInt16.
        *        
        * @param data The array containing the chars.
        * @return UInt16 converted number.
        */
        static uint16_t ConvertUInt8ArrayToUInt16(uint8_t* data);
        
        /** Convert an array of char to Int16.
        *        
        * @param data The array containing the chars.
        * @return Int16 converted number.
        */
        static int16_t ConvertUInt8ArrayToInt16(uint8_t* data);
        
        /** Convert an array of char to Int32.
        *        
        * @param data The array containing the chars.
        * @return Int32 number.
        */
        static int32_t ConvertUInt8ArrayToInt32(uint8_t* data);
};

#endif // MX28_UTILITIES_H 

