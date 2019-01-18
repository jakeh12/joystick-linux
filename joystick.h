#ifndef JOYSTICK_H_
#define JOYSTICK_H_

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <linux/joystick.h>

extern int errno;

/* example of usage, for help see https://www.kernel.org/doc/Documentation/input/joystick-api.txt

#include <stdio.h>
#include "joystick.h"


int main(void) {
	struct joystick* js = joystick_init("/dev/input/js0", JS_NONBLOCKING);
	if (!js)
	{
		printf("Aborting because joystick did not initialize correctly...\n");
		return 1;
	}

	joystick_info(js);

	while(1)
	{
		if (!joystick_read(js))
			{
				printf("Aborting because joystick could not be read...\n");
				return 1;
			}
		printf("%1.2f\t"
				"%1.2f\t"
				"%1.2f\t"
				"%1.2f\t"
				"%1.2f\t"
				"%1.2f\n",
				joystick_get_throttle(js),
				joystick_get_yaw(js),
				joystick_get_pitch(js),
				joystick_get_roll(js),
				joystick_get_left_switch(js),
				joystick_get_right_switch(js));
	}

	joystick_destroy(js);
	return 0;
}
*/


#define JS_NONBLOCKING		0
#define JS_BLOCKING			1

/* setup and calibration for GREAT PLANES InterLink Elite */
#define JS_LEFT_SWITCH		0
#define JS_RIGHT_SWITCH		1
#define JS_THROTTLE_AXIS	2
#define JS_THROTTLE_MIN		21620
#define JS_THROTTLE_MID		0
#define JS_THROTTLE_MAX		-22296
#define JS_YAW_AXIS			4
#define JS_YAW_MIN			-20607
#define	JS_YAW_MID			0
#define JS_YAW_MAX			25336
#define JS_PITCH_AXIS		1
#define	JS_PITCH_MIN		21957
#define JS_PITCH_MID		0
#define JS_PITCH_MAX		-19594
#define JS_ROLL_AXIS		0
#define JS_ROLL_MIN		-20945
#define JS_ROLL_MID		0
#define JS_ROLL_MAX		25336


struct joystick
{
	int fd;
	struct js_event e;
	float axis[4];
	float button[2];
};


float map_(float in, float in_min, float in_max, float out_min, float out_max);
float clip_(float in, float min, float max);


struct joystick* joystick_init(const char* path, int blocking);
int joystick_read(struct joystick* js);
float joystick_get_throttle(struct joystick* js);
float joystick_get_yaw(struct joystick* js);
float joystick_get_pitch(struct joystick* js);
float joystick_get_roll(struct joystick* js);
float joystick_get_left_switch(struct joystick* js);
float joystick_get_right_switch(struct joystick* js);
void joystick_info(struct joystick* js);
void joystick_destroy(struct joystick* js);


#endif
