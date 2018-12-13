
#include <HCuOLED.h>

#include "AmbMeter.h"

//-----------------------------------------------------------------------------
// SH1106 OLED 2.8' LCD (SPI1 is implicit)

HCuOLED         hcu( SH1106, LCD_PIN_CS, LCD_PIN_DC, LCD_PIN_RST );

//-----------------------------------------------------------------------------
// Application mode state-machine

ProgramMode     mode;

//-----------------------------------------------------------------------------
// Let system run in a separate thread and only start network on request

SYSTEM_THREAD( ENABLED );
SYSTEM_MODE( MANUAL );

//-----------------------------------------------------------------------------
// Handler for the user button (SETUP button)

static void button_pressed( system_event_t, int duration )
{
    // Check duration to only act on the 1st press event
    if ( duration == 0 )
        mode.next();    // switch to next program mode
}

//-----------------------------------------------------------------------------
// Setup hardware and application state

void setup()
{
    diag.begin( 9600 ); // Setup diagnostic output
    bme_setup();        // Setup BME280
    dht_setup();        // Setup DHT11
    neostrip_setup();   // Setup NeoPixel strip
    hcu.Reset();        // Setup LCD display

    // Setup handler for user button
    System.on( button_status, button_pressed );

    // Finish setup phase by starting a valid program mode
    mode.begin();
}

//-----------------------------------------------------------------------------
// Application execution loop (about 2 Hz) 

void loop()
{
    // Let ProgramMode state-machine tick away
    mode.tick();

    // Update OLED display
    double temp, hum, hpa;
    hcu.Erase(0,0, 127,63);

    // Update sensor 1 data in the OLED display (BME280)
    bme_get(temp, hum, hpa);

    hcu.SetFont( LCDLarge_24pt );
    hcu.Cursor( 7, 0 );
    hcu.Print( temp, 4,1 );
    hcu.SetFont( Terminal_8pt );
    hcu.Print( "o" );
    hcu.SetFont( MedProp_11pt );
    hcu.Cursor( 32, 36 );
    hcu.Print( static_cast<long>(hum + 0.5) );
    hcu.SetFont( Terminal_8pt );
    hcu.Print( "%" );

    hcu.SetFont( Terminal_8pt );
    hcu.Cursor( 10,56 );
    hcu.Print( hpa, 7,1 );
    hcu.Print( "mbar" );

    // Update sensor 2 data in the OLED display (DHT11)
    dht_get(temp, hum);

    hcu.SetFont( LCDLarge_24pt );
    hcu.Cursor( 70, 0 );
    hcu.Print( temp, 4,1 );
    hcu.SetFont( Terminal_8pt );
    hcu.Print( "o" );
    hcu.SetFont( MedProp_11pt );
    hcu.Cursor( 88, 36 );
    hcu.Print( static_cast<long>(hum + 0.5) );
    hcu.SetFont( Terminal_8pt );
    hcu.Print( "%" );

    hcu.Refresh( );

    // Refresh screen twice per second
    delay( 500 );
}

//-----------------------------------------------------------------------------
