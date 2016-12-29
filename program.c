#pragma config OSC = HS 	// HS oscillator
#pragma config WDT = OFF 	// Watcdog timer off 
#pragma config MCLRE = ON 	// Master clear reset On
#pragma config DEBUG = OFF  // Debug off
#pragma config LVP = OFF 	// Low voltage programming off

#include <p18f4520.h> 	          	  // header files of PIC18F4520
#include <delays.h> 		          	 // include delays
#include <portb.h> 
#include <adc.h>// Header files required for port B interrupts

#define  BUZZER  	PORTCbits.RC0
#define  LED   		PORTCbits.RC1
#define  SENSOR		PORTBbits.RB0


#define  Distance25			0x140
#define  Distance20			0x10E
#define  Distance15			0x157

int adc_convert( void);
void INT0_ISR (void);
void test_isr (void);
void beep_led(int x);
void delay(int y);
void varbeep(int z);

//void beep_flash(int n);

#pragma code my_interrupt = 0x08 	//High Interrupt vector @ 0x08
void my_interrupt (void) 
{			// At location 0x08 we place the assembler instruction GOTO	
	
	_asm       // This is the assembly code at vector location
		GOTO test_isr
	_endasm
}
#pragma code 
/*********************************************************/
/* Bellow we test for the source of the interrupt */
 	// used to allow linker to locate remaining code
#pragma interrupt test_isr
void test_isr (void)
{
	if (INTCONbits.INT0IF == 1) 
		{		// Was interrupt caused by INT0 ?
			while(1)
			{
				INT0_ISR ();
			} //Once the interrupt implemented,the interrupt service routine will always be implemented.
		}
}


void main (void)
{
	int number=0;			//Counting the number of steps  
	int step1=1;			//Flag bit of Part 1 of the program   
	int step2=0;			//Flag bit of Part 2 of the program
	int step3=0;			//Flag bit of Part 3 of the program
	int step4=0;			//Flag bit of Part 4 of the program
/*********************************************************/
	ADCON1 	=	0x0F;    	// Set Ports as Digital I/O rather than analogue
	TRISD 	= 	0x00;  		// Make Port D all output
	LATD 	= 	0x00;  		// Clear Port D
	LATC	=	0x00;		// Clear Port D
	TRISC	=	0x00;		// Make Port D all output
	TRISA 	= 	0xff; 		// Make Port D all input
	LATA	=	0X00;		// Clear Port D

/*********************************************************/
	BUZZER = 0; 			// Turn off the Buzzer
	LED=0;				// Turn off the LED		
	
	OpenRB0INT( PORTB_CHANGE_INT_ON & FALLING_EDGE_INT & PORTB_PULLUPS_ON);
	INTCONbits.INT0IE = 1;  //Enable INT0 interrupts
	INTCONbits.INT0IF = 0;  // Clear INT0  Interrupt flag
	INTCONbits.GIE = 1; // Enable global interrupts

/********************************************************/	
		
		while(1)
	{
		if(step1)		/*If the step1 equal to 1,that's mean the car will turn around to find the object.
					Otherwise,this part of program will not be implemented*/
		{	
		
				if(adc_convert()>Distance25){step2=1;step1=0;}/*If ADC find the distance between car and object is 
										nearer than 25,the car will stop turn around*/
			
				PORTD=0x51;
				Delay10KTCYx(20);
				PORTD=0x49;
				Delay10KTCYx(20);
				PORTD=0x68;
				Delay10KTCYx(20);
				PORTD=0x2a;
				Delay10KTCYx(20);
				PORTD=0xa2;
				Delay10KTCYx(20);
				PORTD=0x86; 
				Delay10KTCYx(20);
				PORTD=0x94; 
				Delay10KTCYx(20);
				PORTD=0x15;
				Delay10KTCYx(20);
		}
	
		if(step2)
					/*If the step2 equal to 1,that's mean the car will stop and ring the buzzer and flash the led.
					Otherwise,this part of program will not be implemented*/
		{		

				delay(10);		//A long delay for 1 s approximately so that the car will stop.
				LED=~LED; 		//Flash LED 
				Delay10KTCYx(20);	//A short delay for 40 ms approximately. 
				BUZZER=~BUZZER;  	//Ring buzzer.
				LED=0;
				BUZZER=0
				step3=1;		//Set step3 to 1
				step2=0;		//Set step2 to 0
		}	
		

	
		if(step3)
					/*If the step3 equal to 1,that's mean the car will run forward until the distance between object is nearer than 15.
					Otherwise,this part of program will not be implemented*/
		{		
				
				PORTD=0x11;
				Delay10KTCYx(10);
				PORTD=0x99;
				Delay10KTCYx(10);
				PORTD=0x88;
				Delay10KTCYx(10);
				PORTD=0xaa;
				Delay10KTCYx(10);
				PORTD=0x22;
				Delay10KTCYx(10);
				PORTD=0x66; 
				Delay10KTCYx(10);
				PORTD=0x44; 
				Delay10KTCYx(10);
				PORTD=0x55;
				Delay10KTCYx(10);	
				if((adc_convert()>Distance15)){step4=1;step3=0;}/*If ADC find the distance between car and object is 
										nearer than 15,the car will stop run forward*/
		}		
		

		if(step4)
		{	
					/*If the step4 equal to 1,that's mean the car will continue run forward 
					and the LED and the buzzer should start responding in 
					a progressively increasing rate to indicate the approach of the vehicle towards the target,
					this part of program will not be implemented*/

				PORTD=0x11;/*When a code of stepper motor implemented,the value of number will increase 1 
					    so that we can get the distance between object and car.*/
				number++;//1
				varbeep(number);/*According to the value of number,this function will choose appropriate delays for each stepper codes so that the LED 					and the buzzer will responding in a progressively increasing rate to indicate the approach of the vehicle towards the target*/ 
				
				PORTD=0x99;
				number++;//2
				varbeep(number);
				
				PORTD=0x88;
				number++;//3
				varbeep(number);
				
				PORTD=0xaa;
				number++;//4
				varbeep(number);
				
				
				PORTD=0x22;
				number++;//5
				varbeep(number);
				
				PORTD=0x66; 
				number++;//6
				varbeep(number);
				
				PORTD=0x44; 
				number++;//7
				varbeep(number);
				
				PORTD=0x55;
				number++;//8
				varbeep(number);
				
		}	
	}
			
}	
		

	
/********************************************************/	


void INT0_ISR (void) 	// This is the interrupt service routine
{
	INTCONbits.INT0IE = 0; 	// Disable any interrupt within interrupts

	PORTD = 0X00;  		// Clear Port D
	
	
	LED=~LED;
	Delay10KTCYx(15);//The duration of delay is 40ms approximately
	BUZZER=~BUZZER;

			// Clear Port D		// Delay for a while so results can be seen 
	INTCONbits.INT0IE = 1;  //  Re-enable INT0 Interrupts
	INTCONbits.INT0IF = 0;	//	Clear INT0 Flag before returning to the main program

}/*	Interrupt routine INT0_ISR follows bellow: */




int adc_convert( void)
{
	
	unsigned int ADCValue = 0;//Initialize the ADCValue which used to store the value from sensor.
	Delay10KTCYx(30);	  //Delay for 60 ms approximately so that sensor will give stable output to ADC.
	/*configuration ADC*/
	OpenADC(ADC_FOSC_32 & ADC_RIGHT_JUST & ADC_20_TAD,ADC_CH0 & ADC_INT_OFF & ADC_VREFPLUS_VDD & ADC_VREFMINUS_VSS, 0b1011);	
	
	SetChanADC(ADC_CH0);//Set ADC channal 0
	ConvertADC();	    //Start convert 
	while(BusyADC( ));  //Wait until ADC is free.
	ADCValue = ReadADC();//Put the value to ADCValue  
	CloseADC();	     //Close ADC
	return ADCValue;     //Return ADCValue as result.
	
}
/*********************************************************/
void delay(int y)//100MS
{

	while(y--)
	{
		Delay10KTCYx(38);//The duration of delay is 100ms approximately
	}
	
	
}

void beep_led(int x)//The LED and the buzzer will responding according to the input parameter.
{
	LED=~LED;
	BUZZER=~BUZZER;	
	delay(x);		
}

void varbeep(int z)
{
	/*According to the input value,this function will choose appropriate delays for each stepper codes so that the LED and the buzzer will responding in a progressively increasing rate to indicate the approach of the vehicle towards the target*/ 	
	if( (z>=1) && (z<25)  )
	{
		beep_led(4);//The duration of delay is 400ms approximately
	}

	if( (z>=26) && (z<=60)  )
	{
		beep_led(2);//The duration of delay is 200ms approximately
	}

	if( (z>=66)&&(z<=100)  )
	{
		beep_led(1);//The duration of delay is 100ms approximately
	}	
		
	if( (z>=101)&&(z<=130)  )
	{
		LED=~LED;
		BUZZER=~BUZZER;	
		Delay10KTCYx(15);//The duration of delay is 40ms approximately
	}	
	
	
}