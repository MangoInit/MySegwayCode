#include "jy901.h"
#include "string.h"
#include "usart.h"

/******************************************* IIC���� ****************************************************/

unsigned char temp[24];
float a[3],w[3],angle[3];
uint8_t str[256];
static uint32_t t;
HAL_StatusTypeDef status = HAL_OK;

//short��ת��Ϊchar��
void ShortToChar(short sData,unsigned char cData[])
{
	cData[0] = sData&0xff;
	cData[1] = sData>>8;
}

//char��ת��Ϊshort��
short CharToShort(unsigned char cData[])
{
	return ((short)cData[1]<<8) | cData[0];
}

//��iic��ȡjy901ģ���ֵ
void Read_JY901(void)
{
	status = HAL_I2C_Mem_Read(&hi2c1, 0XA0, AX, I2C_MEMADD_SIZE_8BIT, &temp[0], 24, 100);			//ԭ��ַΪ0X50
	
	if(status != HAL_OK)																																			//���ɼ�״̬
	{
		HAL_I2C_DeInit(&hi2c1);
		MX_I2C1_Init();
		printf("iic��ȡ����\r\n");
	}
	
	a[0] = (float)CharToShort(&temp[0])/32768*16;					//���ٶ�
	a[1] = (float)CharToShort(&temp[2])/32768*16;
	a[2] = (float)CharToShort(&temp[4])/32768*16;
	w[0] = (float)CharToShort(&temp[6])/32768*2000;				//���ٶ�
	w[1] = (float)CharToShort(&temp[8])/32768*2000;
	w[2] = (float)CharToShort(&temp[10])/32768*2000;
	angle[0] = (float)CharToShort(&temp[18])/32768*180;			//�Ƕ�
	angle[1] = (float)CharToShort(&temp[20])/32768*180;
	angle[2] = (float)CharToShort(&temp[22])/32768*180;
	
	sprintf((char*)str,"		%5d (0x50): Acceleration:%7.3f %7.3f %7.3f	w:%9.3f %9.3f %9.3f		Angle:%8.3f %8.3f %8.3f \r\n",t,a[0],a[1],a[2],w[0],w[1],w[2],angle[0],angle[1],angle[2]);
//	printf("%s",str);
	HAL_UART_Transmit_DMA(&huart1, (uint8_t *)str, strlen((const char *)str));
	t++;
}

//��ȡƽ�⳵��Ҫ��������������
void Get_JY901_I2C()
{
	unsigned char temp[30];
	HAL_I2C_Mem_Read(&hi2c1, 0X50<<1, AX, I2C_MEMADD_SIZE_8BIT, &temp[0], 24, 0x10);
	
	Angle_Balance = (float)CharToShort(&temp[20])/32768*180;			//===����ƽ�����Pitch
	Gyro_Balance  = (float)CharToShort(&temp[8])/32768*2000;			//===����ƽ����ٶ�
	Gyro_Turn     = (float)CharToShort(&temp[10])/32768*2000;			//===����ת����ٶ�
//	Acceleration_Z=accel[2];         //===����Z����ٶȼ�
}





/******************************************* UART���� ****************************************************/
		
uint8_t RxBuffer[12];		//����3����JY901����������
uint8_t RxCnt = 0;
uint8_t Rx_Data3;

struct STime		stcTime;
struct SAcc 		stcAcc;
struct SGyro 		stcGyro;
struct SAngle 	stcAngle;
//struct SMag 		stcMag;
//struct SDStatus stcDStatus;
//struct SPress 	stcPress;
//struct SLonLat 	stcLonLat;
//struct SGPSV 		stcGPSV;

//��ȡƽ�⳵��Ҫ��������������
void Get_JY901_UART()
{
	Angle_Balance = (float)stcAngle.Angle[1]/32768*180;					//===����ƽ�����Pitch
	Gyro_Balance  = (float)stcGyro.w[1]/32768*2000;							//===����ƽ����ٶ�
	Gyro_Turn     = (float)stcGyro.w[2]/32768*2000;							//===����ת����ٶ�
	JY901_Temperature = stcAngle.T;															//===�����������¶�
}
//��ȡ��ʼ������ƽ�����Pitch
float Get_JY901_Init_UART()
{
	return (float)stcAngle.Angle[1]/32768*180;
}

//����3�жϻص���ȡJY901����
void JY901_UART3()
{
	RxBuffer[RxCnt] = Rx_Data3;
	HAL_UART_Receive_IT(&huart3, (uint8_t *)&Rx_Data3, 1);				//���´򿪴���3�����ж�
	RxCnt++;
	if(RxBuffer[0]!=0x55)							//Ѱ������ͷ0x55
	{
		RxCnt = 0;
		return;
	}
	if(RxCnt<11)											//11������
		return;
	else
	{
		switch(RxBuffer[1])							//
		{
			case 0x50:	memcpy(&stcTime,   &RxBuffer[2], 8);	break;
			case 0x51:	memcpy(&stcAcc,    &RxBuffer[2], 8);	break;
			case 0x52:	memcpy(&stcGyro,   &RxBuffer[2], 8);	break;
			case 0x53:	memcpy(&stcAngle,  &RxBuffer[2], 8);	break;
//			case 0x54:	memcpy(&stcMag,    &RxBuffer[2], 8);	break;
//			case 0x55:	memcpy(&stcDStatus,&RxBuffer[2], 8);	break;
//			case 0x56:	memcpy(&stcPress,  &RxBuffer[2], 8);	break;
//			case 0x57:	memcpy(&stcLonLat, &RxBuffer[2], 8);	break;
//			case 0x58:	memcpy(&stcGPSV,   &RxBuffer[2], 8);	break;
		}
		RxCnt = 0;
	}
}





