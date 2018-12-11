
#include "AmbMeter.h"

//-----------------------------------------------------------------------------

void ShowSensorDiags( BME280 &sensor )
{
    diag.println("BME280:");
    diag.println("");

    diag.printlnf(" [D0] CHIP_ID_REG  : %02Xh", sensor.readRegister(BME280_CHIP_ID_REG));
    diag.printlnf(" [E0] RST_REG      : %02Xh", sensor.readRegister(BME280_RST_REG));
    diag.printlnf(" [F4] CTRL_MEAS_REG: %02Xh", sensor.readRegister(BME280_CTRL_MEAS_REG));
    diag.printlnf(" [F2] CTRL_HUMIDITY: %02Xh", sensor.readRegister(BME280_CTRL_HUMIDITY_REG));
    diag.println("");

#if 0
    diag.println("Displaying all regs");
    diag.println("");

    uint8_t memCounter = 0x80;
    for(int rowi = 8; rowi < 16; rowi++ )
    {
        diag.printf(" %X0: ", rowi);
        for(int coli = 0; coli < 16; coli++ )
        {
            diag.printf("%02X ", sensor.readRegister(memCounter));
            memCounter++;
        }
        diag.println("");
    }
    diag.println("");

    diag.println("Displaying concatenated calibration words");
    diag.println("");

    diag.printlnf(" dig_T1, u16: %u", sensor.calibration.dig_T1);
    diag.printlnf(" dig_T2, i16: %d", sensor.calibration.dig_T2);
    diag.printlnf(" dig_T3, i16: %d", sensor.calibration.dig_T3);
    diag.printlnf(" dig_P1, u16: %u", sensor.calibration.dig_P1);
    diag.printlnf(" dig_P2, i16: %d", sensor.calibration.dig_P2);
    diag.printlnf(" dig_P3, i16: %d", sensor.calibration.dig_P3);
    diag.printlnf(" dig_P4, i16: %d", sensor.calibration.dig_P4);
    diag.printlnf(" dig_P5, i16: %d", sensor.calibration.dig_P5);
    diag.printlnf(" dig_P6, i16: %d", sensor.calibration.dig_P6);
    diag.printlnf(" dig_P7, i16: %d", sensor.calibration.dig_P7);
    diag.printlnf(" dig_P8, i16: %d", sensor.calibration.dig_P8);
    diag.printlnf(" dig_P9, i16: %d", sensor.calibration.dig_P9);
    diag.printlnf(" dig_H1,  u8: %u", sensor.calibration.dig_H1);
    diag.printlnf(" dig_H2, i16: %d", sensor.calibration.dig_H2);
    diag.printlnf(" dig_H3,  u8: %u", sensor.calibration.dig_H3);
    diag.printlnf(" dig_H4, i16: %d", sensor.calibration.dig_H4);
    diag.printlnf(" dig_H5, i16: %d", sensor.calibration.dig_H5);
    diag.printlnf(" dig_H6,  u8: %u", sensor.calibration.dig_H6);
    diag.println("");
#endif
}


//-----------------------------------------------------------------------------
