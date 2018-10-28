#include "jy901.h"

//	float a[3],w[3],angle[3];
/*****************short型转化为char型******************/
void ShortToChar(short sData,unsigned char cData[])
{
	cData[0] = sData&0xff;
	cData[1] = sData>>8;
}

/****************char型转化为short型*******************/
short CharToShort(unsigned char cData[])
{
	return ((short)cData[1]<<8) | cData[0];
}

/****************用iic读取jy901模块的值****************/
void jy901(void)
{
	unsigned char temp[30];
	float a[3],w[3],angle[3];
	unsigned char str[100];
	static u32 t;
	
	IICreadBytes(0x50, AX, 24, &temp[0]);
	
	a[0] = (float)CharToShort(&temp[0])/32768*16;				//加速度
	a[1] = (float)CharToShort(&temp[2])/32768*16;
	a[2] = (float)CharToShort(&temp[4])/32768*16;
	w[0] = (float)CharToShort(&temp[6])/32768*2000;				//角速度
	w[1] = (float)CharToShort(&temp[8])/32768*2000;
	w[2] = (float)CharToShort(&temp[10])/32768*2000;
	angle[0] = (float)CharToShort(&temp[18])/32768*180;			//角度
	angle[1] = (float)CharToShort(&temp[20])/32768*180;
	angle[2] = (float)CharToShort(&temp[22])/32768*180;
	
	sprintf((char*)str,"%5d	adress(0x50): Acceleration:%7.3f %7.3f %7.3f		w:%9.3f %9.3f %9.3f		Angle:%8.3f %8.3f %8.3f \r\n",t,a[0],a[1],a[2],w[0],w[1],w[2],angle[0],angle[1],angle[2]);
	printf("%s\r\n",str);
	t++;
}

void Get_JY901()
{
	unsigned char temp[30];
	IICreadBytes(0x50, AX, 24, &temp[0]);
	
	Angle_Balance = (float)CharToShort(&temp[20])/32768*180;			//===更新平衡倾角Pitch
	Gyro_Balance  = (float)CharToShort(&temp[8])/32768*2000;			//===更新平衡角速度
	Gyro_Turn     = (float)CharToShort(&temp[10])/32768*2000;			//===更新转向角速度
//	Acceleration_Z=accel[2];         //===更新Z轴加速度计
}

