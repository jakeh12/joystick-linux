#include "joystick.h"


float map_(float in, float in_min, float in_max, float out_min, float out_max)
{
	return (in - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}


float clip_(float in, float min, float max)
{
	if (in > max)
		return max;
	if (in < min)
		return min;
	return in;
}


struct joystick* joystick_init(const char* path, int blocking)
{
	struct joystick* js =  malloc(sizeof(struct joystick));
	if (!js)
	{
		return js;
	}
	int options = O_RDONLY | O_NONBLOCK;
	if (blocking)
		options = O_RDONLY;
	js->fd = open(path, options);
	if (js->fd == -1)
	{
		char err[64];
		sprintf(err, "Failed to initialize joystick \"%s\"", path);
		perror(err);
		free(js);
		js = NULL;
	}
	return js;
}


int joystick_read(struct joystick* js)
{
	int status = read(js->fd, &(js->e), sizeof(js->e));
	if (status == -1 && errno != EAGAIN)
	{
		perror("Failed to read joystick");
		free(js);
		return 0;
	}

	int val = js->e.value;

	if (js->e.type == JS_EVENT_AXIS || (js->e.type == (JS_EVENT_AXIS | JS_EVENT_INIT)))
	{
		float norm = 0.0;

		if (js->e.number == JS_THROTTLE_AXIS)
		{
			if (val < JS_THROTTLE_MID)
				norm = map_(val, JS_THROTTLE_MIN, JS_THROTTLE_MID, 0.0f, 0.5f);
			else
				norm = map_(val, JS_THROTTLE_MID, JS_THROTTLE_MAX, 0.5f, 1.0f);
			js->axis[0] = clip_(norm, 0.0f, 1.0f);
		}
		else if (js->e.number == JS_YAW_AXIS)
		{
			if (val < JS_YAW_MID)
				norm = map_(val, JS_YAW_MIN, JS_YAW_MID, 0.0f, 0.5f);
			else
				norm = map_(val, JS_YAW_MID, JS_YAW_MAX, 0.5f, 1.0f);
			js->axis[1] = clip_(norm, 0.0f, 1.0f);
		}
		else if (js->e.number == JS_PITCH_AXIS)
		{
			if (val < JS_PITCH_MID)
				norm = map_(val, JS_PITCH_MIN, JS_PITCH_MID, 0.0f, 0.5f);
			else
				norm = map_(val, JS_PITCH_MID, JS_PITCH_MAX, 0.5f, 1.0f);
			js->axis[2] = clip_(norm, 0.0f, 1.0f);
		}
		else if (js->e.number == JS_ROLL_AXIS)
		{
			if (val < JS_ROLL_MID)
				norm = map_(val, JS_ROLL_MIN, JS_ROLL_MID, 0.0f, 0.5f);
			else
				norm = map_(val, JS_ROLL_MID, JS_ROLL_MAX, 0.5f, 1.0f);
			js->axis[3] = clip_(norm, 0.0f, 1.0f);
		}
	}
	else if (js->e.type == JS_EVENT_BUTTON || (js->e.type == (JS_EVENT_BUTTON | JS_EVENT_INIT)))
	{
		if (js->e.number == JS_LEFT_SWITCH)
			js->button[0] = val;
		else if (js->e.number == JS_RIGHT_SWITCH)
			js->button[1] = val;
	}
	return 1;
}


float joystick_get_throttle(struct joystick* js)
{
	return js->axis[0];
}


float joystick_get_yaw(struct joystick* js)
{
	return js->axis[1];
}


float joystick_get_pitch(struct joystick* js)
{
	return js->axis[2];
}


float joystick_get_roll(struct joystick* js)
{
	return js->axis[3];
}


float joystick_get_left_switch(struct joystick* js)
{
	return js->button[0];
}


float joystick_get_right_switch(struct joystick* js)
{
	return js->button[1];
}


void joystick_info(struct joystick* js)
{
	  int version, axes, buttons;
	  version = axes = buttons = 0;
	  char name[128];
	  ioctl(js->fd, JSIOCGAXES, &axes);
	  ioctl(js->fd, JSIOCGBUTTONS, &buttons);
	  ioctl(js->fd, JSIOCGVERSION, &version);
	  ioctl(js->fd, JSIOCGNAME(sizeof(name)), &name);
	  printf("name: %s\n"
			  "version: %d\n"
			  "axes: %d\n"
			  "buttons: %d\n",
			  name,
			  version,
			  axes,
			  buttons);
}


void joystick_destroy(struct joystick* js)
{
	free(js);
}
