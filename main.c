#include <stdio.h>
#include "NuMicro.h"

int sayac = 0;




void TMR1_IRQHandler(void) {
	
	if(TIMER_GetIntFlag(TIMER1) == 1) {
		TIMER_ClearIntFlag(TIMER1);
	}
	sayac++;
	
	if (sayac == 1) {
		PA0 = 1;
		
	}
	else if ( sayac == 2 )
	{
		PA0 = 0 ;
		PA1= 1 ;
	}
	
	else if (sayac ==3 )  {
		PA1 = 0 ;
		PA2= 1 ;
	}
	else if (sayac == 4 ){
	PA2 = 0 ;
		PA3  = 1;
	}
		
	else if (sayac == 5 ){
	
	  PA3 = 0 ;
		PA4 = 1;
	
	}
		else if ( sayac == 6 )
	{
		PA4= 0 ;
		PA3= 1 ;
	}
	
	else if (sayac ==7)  {
		PA3 = 0 ;
		PA2= 1 ;
	}
	else if (sayac == 8 ){
	PA2 = 0 ;
		PA1  = 1;
	}
		
	else if (sayac == 9 ){
	 sayac = 0 ;
	PA1= 0 ;
		PA0 = 1;
	
	}
	
}





void SYS_Init(void)
{
    SYS_UnlockReg();

    /* If the macros do not exist in your project, please refer to the related clk.h in Header folder of the tool package */
    /* Enable clock source */
    CLK_EnableXtalRC(CLK_PWRCTL_LIRCEN_Msk|CLK_PWRCTL_HIRCEN_Msk|CLK_PWRCTL_HIRC48EN_Msk);

    /* Waiting for clock source ready */
    CLK_WaitClockReady(CLK_STATUS_LIRCSTB_Msk|CLK_STATUS_HIRCSTB_Msk|CLK_STATUS_HIRC48STB_Msk);

    /* Set HCLK clock */
    CLK_SetHCLK(CLK_CLKSEL0_HCLKSEL_HIRC, CLK_CLKDIV0_HCLK(1));

    /* Set PCLK-related clock */
    CLK->PCLKDIV = (CLK_PCLKDIV_APB0DIV_HCLK | CLK_PCLKDIV_APB1DIV_HCLK);

    /* Enable IP clock */
    CLK_EnableModuleClock(FMCIDLE_MODULE);
    CLK_EnableModuleClock(ISP_MODULE);
    CLK_EnableModuleClock(TMR0_MODULE);
    CLK_EnableModuleClock(TMR1_MODULE);
    CLK_EnableModuleClock(WDT_MODULE);
    CLK_EnableModuleClock(WWDT_MODULE);

    /* Set IP clock */
    CLK_SetModuleClock(TMR0_MODULE, CLK_CLKSEL1_TMR0SEL_HIRC, MODULE_NoMsk);
    CLK_SetModuleClock(TMR1_MODULE, CLK_CLKSEL1_TMR1SEL_HIRC, MODULE_NoMsk);
    CLK_SetModuleClock(WDT_MODULE, CLK_CLKSEL1_WDTSEL_LIRC, MODULE_NoMsk);
    CLK_SetModuleClock(WWDT_MODULE, CLK_CLKSEL1_WWDTSEL_HCLK_DIV2048, MODULE_NoMsk);

    /* Update System Core Clock */
    /* User can use SystemCoreClockUpdate() to calculate SystemCoreClock. */
    SystemCoreClockUpdate();
		
		
		
		
		   /* If the macros do not exist in your project, please refer to the corresponding header file in Header folder of the tool package */
    SYS->GPA_MFPH = 0x00000000;
    SYS->GPA_MFPL = SYS_GPA_MFPL_PA1MFP_GPIO | SYS_GPA_MFPL_PA0MFP_GPIO;
    SYS->GPB_MFPH = 0x00000000;
    SYS->GPB_MFPL = 0x00000000;
    SYS->GPC_MFPH = 0x00000000;
    SYS->GPC_MFPL = 0x00000000;
    SYS->GPD_MFPH = 0x00000000;
    SYS->GPD_MFPL = 0x00000000;
    SYS->GPE_MFPH = 0x00000000;
    SYS->GPE_MFPL = 0x00000000;
    SYS->GPF_MFPH = 0x00000000;
    SYS->GPF_MFPL = SYS_GPF_MFPL_PF1MFP_ICE_CLK | SYS_GPF_MFPL_PF0MFP_ICE_DAT;
    SYS->GPG_MFPH = 0x00000000;
    SYS->GPG_MFPL = 0x00000000;
    SYS->GPH_MFPH = 0x00000000;
    SYS->GPH_MFPL = 0x00000000;
				

    /* Lock protected registers */
    SYS_LockReg();

}



int main()
{
		SYS_Init();
		
		GPIO_SetMode(PA, BIT0, GPIO_MODE_OUTPUT);
		GPIO_SetMode(PA, BIT1, GPIO_MODE_OUTPUT);		
	  GPIO_SetMode(PA, BIT2, GPIO_MODE_OUTPUT);		
	  GPIO_SetMode(PA, BIT3, GPIO_MODE_OUTPUT);		
	  GPIO_SetMode(PA, BIT4, GPIO_MODE_OUTPUT);		
		PA0 = 0;
		PA1 = 0;
		PA2 = 0;
		PA3 = 0;
		PA4 = 0;
	
		TIMER_Open(TIMER1, TIMER_PERIODIC_MODE, 1);
		TIMER_EnableInt(TIMER1);
		NVIC_EnableIRQ(TMR1_IRQn);
		TIMER_Start(TIMER1);
    while(1);
}

