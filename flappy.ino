#include <SD.h>//https://www.arduino.cc/en/reference/SD
#include <Adafruit_GFX.h>//https://github.com/adafruit/Adafruit-GFX-Library
#include <MCUFRIEND_kbv.h>//https://github.com/prenticedavid/MCUFRIEND_kbv
#include <TouchScreen.h> //https://github.com/adafruit/Adafruit_TouchScreen

MCUFRIEND_kbv tft;
#define LOWFLASH (defined(__AVR_ATmega328P__) && defined(MCUFRIEND_KBV_H_))

#include "bird02.h"
#include "bitmap_RGB.h"


#define SD_CS 10
#define BLACK 0
#define GREY 21845
#define BLUE 31
#define RED 63488
#define GREEN 2016
#define DARKGREEN 1472

#define CYAN 2047
#define MAGENTA 63519
#define YELLOW 65504
#define GOLD 56768
#define WHITE 65535
#define MINPRESSURE 200
#define MAXPRESSURE 1000


const int TS_LEFT = 531, TS_RT = 527, TS_TOP = 485, TS_BOT = 489;

TouchScreen ts = TouchScreen(6, A1, A2, 7, 300);
TSPoint p;
bool up;
int pixel_x, pixel_y;
int currentPage = 0, oldPage = -1;

int score = 0;
int xP = 410;
int yP = 100;
int yB = 50;
int movingRate = 3;
float fallRate = 0;
int fallRateInt = 1;
int lastSpeedUpScore;

Adafruit_GFX_Button restart;
int btnWidth = 100;
int btnHeight = 40;
int btnY = 200;

void drawPillar(int x, int y){
  
    // draw the pillar
    tft.fillRect(x+49,0,50,y-1, GREEN);
    // draw the black frame
    //tft.fillRect(x+50,0,x,y,BLACK);
    // blue rectangle right
    tft.fillRect(x+100,0,50,y-1,BLUE);

    // blue rectangle left
    //tft.fillRect(0,y+80,340-x,204,BLUE);
    // draw the pillar
    tft.fillRect(x+49,y+100,50,240-y-40, GREEN);
    // draw the black frame
    //tft.fillRect(x+50,y+80,x,204,BLACK);
    // blue rectangle right
    tft.fillRect(x+100,y+100,50,240-y-40,BLUE);
    Serial.print("x:"); Serial.println(x);
    if (x <= -50){
      tft.fillRect(0,0,50,300,BLUE);
    }
}

void gameOver(){
  delay(1000);
  tft.fillScreen(BLACK);
  
  tft.setCursor(100, 100);
  tft.setTextSize(3);
  tft.setTextColor(WHITE, BLACK);
  tft.print("GAME OVER");
  tft.setCursor(100, 150);
  tft.setTextSize(2);
  tft.setTextColor(WHITE, BLACK);
  tft.print("Score :");
  tft.print(score);

  
  //restart.initButton(&tft,tft.width()/2., 200, 2* btnWidth, btnHeight, WHITE, GREEN, BLACK, "Restart",2);
  //restart.drawButton(false);
  
  tft.setCursor(150, 200);
  tft.print("Restarting...");
  tft.setCursor(150, 250);
  tft.print(2);
  delay(1000);
  tft.setCursor(150, 250);
  tft.print(1);
  delay(1000);

  initiateGame();
}


void initiateGame()
{
  
  score = 0;
  xP = 320;
  yP = 100;
  yB = 50;
  movingRate = 3;
  fallRate = 0;
  fallRateInt = 0;
  
  // Blue background
  tft.fillScreen(BLUE);

  // Ground
  //tft.fillRect(120,310,210,30,YELLOW);
  

  // text
  tft.setCursor(150, 300);
  tft.setTextSize(3);
  tft.setTextColor(WHITE, GREY);
  tft.print("Score : ");

  
  
}

void setup(void){
  Serial.begin(9600);
  uint16_t ID = tft.readID();
  Serial.print("ID = "); Serial.println(ID, HEX);
  Serial.print("Width = "); Serial.println(tft.width());
  Serial.print("Height = "); Serial.println(tft.height());
  
  tft.begin(ID);

  
  
  tft.setRotation(1);

  initiateGame();

}

void loop(void){
    
    int x = 5, y, w = 62, h = 62;
    uint32_t t;
    const int SZ = w * h / 8;
    uint8_t sram[SZ];

    //p = ts.getPoint();
    

   
    //y = 100; t = millis();
    

    xP -= movingRate;
    drawPillar(xP, yP);

    yB += fallRate;
    
    if (fallRate < 4)
    fallRate+=0.4;
    
    //Serial.print("yB "); Serial.println(yB);
     
    fallRateInt = int(fallRate);

    if (yB >= 250){
      tft.fillRect(50, yB, 62 , 150 , BLUE);
      //tft.fillRect(50, yB + 60 , 62 , 62, BLUE);
      yB = 50;   
    }

    if (yB <= 10){
      
       yB = 50;
    }

    if ((xP<=50) && (xP>=5) && (yB<=yP)){

       gameOver();
    }

    if ((xP<=50) && (xP>=5) && (yB>=yP+100)){
    
       gameOver();
    }

    if (Touch_getXY() && !up){
       fallRate=-6;  
       up = true;  
       //Serial.println("Pressed");
    } else if (Touch_getXY() && up){
      up = false;
    }
    
    //delay(100);
    tft.drawRGBBitmap(50, yB, bird02, 62, 62);
    //Serial.print("yB:");Serial.println(yB);
    
    tft.fillRect(50, 0, 62 , yB , BLUE);
    tft.fillRect(50, yB + 62 , 62 , 240 , BLUE); 

    //Serial.print("yB "); Serial.println(yB);
    score++;

    if (xP <=-51){
      xP = 320;
      yP = rand()%100 + 20;
    }

    

    if ((score - lastSpeedUpScore) == 50){
      lastSpeedUpScore = score;
      movingRate++;
    }

    tft.setCursor(250, 300);
    tft.setTextSize(3);
    tft.setTextColor(WHITE, GREY);
    tft.print(score);

    //restart.press(up && restart.contains(pixel_x, pixel_y));

    /*
    if (restart.justPressed()){
      
      initiateGame();
    }*/
  
}

bool Touch_getXY(void)
{
 p = ts.getPoint();
 pinMode(A1, OUTPUT);      //restore shared pins
 pinMode(A2, OUTPUT);
 digitalWrite(A1, HIGH);
 digitalWrite(A2, HIGH);
 bool pressed = (p.z > MINPRESSURE && p.z < MAXPRESSURE);
 if (pressed) {
   if (tft.width() <= tft.height()) { //Portrait
     pixel_x = map(p.x, TS_LEFT, TS_RT, 0, tft.width()); //.kbv makes sense to me
     pixel_y = map(p.y, TS_TOP, TS_BOT, 0, tft.height());

     //Serial.print("p_x "); Serial.println(p.x);
     //Serial.print("pixel_x "); Serial.println(pixel_x);
   } else {
     pixel_x = map(p.y, 946, 134, 0, 480);
     pixel_y = map(p.x, 147, 913, 0, 320);

     //Serial.print("p_x, p_y "); Serial.print(p.x);Serial.print(" ");Serial.println(p.y);
     //Serial.print("pixel_x, pixel_y "); Serial.print(pixel_x);Serial.print(" ");Serial.println(pixel_y);
   }
 }
 return pressed;
}
