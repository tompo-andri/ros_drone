#ifndef ROS_COORDS_H
#define ROS_COORDS_H

#include "ros_com_channel.h"
#include "ros_com_master.h"
#include "ros_utils.h"

#define COORDS_NREADS 10
#define COORDS_TIMEOUT 2000
#define COORDS_INTERVAL 	1 // interval in seconds
/*size of float digits plus a space and \0*/
#define COORDS_BUF_SIZE (FLOAT_LEN + 1)*COORDS_NREADS + 1
#define COORDS_FILENAME "/tmp/jakopter_coords.txt"

int jakopter_init_coords();
int jakopter_stop_coords();

const char* jakopter_log_coords();

#endif