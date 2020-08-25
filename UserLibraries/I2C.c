/*
 * I2C.c
 *
 *  Created on: Jul 27, 2019
 *      Author: Itachi
 */

#include "Userlibs.h"
#include "Delay_Systick.h"
#include "I2C.h"


void        I2C_Init(void)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C1);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_I2C1));
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOA));
    //GPIO Configure for I2C pins
    GPIOPinConfigure(GPIO_PA6_I2C1SCL);
    GPIOPinConfigure(GPIO_PA7_I2C1SDA);
    GPIOPinTypeI2CSCL(I2C_GPIO_PORT, SCL_PIN);
    GPIOPinTypeI2C(I2C_GPIO_PORT, SDA_PIN);                         //Configuration SDA pin for I2C communication
    I2CMasterInitExpClk(I2C_PORT, SysCtlClockGet(), false);         //Use System clock without fast mode (100Kbps)
}

uint8_t     I2C_Read_Byte(uint8_t ui8Device_Addr, uint8_t ui8Reg_Addr)
{
    uint8_t ui8_datarecv=0;
    I2CMasterSlaveAddrSet(I2C_PORT, ui8Device_Addr, false);         //Setup slave address and force Master to transmitter mode
    I2CMasterDataPut(I2C_PORT, ui8Reg_Addr);                        //Put data into Master Data Reg
    I2CMasterControl(I2C_PORT, I2C_MASTER_CMD_SINGLE_SEND);         //Pump the data from Master Data Reg to I2C buses
    while(I2CMasterBusy(I2C_PORT));                                 //Wait for transmission complete
    I2CMasterSlaveAddrSet(I2C_PORT, ui8Device_Addr, true);          //Setup new communication but this time Master have receiver mode
    I2CMasterControl(I2C_PORT, I2C_MASTER_CMD_SINGLE_RECEIVE);      //Receive data from I2C buses
    while(I2CMasterBusy(I2C_PORT));                                 //wait for receiving process complete
    ui8_datarecv=I2CMasterDataGet(I2C_PORT);                        //Read the data
    return ui8_datarecv;
}

void        I2C_Write_Byte(uint8_t ui8Device_Addr, uint8_t ui8Reg_Addr, uint8_t ui8Data)
{
    I2CMasterSlaveAddrSet(I2C_PORT, ui8Device_Addr, false);
    I2CMasterDataPut(I2C_PORT, ui8Reg_Addr);
    I2CMasterControl(I2C_PORT, I2C_MASTER_CMD_BURST_SEND_START);
    while(I2CMasterBusy(I2C_PORT));
    I2CMasterDataPut(I2C_PORT, ui8Data);
    I2CMasterControl(I2C_PORT, I2C_MASTER_CMD_BURST_SEND_FINISH);
    while(I2CMasterBusy(I2C_PORT));
}
