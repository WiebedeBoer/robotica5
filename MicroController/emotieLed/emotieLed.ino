#include <FastLED.h>

#define LED_PIN     10
#define NUM_LEDS    28
#define BRIGHTNESS  64
#define LED_TYPE    WS2811
#define COLOR_ORDER RGB
CRGB leds[NUM_LEDS];

CRGB color = CRGB(0,0,255);
CRGBPalette16 currentPalette;
TBlendType    currentBlending;

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
  color.red -= 20;
  color.blue += 10;
  color.green -= 30;
  closeEye(150);
  delay(200);
  openEye(150);
  delay(400);  
  
  color.red -= 20;
  color.blue += 10;
  color.green -= 30;
  
  closeCircle(300);
  delay(300);
  openCircle(300);
  delay(400);
  if(NUM_LEDS == 28){
    color.red -= 20;
    color.blue += 10;
    color.green -= 30;
    closeEye2(150);
    delay(200);
    openEye2(150);
    delay(400);  
    
    color.red -= 20;
    color.blue += 10;
    color.green -= 30;
    
    closeCircle2(300);
    delay(300);
    openCircle2(300);
    delay(400);
  }
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
  leds[3+14] = CRGB(0,0,0);
  FastLED.show();
  delay(sleep);

  leds[4+14] = CRGB(0,0,0);
  leds[2+14] = CRGB(0,0,0);
  FastLED.show();
  delay(sleep);
  
  leds[5+14] = CRGB(0,0,0);
  leds[11+14] = CRGB(0,0,0);
  leds[13+14] = CRGB(0,0,0);
  leds[10+14] = CRGB(0,0,0);
  leds[1+14] = CRGB(0,0,0);
  FastLED.show();
  delay(sleep);

  leds[12+14] = CRGB(0,0,0);
  leds[9+14] = CRGB(0,0,0);
  leds[6+14] = CRGB(0,0,0);
  leds[0+14] = CRGB(0,0,0);
  FastLED.show();
  delay(sleep);

  leds[7+14] = CRGB(0,0,0);
  leds[8+14] = CRGB(0,0,0);
  FastLED.show();
  delay(sleep);
}

void openEye2(int sleep){
  leds[7+14] = color;
  leds[8+14] = color;
  FastLED.show();
  delay(sleep);

  leds[12+14] = color;
  leds[9+14] = color;
  leds[6+14] = color;
  leds[0+14] = color;
  FastLED.show();
  delay(sleep);

  leds[5+14] = color;
  leds[11+14] = color;
  leds[13+14] = color;
  leds[10+14] = color;
  leds[1+14] = color;
  FastLED.show();
  delay(sleep);

  leds[4+14] = color;
  leds[2+14] = color;
  FastLED.show();
  delay(sleep);

  leds[3+14] = color;
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
