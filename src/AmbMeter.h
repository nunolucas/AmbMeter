#ifndef AMBMETER_H_INCLUDED_
#define AMBMETER_H_INCLUDED_
//-----------------------------------------------------------------------------

#include <Particle.h>

//-----------------------------------------------------------------------------
// Pins used by the OLED Display (SPI)

#define LCD_PIN_RST     A0
#define LCD_PIN_DC      A1
#define LCD_PIN_CS      A2 /* SPI1_SS   */
#define LCD_PIN_CLK     A3 /* SPI1_CLK  */
#define LCD_PIN_DI      A5 /* SPI1_MOSI */

//-----------------------------------------------------------------------------
// I/O pin used by the DHT11

#define PIN_DHT11_IO    D5

//-----------------------------------------------------------------------------
// I/O pin used by "SmartPixel" strip

#define SMART_PIXEL_COUNT   3
#define SMART_PIXEL_PIN     D6
// #define PIXEL_TYPE WS2812B

//-----------------------------------------------------------------------------
// BME280 sensor

void bme_setup();
bool bme_get( double &temperature, double &humidity, double &pressure );

//-----------------------------------------------------------------------------
// DHT11 sensor

void dht_setup();
bool dht_get( double &temperature, double &humidity );

//-----------------------------------------------------------------------------
// NeoPixel RGB SmartPixel LED strip

void neostrip_setup();

//-----------------------------------------------------------------------------

// Use Serial1 as the diagnostic serial port
#define diag    Serial1

//-----------------------------------------------------------------------------
// Program modes

enum Mode {
    Init = 0,
    Idle,
    ReadDHT11,
    LedShow,
    Connected,
};

//-----------------------------------------------------------------------------
// Execution events

enum Event {
    Begin,
    ButtonNext,
    Tick,
};

//-----------------------------------------------------------------------------
// ProgramMode state machine

class ProgramMode
{
public:
    ProgramMode() : mode_(Mode::Init) {}

    void begin() { event(Event::Begin); }
    void next()  { event(Event::ButtonNext);  }
    void tick()  { event(Event::Tick);  }

private:
    void event(Event ev);
    Mode mode_;
    uint32_t rgb_;
    uint8_t lux_;
    bool inc_;
};

//-----------------------------------------------------------------------------
#endif  // AMBMETER_H_INCLUDED_
