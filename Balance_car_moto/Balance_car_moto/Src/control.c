#include "control.h"

uint16_t JY901_Temperature;											//�������¶�
float Angle_Balance,Gyro_Balance,Gyro_Turn; 		//ƽ����� ƽ�������� ת��������
int16_t Left_Speed, Right_Speed;								//���ҵ���ٶ�
int16_t Moto1, Moto2;														//���PWM����
int16_t Balance_PWM, Velocity_PWM, Turn_PWM;		//ƽ�⻷���ٶȻ���ת�򻷵�PWM
float ZHONGZHI;																	//��е��ֵ
uint8_t Danger_Flag=0;													//ֹͣPWM������б�־
uint8_t Stop_Flag = 1, Qian_Flag,Hou_Flag,Left_Flag,Right_Flag;//�������Ʊ���
uint8_t Manned_Mode = 1, Bluetooth_Mode = 0;	//����/����ģʽ

//PID���ڲ�������
float	Balance_PD_P = 50, Balance_PD_D = 0;			//ֱ����PD����				300/1
float Velocity_PI_P = 0, Velocity_PI_I = 0;			//�ٶȻ�PI����				200/1

/**
  * @brief  ƽ�⳵����
  * @param  None
  * @retval 0
  */
int All_Control()
{
	Left_Speed = -TIM1_CH3_CAPTURE_VAL;																//����ʱ���׼�����ȶ�ȡ�ٶ�ֵ
	Right_Speed= TIM2_CH3_CAPTURE_VAL;																//��������ת180�㣬һ��ȡ�����������������ͬ
	
	Get_JY901_UART();																									//������������̬
	
	Balance_PWM  = Balance_Control(Angle_Balance, Gyro_Balance);			//===ƽ��PID����	
//	Velocity_PWM = Velocity_Control(Left_Speed, Right_Speed);					//===�ٶȻ�PID����	 (�ٶȷ�����������������С�����ʱ��Ҫ����������Ҫ���ܿ�һ��)
//	if(Manned_Mode == 1)
//		Turn_PWM = Turn_Control_Manned_Mode(Gyro_Turn);									//===ת��PID����
//	else if(Bluetooth_Mode == 1)
//		Turn_PWM = Turn_Control_Bluetooth_Mode(Left_Speed, Right_Speed, Gyro_Turn);
//	
	Moto1 = Balance_PWM - Velocity_PWM + Turn_PWM;										//===�������ֵ������PWM
	Moto2 = Balance_PWM - Velocity_PWM + Turn_PWM;										//===�������ֵ������PWM

	
	Xianfu_PWM(); 																										//PWM�޷�
	if(Turn_Off(Angle_Balance-ZHONGZHI)==0)														//�����쳣����ֵPWM
		Set_PWM(Moto1,Moto2);
	return 0;
}

/**
  * @brief  ֱ��PD����
  * @param  �Ƕȡ����ٶ�
  * @retval ֱ������PWM
  */
int Balance_Control(float Angle,float Gyro)
{  
	float Bias;
	int Balance_PWM;
	Bias = Angle - ZHONGZHI;			//===���ƽ��ĽǶ���ֵ �ͻ�е���
	Balance_PWM = Balance_PD_P*Bias + Gyro*Balance_PD_D;	//===����ƽ����Ƶĵ��PWM  PD����   kp��Pϵ�� kd��Dϵ�� 
	return Balance_PWM;
}

/**
  * @brief  �ٶ�PI���� �޸�ǰ�������ٶȣ�����Target_Velocity�����磬�ĳ�60�ͱȽ�����
  * @param  ���ֱ����������ֱ�����
  * @retval �ٶȿ���PWM
  */
int Velocity_Control(int Left_Speed, int Right_Speed)
{  
	static float Velocity,Speed_Least,Speed,Movement;
	static float Speed_Integral, Target_Velocity=90;
	//=============ң��ǰ�����˲���=======================//                
	if(1 == Qian_Flag)
		Movement = Target_Velocity;	         //ǰ����־λ��1 
	else if(1 == Hou_Flag)
		Movement = -Target_Velocity;         //���˱�־λ��1
	else
		Movement=0;	
	//===============�ٶ�PI������======================//	
	Speed_Least =(Left_Speed + Right_Speed)-0;				//===��ȡ�����ٶ�ƫ��==�����ٶȣ����ұ�����֮�ͣ�-Ŀ���ٶȣ��˴�Ϊ�㣩 
	Speed *= 0.8f;																		//===һ�׵�ͨ�˲���       
	Speed += Speed_Least*0.2f;												//===һ�׵�ͨ�˲���    
	Speed_Integral +=Speed;														//===���ֳ�λ�� ����ʱ�䣺10ms
	Speed_Integral=Speed_Integral-Movement;						//===����ң�������ݣ�����ǰ������
	if(Speed_Integral>10000)
		Speed_Integral=10000;														//===�����޷�***
	if(Speed_Integral<-10000)
		Speed_Integral=-10000;													//===�����޷�	
	
	Velocity = Speed*Velocity_PI_P + Speed_Integral*Velocity_PI_I;					//===�ٶ�PI����	
	
	if(Turn_Off(Angle_Balance) == 1 || Stop_Flag == 1)//===����رպ��������
		Speed_Integral=0;
	return Velocity;
}

/**
  * @brief  ת����� ���޸�Kpת���ٶȣ�PD���ƣ�����ģʽ��
  * @param  Z��Ǽ��ٶ�
  * @retval ת�����PWM
  */
int Turn_Control_Manned_Mode(float Gyro_Turn)
{
	int Turn_Amplitude = 100, Kp = 42 ,Kd = 0;
	int Turn;
	Turn = Get_Turn_ADC();
	if(Turn>Turn_Amplitude)  Turn=Turn_Amplitude;    //===ת���ٶ��޷�
	if(Turn<-Turn_Amplitude) Turn=-Turn_Amplitude;
	Turn = Turn*Kp + Gyro_Turn*Kd;
	return Turn;
}

/**
  * @brief  ת����� ���޸�Kpת���ٶȣ�PD���ƣ���������ģʽ��
  * @param  �����ٶȣ������ٶȣ�Z��Ǽ��ٶ�
  * @retval ת�����PWM
  */
int Turn_Control_Bluetooth_Mode(int Left_Speed, int Right_Speed, float gyro)
{
	int Turn;
	
	return Turn;
}

/**
  * @brief  ����ֵ����
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
  * @brief  ����PWM��ֵ 
  * @param  None
  * @retval None
  */
void Xianfu_PWM(void)
{	
	int Amplitude = 2000;    										//===PWM������5400 ������0-3000
	if(Moto1<-Amplitude) Moto1=-Amplitude;
	if(Moto1>Amplitude)  Moto1=Amplitude;
	if(Moto2<-Amplitude) Moto2=-Amplitude;
	if(Moto2>Amplitude)  Moto2=Amplitude;
}

/**
  * @brief  �쳣�رյ��
  * @param  ���(�͵�ѹ)
  * @retval 1���쳣  0������
  */
uint8_t Turn_Off(float angle)
{
	uint8_t temp;
	if(angle<-10 || angle>10 || Stop_Flag == 1)	//===��Ǵ���10�ȹرյ��,���������������¶ȹ���ʱ�رյ��
	{
		temp=1;																		//===Flag_Stop��1�رյ��
		User_PWM1_SetValue(0);
		User_PWM2_SetValue(0);
		Stop_Flag = 1;
	}
	else
		temp=0;
	return temp;		
}

/**
  * @brief  ��ֵ��PWM�Ĵ���
  * @param  ����PWM������PWM
  * @retval None
  */
void Set_PWM(int32_t moto1, int32_t moto2)
{
	if(Danger_Flag == 0)
	{
		if(moto1<0)
			Motor_Turn_Left(1);														//������ת
		else
			Motor_Turn_Left(0);														//���ַ�ת
		User_PWM1_SetValue(Myabs(moto1));

		if(moto2<0)
			Motor_Turn_Right(1);													//������ת
		else
			Motor_Turn_Right(0);													//���ַ�ת
		User_PWM2_SetValue(Myabs(moto2));
	}
	else																							//�رյ�����
	{
		User_PWM1_SetValue(0);
		User_PWM2_SetValue(0);
	}
}

