#include "graphic.h"
 #include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
RF24 radio(9,10); // CE, CSN
const byte address[6] = "00001";
char xyData[32] = "";
int joystick[2];

// this constant won't change:
const int buttonPin = 2;  // the pin that the pushbutton is attached to

// Variables will change:
int buttonPushCounter = 0;  // counter for the number of button presses
int buttonState = 0;        // current state of the button
int lastButtonState = 0;    // previous state of the button

void f_drawJoystickInfo(int x, int y);
void setup() {
  pinMode(buttonPin, INPUT_PULLUP);
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
  
  Serial.begin(9600);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
    for(;;); // Don't proceed, loop forever
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  display.display();
  delay(2000); // Pause for 2 seconds

  // Clear the buffer
  display.clearDisplay();

  // Draw a single pixel in white
 display.setTextColor(BLACK,WHITE);
 display.drawBitmap(0,0,epd_bitmap_agh,128,64,WHITE);
  // Show the display buffer on the screen. You MUST call display() after
  // drawing commands to make them visible on screen!
  display.display();
  delay(2000);


}
void loop() {
    
    
     buttonState = digitalRead(buttonPin);

  // compare the buttonState to its previous state
  if (buttonState != lastButtonState) {
    // if the state has changed, increment the counter
    if (buttonState == HIGH) {
      // if the current state is HIGH then the button went from off to on:
      buttonPushCounter++;
    } else {
      // if the current state is LOW then the button went from on to off:

    }
    // Delay a little bit to avoid bouncing
    delay(50);
  }
  // save the current state as the last state, for next time through the loop
  lastButtonState = buttonState;


  // turns on the LED every four button pushes by checking the modulo of the
  // button push counter. the modulo function gives you the remainder of the
  // division of two numbers:
  if (buttonPushCounter % 2 == 0) {
    joystick[0] = analogRead(A3);
  joystick[1] = analogRead(A2);
   test_joy(joystick[1], joystick[0]);
  } 
  else {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE); 
  display.setCursor(32,0);
  display.println("TRYB JAZDY");
  display.display();
  joystick[0] = analogRead(A3);
  joystick[1] = analogRead(A2);
  radio.write( joystick, sizeof(joystick) );

  }
}
  
  


void test_joy(int x, int y) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println("Joystick");
  display.println("TESTER");
  display.println();
  display.print("X: ");
  display.println(x);
  display.print("Y: ");
  display.println(y);

  int x1 = map(x, 0, 1023, display.width() - 60 - 2, display.width() - 2);
  int y1 = map(y, 0, 1023, display.height() - 60 - 2, display.height() - 2);
  display.drawRect(display.width() - 60 - 2, display.height() - 60 - 2, 60, 60, WHITE);  
  display.drawLine(x1, y1 - 2, x1, y1 + 2, WHITE);  
  display.drawLine(x1 - 2, y1, x1 + 2, y1, WHITE);

  display.display();
}
