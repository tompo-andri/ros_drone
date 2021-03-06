#ifndef PIKOPTER_NAVDATA_H
#define PIKOPTER_NAVDATA_H


/* ################################### INCLUDES ################################### */
// Pikotper common includes
#include "pikopter_common.h"

// Mavros structures includes for the subscribers
#include "std_msgs/Float64.h"
#include "mavros_msgs/BatteryStatus.h"
#include "geometry_msgs/TwistStamped.h"
#include "mavros_msgs/ExtendedState.h"
#include "mavros_msgs/State.h"
#include "geometry_msgs/PoseStamped.h"

// Mavros structures includes for the services used
#include "mavros_msgs/StreamRate.h"




/* ################################### CONSTANTS ################################### */

/* ##### Specific to the navdatas (got from the given codes) ##### */
// The port used for the navdatas
#define PORT_NAVDATA 5554

// A tag to say if it's a demo or not
#define TAG_DEMO 0

// Tag for the checksum packet in full mode
#define TAG_CKS 0
#define NAVDATA_NREADS_INT 4
#define NAVDATA_NREADS_FLOAT 6

// Types definition
#define float32_t float
#define float64_t double

// 2 spaces
#define DEMO_LEN NAVDATA_NREADS_INT*(INT_LEN+1)+(NAVDATA_NREADS_FLOAT*FLOAT_LEN+1)+2


/* ##### Specific to navdata ros parameters ##### */
// For the stream rate requests
#define SR_REQUEST_ON (uint8_t)1
#define SR_REQUEST_EXTENDED_STATE_RATE (uint16_t)1
#define SR_REQUEST_POSITION_RATE (uint16_t)200

// Loop rate in hertz
// For ArDrone, in demo mode it's 15Hz and in normal mode it's 200Hz
#define NAVDATA_DEMO_LOOP_RATE 15
#define NAVDATA_LOOP_RATE 200
#define NAVDATA_DISPLAY_RATE 1000

// Subscribers' buffer size
#define SUB_BUF_SIZE_GLOBAL_POS_REL_ALT 10
#define SUB_BUF_SIZE_BATTERY 10
#define SUB_BUF_SIZE_LOCAL_POS_GP_VEL 10
#define SUB_BUF_SIZE_LOCAL_POS_POSE 10
#define SUB_BUF_SIZE_EXTENDED_STATE 10
#define SUB_BUF_SIZE_STATE 10
#define SUB_BUF_SIZE_CMD_RECEIVED 100


/* ##### Specific to navdata (new constants) ##### */
// The value of the battery percentage
#define BATTERY_PERCENTAGE 100  // To put into percentage
#define CRITICAL_BATTERY_LIMIT 10  // 10%

// Default values of the navdata demo buffer
#define DEFAULT_NAVDATA_DEMO_SEQUENCE 0
#define DEFAULT_NAVDATA_DEMO_HEADER 88776655
#define DEFAULT_NAVDATA_DEMO_VBAT_FLYING_PERCENTAGE 100
#define DEFAULT_NAVDATA_DEMO_ALTITUDE 0
#define DEFAULT_NAVDATA_DEMO_THETA 0
#define DEFAULT_NAVDATA_DEMO_PHI 0
#define DEFAULT_NAVDATA_DEMO_PSI 0
#define DEFAULT_NAVDATA_DEMO_VX 0
#define DEFAULT_NAVDATA_DEMO_VY 0
#define DEFAULT_NAVDATA_DEMO_VZ 0
#define DEFAULT_NAVDATA_DEMO_VISION false
#define DEFAULT_NAVDATA_DEMO_ARDRONE_STATE 0x400  // 1024 because only the 11th bit is at 1
#define DEFAULT_NAVDATA_ARDRONE_STATE 0  // All the bits to 0




/* ################################### TYPE DEF ################################### */

/* ########## State values ########## */
// Main value in the first 16 bits
typedef enum {
	DEFAULT,
	INIT,
	LANDED,
	FLY,
	HOVER,
	CTRL_USELESS_1,
	CTRL_USELESS_2,
	TAKEOFF,
	MOVE,
	LAND,
	LOOP
} ctrl_states;

// Specific value in the last 16 bits
typedef enum {
	FLY_OK,
	FLY_LOST_ALT,
	FLY_LOST_ALT_GO_DOWN,
	FLY_ALT_OUT_ZONE,
	COMBINED_YAW,
	BRAKE,
	NO_VISION
} fly_states;

// Hover states
typedef enum {
	HOVER_OK,
	HOVER_YAW,
	HOVER_YAW_LOST_ALT,
	HOVER_YAW_LOST_ALT_GO_DOWN,
	HOVER_ALT_OUT_ZONE,
	HOVER_YAW_ALT_OUT_ZONE,
	HOVER_LOST_ALT,
	HOVER_LOST_ALT_GO_DOWN,
	LOST_COM,
	LOST_COM_LOST_ALT,
	LOST_COM_LOST_ALT_TOO_LONG,
	LOST_COM_ALT_OK,
	MAGNETO_CALIB,
	DEMO
} hover_states;

// Takeoff states
typedef enum {
	TAKEOFF_GROUND,
	TAKEOFF_AUTO
} takeoff_states;

// Move states
typedef enum {
	GOTO_OK = 0,
	GOTO_LOST_ALT,
	GOTO_YAW
} move_states;

// Land states
typedef enum {
	CLOSED_LOOP,
	OPEN_LOOP,
	OPEN_LOOP_FAST
} land_states;

// Loop states
typedef enum {
	IMPULSION,
	OPEN_LOOP_CTRL,
	PLANIF_CTRL
} loop_states;


/* ########## Navdata structures ########## */
// Some options for the navdata paquets
struct navdata_option {
	uint16_t  tag;  // Type of the packet: DEMO, etc.
	uint16_t  size;  // Size of this struct (options fields can have different sizes)
	uint8_t   data[];  // The real datas received
};

// The navdata paquet itself
struct navdata {
	uint32_t    header;  // Always 88776655
	uint32_t    ardrone_state;  // Bit mask defined in SDK config.h
	uint32_t    sequence;  // Sequence number of the packet
	bool		vision_defined;  // True: vision computed by ardrone onboard chip
	struct navdata_option options[1];  // Static pointer to generic options
};

// Navdata paquet for demo
struct navdata_demo {
	uint32_t   header;  // Always 88776655
	uint32_t   ardrone_state;  // Bit mask defined in SDK config.h
	uint32_t   sequence;  // Sequence number of the packet
	bool	   vision_defined;  // True: vision computed by ardrone onboard chip

	// Here are the specific values for demo mode
	uint16_t   tag;  // Type of the packet: must be TAG_DEMO
	uint16_t   size;  // Size of the packet in bytes
	uint32_t   ctrl_state;  // Flying state (landed, flying, hovering, etc.)
	uint32_t   vbat_flying_percentage;  // Battery in remaining percentage

	// UAV (Unmanned Aerial Vehicle ~ drone) state
	float32_t  theta;  // UAV's pitch in milli-degrees
	float32_t  phi;  // UAV's roll in milli-degrees
	float32_t  psi;  // UAV's yaw in milli-degrees
	int32_t    altitude;  // UAV's altitude in millimeters
	float32_t  vx;  // UAV's estimated linear velocity in millimeters/s
	float32_t  vy;  // UAV's estimated linear velocity in millimeters/s
	float32_t  vz;  // UAV's estimated linear velocity in millimeters/s

	// Deprecated on ARdrone2.0
	uint32_t   num_frames;
	float32_t  detection_camera_rot[9];
	float32_t  detection_camera_trans[3];
	uint32_t   detection_tag_index;
	uint32_t   detection_camera_type;  // NOT DEPRECATED, Type of tag searched in detection
	float32_t  drone_camera_rot[9];
	float32_t  drone_camera_trans[3];
};

// The navdata structure, contains the basic struct and the demo one
union navdata_t {
	struct navdata raw;
	struct navdata_demo demo;
};



/* ################################### Classes ################################### */
/*!
 * \brief Pikopter navdata ros node
 */
class PikopterNavdata {

	// Public part
	public:

		// Public functions
		PikopterNavdata(char *ip_adress, bool in_demo);  // Constructor
		~PikopterNavdata();  // Destructor
		void sendNavdata();  // Send the navdata
		void display();  // Display the current method of the navdata
		void setBitEndOfBootstrap();

		// Handlers
		void getAltitude(const std_msgs::Float64::ConstPtr& msg);
		void handleBattery(const mavros_msgs::BatteryStatus::ConstPtr& msg);
		void handleVelocity(const geometry_msgs::TwistStamped::ConstPtr& msg);
		void getExtendedState(const mavros_msgs::ExtendedState::ConstPtr& msg);
		void getState(const mavros_msgs::State::ConstPtr& msg);
		void handleOrientation(const geometry_msgs::PoseStamped::ConstPtr& msg);
		void handleCmdReceived(const std_msgs::Bool status);

		// Accessors
		bool inDemoMode();

	// Private part
	private:

		// Private functions
		void initNavdata();
		void askMavrosRate();
		void incrementSequenceNumber();

		// Private attributes
		struct sockaddr_in addr_drone_navdata;
		union navdata_t navdata_current;
		int navdata_fd;
		bool demo_mode;
		std::mutex navdata_mutex;
};

#endif
