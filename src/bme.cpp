// Temperature, Pressure & Humidity sensor (I²C via pins D0,1)

#include "AmbMeter.h"

#include <SparkFunBME280.h>

//-----------------------------------------------------------------------------

static void sensor_timer();

//-----------------------------------------------------------------------------

static BME280       s_sensor;
static bool         s_valid;
static double       s_temperature;
static double       s_humidity;
static double       s_pressure;
static Timer        s_timer( 500, sensor_timer );

//-----------------------------------------------------------------------------

void bme_setup()
{
    // Setup GY-BME280 sensor (I²C)
    diag.println( "Starting the BME280 sensor... " );
    s_sensor.settings.commInterface  = I2C_MODE;
    s_sensor.settings.I2CAddress     = 0x76;
    s_sensor.settings.runMode        = 3;       //  3: Normal mode
    s_sensor.settings.tStandby       = 0;       //  0: 0.5ms
    s_sensor.settings.filter         = 16;      //  0: filter off
    // Oversampling can be:
    //  0   : skipped
    //  1..5: oversampling *1, *2, *4, *8, *16 respectively
    s_sensor.settings.pressOverSample = 4;
    s_sensor.settings.tempOverSample  = 1;
    s_sensor.settings.humidOverSample = 0;

    // Calling .begin() causes the settings to be loaded
    s_sensor.begin();

    // Show some diagnostic information about the sensor
    diag.println("");
    diag.println("BME280:");
    diag.println("");

    diag.printlnf(" [D0] CHIP_ID_REG  : %02Xh", s_sensor.readRegister(BME280_CHIP_ID_REG));
    diag.printlnf(" [E0] RST_REG      : %02Xh", s_sensor.readRegister(BME280_RST_REG));
    diag.printlnf(" [F4] CTRL_MEAS_REG: %02Xh", s_sensor.readRegister(BME280_CTRL_MEAS_REG));
    diag.printlnf(" [F2] CTRL_HUMIDITY: %02Xh", s_sensor.readRegister(BME280_CTRL_HUMIDITY_REG));
    diag.println("");

    diag.println("Displaying all regs");
    diag.println("");

    uint8_t memCounter = 0x80;
    for(int rowi = 8; rowi < 16; rowi++ )
    {
        diag.printf(" %X0: ", rowi);
        for(int coli = 0; coli < 16; coli++ )
        {
            diag.printf("%02X ", s_sensor.readRegister(memCounter));
            memCounter++;
        }
        diag.println("");
    }
    diag.println("");

    diag.println("Displaying concatenated calibration words");
    diag.println("");

    diag.printlnf(" dig_T1, u16: %u", s_sensor.calibration.dig_T1);
    diag.printlnf(" dig_T2, i16: %d", s_sensor.calibration.dig_T2);
    diag.printlnf(" dig_T3, i16: %d", s_sensor.calibration.dig_T3);
    diag.printlnf(" dig_P1, u16: %u", s_sensor.calibration.dig_P1);
    diag.printlnf(" dig_P2, i16: %d", s_sensor.calibration.dig_P2);
    diag.printlnf(" dig_P3, i16: %d", s_sensor.calibration.dig_P3);
    diag.printlnf(" dig_P4, i16: %d", s_sensor.calibration.dig_P4);
    diag.printlnf(" dig_P5, i16: %d", s_sensor.calibration.dig_P5);
    diag.printlnf(" dig_P6, i16: %d", s_sensor.calibration.dig_P6);
    diag.printlnf(" dig_P7, i16: %d", s_sensor.calibration.dig_P7);
    diag.printlnf(" dig_P8, i16: %d", s_sensor.calibration.dig_P8);
    diag.printlnf(" dig_P9, i16: %d", s_sensor.calibration.dig_P9);
    diag.printlnf(" dig_H1,  u8: %u", s_sensor.calibration.dig_H1);
    diag.printlnf(" dig_H2, i16: %d", s_sensor.calibration.dig_H2);
    diag.printlnf(" dig_H3,  u8: %u", s_sensor.calibration.dig_H3);
    diag.printlnf(" dig_H4, i16: %d", s_sensor.calibration.dig_H4);
    diag.printlnf(" dig_H5, i16: %d", s_sensor.calibration.dig_H5);
    diag.printlnf(" dig_H6,  u8: %u", s_sensor.calibration.dig_H6);
    diag.println("");

    // Setup timer that will read the sensor data
    s_timer.start();
}

//-----------------------------------------------------------------------------

bool bme_get( double &temperature, double &humidity, double &pressure )
{
    temperature = s_temperature;
    humidity = s_humidity;
    pressure = s_pressure;
    return s_valid;
}

//-----------------------------------------------------------------------------
// Timer triggered every time we want to have a new read of the sensor data.

static
void sensor_timer( )
{
    // Start with temperature, as that data is needed for accurate compensation.
    // Reading the temperature updates the compensators of the other functions
    // in the background.
    s_temperature = s_sensor.readTempC();
    s_humidity    = s_sensor.readFloatHumidity();
    s_pressure    = s_sensor.readFloatPressure() / 100.0;
    s_valid       = true;
}

//-----------------------------------------------------------------------------
