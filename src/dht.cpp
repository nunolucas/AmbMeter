
#include "AmbMeter.h"

#include <PietteTech_DHT.h>

//-----------------------------------------------------------------------------

static void sensor_timer( void );

//-----------------------------------------------------------------------------
// Temperature & Humidity sensor (signal I/O via pin D5)

static PietteTech_DHT   s_sensor( PIN_DHT11_IO, DHT11 );
static double           s_temperature;
static double           s_humidity;
static Timer            s_timer( 60000, sensor_timer );

//-----------------------------------------------------------------------------

void dht_setup()
{
    // Setup timer that will read the sensor data
    s_timer.start();
}

//-----------------------------------------------------------------------------

bool dht_get( double &temperature, double &humidity )
{
    temperature = s_temperature;
    humidity = s_humidity;

    return true;
}

//-----------------------------------------------------------------------------
// Timer triggered every time we want to have a new read of the sensor data.

static
void sensor_timer( )
{
    s_sensor.acquireAndWait();

    s_temperature = s_sensor.getCelsius();
    s_humidity    = s_sensor.getHumidity();
}

//-----------------------------------------------------------------------------
