#ifndef PIKOPTER_COMMON_H
#define PIKOPTER_COMMON_H


/* ################################### INCLUDES ################################### */
// System librairies used by pikopter
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#include <limits.h>
#include <pthread.h>
#include <sys/uio.h>
#include <netinet/in.h>
#include <netdb.h>

// Ros librairies in order to use them
#include <ros/ros.h>
#include <ros/console.h>



/* ################################### CONSTANTS ################################### */
// Size of the packets (navdata or command)
#define PACKET_SIZE 256

// Errors result got if error during socket manipulation
#define SOCKET_ERROR -1
#define SOCKET_ERRNO errno

// DUNO
#define MSG_NOBLOCK (1 << 0)

// DUNO
#define CHUCK_BUFFER_SIZE 14000

// Maximum packets exchange (hours of flying)
#define MAX_CMD_NAVDATA 10000000

// Max size of an IP adress OR a domain name to get it
#define MAX_DOMAINE_NAME_SIZE 256



/* ################################### Classes ################################### */
/*!
 * \brief Pikopter network utilities
 */
class PikopterNetwork {

	// Public methods
	public:
		static int open_udp_socket(int portnum, struct sockaddr_in *serv_addr, char *station_ip);
};

#endif