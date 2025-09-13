#include "sdram.h"

FMC_SDRAM_CommandTypeDef command;	// ����ָ��
/******************************************************************************************************
*	�� �� ��: SDRAM_Initialization_Sequence
*	��ڲ���: hsdram - SDRAM_HandleTypeDef����ı���������ʾ�����sdram
*				 Command	- ����ָ��
*	�� �� ֵ: ��
*	��������: SDRAM ��������
*	˵    ��: ����SDRAM���ʱ��Ϳ��Ʒ�ʽ
*******************************************************************************************************/

void SDRAM_Initialization_Sequence(SDRAM_HandleTypeDef *hsdram, FMC_SDRAM_CommandTypeDef *Command)
{
	__IO uint32_t tmpmrd = 0;

	/* Configure a clock configuration enable command */
	Command->CommandMode 				= FMC_SDRAM_CMD_CLK_ENABLE;	// ����SDRAMʱ�� 
	Command->CommandTarget 				= FMC_COMMAND_TARGET_BANK; 	// ѡ��Ҫ���Ƶ�����
	Command->AutoRefreshNumber 		= 1;
	Command->ModeRegisterDefinition 	= 0;

	HAL_SDRAM_SendCommand(hsdram, Command, SDRAM_TIMEOUT);	// ���Ϳ���ָ��
	HAL_Delay(1);		// ��ʱ�ȴ�

	/* Configure a PALL (precharge all) command */ 
	Command->CommandMode 				= FMC_SDRAM_CMD_PALL;		// Ԥ�������
	Command->CommandTarget 				= FMC_COMMAND_TARGET_BANK;	// ѡ��Ҫ���Ƶ�����
	Command->AutoRefreshNumber 		= 1;
	Command->ModeRegisterDefinition 	= 0;

	HAL_SDRAM_SendCommand(hsdram, Command, SDRAM_TIMEOUT);  // ���Ϳ���ָ��

	/* Configure a Auto-Refresh command */ 
	Command->CommandMode 				= FMC_SDRAM_CMD_AUTOREFRESH_MODE;	// ʹ���Զ�ˢ��
	Command->CommandTarget 				= FMC_COMMAND_TARGET_BANK;          // ѡ��Ҫ���Ƶ�����
	Command->AutoRefreshNumber			= 8;                                // �Զ�ˢ�´���
	Command->ModeRegisterDefinition 	= 0;

	HAL_SDRAM_SendCommand(hsdram, Command, SDRAM_TIMEOUT);	// ���Ϳ���ָ��

	/* Program the external memory mode register */
	tmpmrd = (uint32_t)SDRAM_MODEREG_BURST_LENGTH_2         |
							SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL   |
							SDRAM_MODEREG_CAS_LATENCY_3           |
							SDRAM_MODEREG_OPERATING_MODE_STANDARD |
							SDRAM_MODEREG_WRITEBURST_MODE_SINGLE;

	Command->CommandMode					= FMC_SDRAM_CMD_LOAD_MODE;	// ����ģʽ�Ĵ�������
	Command->CommandTarget 				= FMC_COMMAND_TARGET_BANK;	// ѡ��Ҫ���Ƶ�����
	Command->AutoRefreshNumber 		= 1;
	Command->ModeRegisterDefinition 	= tmpmrd;

	HAL_SDRAM_SendCommand(hsdram, Command, SDRAM_TIMEOUT);	// ���Ϳ���ָ��
	
	HAL_SDRAM_ProgramRefreshRate(hsdram, 918);  // ����ˢ����

}