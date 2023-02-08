#include "opentx.h"
bool HomeSet= false;//HOME: SER
int sigX=1;//plot direction
int sigY=1;//plot direction
struct {int X1, X2, Y1, Y2;} H_XY;//C_XY//int16_t only: cant fit 32
 
void hello_draw() {      
  //int i=19-1;//GPS                
  TelemetryItem & telemetryItem = telemetryItems[19-1];

  div_t gps_LAT  =  div((int)telemetryItem.gps.latitude,  1000000);
  div_t gps_LONG =  div((int)telemetryItem.gps.longitude, 1000000);

  div_t gps_LAT2 =  div((int)gps_LAT.rem, 100);
  div_t gps_LONG2 =  div((int)gps_LONG.rem, 100);

  lcdDrawText(3, 15, "RSSI", SMLSIZE);
  lcdDrawNumber(3+24, 15, telemetryItems[1-1].value, SMLSIZE); 
  
  lcdDrawText(3, 25, "SNR", SMLSIZE);
  lcdDrawNumber(3+24, 25, telemetryItems[4-1].value, SMLSIZE); 

  lcdDrawText(3, 35, "Sats", SMLSIZE);
  lcdDrawNumber(3+24, 35, telemetryItems[23-1].value, SMLSIZE); 

  lcdDrawText(3, 45, "Alt.", SMLSIZE); 
  lcdDrawNumber(3+24, 45, telemetryItems[22-1].value, SMLSIZE);

  lcdDrawText(3, 55, "GSpd", SMLSIZE); 
  lcdDrawNumber(3+24, 55, telemetryItems[20-1].value, SMLSIZE);  

  if(!HomeSet){   
    H_XY.X1= gps_LAT.quot;
    H_XY.Y1= gps_LONG.quot;
    H_XY.X2= gps_LAT2.quot;
    H_XY.Y2= gps_LONG2.quot;

    HomeSet=true; 
    } 
    else{

	    lcdDrawText(3, 5, "Stat->ON", SMLSIZE|BLINK);   
	    //Dist
	    int XP, YP;
	    gps_LAT.quot = abs(H_XY.X1 - gps_LAT.quot);
	    if(gps_LAT.quot>0){if(sigX==1){XP=122;}else{XP=45;}}//min//MAx}//xMax}
	    else{
		//Min=45;Max=125;                   
		gps_LAT2.quot = abs(H_XY.X2 - gps_LAT2.quot)/500;
		if (gps_LAT2.quot>40){
		  if(sigX==1){XP=122;}else{XP=45;}}//min//MAx
		else{XP=85+sigX*gps_LAT2.quot;}//between
	    }

	    gps_LONG.quot = abs(H_XY.Y1 - gps_LONG.quot)/500;
	    if(gps_LONG.quot>0){if(sigY==1){YP=5;}else{YP=58;}}//yMax
	    else{
		//Min=58;Max=5;
		gps_LONG2.quot = abs(H_XY.Y2 - gps_LONG2.quot)/500;
		if (gps_LONG2.quot>30){
		  if(sigY==1){YP=5;}else{YP=58;}}//max/MAX
		else{YP=32+sigY*gps_LONG2.quot;}//between
	    }

	    lcdDrawText(XP-2, YP -3,"X",BOLD|BLINK);//UAV: if not in calibration
	  }		   
}

void hello_stop(){
	//if (globalData.cToolRunning) {globalData.cToolRunning = 0;popMenu();}
	popMenu();
	}

void hello_run(event_t event) {
// run every frame
	    //if (globalData.cToolRunning == 0) {globalData.cToolRunning = 1; }
	    // exit on LONG press CANCEL
	    if(event == EVT_KEY_LONG(KEY_EXIT)) {hello_stop();}
	    if(event == EVT_KEY_FIRST(KEY_UP)){sigY=-sigY;}  
	    if(event == EVT_KEY_FIRST(KEY_DOWN)){sigX=-sigX;}
	    if(event == EVT_KEY_LONG(KEY_MENU)){HomeSet=!HomeSet;AUDIO_TRIM_MIDDLE();}
	    if(event == EVT_KEY_LONG(KEY_RIGHT)){lcdClear();AUDIO_TRIM_MIDDLE();}
	    	     
	    hello_draw();           
		lcdDrawRect(0, 0, 128, 64, SOLID);
	    lcdDrawFilledRect(85 -4, 32 -4, 9, 9, SOLID, 0);//HOME always fixed
	    lcdDrawText(83+sigX*38, 32, "+", SMLSIZE); //x arrow
	    lcdDrawText(83-3, 31-sigY*27-3, "+", SMLSIZE); //y arrow
	    lcdDrawSolidHorizontalLine(45, 32   , 80);// ----
	    lcdDrawSolidVerticalLine(85   , 2, 64 -4 );// |
}
