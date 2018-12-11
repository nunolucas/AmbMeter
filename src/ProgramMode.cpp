
#include "AmbMeter.h"

//-----------------------------------------------------------------------------

void ProgramMode::event(Event ev)
{
    // Run state-machine
    switch ( mode_ )
    {
    case Mode::Init:
        // Can only change to ModeIdle. nothing else to do.
        if ( ev == Event::Begin )
        {
            mode_ = Mode::Idle;
            // Acquire LED control
            RGB.control( true );
            RGB.brightness( 0 );
        }
        break;
    case Mode::Idle:
        // Can change to ModeLedShow. Start Led show mode.
        if ( ev == Event::ButtonNext )
        {
            mode_ = Mode::Connected;
            // ReleaseLedControl()
            RGB.brightness( 0x40 );
            RGB.control( false );
            // Start WiFi and connect to Cloud
            WiFi.on();
            Particle.connect();

        }
        break;
    case Mode::Connected:
        // Let WiFi run until ButtonNext event
        if ( ev == Event::ButtonNext )
        {
            mode_ = Mode::Idle;
            // Disconnect from the cloud and turn off WiFI
            Particle.disconnect();
            WiFi.disconnect();
            WiFi.off();
            // (re-)Acquire LED control
            RGB.control( true );
            RGB.brightness( 0 );
        }
    }
}

//-----------------------------------------------------------------------------
