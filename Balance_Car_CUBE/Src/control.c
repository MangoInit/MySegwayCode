#include "control.h"

uint8_t Flag_Target;													//标志
float Angle_Balance,Gyro_Balance,Gyro_Turn; 	//平衡倾角 平衡陀螺仪 转向陀螺仪
int16_t Left_Speed, Right_Speed;							//左右电机速度
int16_t Moto1, Moto2;													//电机PWM变量
int16_t Balance_PWM, Velocity_PWM, Turn_PWM;	//平衡环，速度还，转向环的PWM

/**
  * @brief  平衡车控制
  * @param  None
  * @retval 0
  */
int All_Control()
{
	Flag_Target = !Flag_Target;									//5ms更新一次陀螺仪数据，10ms控制一次平衡车
	if(Flag_Target)
	{
		Read_JY901();
		return 0;
	}
	
	Left_Speed = TIM2_CH2_CAPTURE_VAL;
	Right_Speed= TIM5_CH3_CAPTURE_VAL;
	Get_JY901();
	
	Balance_PWM  = Balance_Control(Angle_Balance, Gyro_Balance);			//===平衡PID控制	
	Velocity_PWM = Velocity_Control(Left_Speed, Right_Speed);					//===速度环PID控制	 (速度反馈是正反馈，就是小车快的时候要慢下来就需要再跑快一点)
	Turn_PWM     = Turn_Control();																		//===转向环PID控制
	
	Moto1 = Balance_PWM - Velocity_PWM + Turn_PWM;							//===计算左轮电机最终PWM
	Moto2 = Balance_PWM - Velocity_PWM + Turn_PWM;							//===计算右轮电机最终PWM
	
	Moto1 = Moto1/15;
	Moto2 = Moto2/15;
	
//	printf("%5d\t%5d\t%5d\r%5d\r\n",Moto1,Moto2,Left_Speed,Right_Speed);
	
	Xianfu_PWM(); 																			//PWM限幅
	if(Turn_Off(Angle_Balance)==0)											//若无异常，赋值PWM
		Set_PWM(Moto1,Moto2);
	return 0;

}


/**************************************************************************
函数功能：直立PD控制
入口参数：角度、角速度
返回  值：直立控制PWM
**************************************************************************/
int Balance_Control(float Angle,float Gyro)
{  
	float Bias,kp=300,kd=1;
	int Balance_PWM;
	Bias = Angle - ZHONGZHI;			//===求出平衡的角度中值 和机械相关
	Balance_PWM = kp*Bias + Gyro*kd;	//===计算平衡控制的电机PWM  PD控制   kp是P系数 kd是D系数 
	return Balance_PWM;
}

/**************************************************************************
函数功能：速度PI控制 修改前进后退速度，请修Target_Velocity，比如，改成60就比较慢了
入口参数：左轮编码器、右轮编码器
返回  值：速度控制PWM
**************************************************************************/
int Velocity_Control(int Left_Speed, int Right_Speed)
{  
	static float Velocity,Speed_Least,Speed,Movement;
	static float Speed_Integral;
	float kp=80,ki=0.4;
	//=============遥控前进后退部分=======================// 
	/******************************************************
	if(Bi_zhang==1&&Flag_sudu==1)  Target_Velocity=45;                 //如果进入避障模式,自动进入低速模式
	else 	                         Target_Velocity=90;                 
	if(1==Flag_Qian)    	Movement=Target_Velocity/Flag_sudu;	         //===前进标志位置1 
	else if(1==Flag_Hou)	Movement=-Target_Velocity/Flag_sudu;         //===后退标志位置1
	else  Movement=0;	
	if(Bi_zhang==1&&Distance<500&&Flag_Left!=1&&Flag_Right!=1)        //避障标志位置1且非遥控转弯的时候，进入避障模式
	Movement=-Target_Velocity/Flag_sudu;
	******************************************************/
	//=============速度PI控制器=======================//	
	Speed_Least =(Left_Speed + Right_Speed)-0;					//===获取最新速度偏差==测量速度（左右编码器之和）-目标速度（此处为零） 
	Speed *= 0.8f;												//===一阶低通滤波器       
	Speed += Speed_Least*0.2f;									//===一阶低通滤波器    
	Speed_Integral +=Speed;										//===积分出位移 积分时间：10ms
	Speed_Integral=Speed_Integral-Movement;						//===接收遥控器数据，控制前进后退
	if(Speed_Integral>10000)  	Speed_Integral=10000;			//===积分限幅***
	if(Speed_Integral<-10000)	Speed_Integral=-10000;			//===积分限幅	
	Velocity = Speed*kp + Speed_Integral*ki;					//===速度控制	
//	if(Turn_Off(Angle_Balance,Voltage)==1||Flag_Stop==1)   Speed_Integral=0;      //===电机关闭后清除积分
	return Velocity;
}
/**************************************************************************
函数功能：转向控制 可修改Kp转向速度（P控制，载人模式）
入口参数：
返回  值：转向控制PWM
***************************************************************************/
int Turn_Control()
{
	int Turn_Amplitude = 100, Kp = 42;
	int Turn;
	Turn = Get_Turn_ADC();
	if(Turn>Turn_Amplitude)  Turn=Turn_Amplitude;    //===转向速度限幅
	if(Turn<-Turn_Amplitude) Turn=-Turn_Amplitude;
	Turn = Turn*Kp;
	return Turn;
}

/**************************************************************************
函数功能：绝对值函数
入口参数：int
返回  值：unsigned int
**************************************************************************/
int Myabs(int a)
{ 		   
	int temp;
	if(a<0)
		temp=-a;  
	else temp=a;
	return temp;
}

/**************************************************************************
函数功能：限制PWM赋值 
入口参数：无
返回  值：无
**************************************************************************/
void Xianfu_PWM(void)
{	
	int Amplitude = 300;    //===PWM满幅是7200 限制在6900(未测试0-499)
	if(Moto1<-Amplitude) Moto1=-Amplitude;
	if(Moto1>Amplitude)  Moto1=Amplitude;
	if(Moto2<-Amplitude) Moto2=-Amplitude;
	if(Moto2>Amplitude)  Moto2=Amplitude;
}

/**************************************************************************
函数功能：异常关闭电机
入口参数：倾角(和电压)
返回  值：1：异常  0：正常
**************************************************************************/
uint8_t Turn_Off(float angle)
{
	uint8_t temp;
	if(angle<-10||angle>10)				//===倾角大于10度关闭电机
	{
		temp=1;							//===Flag_Stop置1关闭电机
										//===可自行增加主板温度过高时关闭电机
	}
	else
		temp=0;
	return temp;		
}
	
/**************************************************************************
函数功能：赋值给PWM寄存器
入口参数：左轮PWM、右轮PWM
返回  值：无
**************************************************************************/
void Set_PWM(int32_t moto1, int32_t moto2)
{
	if(moto1<0)
		Motor_Turn_Left(1);														//左轮正转
	else
		Motor_Turn_Left(0);														//左轮反转
	User_PWM1_SetValue(Myabs(moto1));
	
	if(moto2<0)
		Motor_Turn_Right(1);													//右轮正转
	else
		Motor_Turn_Right(0);													//右轮反转
	User_PWM1_SetValue(Myabs(moto2));
	
}




