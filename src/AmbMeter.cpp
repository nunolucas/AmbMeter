
#include <HCuOLED.h>

#include "AmbMeter.h"

//-----------------------------------------------------------------------------
// Temperature, Pressure & Humidity sensor (I²C via pins D0,1)

BME280          sensor;

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
    // Setup diagnostic output
    diag.begin( 9600 );
    diag.println( "%%%%%% Application reset... %%%%%%" );
    diag.println( "" );

    // Setup GY-BME280 sensor (I²C)
    diag.println( "Starting BME280... " );
    sensor.settings.commInterface  = I2C_MODE;
    sensor.settings.I2CAddress     = 0x76;
    sensor.settings.runMode        = 3;       //  3: Normal mode
    sensor.settings.tStandby       = 0;       //  0: 0.5ms
    sensor.settings.filter         = 0;       //  0: filter off
    // Oversampling can be:
    //  0   : skipped
    //  1..5: oversampling *1, *2, *4, *8, *16 respectively
    sensor.settings.tempOverSample  = 1;
    sensor.settings.pressOverSample = 1;
    sensor.settings.humidOverSample = 1;
    // Calling .begin() causes the settings to be loaded
    sensor.begin();

    // Show some diagnostic information about the sensor
    ShowSensorDiags( sensor );

    // Setup LCD display
    diag.println( "Starting LCD... " );
    hcu.Reset();
    hcu.SetFont( Terminal_8pt );
    hcu.Cursor( 92, 8 );   hcu.Print( "C" );
    hcu.Cursor( 92,23 );   hcu.Print( "%RH" );
    hcu.Cursor( 92,38 );   hcu.Print( "mbar" );
    hcu.Cursor( 92,53 );   hcu.Print( "m(!)" );
    hcu.SetFont( MedProp_11pt );

    // Setup handler for user button
    System.on( button_status, button_pressed );

    // Setup diagnostic table headers
    diag.println("");
    diag.println("   °C     %RH       hPa          m         ft");
    diag.println(" ------  -----  ---------  ---------  ---------");

    // Finish setup phase by starting a valid program mode
    mode.begin();
}

//-----------------------------------------------------------------------------
// Application execution loop (about 2 Hz) 

void loop() {
    // Start with temperature, as that data is needed for accurate compensation.
    // Reading the temperature updates the compensators of the other functions
    // in the background.
    double temp  = sensor.readTempC();
    double hpa   = sensor.readFloatPressure() / 100.0;
    double alt_m = sensor.readFloatAltitudeMeters();
    double alt_f = sensor.readFloatAltitudeFeet();
    double hum   = sensor.readFloatHumidity();

    // Let ProgramMode state-machine tick away
    mode.tick();

    // Update OLED display
    hcu.Erase( 20,4, 90,60 );

    hcu.Cursor( 20, 4 );    hcu.Print( temp , 7, 1 );
    hcu.Cursor( 20,19 );    hcu.Print( hum  , 7, 1 );
    hcu.Cursor( 20,34 );    hcu.Print( hpa  , 7, 1 );
    hcu.Cursor( 20,49 );    hcu.Print( alt_m, 7, 1 );

    hcu.Refresh( );

    // Show diag line with sensor data.
    diag.printf( " %6.2f  %5.1f  %9.3f  %9.1f  %9.1f\r", temp, hum, hpa, alt_m, alt_f );

    // Change line every minute.
    static unsigned long line_timer = millis();
    if ( millis() - line_timer > 60000 ) {
        line_timer = millis();
        diag.println("");
    }

    // Updates will be twice per second
    delay( 500 );
}

//-----------------------------------------------------------------------------
