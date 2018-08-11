
#include "AppData.h"
#include "WhoAmITask.h"
#include "PacketHandle.h"
#include "DeviceControl.h"
#include "main.h"


//Task
extern taskHandle_t xTask;

extern Flags xFlags;
extern DataLocal xDataLocal;

extern PacketIO xPacketSend;


/******************************************************************************
 * Function		: void vWhoAmITask_Task( void *pvParameters)
 * Description	: Task xu ly ban tin Who Am I
 * Param		: none
 * Return		: none
*******************************************************************************/
void vWhoAmITask_Task( void *pvParameters)
{
	macroTASK_DELAY_MS( 10000 );
	APP_DEBUG("********************** Who Am I Task *********************\r\n");
	
    while( 1 )
    {
		xTask.uiWhoAmITask_Finish = 1;
		uint8_t uCnt = 0;
		uint8_t uResend = 0;
		
		while(uResend <= 8)
		{
			if(uResend > 0)
				APP_DEBUG("\r\n--- WhoAmITask: Resend \"who am i\" to check connection\r\n");
			vPacketHandle_SendWhoAmI();
			uCnt = 0;
			while(1)
			{
				macroTASK_DELAY_MS( 1000 );
				
				if(xFlags.bConnectivityIsConnected == true)
				{
					goto EndWhoAmI;
				}
				
				if(uCnt >= 10)
					break;
				uCnt++;
			}
			uResend++;
		}

		EndWhoAmI:;
	
		if(xFlags.bConnectivityIsConnected == false)
		{
			APP_DEBUG("--- WhoAmITask: ZigB is not Response --> Reset ZigB.\r\n");
			macroTASK_DELAY_MS( 100 );
			//reset Zigb
			macroPOWER_OFF(macroCONN_POWER_GPIO, macroCONN_POWER_PIN);
			macroTASK_DELAY_MS( 1000 );
			macroPOWER_ON(macroCONN_POWER_GPIO, macroCONN_POWER_PIN);
		}
		else
		{
			APP_DEBUG("--- WhoAmITask: ZigB is connected....\r\n");
		}
		
		//Task finish
		xTask.uiWhoAmITask_Finish = 0;
        vTaskSuspend( NULL );								  
    }
}













