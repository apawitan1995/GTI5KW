/*
 * CAN_app.c
 *
 *  Created on: Mar 25, 2014
 *      Author: angga
 */

#include "peripheral/CAN_app.h"

volatile char data_can[8];

uint32_t SendCanMessage
(
	const CAN001_HandleType* Handle,
	uint8_t MsgObjnr,
	uint8_t DataLength,
	const uint8_t* DataPtr
)
{
	uint32_t status;

	CAN001_ClearMOFlagStatus(Handle,MsgObjnr,TRANSMIT_PENDING);

	CAN001_UpdateMODataRegisters(Handle,MsgObjnr,DataLength,DataPtr);

//	sprintf(kal,"Update LMO3 Status %d \r\n", status);
//	UART001_WaitWriteDataMultiple(UART_TTL,kal,strlen(kal));

	CAN001_SendDataFrame(Handle,MsgObjnr); //

//	sprintf(kal,"Send LMO3 Status %d \r\n", status);
//	UART001_WaitWriteDataMultiple(UART_TTL,kal,strlen(kal));

	//while (CAN_SET != CAN001_GetMOFlagStatus(Handle,MsgObjnr,TRANSMIT_PENDING));

	return status;
}

void EventHandlerCAN001_0(void)
{
	static char kal[32];
	uint8_t i=0;
	uint8_t Data3[8] = {0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03};
	status_t status;

	sprintf(kal,"Interrupt! \r\n");
	UART001_WaitWriteDataMultiple(UART_RS232,kal,strlen(kal));

	/* Check transmit pending status in LMO1 */
	if(CAN001_GetMOFlagStatus(&CAN001_Handle0,2,TRANSMIT_PENDING) == CAN_SET)
	{
		/* Clear the flag */
		CAN001_ClearMOFlagStatus(&CAN001_Handle0,2,TRANSMIT_PENDING);

		sprintf(kal,"Transmit Pending LMO1 \r\n");
		UART001_WaitWriteDataMultiple(UART_RS232,kal,strlen(kal));
	}

	/* Check receive pending status in LMO2 */
	if(CAN001_GetMOFlagStatus(&CAN001_Handle0,1,RECEIVE_PENDING) == CAN_SET)
	{

//		sprintf(kal,"Receive Pending LMO1 ");
//		UART001_WaitWriteDataMultiple(UART_RS232,kal,strlen(kal));

		/* Clear the flag */
		CAN001_ClearMOFlagStatus(&CAN001_Handle0,1,RECEIVE_PENDING);
		/* Read the received Message object and stores in variable CanRecMsgObj */
		CAN001_ReadMsgObj(&CAN001_Handle0,&CanRecMsgObj,1);

		/* Switch on LED Pin 3.9  to indicate that the requested message is received*/
		//IO004_ResetPin(IO004_Handle0);

		for (i=0;i<8;i++)
		{
			data_can[i] = CanRecMsgObj.data[i];

			sprintf(kal,"%X",data_can[i]);
			UART001_WaitWriteDataMultiple(UART_RS232,kal,strlen(kal));
		}

		sprintf(kal,"*%c%c#",data_can[0],data_can[1]);
		UART001_WaitWriteDataMultiple(UART_U1,kal,strlen(kal));

//		sprintf(kal," Receive LMO2 End \r\n");
//		UART001_WaitWriteDataMultiple(UART_TTL,kal,strlen(kal));

//		// SEND LMO3
//		status = CAN001_UpdateMODataRegisters(&CAN001_Handle0,3,8,Data3);
//
//		sprintf(kal,"Update LMO3 Status %d \r\n", status);
//		UART001_WaitWriteDataMultiple(UART_TTL,kal,strlen(kal));
//
//		status = CAN001_SendDataFrame(&CAN001_Handle0,3); //
//
//		sprintf(kal,"Send LMO3 Status %d \r\n", status);
//		UART001_WaitWriteDataMultiple(UART_TTL,kal,strlen(kal));

	}

//	/* Check receive pending status in LMO4 */
//	if(CAN001_GetMOFlagStatus(&CAN001_Handle0,4,RECEIVE_PENDING) == CAN_SET)
//	{
//		sprintf(kal,"Receive Pending LMO4 ");
//		UART001_WaitWriteDataMultiple(UART_TTL,kal,strlen(kal));
//
//		/* Clear the flag */
//		CAN001_ClearMOFlagStatus(&CAN001_Handle0,4,RECEIVE_PENDING);
//		/* Read the received Message object and stores in variable CanRecMsgObj */
//		CAN001_ReadMsgObj(&CAN001_Handle0,&CanRecMsgObj,4);
//
//		for (i=0;i<8;i++)
//		{
//			data_can[i] = CanRecMsgObj.data[i];
//			sprintf(kal,"%X ",data_can[i]);
//			UART001_WaitWriteDataMultiple(UART_TTL,kal,strlen(kal));
//		}
//
//		sprintf(kal," Receive LMO4 End \r\n");
//		UART001_WaitWriteDataMultiple(UART_TTL,kal,strlen(kal));
//	}

	/* Check for Node error */
	if(CAN001_GetNodeFlagStatus(&CAN001_Handle0,CAN001_ALERT_STATUS) == CAN_SET)
	{
		sprintf(kal,"Error \r\n");
		UART001_WaitWriteDataMultiple(UART_RS232,kal,strlen(kal));

		/* Clear the flag */
		CAN001_ClearNodeFlagStatus(&CAN001_Handle0,CAN001_ALERT_STATUS);
	}
}
