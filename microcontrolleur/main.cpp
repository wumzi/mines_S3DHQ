/************************************

*************************************/

// ATTENTION : POUR TOUT NOUVEAU PROGRAMME, COPIER LE DOSSIER "MBED"

#include "mbed.h"

// *** DO NOT MODIFY THESE CONSTANTS ***
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

#define MX28_WAIT_AFTER_WRITE       200
// *** END "DO NOT MODIFY THESE CONSTANTS" SECTION ***

#define ID_ENGINE_VERT              0x0A
#define ID_ENGINE_HORIZ             0x01


#define PC_COMMAND_TIMEOUT          5

DigitalOut led(LED1);
DigitalOut LedRouge(p23); 
//DigitalOut dir(p26);
DigitalOut dir(p17);
DigitalOut laser(p13);
//Serial uart2(p28, p27); // tx, rx (c'est l'uart2 du LPC1768)
Serial uart3(p9, p10); // tx, rx
Serial pc(USBTX, USBRX);

// Change angle (received from PC) to goal position (sent to engines)
int angleToGoal(int angle)
{
    return ((angle * 2048) / 180) + 2048;
}

// Change goal position to angle
int goalToAngle(int goal)
{
    return ((goal - 2048) * 180) / 2048;
}

// Handle reception from engines
int listen()
{  
    char bufferC[8];
    uart3.gets(bufferC, 3);
    
    if (!(bufferC[0] == 0xFF && bufferC[1] == 0xFF))
    {
        //pc.printf("Message corrompu\n");
        return listen();
    }
    else
    {
        uart3.gets(bufferC, 2);
        int id = bufferC[0];
        uart3.gets(bufferC, 2);
        int length = bufferC[0];
        uart3.gets(bufferC, 2);
        int error = bufferC[0];
        //pc.printf("ID = %d\n Length = %d\n Status = %d\n", id, length, error);
        
        int r = 0;
        for (int i = 0; i < length-2; i++)
        {
            uart3.gets(bufferC, 2);
            int param = bufferC[0];
            //pc.printf("Param : %d", param);
            r = r + (param << 8 * i);
        }
        
        uart3.gets(bufferC, 2);
        int chs = bufferC[0];
        //pc.printf("\nCkS = %d", chs);
        
        return r;
    }
}

// Handle emission to engines
int write (char id, char instruction, char paramNb = 0, char c1 = NULL, char c2 = NULL, char c3 = NULL, char c4 = NULL)
{
    char Cks, l;
    
    l = paramNb + 0x02;
    
    //pc.printf("Length : %d\n", l);
    
    Cks = ~( id + l + instruction + c1 + c2 + c3 + c4);
    //pc.printf("Cks : %d\n", Cks);
    dir = 1;    
    uart3.putc(MX28_PROTOCOL_HEADER_0);
    uart3.putc(MX28_PROTOCOL_HEADER_1);
    uart3.putc(id);
    uart3.putc(l);
    uart3.putc(instruction);
    if (paramNb >= 1) { uart3.putc(c1); }
    if (paramNb >= 2) { uart3.putc(c2); }
    if (paramNb >= 3) { uart3.putc(c3); }
    if (paramNb >= 4) { uart3.putc(c4); }
    uart3.putc(Cks);
    
    wait_us(MX28_WAIT_AFTER_WRITE);  
    dir = 0;
    
    return listen();
} 

// Send read instruction
int read (char id, char c1)
{
    char response_l;
    
    // If need to read 2 bytes
    if (( c1 == 0x06 ) || ( c1 == 0x08 ) || ( c1 == 0x0E ) || ( c1 == 0x1E ) || ( c1 == 0x20 ) || ( c1 == 0x22 ) || (c1 == 0x24) || ( c1 == 0x30 ))
    {
        response_l = 0x02;
    }
    else
    {
        response_l = 0x01;
    }
    
    return write(id, MX28_READ_DATA, 2, c1, response_l);
}

// Send ping (to check the id)
int ping(char id)
{
    return write(id, MX28_PING, 0);
}

// Light up led
int enableLed(char id)
{
    return write(id, MX28_WRITE_DATA, 2, MX28_LED_ENABLE, 0x01);
}

// Light off led
int disableLed(char id)
{
    return write(id, MX28_WRITE_DATA, 2, MX28_LED_ENABLE, 0x00);
}

int getLed(char id)
{
    return read(id, MX28_LED_ENABLE);
}

// Get goal position of engine
int getPosition(char id)
{
    return read(id, MX28_PRESENT_POSITION_L);
}

// Set goal position of engine
int setPosition(char id, int goal)
{
    char goal_h = goal >> 8;
    char goal_l = goal;
    
    //pc.printf("Goal set : %d %d %d\n", goal, goal_h, goal_l);
    return write(id, MX28_WRITE_DATA, 3, MX28_GOAL_POSITION_L, goal_l, goal_h);
}

// Set integral gain
int setIGain(char id, int gain)
{
    return write(id, MX28_WRITE_DATA, 2, MX28_I_GAIN, gain);
}

int setPGain(char id, int gain)
{
	return write(id, MX28_WRITE_DATA, 2, MX28_P_GAIN, gain);
}

// Set moving speed of engine
int setSpeed(char id, int speed)
{
    char speed_h = speed >> 8;
    char speed_l = speed;
    
    return write(id, MX28_WRITE_DATA, 3, MX28_MOVING_SPEED_L, speed_l, speed_h);
}

// Get angle limit of engine
int getAngleLimit(char id)
{
    return read(id, MX28_CCW_ANGLE_LIMIT_L);
}

// Check if engine is moving
int getMoving(char id)
{
    return read(id, MX28_MOVING);
}

int main() {
    uart3.baud(57142);
    
    //pc.printf("Echoes back to the screen anything you type\n");
    char buffer[11];
    //led = 0;
    //led = 1;
    LedRouge=1;
    laser = 0;
    
    setSpeed(ID_ENGINE_VERT, 10);
    setSpeed(ID_ENGINE_HORIZ, 10);
    
    setIGain(ID_ENGINE_VERT, 8);
    setIGain(ID_ENGINE_HORIZ, 8);
    
    while(1) {
        // Clear buffer
        //memset(&buffer[0], 0, sizeof(buffer));
        pc.gets(buffer, 11);
        //pc.puts("MBED: ");
        //pc.printf("%s\n", buffer);
        
        if (buffer[0] == '$')
        {
            int a0 = (buffer[2] == '-' ? -1 : 1);
            int a1 = buffer[3] - 48;
            int a2 = buffer[4] - 48;
            int a3 = buffer[5] - 48;
            int a4 = (buffer[6] == '-' ? -1 : 1);
            int a5 = buffer[7] - 48;
            int a6 = buffer[8] - 48;
            int a7 = buffer[9] - 48;
            int angleVert = a0 * ((a1 * 100) + (a2 * 10) + a3);
            int angleHoriz = a4 * ((a5 * 100) + (a6 * 10) + a7);
            int goalVert = angleToGoal(angleVert);
            int goalHoriz = angleToGoal(angleHoriz);
            //pc.printf("Char %d %d %d \nAngle : %d\nGoal : %d", a1, a2, a3, angle, goal);
            //pc.printf("%d %d", angleVert, angleHoriz);
            
            if (buffer[1] == '0')
            {
                setPosition(ID_ENGINE_VERT, goalVert);
                setPosition(ID_ENGINE_HORIZ, goalHoriz);
                
                int currentPositionVert = getPosition(ID_ENGINE_VERT);
                int currentPositionHoriz = getPosition(ID_ENGINE_HORIZ);
                int incr = 1;
                while ((currentPositionVert != goalVert
                    || currentPositionHoriz != goalHoriz)
                && incr < PC_COMMAND_TIMEOUT)
                {
                    wait_ms(1500);
                    currentPositionVert = getPosition(ID_ENGINE_VERT);
                    currentPositionHoriz = getPosition(ID_ENGINE_HORIZ);
                    incr++;
                    
                    //pc.printf(" Goal = %d ; Current = %d", goalHoriz, currentPositionHoriz);
                }
                // If response is OK
                if (abs(currentPositionVert - goalVert) < 8
                    && abs(currentPositionHoriz - goalHoriz) < 8)
                {
                    pc.printf("done%c%c%c%c%c%c%c%c%c%c%c", buffer[0], buffer[1], buffer[2], buffer[3], buffer[4], buffer[5], buffer[6], buffer[7], buffer[8], buffer[9], buffer[10]);
                }
                // Else, ask to PC to resend command
                else
                {
                    pc.printf("retr%c%c%c%c%c%c%c%c%c%c%c", buffer[0], buffer[1], buffer[2], buffer[3], buffer[4], buffer[5], buffer[6], buffer[7], buffer[8], buffer[9], buffer[10]);
                }
                
            }
            else if (buffer[1] == '1')
            {
                if (a1 == 1)
                {
                    enableLed(0x01);
                    enableLed(0x0A);
                    laser = 1;
                }
                else
                {   
                    disableLed(0x01);
                    disableLed(0x0A);
                    laser = 0;
                }
            }
            else if (buffer[1] == '2')
            {
                getLed(0x01);
            }   
        }
    }
}
