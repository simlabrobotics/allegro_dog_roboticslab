=================================================================================================
2013/10/1

1. Position gain tuning
-SQ?; // gains for the 1st channel
-SR?; // gains for the 2nd channel
                                      P    I        D
default: SQ0000A000,00004000,007D0000,00AA,000000AA,0140,0020,00000040,0010,0080,00000004
tuning1: SQ0000A000,00004000,007D0000,0080,00000080,0100,0020,00000040,0010,0080,00000004 // by Kim
tuning2: SQ0000A000,00004000,007D0000,0100,1FFFFFFF,01FF,0020,00000040,0010,0080,00000004 // by Sean
tuning3: SQ0000A000,00004000,007D0000,0080,1FFFFFFF,01FF,0020,00000040,0010,0080,00000004 // by Sean (bigger P gain, less position error, bigger overshoot)
tuning4: SQ0000A000,00004000,007D0000,0100,01FFFFFF,02FF,0020,00000040,0010,0080,00000004 // by Sean (lower D gain, less positition error, less vibration)


2. Save the parameter
-EsA55A;
-Turn it off and on again

3. Motor on
-PE0001; // Power enable for the motor drive 1

4. Set mode of operation
-SM0002; // position mode for the 1st channel only
-SM0200; // position mode for the 2nd channel only
-SM0202; // position mode for the both channels

5. Generate square wave
-Ts500,50; // 1st channel, period: 500msec, amplitude: 50
-Tt500,50; // 2nd channel, period: 500msec, amplitude: 50
-Ts0,0; // Disable generation of the square signal for the 1st channel
-Tt0,0; // Disable generation of the square signal for the 2nd channel

6. Motor off
-PD0001; // Power disable for the motor drive 1

=================================================================================================
2013/9/30

1. Homing (configuring parameters)
GT: set detecting voltage threshold for home/limit sensor
SI: set position limit of homing process
SS: set joint velocity for homing(position servo mode)

2. Homing
PE: servo on
SM0202: enable position servo mode

Gj: using one home sensor, in encoder pulse offset (4000 for a rotation)
GJ: using one home sensor, in 0.01 degree  (36000 for a rotation)
-1st param: (01:1st motor only, 02:2nd motor only, 03:both motors)
-2nd param: (00:1st motor (-) direction/2nd motor (-) direction, 01: 1st(+)/2nd(-), 11: 1st(+)/2nd(+), 10: 1st(-)/2nd(+))
-3rd param: offset for the 1st motor (GJ: multiplied by 0.01 degree, Gj: pulse)
-4th param: offset for the 2nd motor (GJ: multiplied by 0.01 degree, Gj: pulse)

Ex)
PE0001;
SM0202;
SS200,200;
GJ03,11,-9000,-9000;


////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////Begining of Position Mode/////////////////////////////
2. Robocube motor driver
if the postion value should be reset, send "QEA55A;", then the position of motor driver be 5000000(initial value)
if "QEA5A5;", then the position of the channel 1 is going to be 5000000 only
if "QE5A5A;", then the postiion of the channel 2 is going to be 5000000 only

P14000; // Channel 1 relative position move 4000 cnts.
P24000; // Channel 2 relative position move 4000 cnts.

PE0001; //SERVO ON
SM0002; //FOR CHANNEL 1
SM0200; //FOR CHANNEL 2
if SM0202; THEN BOTH 

PA5000000,5000000; //THIS IS THE POSTION WHEN POWER UP(INITITAL POSITION)
PA5004000,5000000; //FOR CHANNEL 1

SS1000,2000; //FOR CONTSTANT VELOCITY OF CH1 IS 1000RPM
Ss100,200;   //100 MEAN ACCERATATION TIME [ms], 200 FOR CH2

3. 
PE0001;
SM0F0F; //FOR Sinusoidal position control mode
S11000,1000; //1ST PARAMETER =1000 MEANS 1000[ms] of preiod
	     //2nd parameter =1000 means position value of encoder
for channel 2,
S2500,2000;


4. Homming

Gj : 1st paratmer for motor channel , 00 disisble, 01 for motor 1, 02 for motor 2, 03 for all channel
     2nd parameter for direction of homming, 0 for negative, 1 for positiove
      example) 00 for negative homming for two channel, 01 means, negative homming for chennel 2, 1 means positive homming for channel 1
     3rd parameter for setting offset position of channel 1 (degree) (the value must be ineger)
     4rd parameter for setting offset psoition of channel 2 (degree) (the value must be ineger)
GJ03,11,-100,-100;  // 100 * 0.01 (degree resolution) = 1 degree output
		    // 1000 (input) * 0.01 = 10 degree output
////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////End of Position Mode//////////////////////////////////


=================================================================================================
2013/9/27

1. ID
-------------------------------------------------------------------------------------------
Channel(DML)   CAN channel    Driver REQ ID      Driver ACK ID
-------------------------------------------------------------------------------------------
0              0              1                  17
1              0              1                  18
2              0              2                  19
3              0              2                  20
4              0              3                  21
5              0              3                  22
6              1              4                  23
7              1              4                  24
8              1              5                  25
9              1              5                  26
10             1              6                  27
11             1              6                  28
-------------------------------------------------------------------------------------------

2. current limit command
- Sw: set/get maximum current limit (normally set as twice of nominal current limit of a motor)
- SI: set/get nominal current limit (set nominal current limit and time, 110%~120% of nominal current limit of a motor)


=================================================================================================
2013/8/28

1. Robocube motor drive parameter setting
STFFFF;				제어 파라미터 변경이 가능한 타입으로 설정합니다.
EDA55A;				모든 파라메터를 factory default 값으로 바꿉니다.
SEA55A,0FA0,0FA0,0001,0001;	4체배 한 엔코더 펄스수를 4000 (0x0FA0)로 설정합니다.
SGA55A,86,86,7000,7000;		감속비 1/86, 최대속도 7000rpm
EDA55A;				이 과정에서 바뀐 파라메터에 따른 초기화를 실시합니다.
SX00000070,FFFFFFFF;		// set communication method as CAN
SAFE01;				// set CAN arbitration id (slave id, 1~12)
SmFB04;				// set CAN arbitration id for acknowledgement to PC (17~28)
EsA55A;				// save the parameters.

** 파라미터 세팅은 중간에 ID 변경 가능성 때문에 RS232로 하는 것을 권장함.

2. motor.dml 설정 변경
- torque constant 재설정: 29.2 mNm/A 로 설정
- current lower/upper 재설정: 일단 모터 data sheet의 nominal current(max continuous current) 값으로 설정

3. Conversion from current to command
- Range of current command: 5,000,000 - 1638 ~ 5,000,000 + 1638 (practically 1310 is the maximum limit)
- In case of DC200W motor drive, maximum current is 16A, 1A is correspond to 81.9
- In case of DC100W motor drive, maximum current is 8A, 1A is correspond to 163.8

4. Test motor(RS232)
PE0001;		// servo on
SM0007;		// set current mode for the 1st channel
SC90;		// open-loop current command (about 1A)
PD0001;		// servo off


/////////////////////////////////////////////////////////////////////////////////
// Tune gains for current control (9/9)
/////////////////////////////////////////////////////////////////////////////////
1. Ts command: generate square wave
Ts500,100;    // Generate square wave, period of 500msec and amplitude of 100
Ts0,0; // stop square wave

SQ?;  // print current control gains
SQ00009C40,00003E80,007A1200,00A6,000000A6,060A,0247,000005B2,16C9,038E,00000004

// gain tuning #1
SQ00009C40,00003E80,007A1200,00A6,000000A6,060A,0247,000005B2,16C9,018E,00000004

// gain tuning #2, final
SQ00009C40,00003E80,007A1200,00A6,000000A6,060A,0247,000005B2,16C9,0070,00000004

/////////////////////////////////////////////////////////////////////////////////
// Set gains for the second channel
/////////////////////////////////////////////////////////////////////////////////

// instead of Ts command, use Tt
Tt500,100;    // Generate square wave, period of 500msec and amplitude of 100

// instead of SQ command, use SR
SR00009C40,00003E80,007A1200,00A6,000000A6,060A,0247,000005B2,16C9,0070,00000004







