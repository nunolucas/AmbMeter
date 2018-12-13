
#include "AmbMeter.h"

#include <neopixel.h>

//-----------------------------------------------------------------------------

static void rainbow_effect( void );
static uint32_t Wheel( uint8_t );

//-----------------------------------------------------------------------------
// "SmartPixel" strip

static Adafruit_NeoPixel    s_strip( SMART_PIXEL_COUNT, SMART_PIXEL_PIN );
static unsigned             s_i, s_j;
static Timer                s_timer( 5, rainbow_effect );

//-----------------------------------------------------------------------------

void neostrip_setup()
{
    // Setup NeoPixel strip and timer
    s_strip.begin();
    s_timer.start();
}

//-----------------------------------------------------------------------------

static void rainbow_effect( )
{
    s_strip.setPixelColor( s_i, Wheel(s_i + s_j) );
    s_strip.show();

    if ( ++s_i >= s_strip.numPixels() )
    {
        s_i = 0;
        s_j = (s_j + 1) & 0xFF;
    }
}

//-----------------------------------------------------------------------------
// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.

static uint32_t Wheel( uint8_t WheelPos )
{
    if ( WheelPos < 85 )
        return s_strip.Color( WheelPos * 3, 255 - WheelPos * 3, 0 );

    if ( WheelPos < 170 )
    {
        WheelPos -= 85;
        return s_strip.Color( 255 - WheelPos * 3, 0, WheelPos * 3 );
    }

    WheelPos -= 170;
    return s_strip.Color( 0, WheelPos * 3, 255 - WheelPos * 3 );
}

//-----------------------------------------------------------------------------
