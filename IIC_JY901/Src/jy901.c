#include "jy901.h"
#include "string.h"

unsigned char str[256];
unsigned char temp[30];
float a[3],w[3],angle[3];
static uint32_t t;
HAL_StatusTypeDef status = HAL_OK;

//short型转化为char型
void ShortToChar(short sData,unsigned char cData[])
{
	cData[0] = sData&0xff;
	cData[1] = sData>>8;
}

//char型转化为short型
short CharToShort(unsigned char cData[])
{
	return ((short)cData[1]<<8) | cData[0];
}

//用iic读取jy901模块的值
void Read_JY901(void)
{

	
	status = HAL_I2C_Mem_Read(&hi2c1, 0XA0, AX, I2C_MEMADD_SIZE_8BIT, &temp[0], 24, 100);			//原地址为0X50
	
	if(status != HAL_OK)																																				//检查采集状态
	{
		HAL_I2C_DeInit(&hi2c1);
		MX_I2C1_Init();
		HAL_I2C_Init(&hi2c1);
		printf("iic读取错误\r\n");
	}
	
	a[0] = (float)CharToShort(&temp[0])/32768*16;					//加速度
	a[1] = (float)CharToShort(&temp[2])/32768*16;
	a[2] = (float)CharToShort(&temp[4])/32768*16;
	w[0] = (float)CharToShort(&temp[6])/32768*2000;				//角速度
	w[1] = (float)CharToShort(&temp[8])/32768*2000;
	w[2] = (float)CharToShort(&temp[10])/32768*2000;
	angle[0] = (float)CharToShort(&temp[18])/32768*180;			//角度
	angle[1] = (float)CharToShort(&temp[20])/32768*180;
	angle[2] = (float)CharToShort(&temp[22])/32768*180;
	
	sprintf((char*)str,"%5d (0x50): Acceleration:%7.3f %7.3f %7.3f	w:%9.3f %9.3f %9.3f		Angle:%8.3f %8.3f %8.3f \r\n",t,a[0],a[1],a[2],w[0],w[1],w[2],angle[0],angle[1],angle[2]);
//	printf("%s",str);
	HAL_UART_Transmit_DMA(&huart1, (uint8_t *)str, strlen((const char *)str));
	t++;
}


