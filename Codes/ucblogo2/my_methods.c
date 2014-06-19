/*
 *      my_methods.c          logo user methods module
 *
 *      This is the module in which user can define how a line should
 *      be drawn etc.
 *
 *
 *      This program is free software; you can redistribute it and/or modify
 *      it under the terms of the GNU General Public License as published by
 *      the Free Software Foundation; either version 2 of the License, or
 *      (at your option) any later version.
 *
 *      This program is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *      GNU General Public License for more details.
 *
 *      You should have received a copy of the GNU General Public License
 *      along with this program; if not, write to the Free Software
 *      Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 */



#include "my_methods.h"
#include "logo.h"
#include "globals.h"

#include <math.h>

#include <termios.h>
//#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/signal.h>
#include <sys/types.h>

/*The peninfo struct*/
pen_info xgr_pen;
/*The file descriptor for zigbee communication*/
int fd;
/*The terminal i/o interface settings*/
struct termios oldtio,newtio;       

char* LogoPlatformName = "Firebird";

int back_ground;

int clearing_screen = 0;

/* The factor for changing between radians and degrees */
double factor = 3.14159/180;

/*The no operation. This is required because Interpretor
 * expects us to do a lot of work and we are simply drawing
 * lines. So for the rest we do nop(). See my_methods.h
 * */
void nop() {
}

/* This function is called when program is initialized.
 * In it we start the zigbee communication.
 *
 */
void my_init(){
	xgr_pen.orientation=0;
	   int  tty, c, res, i, error;
   long BAUD=B9600;
   
   long         DATABITS = CS8;
   long            STOPBITS = 0;
   long         PARITYON = 0;
   long         PARITY = 0;

  
      //open the device(com port) to be non-blocking (read will return immediately)
      fd = open("/dev/ttyUSB0", O_RDWR);
      if (fd < 0)
      {
         perror("zigbee(/dev/ttyUSB0) not found");
         exit(0);
      }

      tcgetattr(fd,&oldtio); // save current port settings 
      // set new port settings for canonical input processing 
      newtio.c_cflag = BAUD | DATABITS | STOPBITS | PARITYON | PARITY ;
      newtio.c_iflag = IGNPAR;
      newtio.c_oflag = 0;
      newtio.c_lflag = 0;   
      newtio.c_cc[VMIN]=1;
      newtio.c_cc[VTIME]=0;
      tcflush(fd, TCIFLUSH);
      tcsetattr(fd,TCSANOW,&newtio);
	
}

/*The function is called when program ends
 * It is actually closing the file descriptor and setting the olds settings
 */
void my_finish(){
      tcsetattr(fd,TCSANOW,&oldtio);
      close(fd);

}

/* The following set of functions are
 * called by the interpreter and so we
 * require them to be defined
 *
 */
void ppd(){

}
void logofill(){


}
void string_draw(unsigned char* s) {
	printf("string %s\n",s);
}

void set_palette(int n, unsigned int r, unsigned int g, unsigned int b) {

}

void get_palette(int n, unsigned int *r, unsigned int *g, unsigned int *b) {
}

void save_pen(pen_info *p) {
    memcpy(((char *)(p)),((char *)(&xgr_pen)),sizeof(pen_info));
}

void restore_pen(pen_info *p) {
    memcpy(((char *)(&xgr_pen)),((char *)(p)),sizeof(pen_info));
    set_pen_width(p->pw);
    set_pen_height(p->ph);
}


/*Now our main firebird program starts*/

/*This function returns the movement command
 * given an angle at which the robot is to be moving.
 * e.g. if the angle is 45 the robot should move
 * opposite to wheel B.
 */
unsigned char motor_print(int angle){
	if((angle<30)||(angle>=330)) { /*printf("forwardC \n");*/ return 0x06;}  //110
	if((angle<90)&&(angle>=30)) { /*printf("backwardB \n");*/ return 0x05;}  //101
	if((angle<150)&&(angle>=90)) { /*printf("forwardA \n");*/ return 0x02;}  //010
	if((angle<210)&&(angle>=150)) { /*printf("backwardC \n");*/ return 0x07;} //111
	if((angle<270)&&(angle>=210)) { /*printf("forwardB \n");*/ return 0x04;}  //100
	if((angle<330)&&(angle>=270)) { /*printf("backwardA \n");*/ return 0x03;} //011
	//printf("\n\n\n");
}

/*This function returns absolute orientation when
 * robot has to move dx in x direction and dy is y direction.
 * The angle is measured from y axis cw.
 */
int orient(int dx, int dy){
	if(dx>=0 && dy>=0) return (int)(atan((double)(dx)/(dy))/factor);
	if(dx>=0 && dy < 0) return 180+(int)(atan((double)(dx)/(dy))/factor);
	if(dx<0 && dy < 0) return 180+(int)(atan((double)(dx)/(dy))/factor);
	if(dx<0 && dy >= 0) return 360+(int)(atan((double)(dx)/(dy))/factor);

}

/*This function changes an angle
 * to a deviation of -30 to +30 for omnibot
 */
int deviation(int angle){
	return (angle%60 >= 30)? (angle%60)-60 : (angle%60);
}

/* This is the most important function.
 * It tells the interpretor what to do
 * when a line is to be drawn to (x,y)
 */
void line(int x,int y){
	int curx = xgr_pen.xpos;
	int cury = xgr_pen.ypos;
	if(curx!=x || cury!=y) {
		//printf("line %d %d \n",x-curx,cury-y);
		int orient_dev = deviation(orient(x-curx,cury-y));
		int orient_abs = orient(x-curx,cury-y);
		if (orient_dev != xgr_pen.orientation){
			//printf("orient\n");
			xgr_pen.orientation = orient_dev;
			if(orient_dev<0){
				unsigned char ff = 0x20 | ((unsigned char) abs(orient_dev) & 0x1f );
				//printf(" gg %d\n",ff);
				write(fd,&ff,1); sleep(5);
			}
			else{
				unsigned char ff = 0x00 | ((unsigned char) abs(orient_dev) & 0x1f );
				//printf("gg %d\n",ff);
				write(fd,&ff,1); sleep(5);
			}
		}
		//printf("orientation %d \n" ,orient(x-curx,cury-y));
		//printf("deviation %d \n",deviation(orient(x-curx,cury-y)));
		//printf("distance %d \n",(int)sqrt((x-curx)*(x-curx) + (y-cury)*(y-cury)));
		int d = (int)sqrt((x-curx)*(x-curx) + (y-cury)*(y-cury));
		while(d>32){
			unsigned char ff = (motor_print(orient_abs)<<5)|0x1f;
			write(fd,&ff,1); sleep(5);
			d=d-32;
		}
		//printf("move\n");
		unsigned char ff = (motor_print(orient_abs)<<5)|((unsigned char) abs(d) & 0x1f );
		//printf("gg %d\n",ff);
		write(fd,&ff,1); sleep(5);
		motor_print(orient(x-curx,cury-y));
	}
}

/* This is the most 2nd important function.
 * It tells the interpretor what to do
 * when the bot has to move to (x,y) without drawing line.
 */

void move(int x,int y){
	unsigned char spc = 0xc0;
	write(fd,&spc,1); sleep(5);
	line(x,y);
}



