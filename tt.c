#include <stdio.h>
#include "NuMicro.h"

float distance = 0;

void SYS_Init(void)
{
    // Sistem saatini ve UART0'i baslat
    CLK_EnableXtalRC(CLK_PWRCTL_HIRCEN_Msk);
    CLK_WaitClockReady(CLK_STATUS_HIRCSTB_Msk);
    CLK_SetHCLK(CLK_CLKSEL0_HCLKSEL_HIRC, CLK_CLKDIV0_HCLK(1));
    CLK_EnableModuleClock(UART0_MODULE);
    CLK_SetModuleClock(UART0_MODULE, CLK_CLKSEL1_UART0SEL_HIRC, CLK_CLKDIV0_UART0(1));
    
    // UART0 pinlerini ayarla
    SYS->GPB_MFPH = (SYS->GPB_MFPH & (~(UART0_RXD_PB12_Msk | UART0_TXD_PB13_Msk))) 
                    | UART0_RXD_PB12 | UART0_TXD_PB13;
}

void UART_Init(void)
{
    // UART modülünü sifirla ve 115200 baud hizini ayarla
    SYS_ResetModule(UART0_RST);
    UART_Open(UART0, 115200);
}

uint32_t Read_HCSR04(void)
{
    uint32_t time = 0;

    PF7 = 1;                      // Trigger sinyali gönder
    CLK_SysTickDelay(10);         // 10 µs gecikme
    PF7 = 0;

    while (!PF6);                 // Echo pininin HIGH olmasini bekle
    while (PF6)                   // Echo pinini ölç
    {
        time++;
        CLK_SysTickDelay(1);      // 1 µs gecikme
    }
    return time * 2;              // Çift yönlü zaman döndür
}

int main(void)
{
    // Sistem baslatma
    SYS_UnlockReg();
    SYS_Init();
    UART_Init();
    SYS_LockReg();

    // GPIO pinlerini ayarla
    GPIO_SetMode(PF, BIT7, GPIO_MODE_OUTPUT); // PF7: Trigger için çikis
    GPIO_SetMode(PF, BIT6, GPIO_MODE_INPUT);  // PF6: Echo için giris

    printf("Ultrasonik Mesafe Olcumu Basladi\n");

    while (1)
    {
        uint32_t sensor_time = Read_HCSR04();       // Sensör zamanini oku
        distance = (sensor_time / 2) * 0.0343;     // Mesafeyi hesapla
        printf("Mesafe: %.1f cm\n", distance);     // Mesafeyi UART üzerinden gönder
        CLK_SysTickDelay(2000000);                 // 2 saniye bekle
		}
}
