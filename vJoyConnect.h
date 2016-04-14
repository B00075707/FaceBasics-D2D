#define _USE_MATH_DEFINES

// Monitor Force Feedback (FFB) vJoy device
#include "vjoystdafx.h"
//#include "Devioctl.h"
#include "public.h"
#include <malloc.h>
#include <string.h>
#include <stdlib.h>
#include "vjoyinterface.h"
#include <cmath>

// Windows Header Files
//#include <windows.h>

//#include <Shlobj.h>

// Default device ID (Used when ID not specified)
#define DEV_ID		1

// Prototypes
int setup_vJoy();
int Polar2Deg(BYTE Polar);
int Byte2Percent(BYTE InByte);
int TwosCompByte2Int(BYTE in);
int update_vJoy(int roll, int pitch, int yaw);
//JOYSTICK_POSITION_V2 iReport; // The structure that holds the full position data

//int stat = 0;
//UINT DevID = DEV_ID;
//USHORT X = 0;
//USHORT Y = 0;
//USHORT Z = 0;
//LONG   Btns = 0;

//PVOID pPositionMessage;
//UINT	IoCode = LOAD_POSITIONS;
//UINT	IoSize = sizeof(JOYSTICK_POSITION);
// HID_DEVICE_ATTRIBUTES attrib;
//BYTE id = 1;
//UINT iInterface = 1;