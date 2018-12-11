#ifndef AMBMETER_H_INCLUDED_
#define AMBMETER_H_INCLUDED_
//-----------------------------------------------------------------------------

#include <Particle.h>
#include <SparkFunBME280.h>

//-----------------------------------------------------------------------------
// Pins used by the OLED Display (SPI)

#define LCD_PIN_RST     A0
#define LCD_PIN_DC      A1
#define LCD_PIN_CS      A2 /* SPI1_SS   */
#define LCD_PIN_CLK     A3 /* SPI1_CLK  */
#define LCD_PIN_DI      A5 /* SPI1_MOSI */

//-----------------------------------------------------------------------------

// Use Serial1 as the diagnostic serial port
#define diag    Serial1

//-----------------------------------------------------------------------------
// Program modes

enum Mode {
    Init = 0,
    Idle,
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

void ShowSensorDiags( BME280 &sensor );

//-----------------------------------------------------------------------------
#endif  // AMBMETER_H_INCLUDED_
