#include "control.h"

uint8_t Flag_Target;													//��־
float Angle_Balance,Gyro_Balance,Gyro_Turn; 	//ƽ����� ƽ�������� ת��������
int16_t Left_Speed, Right_Speed;							//���ҵ���ٶ�
int16_t Moto1, Moto2;													//���PWM����
int16_t Balance_PWM, Velocity_PWM, Turn_PWM;	//ƽ�⻷���ٶȻ���ת�򻷵�PWM

/**
  * @brief  ƽ�⳵����
  * @param  None
  * @retval 0
  */
int All_Control()
{
	Flag_Target = !Flag_Target;									//5ms����һ�����������ݣ�10ms����һ��ƽ�⳵
	if(Flag_Target)
	{
		Read_JY901();
		return 0;
	}
	
	Left_Speed = TIM2_CH2_CAPTURE_VAL;
	Right_Speed= TIM5_CH3_CAPTURE_VAL;
	Get_JY901();
	
	Balance_PWM  = Balance_Control(Angle_Balance, Gyro_Balance);			//===ƽ��PID����	
	Velocity_PWM = Velocity_Control(Left_Speed, Right_Speed);					//===�ٶȻ�PID����	 (�ٶȷ�����������������С�����ʱ��Ҫ����������Ҫ���ܿ�һ��)
	Turn_PWM     = Turn_Control();																		//===ת��PID����
	
	Moto1 = Balance_PWM - Velocity_PWM + Turn_PWM;							//===�������ֵ������PWM
	Moto2 = Balance_PWM - Velocity_PWM + Turn_PWM;							//===�������ֵ������PWM
	
	Moto1 = Moto1/15;
	Moto2 = Moto2/15;
	
//	printf("%5d\t%5d\t%5d\r%5d\r\n",Moto1,Moto2,Left_Speed,Right_Speed);
	
	Xianfu_PWM(); 																			//PWM�޷�
	if(Turn_Off(Angle_Balance)==0)											//�����쳣����ֵPWM
		Set_PWM(Moto1,Moto2);
	return 0;

}


/**************************************************************************
�������ܣ�ֱ��PD����
��ڲ������Ƕȡ����ٶ�
����  ֵ��ֱ������PWM
**************************************************************************/
int Balance_Control(float Angle,float Gyro)
{  
	float Bias,kp=300,kd=1;
	int Balance_PWM;
	Bias = Angle - ZHONGZHI;			//===���ƽ��ĽǶ���ֵ �ͻ�е���
	Balance_PWM = kp*Bias + Gyro*kd;	//===����ƽ����Ƶĵ��PWM  PD����   kp��Pϵ�� kd��Dϵ�� 
	return Balance_PWM;
}

/**************************************************************************
�������ܣ��ٶ�PI���� �޸�ǰ�������ٶȣ�����Target_Velocity�����磬�ĳ�60�ͱȽ�����
��ڲ��������ֱ����������ֱ�����
����  ֵ���ٶȿ���PWM
**************************************************************************/
int Velocity_Control(int Left_Speed, int Right_Speed)
{  
	static float Velocity,Speed_Least,Speed,Movement;
	static float Speed_Integral;
	float kp=80,ki=0.4;
	//=============ң��ǰ�����˲���=======================// 
	/******************************************************
	if(Bi_zhang==1&&Flag_sudu==1)  Target_Velocity=45;                 //����������ģʽ,�Զ��������ģʽ
	else 	                         Target_Velocity=90;                 
	if(1==Flag_Qian)    	Movement=Target_Velocity/Flag_sudu;	         //===ǰ����־λ��1 
	else if(1==Flag_Hou)	Movement=-Target_Velocity/Flag_sudu;         //===���˱�־λ��1
	else  Movement=0;	
	if(Bi_zhang==1&&Distance<500&&Flag_Left!=1&&Flag_Right!=1)        //���ϱ�־λ��1�ҷ�ң��ת���ʱ�򣬽������ģʽ
	Movement=-Target_Velocity/Flag_sudu;
	******************************************************/
	//=============�ٶ�PI������=======================//	
	Speed_Least =(Left_Speed + Right_Speed)-0;					//===��ȡ�����ٶ�ƫ��==�����ٶȣ����ұ�����֮�ͣ�-Ŀ���ٶȣ��˴�Ϊ�㣩 
	Speed *= 0.8f;												//===һ�׵�ͨ�˲���       
	Speed += Speed_Least*0.2f;									//===һ�׵�ͨ�˲���    
	Speed_Integral +=Speed;										//===���ֳ�λ�� ����ʱ�䣺10ms
	Speed_Integral=Speed_Integral-Movement;						//===����ң�������ݣ�����ǰ������
	if(Speed_Integral>10000)  	Speed_Integral=10000;			//===�����޷�***
	if(Speed_Integral<-10000)	Speed_Integral=-10000;			//===�����޷�	
	Velocity = Speed*kp + Speed_Integral*ki;					//===�ٶȿ���	
//	if(Turn_Off(Angle_Balance,Voltage)==1||Flag_Stop==1)   Speed_Integral=0;      //===����رպ��������
	return Velocity;
}
/**************************************************************************
�������ܣ�ת����� ���޸�Kpת���ٶȣ�P���ƣ�����ģʽ��
��ڲ�����
����  ֵ��ת�����PWM
***************************************************************************/
int Turn_Control()
{
	int Turn_Amplitude = 100, Kp = 42;
	int Turn;
	Turn = Get_Turn_ADC();
	if(Turn>Turn_Amplitude)  Turn=Turn_Amplitude;    //===ת���ٶ��޷�
	if(Turn<-Turn_Amplitude) Turn=-Turn_Amplitude;
	Turn = Turn*Kp;
	return Turn;
}

/**************************************************************************
�������ܣ�����ֵ����
��ڲ�����int
����  ֵ��unsigned int
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
�������ܣ�����PWM��ֵ 
��ڲ�������
����  ֵ����
**************************************************************************/
void Xianfu_PWM(void)
{	
	int Amplitude = 300;    //===PWM������7200 ������6900(δ����0-499)
	if(Moto1<-Amplitude) Moto1=-Amplitude;
	if(Moto1>Amplitude)  Moto1=Amplitude;
	if(Moto2<-Amplitude) Moto2=-Amplitude;
	if(Moto2>Amplitude)  Moto2=Amplitude;
}

/**************************************************************************
�������ܣ��쳣�رյ��
��ڲ��������(�͵�ѹ)
����  ֵ��1���쳣  0������
**************************************************************************/
uint8_t Turn_Off(float angle)
{
	uint8_t temp;
	if(angle<-10||angle>10)				//===��Ǵ���10�ȹرյ��
	{
		temp=1;							//===Flag_Stop��1�رյ��
										//===���������������¶ȹ���ʱ�رյ��
	}
	else
		temp=0;
	return temp;		
}
	
/**************************************************************************
�������ܣ���ֵ��PWM�Ĵ���
��ڲ���������PWM������PWM
����  ֵ����
**************************************************************************/
void Set_PWM(int32_t moto1, int32_t moto2)
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
	User_PWM1_SetValue(Myabs(moto2));
	
}




