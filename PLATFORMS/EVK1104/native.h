#ifdef WITHMON
extern char ConStat();
extern char Ser1Out();
extern char Ser1In();
extern char Ser1RTS();
extern char Ser1DTR();
extern char GetButtons();
extern char SetOnBoardLEDs();
extern char Dip204_write_data();
extern char ControlLCD();
extern char PwmStart();
extern char PwmStop();
#endif
char nativeCharIn();
char nativeCharOut();
char conStat();
char getButtons();
char setOnBoardLEDs();
char charLCDOut();
char controlLCD();
char nativeExit();
char currentTimeMillis();
char ser1In();
char ser1Out();
char ser1Stat();
char ser1RTS();
char ser1DTR();
char pwmStart();
char pwmStop();
// Grafik-Methoden
char initDisplay();
char clearDisplay();
char drawPixel();
char drawHorizLine();
char drawVertLine();
char drawLine();
char drawFillRect();
char drawFilledCircle();
char drawChar();
// Touch-Wheel
char getButtons();
char isButtonPressed();
// LEDs
char setLEDon();
char setLEDoff();
char adcInit();
char adcEnable();
char adcDisable();
char adcGetValue();
#ifdef WITHMON
void Monitor(int);
int GetMilliSec();
#endif
