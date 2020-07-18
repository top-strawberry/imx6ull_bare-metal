#include "usr_app.h"


static void bsp_gpt1_irq_handler(IRQn_Type irq_num, void *arg)
{
	if((GPT1->SR >> GPT_OF1IE) & 0x1){
		GPT_LOG("bsp_gpt1_irq_handler\r\n");
		GPT1->SR |= 1 << GPT_OF1IE;
	}
}

static void bsp_gpt2_irq_handler(IRQn_Type irq_num, void *arg)
{
	if((GPT2->SR >> GPT_OF1IE) & 0x1){
		GPT_LOG("bsp_gpt2_irq_handler\r\n");
		GPT2->SR |= 1 << GPT_OF1IE;
	}
}


static void bsp_gpt_en(GPT_Type *gpt, FunctionalState NewState)
{
	if(NewState == enable){
		gpt->CR |= 1 << GPT_EN;
	}else if(NewState == disable){
		gpt->CR &= ~(1 << GPT_EN);
	}
}

int8_t bsp_gpt_init(GPT_Type *gpt)
{
	gpt->CR = 0;
	gpt->CR |= 1<< GPT_SWR;
	while((gpt->CR >> GPT_SWR) & 0x1);
	gpt->CR |= (1 << GPT_ENMOD) | (1 << GPT_CLKSRC);
	gpt->PR = 65;
	gpt->OCR[0] = 1000000 / 2;
	gpt->IR = 1 << GPT_OF1IE;
	if(gpt == GPT1){
		GIC_EnableIRQ (GPT1_IRQn);
		bsp_int_system_regiser_irq_handler (GPT1_IRQn, bsp_gpt1_irq_handler, NULL);
		bsp_gpt_en(GPT1, enable);
	}else if(gpt == GPT2){
		GIC_EnableIRQ (GPT2_IRQn);
		bsp_int_system_regiser_irq_handler (GPT2_IRQn, bsp_gpt2_irq_handler, NULL);
		bsp_gpt_en(GPT2, enable);
	}

    return 0;
}






