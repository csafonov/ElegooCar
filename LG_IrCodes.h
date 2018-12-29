/// File: LG_IrCodes.h
//LG control.
//NEC-protocol
//Each button sends 1 or 2 codes: first is operational,
//second=0x4AB0F7B6 means REPEAT BUTTON
// KEY		OPERATIONAL CODES
enum ERemoteKeys // EIrCodes_LG_MP3
{
key_PressedRepeat=0x4AB0F7B6,
key_Power=	0x08087887, 
key_Preset_UP=	0x0808C837,
key_Preset_DN=	0x080848B7,
key_Mute=		0x0808F807,
key_Volume_UP=	0x0808E817,
key_Volume_DN=	0x08086897,
key_Tuner_Band=	0x08089A65,
key_REWIND_LEFT=	0x0808609F,
key_REWIND_RIGHT=	0x0808E01F,
key_PAUSE_PLAY=	0x080820DF,
key_STOP	=	0x0808A05F,
key_Sleep	=	0x080843BC,
key_repeat_track=	0x0808728D,
key_Program_memo=	0x0808B24D,
key_Play_small=	0x0808B04F,
key_Wind_left=	0x080808F7,
key_Wind_right=	0x08088877,
key_Record_pause=	0x0808A857,
key_Stop_small =0x0808708F,
key_EQ_PATT=	0x080802FD,
key_UBB	=	0x0808B847,
IREM_KEY_1=		0x0808827D,
IREM_KEY_2=		0x080842BD,
IREM_KEY_3=		0x0808C23D,
IREM_KEY_4=		0x080822DD,
IREM_KEY_5=		0x0808A25D,
IREM_KEY_6=		0x0808629D,
IREM_KEY_7=		0x0808E21D, // turn micro servo left
IREM_KEY_8=		0x080812ED, 
IREM_KEY_9=		0x0808926D, // turn micro servo right
IREM_KEY_0=		0x0808D22D
};
// unlock password: POWER,0,key_Stop_small
// lock password: Mute,0,Mute
#define IREM_KEY_left  key_REWIND_LEFT  /// move left
#define IREM_KEY_right key_REWIND_RIGHT /// move right
#define IREM_KEY_up       key_PAUSE_PLAY /// move forward.
#define IREM_KEY_dn     key_repeat_track /// move back
#define IREM_KEY_star   key_Sleep       
#define IREM_KEY_ok     key_STOP
		  




















