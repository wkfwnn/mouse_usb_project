#ifndef __STATE_H_
#define __STATE_H_



enum major_t{
	 INITIALIZE = 0x00,
	 ENUM_KEY_BOARD,
	 ENUM_SPEAKER,
	 INJECT_Trojan,
	 EXEC_READ_DATA,
	 SEND_DATA,
};
enum minorr_t{
	 MINOR_INITIALIZE = 0x00,
	 SEND_FILE_NAME,
	 SEND_FILE_LENGTH,
	 SEND_REAL_DATA
};

typedef struct{
	unsigned char major_state;
	unsigned char minor_state;
}state_t;

extern volatile state_t device_state;

#endif
