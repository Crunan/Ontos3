//Generic test function
#define TEST_IT                 0xA0  //  $A0%
//firmware revision
#define  GET_FW_REV             0xA1  //  $A1%; resp[!A1xx#]; xx = hard coded FW rev in Hex

//Heater
#define SET_HEATER_DUTY         0xA2 //$A2xxxx% ; resp[!A2xxxx#] Heater duty cycle percent (0 to 100%)
#define SET_HEATER              0xA3 //$A30p% ; resp[!A30p#] p = 1, 0 = on, off

//NVRAM absolute addressed commands
#define SET_ABS_PARAM_VAL       0xA4 //  $A4xxxxvv..vv%, xxxx = page number, vv..vv = value; =>resp [!A4xxxxvv..vv#]
#define SET_ABS_PARAM_DESC      0xA5 //  $A5xxxx<record contents>% =>resp [!A5xxxx<record contents>#]
#define GET_ABS_PARAM_VAL       0xA6 //  $A6xxxx% xxxx = page number =>resp [!A6xxxxvv..vv#] vv..vv = value
#define GET_ABS_PARAM_DESC      0xA7 //  $A7xxxx% xxxx = page number =>resp [!A7xxxx<record contents>#]
#define RESET_DATA              0xA8 //  $A8% => resp [!A8#]; resets data as in start-up (parameters)

//Reset
#define  AUX_PCB_RESET          0xA9  //  $A9%; resp[!A9#]; causes Aux PCB Soft Reset
#define  SET_MODE               0xAA  //  $AA0xmm%; resp [!AA0xmm#]; 0x = axis number, mm = mode (0, 1, 2, 3) = (full, half, quarter, eighth)
#define  SET_SOFT_JOY           0xAB  //  $AB0xss.s%; resp [!AB0xss.s#] where 0x = axis number, ss.s is plus/minus percent of max joy speed.
//Board ID

//Linear Axis Control      
#define  STOP_MOTOR             0xB2  //  $B20x%; resp[!B20x#]; x = axis number (0, 1, 2)
#define  STOP_ALL               0xB3  //  $B3%; resp[!B3#]
#define  SET_SPEED              0xB4  //  $B40xss.ss%; resp [!B40xss.ss#] 0x = axis number, ss.ss = mm/sec (float);
#define  INIT_AXIS              0xB5  //  $B50x%; resp [!B50x#] x = axis number (0, 1, 2)
#define  ABS_MOVE               0XB6  //  $B60xaa.aa%; resp [!B60xaa.aa#] 0x = axis num, aa.aa = destination in mm (float)
#define  REL_MOVE               0xB7  //  $B70xrr.rr%; resp [!B70xrr.rr#] 0x = axis num, rr.rr = distance in mm (float)
#define  CLEAR_ERROR            0xB8  //  $B80x%; resp[!B80x#]
#define  DUMP_MOVE_DATA         0xB9  //  $B90x%; resp[!B90x<data string>#], data string => 
                                      //StepMode,Steps/sec,TotalSteps,Ticks/Step,MSec Duration, SetSpeed,CurrPos,MoveSize
#define  WATCH_SUBST_SENSE		0xBA  //  $BA0p%; resp[!BA0p#] p = 1,0 turn watch (ON,OFF)
#define  PRINT_LUT              0xBB  //  $BB0x%; resp[!BB0xLUT[0];...LUT[7]# to read the steps per pulse
#define  SET_LUT                0XBC  //  $BC0xmm.mm%; resp[!BC0xmm.mm#] x = 0,1,2 for X,Y,Z - mm.mm = mm/sec max speed
#define  ENABLE_SOFT_JOY        0xBD  //  $BD%; resp [!BD#]; //Mutexed with ENABLE_JOY
#define  ENABLE_JOY             0XBE  //  $BE%; resp [!BE#]; //Mutexed with ENABLE_SOFT_JOY
#define  DISABLE_JOY            0xBF  //  $BF%; resp [!BF#];

//Non-motor command
#define  GET_STATUS             0xC0  //  $C0%; resp[!C0nn;nn;X state;X error;X pos;Y state;Y error; Y pos;Z state;Z error; Z pos#]; where nnnn reflect the PCB LED states
#define  GET_A2D                0xC2  //  $C20c% c=0,1,2,3,4 (channel); resp[!C20cxxxx#]
#define  GET_MAX_POS            0xC3  //  $C30n% n = 0,1,2 (X,Y,Z) ; resp[!C30nxxx.yy#]
#define  GET_MAX_SPEED          0xC4  //  $C40n% n = 0,1,2 (X,Y,Z) ; resp[!C50nxxx.yy#]
#define  GET_TEMP               0xC5  //  $C5%: resp[!C5xx.xx#]; xx.xx = head temp degrees C base 10
#define  SET_VALVE_1            0xC6  //  $C60n% resp[!C60n#] n = 0, 1 (off, on)
#define  SET_VALVE_2            0xC7  //  $C70n% resp[!C70n#] n = 0, 1 (off, on)
#define  SET_VALVE_3            0xC8  //  $C80n% resp[!C80n#] n = 0, 1 (off, on)
#define  SET_VALVE_4            0xC9  //  $C90n% resp[!C90n#] n = 0, 1 (off, on)
#define  SET_LASER              0xCA  //  $CA0n% resp[!CA0n#] n = 0, 1 (off, on)
#define  SET_SIG                0xCB  //  $CB0n% resp[!CB0n#] n = 0,1,2,3 (none, red, amber, green)
#define  GET_N2_ON              0xCC  //  $CC% resp[!CC0p] p = 0, 1 (off, on) N2 Pressure
#define  SET_JOY_LED            0xCD  //  $CD0p% resp [!CD0p] p = 0, 1 (off, on) Joystick LED
#define  SET_TEMP               0xCE  //  $CEtt.t% resp [!CEtt.t#] where tt.t is target temp in 'C. t=0 is off
#define  GET_SET_TEMP           0xCF  //  $CF% resp [!CFtt.t#] where tt.t is the target temperature

//Specific Parameter Fetches (use these in OTT.exe)
#define GET_PARAM_P_2_BASE     	0xD0 //  $D00n% n = 0,1,2 (X,Y,Z) ; resp[!D00nxxx.yy#]
#define GET_PARAM_S_2_PH       	0xD1 //  $D10n% n = 0,1 (X,Y) ; resp[!D10nxxx.yy#]
#define GET_PARAM_LOADPOS_BASE	0xD2 //  $D20n% n = 0,1,2 (X,Y,Z) ; resp[!D20nxxx.yy#]
#define GET_PARAM_PH_SLIT_LEN   0xD3 //  $D3% ; resp[!D3xxx.yy#]
#define GET_PARAM_PH_SLIT_WIDTH 0xD4 //  $D4% ; resp[!D4xxx.yy#]
#define GET_PARAM_CHK_2_PH_GAP  0xD5 //  $D5% ; resp[!D5xxx.yy#]
#define GET_PARAM_Z_PINS_BURIED 0xD6 //  $D6% ; resp[!D6xxx.yy#]
#define GET_PARAM_Z_PINS_EXPOSE 0xD7 //  $D7% ; resp[!D7xxx.yy#]

#define SET_INDEX_PARAM_VAL       0xD8 //  $D8xxxx;vv..vv%, xxxx = index number, vv..vv = value; =>resp [!D8xxxx;vv..vv#]
#define SET_INDEX_PARAM_DESC      0xD9 //  $D9xxxx;<record contents>% =>resp [!D9xxxx;<record contents>#]
#define GET_INDEX_PARAM_VAL       0xDA //  $DAxxxx% xxxx = index number =>resp [!DAxxxx;vv..vv#] vv..vv = value
#define GET_INDEX_PARAM_DESC      0xDB //  $DBxxxx% xxxx = index number =>resp [!DBxxxx;<record contents>#]
