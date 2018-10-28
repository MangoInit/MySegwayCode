#include "control.h"

uint16_t JY901_Temperature;											//陀螺仪温度
float Angle_Balance,Gyro_Balance,Gyro_Turn; 		//平衡倾角 平衡陀螺仪 转向陀螺仪
int16_t Left_Speed, Right_Speed;								//左右电机速度
int16_t Moto1, Moto2;														//电机PWM变量
int16_t Balance_PWM, Velocity_PWM, Turn_PWM;		//平衡环，速度还，转向环的PWM
float ZHONGZHI;																	//机械中值
uint8_t Danger_Flag=0;													//停止PWM输出运行标志
uint8_t Stop_Flag = 1, Qian_Flag,Hou_Flag,Left_Flag,Right_Flag;//蓝牙控制变量
uint8_t Manned_Mode = 1, Bluetooth_Mode = 0;	//载人/蓝牙模式

//PID调节参数变量
float	Balance_PD_P = 50, Balance_PD_D = 0;			//直立环PD参数				300/1
float Velocity_PI_P = 0, Velocity_PI_I = 0;			//速度环PI参数				200/1

/**
  * @brief  平衡车控制
  * @param  None
  * @retval 0
  */
int All_Control()
{
	Left_Speed = -TIM1_CH3_CAPTURE_VAL;																//考虑时间基准，优先读取速度值
	Right_Speed= TIM2_CH3_CAPTURE_VAL;																//电机相对旋转180°，一个取反，保持输出极性相同
	
	Get_JY901_UART();																									//更新陀螺仪姿态
	
	Balance_PWM  = Balance_Control(Angle_Balance, Gyro_Balance);			//===平衡PID控制	
//	Velocity_PWM = Velocity_Control(Left_Speed, Right_Speed);					//===速度环PID控制	 (速度反馈是正反馈，就是小车快的时候要慢下来就需要再跑快一点)
//	if(Manned_Mode == 1)
//		Turn_PWM = Turn_Control_Manned_Mode(Gyro_Turn);									//===转向环PID控制
//	else if(Bluetooth_Mode == 1)
//		Turn_PWM = Turn_Control_Bluetooth_Mode(Left_Speed, Right_Speed, Gyro_Turn);
//	
	Moto1 = Balance_PWM - Velocity_PWM + Turn_PWM;										//===计算左轮电机最终PWM
	Moto2 = Balance_PWM - Velocity_PWM + Turn_PWM;										//===计算右轮电机最终PWM

	
	Xianfu_PWM(); 																										//PWM限幅
	if(Turn_Off(Angle_Balance-ZHONGZHI)==0)														//若无异常，赋值PWM
		Set_PWM(Moto1,Moto2);
	return 0;
}

/**
  * @brief  直立PD控制
  * @param  角度、角速度
  * @retval 直立控制PWM
  */
int Balance_Control(float Angle,float Gyro)
{  
	float Bias;
	int Balance_PWM;
	Bias = Angle - ZHONGZHI;			//===求出平衡的角度中值 和机械相关
	Balance_PWM = Balance_PD_P*Bias + Gyro*Balance_PD_D;	//===计算平衡控制的电机PWM  PD控制   kp是P系数 kd是D系数 
	return Balance_PWM;
}

/**
  * @brief  速度PI控制 修改前进后退速度，请修Target_Velocity，比如，改成60就比较慢了
  * @param  左轮编码器、右轮编码器
  * @retval 速度控制PWM
  */
int Velocity_Control(int Left_Speed, int Right_Speed)
{  
	static float Velocity,Speed_Least,Speed,Movement;
	static float Speed_Integral, Target_Velocity=90;
	//=============遥控前进后退部分=======================//                
	if(1 == Qian_Flag)
		Movement = Target_Velocity;	         //前进标志位置1 
	else if(1 == Hou_Flag)
		Movement = -Target_Velocity;         //后退标志位置1
	else
		Movement=0;	
	//===============速度PI控制器======================//	
	Speed_Least =(Left_Speed + Right_Speed)-0;				//===获取最新速度偏差==测量速度（左右编码器之和）-目标速度（此处为零） 
	Speed *= 0.8f;																		//===一阶低通滤波器       
	Speed += Speed_Least*0.2f;												//===一阶低通滤波器    
	Speed_Integral +=Speed;														//===积分出位移 积分时间：10ms
	Speed_Integral=Speed_Integral-Movement;						//===接收遥控器数据，控制前进后退
	if(Speed_Integral>10000)
		Speed_Integral=10000;														//===积分限幅***
	if(Speed_Integral<-10000)
		Speed_Integral=-10000;													//===积分限幅	
	
	Velocity = Speed*Velocity_PI_P + Speed_Integral*Velocity_PI_I;					//===速度PI控制	
	
	if(Turn_Off(Angle_Balance) == 1 || Stop_Flag == 1)//===电机关闭后清除积分
		Speed_Integral=0;
	return Velocity;
}

/**
  * @brief  转向控制 可修改Kp转向速度（PD控制，载人模式）
  * @param  Z轴角加速度
  * @retval 转向控制PWM
  */
int Turn_Control_Manned_Mode(float Gyro_Turn)
{
	int Turn_Amplitude = 100, Kp = 42 ,Kd = 0;
	int Turn;
	Turn = Get_Turn_ADC();
	if(Turn>Turn_Amplitude)  Turn=Turn_Amplitude;    //===转向速度限幅
	if(Turn<-Turn_Amplitude) Turn=-Turn_Amplitude;
	Turn = Turn*Kp + Gyro_Turn*Kd;
	return Turn;
}

/**
  * @brief  转向控制 可修改Kp转向速度（PD控制，蓝牙控制模式）
  * @param  左轮速度，右轮速度，Z轴角加速度
  * @retval 转向控制PWM
  */
int Turn_Control_Bluetooth_Mode(int Left_Speed, int Right_Speed, float gyro)
{
	int Turn;
	
	return Turn;
}

/**
  * @brief  绝对值函数
  * @param  int
  * @retval unsigned int
  */
int Myabs(int a)
{ 		   
	int temp;
	if(a<0)
		temp = -a;  
	else
		temp = a;
	return temp;
}

/**
  * @brief  限制PWM赋值 
  * @param  None
  * @retval None
  */
void Xianfu_PWM(void)
{	
	int Amplitude = 2000;    										//===PWM满幅是5400 限制在0-3000
	if(Moto1<-Amplitude) Moto1=-Amplitude;
	if(Moto1>Amplitude)  Moto1=Amplitude;
	if(Moto2<-Amplitude) Moto2=-Amplitude;
	if(Moto2>Amplitude)  Moto2=Amplitude;
}

/**
  * @brief  异常关闭电机
  * @param  倾角(和电压)
  * @retval 1：异常  0：正常
  */
uint8_t Turn_Off(float angle)
{
	uint8_t temp;
	if(angle<-10 || angle>10 || Stop_Flag == 1)	//===倾角大于10度关闭电机,可自行增加主板温度过高时关闭电机
	{
		temp=1;																		//===Flag_Stop置1关闭电机
		User_PWM1_SetValue(0);
		User_PWM2_SetValue(0);
		Stop_Flag = 1;
	}
	else
		temp=0;
	return temp;		
}

/**
  * @brief  赋值给PWM寄存器
  * @param  左轮PWM、右轮PWM
  * @retval None
  */
void Set_PWM(int32_t moto1, int32_t moto2)
{
	if(Danger_Flag == 0)
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
		User_PWM2_SetValue(Myabs(moto2));
	}
	else																							//关闭电机输出
	{
		User_PWM1_SetValue(0);
		User_PWM2_SetValue(0);
	}
}

