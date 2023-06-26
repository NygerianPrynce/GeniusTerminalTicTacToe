//tests.cpp - learn how to control the lights here!
//by Alison N. Norman

#include "tests.h"

//blink the light closest to the controller
void blink_light_0_example()
{
  leds[0]=CRGB::White; //defined in colors (pixeltypes.h), sets led 0 to white in strand

  FastLED.delay(1000); //pauses the program for the specified number of milliseconds
                       //do this to display the current light settings for a particular amount of time
                       //calls show, which mirrors the values in leds to the strand

  leds[0]=CRGB::Black; //Black turns the led off

  FastLED.delay(1000);

}

//Function to blink the first and last lights
void blink_lights_0_and_24()
{
  
}

//Function to blink all lights
void blink_all_lights()
{


}

//Function to blink every other light
void blink_alternating_lights()
{
  

}


//Function to fade the brightness of all
void fade_brightness()
{
  //first give each a color
  for(int i = 0; i < NUM_LEDS; i++)
  {
    leds[i].r=random(255);   //random is in the Arduino library.  Sends back a random number between 0 and its argument, inclusive.
    leds[i].g=random(255);   //One way to set the color of the LEDs to a custom color.
    leds[i].b=random(255);
  }
  
//  for()
//  {
//    FastLED.setBrightness(brightness);
//    FastLED.delay(50);
//  }
}


void chase()
{

}

void chase_two_colors_with_scroll()
{


}


