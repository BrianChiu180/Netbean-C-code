/** 
**	test utility   2019 Brian chiu
**/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
/** These two files are necessary for calling CTOS API **/
#include <ctosapi.h>
#include <ctos_clapi.h> //cl test need
#include <linux/input.h> //ioctl lib
#include <fcntl.h>  //open lib
#include <unistd.h> 
#include <sys/types.h> 
#include <dirent.h>
#include <sys/stat.h> //sd card need
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include "ctos_clapi.h"
#include "vwdleapi.h"
#include <signal.h>
#include <time.h>
#include <errno.h>
//defines

#define TypeA       (0x60)
#define TypeB       (0x61)
#define Block       (10)

#define buff 2048 //use in BT test
//struct stat fInfo;
#define BUFF 2048

#define PPP 45000
#define CTMS 5000
#define CONNECT 2
#define DOTLINE 12
//#define

USHORT GSM_Get_Version(BYTE *GSMVersion, USHORT *VersionLen){
    BYTE bkey,baStr[32],baRxBuf[100];
    USHORT i,usRtn,usRxLen,usRecLen;
    ULONG ulTick;
    
    usRtn =CTOS_GSMSendData("AT+CGMR\x0D",strlen("AT+CGMR\x0D"));  //send data to GSM model
    if(usRtn !=d_OK) return usRtn;
    memset(baRxBuf,0x00,sizeof(baRxBuf));  //set baRxBuf zero
    usRxLen =0;
    ulTick =CTOS_TickGet();  //get system tick value 
    while(1){
        if(CTOS_TickGet() -ulTick>20) return-1;   CTOS_KBDHit(&bkey);
        if(bkey ==d_KBD_CANCEL) return-1; usRecLen =sizeof(baRxBuf);  //set len size
        usRtn =CTOS_GSMRecvData(&baRxBuf[usRxLen],&usRecLen);  //put into receive function we just set,data save in baRxBuf[usRxLen]
        if(usRtn !=d_OK) return-1; usRxLen =usRxLen+usRecLen;
        if(usRxLen>2 &&baRxBuf[usRxLen-2] ==0x0D &&baRxBuf[usRxLen -1] ==0x0A){
            for(i=2;i <usRxLen;i++){
                if(baRxBuf[i] ==0x0D){   //detect if there is 0x0D,its last word of gsm version
                    baRxBuf[i]='\0';
                    break;
                }
            }
            *VersionLen =i-2;
            memcpy(GSMVersion,&baRxBuf[2],*VersionLen);
            return 0;
            }
            CTOS_Delay(10);  
        }
    }

int main(int argc,char *argv[])
{
	BYTE key;
        BYTE count;
        USHORT rat,Len=0;
        BYTE ret;
        BYTE str[256], size = 5;
        
 
        //use in selecttest////////
        int S=0,SP=0;
        BYTE Select[32];
        /////BYTE SelectTest[50];
        
        /////color test////////////
        ULONG rb;
        int i,f,j,k,m;
        int c,c1,c2;//color
        int fd;
        struct input_event ev[size];
        /////keyborad test////////
        BOOL isKey;
        int keybor[15];
        /////Flash_test///////////
        BYTE FL[20];
        ULONG size_U[256],size_T[256],size_R[256],size_S[256];
        /////Smart_Card_test
        BYTE rea,reb[16];
        BYTE SC[] = {d_SC_USER, d_SC_SAM1, d_SC_SAM2, d_SC_SAM3, d_SC_SAM4};
        BYTE bStatus,baATR[128], bATRLen,CardType, tmp[128],baRAPDU[128],baRAPDU1[128];
        BYTE baSAPDU[128];
        int card;
        USHORT bSLen,bRLen;
        /////MSR test//////////////
        USHORT pusTk1Len,pusTk2Len,pusTk3Len;
        USHORT rtn;
        BYTE baTk1Buf[128],baTk2Buf[128],baTk3Buf[128],babuff[2048],baBuff[128];
        BYTE baTk1Err, baTk2Err, baTk3Err;
        /////RTC test//////////////
        CTOS_RTC stRTC;  
        /*ctos_rtc function function struct system time and date information
         BYTE bSecond; BYTE bMinute; BYTE bHour; BYTE bDay; BYTE bMonth; BYTE bYear; BYTE bDoW; */
        STR *ti[12];
        STR *week[] = {"0", "Monday", "Tuesday", "Wednesday",\
                  "Thursday", "Friday", "Saturday", "Sunday"};
        BYTE baBUFF[15],time[32];
        BYTE RTCcode[15], isSet = FALSE;
        ULONG cobuff, colen;
        int tb, wd, tx, ty, tz = 0;
        STR *keyboardLayoutNumber[] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "", ""};
        /////Font_test/////////////////////////////////
        struct dirent{
        ino_t d_ino; //inode number
        off_t d_off; //offset of this dirent
        signed short int d_reclen; //length of this d_name
        unsigned char d_type; //type of this d_name
        char d_name[256];  //file name ,max is 256
         };
        BYTE buf[256], FP[3],fcmp[] = "fnt",ttf_temp[32];
        BYTE syb[17];
        DIR *dir;
        STR *ttf[256];
        struct dirent *font_name = NULL;
        CTOS_FONT_ATTRIB Font1;
        CTOS_FONT_ATTRIB Font2;
        /////Printer_test/////////////////////////////
        BYTE pt[32],snb[16],sdb[8],bavalue[128],len,smi[10];
        BYTE baTemp[PAPER_X_SIZE*64];//width is 6*64=384
        CTOS_RTC Pr_time;
        int pa,ppx,ppy,tt;
        USHORT lang=0;
        DWORD USB;
        USHORT PRFontID,PRFontSize,Style;
        
        BOOL fPortable,fPCI;
        USHORT mkHWSupport;
        /////CL card test//////////////////
        BYTE KeyType,CLC_ret,LK,AT,SAK,SNLen;
        BYTE ATQA[2], SN[10], PUPI[2];
        BYTE bAFI,ATQB[2],ATQBLen;
        UCHAR ID[16],PM[16];
        int cc,cd,cf;
        BYTE co[5];
        /////SD card test/////////////////
        int fStat;
        FILE *file;
        CHAR SD_f1[11], SD_f2[11], SD_f3[11], SD_f4[11], SD_f5[11], SD_d[11];
        int SD1, SD2, SD3, SD4, SD5;
        struct stat fInfo; 
        USHORT usRet = d_NO;
        //////wifi test///////////////////
        struct sockaddr_in sa;
        BYTE PW[50];
        BYTE temp[32],W_Num;
        BYTE Page,NG,OK;
        BYTE flg=TRUE;
        BYTE tLen,info[32];
        int current_page,sockfd;
        int tx_len,rx_len,lc,tal,ral,del;
        DWORD Status;
        CHAR W_Data[1024],W_Recv[1024];
        CTOS_stWifiInfo *Wifi, *W_List;
        STR * keyboardLayoutEnglish[] = {"0 ", "1QZqz", "2ABCabc", "3DEFdef", \
                                     "4GHIghi", "5JKLjkl", "6MNOmno", "7PRSprs", \
                                     "8TUVtuv", "9WXYwxy", "", ""};
        /////BT test////////////////
        BYTE BT_Device[buff],breb[32],BT_Num,PASSKEY[32],btinfo[256];
        USHORT btinfolen;
        int g;
        stDeviceInfo *Device;
        /////COMM TEST
	BYTE bPORT,USBPort;
        BYTE Data[2048];
        BYTE round[15],maclen;
        int totalloop,loop;
        int comm;
        USHORT SendLen,RecvLen,RecvLenx;
        //STR *keyboardLayoutNumber[] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "", ""};
        STR *ipKeyBoardLayout[] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "", "."};
        //STR * keyboardLayoutEnglish[] = {"0 ", "1QZqz", "2ABCabc", "3DEFdef", \
                                     "4GHIghi", "5JKLjkl", "6MNOmno", "7PRSprs", \
                                     "8TUVtuv", "9WXYwxy", "", ""};
        BYTE HostIP[128],HostIPlength;
        BYTE length;
        BYTE IMEI[16], rData[2048];
        BYTE GM_buff[128], SIM[] = {d_GPRS_SIM1, d_GPRS_SIM2}, dn[12];
        BYTE GSM_S = 99, GSM_Lbuff, GSM_R, sock;
        BYTE GSM_RState[] = {d_GSM_GPRS_STATE_NOT_REG, d_GSM_GPRS_STATE_REG, \
                            d_GSM_GPRS_STATE_TRYING, d_GSM_GPRS_STATE_DENY, \
                             d_GSM_GPRS_STATE_UNKNOW, d_GSM_GPRS_STATE_ROAM};
        STR *IP[] = {"1st", "2nd", "3rd", "4th"};
        BYTE GPRS_LIP[4] = {0}, showIP[30], gh[] = {218,211,35,219};
        CHAR APN[32];
        USHORT GPRS_Port = 140;
        BYTE ipad[] = "192.120.100.96", mask[] = "255.255.255.0";
        BYTE gate[] = "192.120.100.200", h[] = "192.120.100.213";
        BYTE port[] = "5000", atco[] = "2", maco[] = "0"; 
        stSMS_Submit SMS;
        BYTE phonenum[128];
        /////bt test///////////
        stServiceInfo *Service;
        BYTE SName[] = "WCL Bluetooth Service";
        BYTE ServiceName[] = "DialupNetworking";
        BYTE ServiceName2[] = "SerialPort";
        
        unsigned char my_uuid[16] = {0x00, 0x00, 0x11, 0x03, 0x00, 0x00, 0x10, 0x00,0x80, 0x00, 0x00, 0x80, 0x5F, 0x9B, 0x34, 0xFB};
        unsigned char my_xuuid[16] = {0x00, 0x00, 0x11, 0x01, 0x00, 0x00, 0x10, 0x00,0x80, 0x00, 0x00, 0x80, 0x5F, 0x9B, 0x34, 0xFB};
        //iTxLenA,icounterA;    "00002a25-0000-1000-8000-00805f9b34fb
        //DWORD Rx_Len = 0,Reverse_len=0;
        //USHORT usTotalRxLen;
        //BYTE  C,baTxBuf[2048],baRxBuf[5120],baRxTemp[2048];
        //USHORT  usTxrdy,usRxrdyLen,usTxLen,usTotalTxLen;
        
        
Main:
    CTOS_LCDTClearDisplay();
    CTOS_LCDTSetReverse(TRUE);
    CTOS_LCDTPrintXY(1,1,"  Main Menu         ");
    CTOS_LCDTSetReverse(FALSE);
    CTOS_LCDTPrintXY(1,16,"->");
    SP=S=0; //reset the select
    goto Main1;
    
Main1:
    CTOS_LCDTPrintXY(1,2, "1. LCD               ");
    CTOS_LCDTPrintXY(1,3, "2. KeyBoard          ");
    CTOS_LCDTPrintXY(1,4, "3. Flash             ");
    CTOS_LCDTPrintXY(1,5 ,"4. Smart Card        ");
    CTOS_LCDTPrintXY(1,6, "5. Back Light        ");
    CTOS_LCDTPrintXY(1,7, "6. MSR               ");
    CTOS_LCDTPrintXY(1,8, "7. LED               ");
    CTOS_LCDTPrintXY(1,9, "8. RTC               ");
    CTOS_LCDTPrintXY(1,10,"9. Printer           ");
    CTOS_LCDTPrintXY(1,11,"10.Font              ");
    CTOS_LCDTPrintXY(1,12,"11.CL Transparent    ");
    CTOS_LCDTPrintXY(1,13,"12.CL Card Test      ");
    CTOS_LCDTPrintXY(1,14,"13.SD Card Test      ");
    CTOS_LCDTPrintXY(1,15,"14.Wi-Fi Test        ");
    CTOS_LCDTPrintXY(18,16,"1/2");
    /*
    while(1){
    CTOS_UIKeypad(3, 16, keyboardLayoutNumber, 40, 80, d_TRUE, d_FALSE, 0, 0, SelectTest, 5);  //the four is delay
    if(atoi(SelectTest) ==1) goto LCD_test; if(atoi(SelectTest) ==2) goto LCD_test;
    if(atoi(SelectTest) ==3) goto LCD_test; if(atoi(SelectTest) ==4) goto LCD_test;
    if(atoi(SelectTest) ==5) goto LCD_test; if(atoi(SelectTest) ==6) goto LCD_test;
    if(atoi(SelectTest) ==7) goto LCD_test; if(atoi(SelectTest) ==8) goto LCD_test;
    if(atoi(SelectTest) ==9) goto LCD_test; if(atoi(SelectTest) ==10) goto LCD_test;
    if(atoi(SelectTest) ==11) goto LCD_test; if(atoi(SelectTest) ==12) goto LCD_test;
    if(atoi(SelectTest) ==13) goto LCD_test; if(atoi(SelectTest) ==14) goto LCD_test;   //cancel key is include
    }
    */
Selecttest:
    while(1){
    CTOS_KBDGet(&key);
    if(key ==d_KBD_DOT) goto Main2;
    else if(key ==d_KBD_00) goto Main1;
    else if(key ==d_KBD_CANCEL) exit(0);
    else if(key ==d_KBD_ENTER) {SP=(SP/10); break;}
    else if(key ==d_KBD_CLEAR) SP=(SP/100);
    
    else{
        if(key ==d_KBD_1) {S=1;  SP=SP+S;} if(key ==d_KBD_2) {S=2;  SP=SP+S;} if(key ==d_KBD_3){ S=3;  SP=SP+S;}
        if(key ==d_KBD_4) {S=4;  SP=SP+S;} if(key ==d_KBD_5) {S=5;  SP=SP+S;} if(key ==d_KBD_6) {S=6;  SP=SP+S;}
        if(key ==d_KBD_7) {S=7;  SP=SP+S;} if(key ==d_KBD_8) {S=8;  SP=SP+S;} if(key ==d_KBD_9) {S=9;  SP=SP+S;}
        if(SP !=0 && key ==d_KBD_0) SP =SP*1;
    }
    
        if(SP>=10000000){S=0; SP=0; continue;}
        else if(SP ==0) CTOS_LCDTPrintXY(1,16,"->           ");
        else{
            CTOS_LCDTPrintXY(1,16,"             ");
            sprintf(Select,"->%d",SP);
            CTOS_LCDTPrintXY(1,16,Select);
        SP=SP*10;
        }
    }

    if(SP ==1) goto LCD_test; if(SP ==2) goto Keyboard_test; if(SP ==3) goto Flash_test; if(SP==4) goto Smart_Card_test;
    if(SP ==5) goto Backlight_test; if(SP ==6) goto MSR_test; if(SP ==7) goto LED_test; 
    if(SP ==8) {
        memset(RTCcode,0x00,sizeof(RTCcode));  //erease all code to 0x00
        //sprintf(RTCcode,"12345");             //set default code
        goto RTC_test;}
    if(SP ==9) goto Printer; if(SP ==10) goto Font_test; if(SP ==11) goto Main; if(SP ==12) goto CL_Card;
    if(SP ==13) goto SD_Card_test; if(SP ==14) goto WIFI; if(SP ==15) goto Power_Saving_test; if(SP ==16) goto Comm_test;
    if(SP ==17) goto BT_test;

    else goto Selecttest;

    
    
Main2:
    CTOS_LCDTPrintXY(1,2, "15.Power Saving");
    CTOS_LCDTPrintXY(1,3, "16.Comm Menu");
    CTOS_LCDTPrintXY(1,4, "17.BT Test");
    CTOS_LCDTPrintXY(1,5, "                     ");
    CTOS_LCDTPrintXY(1,6, "                     ");
    CTOS_LCDTPrintXY(1,7, "                     ");
    CTOS_LCDTPrintXY(1,8, "                     ");
    CTOS_LCDTPrintXY(1,9, "                     ");
    CTOS_LCDTPrintXY(1,10,"                     ");
    CTOS_LCDTPrintXY(1,11,"                     ");
    CTOS_LCDTPrintXY(1,12,"                     ");
    CTOS_LCDTPrintXY(1,13,"                     ");
    CTOS_LCDTPrintXY(1,14,"                     ");
    CTOS_LCDTPrintXY(1,15,"                     ");
    CTOS_LCDTPrintXY(18,16,"2/2");
    goto Selecttest;

        
        
 /////1.////
LCD_test:
        i=j=k=m=rb=fd=count=0;
        //SET FONTSIZE=4 x 4 font
        CTOS_LCDTSelectFontSize(0x0404); 
	//SET GRPHIC MODE
        CTOS_LCDSelectMode(d_LCD_GRAPHIC_MODE);
        //CLEAR CANVAS
        CTOS_LCDGClearCanvas();
        //DRAW ON CANVAS
        CTOS_LCDForeGndColor(0x00000000);
        //set a box full with black pixel
        CTOS_LCDGSetBox(0,0,320,480,1);
        CTOS_Delay(500);
        
        //color blue>>green>>red
        //on text mode size160*120
        while(1){
            //color from blue to green
            if(k == 0){
                c=0x00f00000;//blue
                c1=0x00000300;//meidum green
                c2=0x00030000;//medium blue
            }
            //color from green to red
            else if (k == 2){
                c1=0x00000003;//medium red
                c2=0x00000300;//medium green
            }
            for(i = 1; i < 41; i++){
                c=c+c1-c2;
                for(j = 1; j < 121; j++){
                    CTOS_LCDBackGndColor(c);
                    CTOS_LCDTPrintXY(i+k*40,j," ");
                }
            }
            k++;
            if(k==4){
                break;}
        }
        CTOS_KBDGet(&key);
        
         c = 0x00ff0000;//deep blue
         k=0;
         m=0;//control up down task
         while(1){
        if (k == 0 || k == 3){
            c1 = 0x00000200;}//green
        else if (k == 1 || k == 4){
            c1 = 0x00020000;}//blue
        else if (k == 2 || k == 5){
            c1 = 0x00000002;}//red
        
        //text up to down
        if (m == 0){
            for (j = 1; j < 121; j++){
                if (k == 0 || k == 2 || k == 4){
                    c = c + c1;}
                else{
                    c = c - c1;}
                for (i = 1; i < 161; i++){
                    CTOS_LCDBackGndColor(c);
                    CTOS_LCDTPrintXY(i, j, " ");}
            }m = 1;}
        //text down to up
        else{
            for (j = 120; j > 0; j--){
                if (k == 0 || k == 2 || k == 4){
                    c = c + c1;}
                else{
                    c = c - c1;}
                for (i = 1; i < 161; i++){
                    CTOS_LCDBackGndColor(c);
                    CTOS_LCDTPrintXY(i, j, " ");}
            }m = 0;} 
        CTOS_KBDGet(&key);
        k++;
        if (k == 6)
            break;
    }
        CTOS_Delay(2000);
        CTOS_LCDTClearDisplay();
        CTOS_LCDTSelectFontSize(d_FONT_16x30);
        CTOS_LCDBackGndColor(0x00FFFFFF);//white
        CTOS_LCDForeGndColor(0x00000000);//black
        //Clear the screen
        for (i = 0; i < 40; i++){
            for(j = 0; j < 21; j++){
                CTOS_LCDTPrintXY(j, i, " ");
            }
         }
        CTOS_Delay(500);
         //TOUCH PANEL
        CTOS_LCDTPrintXY(1,1,"  Touch Pad Test");
         //set touch field
         k=0; m=0;
         for(i=0;i<3;i++){
             k=20+i*100;
             for(j=0;j<4;j++){
                m=32+j*112;
                CTOS_LCDGSetBox(k,m,80,80,1);
             }
         }
        fd = open("/dev/input/event0", O_RDONLY | O_NONBLOCK);
        if(fd<0){
            CTOS_LCDTPrintXY(1,9,"TP open fail !!");
            exit(0);
        }
        if (ioctl(fd, EVIOCGNAME(sizeof(str)), str) < 0){
        CTOS_LCDTPrintXY(1, 9, " ioctl Open Fail!! ");
        exit(0);}
        int t[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
        while(1){
            CTOS_KBDHit(&key);
            //exit touch panel test
            if(key == d_KBD_CANCEL){
                break;
                exit(0);
            }
            rb = read(fd, ev, sizeof(struct input_event)*size);
            if(rb == -1){
                continue;
            }
            if ( rb < sizeof(struct input_event) ){
            CTOS_LCDTPrintXY(1, 9, "  Read Input Fail!! ");
                break;
                exit(0);
            }
        
        for (count = 0; count < rb/sizeof(struct input_event); count++)
        {
            if(EV_ABS == ev[count].type){
                if(ev[count].code == 0){
                    k = ev[count].value;}
                else if(ev[count].code == 1){
                    m = ev[count].value;}
            }
        } 
        // Key_1
        if(k > 19 && k < 101 && m > 31 && m < 113){
            if (t[0] == 0){
                CTOS_LCDGSetBox(20, 32, 80, 80, 0);
                t[0] = 1;}
        }
        // Key_2
        if(k > 119 && k < 201 && m > 31 && m < 113){
            if (t[1] == 0){
                CTOS_LCDGSetBox(120, 32, 80, 80, 0);
                t[1] = 1;}
        }
        // Key_3
        if(k > 219 && k < 301 && m > 31 && m < 113){
            if (t[2] == 0){
                CTOS_LCDGSetBox(220, 32, 80, 80, 0);
                t[2] = 1;}
        }
        // Key_4
        if(k > 19 && k < 101 && m > 143 && m < 225){
            if (t[3] == 0){
                CTOS_LCDGSetBox(20, 144, 80, 80, 0);
                t[3] = 1;}
        }
        // Key_5
        if(k > 119 && k < 201 && m > 143 && m < 225){
	    if (t[4] == 0){
                CTOS_LCDGSetBox(120, 144, 80, 80, 0);
                t[4] = 1;}
        }
        // Key_6
	if(k > 219 && k < 301 && m > 143 && m < 225){
            if (t[5] == 0){
                CTOS_LCDGSetBox(220, 144, 80, 80, 0);
                t[5] = 1;}
        }
        // Key_7
	if(k > 34 && k < 96 && m > 255 && m < 337){
            if (t[6] == 0){
                CTOS_LCDGSetBox(20, 256, 80, 80, 0);
                t[6] = 1;}
        }
        // Key_8
	if(k > 129 && k < 191 && m > 255 && m < 337){
	    if (t[7] == 0){
                CTOS_LCDGSetBox(120, 256, 80, 80, 0);
                t[7] = 1;}
        }
        // Key_9
	if(k > 224 && k < 286 && m > 255 && m < 337){
	    if (t[8] == 0){
                CTOS_LCDGSetBox(220, 256, 80, 80, 0);
                t[8] = 1;}
        }
        // Key_10
	if(k > 34 && k < 96 && m > 379 && m < 441){
            if (t[9] == 0){
                CTOS_LCDGSetBox(20, 368, 80, 80, 0);
                t[9] = 1;}
        }
        // Key_11
        if(k > 129 && k < 226 && m > 379 && m < 441){
            if (t[10] == 0){
                CTOS_LCDGSetBox(120, 368, 80, 80, 0);
                t[10] = 1;}
        }
        //Key_12
	if(k > 224 && k < 286 && m > 379 && m < 441){
	    if (t[11] == 0){
                CTOS_LCDGSetBox(220, 368, 80, 80, 0);
                t[11] = 1;}
        }CTOS_LCDGSetBox(k, m, 10, 10, 1);
        
        j = 0;
        for (i = 0; i < 12; i++){
            j = j + t[i];}
                
        if (j == 12)
            break;
        }
        CTOS_LCDTClearDisplay();
        CTOS_LCDTSetReverse(TRUE);    
        CTOS_LCDTPrintXY(1, 1, "   Touch Pad Test   ");
        CTOS_LCDTSetReverse(FALSE);        
        CTOS_LCDTPrintXY(4, 9, "Test Success!!");
        CTOS_KBDGet ( &key );
        goto Main;
        
/////2./////        
Keyboard_test:
       
       CTOS_LCDTClearDisplay();
       CTOS_LCDTSetReverse(TRUE);
       CTOS_LCDTPrintXY(1, 1, "  Keyboard test     ");
       CTOS_LCDTSetReverse(FALSE);
       CTOS_LCDTPrintXY(2, 4, "+---+---+---+---+");
       CTOS_LCDTPrintXY(2, 5, "| 1 | 2 | 3 | X |");
       CTOS_LCDTPrintXY(2, 6, "+---+---+---+---+");
       CTOS_LCDTPrintXY(2, 7, "| 4 | 5 | 6 | < |");
       CTOS_LCDTPrintXY(2, 8, "+---+---+---+---+");
       CTOS_LCDTPrintXY(2, 9, "| 7 | 8 | 9 |   |");
       CTOS_LCDTPrintXY(2,10, "+---+---+---| OK|");    
       CTOS_LCDTPrintXY(2,11, "| ^ | 0 | . |   |");
       CTOS_LCDTPrintXY(2,12, "+---+---+---+---+"); 
       for(i=0;i<15;i++){
           keybor[i]=1;
       }
       
       i=15;
        //keyboard test
       while(i != 1){
           CTOS_KBDInKey(&isKey); //detect input is a key or not
           if(isKey){
               CTOS_KBDGet(&key);
               
               if(key == d_KBD_CANCEL){
                   if(keybor[0]==1){
                   CTOS_LCDTPrintXY(16,5," ");
                   keybor[0]=0;
                   i--;
                   }
               }
               else if(key == d_KBD_CLEAR){
                   if(keybor[1]==1){
                   CTOS_LCDTPrintXY(16,7," ");
                   keybor[1]=0;
                   i--;
                   }
               }
               else if(key == d_KBD_ENTER){
                   if(keybor[2]==1){
                   CTOS_LCDTPrintXY(16,10,"  ");
                   keybor[2]=0;
                   i--;
                   }
               }
               else if(key == d_KBD_1){
                   if(keybor[3]==1){
                   CTOS_LCDTPrintXY(4,5," ");
                   keybor[3]=0;
                   i--;
                   }
               }
               else if(key == d_KBD_2){
                   if(keybor[4]==1){
                   CTOS_LCDTPrintXY(8,5," ");
                   keybor[4]=0;
                   i--;
                   }
               }
               else if(key == d_KBD_3){
                   if(keybor[5]==1){
                   CTOS_LCDTPrintXY(12,5," ");
                   keybor[5]=0;
                   i--;
                   }
               }
               else if(key == d_KBD_4){
                   if(keybor[6]==1){
                   CTOS_LCDTPrintXY(4,7," ");
                   keybor[6]=0;
                   i--;
                   }
               }
               else if(key == d_KBD_5){
                   if(keybor[7]==1){
                   CTOS_LCDTPrintXY(8,7," ");
                   keybor[7]=0;
                   i--;
                   }
               }
               else if(key == d_KBD_6){
                   if(keybor[8]==1){
                   CTOS_LCDTPrintXY(12,7," ");
                   keybor[8]=0;
                   i--;
                   }
               }
               else if(key == d_KBD_7){
                   if(keybor[9]==1){
                   CTOS_LCDTPrintXY(4,9," ");
                   keybor[9]=0;
                   i--;
                   }
               }
               else if(key == d_KBD_8){
                   if(keybor[10]==1){
                   CTOS_LCDTPrintXY(8,9," ");
                   keybor[10]=0;
                   i--;
                   }
               }
               else if(key == d_KBD_9){
                   if(keybor[11]==1){
                   CTOS_LCDTPrintXY(12,9," ");
                   keybor[11]=0;
                   i--;
                   }
               }
               else if(key == d_KBD_00){
                   if(keybor[12]==1){
                   CTOS_LCDTPrintXY(4,11," ");
                   keybor[12]=0;
                   i--;
                   }
               }
               else if(key == d_KBD_0){
                   if(keybor[13]==1){
                   CTOS_LCDTPrintXY(8,11," ");
                   keybor[13]=0;
                   i--;
                   }
               }
               else if(key == d_KBD_DOT){
                   if(keybor[14]==1){
                   CTOS_LCDTPrintXY(12,11," ");
                   keybor[14]=0;
                   i--;
                   }
               }
           }
       }
       
       //buzz test
       CTOS_KBDGet(&key);
       CTOS_LCDTClearDisplay();
       CTOS_LCDTPrintXY(2,5,"  Buzzer test  ");
       for(i=0;i<3;i++){
           ret=CTOS_Beep();
           if(ret == d_OK){       //buzz three times
               CTOS_Delay(200); 
           }
           else{
               CTOS_LCDTPrintXY(2, 10, "  Buzzer test fail!");
               CTOS_KBDGet(&key);
               exit(0);
           }
       }
       CTOS_Delay(1000); 
       CTOS_LCDTPrintXY(2, 12, "Press any key.....");
       CTOS_KBDGet(&key);
       goto Main;
        
/////3./////
Flash_test:
	CTOS_LCDTClearDisplay();
        CTOS_LCDTSetReverse(TRUE);
        CTOS_LCDTPrintXY(1, 1, "  Flash test        ");
        CTOS_LCDTSetReverse(FALSE);
        while(1){
            CTOS_LCDTPrintXY(1, 6, "[1] Detect   ");
            CTOS_LCDTPrintXY(1, 7, "[X] Exit     ");
            CTOS_LCDTPrintXY(1, 9, "                    ");
            CTOS_KBDGet(&key);
            if(key ==d_KBD_CANCEL) break;
            if(key ==d_KBD_1){
                CTOS_LCDTPrintXY(1,6,"                    ");
                CTOS_LCDTPrintXY(1,7,"                    ");
                ret ==CTOS_SystemMemoryStatus(size_U,size_T,size_R,size_S);  //used disk,total disk,used ram total ram.
                if(ret ==d_OK) CTOS_LCDTPrintXY(3,9,"Test Success!");
                else CTOS_LCDTPrintXY(3,9,"Test Failed!");
                    CTOS_Delay(1000);
                }
            } 
        CTOS_KBDGet(&key);
        goto Main;

        
/////4./////
Smart_Card_test:
        i=0;
        while(1){
            CTOS_LCDTClearDisplay();
            CTOS_LCDTSetReverse(TRUE);
            CTOS_LCDTPrintXY(1, 1,"  Smart Card Test   ");
            CTOS_LCDTSetReverse(FALSE);
            CTOS_LCDTPrintXY(3, 3,"<1> ICC  Detect");
            CTOS_LCDTPrintXY(3, 4,"<2> SAM1 Detect");
            CTOS_LCDTPrintXY(3, 5,"<3> SAM2 Detect");
            CTOS_LCDTPrintXY(3, 6,"<4> SAM3 Detect");
            CTOS_LCDTPrintXY(3, 7,"<5> SAM4 Detect");
            CTOS_LCDTPrintXY(3, 9,"<x> EXIT       ");
            CTOS_KBDGet(&key);
         
        if (key == d_KBD_CANCEL) goto Main;
        CTOS_LCDTClearDisplay();
        CTOS_LCDTSetReverse(TRUE);
        CTOS_LCDTPrintXY(1, 1,"  Smart Card Test         ");
        CTOS_LCDTSetReverse(FALSE);
        if (key == d_KBD_1){
            card = 1;
            CTOS_LCDTPrintXY(1, 9, "  ICC Detecting...  ");
         }
        if (key == d_KBD_2) {
            card = 2;
            CTOS_LCDTPrintXY(1, 9, "  SAM1 Detecting... ");
            }
        if (key == d_KBD_3) {
            card = 3;
                CTOS_LCDTPrintXY(1, 9, "  SAM2 Detecting... ");
            }
        if (key == d_KBD_4) {
                card = 4;
                 CTOS_LCDTPrintXY(1, 9, "  SAM3 Detecting... ");
             }
        if (key == d_KBD_5) {
                card = 5;
                 CTOS_LCDTPrintXY(1, 9, "  SAM4 Detecting... ");
                 } 
            CTOS_Delay(500);
            CTOS_LCDTClearDisplay();
            CTOS_LCDTPrintXY(1, 1, "       Card Detect  ");
            if (card == 1) CTOS_LCDTPrintXY(3, 1, "ICC");
            if (card == 2) CTOS_LCDTPrintXY(3, 1, "SAM1");
            if (card == 3) CTOS_LCDTPrintXY(3, 1, "SAM2");
            if (card == 4) CTOS_LCDTPrintXY(3, 1, "SAM3");
            if (card == 5) CTOS_LCDTPrintXY(3, 1, "SAM4");
            CTOS_Delay(500);
            
            if(card >= 1) i=0;
            else i=1;
            
            CTOS_SCStatus(SC[(card-1)],&bStatus);
            if (bStatus & d_MK_SC_PRESENT){
                bATRLen = sizeof(baATR);
                rea = CTOS_SCResetEMV(SC[(card-1)], d_SC_5V, baATR,&bATRLen,&CardType);
                if(rea == d_OK){
                    CTOS_LCDTPrintXY(2,3,"ATR Len= ");
                    sprintf(reb,"%d",bATRLen);
                    CTOS_LCDTPrintXY(12,3,reb);
                }
                else{
                    CTOS_LCDTPrintXY(1,9,"Detect fail !!"); 
                    CTOS_Delay(500);
                    goto Smart_Card_test;
                }
                baSAPDU[0]=0x00; //CLA
                baSAPDU[1]=0x86; //INS
                baSAPDU[2]=0x00; //P1
                baSAPDU[3]=0x00; //P2    //SEND 01 0C
                baSAPDU[4]=0x08; //Le
                bSLen = 5;
                bRLen = sizeof(baRAPDU);
                rea = CTOS_SCSendAPDU(SC[(card-1)], baSAPDU, bSLen, baRAPDU, &bRLen);
                if(rea == d_OK){
                    CTOS_LCDTPrintXY(1, 9, "  Detect Success!!  ");
                    CTOS_LCDTPrintXY(1, 16, " Press OK to back...");
                    
                    while(1){
                        if(i == 0){
                            i=1;
                            CTOS_LCDTPrintXY(1, 5, " [1] Show APDU       ");
                        }
                        CTOS_KBDGet(&key);
                        if(key == d_KBD_1){
                            if(i == 1){
                                i =2;
                                CTOS_LCDTPrintXY(1, 5, " [1] Close APDU     ");
                                if(card==1){
                                    sprintf(info,"|-- SW  = %x00",baRAPDU[strlen(baRAPDU)-1]); 
                                    CTOS_LCDTPrintXY(1, 6,info);// 9000 just for detect success
                                }
                                if(card>1){
                                    sprintf(info,"|-- SW  = %x00",baRAPDU[strlen(baRAPDU)-1]); 
                                    CTOS_LCDTPrintXY(1, 6,info);// 9000 just for detect success
                                }
                                sprintf(info,"|-- ret = %04d",(int)rea);
                                CTOS_LCDTPrintXY(1, 7,info);// 0000 just for detect success
                            }
                            else{
                                i =1;
                                CTOS_LCDTPrintXY(1, 5, " [1] Show APDU      ");
                                CTOS_LCDTPrintXY(1, 6, "                    ");
                                CTOS_LCDTPrintXY(1, 7, "                    ");
                            }
                         }
                         if (key ==d_KBD_ENTER) goto Smart_Card_test;
                    }
                    }
                else CTOS_LCDTPrintXY(1, 9, "   Detect Fail!!!   ");
            }
                CTOS_Delay(1000); goto Smart_Card_test;
            }
        CTOS_Delay(1000);
        goto Smart_Card_test;
        
/////5./////
Backlight_test:        
    CTOS_LCDTClearDisplay();
    CTOS_LCDTSetReverse(TRUE);
    CTOS_LCDTPrintXY(1, 1, "   Backlight Test   ");
    CTOS_LCDTSetReverse(FALSE);
    CTOS_LCDTPrintXY(2, 3, "<1> LCD On");
    CTOS_LCDTPrintXY(2, 4, "<2> LCD Off");
    CTOS_LCDTPrintXY(2, 5, "<3> KBD On");
    CTOS_LCDTPrintXY(2, 6, "<4> KBD Off");
    CTOS_LCDTPrintXY(2, 7, "<5> Second LCD On");
    CTOS_LCDTPrintXY(2, 8, "<6> Second LCD Off");
    while(1){
        CTOS_KBDHit(&key);
        if (key == d_KBD_CANCEL)
            break;
        if (key == d_KBD_1){
            CTOS_BackLightSet(d_BKLIT_LCD, d_ON);}
        else if (key == d_KBD_2){
            CTOS_BackLightSet(d_BKLIT_LCD, d_OFF);}
        else if (key == d_KBD_3){
            CTOS_BackLightSet(d_BKLIT_KBD, d_ON);}
        else if (key == d_KBD_4){
            CTOS_BackLightSet(d_BKLIT_KBD, d_OFF);}
    }
    goto Main;
        
/////6.///// 
MSR_test:
        i=0;
	CTOS_LCDTClearDisplay();
        CTOS_LCDTSetReverse(TRUE);
        CTOS_LCDTPrintXY(1, 1, "     MSR test         ");
        CTOS_LCDTSetReverse(FALSE);
        CTOS_LCDTPrintXY(1, 9, "Please Swipe MSR.....");
        pusTk1Len=sizeof(baTk1Buf);
        pusTk2Len=sizeof(baTk2Buf);
        pusTk3Len=sizeof(baTk3Buf);
        i=0;
        while(1){
            CTOS_KBDHit(&key);
            if (key == d_KBD_CANCEL) break;
            rtn = CTOS_MSRRead(baTk1Buf, &pusTk1Len, baTk2Buf, &pusTk2Len, baTk3Buf, &pusTk3Len);
            if(rtn == d_OK && (pusTk1Len !=0  || pusTk2Len !=0 || pusTk3Len !=0)){
                //i=1;
                break;
            }
        }
        
       // if(i){
            memset(baBuff,0x00,sizeof(baBuff));
            CTOS_LCDTPrintXY(1, 9, "                          ");
            sprintf(baBuff,"Track1: %d",pusTk1Len);
            CTOS_LCDTPrintXY(2, 2, baBuff);
            sprintf(baBuff,"Track2: %d",pusTk2Len);
            CTOS_LCDTPrintXY(2, 3, baBuff);
            sprintf(baBuff,"Track3: %d",pusTk3Len);
            CTOS_LCDTPrintXY(2, 4, baBuff);
            
            CTOS_MSRGetLastErr(&baTk1Err, &baTk2Err, &baTk3Err);
            if(rtn){
                CTOS_LCDTPrintXY(2, 6,"last error:");
                sprintf(baBuff,"Track1Err: %02X",baTk1Err);
                CTOS_LCDTPrintXY(2, 7,baBuff);
                sprintf(baBuff,"Track2Err: %02X",baTk2Err);
                CTOS_LCDTPrintXY(2, 8,baBuff);
                sprintf(baBuff,"Track3Err: %02X",baTk3Err);
                CTOS_LCDTPrintXY(2, 9,baBuff);
            }
            CTOS_LCDTPrintXY(2, 16,"press any key.......");
            CTOS_KBDGet(&key);
            goto Main;
       // }
        //goto Main;

/////7./////
LED_test: 
        i=0;
	CTOS_LCDTClearDisplay();
        CTOS_LCDTSetReverse(TRUE);
        CTOS_LCDTPrintXY(1, 1, "  LED test          ");
        CTOS_LCDTSetReverse(FALSE);
        CTOS_LCDTPrintXY(2, 5, "<1> LED ON / OFF  ");   
        while(1){
            CTOS_KBDHit(&key);
            if(key == d_KBD_CANCEL){
                break;
            }
            if(key == d_KBD_1){
                if (i == 0){
                CTOS_LEDSet(d_LED1, d_ON);
                CTOS_LEDSet(d_LED2, d_ON);
                CTOS_LEDSet(d_LED3, d_ON);
                CTOS_LEDSet(d_LED4, d_ON);
                i=1;
                }
                else{
                CTOS_LEDSet(d_LED1, d_OFF);
                CTOS_LEDSet(d_LED2, d_OFF);
                CTOS_LEDSet(d_LED3, d_OFF);
                CTOS_LEDSet(d_LED4, d_OFF);
                i=0;
                }
            }
        }
        CTOS_KBDGet(&key);
        i=0;
        goto Main;
        
/////8./////
RTC_test:
	CTOS_LCDTClearDisplay();
        CTOS_LCDTSetReverse(TRUE);
        CTOS_LCDTPrintXY(1, 1, "  Real Time Clock   ");
        CTOS_LCDTSetReverse(FALSE);
        CTOS_RTCGet(&stRTC);
        
        CTOS_LCDTPrintXY(2, 2, "Date:");
        if(tz == 0){
            sprintf(time,"%02d/%02d/%02d",stRTC.bYear,stRTC.bMonth,stRTC.bDay); //load year month day to pointer time
        }
        else{
            sprintf(time,"%s%s/%s%s/%s%s", ti[0], ti[1], ti[2], ti[3], ti[4], ti[5]);
        }
        CTOS_LCDTPrintXY(8,2,time);
        CTOS_LCDTPrintXY(7,3,"(YY/MM/DD)");
        CTOS_LCDTPrintXY(2,4,"Time:");
        if(tz == 0){
        sprintf(time,"%02d:%02d:%02d",stRTC.bHour,stRTC.bMinute,stRTC.bSecond); 
        }
        else{
            sprintf(time,"%s%s:%s%s:%s%s", ti[6], ti[7], ti[8], ti[9], ti[10], ti[11]);
        }
        CTOS_LCDTPrintXY(8,4,time);
        CTOS_LCDTPrintXY(7,5,"(HH/MM/SS)");
        CTOS_LCDTPrintXY(2,6,"Week:");
        wd=stRTC.bDoW;  // MON~SUN>>1~7
        sprintf(time,"%s",week[wd]);
        CTOS_LCDTPrintXY(8,6,time);
        CTOS_LCDTPrintXY(1,12, "[0] Admin Menu");
        CTOS_LCDTPrintXY(1,13, "[x] Exit");
        
        
        ///read file
        file =fopen("/home/ap/pub/Code.txt","r");
        fscanf(file,"%s",RTCcode); 
        fclose(file);
        //fscanf(file,"%s%s%s%s%s", SD_f1, SD_f2, SD_f3, SD_f4, SD_f5); //load word into five char,every char length is 11.
        //fclose(file);  //remember to close the file after read it
        //    fprintf(file," SD Card Test!!");
        //    fclose(file);
        //ret =  CTOS_FileReopen("Code", d_STORAGE_FLASH, &cobuff);  //find default code in system file
        //colen = sizeof(RTCcode);
        //if(ret != d_OK) sprintf(RTCcode,"12345");
        //else CTOS_FileRead(cobuff, RTCcode, &colen);
       
        
        
        
        while(1){
            CTOS_KBDGet(&key);
            if(key == d_KBD_CANCEL){
               /// CTOS_FileClose(cobuff); 
                goto Main;
            }
            if(key == d_KBD_0){
                goto RTC_adm;
            }
        }
RTC_adm:
        CTOS_LCDTClearDisplay();
        CTOS_LCDTSetReverse(TRUE);
        CTOS_LCDTPrintXY(1, 1, "  RTC Admin Menu    ");
        CTOS_LCDTSetReverse(FALSE);
        CTOS_LCDTPrintXY(3, 4, "[1] Set RTC");
        CTOS_LCDTPrintXY(3, 5, "[2] Change PIN");
        CTOS_LCDTPrintXY(3, 7, "[X] Back");
        //CTOS_LCDTPrintXY(3,10, RTCcode);
        i = 0;
        while(1){
            CTOS_KBDGet(&key);
            if(key == d_KBD_CANCEL) goto RTC_test;
            if (key == d_KBD_1) i = 1;
            if (key == d_KBD_2) i = 2;
            if(i != 0) goto RTC_chk;
        }
        
RTC_chk:
        
        if(i == 1){
            CTOS_LCDTClearDisplay();
            CTOS_LCDTPrintXY(1,1,"  RE-Setting RTC ");
            CTOS_LCDTPrintXY(2,7, "Enter PIN:");
            CTOS_LCDTPrintXY(2,8, "> ");
        }
        if(i == 2){
            CTOS_LCDTClearDisplay();
            CTOS_LCDTPrintXY(1,1,"  Change PIN code ");
            CTOS_LCDTPrintXY(2,7, "Enter Old PIN:");
            CTOS_LCDTPrintXY(2,8, "> ");
        }
        memset(baBUFF,0x00,sizeof(baBUFF));  //reset input code
        CTOS_UIKeypad(4, 8, keyboardLayoutNumber, 40, 80, d_TRUE, d_FALSE, 0, '*', baBUFF, 9); //max 8 input number
        tb =0;  ////bug find!!!!!!!!!!!
        if ( strcmp(baBUFF,RTCcode) !=0 ) tb = 99;  //compare check....
        /*else{                                      
            for(tb ==0;tb < strlen(baBUFF); tb++){
                if(baBUFF[tb] != RTCcode[tb]){
                    tb = 99;
                }
            }
        }
        */
         if (tb == 99){
        CTOS_LCDTPrintXY(2, 11, "Code Not Match!!");
        CTOS_LCDTPrintXY(1, 12, "____________________");
        CTOS_LCDTPrintXY(1, 14, "  [Any Key] Retry");
        CTOS_LCDTPrintXY(1, 15, "  [   X   ] Leave");
        CTOS_KBDGet(&key);
        if (key == d_KBD_CANCEL)  goto RTC_adm; 
        else goto RTC_chk;
         }
        else{
        CTOS_LCDTPrintXY(2, 11, "Input Success!!"); CTOS_Delay(250);
        CTOS_LCDTPrintXY(1, 11, "                    ");
        CTOS_LCDTPrintXY(1, 12, "                    ");
        CTOS_LCDTPrintXY(1, 14, "                    ");
        CTOS_LCDTPrintXY(1, 15, "                    ");
        CTOS_Delay(500); if (i == 1) goto RTC_set; if (i == 2) goto RTC_pin;
        }  
RTC_set:
        CTOS_LCDTClearDisplay(); 
        CTOS_LCDTPrintXY(1, 1, "     Setting RTC    ");
        CTOS_LCDTPrintXY(2, 2, "Date: ");
        CTOS_LCDTPrintXY(10, 2, "/");
        CTOS_LCDTPrintXY(13, 2, "/");
        CTOS_LCDTPrintXY(2, 4, "Time: ");
        CTOS_LCDTPrintXY(10, 4, ":");
        CTOS_LCDTPrintXY(13, 4, ":"); 
        tx = 8, ty = 2, tz = 0;
        
        //can use better method
        while(1){
        if (tz < 12){
            if (tx == 10 || tx == 13) tx++; 
            if (tx == 16){
                ty += 2; tx = 8;
            }
            CTOS_LCDTPrintXY(tx,ty, "_");
        } 
        CTOS_KBDGet(&key);
        if (tz < 12){
            if (key == d_KBD_0){
                CTOS_LCDTPrintXY(tx, ty, "0"); ti[tz] = "0"; tz++; tx++;}
            if (key == d_KBD_1){
                CTOS_LCDTPrintXY(tx, ty, "1"); ti[tz] = "1"; tz++; tx++;}
            if (key == d_KBD_2){
                CTOS_LCDTPrintXY(tx, ty, "2"); ti[tz] = "2"; tz++; tx++;}
            if (key == d_KBD_3){
                CTOS_LCDTPrintXY(tx, ty, "3"); ti[tz] = "3"; tz++; tx++;}
            if (key == d_KBD_4){
                CTOS_LCDTPrintXY(tx, ty, "4"); ti[tz] = "4"; tz++; tx++;}
            if (key == d_KBD_5){
                CTOS_LCDTPrintXY(tx, ty, "5"); ti[tz] = "5"; tz++; tx++;}
            if (key == d_KBD_6){
                CTOS_LCDTPrintXY(tx, ty, "6"); ti[tz] = "6"; tz++; tx++;}
            if (key == d_KBD_7){
                CTOS_LCDTPrintXY(tx, ty, "7"); ti[tz] = "7"; tz++; tx++;}
            if (key == d_KBD_8){
                CTOS_LCDTPrintXY(tx, ty, "8"); ti[tz] = "8"; tz++; tx++;}
            if (key == d_KBD_9){
                CTOS_LCDTPrintXY(tx, ty, "9"); ti[tz] = "9"; tz++; tx++;}
        }
        if (key == d_KBD_CLEAR){
            if (tx == 9 || tx == 12 || tx == 15){
                CTOS_LCDTPrintXY(tx, ty, " "); tx--;}
            else if (tx == 11 || tx == 14){
                CTOS_LCDTPrintXY(tx, ty, " "); tx -= 2;}
            else if (tx == 8 && ty == 4){
                CTOS_LCDTPrintXY(tx, ty, " "); tx = 15; ty = 2;}
            else if (tx == 16 && ty == 4) tx--; if (tz != 0) tz--;}
        if (key == d_KBD_CANCEL){
            CTOS_LCDTPrintXY(1, 9, " Set Time Fail!! ");
            CTOS_Delay(500); 
            tz = 0; 
            goto RTC_test;
        }
        if (key == d_KBD_ENTER){
            if (tz < 12){
                CTOS_LCDTPrintXY(1, 9, " Setting not finish."); CTOS_Delay(500);
                CTOS_LCDTPrintXY(1, 9, "                    ");
            }
            else{
                stRTC.bYear =((atoi(ti[0])*10)+atoi(ti[1]));
                stRTC.bMonth =((atoi(ti[2])*10)+atoi(ti[3]));
                stRTC.bDay =((atoi(ti[4])*10)+atoi(ti[5]));
                stRTC.bHour =((atoi(ti[6])*10)+atoi(ti[7]));
                stRTC.bMinute =((atoi(ti[8])*10)+atoi(ti[9]));
                stRTC.bSecond =((atoi(ti[10])*10)+atoi(ti[11]));
                CTOS_RTCSet(&stRTC);
                CTOS_LCDTPrintXY(1, 9, " Set Time Success!! "); CTOS_Delay(500);
                goto RTC_test;
            }
        }
    } 
RTC_pin:
     CTOS_LCDTPrintXY(2, 7, "Enter New PIN:");
     CTOS_UIKeypad(4, 8, keyboardLayoutNumber, 40, 80,d_TRUE, d_FALSE, 0, 0, baBUFF, 9);
     CTOS_LCDTPrintXY(1, 11, "  Update PIN Code?  ");
     CTOS_LCDTPrintXY(1, 12, "____________________");
     CTOS_LCDTPrintXY(1, 14, "    [OK] Yes");
     CTOS_LCDTPrintXY(1, 15, "    [X]  Cancel"); CTOS_KBDGet(&key);
     if (key == d_KBD_CANCEL){
        CTOS_LCDTPrintXY(1, 11, " Return to Main...  ");
        CTOS_LCDTPrintXY(1, 12, "                    ");
        CTOS_LCDTPrintXY(1, 14, "                    ");
        CTOS_LCDTPrintXY(1, 15, "                    "); 
        CTOS_Delay(500);
     }
    if (key == d_KBD_ENTER){
        memset(RTCcode,0x00,sizeof(RTCcode));  //erease code to 0x00
        strcpy(RTCcode,baBUFF);
        
        file =fopen("/home/ap/pub/Code.txt","w");
        fprintf(file,"                  ");
        fprintf(file,"%s",RTCcode);
        fclose(file);
        //for (tb = 0; tb < strlen(baBUFF); tb++) {
        //    RTCcode [tb] = baBUFF[tb];}
        
        //if (CTOS_FileReopen("Code", d_STORAGE_FLASH, &cobuff) != d_OK){
        //    CTOS_FileOpen("Code", d_STORAGE_FLASH, &cobuff);
        //}
        
        //CTOS_FileWrite(cobuff, RTCcode, sizeof(RTCcode));
        CTOS_LCDTPrintXY(1, 11, " Change Success!!   ");
        CTOS_LCDTPrintXY(1, 12, "                    ");
        CTOS_LCDTPrintXY(1, 14, "                    ");
        CTOS_LCDTPrintXY(1, 15, "                    ");
        CTOS_Delay(500);
    }
    goto RTC_test;

/////9./////
Printer:
        k=0;
	CTOS_LCDTClearDisplay();
        CTOS_LCDTSetReverse(TRUE);
        CTOS_LCDTPrintXY(1,1,"   Printer Test            ");
        CTOS_LCDTSetReverse(FALSE);
        CTOS_LCDTPrintXY(1,3,"[1] Block");
        CTOS_LCDTPrintXY(1,4,"[2] Basic Info");
        CTOS_LCDTPrintXY(1,5,"[3] Ethernet Info");
        CTOS_LCDTPrintXY(1,6,"[4] IMEI Info");
        CTOS_LCDTPrintXY(1,7,"[5] All");
        CTOS_LCDTPrintXY(1,9,"[X] Exit");
        
        while(1){
            CTOS_KBDGet(&key);
            if(key ==d_KBD_CANCEL) pa=0;
            if(key ==d_KBD_1) pa=1;
            if(key ==d_KBD_2) pa=2;
            if(key ==d_KBD_3) pa=3;
            if(key ==d_KBD_4) pa=4;
            if(key ==d_KBD_5) pa=5;
            if(pa !=0){
                memset(baTemp,0x00,sizeof(baTemp));
                CTOS_LCDTClearDisplay;
                CTOS_LCDTPrintXY(1,9,"Ready to Print...    ");
                CTOS_Delay(1000);
                CTOS_LCDTPrintXY(1,9,"Starting Print...     ");
                CTOS_Delay(500);
                CTOS_LCDTPrintXY(1,9,"                 ");
                //pk=0;
                if(pa ==1 || pa==5) goto PR_Block;
                if(pa ==2) goto PR_BasicInfo;
                if(pa ==3) goto PR_EthernetInfo;
                if(pa ==4) goto PR_IMEIInfo;
            }
            else goto Main;
        }
        
PR_Block:
        //LCD display
        CTOS_LCDTClearDisplay();
        CTOS_RTCGet(&Pr_time);
        sprintf(pt,"20%02d/%02d/%02d %02d:%02d:%02d",Pr_time.bYear,Pr_time.bMonth,Pr_time.bDay,Pr_time.bHour,Pr_time.bMinute,Pr_time.bSecond);
        CTOS_LCDTPrintXY(1,1,pt);
        CTOS_LCDTPrintXY(1,2,"VEGA Series Terminal");

        //print on paper
        CTOS_PrinterBufferInit((BYTE *)baTemp,8*48);  //baTemp address,height of buffer
        CTOS_PrinterBufferSelectActiveAddress(baTemp);
        CTOS_PrinterFline(2); //roll the paper without printing
        CTOS_PrinterPutString(pt); //print time
        CTOS_PrinterPutString("VEGA Series Terminal");
        CTOS_PrinterFline(1);
        CTOS_PrinterBufferFill(0,0,383,60,1); //set pixel full of the block
        for(j=0;j<5;j++){                   //print lines
            for(ppy=0;ppy<65;ppy++){
                ppx=ppy;
                for(i=0;i<7;i++){
                    CTOS_PrinterBufferPixel(ppx,ppy+64*j,1);
                    CTOS_PrinterBufferPixel(ppx,ppy+1+64*j,1);
                    CTOS_PrinterBufferPixel(ppx+1,ppy+64*j,1);
                    CTOS_PrinterBufferPixel(ppx+1,ppy+1+64*j,1);
                    ppx=ppx+64;
                }
            }
        }
        CTOS_PrinterBufferOutput((BYTE *)baTemp, 8*5); 
        
        while(1){
        if(pa ==5) {
            CTOS_Delay(100);
            goto PR_BasicInfo;}
        else {
            CTOS_PrinterFline(70);
            CTOS_LCDTPrintXY(1,5,"Print Finish.");
            CTOS_LCDTPrintXY(1,6,"Press Any Key.....");
            CTOS_KBDGet(&key);
            goto Printer;}
        }
        
PR_BasicInfo:
       /////////////////////////LCD display////////////
        CTOS_LCDTClearDisplay();
        CTOS_RTCGet(&Pr_time);
        sprintf(pt,"20%02d/%02d/%02d %02d:%02d:%02d",Pr_time.bYear,Pr_time.bMonth,Pr_time.bDay,Pr_time.bHour,Pr_time.bMinute,Pr_time.bSecond);
        CTOS_LCDTPrintXY(1,1,pt);
        CTOS_LCDTPrintXY(1,2,"VEGA Series Terminal");
        //////////////////////print on paper///////////
        //CTOS_PrinterFline(2); //roll the paper without printing
        if(pa !=5){
        CTOS_PrinterPutString(pt); //print time
        CTOS_PrinterPutString("VEGA Series Terminal");
        CTOS_PrinterFline(1);
        }
        /////////////Serial Number////////////////
        CTOS_LCDTPrintXY(1,3,"[System Info]");
        memset(syb,0x00,sizeof(syb));   //set syb[] zero
        CTOS_LCDTPrintXY(1,4,"SN        :");
        CTOS_GetFactorySN(snb); //get the terminal back serial number
        for(i=0;i<15;i++){
            syb[i]=snb[i];
        }
        CTOS_LCDTPrintXY(13,4,syb);
        /////////////Serial Number print out//////////
        CTOS_PrinterPutString(" [System Info]");
        sprintf(snb,"SN        :%s",syb);
        CTOS_PrinterPutString(snb);
        ////////////Kernel bios Version///////////
        CTOS_LCDTPrintXY(1,5,"Kernal Version");
        memset(sdb,0x00,sizeof(sdb));
        CTOS_LCDTPrintXY(1,6,"BIOS      :");
        CTOS_GetSystemInfo(ID_BIOS, syb);
        for(i=0;i<6;i++){
            sdb[i]=syb[i];
        }
        CTOS_LCDTPrintXY(13,6, sdb);
        /////Kernel bios Version print out//////////////
        CTOS_PrinterPutString("Kernal Version->");
        sprintf(snb,"BIOS      :%s",sdb);
        CTOS_PrinterPutString(snb);
        ////////////Kernel SULD//////////////////////////////////////////////
        CTOS_LCDTPrintXY(1,7,"SULD      :");
        CTOS_GetSystemInfo(ID_BOOTSULD,syb);
        for(i=0;i<6;i++){
            sdb[i]=syb[i];
        }
        CTOS_LCDTPrintXY(13,7,sdb);
       /////Kernel SULD print out//////////
        sprintf(snb,"SULD      :%s",sdb);
        CTOS_PrinterPutString(snb);
        ////////////Kernel LINX//////////////////////////////////////////////
        CTOS_LCDTPrintXY(1,8,"LINUXKNL  :");
        CTOS_GetSystemInfo(ID_LINUX_KERNEL,syb);
        for(i=0;i<6;i++){
            sdb[i]=syb[i];
        }
        CTOS_LCDTPrintXY(13,8,sdb);
       /////Kernel LINX print out//////////
        sprintf(snb,"LINUXKNL  :%s",sdb);
        CTOS_PrinterPutString(snb);
        ////////////Kernel RootFS//////////////////////////////////////////////
        CTOS_LCDTPrintXY(1,9,"RootFS    :");
        CTOS_GetSystemInfo(ID_ROOTFS,syb);
        for(i=0;i<6;i++){
            sdb[i]=syb[i];
        }
        CTOS_LCDTPrintXY(13,9,sdb);
       /////Kernel RootFS print out//////////
        sprintf(snb,"RootFS    :%s",sdb);
        CTOS_PrinterPutString(snb);
        ////////////KO-SECRITY ////////////////////////////////////////////////////////
        CTOS_LCDTPrintXY(1,10,"KO Version->");
        CTOS_LCDTPrintXY(1,11,"SECURITY  :");
        CTOS_GetSystemInfo(ID_SECURITY_KO,syb);
        for(i=0;i<6;i++){
            sdb[i]=syb[i];
        }
        CTOS_LCDTPrintXY(13,11,sdb);
       /////KO-SECRITY print out//////////
        CTOS_PrinterPutString("KO Version->");
        sprintf(snb,"SECURITY  :%s",sdb);
        CTOS_PrinterPutString(snb);
        ////////////KO-KMS //////////////////////////////////////////////
        CTOS_LCDTPrintXY(1,12,"KMS       :");
        CTOS_GetSystemInfo(ID_KMS,syb);
        for(i=0;i<6;i++){
            sdb[i]=syb[i];
        }
        CTOS_LCDTPrintXY(13,12,sdb);
       /////KO-KMS print out//////////
        sprintf(snb,"KMS       :%s",sdb);
        CTOS_PrinterPutString(snb);
        ////////////KO-DRV //////////////////////////////////////////////
        CTOS_LCDTPrintXY(1,13,"DRV       :");
        CTOS_GetSystemInfo(ID_CADRV_KO,syb);
        for(i=0;i<6;i++){
            sdb[i]=syb[i];
        }
        CTOS_LCDTPrintXY(13,13,sdb);
       /////KO-DRV print out//////////
        sprintf(snb,"DRV       :%s",sdb);
        CTOS_PrinterPutString(snb);
        ////////////KO-USB //////////////////////////////////////////////
        CTOS_LCDTPrintXY(1,14,"USB       :");
        ret =CTOS_GetSystemInfo(ID_CAUSB_KO,syb);
        for(i=0;i<6;i++){
            sdb[i]=syb[i];
        }
        if(ret !=d_OK){
            sprintf(sdb,"N/A");
        }
        CTOS_LCDTPrintXY(13,14,sdb);
       /////KO-USB print out//////////
        sprintf(snb,"USB       :%s",sdb);
        CTOS_PrinterPutString(snb);
        ////////////KO-CIF //////////////////////////////////////////////
        CTOS_LCDTPrintXY(1,15,"CIF       :");
        CTOS_GetSystemInfo(ID_CIF_KO,syb);
        for(i=0;i<6;i++){
            sdb[i]=syb[i];
        }
        CTOS_LCDTPrintXY(13,15,sdb);
       /////KO-CIF print out//////////
        sprintf(snb,"CIF       :%s",sdb);
        CTOS_PrinterPutString(snb);
        ////////////KO-SAM //////////////////////////////////////////////
        CTOS_LCDTPrintXY(1,16,"SAM       :");  ///
        CTOS_GetSystemInfo(ID_SAM_KO,syb);
        for(i=0;i<6;i++){
            sdb[i]=syb[i];
        }
        CTOS_LCDTPrintXY(13,16,sdb);
       /////KO-SAM print out//////////
        sprintf(snb,"SAM       :%s",sdb);
        CTOS_PrinterPutString(snb);
        
        //if(pk==0) pk=1;
        
        while(1){
            if(pa ==2 || pa ==5){
                CTOS_Delay(500);
                //CTOS_KBDGet(&key);
                goto PR_Basic1;
            }
            else{
                goto Printer;
                //TOS_KBDGet(&key);
                //if(key ==d_KBD_CANCEL) goto Printer;
                //else goto PR_Basic1;
            }
        }
        
PR_Basic1:    
        CTOS_LCDTClearDisplay();
        ////////////KO-CL //////////////////////////////////////////////
        CTOS_LCDTPrintXY(1,1,"CL        :");  ///
        CTOS_GetSystemInfo(ID_CLDRV_KO,syb);
        for(i=0;i<6;i++){
            sdb[i]=syb[i];
        }
        CTOS_LCDTPrintXY(13,1,sdb);
       /////KO-CL print out//////////
        sprintf(snb,"CL        :%s",sdb);
        CTOS_PrinterPutString(snb);
        
        ////////////KO-SC //////////////////////////////////////////////
        CTOS_LCDTPrintXY(1,2,"SC        :");  ///
        CTOS_GetSystemInfo(ID_SC_KO,syb);
        for(i=0;i<6;i++){
            sdb[i]=syb[i];
        }
        CTOS_LCDTPrintXY(13,2,sdb);
       /////KO-SC print out//////////
        sprintf(snb,"SC        :%s",sdb);
        CTOS_PrinterPutString(snb);

        ///SO VERSION
        CTOS_LCDTPrintXY(1,3,"SO Version->");
        CTOS_PrinterPutString("SO Version->");
        ////////////SO-UART //////////////////////////////////////////////
        CTOS_LCDTPrintXY(1,4,"UART      :");  ///
        CTOS_GetSystemInfo(ID_LIBCAUART_SO,syb);
        for(i=0;i<6;i++){
            sdb[i]=syb[i];
        }
        CTOS_LCDTPrintXY(13,4,sdb);
       /////SO-UART print out//////////
        sprintf(snb,"UART      :%s",sdb);
        CTOS_PrinterPutString(snb);
        
        ////////////SO-USBH //////////////////////////////////////////////
        CTOS_LCDTPrintXY(1,5,"USBH      :");  ///
        CTOS_GetSystemInfo(ID_LIBCAUSBH_SO,syb);
        for(i=0;i<6;i++){
            sdb[i]=syb[i];
        }
        CTOS_LCDTPrintXY(13,5,sdb);
       /////SO-USBH print out//////////
        sprintf(snb,"USBH      :%s",sdb);
        CTOS_PrinterPutString(snb);
        
        ////////////SO-MODEM //////////////////////////////////////////////
        CTOS_LCDTPrintXY(1,6,"MODEM     :");  ///
        CTOS_GetSystemInfo(ID_LIBCAMODEM_SO,syb);
        for(i=0;i<6;i++){
            sdb[i]=syb[i];
        }
        CTOS_LCDTPrintXY(13,6,sdb);
       /////SO-MODEM print out//////////
        sprintf(snb,"MODEM     :%s",sdb);
        CTOS_PrinterPutString(snb);
        
        ////////////SO-Ethernet //////////////////////////////////////////////
        CTOS_LCDTPrintXY(1,7,"ETHERNET  :");  ///
        CTOS_GetSystemInfo(ID_LIBCAETHERNET_SO,syb);
        for(i=0;i<6;i++){
            sdb[i]=syb[i];
        }
        CTOS_LCDTPrintXY(13,7,sdb);
       /////SO-Ethernet print out//////////
        sprintf(snb,"ETHERNET  :%s",sdb);
        CTOS_PrinterPutString(snb);
        
        ////////////SO-Font //////////////////////////////////////////////
        CTOS_LCDTPrintXY(1,8,"Font      :");  ///
        CTOS_GetSystemInfo(ID_LIBCAFONT_SO,syb);
        for(i=0;i<6;i++){
            sdb[i]=syb[i];
        }
        CTOS_LCDTPrintXY(13,8,sdb);
       /////SO-Font print out//////////
        sprintf(snb,"Font      :%s",sdb);
        CTOS_PrinterPutString(snb);
        
        
        ////////////SO-LCD //////////////////////////////////////////////
        CTOS_LCDTPrintXY(1,9,"LCD       :");  ///
        CTOS_GetSystemInfo(ID_LIBCALCD_SO,syb);
        for(i=0;i<6;i++){
            sdb[i]=syb[i];
        }
        CTOS_LCDTPrintXY(13,9,sdb);
       /////SO-Font print out//////////
        sprintf(snb,"LCD       :%s",sdb);
        CTOS_PrinterPutString(snb);
        
        ////////////SO-PRT //////////////////////////////////////////////
        CTOS_LCDTPrintXY(1,10,"PRT       :");  ///
        CTOS_GetSystemInfo(ID_LIBCAPRT_SO,syb);
        for(i=0;i<6;i++){
            sdb[i]=syb[i];
        }
        CTOS_LCDTPrintXY(13,10,sdb);
       /////SO-PRT//////////
        sprintf(snb,"PRT       :%s",sdb);
        CTOS_PrinterPutString(snb);
        
        ////////////SO-RTC //////////////////////////////////////////////
        CTOS_LCDTPrintXY(1,11,"RTC       :");  ///
        CTOS_GetSystemInfo(ID_LIBCARTC_SO,syb);
        for(i=0;i<6;i++){
            sdb[i]=syb[i];
        }
        CTOS_LCDTPrintXY(13,11,sdb);
       /////SO-RTC  print out//////////
        sprintf(snb,"RTC       :%s",sdb);
        CTOS_PrinterPutString(snb);
        
        ////////////SO-ULDPM //////////////////////////////////////////////
        CTOS_LCDTPrintXY(1,12,"ULDPM     :");  ///
        CTOS_GetSystemInfo(ID_LIBCAULDPM_SO,syb);
        for(i=0;i<6;i++){
            sdb[i]=syb[i];
        }
        CTOS_LCDTPrintXY(13,12,sdb);
       /////SO-Font print out//////////
        sprintf(snb,"ULDPM     :%s",sdb);
        CTOS_PrinterPutString(snb);
        
        ////////////SO-PPP Modem //////////////////////////////////////////////
        CTOS_LCDTPrintXY(1,13,"PPPModem  :");  ///
        CTOS_GetSystemInfo(ID_LIBCAPMODEM_SO,syb);
        for(i=0;i<6;i++){
            sdb[i]=syb[i];
        }
        CTOS_LCDTPrintXY(13,13,sdb);
       /////SO-PPP Modem print out//////////
        sprintf(snb,"PPPModem  :%s",sdb);
        CTOS_PrinterPutString(snb);
        
        ////////////SO-KMS //////////////////////////////////////////////
        CTOS_LCDTPrintXY(1,14,"KMS       :");  ///
        CTOS_GetSystemInfo(ID_LIBCAKMS_SO,syb);
        for(i=0;i<6;i++){
            sdb[i]=syb[i];
        }
        CTOS_LCDTPrintXY(13,14,sdb);
       /////SO-KMS print out//////////
        sprintf(snb,"KMS       :%s",sdb);
        CTOS_PrinterPutString(snb);
        
        ////////////SO-FS //////////////////////////////////////////////
        CTOS_LCDTPrintXY(1,15,"FS        :");  ///
        CTOS_GetSystemInfo(ID_LIBCAFS_SO,syb);
        for(i=0;i<6;i++){
            sdb[i]=syb[i];
        }
        CTOS_LCDTPrintXY(13,15,sdb);
       /////SO-FS print out//////////
        sprintf(snb,"FS        :%s",sdb);
        CTOS_PrinterPutString(snb);
        
        ////////////SO-GSM //////////////////////////////////////////////
        CTOS_LCDTPrintXY(1,16,"GSM       :");  ///
        CTOS_GetSystemInfo(ID_LIBCAGSM_SO,syb);
        for(i=0;i<6;i++){
            sdb[i]=syb[i];
        }
        CTOS_LCDTPrintXY(13,16,sdb);
       /////SO-GSM print out//////////
        sprintf(snb,"GSM       :%s",sdb);
        CTOS_PrinterPutString(snb);
        
        while(1){
            if(pa ==2 || pa ==5){
                //CTOS_KBDGet(&key);
                CTOS_Delay(500);
                goto PR_Basic2;
            }
            else{
                //CTOS_KBDGet(&key);
                //if(key ==d_KBD_CANCEL) goto Printer;
                //else goto PR_Basic2;
                goto Printer;
            }
        }
        
 PR_Basic2:
        CTOS_LCDTClearDisplay();
        
        ////////////SO-Barcode //////////////////////////////////////////////
        CTOS_LCDTPrintXY(1,1,"Barcode   :");  ///
        CTOS_GetSystemInfo(ID_LIBCABARCODE_SO,syb);
        for(i=0;i<6;i++){
            sdb[i]=syb[i];
        }
        CTOS_LCDTPrintXY(13,1,sdb);
       /////SO-Barcode print out//////////
        sprintf(snb,"Barcode   :%s",sdb);
        CTOS_PrinterPutString(snb);
        
        
        ////////////SO-TMS //////////////////////////////////////////////
        CTOS_LCDTPrintXY(1,2,"TMS       :");  ///
        CTOS_GetSystemInfo(ID_TMS,syb);
        for(i=0;i<6;i++){
            sdb[i]=syb[i];
        }
        CTOS_LCDTPrintXY(13,2,sdb);
       /////SO-TMS print out//////////
        sprintf(snb,"TMS       :%s",sdb);
        CTOS_PrinterPutString(snb);
        
        ////////////SO-TLS //////////////////////////////////////////////
        CTOS_LCDTPrintXY(1,3,"TLS       :");  ///
        CTOS_GetSystemInfo(ID_LIBTLS_SO,syb);
        for(i=0;i<6;i++){
            sdb[i]=syb[i];
        }
        CTOS_LCDTPrintXY(13,3,sdb);
       /////SO-TLS print out//////////
        sprintf(snb,"TLS       :%s",sdb);
        CTOS_PrinterPutString(snb);
        
        ////////////SO-CLVW //////////////////////////////////////////////
        CTOS_LCDTPrintXY(1,4,"CLVW      :");  ///
        CTOS_GetSystemInfo(ID_LIBCLVW_SO,syb);
        for(i=0;i<6;i++){
            sdb[i]=syb[i];
        }
        CTOS_LCDTPrintXY(13,4,sdb);
       /////SO-CLVW print out//////////
        sprintf(snb,"CLVW      :%s",sdb);
        CTOS_PrinterPutString(snb);
        
        ////////////SO-CTOSAPI //////////////////////////////////////////////
        CTOS_LCDTPrintXY(1,5,"CTOSAPI   :");  ///
        CTOS_GetSystemInfo(ID_LIBCTOSAPI_SO,syb);
        for(i=0;i<6;i++){
            sdb[i]=syb[i];
        }
        CTOS_LCDTPrintXY(13,5,sdb);
       /////SO-CTOSAPI print out//////////
        sprintf(snb,"CTOSAPI   :%s",sdb);
        CTOS_PrinterPutString(snb);
        
        CTOS_PrinterFline(4);
        
        /////////////Plug-in Version//////////////////////////
        CTOS_LCDTPrintXY(1,6,"Plug-in Version");
        CTOS_PrinterPutString("Plug-in Version ->");
        ////////////Plug-in Bluetooth//////////////////////////////////////////////
        CTOS_LCDTPrintXY(1,7,"Bluetooth :");  ///
        ret =CTOS_GetSystemInfo(ID_PLUGIN_BT,syb);
        for(i=0;i<6;i++){
            sdb[i]=syb[i];
        }
        if (sdb[0] =='\0'){
            sprintf(sdb,"N/A");
        }
        CTOS_LCDTPrintXY(13,7,sdb);
       /////Plug-in Bluetooth print out//////////
        sprintf(snb,"Bluetooth :%s",sdb);
        CTOS_PrinterPutString(snb);
        CTOS_PrinterPutString("QT        :N/A");
        
        
        CTOS_PrinterFline(2);
        /////////////EMV EXT Version//////////////////////////
        CTOS_LCDTPrintXY(1,8,"EMV Ext Version");
        CTOS_PrinterPutString("EMV Ext Version ->");
        
        ////////////EMV Ext-CLMDL //////////////////////////////////////////////
        CTOS_LCDTPrintXY(1,9,"CACLMDL   :");  ///
        CTOS_GetSystemInfo(ID_EXTLIBCACLMDL_SO,syb);
        for(i=0;i<6;i++){
            sdb[i]=syb[i];
        }
        CTOS_LCDTPrintXY(13,9,sdb);
       /////EMV Ext-CLMDL print out//////////
        sprintf(snb,"CACLMDL   :%s",sdb);
        CTOS_PrinterPutString(snb);
        
        ////////////EMV Ext-CLENTRY //////////////////////////////////////////////
        CTOS_LCDTPrintXY(1,10,"CACLENTRY :");  ///
        CTOS_GetSystemInfo(ID_EXTLIBCACLENTRY_SO,syb);
        for(i=0;i<6;i++){
            sdb[i]=syb[i];
        }
        CTOS_LCDTPrintXY(13,10,sdb);
       /////EMV Ext-CLENTRY print out//////////
        sprintf(snb,"CACLENTRY :%s",sdb);
        CTOS_PrinterPutString(snb);
          
        ////////////EMV Ext-CAMPP //////////////////////////////////////////////
        CTOS_LCDTPrintXY(1,11,"CAMPP     :");  ///
        CTOS_GetSystemInfo(ID_EXTLIBCAMPP_SO,syb);
        for(i=0;i<6;i++){
            sdb[i]=syb[i];
        }
        CTOS_LCDTPrintXY(13,11,sdb);
       /////EMV Ext-CAMPP print out//////////
        sprintf(snb,"CAMPP     :%s",sdb);
        CTOS_PrinterPutString(snb);
        
        
        
        
        ////////////EMV Ext-CAVPW //////////////////////////////////////////////
        CTOS_LCDTPrintXY(1,12,"CAVPW     :");  ///
        CTOS_GetSystemInfo(ID_EXTLIBCAVPW_SO,syb);
        for(i=0;i<6;i++){
            sdb[i]=syb[i];
        }
        CTOS_LCDTPrintXY(13,12,sdb);
       /////EMV Ext-CAVPW print out//////////
        sprintf(snb,"CAVPW     :%s",sdb);
        CTOS_PrinterPutString(snb);
        
        
        ////////////EMV Ext-CAAEP //////////////////////////////////////////////
        CTOS_LCDTPrintXY(1,13,"CAAEP     :");  ///
        CTOS_GetSystemInfo(ID_EXTLIBCAAEP_SO,syb);
        for(i=0;i<6;i++){
            sdb[i]=syb[i];
        }
        CTOS_LCDTPrintXY(13,13,sdb);
       /////EMV Ext-CAAEP print out//////////
        sprintf(snb,"CAAEP     :%s",sdb);
        CTOS_PrinterPutString(snb);
        
        
        ////////////EMV Ext-CAJCT //////////////////////////////////////////////
        CTOS_LCDTPrintXY(1,14,"CAJCT     :");  ///
        CTOS_GetSystemInfo(ID_EXTLIBCAJCT_SO,syb);
        for(i=0;i<6;i++){
            sdb[i]=syb[i];
        }
        CTOS_LCDTPrintXY(13,14,sdb);
       /////EMV Ext-CAJCT print out//////////
        sprintf(snb,"CAJCT     :%s",sdb);
        CTOS_PrinterPutString(snb);
        
        ////////////EMV Ext-CAVAP //////////////////////////////////////////////
        CTOS_LCDTPrintXY(1,15,"CAVAP     :");  ///
        CTOS_GetSystemInfo(ID_EXTLIBCAVAP_SO,syb);
        for(i=0;i<6;i++){
            sdb[i]=syb[i];
        }
        CTOS_LCDTPrintXY(13,15,sdb);
       /////EMV Ext-CAVAP print out//////////
        sprintf(snb,"CAVAP     :%s",sdb);
        CTOS_PrinterPutString(snb);
        
        
        while(1){
            if(pa ==2 || pa ==5) {
                CTOS_Delay(500);
                //CTOS_KBDGet(&key);
                goto PR_Basic3;
            }
            else{
                //CTOS_KBDGet(&key);
                //if(key ==d_KBD_CANCEL) goto Printer; 
                //else goto PR_Basic3;
                goto Printer;
            }
        }

PR_Basic3:
        CTOS_LCDTClearDisplay();
        ////////////EMV Ext-CACQP //////////////////////////////////////////////
        CTOS_LCDTPrintXY(1,1,"CACQP     :");  ///
        CTOS_GetSystemInfo(ID_EXTLIBCACQP_SO,syb);
        for(i=0;i<6;i++){
            sdb[i]=syb[i];
        }
        CTOS_LCDTPrintXY(13,1,sdb);
       /////EMV Ext-CACQP print out//////////
        sprintf(snb,"CACQP     :%s",sdb);
        CTOS_PrinterPutString(snb);
        
        ////////////EMV Ext-CAIFH //////////////////////////////////////////////
        CTOS_LCDTPrintXY(1,2,"CAIFH     :");  ///
        CTOS_GetSystemInfo(ID_EXTLIBCAIFH_SO,syb);
        for(i=0;i<6;i++){
            sdb[i]=syb[i];
        }
        CTOS_LCDTPrintXY(13,2,sdb);
       /////EMV Ext-CAIFH print out//////////
        sprintf(snb,"CAIFH     :%s",sdb);
        CTOS_PrinterPutString(snb);
        
        
        ////////////EMV Ext-CADDP //////////////////////////////////////////////
        CTOS_LCDTPrintXY(1,3,"CADDP     :");  ///
        CTOS_GetSystemInfo(ID_EXTLIBCADDP_SO,syb);
        for(i=0;i<6;i++){
            sdb[i]=syb[i];
        }
        CTOS_LCDTPrintXY(13,3,sdb);
       /////EMV Ext-CADDP print out//////////
        sprintf(snb,"CADDP     :%s",sdb);
        CTOS_PrinterPutString(snb);
        
        ////////////EMV Ext-CAEMVL2 //////////////////////////////////////////////
        CTOS_LCDTPrintXY(1,4,"CAEMVL2   :");  ///
        CTOS_GetSystemInfo(ID_EXTLIBCAEMVL2_SO,syb);
        for(i=0;i<6;i++){
            sdb[i]=syb[i];
        }
        CTOS_LCDTPrintXY(13,4,sdb);
       /////EMV Ext-CAEMEL2 print out//////////
        sprintf(snb,"CAEMVL2   :%s",sdb);
        CTOS_PrinterPutString(snb);
        
        ////////////EMV Ext-CAEMVL2AP //////////////////////////////////////////////
        CTOS_LCDTPrintXY(1,5,"CAEMVL2AP :");  ///
        CTOS_GetSystemInfo(ID_EXTLIBCAEMVL2AP_SO,syb);
        for(i=0;i<6;i++){
            sdb[i]=syb[i];
        }
        CTOS_LCDTPrintXY(13,5,sdb);
       /////EMV Ext-CAEMEL2AP print out//////////
        sprintf(snb,"CAEMVL2AP :%s",sdb);
        CTOS_PrinterPutString(snb);
        
        ////////////EMV Ext-CAPCIPAD //////////////////////////////////////////////
        CTOS_LCDTPrintXY(1,6,"CAPCIPAD  :");  ///
        CTOS_GetSystemInfo(ID_EXTLIBCAPCIPAD_SO,syb);
        for(i=0;i<6;i++){
            sdb[i]=syb[i];
        }
        CTOS_LCDTPrintXY(13,6,sdb);
       /////EMV Ext-CAPCIPAD print out//////////
        sprintf(snb,"CAPCIAD  :%s",sdb);
        CTOS_PrinterPutString(snb);
        
        ////////////EMV Ext-CAREDIRECT //////////////////////////////////////////////
        CTOS_LCDTPrintXY(1,7,"CAREDIRECT:");  ///
        CTOS_GetSystemInfo(ID_EXTLIBCAREDIECT_SO,syb);
        for(i=0;i<6;i++){
            sdb[i]=syb[i];
        }
        CTOS_LCDTPrintXY(13,7,sdb);
       /////EMV Ext-CAREDIRECT print out//////////
        sprintf(snb,"CAREDIRECT:%s",sdb);
        CTOS_PrinterPutString(snb);
        
        ////////////EMV Ext-CABARCODESCAN //////////////////////////////////////////////
        CTOS_LCDTPrintXY(1,8,"CABARCODESCAN:");  ///
        CTOS_GetSystemInfo(ID_EXTLIBCABARCODESCAN_SO,syb);
        for(i=0;i<6;i++){
            sdb[i]=syb[i];
        }
        CTOS_LCDTPrintXY(13,8,sdb);
       /////EMV Ext-CABARCODESCAN print out//////////
        sprintf(snb,"CABARCODESCAN:%s",sdb);
        CTOS_PrinterPutString(snb);
        
        ////////////EMV Ext-CATOUCH //////////////////////////////////////////////
        CTOS_LCDTPrintXY(1,9,"CATOUCH   :VR0001");  ///
        CTOS_PrinterPutString("CATOUCH   :VR0001");
        //CTOS_GetSystemInfo(ID_LIBCATOUCH,syb);
        
        
        
        
        ////////////EMV Ext-CAMMS //////////////////////////////////////////////
        CTOS_LCDTPrintXY(1,10,"CAMMS     :");  ///
        CTOS_GetSystemInfo(ID_EXTLIBCAMMS_SO,syb);
        for(i=0;i<6;i++){
            sdb[i]=syb[i];
        }
        CTOS_LCDTPrintXY(13,10,sdb);
       /////EMV Ext-CAMMS print out//////////
        sprintf(snb,"CAMMS     :%s",sdb);
        CTOS_PrinterPutString(snb);
        
        //CTOS_GetSystemInfo(ID_EXTLIB_SO,syb);
        
        ///////////ULDPM AP Version/////////////
        CTOS_LCDTPrintXY(1,11,"ULDM AP Version: "); 
        CTOS_PrinterPutString("ULDPM AP Version->");
        /////
        CTOS_LCDTPrintXY(1,12,"ULDM-AP:  ");  ///
        CTOS_GetSystemInfo(ID_ULDPM,syb);
        for(i=0;i<6;i++){
            sdb[i]=syb[i];
        }
        CTOS_LCDTPrintXY(13,12,sdb);
       ///// ULDM-AP print out//////////
        sprintf(snb,"ULDM-AP   :%s",sdb);
        CTOS_PrinterPutString(snb);
        
        
        ///////////HWM Version/////////////
        CTOS_LCDTPrintXY(1,13,"HWM Version: "); 
        CTOS_PrinterPutString("HWM Version->");
        //////
        CTOS_LCDTPrintXY(1,14,"CRDL/ETHE : ONCHIP");
        CTOS_PrinterPutString("CRDL/ETHE : ONCHIP");
        CTOS_LCDTPrintXY(1,15,"CLM-MP    : N/A");
        CTOS_PrinterPutString("CLM-MP    : N/A");
        
        
        while(1){
            if(pa ==2 || pa ==5){
                CTOS_Delay(500);
                //CTOS_KBDGet(&key);
                goto PR_Basic4;
            }
            else{
                //CTOS_KBDGet(&key);
                //if(key ==d_KBD_CANCEL) goto Printer;
                //else goto PR_Basic4;
                goto Printer;
            }
        }
        
                
        
PR_Basic4:
        CTOS_LCDTClearDisplay();
        //////Font Info///////////////
        CTOS_LCDTPrintXY(1,1,"[Font info]");
        CTOS_PrinterPutString("[Font info]");
        CTOS_FontFNTNum(&lang);
        sprintf(snb,"Language Num:%d",lang);
        CTOS_PrinterPutString(snb);
        /*
        //chinese 16*16
        CTOS_FontSelectMode(d_FONT_DEVICE_PRINTER,d_FONT_TTF_MODE);
        
        CTOS_PrinterPutString("1.Traditional Chinese(16x16)");
        CTOS_FontSelectMode(d_FONT_DEVICE_PRINTER,d_FONT_FNT_MODE); //SW
        CTOS_FontFNTSelectSize(d_FONT_DEVICE_PRINTER,d_FONT_16x16);
        CTOS_FontFNTSelectLanguage(d_FONT_DEVICE_PRINTER,d_FNT_LANGUAGE_CHINESE_TRADITIONAL);
        CTOS_FontFNTSelectFont(d_FONT_DEVICE_PRINTER, d_FNT_FONTID_CHINESE_TAIWAN);
        CTOS_PrinterTTFSelect("bkai00mp.ttf", 0);    
        //CTOS_FontFNTSelectStyle(d_FONT_DEVICE_LCD_0, d_FONT_STYLE_NORMAL);
        CTOS_PrinterPutString("?¹å ¡ç§‘æ??¹å ¡ç§‘æ??¹å ¡ç§‘æ??¹å ¡ç§‘æ?");
        
        //chinese 24*24
        CTOS_FontSelectMode(d_FONT_DEVICE_PRINTER,d_FONT_TTF_MODE);
        CTOS_FontTTFSelectSize(d_FONT_DEVICE_PRINTER,d_FONT_12x24);
        CTOS_PrinterPutString("2.Traditional Chinese(24x24)");
        CTOS_FontSelectMode(d_FONT_DEVICE_PRINTER,d_FONT_FNT_MODE); //SW
        CTOS_FontTTFSelectSize(d_FONT_DEVICE_PRINTER,d_FONT_24x24);
        //CTOS_FontFNTSelectLanguage(d_FONT_DEVICE_PRINTER,d_FNT_LANGUAGE_CHINESE_TRADITIONAL);
        CTOS_FontFNTSelectFont(d_FONT_DEVICE_PRINTER, d_FNT_FONTID_CHINESE_TAIWAN);
        CTOS_PrinterTTFSelect("bkai00mp.ttf", 0); 
        CTOS_PrinterPutString("?¹å ¡ç§‘æ??¹å ¡ç§‘æ??¹å ¡ç§‘æ??¹å ¡ç§‘æ?");
        
        CTOS_FontSelectMode(d_FONT_DEVICE_PRINTER,d_FONT_TTF_MODE); //SW
        CTOS_PrinterTTFSelect("ca_default.ttf", 0);
        CTOS_FontTTFSelectSize(d_FONT_DEVICE_PRINTER,d_FONT_12x24);
        //CTOS_FontTTFSelectSize(d_FONT_DEVICE_PRINTER,d_FONT_16x30);
        CTOS_FontTTFSelectStyle(d_FONT_DEVICE_PRINTER,d_FONT_STYLE_NORMAL);
         
         
         
         */
        CTOS_PrinterFline(4);
        /////Font-TTF Num
        //CTOS_LCDTPrintXY(1,2,"TTF Num :"); 
        //CTOS_PrinterPutString("TTF Num :");
        dir =opendir("/home/font"); 
        i=0;
        while(1){
            if(dir !=NULL &&(font_name =readdir(dir)) !=NULL){
                sprintf(ttf_temp,"%s",font_name->d_name);
                k =strlen(ttf_temp)-3;
                for(j=0;j<3;j++){
                    FP[j] =ttf_temp[k+j];
                }
                if(strcmp(fcmp,FP)){
                    ttf[i] =font_name->d_name;
                    i++;
                }
                else tt++;
            }
            else break;
        }
        sprintf(syb,"TTF Num   : %d",i-2);
        CTOS_LCDTPrintXY(1,3,syb);
        CTOS_PrinterPutString(syb);
        for(k=0;k<i-2;k++){
            sprintf(syb,"%d. %s",k+1,ttf[k +2]);
            CTOS_PrinterPutString(syb);
        }
        
        
        CTOS_PrinterFline(4);
        ///////////////System setting/////////
        /////HUSBID/////
        CTOS_LCDTPrintXY(1,8,"[System Setting]");
        CTOS_PrinterPutString("[System Setting]");
        CTOS_LCDTPrintXY(1,9,"HUSBID");
        CTOS_USBGetVidPid(&USB);
        sprintf(syb,"0%07x0          ",USB);
        for(i=0;i<7;i++){
            sdb[i]=syb[i];
        }
        CTOS_LCDTPrintXY(13,9,sdb);
        /////HUSBID print out///////
        sprintf(snb,"HUSBID    :%s",sdb);
        CTOS_PrinterPutString(snb);
        //CTOS_LCDTPrintXY(1,10,"CUSBID    :N/A");
        

        /////CUSBID/////
        CTOS_LCDTPrintXY(1,10,"CUSBID");
        ret=CTOS_BaseUSBGetVidPid(&USB);
        sprintf(syb,"0%07x0              ",USB);
        for(i=0;i<7;i++){
            sdb[i]=syb[i];
        }
        if(ret !=d_OK){
            strcpy(sdb,"N/A");
        }
        CTOS_LCDTPrintXY(13,10,sdb);
        /////HUSBID print out///////
        sprintf(snb,"CUSBID    :%s",sdb);
        CTOS_PrinterPutString(snb);
        
        
        
        
        
        ///////usb cdc mode//////
        CTOS_USBGetStatus(&USB);
        if(USB ==d_MK_USB_STATUS_CDCMODE) {
            CTOS_LCDTPrintXY(1,11,"USB CDC Mode  :Y");
            CTOS_PrinterPutString("USB CDC Mode  :Y");
        }
        else {
            CTOS_LCDTPrintXY(1,11,"USB CDC Mode  :N");
            CTOS_PrinterPutString("USB CDC Mode  :N");
        }
        //////Printer font bold
        CTOS_LanguagePrinterGetFontInfo(&PRFontID,&PRFontSize,&Style);
        if((int*)Style>=2){
            CTOS_LCDTPrintXY(1,12,"Printer Font Bold:Y");
            CTOS_PrinterPutString("Printer Font Bold:Y");
        }
        else{
            CTOS_LCDTPrintXY(1,12,"Printer Font Bold:N/A");
            CTOS_PrinterPutString("Printer Font Bold:N/A");
        }
        
        //CTOS_KBDGet(&key);
        while(1){
            if(pa ==5){
                CTOS_Delay(500);
                //CTOS_KBDGet(&key);
                goto PR_EthernetInfo;
            }
            else{ 
                CTOS_PrinterFline(70);
                CTOS_LCDTPrintXY(1,13,"Print Finish.");
                CTOS_LCDTPrintXY(1,14,"Press Any Key.....");
                CTOS_KBDGet(&key);
                goto Printer;}
        }
        
        
PR_EthernetInfo:
         /////////////////////////LCD display////////////
        CTOS_LCDTClearDisplay();
        CTOS_RTCGet(&Pr_time);
        sprintf(pt,"20%02d/%02d/%02d %02d:%02d:%02d",Pr_time.bYear,Pr_time.bMonth,Pr_time.bDay,Pr_time.bHour,Pr_time.bMinute,Pr_time.bSecond);
        CTOS_LCDTPrintXY(1,1,pt);
        CTOS_LCDTPrintXY(1,2,"VEGA Series Terminal");
        //////////////////////print on paper///////////
        if(pa !=5){
        CTOS_PrinterFline(2); //roll the paper without printing
        CTOS_PrinterPutString(pt); //print time
        CTOS_PrinterPutString("VEGA Series Terminal");
        CTOS_PrinterFline(1);
        }
        //////////Ethernet Info
        CTOS_LCDTPrintXY(1,3," [Ethernet Info]");
        CTOS_PrinterPutString("[Ethernet Info]");
        
        
        //ETHERNET
        ret =CTOS_HWSettingGet(&fPortable,&fPCI,&mkHWSupport);
        //if ((mkHWSupport & d_MK_HW_CONTACTLESS) == d_MK_HW_CONTACTLESS)
            
        if (mkHWSupport &d_MK_HW_ETHERNET){
            if(fPortable ==d_FALSE){         //countertop module
                CTOS_PrinterPutString("ETHERNET  : ONCHIP");
            }
            else if(fPortable ==d_TRUE){    //portable module
                CTOS_PrinterPutString("ETHERNET  : ONCARDLE");
            }
        }
        else{
            CTOS_PrinterPutString("Ethernet Not Support");
        }
                
                
        //////LOCAL IP///////////
        ret=CTOS_EthernetConfigGet(d_ETHERNET_CONFIG_IP,str,&len);
        if(ret !=d_OK){
            strcpy(str,"N/A");
        }
        strcpy(snb,"LOCAL IP  :");
        strcat(snb,str);
        CTOS_LCDTPrintXY(1,4,snb);
        CTOS_PrinterPutString(snb);
        
        
        //////MASK////////////
        ret=CTOS_EthernetConfigGet(d_ETHERNET_CONFIG_MASK,str,&len);
        if(ret !=d_OK){
            strcpy(str,"N/A");
        }
        strcpy(snb,"MASK      :");
        strcat(snb,str);
        CTOS_LCDTPrintXY(1,5,snb);
        CTOS_PrinterPutString(snb);
        
        //////GATEWAY IP////////////
        ret=CTOS_EthernetConfigGet(d_ETHERNET_CONFIG_GATEWAY,str,&len);
        if(ret !=d_OK){
            strcpy(str,"N/A");
        }
        strcpy(snb,"GATEWAY IP:");
        strcat(snb,str);
        CTOS_LCDTPrintXY(1,6,snb);
        CTOS_PrinterPutString(snb);
        
        //////HOST IP////////////
        ret=CTOS_EthernetConfigGet(d_ETHERNET_CONFIG_HOSTIP,str,&len);
        if(ret !=d_OK){
            strcpy(str,"N/A");
        }
        strcpy(snb,"HOST  IP  :");
        strcat(snb,str);
        CTOS_LCDTPrintXY(1,7,snb);
        CTOS_PrinterPutString(snb);
        
        //////HOST PORT////////////
        ret=CTOS_EthernetConfigGet(d_ETHERNET_CONFIG_HOSTPORT,str,&len);
        if(ret !=d_OK){
            strcpy(str,"N/A");
        }
        strcpy(snb,"HOST PORT :");
        strcat(snb,str);
        CTOS_LCDTPrintXY(1,8,snb);
        CTOS_PrinterPutString(snb);
        
        //////MAC////////////
        memset(str, 0, sizeof(str)); 
        ret=CTOS_EthernetConfigGet(d_ETHERNET_CONFIG_MAC,str,&len);
        if(ret !=d_OK){
            strcpy(str,"N/A");
        }
        strcpy(snb,"MAC       :");
        strcat(snb,str);
        CTOS_LCDTPrintXY(1,9,snb);
        CTOS_PrinterPutString(snb);
        
        //////CONN MODE////////////
        memset(str, 0, sizeof(str)); 
        ret=CTOS_EthernetConfigGet(d_ETHERNET_CONFIG_AUTOCON,str,&len);
        if(ret !=d_OK){
            strcpy(str,"N/A");
        }
        
        strcpy(snb,"CONN MODE :");
        
        if(atoi(str) ==0){
            strcpy(str,"Auto-connection");
        }
        if(atoi(str) ==1){
            strcpy(str,"No Support");
        }
        if(atoi(str) ==2){
            strcpy(str,"Manual");
        }
            

        strcat(snb,str);
        CTOS_LCDTPrintXY(1,10,snb);
        CTOS_PrinterPutString(snb);
        
        //////IP MODE////////////
        memset(str, 0, sizeof(str)); 
        ret=CTOS_EthernetConfigGet(d_ETHERNET_CONFIG_DHCP,str,&len);
        if(ret !=d_OK){
            strcpy(str,"N/A");
        }
        strcpy(snb,"IP MODE   :");
        
        if(atoi(str) ==0){
            strcpy(str,"Static");
        }
        if(atoi(str) ==1){
            strcpy(str,"DHCP");
        }
        
        
        strcat(snb,str);
        CTOS_LCDTPrintXY(1,11,snb);
        CTOS_PrinterPutString(snb);
        
        //////DNS Server////////////
        memset(str, 0, sizeof(str)); 
        ret=CTOS_EthernetConfigGet(d_ETHERNET_CONFIG_DNSIP,str,&len);
        if(ret !=d_OK){
            strcpy(str,"N/A");
        }
        strcpy(snb,"DNS SERVER:");
        strcat(snb,str);
        CTOS_LCDTPrintXY(1,12,snb);
        CTOS_PrinterPutString(snb);
        
        //////HOST URL////////////
        memset(str, 0, sizeof(str)); 
        ret=CTOS_EthernetConfigGet(d_ETHERNET_CONFIG_HOSTURL,str,&len);
        if(ret !=d_OK){
            strcpy(str,"N/A");
        }
        
        strcpy(snb,"HOST URL  :");
        strcat(snb,str);
        CTOS_LCDTPrintXY(1,13,snb);
        CTOS_PrinterPutString(snb);
        
       
        //CTOS_KBDGet(&key);
        //if(pa ==5) goto PR_IMEIInfo;
        //else goto Printer;
        
        while(1){
            if(pa ==5){
                CTOS_Delay(500);
               goto PR_IMEIInfo;}
            else {
                CTOS_PrinterFline(70);
                CTOS_LCDTPrintXY(1,5,"Print Finish.");
                CTOS_LCDTPrintXY(1,6,"Press Any Key.....");
                CTOS_KBDGet(&key);
            goto Printer;}
        }
        
PR_IMEIInfo:
        CTOS_LCDTClearDisplay();
        CTOS_GSMOpen(115200,TRUE);
        CTOS_LCDTPrintXY(1,1,"IMEI      : ");
        CTOS_GSMGetIMEI(snb);
        CTOS_LCDTPrintXY(1,2,snb);
        GSM_Get_Version(snb,&lang);
        CTOS_LCDTPrintXY(1,3,"Version   : ");
        for(i=0;i<10;i++){
            if(i<9) smi[i]=snb[i];
        }
        CTOS_LCDTPrintXY(1, 4, smi);
        //print out
        CTOS_PrinterFline(4);
        CTOS_GSMGetIMEI(snb);
        sprintf(syb,"IMEI      :%s",snb);
        CTOS_PrinterPutString(syb);
        GSM_Get_Version(snb,&lang);
        //for(i=0;i<10;i++){
        //    if(i<9) smi[i]=snb[i];
        //}
        //sprintf(syb,"Version: %s",smi);
        sprintf(syb,"Version: %s",snb);
        CTOS_PrinterPutString(syb);
        CTOS_PrinterFline(70);            
        //CTOS_Delay(1000);
        CTOS_LCDTPrintXY(1,5,"Print Finish.");
        CTOS_LCDTPrintXY(1,6,"Press Any Key.....");
        CTOS_KBDGet(&key);
        goto Printer;
        
    
    

    
/////10./////
Font_test:
        i=f=j=k=0;
	CTOS_LCDTClearDisplay();
        CTOS_LCDTSetReverse(TRUE);
        CTOS_LCDTPrintXY(1, 1, "  Plug-in Font List ");
        CTOS_LCDTSetReverse(FALSE);
        dir = opendir("/home/font"); //open the list of font folder
        while(1){
            if(dir != NULL && (font_name = readdir(dir)) != NULL){   //   font_name get all information from dir 
                sprintf(ttf_temp,"%s",font_name->d_name);  //convert d_name into char format and store into ttf_temp
                k = strlen(ttf_temp)-3;     
                for(j = 0;j < 3;j++){       //store last 3 char in FP
                    FP[j] = ttf_temp[ k + j];  
                }
                if(strcmp(fcmp,FP)){           //makesure its ttf format file
                    ttf[i] = font_name->d_name;//store the file name in ttf[]
                    i++;
                }
                else f++;                     //if its not fnf file,it must be fnt file.
            }
            else break;
        }
        sprintf(buf,"TTF Num :%d",i-2 );
        CTOS_LCDTPrintXY(1, 3, buf);
        sprintf(buf," %d fonts in it",i-2 );
        CTOS_LCDTPrintXY(1, 4, buf);
        /*
        if(t > 0 ){
            sprintf(buf," %d FNT in here.",t);
            CTOS_LCDTPrintXY(1, 5, buf);
        }
        */
        CTOS_LCDTPrintXY(1, 9, "____________________");
        CTOS_LCDTPrintXY(1, 11, " [Any key] View List");
        CTOS_LCDTPrintXY(1, 12, " [   X   ] Back");
        CTOS_KBDGet(&key);
        CTOS_LCDTClearDisplay();
        //CTOS_FontFNTSelectFont( d_FONT_DEVICE_LCD_0, d_FNT_FONTID_CHINESE_TAIWAN);
        //CTOS_FontFNTSelectStyle(d_FONT_DEVICE_LCD_0, d_FONT_STYLE_NORMAL);
        //CTOS_LCDTTFSelect("bkai00mp.ttf", 0);    
        CTOS_LCDTPrintXY(1, 2, "Chinese");
        CTOS_LCDTPrintXY(1, 3, "(Taiwan)");
        CTOS_LCDTPrintXY(1, 4, "16X16");
        CTOS_LCDTPrintXY(1, 5, "Style: 0");
        
        CTOS_KBDGet(&key);
        CTOS_LCDTClearDisplay();
        //CTOS_FontFNTSelectFont( d_FONT_DEVICE_LCD_0, d_FNT_FONTID_CHINESE_TAIWAN);
        //CTOS_FontFNTSelectStyle(d_FONT_DEVICE_LCD_0, d_FONT_STYLE_BOLD);
        CTOS_FontTTFSelectStyle(d_FONT_DEVICE_LCD_0,  d_FONT_STYLE_BOLD);
        CTOS_LCDTPrintXY(1, 2, "Chinese");
        CTOS_LCDTPrintXY(1, 3, "(Taiwan)");
        CTOS_LCDTPrintXY(1, 4, "24X24");
        CTOS_LCDTPrintXY(1, 5, "Style: 0");
        
        CTOS_FontTTFSelectStyle(d_FONT_DEVICE_LCD_0,  d_FONT_STYLE_NORMAL);
        while(1){
            CTOS_KBDGet(&key);
            if (key == d_KBD_CANCEL) break;
            CTOS_LCDTClearDisplay();
            CTOS_LCDTPrintXY(1, 3, "  [TTF File List]   ");
            CTOS_LCDTPrintXY(1, 9, "                    ");
            CTOS_LCDTPrintXY(1, 11,"                    ");
            CTOS_LCDTPrintXY(1, 12,"                    ");
            if(i ==1)  CTOS_LCDTPrintXY(1,6, "  File not found   ");
        else{
            for(k = 0;k < (i-2);k++ ){
                CTOS_LCDTPrintXY(1, k+5,"                 ");
                sprintf(buf,"%d. %s",k+1,ttf[k+2]);
                CTOS_LCDTPrintXY(1, k+5,buf);
            }
             CTOS_LCDTPrintXY(1, 16,"  Press any Key...");
             CTOS_KBDGet(&key);
             break;
            }
        }
        //CTOS_FontSelectMode(d_FONT_DEVICE_LCD_0,d_FONT_TTF_MODE);
        //CTOS_LCDTTFSelect("ca_default.ttf", 0);
        //CTOS_FontTTFSelectSize(d_FONT_DEVICE_LCD_0,d_FONT_16x30);
        //CTOS_FontTTFSelectStyle(d_FONT_DEVICE_LCD_0,d_FONT_STYLE_NORMAL);
        //CTOS_LCDTSelectFontSize(0x131E);
        
        //CTOS_FontTTFSelectFontFile(d_FONT_DEVICE_LCD_0,ca_default.ttf,BYTE bIndex)
        //CTOS_FontFNTSelectFont( d_FONT_DEVICE_LCD_0,  d_FONT_EMPTY);
        //CTOS_FontFNTSelectStyle(d_FONT_DEVICE_LCD_0, );
        goto Main;
        
/////12/////////////
CL_Card:
       i=0;
       ret=CTOS_Beep();
       cc=20;
       CTOS_LCDTClearDisplay();
       CTOS_LCDTSetReverse(TRUE);
       CTOS_LCDTPrintXY(1,1,"  Contactless Test  ");
       CTOS_LCDTSetReverse(FALSE);
       if(ret != d_OK){
           CTOS_LCDTPrintXY(1,3,"Can't found beep! ");
           CTOS_LCDTPrintXY(1,6,"Please check divice. ");
           CTOS_LCDTPrintXY(1,9,"Press key to left.....");
           CTOS_KBDGet(&key);
           exit(0);
       }
       
CL_Menu:
        CTOS_LCDTClearDisplay();
        CTOS_LCDTSetReverse(TRUE);
        CTOS_LCDTPrintXY(1,1,"  Contactless Test  ");
        CTOS_LCDTSetReverse(FALSE);
        CTOS_LCDTPrintXY(2,3,"<1> Mifare Test");
        CTOS_LCDTPrintXY(2,4,"<2> Type A Test");
        CTOS_LCDTPrintXY(2,5,"<3> Type B Test");
        CTOS_LCDTPrintXY(2,6,"<4> Felica Test");
        CTOS_LCDTPrintXY(2,7,"<5> Set counter");
        CTOS_LCDTPrintXY(2,8,"Count:");
        CTOS_LCDTPrintXY(2,10,"<X> Exit");
        cd=0;
        sprintf(co,"%d",cc);
        CTOS_LCDTPrintXY(2,9,co);
       
        while(1){
            CTOS_KBDGet(&key);
            if(key == d_KBD_CANCEL) goto Main;
            if(key == d_KBD_1) goto Mifare_Test;
            if(key == d_KBD_2) goto Type_A_Test;
            if(key == d_KBD_3) goto Type_B_Test;
            if(key == d_KBD_4) goto Felica_Test;
            if(key == d_KBD_5) goto Set_count;
        }
        
Mifare_Test:
        CTOS_LCDTClearDisplay();
        CTOS_LCDTSetReverse(TRUE);
        CTOS_LCDTPrintXY(1,1,"  Mifare Test       ");
        CTOS_LCDTSetReverse(FALSE);
        CTOS_LCDTPrintXY(1,3,"OK  [         ]");
        CTOS_LCDTPrintXY(1,4,"NG  [         ]");
        KeyType = TypeA;
        cd=cf=0;
        while((cd+cf)<cc){
            CTOS_KBDHit(&key);
            if(key == d_KBD_CANCEL) goto CL_Menu;
            ret = CTOS_CLTypeAActiveFromIdle(0, ATQA, &SAK, SN, &SNLen);
             if (ret == d_OK){
                    LK = CTOS_MifareLOADKEY("\x0F\x0F\x0F\x0F\x0F\x0F\x0F\x0F\x0F\x0F\x0F\x0F");
                    AT = CTOS_MifareAUTHEx(KeyType, Block, SN, SNLen);
                        if(LK == d_OK){
                             if(AT == d_OK){
                                 cd++;
                                sprintf(co,"%d",cd);
                                 CTOS_LCDTPrintXY(6,3,co);
                                 CTOS_Delay(100);
                            }
                        } 
             }
             if (ret != d_OK || LK != d_OK || AT != d_OK){
                 cf++;
                 sprintf(co,"%d",cf);
                 CTOS_LCDTPrintXY(6,4,co);
                 CTOS_Delay(100);
            }
        }
        goto CL_Menu;
        
Type_A_Test: 
        CTOS_LCDTClearDisplay();
        CTOS_LCDTSetReverse(TRUE);
        CTOS_LCDTPrintXY(1,1,"  Type A Test       ");
        CTOS_LCDTSetReverse(FALSE);
        CTOS_LCDTPrintXY(1,3,"OK  [         ]");
        CTOS_LCDTPrintXY(1,4,"NG  [         ]");
        KeyType = TypeA;
        cd=cf=0;
        while((cd+cf)<cc){
            CTOS_KBDHit(&key);
            if(key == d_KBD_CANCEL) goto CL_Menu;
                ret=CTOS_REQA(ATQA);
                     CTOS_Delay(100);
                if(ret == d_OK){
                    cd++;
                    sprintf(co,"%d",cd);
                    CTOS_LCDTPrintXY(6,3,co);
                 }
                else if (ret != d_OK){
                 cf++;
                 sprintf(co,"%d",cf);
                 CTOS_LCDTPrintXY(6,4,co);
                 }
                     CTOS_CLTypeAHalt();
        }      
        goto CL_Menu;

Type_B_Test:
        CTOS_LCDTClearDisplay();
        CTOS_LCDTSetReverse(TRUE);
        CTOS_LCDTPrintXY(1,1,"  Type B Test       ");
        CTOS_LCDTSetReverse(FALSE);
        CTOS_LCDTPrintXY(1,3,"OK  [         ]");
        CTOS_LCDTPrintXY(1,4,"NG  [         ]");
        KeyType = TypeB;
        cd=cf=0;
        while((cd+cf)<cc){
                    CTOS_KBDHit(&key);
                    if(key == d_KBD_CANCEL) goto CL_Menu;
                    ret = CTOS_REQB(0,ATQB,&ATQBLen);
                    if(ret == d_OK){
                        cd++;
                        sprintf(co,"%d",cd);
                        CTOS_LCDTPrintXY(6,3,co);
                        CTOS_Delay(100);
                    }
                    else if (ret != d_OK){
                 cf++;
                 sprintf(co,"%d",cf);
                 CTOS_LCDTPrintXY(6,4,co);
                 CTOS_Delay(100);
            }
        }      
goto CL_Menu;

Felica_Test:
        CTOS_LCDTClearDisplay();
        CTOS_LCDTSetReverse(TRUE);
        CTOS_LCDTPrintXY(1,1,"  Felica Test       ");
        CTOS_LCDTSetReverse(FALSE);
        CTOS_LCDTPrintXY(1,3,"OK  [         ]");
        CTOS_LCDTPrintXY(1,4,"NG  [         ]");
        KeyType = TypeB;
        cd=cf=0;
        while((cd+cf)<cc){
                    CTOS_KBDHit(&key);
                    if(key == d_KBD_CANCEL) goto CL_Menu;
                    LK = CTOS_FelicaPolling(ID, PM);
                    if(LK == d_OK){
                        cd++;
                        sprintf(co,"%d",cd);
                        CTOS_LCDTPrintXY(6,3,co);
                        CTOS_Delay(100);
                    }
             if (LK != d_OK){
                 cf++;
                 sprintf(co,"%d",cf);
                 CTOS_LCDTPrintXY(6,4,co);
                 CTOS_Delay(100);
            }
        }      
goto CL_Menu;


Set_count:
        CTOS_LCDTClearDisplay();
        CTOS_LCDTPrintXY(1,1,"  Count:");
        cc=0;
        while(1){
        CTOS_KBDGet(&key);
        if(key == d_KBD_CANCEL){
            cc=20;
            goto CL_Menu;
        }
        if(key == d_KBD_ENTER) goto CL_Menu;
     
        if(key == d_KBD_1) cc=cc*10+1;
        if(key == d_KBD_2) cc=cc*10+2;
        if(key == d_KBD_3) cc=cc*10+3;
        if(key == d_KBD_4) cc=cc*10+4;
        if(key == d_KBD_5) cc=cc*10+5;
        if(key == d_KBD_6) cc=cc*10+6;
        if(key == d_KBD_7) cc=cc*10+7;
        if(key == d_KBD_8) cc=cc*10+8;
        if(key == d_KBD_9) cc=cc*10+9;
        if(key == d_KBD_0) cc=cc*10;
        sprintf(co,"%d",cc);
        CTOS_LCDTPrintXY(2,9,co);
        }
    goto CL_Menu;        

   
    
SD_Card_test:
        CTOS_LCDTClearDisplay();
        i=0;
        CTOS_LCDTSetReverse(TRUE);
        CTOS_LCDTPrintXY(1, 1, "  SD Card Test      ");
        CTOS_LCDTSetReverse(FALSE);
        CTOS_LCDTPrintXY(1, 9, "    Detecting...    ");
        while(1){
            //fStat = stat("/media/mdisk/",&fInfo);  //if sdcard is detect>>fStat =0  
            usRet =system("ls -l /sys/block/ | grep 'mmc'");
            CTOS_Delay(50); //detect sdcard every 0.050sec
            if(usRet ==d_OK) break;
            if(i == 100){
                CTOS_LCDTPrintXY(1,9," No Card Insert.");
                CTOS_LCDTPrintXY(1,10," Press any key...");
                CTOS_KBDGet(&key);
                i=0;
                goto Main;
            }
            else i++;
         }
        
        fStat = stat("/media/mdisk/",&fInfo);  //add
        if( fStat ==0){
            file= fopen("/media/mdisk/test.txt","w");  //write test.txt file in sdcard
            fprintf(file,"[test123]\n");
            fprintf(file,"2018.12.13\n");
            fprintf(file," SD Card Test!!");
            fclose(file);
        
            file= fopen("/media/mdisk/test.txt","r"); //read test.txt file just construct
            fscanf(file,"%s%s%s%s%s", SD_f1, SD_f2, SD_f3, SD_f4, SD_f5); //load word into five char,every char length is 11.
            fclose(file);  //remember to close the file after read it
        
        
            CTOS_LCDTPrintXY(1,3,"File :test.txt");
            CTOS_LCDTPrintXY(2,5,SD_f1);
            sprintf(SD_d,"[test123]");
            SD1 = strcmp(SD_f1, SD_d); //char compare
        
            CTOS_LCDTPrintXY(2,6,SD_f2);
            sprintf(SD_d,"2018.12.13");
            SD2 = strcmp(SD_f2, SD_d); //char compare
        
            CTOS_LCDTPrintXY(2,7,SD_f3);
            sprintf(SD_d,"SD");
            SD3 = strcmp(SD_f3, SD_d); //char compare
        
            CTOS_LCDTPrintXY(5,7,SD_f4);
            sprintf(SD_d,"Card");
            SD4 = strcmp(SD_f4, SD_d); //char compare
        
            CTOS_LCDTPrintXY(10,7,SD_f5);
            sprintf(SD_d,"Test!!");
            SD5 = strcmp(SD_f5, SD_d); //char compare
        
            if(SD1==0 && SD2==0 && SD3==0 && SD4==0 && SD5==0){   //if compare success,SD will be all zero
                CTOS_LCDTPrintXY(1,9,"DETECT SUCCESS!!");
                CTOS_Delay(1000);
            }
            else{ CTOS_LCDTPrintXY(1,9,"DETECT FAIL.");
            CTOS_Delay(1000);}
        }
        else {CTOS_LCDTPrintXY(1,9,"READ FILE DATA FAIL.");
            CTOS_Delay(1000);} //if there is any detect error,fStat!=0. 
        
        goto Main;
       
/////14//////////
WIFI:     
        cc=i=j=k=0;
        CTOS_LCDTClearDisplay();
        CTOS_LCDTSetReverse(TRUE);
        CTOS_LCDTPrintXY(1, 1, "  Wi-Fi Test Menu   ");
        CTOS_LCDTSetReverse(FALSE);
        CTOS_LCDTPrintXY(1, 2, "<1>  Wi-Fi Test");
        CTOS_LCDTPrintXY(1, 3, "<2>  Wi-Fi Info");
        CTOS_LCDTPrintXY(1, 8, "press key to exit...");
        CTOS_KBDGet(&key);
        if(key == d_KBD_1) goto WIFI_test;      
        else if(key == d_KBD_2) goto WIFI_Info; 
        else goto Main;

WIFI_test:        
        CTOS_LCDTClearDisplay();
        CTOS_LCDTPrintXY(1, 1, "   Wi-Fi Scan");
        CTOS_WifiConfigSet(d_WIFI_CONFIG_AUTOCON_AP,"0", 1); //No auto-connection to the AP 
        CTOS_WifiConfigSet(d_WIFI_CONFIG_DHCP,"1", 1);  //DHCP
	CTOS_LCDTPrintXY(1, 3, "  Wi-Fi Open.....");
        CTOS_Delay(500);
        ret=CTOS_WifiOpen();
        if(ret !=d_OK){
            CTOS_LCDTPrintXY(1, 3, "Open Fail.   ");
            sprintf(temp,"Error Return:0x%04x",ret);
            CTOS_LCDTPrintXY(1, 4,temp);
            CTOS_KBDGet(&key);
            goto WIFI;
        }
        
        CTOS_LCDTPrintXY(1, 3, "Open Success.   ");
        CTOS_Delay(500);
        CTOS_LCDTPrintXY(1, 3, "Searching Wi-Fi....");
        CTOS_Delay(500);
        CTOS_WifiScan();
        while(1){  //check wifi status 
            ret =CTOS_WifiStatus(&Status);
            if(ret == d_OK) break;
            else if(Status == d_WIFI_STATE_SCANNING)  continue;
            else {
                CTOS_LCDTPrintXY(1, 3, "Searching Fail!");
                sprintf(temp,"> Return :0x%04x",ret);
                CTOS_LCDTPrintXY(1, 4 ,temp);
                sprintf(temp,"> Status :0x%04x",Status);
                CTOS_LCDTPrintXY(1, 5 ,temp);
                CTOS_KBDGet(&key);
                goto WIFI;
            }
        }
            CTOS_LCDTPrintXY(1, 3, "> Search Success.");
            CTOS_Delay(200);
            CTOS_LCDTPrintXY(1, 3, "> Get list.....  ");
            CTOS_Delay(200);
            CTOS_WifiInfoGet(&W_Num, &W_List);
            current_page =0;
            Page =((W_Num)/7)+(W_Num % 7 ?1 :0);  //every page has 7 selection,count the total page
            if(Page > 0) current_page =1;
            goto WIFI_list;
            
WIFI_list:
            CTOS_LCDTPrintXY(1, 3, "WiFi List          ");
            CTOS_LCDTPrintXY(1, 13, "[0] Rescan");
            CTOS_LCDTPrintXY(1, 14, "[X] Back");
            while(1){
            sprintf(temp,"                     ");  //erease temp
            sprintf(temp,"Device list:%02d/%02d",current_page,Page);
            CTOS_LCDTPrintXY(1, 16, temp);
            //over 7 wifi
           
                if(current_page <Page) CTOS_LCDTPrintXY(1, 12, "[^]Pre       [.]Next");
                else if(current_page ==1) CTOS_LCDTPrintXY(1, 12, "             [.]Next");  //the first page
                else if(current_page ==Page) CTOS_LCDTPrintXY(1, 12, "[^]Pre              "); // the last page
                for(i=1;i<8;i++){
                    sprintf(temp,"                   ");//erease temp 20
                    CTOS_LCDTPrintXY(1,(i+3),temp);         //erease
                    sprintf(temp,"[%d]%s",i,(W_List + ((current_page-1)*7) +i)->ESSID);
                    CTOS_LCDTPrintXY(1,(i+3),temp);         //print start on screen x=1 y=4
                }
                CTOS_KBDGet(&key);     
                if(key == d_KBD_DOT) { 
                    current_page++;
                    if(current_page >Page) current_page=Page;
                }
                else if(key == d_KBD_00) {
                    current_page--;
                    if(current_page <1) current_page=1;
                }
                else if(key == d_KBD_0) goto WIFI_test;
                else if(key == d_KBD_CANCEL) goto WIFI;
                else{
                    if(key==d_KBD_1) Wifi=(W_List +1 + (current_page-1)*7);
                    if(key==d_KBD_2) Wifi=(W_List +2 + (current_page-1)*7);
                    if(key==d_KBD_3) Wifi=(W_List +3 + (current_page-1)*7);
                    if(key==d_KBD_4) Wifi=(W_List +4 + (current_page-1)*7);
                    if(key==d_KBD_5) Wifi=(W_List +5 + (current_page-1)*7);
                    if(key==d_KBD_6) Wifi=(W_List +6 + (current_page-1)*7);
                    if(key==d_KBD_7) Wifi=(W_List +7 + (current_page-1)*7);
                    goto WIFI_connect;
                }
            }
            
WIFI_connect:        
        CTOS_LCDTClearDisplay();
        CTOS_LCDTPrintXY(1, 1, "    Wifi Connect    ");
        sprintf(temp,"%s             ",Wifi->ESSID);
        CTOS_LCDTPrintXY(1, 2,temp);
        CTOS_LCDTPrintXY(1, 3, " > Ready to connect.");
        CTOS_LCDTPrintXY(1, 4, " > Security key :   ");
        while(1){
        CTOS_UIKeypad(5, 5, keyboardLayoutEnglish, 40, 80, d_TRUE, d_FALSE, 0, 0, PW, 50);
        CTOS_WifiConnectAP(Wifi, PW, strlen(PW));
                    
            while(1){
                    //Verify(k, temp); 
                    //CTOS_LCDTPrintXY(1, 5, temp);
                    ret=CTOS_WifiStatus(&Status);
                    if(ret ==d_OK) break;  //connect success
                    else if(Status ==d_WIFI_STATE_AP_CONNECTING)  continue; //detect wifi wait for connecting
                    else {
                        CTOS_LCDTPrintXY(1, 15, "Connect fail    ");
                        sprintf(temp,"Return :0x%04x   ",ret);
                        CTOS_LCDTPrintXY(1,7,temp);
                        sprintf(temp,"Return :0x%04x   ",Status);
                        CTOS_LCDTPrintXY(1,8,temp);
                        CTOS_LCDTPrintXY(1, 9, "--------------------");
                        CTOS_LCDTPrintXY(1, 13,"[x] Back  [.] Retry");
                        CTOS_KBDGet(&key);
                        if(key==d_KBD_CANCEL) goto WIFI_list;
                        else if(key==d_KBD_DOT) goto WIFI_connect;
                        else goto WIFI;
                    } 
                }
            CTOS_LCDTPrintXY(1,15,"Connect Success!!");
            CTOS_Delay(1000);
            goto WIFI_run;
                }

       
WIFI_run:
        CTOS_LCDTClearDisplay();
        CTOS_LCDTPrintXY(1,1,"    WiFi Run    ");
        CTOS_LCDTPrintXY(1,2," Link Socket...");
        CTOS_Delay(500);
        sockfd =socket(AF_INET, SOCK_STREAM, 0);
        if(sockfd ==-1){
            CTOS_LCDTPrintXY(1,3,"Link Fail.");
            goto Shut_down;
        }
        memset(&sa,'\0',sizeof(sa));  //initialization 0
        sa.sin_family =AF_INET;     //AF_INET,because its ipv4
        sa.sin_addr.s_addr =inet_addr("218.211.35.219"); //convert ip ASCII format to integer
        sa.sin_port =htons(140);    //store port no
        if( connect(sockfd,(struct sockaddr *)&sa,sizeof(sa))==-1){
            CTOS_LCDTPrintXY(1,3,"Connect Error.");
            CTOS_Delay(500);
        }
        CTOS_LCDTPrintXY(1,3,"Send/Get data...");
        CTOS_Delay(500);
        CTOS_LCDTPrintXY(1,6,"-----Txdata-----");
        CTOS_LCDTPrintXY(1,7, "Send >");
        CTOS_LCDTPrintXY(1,9,"-----Rxdata-----");
        CTOS_LCDTPrintXY(1,10,"Recv >");
        CTOS_LCDTPrintXY(1,12,"OK > 00.  NG > 00. ");
        tx_len =1024, rx_len =0 ,NG =OK =0;
        
        //while(1){
         for(i=0;i<20;i++){ 
            CTOS_LCDTPrintXY(1,14,"Loop Round:20");
            //CTOS_LCDTPrintXY(1,14,"Loop Round:  /");
            //sprintf(co,"%02d",lc);
            //CTOS_LCDTPrintXY(15,14,co);
            //if(cc ==0) sprintf(co,"01");
            //else sprintf(co,"%02d",cc);
            //CTOS_LCDTPrintXY(18,14,co);
            //if(lc ==0){
              //  CTOS_LCDTPrintXY(1,3,"[WiFi  Test Finish]");
                //goto Shut_down;
            //}
            tal =0;
            CTOS_LCDTPrintXY(9,7,"                    ");
            CTOS_LCDTPrintXY(9,10,"                    ");
            
            for(k =0;k <1024;k++){          //   set random data
                W_Data[k]=(rand()%0xff);
            }
            W_Data[0] =1022 >>8;//right shift 8
            W_Data[1] =1022 &0xff; //clear the left zero
            
            while(tal !=tx_len){
                ral =send(sockfd, &W_Data[tal], (tx_len-tal),0);
                if(ral !=-1) tal+=ral;
                if(ral ==0){
                    CTOS_LCDTPrintXY(1,3,">Send data fail.");
                    NG++;
                    flg =FALSE;
                    break;
                }
            }
                sprintf(temp,"%04d Bytes",rx_len);
                CTOS_LCDTPrintXY(9,7,temp);
                rx_len =ral =del =0;
                memset(W_Recv,0,tx_len);
                
                
            while(rx_len !=tal && del<500){
                ral =recv(sockfd,&W_Recv[rx_len],(tx_len-rx_len),0);
                if(ral !=-1){
                    rx_len +=ral;
                    del =0;
                }
                else del++;
                if(ral ==0){
                    CTOS_LCDTPrintXY(1,3," > Recv data fail.");
                    NG++;
                    flg =FALSE;
                    break;
                }
                sprintf(temp,"%04d Bytes",rx_len);
                CTOS_LCDTPrintXY(9,10,temp);
            }
                
            if(flg ==TRUE &&del<500){
                if(tx_len ==rx_len){
                    if(strcmp(W_Data,W_Recv) ==0) OK++;
                }
                 else NG++;
                }
            else goto Shut_down;
            //lc--;
            sprintf(temp,"%02d",OK);
            CTOS_LCDTPrintXY(7,12,temp);
            sprintf(temp,"%02d",NG);
            CTOS_LCDTPrintXY(17,12,temp);
        }
        goto Shut_down;
         
                
            
WIFI_Info:
        CTOS_LCDTClearDisplay();
        CTOS_LCDTSetReverse(TRUE);
        CTOS_LCDTPrintXY(1,1,"  WiFi Info         ");   
        CTOS_LCDTSetReverse(FALSE);
        memset(info, 0, sizeof(info)); 
        tLen = sizeof(info);
        CTOS_WifiConfigGet(d_WIFI_CONFIG_DHCP, info, &tLen);
        sprintf(temp, " Wifi DHCP   : %s", info); 
        CTOS_LCDTPrintXY(1, 3, temp);
        memset(info, 0, sizeof(info)); 
        tLen = sizeof(info);
        CTOS_WifiConfigGet(d_WIFI_CONFIG_AUTOCON_AP, info, &tLen);
        sprintf(temp, " Auto-con AP : %s", info); 
        CTOS_LCDTPrintXY(1, 4, temp);
        memset(info, 0, sizeof(info)); 
        tLen = sizeof(info);
        CTOS_WifiConfigGet(d_WIFI_CONFIG_DNSIP, info, &tLen);
        CTOS_LCDTPrintXY(1, 5, " Wifi DNS IP :");
        sprintf(temp, "  %s", info); 
        CTOS_LCDTPrintXY(1, 6, temp);
        memset(info, 0, sizeof(info)); 
        tLen = sizeof(info);
        CTOS_WifiConfigGet(d_WIFI_CONFIG_MAC, info, &tLen);
        CTOS_LCDTPrintXY(1, 7, " Wifi MAC :");
        sprintf(temp, "  %s", info); 
        CTOS_LCDTPrintXY(1, 8, temp);
        CTOS_LCDTPrintXY(1, 16, "Press key to back...");
        CTOS_KBDGet(&key); 
        goto WIFI;

Shut_down:            
        CTOS_LCDTPrintXY(1,16,"Press Key to left......");
        CTOS_KBDGet(&key);
        CTOS_LCDTPrintXY(1,16,"                    ");
        CTOS_LCDTPrintXY(1,3,"WiFi closing.....");
        close(sockfd);
        CTOS_WifiDisconnectAP(); 
        CTOS_WifiClose(); 
        CTOS_Delay(500); 
        goto WIFI;
                
/////15////////////
Power_Saving_test:
      while(1){
	CTOS_LCDTClearDisplay();
        CTOS_LCDTPrintXY(1, 1, "Power Saving Test");
        CTOS_LCDTPrintXY(3, 3, "[1] Standby mode");
        CTOS_LCDTPrintXY(3, 4, "[2] Sleep Mode");
        CTOS_LCDTPrintXY(3, 5, "[X] Exit");
        CTOS_KBDGet(&key);
        if(key == d_KBD_CANCEL) goto Main;
        if(key ==d_KBD_1){
            CTOS_LCDTPrintXY(3, 3, "[1] Standby mode");
            CTOS_LCDTPrintXY(3, 4, "Press any key...");
            CTOS_LCDTPrintXY(3, 5, "to weak up");
            ret =CTOS_PowerMode(d_PWR_STANDBY_MODE);
        }
        if(key ==d_KBD_2){
            CTOS_LCDTPrintXY(3, 4, "[2] Sleep mode");
            CTOS_Delay(1000);
            //CTOS_LCDTPrintXY(3, 5, "Testing.....");
            CTOS_LCDTPrintXY(3, 5, "Press any key...");
            CTOS_LCDTPrintXY(3, 6, "to weak up");
            ret =CTOS_PowerMode(d_PWR_SLEEP_MODE);  
        }
        if((key !=d_KBD_1)&&(key !=d_KBD_2)){
            CTOS_LCDTClearDisplay();
            CTOS_LCDTPrintXY(1, 1, "Power Saving Test");
            if(ret == d_OK) CTOS_LCDTPrintXY(1, 9, "Test Success!");
            else CTOS_LCDTPrintXY(1, 9, "Test Fail!");
        }
        CTOS_Delay(1000);
        }
        goto Power_Saving_test;
        
        
        
/////16///////////      

Comm_test:
        i=j=k=0;
	CTOS_LCDTClearDisplay();
        CTOS_LCDTSetReverse(TRUE);
        CTOS_LCDTPrintXY(1, 1, "  Communicate Test  ");
        CTOS_LCDTSetReverse(FALSE);
        CTOS_LCDTPrintXY(1, 2, "1.COM1  2.COM2");
        CTOS_LCDTPrintXY(1, 3, "3.COM3        ");
        CTOS_LCDTPrintXY(1, 4, "4.Ethernet Test");
        CTOS_LCDTPrintXY(1, 5, "5.USB      Test");
        CTOS_LCDTPrintXY(1, 6, "6.Modem    Test");
        CTOS_LCDTPrintXY(1, 7, "7.GPRS     Test");
        //CTOS_LCDTPrintXY(1, 8, "8.ALL      Test");
        
        totalloop =1;
        while(1){
            CTOS_KBDGet(&key);
            if(key ==d_KBD_1) {comm=1; bPORT=d_COM1; goto COM;} if(key ==d_KBD_2) {comm=2; bPORT=d_COM2; goto COM;}
            if(key ==d_KBD_3) {comm=3; bPORT=d_COM3; goto COM;} if(key ==d_KBD_4) {comm=4; strcpy(HostIP,"192.120.100.213"); goto Ethernet_test; }  //COM3 only for V3p
            if(key ==d_KBD_5) {comm=5; goto USB;} if(key ==d_KBD_6) {comm=6; sprintf(dn, "803"); goto Modem;}
            if(key ==d_KBD_7) {comm=7; goto GPRS;} //if(key ==d_KBD_8) {comm=8; goto COM;}    
            if(key ==d_KBD_CANCEL)  goto Main;
        }    
        
        COM:  //for COM1 COM2 COM3
            CTOS_LCDTClearDisplay();
            CTOS_LCDTSetReverse(TRUE);
            CTOS_LCDTPrintXY(1,1,"  COM Transmit Test ");
            CTOS_LCDTSetReverse(FALSE);
            CTOS_LCDTPrintXY(1,2,"[1] Start test ");
            CTOS_LCDTPrintXY(1,3,"[2] Set RunLup ");
            CTOS_LCDTPrintXY(1,4,"[X] Back       ");
            
            while(1){
                loop=0;
                CTOS_KBDGet(&key);
                if(key ==d_KBD_CANCEL) goto Comm_test;
                if(key ==d_KBD_2) goto Loop;
                if(key ==d_KBD_1){
                    CTOS_LCDTPrintXY(1,2,"                    ");
                    CTOS_LCDTPrintXY(1,3,"                    ");
                    CTOS_LCDTPrintXY(1,4,"                    ");
                    ret=CTOS_RS232Open(bPORT, 38400,'N',8,1);   //Check cable connection
                    if(ret !=d_OK){
                        CTOS_Beep();
                        CTOS_LCDTPrintXY(1,2,"COM Connect Failed...");
                        CTOS_Delay(1000);
                        CTOS_RS232Close(bPORT);
                        goto COM;}
                    if(ret ==d_OK){
                        CTOS_LCDTPrintXY(1,2,"  COM Open Success  ");}
                    CTOS_RS232FlushTxBuffer(bPORT); CTOS_RS232FlushRxBuffer(bPORT); //Flush the data
                    while(1){
                        memset(Data,0x00,sizeof(Data)); //reset the data
                        for(i =0;i <0x08;i++){          //Write in the data
                            for(j=0;j<=0xff;j++){
                                k =j+i*0x100;
                                Data[k] =j;
                            }
                        }
                        
                        memset(str,0x00,sizeof(str));
                        loop++;
                        sprintf(str," Loop: %02d/%02d",loop,totalloop); //print out loop
                        CTOS_LCDTPrintXY(1,16,str);
                        
                        SendLen =2048; RecvLen=2048; //set send/recv default size
                        ////Prepare to send data
                        if(CTOS_RS232TxReady(bPORT)!=d_OK || CTOS_RS232TxData(bPORT,Data,SendLen)!=d_OK){
                            CTOS_Beep();
                            CTOS_LCDTPrintXY(1,3,"  Tx Not Ready...");
                            CTOS_Delay(1000);
                            break;
                        }
                        CTOS_LCDTPrintXY(1,3,"  Tx Ready          ");
                        CTOS_Delay(500);
                        CTOS_LCDTPrintXY(1,3,"-----Send Data------");
                        memset(str,0x00,sizeof(str)); 
                        sprintf(str,"Send->%d Bytes",SendLen);
                        CTOS_LCDTPrintXY(1,4,str);
                        CTOS_LCDTPrintXY(1,5,"Data:000102030405060708090A0B0C0D0E0F");
                        CTOS_LCDTPrintXY(1,6,"-----Recv Data------");
                        CTOS_LCDTPrintXY(1,7,"Receive>");
                        CTOS_Delay(500);
                        /////Recevie data check
                        if(CTOS_RS232RxReady(bPORT,&RecvLen) ==d_OK){   
                            if(RecvLen >SendLen) RecvLen=SendLen;
                            if(CTOS_RS232RxData(bPORT,Data,&RecvLen) !=d_OK) {
                                //memset(RecvLen,0X00,sizeof(RecvLen));
                                CTOS_LCDTPrintXY(1,8,"Data:00000000000000000000000000000000");
                            }
                        }
                        else{
                            CTOS_Beep();
                            CTOS_LCDTPrintXY(1,7,"Rx Not Ready...   ");
                            CTOS_Delay(1000);
                            break;}
                        
                        if(RecvLen >0){
                            memset(str,0x00,sizeof(str));
                            for(k =0;k<=SendLen;k+=16){
                                sprintf(str,"%dBytes",k);
                                CTOS_LCDTPrintXY(11,7,str);
                            }
                            CTOS_LCDTPrintXY(1,8,"FFFEFDFCFBFAF9F8F7F6F5F4F3F2F1F0");
                            CTOS_KBDHit(&key);
                            if(key ==d_KBD_CANCEL) break;
                            CTOS_Delay(500);
                        }
                        else{
                            CTOS_Beep();
                            CTOS_LCDTPrintXY(1,9,"Rx Data Error... ");
                            CTOS_Delay(500);
                            break;
                        }
                        
                        if(loop >=totalloop){
                            CTOS_LCDTPrintXY(1,3,"Test finish!!");
                            loop=0;
                            break;}
                    }    
                        CTOS_RS232FlushTxBuffer(bPORT); CTOS_RS232FlushRxBuffer(bPORT); //Flush the data
                        CTOS_RS232Close(bPORT);
                        
                        goto COM;
                }  
            }
                
            

Ethernet_test:
            CTOS_LCDTClearDisplay();
            CTOS_LCDTSetReverse(TRUE);
            CTOS_LCDTPrintXY(1,1,"Ethernet Test               ");
            CTOS_LCDTSetReverse(FALSE);
            
            CTOS_LCDTPrintXY(1,2,"[1] Go Test"); 
            CTOS_LCDTPrintXY(1,3,"[2] Set Host IP"); 
            CTOS_LCDTPrintXY(1,4,"[3] Set Eth MAC "); 
            CTOS_LCDTPrintXY(1,5,"[4] Ethernet Info "); 
            CTOS_LCDTPrintXY(1,6,"[5] Set Loop-Time");
            CTOS_LCDTPrintXY(1,7,"[X] Back"); 
            CTOS_LCDTPrintXY(1,9,"[Host IP]");
            CTOS_LCDTPrintXY(1,10,HostIP);
            
            while(1){
                CTOS_KBDGet(&key);
                if(key ==d_KBD_1) {loop=0;  goto Ether_Run;}
                if(key ==d_KBD_2) goto Set_IP;
                if(key ==d_KBD_3) goto Set_MAC;
                if(key ==d_KBD_4) goto Page1;
                if(key ==d_KBD_5) goto Loop; 
                if(key ==d_KBD_CANCEL) goto Comm_test;
            }
            
Ether_Run:
    CTOS_LCDTClearDisplay();
    CTOS_LCDTSetReverse(TRUE);
    CTOS_LCDTPrintXY(1,1,"Ethernet Test             ");
    CTOS_LCDTSetReverse(FALSE);       
            
    CTOS_LCDTPrintXY(1,9,"Open Lan......    ");
    CTOS_Delay(100);
    
    memset(temp, 0, sizeof(temp)); memcpy(temp, (BYTE*)ipad,strlen(ipad));
    if (CTOS_EthernetConfigSet(d_ETHERNET_CONFIG_IP, temp, strlen(temp)) != d_OK){
        CTOS_LCDTPrintXY(1, 9, " Set IP Fail.       ");
        CTOS_Delay(1000); goto Ethernet_test;}
    else CTOS_LCDTPrintXY(1, 9, " Set IP Finish.     "); CTOS_Delay(100);
    
    memset(temp, 0, sizeof(temp)); memcpy(temp, (BYTE*)mask,strlen(mask));
    if (CTOS_EthernetConfigSet(d_ETHERNET_CONFIG_MASK, temp, strlen(temp)) != d_OK){
        CTOS_LCDTPrintXY(1, 9, " Set Mask Fail.     ");
        CTOS_Delay(1000); goto Ethernet_test;}
    else CTOS_LCDTPrintXY(1, 9, " Set Mask Finish.   "); CTOS_Delay(100);
    
    memset(temp, 0, sizeof(temp)); memcpy(temp, (BYTE*)gate,strlen(gate));
    if (CTOS_EthernetConfigSet(d_ETHERNET_CONFIG_GATEWAY, temp, strlen(temp)) != d_OK){
        CTOS_LCDTPrintXY(1, 9, " Set Gate Fail.     ");
        CTOS_Delay(1000); goto Ethernet_test;}
    else CTOS_LCDTPrintXY(1, 9, " Set Gate Finish.   "); CTOS_Delay(100);
    
    memset(temp, 0, sizeof(temp)); memcpy(temp, (BYTE*)HostIP,strlen(HostIP));
    if (CTOS_EthernetConfigSet(d_ETHERNET_CONFIG_HOSTIP, temp, strlen(temp)) != d_OK){
        CTOS_LCDTPrintXY(1, 9, " Set Host Fail.     ");
        CTOS_Delay(1000); goto Ethernet_test;}
    else CTOS_LCDTPrintXY(1, 9, " Set Host Finish.   "); CTOS_Delay(100);
    
    memset(temp, 0, sizeof(temp)); memcpy(temp, (BYTE*)port,strlen(port));
    if (CTOS_EthernetConfigSet(d_ETHERNET_CONFIG_HOSTPORT, temp, strlen(temp)) != d_OK){
        CTOS_LCDTPrintXY(1, 9, " Set Port Fail.     ");
        CTOS_Delay(1000); goto Ethernet_test;}
    else CTOS_LCDTPrintXY(1, 9, " Set Port Finish.   "); CTOS_Delay(100);
    
    memset(temp, 0, sizeof(temp)); temp[0] = 0x32;
    if (CTOS_EthernetConfigSet(d_ETHERNET_CONFIG_AUTOCON, temp, 1) != d_OK){
        CTOS_LCDTPrintXY(1, 9, " Set Con-Mode Fail. ");
        CTOS_Delay(1000); goto Ethernet_test;}
    else CTOS_LCDTPrintXY(1, 9, " Set Con-Mode Finish."); CTOS_Delay(100);
    
    memset(temp, 0, sizeof(temp)); temp[0] = 0x30;
    if (CTOS_EthernetConfigSet(d_ETHERNET_CONFIG_DHCP, temp, 1) != d_OK){
        CTOS_LCDTPrintXY(1, 9, " Set IP Mode Fail.  ");
        CTOS_Delay(1000); goto Ethernet_test;}
    else CTOS_LCDTPrintXY(1, 9, " Set IP Mode Finish."); CTOS_Delay(100);
    
    if (CTOS_EthernetConfigSet(d_ETHERNET_CONFIG_UPDATE_EXIT, temp, 0) != d_OK){
        CTOS_LCDTPrintXY(1, 9, " Save Setting Fail. ");
        CTOS_Delay(1000); goto Ethernet_test;}
    else CTOS_LCDTPrintXY(1, 9, " Save Setting Finish."); CTOS_Delay(100);
    
    if (CTOS_EthernetOpen() != d_OK){
        CTOS_Beep(); CTOS_LCDTPrintXY(1, 9, " Ether Open Fail.   ");
        CTOS_Delay(1000); goto Ethernet_test;}
    else CTOS_LCDTPrintXY(1, 9, " Ether Open Success."); CTOS_Delay(100);
    
    if (CTOS_EthernetConnect() != d_OK){
        CTOS_Beep(); CTOS_LCDTPrintXY(1, 9, " Ether Disconnect.  ");
        CTOS_Delay(1500); goto Ethernet_test;}
    else CTOS_LCDTPrintXY(1, 9, " Ether Connecting..."); CTOS_Delay(200);

    
    
    while(1){
        memset(Data,0x00,sizeof(Data));
        for(i =0;i <0x08; i++){
            for(j =0;j <=0xFF; j++){
                k=j+i*0x100;
                Data[k] =j;}
        }
        SendLen =sizeof(Data);
        CTOS_LCDTPrintXY(1, 9, "                    ");
        
        memset(str,0x00,sizeof(str));
        loop++;
        sprintf(str," Loop: %02d/%02d",loop,totalloop); //print out loop
        CTOS_LCDTPrintXY(1,16,str);
        
        
        SendLen=2048; RecvLen=2048;
        CTOS_EthernetStatus(&Status);
        if(!(Status & d_STATUS_ETHERNET_CONNECTED) || CTOS_EthernetTxReady() != d_OK || CTOS_EthernetTxData(Data, SendLen) != d_OK){
            CTOS_Beep();
            CTOS_LCDTPrintXY(1, 10, " Tx Not Ready...      ");
            CTOS_Delay(1500);
            break;}
        CTOS_LCDTPrintXY(1,3,"-----Send Data------");
        CTOS_LCDTPrintXY(1,3,"  Tx Ready             ");
        memset(str,0x00,sizeof(str)); 
        sprintf(str,"Send->%d Bytes",SendLen);
        CTOS_LCDTPrintXY(1,4,str);
        CTOS_LCDTPrintXY(1,5,"Data:000102030405060708090A0B0C0D0E0F");
        CTOS_LCDTPrintXY(1,6,"-----Recv Data------");
        CTOS_LCDTPrintXY(1,7,"Receive>");
        CTOS_Delay(500);

        if(CTOS_EthernetRxReady(&RecvLen) ==d_OK){   
            if(RecvLen >SendLen) RecvLen=SendLen;
            if(CTOS_EthernetRxData(Data,&RecvLen) !=d_OK) {
            //memset(RecvLen,0X00,sizeof(RecvLen));
               CTOS_LCDTPrintXY(1,8,"Data:00000000000000000000000000000000");
            }
        }
        else{
            CTOS_Beep();
            CTOS_LCDTPrintXY(1,7,"Rx Not Ready...   ");
            CTOS_Delay(1000);
            break;}
        
        if(RecvLen >0){
            memset(str,0x00,sizeof(str));
            for(k =0;k<=SendLen;k+=16){
                sprintf(str,"%dBytes",k);
                CTOS_LCDTPrintXY(11,7,str);
            }
            CTOS_LCDTPrintXY(1,8,"FFFEFDFCFBFAF9F8F7F6F5F4F3F2F1F0");
            CTOS_KBDHit(&key);
            if(key ==d_KBD_CANCEL) break;
            CTOS_Delay(500);
        }
        else{
            CTOS_Beep();
            CTOS_LCDTPrintXY(1,9,"Rx Data Error... ");
            CTOS_Delay(500);
            break;
        }
                        
        if(loop >=totalloop){
        CTOS_LCDTPrintXY(1,3,"Test finish!!");
        CTOS_Delay(500);
        loop=0;
        break;}
    }
    CTOS_EthernetDisconnect();
    CTOS_EthernetClose();
    goto Ethernet_test;
    
Set_IP:
    CTOS_LCDTClearDisplay();
    CTOS_LCDTSetReverse(TRUE);
    CTOS_LCDTPrintXY(1,1,"Ethernet Config      ");
    CTOS_LCDTSetReverse(FALSE);
    CTOS_LCDTPrintXY(1,2,"Set Host IP: ");
    CTOS_LCDTPrintXY(1,3,">");
    CTOS_LCDTPrintXY(1,4,"IP Max Value:");
    CTOS_LCDTPrintXY(1,5,"[223.255.255.255]");
    CTOS_LCDTPrintXY(1,6,"Range:");
    CTOS_LCDTPrintXY(1,7," First :1~223");
    CTOS_LCDTPrintXY(1,8," Others:0~255");

    while(1){
        HostIPlength =sizeof(HostIP);
        memset(HostIP,0x00,sizeof(HostIP));
        CTOS_UIKeypad(2, 3, ipKeyBoardLayout, 40, 80, d_TRUE, d_FALSE, 0, 0, HostIP, 16);
        HostIPlength =strlen(HostIP);
        ret =CTOS_EthernetConfigSet(d_ETHERNET_CONFIG_HOSTIP, HostIP, HostIPlength);
        if(ret ==d_OK) {
            CTOS_LCDTPrintXY(1,9,"Set Host IP OK "); 
            goto Ethernet_test;
        }
        if(ret !=d_OK) {
            CTOS_LCDTPrintXY(1,9,"Set IP  Failed ");
            strcpy(HostIP,"192.120.100.213");
            CTOS_Delay(500);
            CTOS_LCDTPrintXY(1,10,"Press any key reset");
            CTOS_KBDGet(&key);
            if(key ==d_KBD_CANCEL) goto Ethernet_test;
        }
    }
    
    
Set_MAC:
    CTOS_LCDTClearDisplay();
    CTOS_LCDTSetReverse(TRUE);
    CTOS_LCDTPrintXY(1,1,"Ethernet Set MAC       ");
    CTOS_LCDTSetReverse(FALSE);
    CTOS_LCDTPrintXY(1,3,"MAC address:");
    memset(temp, 0x00, sizeof(temp)); length = sizeof(temp);
    CTOS_EthernetConfigGet(d_ETHERNET_CONFIG_MAC, temp, &length);
    CTOS_LCDTPrintXY(2, 5, temp);
    CTOS_LCDTPrintXY(1,7," < No Need to Set >");
    CTOS_KBDGet(&key); goto Ethernet_test;
    
    
Page1:
    CTOS_LCDTClearDisplay();
    CTOS_LCDTSetReverse(TRUE);
    CTOS_LCDTPrintXY(1, 1, "   Ethernet Info        ");
    CTOS_LCDTSetReverse(FALSE);
    CTOS_LCDTPrintXY(1, 3, " Ethernet : ONCHIP  ");
    CTOS_LCDTPrintXY(1, 4, " Local :            ");
    memset(temp, 0x00, sizeof(temp)); length = sizeof(temp);
    CTOS_EthernetConfigGet(d_ETHERNET_CONFIG_IP, temp, &length);
    CTOS_LCDTPrintXY(2, 5, temp); CTOS_LCDTPrintXY(1, 6, " Mask :             ");
    memset(temp, 0x00, sizeof(temp)); length = sizeof(temp);
    CTOS_EthernetConfigGet(d_ETHERNET_CONFIG_MASK, temp, &length);
    CTOS_LCDTPrintXY(2, 7, temp); CTOS_LCDTPrintXY(1, 8, " Gateway :          ");
    memset(temp, 0x00, sizeof(temp)); length = sizeof(temp);
    CTOS_EthernetConfigGet(d_ETHERNET_CONFIG_GATEWAY, temp, &length);
    CTOS_LCDTPrintXY(2, 9, temp); CTOS_LCDTPrintXY(1, 10, " Host IP :         ");
    memset(temp, 0x00, sizeof(temp)); length = sizeof(temp);
    CTOS_EthernetConfigGet(d_ETHERNET_CONFIG_HOSTIP, temp, &length);
    CTOS_LCDTPrintXY(2, 11, temp); CTOS_LCDTPrintXY(1, 12, " Host Port :       ");
    memset(temp, 0x00, sizeof(temp)); length = sizeof(temp);
    CTOS_EthernetConfigGet(d_ETHERNET_CONFIG_HOSTPORT, temp, &length);
    CTOS_LCDTPrintXY(2, 13, temp); CTOS_LCDTPrintXY(1, 16, " [.] Go Next    1/2");

    while(1){
        CTOS_KBDGet(&key);
        if (key == d_KBD_CANCEL) goto Ethernet_test; if (key == d_KBD_DOT) goto Page2;}

Page2:
    CTOS_LCDTClearDisplay();
    CTOS_LCDTSetReverse(TRUE);
    CTOS_LCDTPrintXY(1, 1, "   Ethernet Info        ");
    CTOS_LCDTSetReverse(FALSE);
    CTOS_LCDTPrintXY(1, 3, " MAC Adress :       ");
    memset(temp, 0x00, sizeof(temp)); length = sizeof(temp);
    CTOS_EthernetConfigGet(d_ETHERNET_CONFIG_MAC, temp, &length);
    CTOS_LCDTPrintXY(2, 4, temp); CTOS_LCDTPrintXY(1, 5, " Connect Mode :     ");
    memset(temp, 0x00, sizeof(temp)); length = sizeof(temp);
    CTOS_EthernetConfigGet(d_ETHERNET_CONFIG_AUTOCON, temp, &length);
    CTOS_LCDTPrintXY(17, 5, temp); CTOS_LCDTPrintXY(1, 6, " IP Mode :          ");
    memset(temp, 0x00, sizeof(temp)); length = sizeof(temp);
    CTOS_EthernetConfigGet(d_ETHERNET_CONFIG_DHCP, temp, &length);
    CTOS_LCDTPrintXY(12, 6, temp); CTOS_LCDTPrintXY(1, 7, " DNS Server :       ");
    memset(temp, 0x00, sizeof(temp)); length = sizeof(temp);
    CTOS_EthernetConfigGet(d_ETHERNET_CONFIG_DNSIP, temp, &length);
    CTOS_LCDTPrintXY(2, 8, temp); CTOS_LCDTPrintXY(1, 9, " Host URL :       ");
    memset(temp, 0x00, sizeof(temp)); length = sizeof(temp);
    CTOS_EthernetConfigGet(d_ETHERNET_CONFIG_HOSTURL, temp, &length);
    CTOS_LCDTPrintXY(2, 10, temp); CTOS_LCDTPrintXY(1, 16, " [^] Go Back    2/2");
    
    while(1){
        CTOS_KBDGet(&key);
        if (key == d_KBD_CANCEL) goto Ethernet_test; if (key == d_KBD_00) goto Page1;}
        
            
 
USB:
    CTOS_LCDTClearDisplay();
    CTOS_LCDTSetReverse(TRUE);
    CTOS_LCDTPrintXY(1,1,"  USB Connect Test  ");    
    CTOS_LCDTSetReverse(FALSE);
    CTOS_LCDTPrintXY(1,3,"[1] Device USB"); 
    CTOS_LCDTPrintXY(1,4,"[2] Cradle USB"); 
    CTOS_LCDTPrintXY(1,5,"[3] Set Loop-Time"); 
    CTOS_LCDTPrintXY(1,6,"[X] Back");
    //CTOS_LCDTPrintXY(1,7,"Loop-Time:");
    
    while(1){
        CTOS_KBDGet(&key); 
        if(key ==d_KBD_1) {loop=0; goto Device_USB;}
        if(key ==d_KBD_2) {loop=0; goto Cradle_USB;}
        if(key ==d_KBD_3) goto Loop;
        if(key ==d_KBD_CANCEL) goto Comm_test;
    }
      

Device_USB:
    CTOS_LCDTClearDisplay();
    CTOS_LCDTSetReverse(TRUE);
    CTOS_LCDTPrintXY(1,1,"  Device USB Test   ");
    CTOS_LCDTSetReverse(FALSE);
    
    if(CTOS_USBOpen() !=d_OK){
        CTOS_LCDTPrintXY(1,2,"USB Open Failed...");
        goto USB;
    }
    
    while(1){
        CTOS_USBTxFlush(); CTOS_USBRxFlush();
        memset(Data,0x00,sizeof(Data)); //reset the data
        for(i =0;i <0x08;i++){          //Write in the data
            for(j=0;j<=0xff;j++){
                k =j+i*0x100;
                Data[k] =j;
                }
        }              
        memset(str,0x00,sizeof(str));
        loop++;
        sprintf(str," Loop: %02d/%02d",loop,totalloop); //print out loop
        CTOS_LCDTPrintXY(1,16,str);
        SendLen =2048; RecvLen=2048; //set send/recv default size
         ////Prepare to send data
        if(CTOS_USBTxReady()!=d_OK || CTOS_USBTxData(Data,SendLen)!=d_OK){   ///LLL
            CTOS_Beep();
            CTOS_LCDTPrintXY(1,3,"  Tx Not Ready...");
            CTOS_Delay(1000);
            break;
        }
        CTOS_LCDTPrintXY(1,3,"  Tx Ready          ");
        CTOS_Delay(500);
        CTOS_LCDTPrintXY(1,3,"-----Send Data------");
        memset(str,0x00,sizeof(str)); 
        sprintf(str,"Send->%d Bytes",SendLen);
        CTOS_LCDTPrintXY(1,4,str);
        CTOS_LCDTPrintXY(1,5,"Data:000102030405060708090A0B0C0D0E0F");
        CTOS_LCDTPrintXY(1,6,"-----Recv Data------");
        CTOS_LCDTPrintXY(1,7,"Receive>");
        CTOS_Delay(500);
        /////Recevie data check
        if(CTOS_USBRxReady(&RecvLen) ==d_OK){   
            if(RecvLen >SendLen) RecvLen=SendLen;
            if(CTOS_USBRxData(Data,&RecvLen) !=d_OK) {
                 //memset(RecvLen,0X00,sizeof(RecvLen));
                CTOS_LCDTPrintXY(1,8,"Data:00000000000000000000000000000000");
            }
        }
        else{
            CTOS_Beep();
            CTOS_LCDTPrintXY(1,7,"Rx Not Ready...   ");
            CTOS_Delay(1000);
            break;}
                        
        if(RecvLen >0){
            memset(str,0x00,sizeof(str));
            for(k =0;k<=SendLen;k+=16){
                sprintf(str,"%dBytes",k);
                CTOS_LCDTPrintXY(11,7,str);
            }
            CTOS_LCDTPrintXY(1,8,"FFFEFDFCFBFAF9F8F7F6F5F4F3F2F1F0");
            CTOS_KBDHit(&key);
            if(key ==d_KBD_CANCEL) break;
            CTOS_Delay(500);
        }
        else{
            CTOS_Beep();
            CTOS_LCDTPrintXY(1,9,"Rx Data Error... ");
            CTOS_Delay(500);
            break;
        }
                        
        if(loop >=totalloop){
            CTOS_LCDTPrintXY(1,3,"Test finish!!");
            CTOS_Delay(500);
            loop=0;
            break;}
    }    
    CTOS_USBTxFlush(); CTOS_USBRxFlush(); //Flush the data
    CTOS_USBClose();                     
    goto USB;
     
  
    
    
Cradle_USB:
    CTOS_LCDTClearDisplay();
    CTOS_LCDTSetReverse(TRUE);
    CTOS_LCDTPrintXY(1,1,"  Device BaseUSB Test   ");
    CTOS_LCDTSetReverse(FALSE);
    
    if(CTOS_BaseUSBOpen() !=d_OK){
        CTOS_LCDTPrintXY(1,2,"BaseUSB Open Failed...");
        goto USB;
    }
    
    while(1){
        CTOS_BaseUSBTxFlush(); CTOS_BaseUSBRxFlush();
        memset(Data,0x00,sizeof(Data)); //reset the data
        for(i =0;i <0x08;i++){          //Write in the data
            for(j=0;j<=0xff;j++){
                k =j+i*0x100;
                Data[k] =j;
                }
        }              
        memset(str,0x00,sizeof(str));
        loop++;
        sprintf(str," Loop: %02d/%02d",loop,totalloop); //print out loop
        CTOS_LCDTPrintXY(1,16,str);
        SendLen =2048; RecvLen=2048; //set send/recv default size
         ////Prepare to send data
        if(CTOS_BaseUSBTxReady()!=d_OK || CTOS_BaseUSBTxData(Data,SendLen)!=d_OK){   ///LLL
            CTOS_Beep();
            CTOS_LCDTPrintXY(1,3,"  Tx Not Ready...");
            CTOS_Delay(1000);
            break;
        }
        CTOS_LCDTPrintXY(1,3,"  Tx Ready          ");
        CTOS_Delay(500);
        CTOS_LCDTPrintXY(1,3,"-----Send Data------");
        memset(str,0x00,sizeof(str)); 
        sprintf(str,"Send->%d Bytes",SendLen);
        CTOS_LCDTPrintXY(1,4,str);
        CTOS_LCDTPrintXY(1,5,"Data:000102030405060708090A0B0C0D0E0F");
        CTOS_LCDTPrintXY(1,6,"-----Recv Data------");
        CTOS_LCDTPrintXY(1,7,"Receive>");
        CTOS_Delay(500);
        /////Recevie data check
        if(CTOS_BaseUSBRxReady(&RecvLen) ==d_OK){   
            if(RecvLen >SendLen) RecvLen=SendLen;
            if(CTOS_BaseUSBRxData(Data,&RecvLen) !=d_OK) {
                 //memset(RecvLen,0X00,sizeof(RecvLen));
                CTOS_LCDTPrintXY(1,8,"Data:00000000000000000000000000000000");
            }
        }
        else{
            CTOS_Beep();
            CTOS_LCDTPrintXY(1,7,"Rx Not Ready...   ");
            CTOS_Delay(1000);
            break;}
                        
        if(RecvLen >0){
            memset(str,0x00,sizeof(str));
            for(k =0;k<=SendLen;k+=16){
                sprintf(str,"%dBytes",k);
                CTOS_LCDTPrintXY(11,7,str);
            }
            CTOS_LCDTPrintXY(1,8,"FFFEFDFCFBFAF9F8F7F6F5F4F3F2F1F0");
            CTOS_KBDHit(&key);
            if(key ==d_KBD_CANCEL) break;
            CTOS_Delay(500);
        }
        else{
            CTOS_Beep();
            CTOS_LCDTPrintXY(1,9,"Rx Data Error... ");
            CTOS_Delay(500);
            break;
        }
                        
        if(loop >=totalloop){
            CTOS_LCDTPrintXY(1,3,"Test finish!!");
            break;}
    }    
    CTOS_BaseUSBTxFlush(); CTOS_BaseUSBRxFlush(); //Flush the data
    CTOS_BaseUSBClose();                     
    goto USB;        



Modem:
    CTOS_LCDTClearDisplay();
    CTOS_LCDTSetReverse(TRUE);
    CTOS_LCDTPrintXY(1,1,"  Modem Test        ");
    CTOS_LCDTSetReverse(FALSE);
    CTOS_LCDTPrintXY(1,3," [Dialup Number]   ");
    CTOS_LCDTPrintXY(1,4,"> ");
    CTOS_LCDTPrintXY(3,4,dn);
    CTOS_LCDTPrintXY(1,6,"[1] Start Test       ");
    CTOS_LCDTPrintXY(1,7,"[2] Set Dialup Num   ");
    //TOS_LCDTPrintXY(1,8,"[3] Set Loop-Time    ");
    CTOS_LCDTPrintXY(1,10,"[X] Exit             ");
    
    while(1){
        CTOS_KBDGet(&key); 
        if(key ==d_KBD_1){loop=0; goto Modem_test;}
        if(key ==d_KBD_2) goto Dialup_Num;
        //if(key ==d_KBD_3) goto Loop;
        if(key ==d_KBD_CANCEL) goto Comm_test;
    }
      
Modem_test:
    CTOS_LCDTClearDisplay();
    CTOS_LCDTSetReverse(TRUE);
    CTOS_LCDTPrintXY(1,1,"  Modem Test        ");
    CTOS_LCDTSetReverse(FALSE);
    while(1){
        CTOS_LCDTPrintXY(1,2,"Modem Opening...");
        CTOS_Delay(500);
        //open Modem
        //CTOS_ModemOpen(d_M_MODE_AYNC_NORMAL, d_M_HANDSHAKE_V22_ONLY, d_M_COUNTRY_TAIWAN);
        CTOS_ModemOpen(d_M_MODE_AYNC_NORMAL, d_M_HANDSHAKE_V34_ONLY, d_M_COUNTRY_TAIWAN);
        while(1){
        ret =CTOS_ModemStatus(&Status);
        if(ret !=d_OK){
            CTOS_LCDTPrintXY(1,3,"Modem Open Failed...");
            goto Modem;
        }
        if(ret ==d_OK){
            CTOS_LCDTPrintXY(1,3,"Modem Open Success.");
            CTOS_Delay(1000);
            break;}
        }
        
        CTOS_LCDTPrintXY(1,3,"Get Modem number:   ");
        CTOS_LCDTPrintXY(1,4,dn);
        CTOS_LCDTPrintXY(1,5,"Dialing...");
        CTOS_Delay(500);
        CTOS_ModemDialup((BYTE*)dn,strlen(dn));
        while(1){   //check the Modem Status
            ret =CTOS_ModemStatus(&Status);
            if(!(Status &d_M_STATUS_DIALING )){
                if(Status &d_M_STATUS_MODEM_ONLINE){
                    CTOS_LCDTPrintXY(1,5,"Dial Success.      ");
                    CTOS_Delay(100);
                    break;}
                else{
                    CTOS_LCDTPrintXY(1,5,"Dial Failed...     ");
                    CTOS_ModemHookOn();
                    CTOS_KBDGet(&key); 
                    goto Modem; }
            }
        }
        
        SendLen =2048; RecvLen=2048; //set send/recv default size
        memset(Data,0x00,sizeof(Data)); //reset the data
        k=0;
        for(i =0;i <0x08;i++){          //Write in the data
            for(j=0;j<=0xFF;j++){
                k =j+i*0x100;
                Data[k] =j;
                }
        }    
        SendLen = sizeof(Data);
        

        memset(str,0x00,sizeof(str));
        //loop
        loop++;
        sprintf(str," Loop: %02d/%02d",loop,totalloop); //print out loop
        CTOS_LCDTPrintXY(1,16,str);
        ///////////////
        
         ////Prepare to send data
        if(CTOS_ModemTxReady()!=d_OK || CTOS_ModemTxData(Data,SendLen)!=d_OK){   ///LLL
            CTOS_Beep();
            CTOS_LCDTPrintXY(1,3,"  Tx Not Ready...");
            CTOS_Delay(1000);
            break;
        }
        
        while(1){
            CTOS_ModemStatus(&Status);
            CTOS_Delay(100);
            if((Status & d_M_STATUS_DIALING) || (Status & d_M_STATUS_TX_BUSY))  continue;
            else break;}
        
        CTOS_LCDTPrintXY(1,3,"  Tx Ready          ");
        CTOS_Delay(500);
        CTOS_LCDTPrintXY(1,3,"-----Send Data------");
        memset(str,0x00,sizeof(str)); 
        sprintf(str,"Send->%d Bytes",SendLen);
        CTOS_LCDTPrintXY(1,4,str);
        CTOS_LCDTPrintXY(1,5,"Data:000102030405060708090A0B0C0D0E0F");
        CTOS_LCDTPrintXY(1,6,"-----Recv Data------");
        CTOS_LCDTPrintXY(1,7,"Receive>");
        CTOS_Delay(500);
        memset(rData,0x00,sizeof(rData));
        /////Recevie data check
        /*
        while(1){
            ret =CTOS_ModemRxReady(&RecvLen);
            CTOS_ModemStatus(&Status);
            if(ret ==d_OK){
                CTOS_LCDTPrintXY(1,8,"Receive Data...");
            }
            else{
                CTOS_Beep();
                CTOS_LCDTPrintXY(1,8,"Receive Failed.  ");
                CTOS_Delay(1000);
                goto Modem_test;
            }
            ret =CTOS_ModemRxData(rData,&RecvLen);
            RecvLen =strlen(rData);
            
        if(ret ==d_OK){
            if(RecvLen >=2048){
            CTOS_LCDTPrintXY(1,8,"FFFEFDFCFBFAF9F8F7F6F5F4F3F2F1F0");
            CTOS_LCDTPrintXY(1,9,"Modem Rx Success... ");
            CTOS_Delay(100);
            CTOS_KBDGet(&key);
            break;
            }
        }
        else{
            CTOS_LCDTPrintXY(1,8,"Modem Rx Failed...");
            CTOS_KBDGet(&key);
            goto Modem_test;
            }
        }
         */
        while(1){
                //Check if the data is currently available in Modem //
                //ret = CTOS_ModemRxReady(&usr_Len);
                ret =CTOS_ModemRxReady(&RecvLen);
                if (ret == d_OK){
                //Receive data via the modem channel //
                //CTOS_ModemRxData(&babuff[usR_Len],&usr_Len);
                CTOS_ModemRxData(&rData[RecvLen],&RecvLen);
                CTOS_LCDTPrintXY(1,8,"FFFEFDFCFBFAF9F8F7F6F5F4F3F2F1F0");
                CTOS_LCDTPrintXY(1,9,"Modem Rx Success... ");
                CTOS_Delay(1000);
                }
                else{
                CTOS_LCDTPrintXY(1, 8, "RxReady Failed      ");
                }
                CTOS_KBDHit(&key);
                //Go back to main() //
                if (key == d_KBD_CANCEL){
                //Clear the receive buffer of Modem //
                CTOS_ModemFlushRxData();
                break;
                }
        }
        
        if(loop >=totalloop){
            CTOS_LCDTPrintXY(1,10,"Test finish!!");
            CTOS_Delay(500);
            loop=0;
            break;}
        
    }    
  //  CTOS_ModemFlushRxData(); //Flush the data
    CTOS_KBDGet(&key);
    //CTOS_ModemHookOn();               
    goto Modem;
    
    
        
Dialup_Num:
    CTOS_LCDTClearDisplay();
    CTOS_LCDTSetReverse(TRUE);
    CTOS_LCDTPrintXY(1,1,"  Modem Test        ");
    CTOS_LCDTSetReverse(FALSE);
    
    CTOS_LCDTPrintXY(1,2,"Dialup Num:         ");
    while(1){
        CTOS_UIKeypad(2, 3, keyboardLayoutNumber, 40, 80, d_TRUE, d_FALSE, 0, 0,phonenum, 16);
        sprintf(dn,"%s",phonenum);
        CTOS_LCDTPrintXY(1,5,"Set Number Success!");
        CTOS_Delay(1000);
        goto Modem;
    }
    
    

        
GPRS:
    CTOS_LCDTClearDisplay();
    CTOS_LCDTSetReverse(TRUE);
    CTOS_LCDTPrintXY(1,1," GSM/GPRS Test V7   ");
    CTOS_LCDTSetReverse(FALSE);
    CTOS_Delay(1000);
    CTOS_TCP_GPRSInit();
    CTOS_LCDTPrintXY(1,2,"Select APN:");
    CTOS_LCDTPrintXY(1,3,"[1] internet");
    CTOS_LCDTPrintXY(1,4,"[2] CM-net");
    CTOS_LCDTPrintXY(1,5,"[3] CT-net");
    CTOS_LCDTPrintXY(1,6,"[4] User Input");
    CTOS_LCDTPrintXY(1,7,"[X] Cancel");
    i=0;
    
    while(1){
        CTOS_KBDGet(&key);
        if(key ==d_KBD_1){sprintf(APN,"internet"); goto GPRS_A;}
        if(key ==d_KBD_2){sprintf(APN,"cmnet"); goto GPRS_A;}
        if(key ==d_KBD_3){sprintf(APN,"ctent"); goto GPRS_A;}
        if(key ==d_KBD_4){
            CTOS_LCDTClearDisplay();
            CTOS_LCDTPrintXY(1,3,"Input APN");
            CTOS_LCDTPrintXY(1,4,">");
            CTOS_UIKeypad(2,4, keyboardLayoutEnglish, 40, 80, d_TRUE, d_FALSE, 0, '\0', APN, 21);
            goto GPRS_A;
        }
        if(key ==d_KBD_CANCEL) goto Main;
    }
    
GPRS_A:
    CTOS_GSMOpen(115200,TRUE);
    CTOS_LCDTClearDisplay();
    CTOS_LCDTSetReverse(TRUE);
    CTOS_LCDTPrintXY(1,1," GSM/GPRS Test V7   ");
    CTOS_LCDTSetReverse(FALSE);        
    CTOS_LCDTPrintXY(1,5," Detecting SIM....  ");
    CTOS_Delay(50);
    CTOS_GSMSelectSIM(SIM[i]);
    CTOS_LCDTPrintXY(1,5,"                    ");
    CTOS_LCDTPrintXY(1,3," Slot  :");
    CTOS_LCDTPrintXY(1,4," Signal:");     
    CTOS_LCDTPrintXY(1,5," States:"); 
    CTOS_LCDTPrintXY(1,7,"[0] Ropen SIM"); 
    CTOS_LCDTPrintXY(1,9,"[2] Select SIM Slot"); 
    CTOS_LCDTPrintXY(1,13,"[X] Exit");
    
    while(1){
        CTOS_GSMSignalQuality(&GSM_S);
        if(CTOS_SIMCheckReady() !=d_GSM_SIM_READY){
            if(i ==0) CTOS_LCDTPrintXY(9,3,"SIM1/None   ");
            else CTOS_LCDTPrintXY(9,3,"SIM2/None   ");
        }
        else{
            if(i ==0) CTOS_LCDTPrintXY(9,3,"SIM1/Ready  ");
            else CTOS_LCDTPrintXY(9,3,"SIM2/Ready  ");
        }
        CTOS_GPRSGetRegState(&GSM_R);
        if(GSM_R ==GSM_RState[0]) CTOS_LCDTPrintXY(11,5,"0_Not Reg ");
        else if(GSM_R ==GSM_RState[1]) CTOS_LCDTPrintXY(11,5,"1_Reg     ");
        else if(GSM_R ==GSM_RState[2]) CTOS_LCDTPrintXY(11,5,"2_Trying  ");
        else if(GSM_R ==GSM_RState[3]) CTOS_LCDTPrintXY(11,5,"3_Deny    ");
        else if(GSM_R ==GSM_RState[4]) CTOS_LCDTPrintXY(11,5,"4_Unknow  ");
        else if(GSM_R ==GSM_RState[5]) CTOS_LCDTPrintXY(11,5,"5_Roam    ");
        
        CTOS_LCDTPrintXY(1,8,"[1] GSM Test");
        CTOS_LCDTPrintXY(1,10,"[3] GPRS Test");
        CTOS_LCDTPrintXY(1,11,"[4] GSM Info");
        
        CTOS_Delay(50); memset(GM_buff, 0x00, sizeof(GM_buff));
        sprintf(GM_buff, "%d", GSM_S); CTOS_LCDTPrintXY (11, 4, GM_buff);
        CTOS_KBDHit(&key); if (key == d_KBD_CANCEL) goto GPRS;
        if (key == d_KBD_0) goto GPRS;
        if (key == d_KBD_2){
            CTOS_LCDTClearDisplay();
            CTOS_LCDTSetReverse(TRUE);
            CTOS_LCDTPrintXY(1, 1, "  Select SIM Slot   ");
            CTOS_LCDTSetReverse(FALSE);
            CTOS_LCDTPrintXY(3, 4, " [1] SIM 1");
            CTOS_LCDTPrintXY(3, 5, " [2] SIM 2");
            CTOS_LCDTPrintXY(3, 7, " [X] Back");
            
            while(1){
                CTOS_KBDGet(&key);
                if (key == d_KBD_CANCEL) goto GPRS_A;
                if (key == d_KBD_1){
                    i = 0; CTOS_LCDTPrintXY(1, 10, " Select SIM 1 Slot. ");}
                if (key == d_KBD_2){
                    i = 1; CTOS_LCDTPrintXY(1, 10, " Select SIM 2 Slot. ");}
                if (key == d_KBD_1 || key == d_KBD_2){
                    CTOS_LCDTPrintXY(1, 11, " APN : ");
                    CTOS_LCDTPrintXY(8, 11, APN); CTOS_Delay(2000); goto GPRS_A;}
            }
        }
        if (GSM_R == GSM_RState[1]){
            if (key == d_KBD_1) goto GPRS_GSM; if (key == d_KBD_3) goto GPRS_Test;
            if (key == d_KBD_4){
                CTOS_LCDTClearDisplay();
                CTOS_LCDTSetReverse(TRUE);
                CTOS_LCDTPrintXY(1, 1, "  GSM Information   ");
                CTOS_LCDTSetReverse(FALSE);
                CTOS_LCDTPrintXY(1, 3, " Version : ");
                GSM_Get_Version(GM_buff, &Len); CTOS_LCDTPrintXY(2, 4, GM_buff);
                CTOS_LCDTPrintXY(1, 6, " IMEI : ");
                CTOS_GSMGetIMEI(IMEI); CTOS_LCDTPrintXY(2, 7, IMEI);
                CTOS_LCDTPrintXY(1, 16, "...Press key to back");
                CTOS_KBDGet(&key); goto GPRS_A;}
        }
    }
    
GPRS_GSM:
    CTOS_LCDTClearDisplay();
    CTOS_LCDTSetReverse(TRUE);
    CTOS_LCDTPrintXY(1, 1, "      GSM Test      ");
    CTOS_LCDTSetReverse(FALSE);
    CTOS_LCDTPrintXY(1, 3, " [1] GSM Dial");
    CTOS_LCDForeGndColor(0x00D0D0D0);
    CTOS_LCDTPrintXY(1, 4, " [2] Send Data");
    CTOS_LCDTPrintXY(1, 5, " [3] Receive Data");
    CTOS_LCDTPrintXY(1, 6, " [4] Disconnect");
    CTOS_LCDForeGndColor(0x00000000);
    CTOS_LCDTPrintXY(1, 7, " [5] Voice Call");
    CTOS_LCDTPrintXY(1, 8, " [6] Send SMS");
    CTOS_LCDTPrintXY(1, 10, " [X] Back");

    while(1){
        CTOS_KBDGet(&key);
        if (key == d_KBD_CANCEL) goto GPRS_A;
        if (key == d_KBD_1){
            CTOS_LCDTClearDisplay();
            CTOS_LCDTSetReverse(TRUE);
            CTOS_LCDTPrintXY(1, 1, "      GSM Dial      ");
            CTOS_LCDTSetReverse(FALSE);
            CTOS_LCDTPrintXY(1, 3, " Dial phone number.");
            CTOS_LCDTPrintXY(1, 4, " > ");
            CTOS_UIKeypad(4, 4, keyboardLayoutNumber, 40, 80, d_TRUE, \
                          d_FALSE, 0, 0, GM_buff, 21); Len = strlen(GM_buff);
            GM_buff[Len++] = ';'; CTOS_LCDTPrintXY(1, 9, "---Report Message---");
            CTOS_LCDTPrintXY(1, 10, " > Dialing...");
            rat = CTOS_GSMGetModuleOpMode(&GSM_Lbuff);
            if (rat == d_GSM_MODULE_CMD_MODE) CTOS_LCDTPrintXY(1, 11, " > Work on CMD mode");
            else CTOS_LCDTPrintXY(1, 11, " > Work on DATA mode");
            rat = CTOS_GSMDial(GM_buff, Len);
            if (rat != d_OK){
                CTOS_LCDTPrintXY(1, 12, " > Dial Fail...");
                sprintf(GM_buff, " > Error : 0x%04x", rat);
                CTOS_LCDTPrintXY(1, 13, GM_buff);}
            else CTOS_LCDTPrintXY(1, 12, " > Dial Success.");
            CTOS_KBDGet(&key); goto GPRS_GSM;}
//        if (key == d_KBD_2) goto GPRS_GSM;
//        if (key == d_KBD_3) goto GPRS_GSM;
//        if (key == d_KBD_4) goto GPRS_GSM;
        if (key == d_KBD_5){
            CTOS_LCDTClearDisplay();
            CTOS_LCDTSetReverse(TRUE);
            CTOS_LCDTPrintXY(1, 1, "      GSM Dial      ");
            CTOS_LCDTSetReverse(FALSE);
            CTOS_LCDTPrintXY(1, 3, " Dial phone number.");
            CTOS_LCDTPrintXY(1, 4, " > ");
            CTOS_UIKeypad(4, 4, keyboardLayoutNumber, 40, 80, d_TRUE, \
                          d_FALSE, 0, 0, GM_buff, 21);
            Len = strlen(GM_buff); GM_buff[Len++] = ';';
            CTOS_LCDTPrintXY(1, 9, "---Report Message---");
            CTOS_LCDTPrintXY(1, 10, " > Dialing...");
            rat = CTOS_GSMGetModuleOpMode(&GSM_Lbuff);
            if (rat == d_GSM_MODULE_CMD_MODE) CTOS_LCDTPrintXY(1, 11, " > Work on CMD mode");
            else CTOS_LCDTPrintXY(1, 11, " > Work on DATA mode");
            rat = CTOS_GSMDial(GM_buff, Len);
            if (rat != d_OK){
                CTOS_LCDTPrintXY(1, 12, " > Dial Fail...");
                sprintf(GM_buff, " > Error : 0x%04x", rat);
                CTOS_LCDTPrintXY(1, 13, GM_buff);}
            else CTOS_LCDTPrintXY(1, 12, " > Dial Success.");
            CTOS_KBDGet(&key); goto GPRS_GSM;}
        if (key == d_KBD_6){
            CTOS_LCDTClearDisplay();
            CTOS_LCDTSetReverse(TRUE);
            CTOS_LCDTPrintXY(1, 1, "      Send SMS      ");
            CTOS_LCDTSetReverse(FALSE);
            CTOS_LCDTPrintXY(1, 3, " Dial phone number. ");
            CTOS_LCDTPrintXY(1, 4, " > ");
            CTOS_UIKeypad(4, 4, keyboardLayoutNumber, 40, 80, d_TRUE, \
                          d_FALSE, 0, 0, SMS.baDa, 21);
            SMS.bDaLen = strlen(SMS.baDa);
            SMS.baDa[SMS.bDaLen++] = ';'; SMS.bVp = 12; SMS.bDcs = 0;
            sprintf(SMS.baMessage, "Hello World!!"); SMS.usMessageLen = 13;
            CTOS_LCDTPrintXY(1, 9, "---Report Message---");
            CTOS_LCDTPrintXY(1, 10, " > Sending SMS...");
            rat = CTOS_SMSSend(&SMS, &GSM_Lbuff);
            if (rat != d_OK){
                CTOS_LCDTPrintXY(1, 11, " > Send SMS Fail... ");
                sprintf(GM_buff, " > Error : 0x%04x", rat);
                CTOS_LCDTPrintXY(1, 12, GM_buff);}
            else{
                CTOS_LCDTPrintXY(1, 11, " > Send SMS Success.");
                sprintf(GM_buff, " > SMS Index : %d", GSM_Lbuff);
                CTOS_LCDTPrintXY(1, 12, GM_buff);}
            CTOS_LCDTPrintXY(1, 16, "...Press key to back");
            CTOS_KBDGet(&key); goto GPRS_GSM;}
    }

GPRS_Test:
    CTOS_LCDTClearDisplay();
    CTOS_LCDTSetReverse(TRUE);
    CTOS_LCDTPrintXY(1, 1, "     GPRS Test      ");
    CTOS_LCDTSetReverse(FALSE);
    CTOS_LCDTPrintXY(3, 4, "[1] Start");
    //CTOS_LCDTPrintXY(3, 5, "[2] Set Loop-Time");
    CTOS_LCDTPrintXY(3, 7, "[X] Back");
    //CTOS_LCDTPrintXY(3, 10, "Loop-Time : ");
    
    
    while(1){
    //for(i=0;i<20;i++){
        CTOS_KBDGet(&key); if (key == d_KBD_CANCEL) goto GPRS_A;
        //if (key == d_KBD_2) goto Loop;
        if (key == d_KBD_1){
            CTOS_LCDTPrintXY(1, 4, "                    ");
            CTOS_LCDTPrintXY(1, 5, "                    ");
            CTOS_LCDTPrintXY(1, 7, "                    ");
            CTOS_LCDTPrintXY(1,10, "                    ");
            // GPRS Initial
            CTOS_TCP_GPRSInit(); CTOS_PPP_SetTO(PPP);//>>>>>>disable
            CTOS_LCDTPrintXY(1, 9, " GPRS Initial...    "); CTOS_Delay(500);
            CTOS_LCDTPrintXY(1, 9, " Initial Finish.    "); CTOS_Delay(500);
            CTOS_LCDTPrintXY(1, 9, "                    ");
            //CTOS_LCDTPrintXY(1, 16, " Loop Round :   /   ");
            //if (cc == 0) sprintf(co, "01"); else sprintf(co, "%02d", cc);
            //CTOS_LCDTPrintXY(18, 16, co); sprintf(co, "%02d", lc);
            //CTOS_LCDTPrintXY(15, 16, co);
            // GPRS Open
            CTOS_LCDTPrintXY(1, 3, " > GPRS Opening...  ");
            CTOS_LCDTPrintXY(1, 4, " > Return : ------");
            CTOS_LCDTPrintXY(1, 5, " > Status : ------");
            CTOS_TCP_GPRSOpen(GPRS_LIP, APN, "", ""); Status = 0;  
            
            
            
            while(1){
                rat = CTOS_TCP_GPRSStatus(&Status);
                sprintf(temp, "0x%04x", rat); CTOS_LCDTPrintXY(13, 4, temp);
                sprintf(temp, "0x%04d", Status); CTOS_LCDTPrintXY(13, 5, temp);
                if (rat == 0x2321) {
                    CTOS_Delay(1000);
                    continue;}
                else if (rat != d_OK){
                    CTOS_LCDTPrintXY(1, 3, " > GPRS open fail.  ");
                    CTOS_KBDGet(&key); goto GPRS_Test;}
                else break;}
            
            CTOS_LCDTPrintXY(1, 3, " > Open success.    "); CTOS_Delay(100);
            while(1){
                //sprintf(co, "%02d", lc); CTOS_LCDTPrintXY(15, 16, co);
                //if (lc == 0){CTOS_Delay(100); break;}
                
                // Get GPRS IP
                CTOS_LCDTPrintXY(1, 3, " > Get GPRS IP...   ");
                CTOS_LCDTPrintXY(13, 4, "------");
                CTOS_LCDTPrintXY(13, 5, "------");
                rat = CTOS_TCP_GPRSGetIP(GPRS_LIP);
                if (rat != d_OK){
                    CTOS_LCDTPrintXY(1, 3, " > Get IP fail.     ");
                    CTOS_KBDGet(&key); goto GPRS_Test;}
                CTOS_LCDTPrintXY(1, 6, " > Local IP :       ");
                sprintf(showIP, "    %d.%d.%d.%d         ", GPRS_LIP[0], GPRS_LIP[1], GPRS_LIP[2], GPRS_LIP[3]);
                CTOS_LCDTPrintXY(1, 7, showIP);
                CTOS_LCDTPrintXY(1, 3, " > Get IP success.  "); CTOS_Delay(100);
                // Connect GPRS
                CTOS_TCP_SetConnectTO(CTMS); CTOS_TCP_SetRetryCounter(CONNECT);
                CTOS_LCDTPrintXY(1, 3, " > GPRS Connect...  ");
                CTOS_LCDTPrintXY(13, 4, "------");
                CTOS_LCDTPrintXY(13, 5, "------");
                CTOS_TCP_GPRSConnectEx(&sock, gh, GPRS_Port); Status = 0;
                while(1){
                    rat = CTOS_TCP_GPRSStatus(&Status);
                    sprintf(temp, "0x%04x", rat); CTOS_LCDTPrintXY(13, 4, temp);
                    sprintf(temp, "0x%04d", Status); CTOS_LCDTPrintXY(13, 5, temp);
                    if (rat == 0x2321) {
                        CTOS_Delay(1000);
                        continue;}
                    else if (rat != d_OK){
                        CTOS_LCDTPrintXY(1, 3, " > GPRS unconnected.");
                        CTOS_KBDGet(&key); goto GPRS_Test;}
                    else break;}
                CTOS_LCDTPrintXY(1, 3, " > Connect success. "); CTOS_Delay(100);
                
                
        for(m=0;m<10;m++){
                // GPRS Tx Send Data
                CTOS_LCDTPrintXY(1, 9, "                                 ");
                CTOS_LCDTPrintXY(1,10, "                                 ");
                CTOS_LCDTPrintXY(1,11, "                                 ");
                CTOS_LCDTPrintXY(1,12, "                                 ");
                CTOS_LCDTPrintXY(1,13, "                                 ");
                CTOS_LCDTPrintXY(1,14, "                                 ");
                
                //memset(Data, 0x00,sizeof(Data)); k = 0;
                //for (i = 0; i < 0x08; i++){
                //    for (j = 0; j <= 0xFF; j++){k = j + i*0x100; Data[k]= j;}
                //} 
                SendLen =1024;
                for (k = 0; k < 1024; k++) Data[k] = (rand() % 0xff);
                    Data[0] = 1022 >> 8; Data[1] = 1022 & 0xff;
                
                CTOS_LCDTPrintXY(1, 3, " > GPRS send data...");
                CTOS_LCDTPrintXY(13, 4, "------");
                CTOS_LCDTPrintXY(13, 5, "------");
                CTOS_TCP_GPRSTx(sock, Data, SendLen); Status = 0;
                
                while(1){
                    rat = CTOS_TCP_GPRSStatus(&Status);
                    sprintf(temp, "0x%04x", rat); CTOS_LCDTPrintXY(13, 4, temp);
                    sprintf(temp, "0x%04d", Status); CTOS_LCDTPrintXY(13, 5, temp);
                    if (rat == 0x2321) {
                        CTOS_Delay(50);
                        continue;}
                    //if (rat == 0x238a){
                    //    sprintf(temp, "run:%dtimes",m);
                    //    CTOS_LCDTPrintXY(1, 10, "temp");
                    //    CTOS_LCDTPrintXY(1, 3, " > GPRS Rx Fail.");
                    //    CTOS_Delay(1000);
                    //    goto disconnect;}
                    else if (rat != d_OK){
                        CTOS_LCDTPrintXY(1, 3, " > GPRS Tx fail.    ");
                        CTOS_KBDGet(&key); goto GPRS_Test;}
                    else break;}
                 
                CTOS_LCDTPrintXY(1, 9, " ---- Tx Data ----  ");
                CTOS_LCDTPrintXY(1, 10, " Send > 000102030405060708090A0B0C0D0E0F");
                sprintf(temp, "  Len > %d Bytes  ", SendLen); CTOS_LCDTPrintXY(1,11, temp);
                CTOS_LCDTPrintXY(1, 3, " > GPRS Tx success. "); CTOS_Delay(50);
                // GPRS Rx Receive Data
                CTOS_LCDTPrintXY(1, 3, " > Receive data...  ");
                CTOS_LCDTPrintXY(13, 4, "------");
                CTOS_LCDTPrintXY(13, 5, "------");
                //RecvLen = sizeof(Data);
                RecvLen = 1024;
                //memset(Data, 0x00, RecvLen); 
                CTOS_TCP_GPRSRx(sock, Data, &RecvLen); Status = 0;
                
                while(1){
                    rat = CTOS_TCP_GPRSStatus(&Status);
                    sprintf(temp, "0x%04x", rat); CTOS_LCDTPrintXY(13, 4, temp);
                    sprintf(temp, "0x%04d", Status); CTOS_LCDTPrintXY(13, 5, temp);
                    if (rat == 0x2321) {
                        CTOS_Delay(50);
                        continue;}
                    else if (rat != d_OK){
                        CTOS_LCDTPrintXY(1, 3, " > GPRS Rx fail.    ");
                        CTOS_KBDGet(&key); goto GPRS_Test;}
                    else break;}
                
                if (RecvLen > 1024) RecvLen = 1024;
                if (RecvLen < 1024){RecvLen =1024; CTOS_TCP_GPRSRx(sock, Data, &RecvLen); }
                CTOS_LCDTPrintXY(1, 12, " ---- Rx Data ----  ");
                CTOS_LCDTPrintXY(1, 13, " Recv > FFFEFDFCFBFAF9F8F7F6F5F4F3F2F1F0");
                sprintf(temp, "  Len > %d Bytes  ", RecvLen); CTOS_LCDTPrintXY(1,14, temp);
                CTOS_LCDTPrintXY(1, 3, " > GPRS Rx success. "); CTOS_Delay(50);
                //lc--;
                //CTOS_TCP_GPRSCancelTask((void *)0);
            }
                break;
    }

disconnect:
            // Disconnect GPRS
            CTOS_LCDTPrintXY(1, 3, " > Disconnecting... ");
            CTOS_LCDTPrintXY(13, 4, "------");
            CTOS_LCDTPrintXY(13, 5, "------");
            CTOS_TCP_GPRSDisconnect(sock); Status = 0;
            while(1){
                rat = CTOS_TCP_GPRSStatus(&Status);
                sprintf(temp, "0x%04x", rat); CTOS_LCDTPrintXY(13, 4, temp);
                sprintf(temp, "0x%04d", Status); CTOS_LCDTPrintXY(13, 5, temp);
                if (rat == 0x2321) continue;
                else if (rat != d_OK){
                    CTOS_LCDTPrintXY(1, 3, " > Disconnect fail. ");
                    CTOS_KBDGet(&key); goto GPRS_Test;}
                else break;}
            CTOS_LCDTPrintXY(1, 3, " > Disconnected.    "); CTOS_Delay(100);
            // GPRS Close
            CTOS_LCDTPrintXY(1, 3, " > GPRS Closing...  ");
            CTOS_LCDTPrintXY(13, 4, "------"); CTOS_LCDTPrintXY(13, 5, "------");
            CTOS_TCP_GPRSClose(); Status = 0;
            while(1){
                rat = CTOS_TCP_GPRSStatus(&Status);
                sprintf(temp, "0x%04x", rat); CTOS_LCDTPrintXY(13, 4, temp);
                sprintf(temp, "0x%04d", Status); CTOS_LCDTPrintXY(13, 5, temp);
                if (rat == 0x2321) continue;
                else if (rat != d_OK){
                    CTOS_LCDTPrintXY(1, 3, " > GPRS close fail. ");
                    CTOS_KBDGet(&key); goto GPRS_Test;}
                else break;}
            CTOS_LCDTPrintXY(1, 3, " > GPRS close.      "); CTOS_Delay(100);
            CTOS_LCDTPrintXY(1, 3, " [GPRS Test Finish] "); CTOS_KBDGet(&key);
            goto GPRS_Test;}
    }

    
    
    
    
Loop:
    CTOS_LCDTClearDisplay();
    CTOS_LCDTSetReverse(TRUE);
    CTOS_LCDTPrintXY(1,1,"Set Loop-Round         ");
    CTOS_LCDTSetReverse(FALSE);
    CTOS_LCDTPrintXY(1,4,"Input Round:");
    CTOS_LCDTPrintXY(1,5," > ");
            
    while(1){
        CTOS_LCDTPrintXY(1,8,"                    ");
        //CTOS_KBDGet(&key);
        //if(key ==d_KBD_CANCEL){
        //    if(comm ==1 || comm ==2 || comm==3) goto COM;
        //    if(comm ==4) goto Exit; if(comm ==5) goto Exit;
       //     if(comm ==6) goto Exit; if(comm ==7) goto Exit;
       // }
        CTOS_UIKeypad(3,5, keyboardLayoutNumber, 40, 80, d_TRUE, d_FALSE, 0, 0,round, 5);
        
        totalloop=atoi(round);
        if(totalloop>1000){
            CTOS_LCDTPrintXY(1,6,"Overflow,Plz reset");
            totalloop =1;
            goto Loop;
        }
        else{
            CTOS_LCDTPrintXY(1,6,"Loop Set Finish!  ");
            memset(str,0x00,sizeof(str));
            sprintf(str,"Rounds:%d",totalloop);
            CTOS_LCDTPrintXY(1,7,str);
            CTOS_Delay(1000);
            if(comm ==1 || comm ==2 || comm==3) goto COM;
            if(comm ==4) goto Ethernet_test; if(comm ==5) goto USB;
            if(comm ==6) goto Modem; if(comm ==7) goto Main;
        }
    }


/////17/////////     
BT_test:
        i=0;k=0;
        CTOS_LCDTClearDisplay();
        CTOS_LCDTPrintXY(1,1,"  [1]BT Setting");
        CTOS_LCDTSetReverse(TRUE);
        CTOS_LCDTPrintXY(1,2,"  BT OpenMode Test  ");
        CTOS_LCDTSetReverse(FALSE);
        CTOS_LCDTPrintXY(1,3,"  [2]Start Test");
        
        CTOS_LCDTSetReverse(TRUE);
        CTOS_LCDTPrintXY(1,5,"  BT ServerMode Test");
        CTOS_LCDTSetReverse(FALSE);
        CTOS_LCDTPrintXY(1,6,"  [3]Start Test");
        CTOS_LCDTPrintXY(1,9,"  [X]Exit");
       /*
        CTOS_LCDTPrintXY(1,6,"    BT ServerMode Test");
        CTOS_LCDTPrintXY(1,7,"  [3] Start Test");
        CTOS_LCDTPrintXY(1,8,"  [4] BT Setting");*/
        while(1){
            CTOS_KBDGet(&key);
            if(key ==d_KBD_1) goto BT_Set;
            if(key ==d_KBD_2) goto BT_Scan;
            if(key ==d_KBD_3) goto BT_ServerMode_Test;
            if(key ==d_KBD_CANCEL) goto Main;
        }
        
BT_Scan:
	CTOS_LCDTClearDisplay();
        CTOS_LCDTPrintXY(1,1,"");
        CTOS_LCDTPrintXY(1,2,"Bluetooth Open.....");
        CTOS_Delay(500);
        ret =CTOS_BluetoothOpen();
        if(ret !=d_OK){
            CTOS_LCDTPrintXY(1,2,"BT Open Fail");
            sprintf(BT_Device," Return:0x%04x",ret);
            CTOS_LCDTPrintXY(1,2,BT_Device);
            CTOS_KBDGet(&key);
            goto BT_test;
        }
        CTOS_LCDTPrintXY(1,2,"                    ");
        CTOS_LCDTPrintXY(1,3,"Scanning......");
        CTOS_BluetoothDeviceScan();
        /* ret =CTOS_BluetoothOpen();
        if(ret !=d_OK){
            CTOS_LCDTPrintXY(1,2,"BT Open Fail");
            sprintf(BT_Device," Return:0x%04x",ret);
            CTOS_LCDTPrintXY(1,2,BT_Device);
            CTOS_KBDGet(&key);
            goto BT_test;
        }
        
        CTOS_LCDTPrintXY(1,
        if(ret !=d_OK){
            CTOS_LCDTPrintXY(1,2,"BT Scan Fail");
            goto BT_test;
        } 
        else{*/
            /*k =rand() % 32;
            g =rand() % 32;*/
            while(1){
                CTOS_KBDHit(&key);
                if(key==d_KBD_CANCEL) goto BT_test;
                ret =CTOS_BluetoothStatus(&Status);
                if(ret ==d_OK) break;
                else if(ret & d_BLUETOOTH_IO_PROCESSING) continue;  
                //  d_BLUETOOTH_IO_PROCESSING not found in book4.
                else{
                    CTOS_LCDTClearDisplay();
                    CTOS_LCDTPrintXY(1,9," Device Search Fail.");
                    sprintf(breb,"Return: 0x%04x",ret);
                    CTOS_LCDTPrintXY(1,11,breb);
                    sprintf(breb,"Status: 0x%04x",Status);
                    CTOS_LCDTPrintXY(1,12,breb);
                    CTOS_KBDGet(&key);
                    CTOS_LCDTPrintXY(1,9,"    BT closing...");
                    CTOS_Delay(1000);
                    CTOS_BluetoothClose();
                    goto BT_test;
                }
            }
        
        CTOS_LCDTClearDisplay();
        CTOS_LCDTPrintXY(1,1,"    Search Device    ");
        CTOS_LCDTPrintXY(1,9,"    Search finish    ");
        CTOS_Delay(500);
        CTOS_LCDTPrintXY(1,9,"                     ");
        CTOS_BluetoothDeviceInfoGet(&BT_Num, &Device);
        Page =(BT_Num/7) +(BT_Num %7 ?1 :0);
        
        if(Page >0)  current_page=1;
        else current_page=0;
        
        CTOS_LCDTClearDisplay();
        if(Page ==0){ 
            CTOS_LCDTPrintXY(1,1,"   Device not found");
            CTOS_LCDTPrintXY(1, 13, "[0] Rescan");
            CTOS_LCDTPrintXY(1, 14, "any key to exit");
            CTOS_KBDGet(&key);
            if(key== d_KBD_0) goto BT_Scan;
            else goto EXIT;
        }
        else goto BT_List;

BT_List:           
            CTOS_LCDTClearDisplay();
            CTOS_LCDTPrintXY(1,13,"[0] Rescan");
            CTOS_LCDTPrintXY(1,14,"[X] Back");
            CTOS_LCDTSetReverse(TRUE);
            CTOS_LCDTPrintXY(1,1,"  Found Device      ");
            CTOS_LCDTSetReverse(FALSE);
            /*
            while(1){
            CTOS_KBDGet(&key);
            i++;
            if(i >BT_Num) break;
            else{
                    sprintf(BT_Device,"                   ");//erease temp 20
                    CTOS_LCDTPrintXY(1,(i+3),BT_Device);         //erease
                    //sprintf(BT_Device,"[%d]%s",i,"   ");
                   
                    snprintf(BT_Device,sizeof(BT_Device),"[%d]%s",i,(Device + ((current_page-1)*7) +i)->pbDeviceName);  //has error
                    CTOS_LCDTPrintXY(1,(i+3),BT_Device);         //print start on screen x=1 y=4
            }
            }
            CTOS_LCDTPrintXY(1,1,"   Device not found");
            CTOS_Delay(3000);
            exit(0);
         */   
        while(1){
                sprintf(breb,"Device List %02d/%02d",current_page,Page);
                CTOS_LCDTPrintXY(1,16,breb);
                if(Page ==1)  CTOS_LCDTPrintXY(1, 12, "                    ");  //if only one page,don't screen
                else if(current_page <Page) CTOS_LCDTPrintXY(1, 12, "[^]Pre       [.]Next");
                else if(current_page ==1) CTOS_LCDTPrintXY(1, 12, "             [.]Next");  //the first page
                else if(current_page ==Page) CTOS_LCDTPrintXY(1, 12, "[^]Pre              "); // the last page
                
                for(i=1;i<8;i++){
                    if(i>=BT_Num) break;
                    else{
                    sprintf(BT_Device,"                   ");//erease temp 20
                    CTOS_LCDTPrintXY(1,(i+3),BT_Device);         //erease
                    snprintf(BT_Device,sizeof(BT_Device),"[%d]%10s",i,(Device + ((current_page-1)*7) +i)->pbDeviceName);  //has error
                    CTOS_LCDTPrintXY(1,(i+3),BT_Device);         //print start on screen x=1 y=4
                    }
                } 
                CTOS_Delay(3000);
                CTOS_KBDGet(&key);
                if(key == d_KBD_DOT) { 
                    current_page++;
                    if(current_page >Page) current_page=Page;
                }
                else if(key == d_KBD_00) {
                    current_page--;
                    if(current_page <1) current_page=1;
                }
                else if(key == d_KBD_0) goto BT_Scan;
                else if(key == d_KBD_CANCEL) goto BT_test;
                //  has error
                else{
                    if(key ==d_KBD_1) Device=(Device +1 + (current_page-1)*7);
                    if(key ==d_KBD_2) Device=(Device +2 + (current_page-1)*7);
                    if(key ==d_KBD_3) Device=(Device +3 + (current_page-1)*7);
                    if(key ==d_KBD_4) Device=(Device +4 + (current_page-1)*7);
                    if(key ==d_KBD_5) Device=(Device +5 + (current_page-1)*7);
                    if(key ==d_KBD_6) Device=(Device +6 + (current_page-1)*7);
                    if(key ==d_KBD_7) Device=(Device +7 + (current_page-1)*7);
                    goto BT_connect;
                }
            }

 BT_connect:
            CTOS_LCDTClearDisplay();
            CTOS_LCDTPrintXY(1, 1, " Scan Device Server ");
            CTOS_BluetoothServiceScan(Device);
            while(1){
                CTOS_KBDHit(&key);
                if(key ==d_KBD_CANCEL) goto BT_test;
                ret =CTOS_BluetoothStatus(&Status);
                CTOS_LCDTPrintXY(1,9,"    Scanning...");
                CTOS_Delay(500);
                CTOS_LCDTPrintXY(1,9,"               ");
                CTOS_Delay(500);
                if(ret ==d_OK) break;
                else if(ret & d_BLUETOOTH_IO_PROCESSING) continue;
                else{
                    CTOS_LCDTPrintXY(1,9,"Service scan Fail...");
                    sprintf(breb,"Return:0x%04x",ret);
                    CTOS_LCDTPrintXY(1,11,breb);
                    sprintf(breb,"Status:0x%04x",Status);
                    CTOS_LCDTPrintXY(1,12,breb);
                    CTOS_KBDHit(&key);
                    CTOS_LCDTPrintXY(1,12,"BT Closing...    ");
                    CTOS_BluetoothClose();
                    goto BT_test;
                }
            }
            CTOS_LCDTPrintXY(1,9,"BT Scan Finish...      ");
            CTOS_Delay(500);
            ret =CTOS_BluetoothServiceInfoGet(&BT_Num,&Service);
            if(ret !=d_OK) CTOS_LCDTPrintXY(1,9,"Get Info Failed...      ");
            if(BT_Num ==0) CTOS_LCDTPrintXY(1,9,"No Test Device...      ");
            if(ret !=d_OK || BT_Num ==0){
                goto BT_test;
            }
            
            CTOS_LCDTPrintXY(1,9,"Get Info Success.      ");
            CTOS_Delay(500);
            
            for (i = 0; i < BT_Num; i++){
                k = strcmp(SName, (Service + i)->pbSeviceName);
                if (k == 0){Service += i; break;}
            }

            if(k !=0){
                ret =CTOS_BluetoothStatus(&Status);
                CTOS_LCDTPrintXY(1,9,"Match Name Failed...  ");
                CTOS_KBDGet(&key);
                CTOS_BluetoothClose();
                goto BT_test;}
            
            CTOS_LCDTPrintXY(1,9,"BT Match Success.       ");
            CTOS_Delay(500);
            memset(Data,0x00,sizeof(Data)); //reset the data
            for(i =0;i <0x08;i++){          //Write in the data
                for(j=0;j<=0xff;j++){
                    k =j+i*0x100;
                    Data[k] =j;}
                }
            SendLen =2048; RecvLen=2048;
            
            CTOS_BluetoothConnect(Device,Service);
            while(1){
                ret =CTOS_BluetoothStatus(&Status);
                CTOS_LCDTPrintXY(1,9,"Connecting....      ");
                if(ret ==d_OK) break;
                else if(ret &d_BLUETOOTH_IO_PROCESSING) continue;
                else{
                    CTOS_LCDTPrintXY(1,9,"BT Prepare Failed....  ");
                    CTOS_KBDGet(&key);
                    CTOS_BluetoothDisconnect();
                    CTOS_BluetoothClose();
                    goto BT_test;}
            }
            
            CTOS_LCDTPrintXY(1,9,"BT Send Prepare....  ");
            CTOS_Delay(500);
            CTOS_BluetoothTxReady();
            
            CTOS_LCDTPrintXY(1,9,"BT Send Ready....    ");
            CTOS_Delay(500);
            CTOS_LCDTPrintXY(1,9,"Sending Data...      ");
            ret =CTOS_BluetoothTxData(Data,SendLen);
            if(ret !=d_OK){
                CTOS_LCDTPrintXY(1,9,"Send Failed....      ");
                CTOS_BluetoothDisconnect();
                CTOS_BluetoothClose();
                goto BT_test;}
            
            CTOS_LCDTPrintXY(1, 2, " ---- Tx Data ----  ");
            CTOS_LCDTPrintXY(1, 3, " Send > 000102030405060708090A0B0C0D0E0F");
            sprintf(str, "  Len > %d Bytes  ", SendLen); CTOS_LCDTPrintXY(1,4, str);
            CTOS_LCDTPrintXY(1, 9, "   BT Tx finish.     "); CTOS_Delay(500);
            memset(Data, 0x00, sizeof(Data)); Len = sizeof(Data);
            CTOS_LCDTPrintXY(1, 9, "   Rx prepare...    "); CTOS_Delay(500);
            CTOS_BluetoothRxReady(&RecvLen); //ret = CTOS_BluetoothStatus(&Status);
            ret =CTOS_BluetoothRxData(Data,&RecvLen);
            if(ret !=d_OK){
                CTOS_LCDTPrintXY(1, 2, " Receive Data Failed...  ");
                CTOS_BluetoothDisconnect();
                CTOS_BluetoothClose();
                goto BT_test;} 
           
            if(RecvLen >0){
                memset(str,0x00,sizeof(str));
                for(k =0;k<=SendLen;k+=16){
                    CTOS_LCDTPrintXY(1,10," Recv > FFFEFDFCFBFAF9F8F7F6F5F4F3F2F1F0");
                    sprintf(str,"  RECV > %dBytes",k);
                    CTOS_LCDTPrintXY(1,11,str);}
                    CTOS_KBDHit(&key);
                    //if(key ==d_KBD_CANCEL) break;
                    //CTOS_Delay(500);
            }
            CTOS_LCDTPrintXY(1, 15, "BT Test Finish");
            CTOS_LCDTPrintXY(1, 16, "Press any key...");
            CTOS_KBDGet(&key);
            goto BT_test;
            
            
            
 BT_Set:    
        CTOS_LCDTClearDisplay();
        CTOS_LCDTSetReverse(TRUE);
        CTOS_LCDTPrintXY(1,1," Set Bluetooth Mode ");
        CTOS_LCDTSetReverse(FALSE);
        CTOS_LCDTPrintXY(1,3,"[1] Set Handset");
        CTOS_LCDTPrintXY(1,4,"[2] Set UART");
        CTOS_LCDTPrintXY(1,5,"[X] Back");
        while(1){
            CTOS_KBDGet(&key);
            if(key ==d_KBD_CANCEL) goto BT_test;
            if(key ==d_KBD_1){
                CTOS_BluetoothConfigSet(d_BLUETOOTH_CONFIG_MODE, "0",sizeof("0"));
                CTOS_BluetoothConfigSet(d_BLUETOOTH_CONFIG_AUTOCON, "0",sizeof("0"));
                CTOS_LCDTPrintXY(1,6,"Set BT Mode : O ");
                CTOS_LCDTPrintXY(1,7,"BT Auto-con : N ");
                CTOS_LCDTPrintXY(1,8,"BT setting finish");
                CTOS_KBDGet(&key);
                goto BT_test;                
            }
            if(key ==d_KBD_2){
                CTOS_BluetoothConfigSet(d_BLUETOOTH_CONFIG_MODE, "C",sizeof("C"));   //set bluetooth cradle mode
                CTOS_LCDTPrintXY(1, 6,"Please wait...");
                CTOS_Delay(500);
                ret=CTOS_CradleAttached();
                if(ret ==d_YES){
                    CTOS_LCDTPrintXY(1, 6,"Set BT Mode : C ");
                    CTOS_LCDTPrintXY(1, 7,"BT Auto-con : N ");
                    CTOS_LCDTPrintXY(1, 8,"BT setting finish");
                    CTOS_KBDGet(&key);
                    goto BT_Set;}
                
                if(ret !=d_YES){
                    CTOS_LCDTPrintXY(1, 6,"Please attach Cradle");
                    CTOS_KBDGet(&key);
                    goto BT_Set;}
            }
        }   
        
BT_ServerMode_Test:
            CTOS_LCDTClearDisplay();
            CTOS_LCDTPrintXY(1, 2,"  1.Secure Mode");
            CTOS_LCDTPrintXY(1, 3,"  2.Insecure Mode ");
            CTOS_KBDGet(&key);
            //CTOS_BluetoothOpen();
            while(1){
            //CTOS_BluetoothConfigSet(d_BLUETOOTH_CONFIG_SECURE,"0",sizeof("0"));
            if(key ==d_KBD_1){
                CTOS_BluetoothConfigSet(d_BLUETOOTH_CONFIG_DEVICE_NAME, "castlesjjbt",sizeof("castlesjjbt"));
                CTOS_LCDTClearDisplay();
                //CTOS_BluetoothConfigSet(d_BLUETOOTH_CONFIG_SECURE,"1",sizeof("1"));  //open ssp
                rat =CTOS_BluetoothConfigSet (d_BLUETOOTH_CONFIG_SECURE, (unsigned char *) "1", 1);
                if(rat !=d_OK){
                    CTOS_LCDTPrintXY(1, 4,"Set Failed...");
                    sprintf(str,"Err, Rtn %4x   ",rat);
                    CTOS_LCDTPrintXY(1, 5,str);
                    CTOS_KBDGet(&key);
                    goto BT_test;}
                CTOS_LCDTPrintXY(1, 4,"  Set Success!!   ");
                CTOS_Delay(500);
                memset(PASSKEY,0x00,sizeof(PASSKEY));
                CTOS_BluetoothConfigSet(d_BLUETOOTH_CONFIG_AUTOCON,"0",sizeof("0"));  //close auto connect
                //TOS_BluetoothConfigSet(d_BLUETOOTH_CONFIG_PASSKEY,"1",sizeof("0"));     
                        
                CTOS_LCDTPrintXY(1, 4,"  Set Passkey   "); 
                CTOS_LCDTPrintXY(1, 5,"Pin:"); 
                CTOS_UIKeypad(4, 8, keyboardLayoutNumber, 40, 80, d_TRUE, d_FALSE, 0, 0, PASSKEY, 9); 
                CTOS_BluetoothConfigSet(d_BLUETOOTH_CONFIG_PASSKEY,PASSKEY,strlen(PASSKEY));
                   
                CTOS_BluetoothOpen();
                CTOS_LCDTClearDisplay();
                CTOS_LCDTPrintXY(1, 4,"Set Success!!");
                CTOS_Delay(500);
                
                memset(info,0,sizeof(info));
                btinfolen =sizeof(info);
                CTOS_BluetoothConfigGet(d_BLUETOOTH_CONFIG_DEVICE_NAME,info,&btinfolen);
                sprintf(btinfo,"Name:%s",info);
                CTOS_LCDTPrintXY(1, 4,"Wait connection");
                CTOS_LCDTPrintXY(1, 5,"                  ");
                CTOS_LCDTPrintXY(1, 5,btinfo);
                
                memset(info,0,sizeof(info));
                btinfolen =sizeof(info);
                CTOS_BluetoothConfigGet(d_BLUETOOTH_CONFIG_MAC,info,&btinfolen);
                CTOS_LCDTPrintXY(1, 6,"             ");
                CTOS_LCDTPrintXY(1, 6,"MAC:");
                sprintf(btinfo,"%s",info);
                CTOS_LCDTPrintXY(1,7,btinfo);
                
                
                /*
                while(1){
                CTOS_BluetoothStatus(&Status);
                if(((Status & d_BLUETOOTH_STATE_CONNECTED) != d_BLUETOOTH_STATE_CONNECTED)&& ((Status & d_BLUETOOTH_STATE_SENDING) != d_BLUETOOTH_STATE_SENDING)) {
                    CTOS_LCDTPrintXY(1, 3, "connectting...");
                    continue;
		}
                else {
                    CTOS_LCDTPrintXY(1, 3, "connect");
                    break;}
                }

                */
                
                while(1){
                    CTOS_KBDHit(&key);
                    if(key == d_KBD_CANCEL) goto BT_disconnect;
                    ret =CTOS_BluetoothListen(ServiceName2,my_xuuid);
                    if(ret !=d_OK) {
                        CTOS_LCDTPrintXY(1,3,"Listen Failed...");
                        sprintf(str,"return:%04x",ret);
                        CTOS_LCDTPrintXY(1,16,str);
                        CTOS_KBDGet(&key);
                        goto BT_disconnect;
                    }
                    else{
                        while(1){
                            CTOS_BluetoothStatus(&Status);
                            if(Status & 0x00000002){                                       //d_BLUETOOTH_STATE_CONNECTED = 0x00000002
                                CTOS_LCDTPrintXY(1,5, "Connect Success");
                                CTOS_Delay(1500);
                                break;
                            }	
                            CTOS_KBDHit(&key);
                        if(key == d_KBD_CANCEL) goto BT_disconnect;
                            }
                        //et =CTOS_BluetoothStatus(&Status);
                        //if(Status & 0x00000002){
                            CTOS_LCDTPrintXY(1, 4,"BT connected...       ");
                            //CTOS_KBDGet(&key);
                            break;}    
                }
                
                CTOS_Delay(1000);
                CTOS_LCDTClearDisplay();
                CTOS_LCDTPrintXY(1,1,"BT ServerMode Test");
                CTOS_LCDTPrintXY(1,2,"Wait Rx Data...");
                ret=CTOS_BluetoothRxReady(&RecvLen);
                if(ret !=d_OK){
                    CTOS_LCDTPrintXY(1,3,"BT Rx Not Ready...");
                    CTOS_KBDGet(&key);
                    goto BT_disconnect;}
                if(RecvLen >0){
                        sprintf(str,"Receive:%d",(int*)RecvLen);
                        CTOS_LCDTPrintXY(1,3,str);
                        ret=CTOS_BluetoothTxReady();
                        if(ret !=d_OK){
                            CTOS_LCDTPrintXY(1,3,"BT Tx Not Ready...");
                            CTOS_KBDGet(&key);
                            goto BT_disconnect;}
                        else{
                            memset(Data,0x00,sizeof(Data)); //reset the data
                            for(i =0;i <0x08;i++){          //Write in the data
                                for(j=0;j<=0xff;j++){
                                k =j+i*0x100;
                                Data[k] =j;}
                            }
                            SendLen =2048; RecvLen=2048;
                            
                            ret =CTOS_BluetoothTxData(Data,SendLen);
                            
                            sprintf(str,"Send   :%d",(int*)SendLen);
                            CTOS_LCDTPrintXY(1,5,str);
                            CTOS_LCDTPrintXY(1,2,"Test  Success!!");
                            CTOS_Delay(500);
                            CTOS_LCDTPrintXY(1,6,"Press Any KEY...");
                            CTOS_KBDGet(&key);
                            goto BT_disconnect;
                        }
                }
                else{
                    CTOS_LCDTPrintXY(1,2,"TEST FAILED...");
                    CTOS_KBDGet(&key);
                    goto BT_disconnect;}
                
            }
            
            
            if(key ==d_KBD_2){
                CTOS_LCDTClearDisplay();
                
                CTOS_BluetoothOpen();
                CTOS_BluetoothConfigSet(d_BLUETOOTH_CONFIG_DEVICE_NAME, "castlesjjbt",sizeof("castlesjjbt"));
                //ret=CTOS_BluetoothConfigSet(d_BLUETOOTH_CONFIG_SECURE,"0",sizeof("0"));
                rat =CTOS_BluetoothConfigSet (d_BLUETOOTH_CONFIG_SECURE, (unsigned char *) "0", 1);
                if(rat !=d_OK){
                    CTOS_LCDTPrintXY(1, 4,"Set Failed...");
                    sprintf(str,"Err, Rtn %4x   ",rat);
                    CTOS_LCDTPrintXY(1, 5,str);
                    CTOS_KBDGet(&key);
                    goto BT_test;}
                
                
                CTOS_BluetoothConfigSet(d_BLUETOOTH_CONFIG_AUTOCON,"0",sizeof("0"));  // auto connect
                
                CTOS_BluetoothConfigSet(d_BLUETOOTH_INSECURE_MODE,"0",sizeof("0")); 
                //CTOS_BluetoothOpen();
                CTOS_LCDTPrintXY(1, 4,"Set Success!!");
                CTOS_Delay(500);
                memset(btinfo,0x00,sizeof(info));
                btinfolen=sizeof(info);
                CTOS_BluetoothConfigGet(d_BLUETOOTH_CONFIG_DEVICE_NAME,info,&btinfolen);
                sprintf(btinfo,"Name:%s",info);
                CTOS_LCDTPrintXY(1, 4,"Wait connection");
                CTOS_LCDTPrintXY(1, 5,"             ");
                CTOS_LCDTPrintXY(1, 5,btinfo);
                
                memset(info,0,sizeof(info));
                btinfolen =sizeof(info);
                CTOS_BluetoothConfigGet(d_BLUETOOTH_CONFIG_MAC,info,&btinfolen);
                CTOS_LCDTPrintXY(1, 6,"             ");
                CTOS_LCDTPrintXY(1, 6,"MAC:");
                sprintf(btinfo,"%s",info);
                CTOS_LCDTPrintXY(1,7,btinfo);
                
                
                while(1){
                    CTOS_KBDHit(&key);
                    if(key == d_KBD_CANCEL) goto BT_disconnect;
                    ret =CTOS_BluetoothListen(ServiceName2,my_xuuid);
                    if(ret !=d_OK) {
                        CTOS_LCDTPrintXY(1,3,"Listen Failed...");
                        sprintf(str,"return:%04x",ret);
                        CTOS_LCDTPrintXY(1,16,str);
                        CTOS_KBDGet(&key);
                        goto BT_disconnect;
                    }
                    else{
                        while(1){
                            CTOS_BluetoothStatus(&Status);
                            if(Status & 0x00000002){                                       //d_BLUETOOTH_STATE_CONNECTED = 0x00000002
                                CTOS_LCDTPrintXY(1,5, "Connect Success");
                                CTOS_Delay(1500);
                                break;
                            }	
                            CTOS_KBDHit(&key);
                        if(key == d_KBD_CANCEL) goto BT_disconnect;
                            }
                            CTOS_LCDTPrintXY(1, 4,"BT connected...       ");
                            //CTOS_KBDGet(&key);
                            break;}    
                }
                
                CTOS_Delay(1000);
                CTOS_LCDTClearDisplay();
                CTOS_LCDTPrintXY(1,1,"BT ServerMode Test");
                CTOS_LCDTPrintXY(1,2,"Wait Rx Data...");
                ret=CTOS_BluetoothRxReady(&RecvLen);
                if(ret !=d_OK){
                    CTOS_LCDTPrintXY(1,3,"BT Rx Not Ready...");
                    CTOS_KBDGet(&key);
                    goto BT_disconnect;}
                if(RecvLen >0){
                        sprintf(str,"Receive:%d",(int*)RecvLen);
                        CTOS_LCDTPrintXY(1,3,str);
                        ret=CTOS_BluetoothTxReady();
                        if(ret !=d_OK){
                            CTOS_LCDTPrintXY(1,3,"BT Tx Not Ready...");
                            CTOS_KBDGet(&key);
                            goto BT_disconnect;}
                        else{
                            memset(Data,0x00,sizeof(Data)); //reset the data
                            for(i =0;i <0x08;i++){          //Write in the data
                                for(j=0;j<=0xff;j++){
                                k =j+i*0x100;
                                Data[k] =j;}
                            }
                            SendLen =2048; RecvLen=2048;
                            
                            ret =CTOS_BluetoothTxData(Data,SendLen);
                            
                            sprintf(str,"Send   :%d",(int*)SendLen);
                            CTOS_LCDTPrintXY(1,5,str);
                            CTOS_LCDTPrintXY(1,2,"Test  Success!!");
                            CTOS_Delay(500);
                            CTOS_LCDTPrintXY(1,6,"Press Any KEY...");
                            CTOS_KBDGet(&key);
                            goto BT_disconnect;
                        }
                }
                else{
                    CTOS_LCDTPrintXY(1,2,"TEST FAILED...");
                    CTOS_KBDGet(&key);
                    goto BT_disconnect;}
                
            }

            if(key ==d_KBD_CANCEL){
                goto BT_test;
                }
            }
            
                
BT_disconnect:
                CTOS_KBDGet(&key);
                CTOS_LCDTPrintXY(1, 7,"  Disconnecting...");
                CTOS_Delay(200);
                CTOS_BluetoothDisconnect();
                CTOS_LCDTPrintXY(1, 8,"  Closing...");
                CTOS_Delay(200);
                CTOS_BluetoothDisconnect();
                CTOS_BluetoothClose();
                goto BT_test;
              
            

        CTOS_KBDGet(&key);
        
EXIT:           
  exit(0);
}




