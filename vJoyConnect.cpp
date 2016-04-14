// vJoyClient.cpp : Simple feeder application with a FFB demo
//

#include "vJoyConnect.h"

int pitch = 0, yaw = 0, roll = 0;

int setup_vJoy()
{
	
	UINT DevID = DEV_ID;
	// Set the target Joystick - get it from the command-line 
	//if (argc>1)
	//	DevID = _tstoi(argv[1]);

	// Get the driver attributes (Vendor ID, Product ID, Version Number)
	if (!vJoyEnabled())
	{
		//tprintf("Function vJoyEnabled Failed - make sure that vJoy is installed and enabled\n");
		//int dummy = getchar();
			//stat = -2;
		//goto Exit;
		return -1;
	}
	else
	{
		//wprintf(L"Vendor: %s\nProduct :%s\nVersion Number:%hs\n", static_cast<TCHAR *> (GetvJoyManufacturerString()), static_cast<TCHAR *>(GetvJoyProductString()), static_cast<TCHAR *>(GetvJoySerialNumberString()));
		//_tprintf("Function vJoyEnabled Succeeded - vJoy is installed and enabled\n");
	};

	// Get the status of the vJoy device before trying to acquire it
	VjdStat status = GetVJDStatus(DevID);

	switch (status)
	{
	case VJD_STAT_OWN:
		//_tprintf("vJoy device %d is already owned by this feeder\n", DevID);
		break;
	case VJD_STAT_FREE:
		//_tprintf("vJoy device %d is free\n", DevID);
		break;
	case VJD_STAT_BUSY:
		//_tprintf("vJoy device %d is already owned by another feeder\nCannot continue\n", DevID);
		return -3;
	case VJD_STAT_MISS:
		//_tprintf("vJoy device %d is not installed or disabled\nCannot continue\n", DevID);
		return -4;
	default:
		//_tprintf("vJoy device %d general error\nCannot continue\n", DevID);
		return -1;
	};

	// Acquire the vJoy device
	if (!AcquireVJD(DevID))
	{
		//_tprintf("Failed to acquire vJoy device number %d.\n", DevID);
		//int dummy = getchar();
		//stat = -1;
		return -1;
		//goto Exit;
	}
	//else
		//_tprintf("Acquired device number %d - OK\n", DevID);
}

	// Start endless loop
	// The loop injects position data to the vJoy device
	// If it fails it lets the user try again
int update_vJoy(int roll, int pitch, int yaw)
{
		UINT DevID = DEV_ID;
		PVOID pPositionMessage;
		JOYSTICK_POSITION_V2 iReport; // The structure that holds the full position data
		// Set destination vJoy device
		BYTE id = (BYTE)DevID;
		iReport.bDevice = id;

		// Set position data of 3 first axes
		/*if (Z>35000) Z = 0;
		Z += 200;*/
		/*iReport.wAxisZ = roll * 1000;
		iReport.wAxisX = yaw * 1000;
		iReport.wAxisY = pitch * 1000;*/

		long scaled_roll = (long)((roll * 17500 / 35) + 17500);
		long scaled_yaw = (long)((yaw * -17500 / 45) + 17500);
		long scaled_pitch = (long)((pitch * 17500 / 25) + 17500);

		iReport.wAxisZ = scaled_roll;
		iReport.wAxisX = scaled_yaw;
		iReport.wAxisY = scaled_pitch;

		// Set position data of first 8 buttons
		//Btns = 1 << (Z / 4000);
		//iReport.lButtons = Btns;
		//iReport.lButtons = 1;

		if (pitch > 5)
		{
			iReport.lButtons = 1;
		}
		else if (pitch < -5)
		{
			iReport.lButtons = 4;
		}
		else
			iReport.lButtons = 0;

		// Send position data to vJoy device
		pPositionMessage = (PVOID)(&iReport);
		if (!UpdateVJD(DevID, pPositionMessage))
		{
			/*printf("Feeding vJoy device number %d failed - try to enable device then press enter\n", DevID);
			getchar();*/
			AcquireVJD(DevID);
		}
		Sleep(2);
		return 0;
}

//Exit:
//	RelinquishVJD(DevID);
//	return 0;
//}

// Polar values (0x00-0xFF) to Degrees (0-360)
int Polar2Deg(BYTE Polar)
{
	return ((UINT)Polar * 360) / 255;
}

// Convert range 0x00-0xFF to 0%-100%
int Byte2Percent(BYTE InByte)
{
	return ((UINT)InByte * 100) / 255;
}

// Convert One-Byte 2's complement input to integer
int TwosCompByte2Int(BYTE in)
{
	int tmp;
	BYTE inv = ~in;
	BOOL isNeg = in >> 7;
	if (isNeg)
	{
		tmp = (int)(inv);
		tmp = -1 * tmp;
		return tmp;
	}
	else
		return (int)in;
}

