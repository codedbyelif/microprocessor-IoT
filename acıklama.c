#include <stdio.h>
#include "NuMicro.h"

// ileri adinda geri dönüs degeri olmayan parametre olarak int tipinde bir degisken alan void türünden fonksiyon olusturuluyor
void ileri(int a)                               
{
	PH6=1;					// Dc motor yön kontrolü için IN1 aktif ediliyor
	PH7=0;					// Dc motor yön kontrolü için IN2 pasif ediliyor
	// Birinci Parametre PWM numarasi, Ikinci Parametre PWM kanal numarasi, Üçüncü Parametre Frekans, Dördüncü Parametre PWM Orani
	BPWM_ConfigOutputChannel(BPWM1, 0, 5000, a); 
}

// geri adinda geri dönüs degeri olmayan parametre olarak int tipinde bir degisken alan void türünden fonksiyon olusturuluyor
void geri(int a)                  
{
	PH6=0;     	// Dc motor yön kontrolü için IN1 pasif ediliyor
	PH7=1;			// Dc motor yön kontrolü için IN2 aktif ediliyor
	// Birinci Parametre PWM numarasi, Ikinci Parametre PWM kanal numarasi, Üçüncü Parametre Frekans, Dördüncü Parametre PWM Orani
	BPWM_ConfigOutputChannel(BPWM1, 0, 5000, a);  
}
void dur()           // dur adinda geri dönüs degeri olmayan void türünden fonksiyon olusturuluyor
{ 
	PH6=0;            // Dc motor yön kontrolü için IN1 pasif ediliyor
	PH7=0;            // Dc motor yön kontrolü için IN2 pasif ediliyor
}




void SYS_Init(void)
{
    /---------------------------------------------------------------------------------------------------------/
    /* Init System Clock                                                                                       */
    /---------------------------------------------------------------------------------------------------------/
    /* Enable HIRC clock */
    CLK_EnableXtalRC(CLK_PWRCTL_HIRCEN_Msk);

    /* Waiting for HIRC clock ready */
    CLK_WaitClockReady(CLK_STATUS_HIRCSTB_Msk);

    /* Switch HCLK clock source to HIRC */
    CLK_SetHCLK(CLK_CLKSEL0_HCLKSEL_HIRC, CLK_CLKDIV0_HCLK(1));

    /* Enable PLL */
    CLK->PLLCTL = CLK_PLLCTL_128MHz_HIRC;

    /* Waiting for PLL stable */
    CLK_WaitClockReady(CLK_STATUS_PLLSTB_Msk);

    /* Select HCLK clock source as PLL and HCLK source divider as 2 */
    CLK_SetHCLK(CLK_CLKSEL0_HCLKSEL_PLL, CLK_CLKDIV0_HCLK(2));

    /* Set SysTick source to HCLK/2*/
    CLK_SetSysTickClockSrc(CLK_CLKSEL0_STCLKSEL_HCLK_DIV2);

    /* Enable UART module clock */
    CLK_EnableModuleClock(UART0_MODULE);
    CLK_SetModuleClock(UART0_MODULE, CLK_CLKSEL1_UART0SEL_HIRC, CLK_CLKDIV0_UART0(1));

    /* Enable TIMER module clock */
    CLK_EnableModuleClock(TMR0_MODULE);
    CLK_SetModuleClock(TMR0_MODULE, CLK_CLKSEL1_TMR0SEL_HIRC, 0);

    /---------------------------------------------------------------------------------------------------------/
    /* Init I/O Multi-function                                                                                 */
    /---------------------------------------------------------------------------------------------------------/
		
		CLK_EnableModuleClock(BPWM1_MODULE);
		SYS->GPF_MFPL = (SYS->GPF_MFPL & (~(BPWM1_CH0_PF3_Msk )))  | BPWM1_CH0_PF3;
		
    /* Set multi-function pins for UART0 RXD and TXD */
    SYS->GPB_MFPH = (SYS->GPB_MFPH & (~(UART0_RXD_PB12_Msk | UART0_TXD_PB13_Msk))) | UART0_RXD_PB12 | UART0_TXD_PB13;
		

}

int main(void)
{
    /* Unlock protected registers */
    SYS_UnlockReg();

    /* Init System, peripheral clock and multi-function I/O */
    SYS_Init();

    /* Lock protected registers */
    SYS_LockReg();
	
	  //GPIO_SetMode(PA, 0xFFFF, GPIO_MODE_OUTPUT); // Set PA0 ~ PA15 to output mode. 
		
		
		GPIO_SetMode(PH,BIT6,GPIO_MODE_OUTPUT);       // 'H' Portunun 6. pini dijital çikis olarak ayarlaniyor.
		GPIO_SetMode(PH,BIT7,GPIO_MODE_OUTPUT);       // 'H' Portunun 7. pini dijital çikis olarak ayarlaniyor.
		BPWM_EnableOutput(BPWM1, BPWM_CH_0_MASK);     // BPWM1 Channel 0 Enable ediliyor.
		BPWM_Start(BPWM1, BPWM_CH_0_MASK);            // BPWM1 Channel 0 baslatiliyor.
    while(1)
    {
			PH6=1;
			PH7=0;
						// Dc motor yön kontrolü için IN2 pasif ediliyor
			for(int i=0;i<=100;i++)     // PWM Duty oranini 0'dan 100'e kadar arttiriliyor
 			{
				BPWM_ConfigOutputChannel(BPWM1, 0, 50, i);// Birinci Parametre PWM numarasi, Ikinci Parametre PWM kanal numarasi, Üçüncü Parametre Frekans, Dördüncü Parametre PWM Orani	
				CLK_SysTickLongDelay(100000);// 100 mili saniye gecikme veriyoruz.   			
			}
			
			
			for(int i=100;i>0;i--)     // PWM Duty oranini 100'den 0'a kadar azaltiliyor
			{
				BPWM_ConfigOutputChannel(BPWM1, 0, 50, i);   // Birinci Parametre PWM numarasi, Ikinci Parametre PWM kanal numarasi, Üçüncü Parametre Frekans, Dördüncü Parametre PWM Orani
				CLK_SysTickLongDelay(100000);				
			}
   }
}
