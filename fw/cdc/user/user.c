
/** I N C L U D E S **********************************************************/
#include <p18cxxx.h>
#include "system\typedefs.h"
#include "system\usb\usb.h"
#include "io_cfg.h"             // I/O pin mapping
#include "user\user.h"
/** V A R I A B L E S ********************************************************/
#pragma udata
char input_buffer[64];
char my_buffer[64];
char output_buffer[32];
unsigned int counter,mPtr,index,inPtr,outPtr,Ost,Ist,Ast;
volatile unsigned char DATA,ch,OutputState,Ach;
unsigned int ADC;


/** P R I V A T E  P R O T O T Y P E S ***************************************/
void User_Process(void);
void Check_Update_Output(void);
void Check_Update_Input(void);
void update_bit(unsigned char Chan,unsigned char Data);
void GetInputStatus(void);
void Check_Update_ADC(void);
void Read_ADC(unsigned char ch);

#pragma code
void UserInit(void)
{
    mInitADC();
    mInitInput();
    mInitOutput();    
    inPtr=outPtr=Ost=Ist=Ast=0;        
}//end UserInit



/******************************************************************************
 * Function:        void ProcessIO(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        This function is a place holder for other user routines.
 *                  It is a mixture of both USB and non-USB tasks.
 *
 * Note:            None
 *****************************************************************************/
void ProcessIO(void)
{   
    // User Application USB tasks
    if((usb_device_state < CONFIGURED_STATE)||(UCONbits.SUSPND==1)) return;

    User_Process();

}//end ProcessIO


//----------------------------
// Read ADC
void Read_ADC(unsigned char CHANNEL)
{
	  unsigned char i;
	  
	    CHANNEL=CHANNEL & 0x07;
		ADCON0bits.CHS0=(CHANNEL&0b00000001);
		ADCON0bits.CHS1=((CHANNEL&0b00000010)>>1);
		ADCON0bits.CHS2=((CHANNEL&0b00000100)>>2);
	 
	    ADCON0bits.GO = 1;              // Start AD conversion
    	while(ADCON0bits.NOT_DONE);     // Wait for conversion
    	
    	output_buffer[0] = '*';
    	output_buffer[1] = 'C';
    	output_buffer[2] = 0x30 | CHANNEL;
    	ADC=ADRES;    	
    	// Populate string 
    	for(i=0;i<4;i++)
    	{
        	output_buffer[i+3] = (((char)(ADC % 10)) & 0x0F) | 0x30;
        	ADC /= 10;
   	 	}//end for               
        output_buffer[7] = 0x0D;
                
	    if(mUSBUSARTIsTxTrfReady())
        {
           	mUSBUSARTTxRam((byte*)output_buffer,8);
        }
    	
}	


//----------------------------
// Read ADC
void Check_Update_ADC(void)
{
		if (Ast==0)
	    {
		  if (DATA=='*') Ast=1;
		}
		else
		if (Ast==1)
		{
			Ast=0;
			if (DATA=='C') Ast=2;			
		}	
		else
		if (Ast==2)
		{
			Ach=DATA & 0x07;
			Ast=3;
		}
		else
		if (Ast==3)
		{
			if (DATA==0x0D)
			Read_ADC(Ach);
			Ast=0;
		}
		
}
	

//----------------------------
// Update Output
//----------------------------
void update_bit(unsigned char Chan,unsigned char Data)
{
			switch (Chan)
       		{
		    	case 0x00 : mLED_1=Data & 0x01;break;		        			
		    	case 0x01 : mLED_2=Data & 0x01;break;
		    	case 0x02 : mLED_3=Data & 0x01;break;
		    	case 0x03 : mLED_4=Data & 0x01;break;
		    	case 0x04 : mLED_5=Data & 0x01;break;
		    	case 0x05 : mLED_6=Data & 0x01;break;
		    	case 0x06 : mLED_7=Data & 0x01;break;
		    	case 0x07 : mLED_8=Data & 0x01;break;		               
		    }		    
}	

//----------------------------
// Get Input Status
// input : none
// output: USB Port <== [*]['B'][Status:Byte][0x0D] , total 4 byte
//----------------------------
void GetInputStatus(void)
{
	    output_buffer[0]='*';
	    output_buffer[1]='B';
	    output_buffer[2]=(PORTB & 0x0F) | 0x30;
	    output_buffer[3]=((PORTB>>4) & 0x0F) | 0x30;
	    output_buffer[4]=0x0D;
	    if(mUSBUSARTIsTxTrfReady())
        {
         	mUSBUSARTTxRam((byte*)output_buffer,5);
        }      
}	

//----------------------------
// Check to update output
//----------------------------
void Check_Update_Output(void)
{
         
	      if (Ost==0)
	      {
		    if (DATA=='*')Ost=1;
		  }
		  else
	 	  if (Ost==1)
		  {
			Ost=0;   
			if (DATA=='A')
			Ost=2;			
		  }
		  else
		  if (Ost==2)
		  {			
			ch=DATA & 0x07;
			Ost=3;
		  }
		  else
		  if (Ost==3)
		  {
			  OutputState=DATA & 0x01;
			 Ost=4;
		  }
		  else
		  if (Ost==4)
		  {			  
			  if (DATA==0x0D)			  
			  update_bit(ch,OutputState);
			 Ost=0;	
		  }			  
}

//----------------------------
// Check to update Input
//----------------------------
void Check_Update_Input(void)
{
  
	      if (Ist==0)
	      {
		    if (DATA=='*') Ist=1;
		  }
		  else
	 	  if (Ist==1)
		  {
			Ist=0;   
			if (DATA=='B')
			Ist=2;			
		  }
		  else
		  if (Ist==2)
		  {			  
			  if (DATA==0x0D)			  
			  GetInputStatus();
			  Ist=0;	
		  }			  
}
	
//****************************************
// User change here
//****************************************
void User_Process(void)
{
	

    if(getsUSBUSART(input_buffer,32))
    {
		mPtr = mCDCGetRxLength();
		for(index = 0; index < mPtr; index++)
		{  		
		   my_buffer[inPtr]= input_buffer[index];
		   inPtr++;
		   if (inPtr==64) inPtr=0;
		}  	    
    }
        
    if (inPtr!=outPtr)
    {
	      DATA=my_buffer[outPtr];
	      outPtr++;
	      if (outPtr==64) outPtr=0;

		  Check_Update_Output();
    	  Check_Update_Input();	      
    	  Check_Update_ADC();
	}      
	    
}//end 

