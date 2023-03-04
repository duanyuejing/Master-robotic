#include <stdio.h>
#include <time.h>
#include "dhdc.h"
#include "drdc.h"

int main(int argc, char** argv)
{
	int i, j, n;
	int done = 0;
	double r[3][3];
	int enc[DHD_MAX_DOF];
	double p[3];
	int encCount;

	// message
	printf("force dimension - encoder reading example %s\n", dhdGetSDKVersionStr());

	// open the first available device
	if (dhdOpen() < 0)
	{
		printf("error: cannot open device (%s)\n", dhdErrorGetLastStr());
		dhdSleep(2.0);
		return -1;
	}

	// identify device
	printf("%s device detected\n\n", dhdGetSystemName());

	// identify number of encoders to report based on device type
	switch (dhdGetSystemType())
	{
	case DHD_DEVICE_DELTA3:
	case DHD_DEVICE_OMEGA3:
	case DHD_DEVICE_FALCON:
		encCount = 3;
		break;
	case DHD_DEVICE_OMEGA33:
	case DHD_DEVICE_OMEGA33_LEFT:
		encCount = 6;
		break;
	case DHD_DEVICE_OMEGA331:
	case DHD_DEVICE_OMEGA331_LEFT:
		encCount = 7;
		break;
	case DHD_DEVICE_CONTROLLER:
	case DHD_DEVICE_CONTROLLER_HR:
	default:
		encCount = 8;
		break;
	}

	// display instructions
	printf("press 'q' to quit\n\n");
	printf("encoder values:\n");

	// configure device
	dhdEnableExpertMode();
	// haptic loop
	while (!done)
	{
		// get encoder values
		// apply zero force for convenience
		dhdSetForceAndTorqueAndGripperForce(0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);

		dhdGetPosition(&p[0], &p[1], &p[2]);
		for (j = 0; j <= 2; j++)
		{
			printf("{x:%.4f,y:%.4f,z:%.4f}\n", p[0], p[1], p[2]);
		}
		// check for exit condition
		if (dhdKbHit() && dhdKbGet() == 'q')
			done = 1;

	}
	// close the connection
	// happily exit
	printf("\ndone.\n");
	return 0;
}

