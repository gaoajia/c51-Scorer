#include<reg51.h>
#include<stdio.h>

unsigned char a=0;	
unsigned char b=0;    
unsigned int table[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71};
unsigned int off[]={0xfe,0xfd,0xfb,0xf7,0xef,0xdf,0xbf,0x7f}; 
unsigned char A_bai=0,A_shi=0,A_ge=0,B_bai=0,B_shi=0,B_ge=0;
unsigned char show_exchange_flag=0;	  //显示交换标志位：
unsigned char team_exchange_flag=0;	  //定义队伍标志位：0=A，1=B
unsigned char sec,min,hour;

sbit KEY1=P1^0;	//+1
sbit KEY2=P1^1;	//-1
sbit KEY3=P1^2;	//+3
sbit KEY4=P1^3;	//换场
sbit KEY5=P1^4;	//A\B加分选择
sbit KEY6=P1^5; //计时启动
sbit KEY7=P1^6; //计时暂停
sbit KEY8=P1^7; //计时重置

void delay(int z)
{			 
	int i,j;
	for(i=0;i<z;i++);
		for(j=0;j<120;j++);
}

void Add()
{
	P1=0Xff;//置1
	P2=0x80;
	P0=0x80;
	if(team_exchange_flag==0) //A加分
	{
		if(!KEY1)
		{
			delay(10);
			if(!KEY1)
			{
				while(!KEY1); //按键松开
				a+=1;
			}
		}
		else if(!KEY2)
		{
			delay(10);
			if(!KEY2)
			{
				while(!KEY2);
				a-=1;
			}
		}
		else if(!KEY3)
		{
			delay(10);
			if(!KEY3)
			{
				while(!KEY3);
				a=0;
			}
		}	
	}
	else if(team_exchange_flag==1) //B加分
	{
		if(!KEY1)
		{
			delay(10);
			if(!KEY1)
			{
				while(!KEY1);
				b+=1;
			}
		}
		else if(!KEY2)
		{
			delay(10);
			if(!KEY2)
			{
				while(!KEY2);
				b-=1;
			}
		}
		else if(!KEY3)
		{
			delay(10);
			if(!KEY3)
			{
				while(!KEY3);
				b=0;
			}
		}	
	}
	P2=0x80;
	P0=0x80;
}
void key_scan()
{
   P1=0Xff;
   if(!KEY4) 
   {   	
		delay(1);
		if(!KEY4) 
		{
			while(!KEY4);
			show_exchange_flag+=1;	//取反，即显示内容交换
		}
		if(show_exchange_flag>=2) show_exchange_flag=0;
   }
   
	P1=0Xff;
	if(!KEY5)
	{
		delay(1);
		if(!KEY5)
		{
			while(!KEY5);
			team_exchange_flag+=1;	  //加分选择在A、B之间切换；0表示A队，1表示B队
		}
		if(team_exchange_flag>=2) team_exchange_flag=0;
	}
	
	Add();
}
 
void  decompose()//分解
{
	A_bai=(a/100);
	A_shi=(a%100/10);
	A_ge=(a%10);
 
	B_bai=(b/100);
	B_shi=(b%100/10);
	B_ge=(b%10);
 
}
void display_team()	//加分指示
{
	if(show_exchange_flag==0)
	{
		if(team_exchange_flag==0)
		{
			P2=0xf7;
			P0=table[10];
			delay(1);	
		}
		 if(team_exchange_flag==1)
		{
			P2=0xef;
			P0=table[11];
			delay(1);		
		}
	}
	else if(show_exchange_flag==1)
	{
		if(team_exchange_flag==0)
		{
			P2=0xef; 
			P0=table[10];
			delay(1);	
		}
		 if(team_exchange_flag==1)
		{
			P2=0xf7;
			P0=table[11];
			delay(1);		
		}
	}
}
void display()
{
	decompose();//分解出百位，十位，个位
	display_team();//加分指示
	if(show_exchange_flag==0)  //显示模式1
	{
        /*********A的分数显示*************/
		P2=0xfe; 
		P0=table[A_bai];
		delay(1);
		P2=0xfd; 
		P0=table[A_shi];
		delay(1);
		P2=0xfb;  
		P0=table[A_ge];
		delay(1);
	    /*********B的分数显示********/
		P2=0xdf;	
		P0=table[B_bai];					
		delay(1);
		P2=0xbf;			
		P0=table[B_shi];
		delay(1);
		P2=0x7f;	
		P0=table[B_ge];
		delay(1);
	}
	else if(show_exchange_flag==1)  
	{
	    /**********B的分数显示*****************/
		P2=0xfe;
		P0=table[B_bai];
		delay(1);
		P2=0xfd;
		P0=table[B_shi]; 
		delay(1);
		P2=0xfb;		
		P0=table[B_ge];
		delay(1);
	    /**********A的分数显示*****************/
		P2=0xdf;
		P0=table[A_bai];	
		delay(1);
		P2=0xbf;
		P0=table[A_shi];	
		delay(1);
		P2=0x7f;
		P0=table[A_ge];	
		delay(1);
	}
}

void time()
{

}

void main()
{
	a=0;
	b=0;
	P1=0Xff;//按键输入端口电平置1；检测出低电平即为按下
	P3=0x3f;
	while(1)
	{
		display();
		key_scan(); 
	}
}