#ifndef ROVERLINK_H
#define ROVERLINK_H

// BYU Mars Rover Communications Protocol Definitions
// Use tabs=4, written in Notepad++

// !!! BE ADVISED !!!
// This file is generated from RoverLink.xml using RoverLinkGenerator.py
// If this file needs to be modified, make the appropriate changes in the XML file.
// The XML file is used to generate the same protocol in the Basestation GUI, 
// thus synchronizing the entire comms system.

// Protocol notes are inserted at the bottom of this document.

// Build Timestemp: 2014-11-21T07:43:02.793000

#include <stdint.h>

// -- Defines
#define ROVERLINK_SYS_HEALTH_MSG_ID	0
#define ROVERLINK_ROVER_STATUS_MSG_ID	1
#define ROVERLINK_ARM_STATUS_MSG_ID	2
#define ROVERLINK_RESERVED_1_MSG_ID	3
#define ROVERLINK_GPS_MSG_ID	4
#define ROVERLINK_IMU_MSG_ID	5
#define ROVERLINK_RESERVED_2_MSG_ID	6
#define ROVERLINK_LRS_MSG_ID	7
#define ROVERLINK_DRIVE_MSG_ID	8
#define ROVERLINK_VIDEO_MSG_ID	9
#define ROVERLINK_ARM_MSG_ID	10
#define ROVERLINK_ISHAAMA_MSG_ID	11
#define ROVERLINK_LIFERAY_MSG_ID	12
#define ROVERLINK_CUSTOM_DEBUG_1_MSG_ID	13
#define ROVERLINK_CUSTOM_DEBUG_2_MSG_ID	14
#define ROVERLINK_CUSTOM_DEBUG_3_MSG_ID	15
#define ROVERLINK_PING_MSG_ID	60
#define ROVERLINK_TELEMETRY_CONFIG_MSG_ID	61
#define ROVERLINK_REQ_FIELDS_ONCE_MSG_ID	62
#define ROVERLINK_BROADCAST_MSG_ID	63


#define ROVERLINK_SYS_HEALTH_MSG_LEN	13
#define ROVERLINK_ROVER_STATUS_MSG_LEN	13
#define ROVERLINK_ARM_STATUS_MSG_LEN	9
#define ROVERLINK_RESERVED_1_MSG_LEN	0
#define ROVERLINK_GPS_MSG_LEN	26
#define ROVERLINK_IMU_MSG_LEN	18
#define ROVERLINK_RESERVED_2_MSG_LEN	0
#define ROVERLINK_LRS_MSG_LEN	16
#define ROVERLINK_DRIVE_MSG_LEN	4
#define ROVERLINK_VIDEO_MSG_LEN	7
#define ROVERLINK_ARM_MSG_LEN	14
#define ROVERLINK_ISHAAMA_MSG_LEN	4
#define ROVERLINK_LIFERAY_MSG_LEN	1
#define ROVERLINK_CUSTOM_DEBUG_1_MSG_LEN	16
#define ROVERLINK_CUSTOM_DEBUG_2_MSG_LEN	16
#define ROVERLINK_CUSTOM_DEBUG_3_MSG_LEN	16
#define ROVERLINK_PING_MSG_LEN	1
#define ROVERLINK_TELEMETRY_CONFIG_MSG_LEN	6
#define ROVERLINK_REQ_FIELDS_ONCE_MSG_LEN	2
#define ROVERLINK_BROADCAST_MSG_LEN	1


// -- Constants


const uint8_t ROVERLINK_MSG_LEN_ARRAY[] = {
	ROVERLINK_SYS_HEALTH_MSG_LEN,
	ROVERLINK_ROVER_STATUS_MSG_LEN,
	ROVERLINK_ARM_STATUS_MSG_LEN,
	ROVERLINK_RESERVED_1_MSG_LEN,
	ROVERLINK_GPS_MSG_LEN,
	ROVERLINK_IMU_MSG_LEN,
	ROVERLINK_RESERVED_2_MSG_LEN,
	ROVERLINK_LRS_MSG_LEN,
	ROVERLINK_DRIVE_MSG_LEN,
	ROVERLINK_VIDEO_MSG_LEN,
	ROVERLINK_ARM_MSG_LEN,
	ROVERLINK_ISHAAMA_MSG_LEN,
	ROVERLINK_LIFERAY_MSG_LEN,
	ROVERLINK_CUSTOM_DEBUG_1_MSG_LEN,
	ROVERLINK_CUSTOM_DEBUG_2_MSG_LEN,
	ROVERLINK_CUSTOM_DEBUG_3_MSG_LEN,
	ROVERLINK_PING_MSG_LEN,
	ROVERLINK_TELEMETRY_CONFIG_MSG_LEN,
	ROVERLINK_REQ_FIELDS_ONCE_MSG_LEN,
	ROVERLINK_BROADCAST_MSG_LEN,
	};
const uint8_t ROVERLINK_WRITE = 0xC0;
const uint8_t ROVERLINK_REQUEST = 0x40;
const uint8_t ROVERLINK_RESPONSE = 0x00;


// -- Typedefs
typedef uint8_t systemState_t;
typedef uint16_t messageMask_t;
typedef uint8_t broadcast_t;


// -- Enum Definitions
// List of aliases. Entries represent the last entry in IP address. (192.168.1.1 for Basestation, 1.10 for Arm, etc)
enum BOARD_ID{
	BASESTATION_1 = 1,
	BASESTATION_2 = 2,
	BASESTATION_3 = 3,
	MAIN_CONTROLLER = 13,
	ARM_CONTROLLER = 10,
};

// systemState field in the heartbeat and status messages that indicates board health
enum SYSTEM_STATE{
	STARTUP = 0,
	NORMAL = 1,
	FAILSAFE = 2,
	ERROR = 3,
};

// fix_type in the GPS message
enum GPS_FIX_TYPE{
	NONE = 0,
	FIX_2D = 1,
	FIX_3D = 2,
	OTHER = 3,
};

// The Telemetry Config and One-Shot Message Mask. Basically, 2^(Message Id) forms the bitmask field.
enum MESSAGE_MASK{
	SYS_HEALTH = 0x0001,
	ROVER_STATUS = 0x0002,
	ARM_STATUS = 0x0004,
	RESERVED_1 = 0x0008,
	GPS = 0x0010,
	IMU = 0x0020,
	RESERVED_2 = 0x0040,
	LRS = 0x0080,
	DRIVE = 0x0100,
	VIDEO = 0x0200,
	ARM = 0x0400,
	ISHAAMA = 0x0800,
	LIFERAY = 0x1000,
	CUSTOM_DEBUG_1 = 0x2000,
	CUSTOM_DEBUG_2 = 0x4000,
	CUSTOM_DEBUG_3 = 0x8000,
};

// Broadcast Message
enum BROADCAST_COMMAND{
	SAVE_FAILSAFE_VALUES_NOW = 0x01,
	PANIC_STOP = 0xF0,
};



// -- Message Type-Definitions
// Each board must provide a status report of its health periodically.
typedef struct __roverlink_sys_health_t {
	uint32_t systemTimestamp;	// Milliseconds from boot
	systemState_t systemState;	// System status code, see enum definition
	uint16_t cpuLoad;	// The CPU Load, 0% means idle, 100% means saturated
	uint16_t busErrors;	// Count of I2C/SPI Bus errors
	uint16_t ethernetErrors;	// Count of Ethernet errors
	uint16_t someOtherMetric;	// Probably packet lag time, etc
} roverlink_sys_health_t;

// The Main Controller Board handles the Camera and Gimbal, Drive and Power systems.
typedef struct __roverlink_rover_status_t {
	systemState_t systemState;	// System status code, see enum definition
	uint16_t roverVoltage;	// Voltage in mV, Ex: 12450 means 12.450 Volts
	int16_t roverCurrent;	// Current in 10mA, Ex: 16500 means 165.00 Amps
	int32_t mAhCounter;	// Battery used in mAh
	uint16_t ubntLinkInteg;	// 100% means perfect link, 0% is lost link, in 0.1% units
	uint16_t dragonLinkRSSI;	// Same as ubntLinkInteg, for backup-link LRS receiver
} roverlink_rover_status_t;

// The Arm Controller Board handles the Arm, including PID controllers. May also handle soil taster "ISHAAMA".
typedef struct __roverlink_arm_status_t {
	uint8_t systemState;	// System status code
	uint16_t dynamixelErrors;	// Number of bad reads or writes to Dynamixel servos.
	int16_t pid1_error;	// Error as in the error term, a scalar value.
	int16_t pid2_error;	// Same as above, for the 2nd PID controller
	int16_t pid3_error;	// Same as above, for the 3rd PID controller
} roverlink_arm_status_t;

// Reserved for future use.
typedef struct __roverlink_reserved_1_t {
} roverlink_reserved_1_t;

// Parsed NMEA sentence data. May disband these fields in favor of echoing entire NMEA sentence.
typedef struct __roverlink_gps_t {
	uint32_t systemTimestamp;	// Milliseconds from boot
	int32_t lat;	// Latitude in 1E-7 units
	int32_t lon;	// Longitude in 1e-7 units
	int32_t alt;	// Altitude in millimeters
	uint16_t hdop;	// Horizontal DOP in centimeters
	uint16_t vdop;	// Vertical DOP in centimeters
	uint16_t vel;	// Velocity in cm/sec
	uint16_t course;	// Course in 0.1 degrees units
	uint8_t fix_type;	// Fix Type, see the enum definition
	uint8_t sats;	// Number of satellites locked on
} roverlink_gps_t;

// The samples from the IMU, probably a MPU-9250. North-East-Down conventions.
typedef struct __roverlink_imu_t {
	int16_t xacc;	// X Acceleration, Front+
	int16_t yacc;	// Y Acceleration, Starboard+
	int16_t zacc;	// Z Acceleration, Down+
	int16_t xgyro;	// X Rotation Rate
	int16_t ygyro;	// Y Rotation Rate
	int16_t zgyro;	// Z Rotation Rate
	int16_t xmag;	// X B-Field Component
	int16_t ymag;	// Y B-Field Component
	int16_t zmag;	// Z B-Field Component
} roverlink_imu_t;

// Reserved for future use.
typedef struct __roverlink_reserved_2_t {
} roverlink_reserved_2_t;

// The DragonLink LRS is the backup R/C receiver on the rover, for if the Rocket M2 modems lose link.
typedef struct __roverlink_lrs_t {
	uint16_t ppmCh1;	// Channel 1, usually Roll (Rover Left-Right)
	uint16_t ppmCh2;	// Channel 2, usually Pitch (Rover Foward-Back)
	uint16_t ppmCh3;	// Channel 3, throttle stick (Gimbal Tilt)
	uint16_t ppmCh4;	// Channel 4, rudder (Gimbal Pan)
	uint16_t ppmCh5;	// Channel 5
	uint16_t ppmCh6;	// Channel 6
	uint16_t ppmCh7;	// Channel 7, Backup-Link Asserts control if >1800us, doubles as Panic Stop
	uint16_t ppmCh8;	// Channel 8, DragonLink in Failsafe Mode if >1800us
} roverlink_lrs_t;

// Drive commands for rover movement.
typedef struct __roverlink_drive_t {
	int16_t driveFwd;	// Forward Drive Component
	int16_t driveTurn;	// Turning Drive Component
} roverlink_drive_t;

// Camera selection and zoom, Gimbal Pan and Tilt.
typedef struct __roverlink_video_t {
	int16_t gimbalPan;	// Gimbal Pan Value
	int16_t gimbalTilt;	// Gimbal Tilt Value
	uint16_t gimbalZoom;	// Gimbal Zoom Value
	int8_t camSelect;	// Camera Mux Selector
} roverlink_video_t;

// Arm/joint positions and Drill and Claw setpoints.
typedef struct __roverlink_arm_t {
	int16_t baseAzimuth;	// Also known as turret, pans entire arm
	int16_t shoulder;	// Shoulder: first pitch joint
	int16_t elbow;	// Elbow: second pitch joint
	int16_t wristTilt;	// Wrist flap motion, final pitch joint
	int16_t wristRotate;	// Wrist roll motion
	int16_t effectorA;	// Open/Close Claw
	int16_t effectorB;	// Roller or Drill, configuration-dependant
} roverlink_arm_t;

// The soil taste tester. Likely is Read-Only, nothing to write to. Assuming 12-bit ADC
typedef struct __roverlink_ishaama_t {
	uint16_t hygrometer;	// The Soil Hygrometer probe
	uint16_t phMeter;	// The Soil ph Meter probe
} roverlink_ishaama_t;

// Big scary laser.
typedef struct __roverlink_liferay_t {
	uint8_t laserDutyCycle;	// Set the laser's intensity
} roverlink_liferay_t;

// All the custom fields can be anything, but the payload size (16 bytes) is constant.
typedef struct __roverlink_custom_debug_1_t {
	uint16_t custom00;	// Dynamixel Servo 1 Stress/Current Value
	uint16_t custom01;	// Dynamixel Servo 2 Stress/Current Value
	uint16_t custom02;	// Dynamixel Servo 3 Stress/Current Value
	uint16_t custom03;	// Dynamixel Servo 4 Stress/Current Value
	uint16_t custom04;	// Description
	uint16_t custom05;	// Description
	uint16_t custom06;	// Description
	uint16_t custom07;	// Description
} roverlink_custom_debug_1_t;

// All the custom fields can be anything, but the payload size (16 bytes) is constant.
typedef struct __roverlink_custom_debug_2_t {
	uint16_t custom20;	// Description
	uint16_t custom21;	// Description
	uint16_t custom22;	// Description
	uint16_t custom23;	// Description
	uint16_t custom24;	// Description
	uint16_t custom25;	// Description
	uint16_t custom26;	// Description
	uint16_t custom27;	// Description
} roverlink_custom_debug_2_t;

// All the custom fields can be anything, but the payload size (16 bytes) is constant.
typedef struct __roverlink_custom_debug_3_t {
	uint16_t custom30;	// Description
	uint16_t custom31;	// Description
	uint16_t custom32;	// Description
	uint16_t custom33;	// Description
	uint16_t custom34;	// Description
	uint16_t custom35;	// Description
	uint16_t custom36;	// Description
	uint16_t custom37;	// Description
} roverlink_custom_debug_3_t;

// Used to verify that the entire UDP comm system is functional. Payload proves this if uniquely generated at basestation and compared to response.
typedef struct __roverlink_ping_t {
	uint8_t payload[8];	// Description
} roverlink_ping_t;

// The 16 rover messages can be assigned to send cyclically by setting bits in these bitmasks.
typedef struct __roverlink_telemetry_config_t {
	messageMask_t telemetryMask_10HZ;	// Fast Telemetry Loop
	messageMask_t telemetryMask_1HZ;	// Intermediate Loop
	messageMask_t telemetryMask_10SEC;	// Slow Loop
} roverlink_telemetry_config_t;

// Just one time instead of configuring continuous telemetry. Probably superfiluous if Read/Write mechanism of this protocol can stimulate a single register.
typedef struct __roverlink_req_fields_once_t {
	messageMask_t oneShotMask;	// Request a set of messages only one time, not set for cyclic
} roverlink_req_fields_once_t;

// Broadcast a message to all controller boards at once. Used for universal commands like Panic Stop, etc
typedef struct __roverlink_broadcast_t {
	broadcast_t broadcastCommand;	// See broadcast_t description
} roverlink_broadcast_t;



// -- Message Data Structure Instances
static roverlink_sys_health_t sys_health;
static roverlink_rover_status_t rover_status;
static roverlink_arm_status_t arm_status;
static roverlink_gps_t gps;
static roverlink_imu_t imu;
static roverlink_lrs_t lrs;
static roverlink_drive_t drive;
static roverlink_video_t video;
static roverlink_arm_t arm;
static roverlink_ishaama_t ishaama;
static roverlink_liferay_t liferay;
static roverlink_custom_debug_1_t custom_debug_1;
static roverlink_custom_debug_2_t custom_debug_2;
static roverlink_custom_debug_3_t custom_debug_3;
static roverlink_ping_t ping;
static roverlink_telemetry_config_t telemetry_config;
static roverlink_req_fields_once_t req_fields_once;
static roverlink_broadcast_t broadcast;
#endif // ROVERLINK_H 


/* Packet Structure
The structure following is a description of an entire RoverLink packet as sent via UDP:
<roverlink_frame>
	<src_board_id 	bits="4">Source/Originating device. 0x00 for Basestation, see BOARD_ID enum for others</src_board_id>
	<dst_board_id 	bits="4">Intended Recipient device. 0x01 for Main Controller, see BOARD_ID enum for others</dst_board_id>
	<write_bit		bits="1">The address byte's MSB is 1 for setpoint write packets, 0 for all else</write_bit>
	<request_bit	bits="1">This bit is set to 1 when requesting the contents of a message or writing, 0 otherwise</request_bit>
	<message_id 	bits="6">ROVER_STATUS response would give 0x01 for this field</message_id>
	<sequence_cnt 	bits="8">Packet unique number, rolls over at 255</sequence_cnt>
	<payload_len	bits="8">Payload size</payload_len>
	<payload bits=8*payload_len>This is the message struct: All fields sent in sequence, MSB first (Big-endian)</payload>
</roverlink_frame>
*/

/* Message IDs Table
The following is a synopsis of the possible packet IDs ([write_bit][request_bit][message_id]):
Note that we ommit 0b10xxxxxx as a possible pattern; this is invalid / reserved for later use.
                            WRITE       REQUEST     RESPONSE
                            0b11xxxxxx  0b01xxxxxx  0b00xxxxxx
HEALTH_HEARTBEAT    0                   64          0
ROVER_STATUS        1                   65          1
ARM_STATUS          2                   66          2
RESERVED_1          3                   67          3
GPS_POSITION        4                   68          4
IMU_SAMPLE          5                   69          5
RESERVED_2          6                   70          6
DRAGONLINK_FRAME    7                   71          7
DRIVE_SETPOINTS     8       200         72          8
VIDEO_SETPOINTS     9       201         73          9
ARM_SETPOINTS      10       202         74          10
ISHAAMA            11       203         75          11
LIFERAY            12       204         76          12
CUSTOM_DEBUG_1     13       205         77          13
CUSTOM_DEBUG_2     14       206         78          14
CUSTOM_DEBUG_3     15       207         79          15

PING               60       252         124         60
TELEMETRY_CONFIG   61       253         125         61
REQ_FIELDS_ONCE    62       254        
BROADCAST          63       255        

Address Bit Descriptions:
WRITE
		Whatever values are in the payload, put them into the relevant struct. 
		Example: Drive command packet sent from the basestation
REQUEST
		Ask for the contents of the struct that that message corresponds to. 
		Example: Basestation wants to know the current values, asks using this packet.
RESPONSE
		The response to a previous Request packet, -or- is a periodic telemetry update.
		Example 1: As asked by the basestation for the current Arm positions, this is our response.
		Example 2: Just got a GPS sentence, send it to the basestation with this packet.
*/

/* Packet Examples
Basestation write Drive Setpoints to Main Controller
    0000   |Src MAC Address| |Dst MAC Address| Type  VL DS
    0010   Lngth Ident Fg Of TT Pr chksm SrcIpAddres DstIp
    0020   Addrs SrcPr DstPr Lngth chksm 01 c8 00 04 03 e8
    0030   00 00 

    01 C8 00 04 03 E8 00 00    Description
    
    0                          Source is Base Station (0x00)
     1                         Destination is Main Controller Board (0x01)
       C8                      0b11001000: Write (MSB=1), 1000 = 0x08 = DRIVE_SETPOINTS Message
          00                   uint8_t 0: 1st Packet sent from Basestation (or is it called zeroth?)
             04                uint8_t 4: 4 bytes remaining after this byte
                03 E8          int16_t 1000: Drive full forward +1000
                      00 00    int16_t 0: 0 Turn component

Basestation requests Arm Setpoints	
    02 4A 01 00
    Src: Basestation, Dst: Arm Board
       Read ARM_SETPOINTS
          2nd Packet sent by Basestation
             No Payload, this should be the last byte of the packet

Arm Board responds with the current arm setpoints
    20 0A 00 0E 0000 0000 0000 0000 0000 0000 0000
    Src: Arm Board, Dst: Basestation
       ARM_SETPOINTS Report, bits 7 and 6 clears, representing this is a response packet.
          1st Packet sent by the Arm board (Sequence starts at 0)
             14 Bytes in payload
                All zeros, initial values for rebooted arm board.

Basestation write new values to the Arm Board
    02 CA 02 0E 0000 00FA 02BC 00FA 0000 0000 0000
    Src: Basestation, Dst: Arm Board
       Message: Write ARM_SETPOINTS
          3rd Packet sent by Basestation (0,1,2; 0x02 represends 2, but there have been 3 packets)
             14 Bytes in the payload
                0->baseAzimuth
                     250->shoulder
                          700->elbow
                               250->wristTilt
                                    0->wristRotate
                                         etc

Basestation requests Arm Setpoints
    02 4A 03 00
    Src: Basestation, Dst: Arm Board
       Request ARM_SETPOINTS, bit 6 is set indicating request
          4th Packet sent by Basestation
             No Payload; even if there are more bytes after this, they should be ignored

Arm Board responds with the current arm setpoints
    20 0A 01 0E 0000 00FA 02BC 00FA 0000 0000 0000
    Src: Arm Board, Dst: Basestation
       ARM_SETPOINTS Report, bits 7 and 6 cleared
          Seq count is 0x01, this is the 2nd packet originating from the Arm Board
             14 Bytes in payload
                Bytes match the write made earlier.

Basestation asks for a set of messages to be transmitted back once
    02 FD 04 02 0500
    Basestation to Arm Board
       Message #253 with WRITE and REQUEST bits active (signifing a write)
          5th Message transmitted by Basestation
             2-byte payload
                Bitmask with 1's on the 8th and 10th digits, corresponding to messages #8 (Drive) and #10 (Arm).
*/

