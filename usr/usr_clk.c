#include "usr_clk.h"



// 使能外设时钟
void usr_clk_set_enable(FunctionalState NewState)
{
    if(NewState == enable){
        CCM->CCGR0 = 0xffffffff;
        CCM->CCGR1 = 0xffffffff;
        CCM->CCGR2 = 0xffffffff;
        CCM->CCGR3 = 0xffffffff;
        CCM->CCGR4 = 0xffffffff;
        CCM->CCGR5 = 0xffffffff;
        CCM->CCGR6 = 0xffffffff;
    }else if(NewState == disable){
        CCM->CCGR0 = 0x00000000;
        CCM->CCGR1 = 0x00000000;
        CCM->CCGR2 = 0x00000000;
        CCM->CCGR3 = 0x00000000;
        CCM->CCGR4 = 0x00000000;
        CCM->CCGR5 = 0x00000000;
        CCM->CCGR6 = 0x00000000;
    }
}


void usr_clk_init(void)
{
    //usr_clk_set_enable(disable);//关闭外设时钟

    //开始设置系统时钟(包括PLL1、PLL2、PLL3、PLL4、PLL5、PLL6、PLL7)

    //开始设置PLL1  1056MHz
    CCM->CACRR &= ~(0x3 << ARM_PODF);       //清除这3位
    CCM->CACRR |= cacrr_div4 << ARM_PODF;   //设置4分频

    //切换pll1_sw_clk时钟源切换到step_sel
    CCM->CCSR &= ~(0x1 << STEP_SEL);        //设置step_sel时钟源 为晶振时钟
    CCM->CCSR |= 0x1 << PLL1_SW_CLK_SEL;    //将pll1_sw_clk时钟源切换到step_sel

    //设置PLL1时钟频率
    CCM_ANALOG->PLL_ARM &= ~(0x7f << DIV_SELECT);  //
    CCM_ANALOG->PLL_ARM |= (0x58 << DIV_SELECT); //Valid range for divider value: 54-108. Fout = Fin * div_select/2.0
    CCM_ANALOG->PLL_ARM |= (0x1 << ENABLE); //使能PLL

    //切换pll1_sw_clk时钟源切换到pll1_main_clk
    CCM->CCSR &= ~(0x1 << PLL1_SW_CLK_SEL); //切换pll1_sw_clk时钟源切换到pll1_main_clk
    //结束设置PLL1

    //开始设置PLL2(默认是528MHz) 及 PFD
    CCM->CCSR = 0x1 << SECONDARY_CLK_SEL; //设置secondary_clk(pll2_sw_clk)选择pll2_main_clk(PLL2 528MHz)
    //设置PFD
    CCM_ANALOG->PFD_528 &= ~0x3f3f3f3f;
    CCM_ANALOG->PFD_528 |=  (32 << PFD3_FRAC);
    CCM_ANALOG->PFD_528 |=  (24 << PFD2_FRAC);
    CCM_ANALOG->PFD_528 |=  (16 << PFD1_FRAC);
    CCM_ANALOG->PFD_528 |=  (27 << PFD0_FRAC);
    //结束设置PLL2

    //开始设置PLL3(默认是480MHz) 及 PFD
    CCM->CCSR &= ~(0x1 << PLL3_SW_CLK_SEL); //设置pll3_sw_clk 选择pll3_main_clk
    //设置PFD
    CCM_ANALOG->PFD_480 &= ~0x3f3f3f3f;
    CCM_ANALOG->PFD_480 |=  (19 << PFD3_FRAC);
    CCM_ANALOG->PFD_480 |=  (17 << PFD2_FRAC);
    CCM_ANALOG->PFD_480 |=  (16 << PFD1_FRAC);
    CCM_ANALOG->PFD_480 |=  (12 << PFD0_FRAC);
    //结束设置PLL3

    //开始设置ARM_CLK_ROOT内核时钟528MHz
    CCM->CACRR |= cacrr_div2 << ARM_PODF;   //设置2分频
    //结束设置ARM_CLK_ROOT内核时钟


    //开始设置AHB_CLK_ROOT外设总线时钟132MHz
    // while( (CCM->CDHIPR >> AHB_PODF_BUSY) & 0x1 );
    // CCM->CBCDR &= ~(0x7 << AHB_PODF); //设置CBCDR[AHB_PODF]不分频
    // while( (CCM->CDHIPR >> AHB_PODF_BUSY) & 0x1 );
    // CCM->CBCDR |= ahb_podf_div4 << AHB_PODF; //设置CBCDR[AHB_PODF]四分频

    //设置IPG_CLK_ROOT
    CCM->CBCDR &= ~(0x3 << IPG_PODF);
    CCM->CBCDR |= ipg_podf_div2 << IPG_PODF;
    //结束IPG_CLK_ROOT

    //开始设置PERCLK_CLK_ROOT
    CCM->CSCMR1 &= ~(0x3f << PERCLK_PODF);
    CCM->CSCMR1 |= perclk_podf_div1 << PERCLK_PODF;
    //结束设置PERCLK_CLK_ROOT

    //配置UART时钟源 480MHz / 6 = 80MHz
    CCM->CSCDR1 &= ~(0x1 << UART_CLK_SEL); //选择时钟源
    CCM->CSCDR1 &= ~(0x3f << UART_CLK_PODF);//
    CCM->CSCDR1 |= clk_podf_div1 << UART_CLK_PODF;//1分频

    //使能时钟
    usr_clk_set_enable(enable);
}






