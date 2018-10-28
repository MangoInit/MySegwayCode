#include "control.h"
u8 Flag_Target;								//��־
int Left_Speed,Right_Speed;					//���ҵ���ٶ�
u32 Balance_Pwm,Velocity_Pwm,Turn_Pwm;		//��������PWM
s32 Moto1,Moto2;							//���PWM����
float Angle_Balance,Gyro_Balance,Gyro_Turn; //ƽ����� ƽ�������� ת��������

int Control()
{
	Flag_Target=!Flag_Target;										//5ms����һ�����������ݣ�10ms����ƽ�⳵һ��
	if(Flag_Target)
	{
		Get_JY901();												//��������������
		return 0;
	}
	
	Left_Speed = -Read_Speed(2);									//��ȡ�����ٶ�
	Right_Speed = Read_Speed(5);									//��ȡ�ҵ���ٶ�
	Get_JY901();													//��������������

	Balance_Pwm  = Balance_Control(Angle_Balance, Gyro_Balance);			//===ƽ��PID����	
	Velocity_Pwm = Velocity_Control(Left_Speed, Right_Speed);				//===�ٶȻ�PID����	 (�ٶȷ�����������������С�����ʱ��Ҫ����������Ҫ���ܿ�һ��)
	Turn_Pwm     = Turn_Control();											//===ת��PID����
	
	Moto1 = Balance_Pwm - Velocity_Pwm + Turn_Pwm;							//===�������ֵ������PWM
	Moto2 = Balance_Pwm - Velocity_Pwm + Turn_Pwm;							//===�������ֵ������PWM
	
	Moto1 = Moto1/15;
	Moto2 = Moto2/15;
	
//	printf("%5d\t%5d\t%5d\r%5d\r\n",Moto1,Moto2,Left_Speed,Right_Speed);
	
	Xianfu_Pwm(); 															//PWM�޷�
	if(Turn_Off(Angle_Balance)==0)											//�����쳣����ֵPWM
		PWM_Out(Moto1,Moto2);
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
	int Balance_Pwm;
	Bias = Angle - ZHONGZHI;			//===���ƽ��ĽǶ���ֵ �ͻ�е���
	Balance_Pwm = kp*Bias + Gyro*kd;	//===����ƽ����Ƶĵ��PWM  PD����   kp��Pϵ�� kd��Dϵ�� 
	return Balance_Pwm;
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
	Turn = Get_Steer();
	if(Turn>Turn_Amplitude)  Turn=Turn_Amplitude;    //===ת���ٶ��޷�
	if(Turn<-Turn_Amplitude) Turn=-Turn_Amplitude;
	Turn = Turn*Kp;
	return Turn;
}

/**************************************************************************
�������ܣ�ת�����  �޸�ת���ٶȣ����޸�Turn_Amplitude���ɣ�PD���ƣ�������������ģʽ�Ƚ��ʺϣ�
��ڲ��������ֱ����������ֱ�������Z��������
����  ֵ��ת�����PWM
**************************************************************************/
/*
int Turn_Control(int Left_Speed, int Right_Speed, float gyro)//ת�����
{
	static float Turn_Target,Turn,Speed_temp,Turn_Convert=0.9,Turn_Count;
	float Turn_Amplitude=88, Kp=42, Kd=0;     
	//=============ң��������ת����=======================//
	if(Steer_Value)                      //��һ������Ҫ�Ǹ�����תǰ���ٶȵ����ٶȵ���ʼ�ٶȣ�����С������Ӧ��
	{
		if(++Turn_Count==1)
			Speed_temp=Myabs(Left_Speed + Right_Speed);
		Turn_Convert=50/Speed_temp;
		if(Turn_Convert<0.6)Turn_Convert=0.6;
		if(Turn_Convert>3)Turn_Convert=3;
	}
	else
	{
		Turn_Convert=0.9;
		Turn_Count=0;
		Speed_temp=0;
	}			
	if(1==Flag_Left)				Turn_Target-=Turn_Convert;
	else if(1==Flag_Right)			Turn_Target+=Turn_Convert; 
	else Turn_Target=0;

	if(Turn_Target>Turn_Amplitude)  Turn_Target=Turn_Amplitude;    //===ת���ٶ��޷�
	if(Turn_Target<-Turn_Amplitude) Turn_Target=-Turn_Amplitude;
//	if(Flag_Qian==1||Flag_Hou==1)  Kd=0.5;        //��������
	else Kd=0;   //ת���ʱ��ȡ�������ǵľ��� �е�ģ��PID��˼��
	//=============ת��PD������=======================//
	Turn=-Turn_Target*Kp -gyro*Kd;					//===���Z�������ǽ���PD����
	return Turn;
}
*/

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
void Xianfu_Pwm(void)
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
u8 Turn_Off(float angle)
{
	u8 temp;
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
void Set_PWM(s32 moto1, s32 moto2)
{
	if(moto1<0)
		ZF1 = 1;
	else
		ZF1 = 0;
//	PWMA=Myabs(moto1);
	
	if(moto2<0)
		ZF2 = 1;
	else
		ZF2 = 0;
//	PWMB=Myabs(moto2);
	
	PWM_Out(Myabs(moto1), Myabs(moto2));
}
