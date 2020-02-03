#include "MyTMP117.h"

uint8_t MyTMP117_DeviceID=MyTMP117_GND;

void MyTMP117_Init(I2C_HandleTypeDef i2c) //this function will initialize the sensor using custom parameters
{
      MyTMP117_set_Config(i2c,0x02,0x20);
      MyTMP117_set_Temp_Offset(i2c,0x00,0x00); 			
      MyTMP117_set_LowLimit(i2c,0x28,0x16);           //Set 22 Celcius
      MyTMP117_set_HighLimit(i2c,0x76,0x80);          //Set 60 Celcius
}

void MyTMP117_set_Averaging(I2C_HandleTypeDef i2c,TMP117_AVE ave) //set the high and low limit register for alert
{
	 uint16_t reg_value = MyTMP117_get_Config (&i2c);
	 reg_value &= ~((1UL << 6) | (1UL << 5) );      	 	//clear bits
     reg_value = reg_value |( ( ave & 0x03 ) << 5);          //set bits
     uint8_t first = (reg_value&0x0F); 
	 uint8_t second =reg_value>>8;
	 MyTMP117_set_Config(&i2c,first,second);
}

void MyTMP117_set_Alert_Temp(I2C_HandleTypeDef i2c,uint8_t Lowfirst,uint8_t LowSecond,uint8_t Highfirst,uint8_t HighSecond)
{
	static uint8_t bufLow[3],bufHigh[3];
      bufLow[0]=MyTMP117_TempLowLimit;
      bufLow[1]=Lowfirst;     
      bufLow[2]=LowSecond;    
      HAL_I2C_Master_Transmit(&i2c,MyTMP117_DeviceID,bufLow,2,HAL_MAX_DELAY);
      HAL_Delay(1);
	  
      bufHigh[0]=MyTMP117_TempHighLimit;
      bufHigh[1]=Highfirst;     
      bufHigh[2]=HighSecond;   
      HAL_I2C_Master_Transmit(&i2c,MyTMP117_DeviceID,bufHigh,2,HAL_MAX_DELAY);
      HAL_Delay(1);
}

float MyTMP117_get_Temp(I2C_HandleTypeDef i2c) //This function will return the temp in float.
{
      static uint8_t buf[3];
      buf[0]=MyTMP117_TempReg;
      HAL_I2C_Master_Transmit(&i2c,MyTMP117_DeviceID,buf,0,HAL_MAX_DELAY); 
      HAL_Delay(1);
      HAL_I2C_Master_Receive(&i2c,MyTMP117_DeviceID,buf,2,HAL_MAX_DELAY);
      float Temp= ((buf[0]<<8)|buf[1])*0.0078125;
      return Temp;
}

uint16_t MyTMP117_get_Config(I2C_HandleTypeDef i2c) //this function will return the configuration register value
{
      static uint8_t buf[3];
      buf[0]=MyTMP117_ConfigReg;
      HAL_I2C_Master_Transmit(&i2c,MyTMP117_DeviceID,buf,0,HAL_MAX_DELAY);
      HAL_Delay(1);
      HAL_I2C_Master_Receive(&i2c,MyTMP117_DeviceID,buf,2,HAL_MAX_DELAY);
	  uint16_t Config= ((buf[0]<<8)|buf[1]);
      return Config;
}

void MyTMP117_set_Config(I2C_HandleTypeDef i2c,uint8_t first,uint8_t second) //this function will set the configuration register
{
      static uint8_t buf[3];
      buf[0]=MyTMP117_ConfigReg;
      buf[1]=first;     
      buf[2]=second;    
      HAL_I2C_Master_Transmit(&i2c,MyTMP117_DeviceID,buf,2,HAL_MAX_DELAY);
      HAL_Delay(1);
}

void MyTMP117_set_HighLimit(I2C_HandleTypeDef i2c,uint8_t first,uint8_t second) //this function will set the the high limit for alert
{
      static uint8_t buf[3];
      buf[0]=MyTMP117_TempHighLimit;
      buf[1]=first;     
      buf[2]=second;    
      HAL_I2C_Master_Transmit(&i2c,MyTMP117_DeviceID,buf,2,HAL_MAX_DELAY);
      HAL_Delay(1);
}

uint16_t MyTMP117_get_HighLimit(I2C_HandleTypeDef i2c) //this function will return the highlimit register value
{
      static uint8_t buf[3];
      buf[0]=MyTMP117_TempHighLimit;   
      HAL_I2C_Master_Transmit(&i2c,MyTMP117_DeviceID,buf,0,HAL_MAX_DELAY);
      HAL_Delay(1);
      HAL_I2C_Master_Receive(&i2c,MyTMP117_DeviceID,buf,2,HAL_MAX_DELAY);
			uint16_t HighLimit= ((buf[0]<<8)|buf[1]);
      return HighLimit;    
}

void MyTMP117_set_LowLimit(I2C_HandleTypeDef i2c,uint8_t first,uint8_t second)//this function will set the low limit for alert
{
      static uint8_t buf[3];
      buf[0]=MyTMP117_TempLowLimit;
      buf[1]=first;     
      buf[2]=second;    
      HAL_I2C_Master_Transmit(&i2c,MyTMP117_DeviceID,buf,2,HAL_MAX_DELAY);
      HAL_Delay(1);    
}

uint16_t MyTMP117_get_LowLimit(I2C_HandleTypeDef i2c) //this function will return the lowlimit register value
{
      static uint8_t buf[3];
      buf[0]=MyTMP117_TempLowLimit;
      HAL_I2C_Master_Transmit(&i2c,MyTMP117_DeviceID,buf,0,HAL_MAX_DELAY);
      HAL_Delay(1);
      HAL_I2C_Master_Receive(&i2c,MyTMP117_DeviceID,buf,2,HAL_MAX_DELAY);
      return ((buf[0]<<8)|buf[1]);    
}

uint16_t MyTMP117_get_EEPROM_Unlock(I2C_HandleTypeDef i2c) //this will return the EEPROM unlock register value
{
      static uint8_t buf[3];
      buf[0]=MyTMP117_EEPROM_Unlock;
      HAL_I2C_Master_Transmit(&i2c,MyTMP117_DeviceID,buf,0,HAL_MAX_DELAY);
      HAL_Delay(1);
      HAL_I2C_Master_Receive(&i2c,MyTMP117_DeviceID,buf,2,HAL_MAX_DELAY);
			uint16_t Unlock= ((buf[0]<<8)|buf[1])
      return Unlock;     
}

void MyTMP117_set_EEPROM_Unlock(I2C_HandleTypeDef i2c,uint8_t first,uint8_t second) //by this function we will set the EEPROM register value for read and write
{
      static uint8_t buf[3];
      buf[0]=MyTMP117_EEPROM_Unlock;
      buf[1]=first;     
      buf[2]=second;    
      HAL_I2C_Master_Transmit(&i2c,MyTMP117_DeviceID,buf,2,HAL_MAX_DELAY);
      HAL_Delay(1);
}

void MyTMP117_set_EEPROM1(I2C_HandleTypeDef i2c,uint8_t first,uint8_t second) //this function will set EEPROM1 value
{
      static uint8_t buf[3];
      buf[0]=MyTMP117_EEPROM1;
      buf[1]=first;     
      buf[2]=second;    
      HAL_I2C_Master_Transmit(&i2c,MyTMP117_DeviceID,buf,2,HAL_MAX_DELAY);
      HAL_Delay(1); 
}

uint16_t MyTMP117_get_EEPROM1(I2C_HandleTypeDef i2c) //by this function we wiil get EEPROM1 value
{
      static uint8_t buf[3];
      buf[0]=MyTMP117_EEPROM1;
      HAL_I2C_Master_Transmit(&i2c,MyTMP117_DeviceID,buf,0,HAL_MAX_DELAY);
      HAL_Delay(1);
      HAL_I2C_Master_Receive(&i2c,MyTMP117_DeviceID,buf,2,HAL_MAX_DELAY);
			uint16_t EEPROM1= ((buf[0]<<8)|buf[1]);
      return EEPROM1;     
}

void MyTMP117_set_EEPROM2(I2C_HandleTypeDef i2c,uint8_t first,uint8_t second) //this function will set EEPROM2 value
{
      static uint8_t buf[3];
      buf[0]=MyTMP117_EEPROM2;
      buf[1]=first;     
      buf[2]=second;    
      HAL_I2C_Master_Transmit(&i2c,MyTMP117_DeviceID,buf,2,HAL_MAX_DELAY);
      HAL_Delay(1);     
}

uint16_t MyTMP117_get_EEPROM2(I2C_HandleTypeDef i2c) //by this function we wiil get EEPROM2 value
{
      static uint8_t buf[3];
      buf[0]=MyTMP117_EEPROM2;
      HAL_I2C_Master_Transmit(&i2c,MyTMP117_DeviceID,buf,0,HAL_MAX_DELAY);
      HAL_Delay(1);
      HAL_I2C_Master_Receive(&i2c,MyTMP117_DeviceID,buf,2,HAL_MAX_DELAY);
			uint16_t EEPROM2= ((buf[0]<<8)|buf[1]);
      return EEPROM2;
}

void MyTMP117_set_EEPROM3(I2C_HandleTypeDef i2c,uint8_t first,uint8_t second) //this function will set EEPROM3 value
{
      static uint8_t buf[3];
      buf[0]=MyTMP117_EEPROM3;
      buf[1]=first;     
      buf[2]=second;    
      HAL_I2C_Master_Transmit(&i2c,MyTMP117_DeviceID,buf,2,HAL_MAX_DELAY);
      HAL_Delay(1);
}

uint16_t MyTMP117_get_EEPROM3(I2C_HandleTypeDef i2c) //by this function we wiil get EEPROM3 value
{
      static uint8_t buf[3];
      buf[0]=MyTMP117_EEPROM3;   
      HAL_I2C_Master_Transmit(&i2c,MyTMP117_DeviceID,buf,0,HAL_MAX_DELAY);
      HAL_Delay(1);
      HAL_I2C_Master_Receive(&i2c,MyTMP117_DeviceID,buf,2,HAL_MAX_DELAY);
			uint16_t EEPROM3= ((buf[0]<<8)|buf[1]);
      return EEPROM3;     
}

void MyTMP117_set_Temp_Offset(I2C_HandleTypeDef i2c,uint8_t first,uint8_t second) //this function will set the temperature offset value
{
      static uint8_t buf[3];
      buf[0]=MyTMP117_Temp_Offset;
      buf[1]=first;     
      buf[2]=second;    
      HAL_I2C_Master_Transmit(&i2c,MyTMP117_DeviceID,buf,2,HAL_MAX_DELAY);
      HAL_Delay(1);
}

uint16_t MyTMP117_get_Temp_Offset(I2C_HandleTypeDef i2c) //by this function we will get the temprature offset value
{
      static uint8_t buf[3];
      buf[0]=MyTMP117_Temp_Offset;
      HAL_I2C_Master_Transmit(&i2c,MyTMP117_DeviceID,buf,0,HAL_MAX_DELAY);
      HAL_Delay(1);
      HAL_I2C_Master_Receive(&i2c,MyTMP117_DeviceID,buf,2,HAL_MAX_DELAY);
			uint16_t TempOffset= ((buf[0]<<8)|buf[1]);
      return TempOffset;
}
uint16_t MyTMP117_get_ID_Reg(I2C_HandleTypeDef i2c) //This function will return the ID register
{
      static uint8_t buf[3];
      buf[0]=MyTMP117_ID_Reg;
      HAL_I2C_Master_Transmit(&i2c,MyTMP117_DeviceID,buf,0,HAL_MAX_DELAY);
      HAL_Delay(1);
      HAL_I2C_Master_Receive(&i2c,MyTMP117_DeviceID,buf,2,HAL_MAX_DELAY);
      uint16_t ID= ((buf[0]<<8)|buf[1]);
      return ID;
}
