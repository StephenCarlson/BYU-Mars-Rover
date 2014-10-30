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

// Build Timestemp: 2014-10-30T18:14:05.123000

// -- Constants
constant uint8_t WRITE_BIT = 0x80;
constant uint8_t REQUEST_BIT = 0x40;


// -- Typedefs
typedef uint8_t systemState_t;
typedef uint16_t messageMask_t;
typedef uint8_t broadcast_t;


// -- Enum Definitions
enum BOARD_ID{
	BASESTATION = 0,
	MAIN_CONTROLLER = 1,
	ARM_CONTROLLER = 2,
	BASESTATION_2 = 8,
};

enum SYSTEM_STATE{
	STARTUP = 0,
	NORMAL = 1,
	FAILSAFE = 2,
	ERROR = 3,
};

enum GPS_FIX_TYPE{
	NONE = 0,
	FIX_2D = 1,
	FIX_3D = 2,
	OTHER = 3,
};

enum MESSAGE_MASK{
	HEALTH_HEARTBEAT = 0x0001,
	ROVER_STATUS = 0x0002,
	ARM_STATUS = 0x0004,
	RESERVED_1 = 0x0008,
	GPS_POSITION = 0x0010,
	IMU_SAMPLE = 0x0020,
	RESERVED_2 = 0x0040,
	DRAGONLINK_FRAME = 0x0080,
	DRIVE_SETPOINTS = 0x0100,
	VIDEO_SETPOINTS = 0x0200,
	ARM_SETPOINTS = 0x0400,
	ISHAAMA = 0x0800,
	LIFERAY = 0x1000,
	CUSTOM_DEBUG_1 = 0x2000,
	CUSTOM_DEBUG_2 = 0x4000,
	CUSTOM_DEBUG_3 = 0x8000,
};

enum BROADCAST_COMMAND{
	SAVE_FAILSAFE_VALUES_NOW = 0x01,
	PANIC_STOP = 0xF0,
};



// -- Message Definitions
typedef struct __roverlink_health_heartbeat_t {
	uint32_t systemTimestamp,
	systemState_t systemState,
	uint16_t cpuLoad,
	uint16_t busErrors,
	uint16_t ethernetErrors,
	uint16_t someOtherMetric,
} roverlink_health_heartbeat_t;

typedef struct __roverlink_rover_status_t {
	systemState_t systemState,
	uint16_t roverVoltage,
	int16_t roverCurrent,
	int32_t mAhCounter,
	uint16_t ubntLinkInteg,
	uint16_t dragonLinkRSSI,
} roverlink_rover_status_t;

typedef struct __roverlink_arm_status_t {
	uint8_t systemState,
	uint16_t dynamixelErrors,
	int16_t pid1_error,
	int16_t pid2_error,
	int16_t pid3_error,
} roverlink_arm_status_t;

typedef struct __roverlink_reserved_1_t {
} roverlink_reserved_1_t;

typedef struct __roverlink_gps_position_t {
	int32_t lat,
	int32_t lon,
	int32_t alt,
	uint16_t hdop,
	uint16_t vdop,
	uint16_t vel,
	uint16_t course,
	uint8_t fix_type,
	uint8_t sats,
} roverlink_gps_position_t;

typedef struct __roverlink_imu_sample_t {
	int16_t xacc,
	int16_t yacc,
	int16_t zacc,
	int16_t xgyro,
	int16_t ygyro,
	int16_t zgyro,
	int16_t xmag,
	int16_t ymag,
	int16_t zmag,
} roverlink_imu_sample_t;

typedef struct __roverlink_reserved_2_t {
} roverlink_reserved_2_t;

typedef struct __roverlink_dragonlink_frame_t {
	uint16_t ppmCh1,
	uint16_t ppmCh2,
	uint16_t ppmCh3,
	uint16_t ppmCh4,
	uint16_t ppmCh5,
	uint16_t ppmCh6,
	uint16_t ppmCh7,
	uint16_t ppmCh8,
} roverlink_dragonlink_frame_t;

typedef struct __roverlink_drive_setpoints_t {
	int16_t driveFwd,
	int16_t driveTurn,
} roverlink_drive_setpoints_t;

typedef struct __roverlink_video_setpoints_t {
	int16_t gimbalPan,
	int16_t gimbalTilt,
	uint16_t gimbalZoom,
	int8_t camSelect,
} roverlink_video_setpoints_t;

typedef struct __roverlink_arm_setpoints_t {
	int16_t baseAzimuth,
	int16_t shoulder,
	int16_t elbow,
	int16_t wristTilt,
	int16_t wristRotate,
	int16_t effectorA,
	int16_t effectorB,
} roverlink_arm_setpoints_t;

typedef struct __roverlink_ishaama_t {
	uint16_t hygrometer,
	uint16_t phMeter,
} roverlink_ishaama_t;

typedef struct __roverlink_liferay_t {
	uint8_t laserDutyCycle,
} roverlink_liferay_t;

typedef struct __roverlink_custom_debug_1_t {
	uint16_t custom00,
	uint16_t custom01,
	uint16_t custom02,
	uint16_t custom03,
	uint16_t custom04,
	uint16_t custom05,
	uint16_t custom06,
	uint16_t custom07,
} roverlink_custom_debug_1_t;

typedef struct __roverlink_custom_debug_2_t {
	uint16_t custom20,
	uint16_t custom21,
	uint16_t custom22,
	uint16_t custom23,
	uint16_t custom24,
	uint16_t custom25,
	uint16_t custom26,
	uint16_t custom27,
} roverlink_custom_debug_2_t;

typedef struct __roverlink_custom_debug_3_t {
	uint16_t custom30,
	uint16_t custom31,
	uint16_t custom32,
	uint16_t custom33,
	uint16_t custom34,
	uint16_t custom35,
	uint16_t custom36,
	uint16_t custom37,
} roverlink_custom_debug_3_t;

typedef struct __roverlink_ping_t {
	uint8_t[8] payload,
} roverlink_ping_t;

typedef struct __roverlink_telemetry_config_t {
	messageMask_t telemetryMask_10HZ,
	messageMask_t telemetryMask_1HZ,
	messageMask_t telemetryMask_10SEC,
} roverlink_telemetry_config_t;

typedef struct __roverlink_req_fields_once_t {
	messageMask_t oneShotMask,
} roverlink_req_fields_once_t;

typedef struct __roverlink_broadcast_t {
	broadcast_t broadcastCommand,
} roverlink_broadcast_t;

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

