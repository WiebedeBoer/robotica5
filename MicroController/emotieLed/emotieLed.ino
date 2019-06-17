#include <FastLED.h>

#define LED_PIN     2
#define NUM_LEDS    28
#define BRIGHTNESS  64 //128
#define LED_TYPE    WS2811
#define COLOR_ORDER RGB
CRGB leds[NUM_LEDS];

CRGB color = CRGB(0,0,255);
CRGBPalette16 currentPalette;
TBlendType    currentBlending;
int sleep = 200;
extern CRGBPalette16 myRedWhiteBluePalette;
extern const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM;

#define UPDATES_PER_SECOND 100

void setup() {
    delay( 3000 ); // power-up safety delay
    FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
    FastLED.setBrightness(  BRIGHTNESS );
    
    currentPalette = RainbowColors_p;
    currentBlending = LINEARBLEND;

}

void loop() {
  static uint8_t startIndex = 0;
  startIndex = startIndex + 1; /* motion speed */
  //setDefault();
 // turn_on(900, 255, 0, 255);
//  turn_on(900, 255, 0, 0);
//  turn_on(900, 0, 255, 255);
 // Grijs(200);
  delay(500);
//  Wit(200);
  delay(500);
//  Cyaan(200);
    Blauw(10000);
    closeEye2(250);
    delay(200); 
    openEye2(250, color);
  if(NUM_LEDS == 28){
     delay(400);  
  }
}
void turn_on(int sleep, int c1, int c2, int c3){
    int temp1 = 0;
    int temp2 = 0;
    int temp3 = 0;
    while(temp1< c1 || temp2 < c2 || temp3 << c3){
      for( int i = 0; i < NUM_LEDS; i++) {
        //leds[i] = ColorFromPalette( currentPalette, colorIndex, brightness, currentBlending);
        leds[i] = CRGB(temp1,temp2,temp3);
      }
      if(temp1< c1){
        temp1 += 5;
      }
      if(temp2< c2){
        temp2 += 5;
      }
      if(temp3< c3){
        temp3 += 5;
      }
      FastLED.show();
      delay(100);
    }
    delay(sleep);
}
void FillLEDsFromPaletteColors( uint8_t colorIndex)
{
    uint8_t brightness = 255;
    for( int i = 0; i < NUM_LEDS; i++) {
        //leds[i] = ColorFromPalette( currentPalette, colorIndex, brightness, currentBlending);
        leds[i] = CRGB(0,0,255);
        colorIndex += 3;
    }

}

void setDefault(){
    uint8_t brightness = 255;  
    for( int i = 0; i < NUM_LEDS; i++) {
        //leds[i] = ColorFromPalette( currentPalette, colorIndex, brightness, currentBlending);
        
        leds[i] = color;
        FastLED.show();
        delay(150);
        //colorIndex += 3;
    }
}
void closeEye(int sleep){
  leds[3] = CRGB(0,0,0);
  FastLED.show();
  delay(sleep);

  leds[4] = CRGB(0,0,0);
  leds[2] = CRGB(0,0,0);
  FastLED.show();
  delay(sleep);
  
  leds[5] = CRGB(0,0,0);
  leds[11] = CRGB(0,0,0);
  leds[13] = CRGB(0,0,0);
  leds[10] = CRGB(0,0,0);
  leds[1] = CRGB(0,0,0);
  FastLED.show();
  delay(sleep);

  leds[12] = CRGB(0,0,0);
  leds[9] = CRGB(0,0,0);
  leds[6] = CRGB(0,0,0);
  leds[0] = CRGB(0,0,0);
  FastLED.show();
  delay(sleep);

  leds[7] = CRGB(0,0,0);
  leds[8] = CRGB(0,0,0);
  FastLED.show();
  delay(sleep);
}

void openEye(int sleep){
  leds[7] = color;
  leds[8] = color;
  FastLED.show();
  delay(sleep);

  leds[12] = color;
  leds[9] = color;
  leds[6] = color;
  leds[0] = color;
  FastLED.show();
  delay(sleep);

  leds[5] = color;
  leds[11] = color;
  leds[13] = color;
  leds[10] = color;
  leds[1] = color;
  FastLED.show();
  delay(sleep);

  leds[4] = color;
  leds[2] = color;
  FastLED.show();
  delay(sleep);

  leds[3] = color;
  FastLED.show();
  delay(sleep);
}

void closeCircle(int sleep){
  for(int i = 0; i <9; i++){
     leds[i] = CRGB(0,0,0);
  }
  FastLED.show();
  delay(sleep);
    
  for(int i = 9; i <13; i++){
     leds[i] = CRGB(0,0,0);
  }
  FastLED.show();
  delay(sleep);

  leds[13] = CRGB(0,0,0);
  FastLED.show();
}

void openCircle(int sleep){
  leds[13] = color;
  FastLED.show();
  delay(sleep);

  for(int i = 9; i <13; i++){
     leds[i] = color;
  }
  FastLED.show();
  delay(sleep);

  for(int i = 0; i <9; i++){
     leds[i] = color;
  }
  FastLED.show();
}

void closeEye2(int sleep){
  leds[15] = CRGB(0,0,0);
  FastLED.show();
  delay(sleep);

  leds[16] = CRGB(0,0,0);
  leds[14] = CRGB(0,0,0);
  FastLED.show();
  delay(sleep);
  
  leds[17] = CRGB(0,0,0);
  leds[24] = CRGB(0,0,0);
  leds[23] = CRGB(0,0,0);
  leds[22] = CRGB(0,0,0);
  leds[27] = CRGB(0,0,0);
  FastLED.show();
  delay(sleep);

  leds[18] = CRGB(0,0,0);
  leds[25] = CRGB(0,0,0);
  leds[26] = CRGB(0,0,0);
  leds[21] = CRGB(0,0,0);
  FastLED.show();
  delay(sleep);

  leds[19] = CRGB(0,0,0);
  leds[20] = CRGB(0,0,0);
  FastLED.show();
  delay(sleep);
}

void openEye2(int sleep, CRGB color){
  leds[19] = color;
  leds[20] = color;
  FastLED.show();
  delay(sleep);
  
  leds[18] = color;
  leds[25] = color;
  leds[26] = color;
  leds[21] = color;
  FastLED.show();
  delay(sleep);

  leds[17] = color;
  leds[24] = color;
  leds[23] = color;
  leds[22] = color;
  leds[27] = color;
  FastLED.show();
  delay(sleep);

  leds[16] = color;
  leds[14] = color;
  FastLED.show();
  delay(sleep);


  leds[15] = color;
  FastLED.show();
  delay(sleep);
}

void closeCircle2(int sleep){
  for(int i = 0; i <9; i++){
     leds[i+14] = CRGB(0,0,0);
  }
  FastLED.show();
  delay(sleep);
    
  for(int i = 9; i <13; i++){
     leds[i+14] = CRGB(0,0,0);
  }
  FastLED.show();
  delay(sleep);

  leds[13+14] = CRGB(0,0,0);
  FastLED.show();
}

void openCircle2(int sleep){
  leds[13+14] = color;
  FastLED.show();
  delay(sleep);

  for(int i = 9; i <13; i++){
     leds[i+14] = color;
  }
  FastLED.show();
  delay(sleep);

  for(int i = 0; i <9; i++){
     leds[i+14] = color;
  }
  FastLED.show();
}

void Rood(int sleep){
  for(int i = 0; i < NUM_LEDS; i++){
    leds[i] = CRGB(0,255,0);
  }
  
  FastLED.show();
  delay(sleep);
}

void Groen(int sleep){
  for(int i = 0; i < NUM_LEDS; i++){
    leds[i] = CRGB(255,0,0);
  }
  
  FastLED.show();
  delay(sleep);
}

void Blauw(int sleep){
  for(int i = 0; i < NUM_LEDS; i++){
    leds[i] = CRGB(0,0,255);
  }
  
  FastLED.show();
  delay(sleep);
  color = CRGB(0,0,255);
}

void Geel(int sleep){
  for(int i = 0; i < NUM_LEDS; i++){
    leds[i] = CRGB(255,255,0);
  }
  
  FastLED.show();
  delay(sleep);
  color = CRGB(255,255,0);
}

void Roze(int sleep){
  for(int i = 0; i < NUM_LEDS; i++){
    leds[i] = CRGB(0,255,255);
  }
  
  FastLED.show();
  delay(sleep);
  color = CRGB(0,255,255);
}


void Cyaan(int sleep){
  for(int i = 0; i < NUM_LEDS; i++){
    leds[i] = CRGB(255,0,255);
  }
  
  FastLED.show();
  delay(sleep);
  color = CRGB(255,0,255);
}

void Wit(int sleep){
  for(int i = 0; i < NUM_LEDS; i++){
    leds[i] = CRGB(255,255,255);
  }
  
  FastLED.show();
  delay(sleep);
  color = CRGB(255,255,255);
}

void Grijs(int sleep){
  for(int i = 0; i < NUM_LEDS; i++){
    leds[i] = CRGB(200,200,200);
  }
  
  FastLED.show();
  delay(sleep);
  color = CRGB(200,200,200);
}


void set_color(int led, CRGB col){
  leds[led] = col;
  FastLED.show();
  color = col;
}
