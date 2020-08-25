/*
 * L3G4200D.c
 *
 *  Created on: Jul 27, 2019
 *      Author: Itachi
 */

#include "Userlibs.h"
#include "Delay_Systick.h"
#include "L3G4200D.h"
#include "I2C.h"

void        L3G4200D_Init(void)
{
    delay_us(10000);
    I2C_Write_Byte(L3G4200D_ADDR, CTRL_REG1, 0x0C);
    delay_us(1000);
    I2C_Write_Byte(L3G4200D_ADDR, CTRL_REG2, 0x00);
    delay_us(1000);
    I2C_Write_Byte(L3G4200D_ADDR, CTRL_REG3, 0x00);
    delay_us(1000);
    I2C_Write_Byte(L3G4200D_ADDR, CTRL_REG4, 0x80);
    delay_us(1000);
    I2C_Write_Byte(L3G4200D_ADDR, CTRL_REG5, 0x00);
    delay_us(1000);
//    while( (I2C_Read_Byte(L3G4200D_ADDR, WHO_AM_I)!=0xD3) );
}

int16_t     L3G4200D_Get_RawValue(void)
{
    int16_t i16_realdata=0;
    uint8_t ui8lowbyte=0,ui8highbyte=0;
    uint8_t ui8_confirm=0;
    ui8_confirm=I2C_Read_Byte(L3G4200D_ADDR, WHO_AM_I);
    if(ui8_confirm==0xD3)
    {
        ui8lowbyte=I2C_Read_Byte(L3G4200D_ADDR, OUT_Z_L);
//        delay_us(10);
        ui8highbyte=I2C_Read_Byte(L3G4200D_ADDR, OUT_Z_H);
        i16_realdata=(ui8highbyte<<8)|ui8lowbyte;
    }
    return i16_realdata;
}

float       L3G4200D_Cal_Offset(uint16_t ui16Sample_time)
{
    float fl_offset;
    uint32_t sumval=0;
    uint16_t ui16_count=0;
    for(ui16_count=0;ui16_count<ui16Sample_time;ui16_count++)
    {
        sumval+=L3G4200D_Get_RawValue();
        delay_us(200);
    }
    fl_offset=sumval/ui16Sample_time;
    return fl_offset;
}
