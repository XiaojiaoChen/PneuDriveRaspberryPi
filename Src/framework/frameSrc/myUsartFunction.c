/*
 * myUsartFunction.cpp
 *
 *  Created on: Jun 12, 2018
 *      Author: 402072495
 */


#include <PneuDriveLL.h>
#include "myUsartFunction.h"
#include "cmsis_os.h"

#include "string.h"
#include <stdarg.h>
#include <stdio.h>

//To PC usb
UART_DEVICE Usart3Device;

//To raspberry pi's usart 1
UART_DEVICE Usart1Device;

//for user usage.
UART_DEVICE Usart2Device;

static void myInitUsartMode(UART_HandleTypeDef *huart,USARTMode usartMode);
static void printfBin_init();
static void _serialCallback(UART_DEVICE *usartDevice);
static void UsartRxIntReceivedPolling(UART_DEVICE *UsartDevice);
static void UsartRxDMAIdleReceivedPolling(UART_DEVICE *UsartDevice);
static UART_DEVICE *getUsartDevice(UART_HandleTypeDef *huart);


/*This file enables to use printf() to output strings to a uart:
 * 1.None blocking Tx and Rx using DMA
 * 2.Tx has Buffers without loss of transmission data
 * 3.Rx accept variable size
 *
 *
 * Usage:
 * 1.configure as below.
 * 2.For transmitting, use printf() as usual.
 * 3.For receiving, received string is automatically updated in serialxCallback(char *);
 *
 * Stm32CubeMx configuration:
 * Usart configuration: Tx DMA  Rx	DMA Enable Interrupt
 * file location:
 * 1. put my_UsartInit() in initialization part
 * 2. put myUsartDMAIRQ() or myUsartIntIRQ() in ISR for receiving data
 * 3. put Usart_ReceivePolling() in polling loop for processing data
 *
 */

/*put this function in the main.c for initilization*/
void my_UsartInit()
{

	 //Usart 1 is connected to raspberry pi, we handle Tx, rosserial handles Rx
	 myInitUsartMode(&huart1,usartDMACircularMode);

	 //for application use
	 myInitUsartMode(&huart2,usartIntMode);

	 //Usart 3 is connecting through usb by defaut, configured to DMA mode
	 myInitUsartMode(&huart3,usartDMAIdleMode);

	 //Enable printf Binany
	 printfBin_init();
}

static void myInitUsartMode(UART_HandleTypeDef *huart,USARTMode usartMode){
	UART_DEVICE *uartDev=getUsartDevice(huart);
	memset(uartDev,0,sizeof(UART_DEVICE));
	uartDev->huart = huart;
	uartDev->pRxBuf = uartDev->RxBuf;
	uartDev->pRxLineBuf=uartDev->RxLineBuf;
	uartDev->receveBinPtr = (RECEIVEBINSTRUCT *)uartDev->RxBuf;
	uartDev->usartmode=usartMode;
	if(usartMode==usartIntMode){
		 __HAL_UART_ENABLE_IT(huart,UART_IT_RXNE);
	}
	else if(usartMode==usartDMACircularMode){
		 /*get ready for receive*/
		 HAL_UART_Receive_DMA(uartDev->huart, uartDev->RxBuf, UART_RX_BUF_SIZE-1);

	}
	else if(usartMode==usartDMAIdleMode){

		 /*get ready for receive*/
		 HAL_UART_Receive_DMA(uartDev->huart, uartDev->RxBuf, UART_RX_BUF_SIZE-1);

		 /*Enable USART_Rx IDLE Detection to stop USART1_Rx_DMA*/
		 __HAL_UART_ENABLE_IT(uartDev->huart, UART_IT_IDLE);
	}
}

static UART_DEVICE *getUsartDevice(UART_HandleTypeDef *huart){
	if(huart==&huart1)
		return &Usart1Device;
	if(huart==&huart2)
		return &Usart2Device;
	if(huart==&huart3)
		return &Usart3Device;
	return &Usart3Device;
}




/************************************			*************************************/
/************************************	Transfer*************************************/
/************************************			************************************/
/*Redirect printf() by implementing (weak) _write function.
 *Every printf() call would store the output string in TxBuf[], ready for Usart DMA output instead of directly output*/
int _write(int file, char *pSrc, int len){
	return my_write_DMA(&huart3,(uint8_t *)pSrc,len);
}
int my_write_DMA(UART_HandleTypeDef *huart, uint8_t *pSrc, int len)
{
	UART_DEVICE *uartDev=getUsartDevice(huart);
	uint8_t *pDes=uartDev->TxBuf[uartDev->producerTxBufNum];

	//store the string to next buffer
	memcpy(pDes,pSrc,len);
	*(pDes+len)='\0';
	uartDev->countTxBuf[uartDev->producerTxBufNum] = len;

	//add one bufferedTxNum, recording how many buffered strings that haven't been sent
	uartDev->bufferedTxNum++;

	//Try to send just buffered string if this is the only one
	if(uartDev->bufferedTxNum == 1){
		HAL_UART_Transmit_DMA(uartDev->huart,pDes,uartDev->countTxBuf[uartDev->producerTxBufNum]);
		uartDev->TxStart = micros();
	}
	else{
	//TO DO, There is a bug here, when the builtInPWMFrequency is changed, the uartDevs would somehow suddenly lost the configurations
		uartDev->bufferedTxNum=uartDev->bufferedTxNum-1+1;
	}
	//move producerTxBufNum forward
	uartDev->producerTxBufNum++;
	uartDev->producerTxBufNum%=UART_TX_BUF_NUM;

	//Buffered term full, wait for consumer to reduce producerTxBufNum
//	while(uartDev->bufferedTxNum > (UART_TX_BUF_NUM-2)){
//		//Danger! May block the main program continuously !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//		//This waiting mechanism is to take care of the high frequency output within a short period during the Ethercat Initialization
//		//If the producer is always quicker than consumer, for example a high frequency output ,this function would block the program permanently
//	};
	return len;
}





/************************************							*************************************/
/************************************Transfer Complete Callback*************************************/
/************************************							************************************/
/*this function would overwrite HAL's weak HAL_UART_TxCpltCallback for all usart*/
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	UART_DEVICE *uartDev=getUsartDevice(huart);
	if(uartDev->usartmode==usartDMAIdleMode){
		UART_DEVICE *uartDev=getUsartDevice(huart);
		 /*update information*/
		 uartDev->TxEnd = micros();
		 uartDev->lastTxTime = uartDev->TxEnd - uartDev->TxStart;
		 uartDev->lastTxCount = uartDev->countTxBuf[uartDev->consumerTxBufNum];

		/*One consumption done. move consumer forward*/
		uartDev->consumerTxBufNum++;
		uartDev->consumerTxBufNum%=UART_TX_BUF_NUM;

		/*reduce one bufferedTxNum*/
		 uartDev->bufferedTxNum--;

		/*If it is still positive, go on consume next*/
		if(uartDev->bufferedTxNum>0){
			uartDev->TxStart = micros();
			uint8_t *px = &uartDev->TxBuf[uartDev->consumerTxBufNum][0];
			HAL_UART_Transmit_DMA(uartDev->huart,px,uartDev->countTxBuf[uartDev->consumerTxBufNum]);
		}
	}

}

/************************************							*************************************/
/************************************Receive Complete Callback*************************************/
/************************************							************************************/
/*this function would overwrite HAL's weak HAL_UART_RxCpltCallback for all usart*/






/************************************							*************************************/
/************************************Receive Callback*************************************/
/************************************							************************************/
/*put this function in stm32f7xx_it.c as below
	void USART3_IRQHandler(void){
		myUsartDMAIRQ(&huart3) ;
		HAL_UART_IRQHandler(&huart3);
	}
*/
void myUsartDMAIdleIRQ(UART_HandleTypeDef *huart)
{
	UART_DEVICE *uartDev=getUsartDevice(huart);
	 uint32_t tmp_flag = __HAL_UART_GET_FLAG(huart, UART_FLAG_IDLE);
	 uint32_t tmp_it_source = __HAL_UART_GET_IT_SOURCE(huart, UART_IT_IDLE);
	 /* UART RX Idle interrupt*/
	 if((tmp_flag != RESET) && (tmp_it_source != RESET)){

		 /*Clear Idle Flag*/
		__HAL_UART_CLEAR_IDLEFLAG(huart);

		/*receive flag*/
		uartDev->Received = 1;
		/*We stop the DMA in the polling task, not here, since some data are still on the fly now*/
	}
}

void myUsartDMAIRQ(UART_HandleTypeDef *huart)
{

}


/*put this function in stm32f7xx_it.c as below
	void USART2_IRQHandler(void)
	{
	  myUsartIntIRQ(&huart2);
	  HAL_UART_IRQHandler(&huart2);
	}
*/
void myUsartIntIRQ(UART_HandleTypeDef *huart)
{
	UART_DEVICE *uartDev=getUsartDevice(huart);
	if(uartDev->usartmode==usartIntMode){
	UART_DEVICE *uartDev=getUsartDevice(huart);
 	if(__HAL_UART_GET_FLAG(huart, UART_FLAG_RXNE)!=RESET)   //receive interrupt
	{
		*(uartDev->pRxBuf)=(uint8_t)(huart->Instance->RDR & (uint8_t)0x00FF);  //read and clear flag
		if(*(uartDev->pRxBuf)==0x0a) // if current char is 0x0a, take care. If not, go on receiving.
		{
			if((uartDev->pRxBuf)!=(uartDev->RxBuf)) // if '0x0a' is not in the beginning
			{
				if(*(uartDev->pRxBuf-1)==0x0d)// if previous char is 0x0d, legal end.
			    {
					uartDev->Received = 1;
				}
				else//if previous char is not 0x0d, illegal end. Reset Buffer.
				{
					memset(uartDev->RxBuf,0,UART_RX_BUF_SIZE);
					uartDev->pRxBuf=uartDev->RxBuf;
				}
			}//if '0x0a' is received in the beginning. do nothing.
		}
		else uartDev->pRxBuf++;
	}
	}
}

/*This function is put in the control loop in freeRTOS.c for polling*/
void Usart_ReceivePolling()
{
	/********************************Usart 1 DMA reception is circular****************************/

	/********************************Usart 2 Int reception****************************/
	UsartRxIntReceivedPolling(&Usart2Device);

	/********************************Usart 3 DMA reception****************************/
	UsartRxDMAIdleReceivedPolling(&Usart3Device);
}


inline static void _serialCallback(UART_DEVICE *usartDevice){
	char *pSerialBuf=(usartDevice->usartmode==usartDMACircularMode)?((char *)usartDevice->RxLineBuf):((char *)usartDevice->RxBuf);

	if(usartDevice==&Usart2Device){
		serial2Callback(pSerialBuf);
	}
	else if(usartDevice==&Usart3Device){
		serial3Callback(pSerialBuf);
	}
}

static void UsartRxIntReceivedPolling(UART_DEVICE *UsartDevice){
	if (UsartDevice->Received == 1) {
		_serialCallback(UsartDevice);
		memset(UsartDevice->RxBuf,0,UART_RX_BUF_SIZE);
		UsartDevice->pRxBuf=UsartDevice->RxBuf;
		UsartDevice->Received = 0;
	}
}

static void UsartRxDMAIdleReceivedPolling(UART_DEVICE *UsartDevice)
{
	/********************************Usart DMA reception****************************/
		/*Only process with idle receiving detection*/
		if (UsartDevice->Received == 1) {

			/* Stop UART DMA Rx request if ongoing */
			if ((UsartDevice->huart->RxState == HAL_UART_STATE_BUSY_RX) && (HAL_IS_BIT_SET(UsartDevice->huart->Instance->CR3, USART_CR3_DMAR))) {
				CLEAR_BIT(UsartDevice->huart->Instance->CR3, USART_CR3_DMAR);

				/* Abort the UART DMA Rx channel */
				if (UsartDevice->huart->hdmarx != NULL) {
					/* Determine how many items of data have been received */
					UsartDevice->countRxBuf = UsartDevice->huart->RxXferSize - __HAL_DMA_GET_COUNTER(UsartDevice->huart->hdmarx);
					UsartDevice->huart->RxXferCount = 0;

					/*Abort DMA*/
					HAL_DMA_Abort(UsartDevice->huart->hdmarx);
				}

				/* Disable RXNE, PE and ERR (Frame error, noise error, overrun error) interrupts */
				CLEAR_BIT(UsartDevice->huart->Instance->CR1, (USART_CR1_RXNEIE | USART_CR1_PEIE));
				CLEAR_BIT(UsartDevice->huart->Instance->CR3, USART_CR3_EIE);

				/* At end of Rx process, restore huart->RxState to Ready */
				UsartDevice->huart->RxState = HAL_UART_STATE_READY;
			}

			/*Process commands buffer*/
			//since this termination is triggered by IDLE LINE, we keep looking for end of line '\r\n'
			int len=UsartDevice->countRxLineBuf+UsartDevice->countRxBuf;

			//If overflow Clear
			if(len>UART_RX_BUF_SIZE)
			{
				memset(UsartDevice->RxLineBuf,0,UART_RX_BUF_SIZE);
				UsartDevice->pRxLineBuf=UsartDevice->RxLineBuf;
				UsartDevice->countRxLineBuf=0;
			}
			else{
				//buffer to line
				memcpy(&(UsartDevice->RxLineBuf[UsartDevice->countRxLineBuf]),UsartDevice->RxBuf,UsartDevice->countRxBuf);
				UsartDevice->pRxLineBuf+=UsartDevice->countRxBuf;
				UsartDevice->countRxLineBuf=len;

				//if end of line
				if (UsartDevice->RxLineBuf[UsartDevice->countRxLineBuf-1]=='\n')
				{
					_serialCallback(UsartDevice);
					memset(UsartDevice->RxLineBuf,0,len);
					UsartDevice->pRxLineBuf=UsartDevice->RxLineBuf;
					UsartDevice->countRxLineBuf=0;
				}
			}

			/*clear Recived flag*/
			UsartDevice->Received = 0;

			/*clear buffer*/
			memset(UsartDevice->szCmd, 0, sizeof(UsartDevice->szCmd));
			memset(UsartDevice->RxBuf, 0, UART_RX_BUF_SIZE);
			UsartDevice->countRxBuf = 0;
		}
		//Always try to start a new reception
		HAL_UART_Receive_DMA(UsartDevice->huart, UsartDevice->RxBuf, UART_RX_BUF_SIZE - 1);
}


int my_read_DMA_byte(UART_HandleTypeDef *huart){
	int c=-1;
	UART_DEVICE *uartDev=getUsartDevice(huart);
	uint32_t RdmaInd=(UART_RX_BUF_SIZE - __HAL_DMA_GET_COUNTER(huart->hdmarx)) & (UART_RX_BUF_SIZE - 1);
    if(uartDev->RxInd != RdmaInd){
      c = uartDev->RxBuf[uartDev->RxInd++];
      uartDev->RxInd &= (UART_RX_BUF_SIZE - 1);
    }
    return c;
}






typedef struct
{
	unsigned char header[4];
	int16_t data[UART_TX_BUF_SIZE/2];
	int16_t *pData;
} __attribute__((packed)) PRINTFBINSTRUCT;
/*serial bin format:
 * Hex: 0x5a  0x5a 0xa5  0xa5 data 	0x0D 	0x0A
 * Dec: 90    90   165   165  data  13 		10
 * char: Z    Z     	    data  \r  	\n
 */
static PRINTFBINSTRUCT printfBinStruct;
//input: array pointer, and data number
//function: add header and tail, send into buffer
static void printfBin_init()
{
	memset(&printfBinStruct,0,sizeof(PRINTFBINSTRUCT));
	printfBinStruct.header[0]=0x5a;
	printfBinStruct.header[1]=0x5a;
	printfBinStruct.header[2]=0xa5;
	printfBinStruct.header[3]=0xa5;
	printfBinStruct.pData=printfBinStruct.data;
}
void printfBin_appendData(int16_t dat)
{
*(printfBinStruct.pData)++=dat;
}
void printfBin()
{
int len=(printfBinStruct.pData-printfBinStruct.data)*sizeof(int16_t);
char *pChar=(char *)(printfBinStruct.pData);
*(pChar)++='\r';
*(pChar)++='\n';
len= len+6;
printfBinStruct.pData = printfBinStruct.data;
_write(0,(char *)(&printfBinStruct),len);
}

