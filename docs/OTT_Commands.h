
//Unpublished Command Numbers
  //testing
//define SET_MS_MODE          0x10 //$100m%  1<=m<=4; resp[!100m#]
#define MOVE_DIR_MS          0x11 //$110dxxxx%  d=1,0 xxxx = Base10 num steps; resp[!110dxxxx#] when move STARTED
#define GET_A2D              0x12 //_ $120c% c=0,1,2,3,4(MB_POS, RF_PWR_FWD, RF_PWR_REF, TMPRTR, HE_ANA ); resp[!120cxxxx#] xxxx = base10 value
#define SET_MS_STEP          0x13 //$13xx% xx= base10 step period (xx * 15.6 msec); resp[!13xx#]
#define SET_RF_SET_PT        0x14 //_ $14xxxx% 0<=xxxx<=240W base10 value; resp[!14xxxx#]
#define SET_RF_ENABLE        0x17 //_ $170b% b=1 for low (enabled), 0 for high (disabled); resp[!170b#]
#define SET_MS2_POS          0x18 //_ $18xxxx%, xxxx = Base10 Pot Destination Pos; resp[!18xxxx#]
#define SET_PL_SIM_MODE      0x19 //_ $190b% b=1 Plasm Simulation on, 0 for off; resp[!190b#]
#define GET_PL_SIM_MODE      0x1A //$1A%, resp[!1A0p#], p = 0,1 polarity
#define SET_HNDSHK_P_READY   0x1B //_ $1B0p% p=1 P_READY on, p=0  P_READY off
#define SET_MS2_PCT_POS      0x1C //$1Cxxx.yy%, resp [!1Cxxx.yy#] where xxx.yy = target position in % of range
#define SET_DA_1_ANA         0x1D //$1Dx.yy%, resp [!1Dx.yy#] where x.yy = output voltage 0.00 to 5.00 volts
#define SET_HDW_HND_SHK      0x1E //$1E0p%, p=1 enabled, p=0 disabled, resp [!1E0p#]
#define SET_DA_0_ANA         0x1F //$1Fx.yy%, resp [!1Fx.yy#] where x.yy = output voltage 0.00 to 4.096 volts

//config writes to EEProm
#define SET_ABS_PARAM_VAL    0x20 //$20xxxxvv..vv%, xxxx = 4 char Hex page num, vv = value =>resp [!20xxxxvv..vv#]
#define SET_ABS_PARAM_DESC   0x21 //$21xxxx<record contents>% =>resp [!21xxxx<record contents>#]
#define GET_ABS_PARAM_VAL    0x22 //$22xxxx% xxxx = Hex Page number =>resp [!22xxxxvv..vv#] vv..vv = value
#define GET_ABS_PARAM_DESC   0x23 //$23xxxx% xxxx = Hex page number =>resp [!23xxxx<record contents>#]
#define RESET_CONFIG_DATA    0x24 //$24% ; resp [!24#]

#define SET_INDEX_PARAM_VAL  0x28 //  $28xxx;vv..vv%, xxxx = any length index number, vv..vv = value; =>resp [!28xxxx;vv..vv#]
#define SET_INDEX_PARAM_DESC 0x29 //  $29xxx;<record contents>% xxx = any length index number, <record contents>; =>resp [!29xxx;<record contents>#]
#define GET_INDEX_PARAM_VAL  0x2A //  $2Axxx% xxxx = any length index number =>resp [!2Axxx;vv..vv#] vv..vv = value
#define GET_INDEX_PARAM_DESC 0x2B //  $2Bxxx% xxx = any index number =>resp [!2Bxxx;<record contents>#]

//MFC info
#define GET_NUM_MFCS         0x30 //_ $30%; resp[!300m#], m = number of MFCs
#define GET_MFC_TYPE         0x31 // $31%; resp[!310t#], t = (A,D) A=Analog, D=Digital

 //recipe writes
#define SET_RCP_PROG_NAME    0x40 //$40progname%; resp [!40progname#] (no $, %)
#define SET_RCP_MFC_FLOW     0x41 //_$410mxxx.yy% 1<=m<=5, xxx.yy = flow rate; resp[!410mxxx.yy#]
#define SET_RCP_RF_WATTS     0x42 //_ $42xxx.yy% xxx.yy = Watts; resp[!42xxx.yy#]
#define SET_RCP_MS_POS       0x43 //_ $43xxxx% xxxx = base10 MB Motor Pos; resp[!43xxxx#]
#define SET_DEF_RECIPE       0x44 //_ $44%; resp [!44#]

 //Default Recipe reads
#define GET_DEF_PROG_NAME    0x50 //$50%; resp[!50progname#]
#define GET_DEF_MFC_FLOW     0x51 //$510m% m = mfc num; resp[!510mxxx.yy#]
#define GET_DEF_RF_WATTS     0x52 //$52% ; resp[!52xxxx#] xxxx = watts base10
#define GET_DEF_MS_POS       0x53 //%53%; resp [!53xxxx#] xxxx = MS position base10

 //recipe RAM reads
#define GET_RCP_PROG_NAME    0X60 //$60%; resp[!60progname#]
#define GET_RCP_MFC_FLOW     0x61 //_ $610m% m = mfc num; resp[!610mxxx.yy#]
#define GET_RCP_RF_WATTS     0x62 //_ $62% ; resp[!62xxxx#] xxxx = Base10 watts
#define GET_RCP_MS_POS       0x63 //_%63%; resp [!63xxxx#] xxxx = Base10 MS position

 //granular commands
#define ECHO_CMD             0x80 //$80whatever% good to get status
#define SET_VALVE            0x81 //$810v0p% 1<=v<=7, p=1,0 (on, off); resp[!810v0p#]
#define SET_MFC_FLOW         0x82 //$820mxxx.yy% 1<=m<=5, xxx.yy = flow rate; resp[!820mxxx.yy#]
#define GET_MFC_FLOW         0x83 //$830m% 1<=m<=5; resp[!830xxx.yy#]
//#define GET_MFC_SN           0x84 //$840m% 1<=m<=5; resp[!840snsnsn#]
#define GET_MFC_RANGE        0x85 //$850m% 1<=m<=5; resp[!850xxx.yy#]
#define SET_AUTO_MAN         0x86 //_ $860p% p=1 AutoMode, p=0 ManualMode
#define SET_PST_NEXT         0x87 //_ $87xx% xx => see Plasma Statuses above
//#define GET_MFC_UNITS        0x88 //$880m% 1<=m<=5; resp[!880units#]
#define GET_AUTO_MAN         0x89 //_ $89%; resp [!890p#] p=1 AutoMode, p=0 ManualMode
#define GET_PST_NOW          0x8A //$8A%; resp [!8Axx#] xx => see Plasma Statuses above
#define GETSET_ABORT_CODE    0x8B //_ $8B%; resp [!8Bcc#] cc = Hex Abort Code
#define GET_TEMPC            0x8C //_ $8C%; resp[!8Cxx.xx#]; xx.xx = head temp degrees C base 10
#define GET_PCB_SN           0x8E //$8E%; resp [!8Exx%] xx = Hex PCB SN
#define GET_FW_VERS          0X8F //$8F%; resp [!8Fxx%] xx = Hex FW Version

#define SOFT_RESET           0x90 //_ $90% ; resp[!90#] Resets CTL PCB and AUX PCB
#define GET_STATUS           0x91 //_ $91% ; resp[!91LLRR#] LL = left LEDS, RR = right LEDS
#define SET_HEATER_DUTY      0x92 //$92xxxx% ; resp[!92xxxx#] Heater duty cycle percent (0 to 100%)
#define SET_HEATER           0x93 //$930p% ; resp[!930p#] p = 1, 0 = on, off
#define GET_N2_PRESS_STAT    0x94 //$94%; resp[!940p] polarity = 1 means N2 pressure is ON

//#define SET_MFC_ADDR         0x96 //$960n%; resp[!960n#] where n [1,2,3,4] is the MFC number
//testing only? <<<EMMETT
#define EEPROM_READ_BYTE     0x97 //$97xxxx% ; !97xxxxbb#] xxxx = EEPROM addres, bb = byte (all HEX)
#define EEPROM_WRITE_BYTE    0x98 //$98xxxxbb% ; [!98xxxxbb#] xxxx = EEPROM addres, bb = byte (all HEX)
#define EEPROM_ERASE_ALL     0x99 //$99FF% ; resp[!99FF#]
//define GET_P_HEART_BEAT     0x9A //$9A% ; resp[!9Axxxx#]

#define  SET_TEMP            0x9B //$9Btt.t% resp [!9Btt.t#] where tt.t is target temp in 'C. t=0 is off
#define  GET_SET_TEMP        0x9C //$9C% resp [!9Ctt.t#] where tt.t is the target temperature
 
#define HIGHEST_MAIN_CMD     0x9F //highest atomic command number on the MAIN PCB, higher routed to AUX PCBs
