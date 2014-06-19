#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

/******************************************************************************/
/*****************         Constants/globals     ******************************/
/******************************************************************************/
unsigned long int ShaftCountLeft = 0; //to keep track of left position encoder 
unsigned long int ShaftCountRight = 0; //to keep track of right position encoder
unsigned long int ShaftCountC = 0; //to keep track of C motor position encoder
unsigned int Degrees; //to accept angle in degrees for turning

/*The following 3 constants are the ones that
 * depend on physical properties of robot and pen.
 * These need to be caliberated accordingly.
 */
double angleres = (1.00/0.31)*(25.00/30.00);
double distres = 8.07;
int half_robot_len = 154;

unsigned char data; //to store received data from UDR1
int penpos;      // To keep track of penup/pendown
int orient;      // To keep track of the angular orientation of robot.

unsigned char queue;  // queue will contain the coming command when it comes.
int size;             //size of the queue (currently it is one or zero.


/******************************************************************************/
/*****************     Initializations of devices and ports     ***************/
/******************************************************************************/



//Configure PORTB 5 pin for servo motor 1 operation
void servo1_pin_config (void)
{
 DDRB  = DDRB | 0x20;  //making PORTB 5 pin output
 PORTB = PORTB | 0x20; //setting PORTB 5 pin to logic 1
}

//Configure PORTB 6 pin for servo motor 2 operation
void servo2_pin_config (void)
{
 DDRB  = DDRB | 0x40;  //making PORTB 6 pin output
 PORTB = PORTB | 0x40; //setting PORTB 6 pin to logic 1
}

//Configure PORTB 7 pin for servo motor 3 operation
void servo3_pin_config (void)
{
 DDRB  = DDRB | 0x80;  //making PORTB 7 pin output
 PORTB = PORTB | 0x80; //setting PORTB 7 pin to logic 1
}



//Function to configure ports to enable robot's motion
void motion_pin_config (void) 
{
 DDRA = DDRA | 0xCF;
 PORTA = PORTA & 0x30;
 DDRL = DDRL | 0x38;   //Setting PL3, PL4 and PL5 pins as output for PWM generation
 PORTL = PORTL | 0x38; //Setting PL3, PL4 and PL5 pins as logic 1
}

//Function to configure INT4 (PORTE 4) pin as input for the left position encoder
void left_encoder_pin_config (void)
{
 DDRE  = DDRE & 0xEF;  //Set the direction of the PORTE 4 pin as input
 PORTE = PORTE | 0x10; //Enable internal pull-up for PORTE 4 pin
}

//Function to configure INT5 (PORTE 5) pin as input for the right position encoder
void right_encoder_pin_config (void)
{
 DDRE  = DDRE & 0xDF;  //Set the direction of the PORTE 5 pin as input
 PORTE = PORTE | 0x20; //Enable internal pull-up for PORTE 5 pin
}

//Function to configure INT6 (PORTE 6) pin as input for the C position encoder
void C_encoder_pin_config (void)
{
 DDRE  = DDRE & 0xBF;  //Set the direction of the PORTE 6 pin as input
 PORTE = PORTE | 0x40; //Enable internal pull-up for PORTE 6 pin
}

//Function to initialize ports
void init_ports()
{
 motion_pin_config();
  left_encoder_pin_config(); //left encoder pin config
 right_encoder_pin_config(); //right encoder pin config	
 C_encoder_pin_config(); //right encoder pin config	
 servo1_pin_config(); //Configure PORTB 5 pin for servo motor 1 operation
 servo2_pin_config(); //Configure PORTB 6 pin for servo motor 2 operation 
 servo3_pin_config(); //Configure PORTB 7 pin for servo motor 3 operation  

}


//Function To Initialize UART2
// desired baud rate:9600
// actual baud rate:9600 (error 0.0%)
// char size: 8 bit
// parity: Disabled
void uart0_init(void)
{
 UCSR0B = 0x00; //disable while setting baud rate
 UCSR0A = 0x00;
 UCSR0C = 0x06;
 UBRR0L = 0x47; //set baud rate lo
 UBRR0H = 0x00; //set baud rate hi
 UCSR0B = 0x98;
}


//TIMER1 initialization in 10 bit fast PWM mode  
//prescale:256
// WGM: 7) PWM 10bit fast, TOP=0x03FF
// actual value: 42.187Hz 
void timer1_init(void)
{
 TCCR1B = 0x00; //stop
 TCNT1H = 0xFC; //Counter high value to which OCR1xH value is to be compared with
 TCNT1L = 0x01;	//Counter low value to which OCR1xH value is to be compared with
 OCR1AH = 0x03;	//Output compare Register high value for servo 1
 OCR1AL = 0xFF;	//Output Compare Register low Value For servo 1
 OCR1BH = 0x03;	//Output compare Register high value for servo 2
 OCR1BL = 0xFF;	//Output Compare Register low Value For servo 2
 OCR1CH = 0x03;	//Output compare Register high value for servo 3
 OCR1CL = 0xFF;	//Output Compare Register low Value For servo 3
 ICR1H  = 0x03;	
 ICR1L  = 0xFF;
 TCCR1A = 0xAB; /*{COM1A1=1, COM1A0=0; COM1B1=1, COM1B0=0; COM1C1=1 COM1C0=0}
 					For Overriding normal port functionality to OCRnA outputs.
				  {WGM11=1, WGM10=1} Along With WGM12 in TCCR1B for Selecting FAST PWM Mode*/
 TCCR1C = 0x00;
 TCCR1B = 0x0C; //WGM12=1; CS12=1, CS11=0, CS10=0 (Prescaler=256)
}


void left_position_encoder_interrupt_init (void) //Interrupt 4 enable
{
 cli(); //Clears the global interrupt
 EICRB = EICRB | 0x02; // INT4 is set to trigger with falling edge
 EIMSK = EIMSK | 0x10; // Enable Interrupt INT4 for left position encoder
 sei();   // Enables the global interrupt 
}

void right_position_encoder_interrupt_init (void) //Interrupt 5 enable
{
 cli(); //Clears the global interrupt
 EICRB = EICRB | 0x08; // INT5 is set to trigger with falling edge
 EIMSK = EIMSK | 0x20; // Enable Interrupt INT5 for right position encoder
 sei();   // Enables the global interrupt 
}

void C_position_encoder_interrupt_init (void) //Interrupt 6 enable
{
 cli(); //Clears the global interrupt
 EICRB = EICRB | 0x20; // INT6 is set to trigger with falling edge
 EIMSK = EIMSK | 0x40; // Enable Interrupt INT6 for right position encoder
 sei();   // Enables the global interrupt 
}


void init_devices (void) //use this function to initialize all devices
{
 cli(); //disable all interrupts
 init_ports();
 left_position_encoder_interrupt_init();
 right_position_encoder_interrupt_init();
 C_position_encoder_interrupt_init();
 timer1_init();
 uart0_init(); //Initailize UART1 for serial communiaction
 sei(); //re-enable interrupts
}


/******************************************************************************/
/*************************     Robot Motion code     **************************/
/******************************************************************************/


//Function used for setting motor's direction
void motion_set (unsigned char ucDirection)
{
 unsigned char ucPortARestore = 0;

 ucDirection &= 0xCF; // removing upper 6-bits for the protection
 ucPortARestore = PORTA; // reading the PORTA original status
 ucPortARestore &= 0x30; // making upper 6-bits to 0
 ucPortARestore |= ucDirection; // adding lower nibbel for forward command and restoring the PORTA status
 PORTA = ucPortARestore; // executing the command
}

void forwardC(void) 
{
  motion_set(0x09);
}

void backC(void)  
{
  motion_set(0x06);  
}

void forwardB(void) 
{
  motion_set(0x42);
}

void backB(void)  
{
  motion_set(0x81);  
}

void forwardA(void) 
{
  motion_set(0x84);
}

void backA(void)  
{
  motion_set(0x48);  
}



void rotate_left (void)
{
	motion_set(0x45);
}

void rotate_right (void)
{
	motion_set(0x8A);
}

void stop(void)
{
  motion_set(0x00);
}


//ISR for right position encoder
ISR(INT5_vect)  
{
 ShaftCountRight++;  //increment right shaft position count
}


//ISR for left position encoder
ISR(INT4_vect)
{
 ShaftCountLeft++;  //increment left shaft position count
}

ISR(INT6_vect)
{
 ShaftCountC++;  //increment left shaft position count
}




//Function used for turning robot by specified degrees
void angle_rotate(unsigned int Degrees)
{
 float ReqdShaftCount = 0;
 unsigned long int ReqdShaftCountInt = 0;

 ReqdShaftCount = (float) Degrees/ angleres; // division by resolution to get shaft count
 ReqdShaftCountInt = (unsigned int) ReqdShaftCount;
 ShaftCountRight = 0; 
 ShaftCountLeft = 0; 
ShaftCountC = 0;
 
 while (1)
 {
  if((ShaftCountRight >= ReqdShaftCountInt) || (ShaftCountLeft >= ReqdShaftCountInt))
  break;
 }
 //stop(); //Stop robot
}

//Function used for moving robot forward by specified distance

void linear_distance_mm(unsigned int DistanceInMM)
{
 float ReqdShaftCount = 0;
 unsigned long int ReqdShaftCountInt = 0;

 ReqdShaftCount = DistanceInMM / distres; // division by resolution to get shaft count
 ReqdShaftCountInt = (unsigned long int) ReqdShaftCount;
  
 ShaftCountRight = 0;
 ShaftCountLeft = 0;
 ShaftCountC = 0;
 while(1)
 {
  if(((ShaftCountRight > ReqdShaftCountInt) || (ShaftCountLeft > ReqdShaftCountInt)) || (ShaftCountC > ReqdShaftCountInt))
  {
  	break;
  }
 } 
 stop(); //Stop robot
}


/******************************************************************************/
/*********************     Servo motors/penup pendown code     ****************/
/******************************************************************************/



//Function to rotate Servo 1 by a specified angle in the multiples of 2.25 degrees
void servo_1(unsigned char degrees)  
{
 float PositionPanServo = 0;
 PositionPanServo = ((float)degrees / 2.25) + 21.0;
 OCR1AH = 0x00;
 OCR1AL = (unsigned char) PositionPanServo;
}


//Function to rotate Servo 2 by a specified angle in the multiples of 2.25 degrees
void servo_2(unsigned char degrees)
{
 float PositionTiltServo = 0;
 PositionTiltServo = ((float)degrees / 2.25) + 21.0;
 OCR1BH = 0x00;
 OCR1BL = (unsigned char) PositionTiltServo;
}

//Function to rotate Servo 3 by a specified angle in the multiples of 2.25 degrees
void servo_3(unsigned char degrees)
{
 float PositionServo = 0;
 PositionServo = ((float)degrees / 2.25) + 21.0;
 OCR1CH = 0x00;
 OCR1CL = (unsigned char) PositionServo;
}

//servo_free functions unlocks the servo motors from the any angle 
//and make them free by giving 100% duty cycle at the PWM. This function can be used to 
//reduce the power consumption of the motor if it is holding load against the gravity.

void servo_1_free (void) //makes servo 1 free rotating
{
 OCR1AH = 0x03; 
 OCR1AL = 0xFF; //Servo 1 off
}

void servo_2_free (void) //makes servo 2 free rotating
{
 OCR1BH = 0x03;
 OCR1BL = 0xFF; //Servo 2 off
}

void servo_3_free (void) //makes servo 3 free rotating
{
 OCR1CH = 0x03;
 OCR1CL = 0xFF; //Servo 3 off
} 

void penup(){
  servo_1(0);
  _delay_ms(300);
  servo_2(0);
  _delay_ms(300);
  servo_3(0);
  _delay_ms(300);

}

void pendown(){
  servo_1(360);
  _delay_ms(300);
  servo_2(360);
  _delay_ms(300);
  servo_3(360);
  _delay_ms(300);


}


/******************************************************************************/
/************************     Zigbee recieving code     ***********************/
/******************************************************************************/


SIGNAL(SIG_USART0_RECV) 		// ISR for receive complete interrupt
{	
	penup();
	
	data = UDR0; 				//making copy of data from UDR0 in 'data' variable 
	
	UDR0 = data; 				//echo data back to PC
    queue=data;
	size++;
}


/******************************************************************************/
/************************     Executing the command     ***********************/
/******************************************************************************/

void cmd(unsigned char data){
	unsigned char comm = data & 0xe0;  //getting the command
	unsigned char val = data & 0x1f;   //getting the value
	if(comm == 0x00){                  //orient +ve
		backC();
		linear_distance_mm(half_robot_len);
		if(val>orient){
		     	rotate_right();	
	    		angle_rotate(val-orient);
		}
		else{
		     	rotate_left();	
	    		angle_rotate(orient-val);
		
		}
		orient = val;
		forwardC();
		linear_distance_mm(half_robot_len);

	}
	else if (comm == 0x20){                //orient -ve
		backC();
		linear_distance_mm(half_robot_len);
		if(val> (-orient)){
		     	rotate_left();	
	    		angle_rotate(val+orient);
		}
		else{
		     	rotate_right();	
	    		angle_rotate(-orient-val);
		
		}
		orient = -val;
		forwardC();
		linear_distance_mm(half_robot_len);
	}
	else if (comm == 0x40){                    // forward A
		if (penpos==0) pendown();
		forwardA();
		linear_distance_mm(val);
		penpos=0;
	
	}
	else if (comm == 0x60){                       //backward A
		if (penpos==0) pendown();
		backA();
		linear_distance_mm(val);
		penpos=0;
	
	}
	else if (comm == 0x80){                          // forward B
		if (penpos==0) pendown();
		forwardB();
		linear_distance_mm(val);
		penpos=0;
	
	}
	else if (comm == 0xa0){	                           //backward B
		if (penpos==0) pendown();
		backB();
		linear_distance_mm(val);
		penpos=0;
	
	}
	else if (comm == 0xc0){                       // forward C
		if (penpos==0) pendown();
		forwardC();
		linear_distance_mm(val);
		if(val==0){
			penpos =1;
		}
		else{
			penpos=0;
		}
	}
	else if (comm == 0xe0){                   //backward C
		if (penpos==0) pendown();
		backC();
		linear_distance_mm(val);
		penpos=0;
			
	}
		
}

/******************************************************************************/
/**************************     Main Function     *****************************/
/******************************************************************************/


int main()
{
 size=0;penpos=0;orient=0;
 init_devices();
 pendown();
 stop();



 while(1){
 	if(size>0) {
		cmd(queue);
		size--;
	}
 }
}

