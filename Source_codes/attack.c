#include <stdio.h>
#include <stdlib.h>
#include "ev3.h"
#include "ev3_port.h"
#include "ev3_tacho.h"
#include "ev3_sensor.h"
#include <unistd.h>
#define Sleep( msec ) usleep(( msec ) * 1000 )
const char const *color[] = { "?", "YELLOW" };
#define COLOR_COUNT  (( int )( sizeof( color ) / sizeof( color[ 0 ])))

//***************************************************//
uint8_t sn_sonar;
uint8_t sn_compass;
float value;
int value_color;
int val;
uint8_t sn;
uint8_t sn1;
uint8_t sn2;
uint8_t sn3;
int max_speed = 250;
float v;

//***************************************************//

int get_sonar_value(){
get_sensor_value0(sn_sonar, &value);
return value;
}

int get_compass_value(){
float cmp;
get_sensor_value0(sn_compass, &cmp );
return cmp;
}

void motor_stop(uint8_t sn){
set_tacho_stop_action_inx(sn, TACHO_HOLD);
set_tacho_command_inx(sn, TACHO_STOP);
printf("stopping/n");
}

void rotate_right(int val){
val = val+180;
if(val>=360){
int dec = val-360;
val = 0;
val = dec;
}
set_tacho_speed_sp(sn, 100);
set_tacho_speed_sp(sn1, -100);
set_tacho_time_sp(sn,50000);
set_tacho_time_sp(sn1,50000);
set_tacho_command_inx(sn,TACHO_RUN_TIMED);
set_tacho_command_inx(sn1, TACHO_RUN_TIMED);
while(1){
get_sensor_value0(sn_compass, &v );
int valu = (int)v;
printf("intital value: %d\n",valu);
//printf("turing to this angle: %d\n", meas);
printf("rotatingright");
if(valu==val||valu==val+1||valu==val-1){
set_tacho_command_inx(sn, TACHO_STOP);
set_tacho_command_inx(sn1, TACHO_STOP);
break;}
}
}

void rotate_right_dir(int val){
set_tacho_stop_action_inx(sn, TACHO_COAST);
set_tacho_stop_action_inx(sn1, TACHO_COAST);
set_tacho_speed_sp(sn, -max_speed+150);
set_tacho_speed_sp(sn1 ,max_speed-150);
set_tacho_time_sp(sn, val);
set_tacho_time_sp(sn1, val);
set_tacho_command_inx(sn, TACHO_RUN_TIMED);
set_tacho_command_inx(sn1, TACHO_RUN_TIMED);
printf("turing right\n");
}

void rotate_left_dir(int val)
{
set_tacho_stop_action_inx(sn, TACHO_COAST);
set_tacho_stop_action_inx(sn1, TACHO_COAST);
set_tacho_speed_sp(sn1, -max_speed+150);
set_tacho_speed_sp(sn ,max_speed-150);
set_tacho_time_sp(sn, val);
set_tacho_time_sp(sn1, val);
set_tacho_command_inx(sn, TACHO_RUN_TIMED);
set_tacho_command_inx(sn1, TACHO_RUN_TIMED);
printf("turingleft\n");
}

void pickup(int val){
set_tacho_stop_action_inx(sn2, TACHO_COAST);
set_tacho_speed_sp(sn2,600);
set_tacho_time_sp(sn2, val);
set_tacho_command_inx(sn2, TACHO_RUN_TIMED);
printf("picking up \n");
}
void pickdown(int val){
set_tacho_stop_action_inx(sn2, TACHO_COAST);
set_tacho_speed_sp(sn2,-600);
set_tacho_time_sp(sn2, val);
set_tacho_command_inx(sn2, TACHO_RUN_TIMED);
printf("picking down  \n");
}

int object_scan_left()
{
int angle,sensor_val, min, min_angle=0;
angle=0;
int time_dur = 0, time = 0;
min=get_sonar_value();
int min_compass = get_compass_value();
printf("min value %d\n", min);
printf("initial compass value %d\n", min);
while(angle<=2000)
{
sleep(1);
rotate_right_dir(300);
sensor_val=get_sonar_value();
time_dur+=250;
if(sensor_val<min)
{
min=sensor_val;
min_angle=angle;
time = time_dur;
min_compass = get_compass_value();
}
printf("For %d angle, value is %d\n, compass value is %d\n",angle,sensor_val,get_compass_value());
sleep(1);
angle+=350;
}
}
int object_scan_right()
{
int angle,sensor_val, min, min_angle=0;
angle=0;
int time_dur = 0, time = 0;
min=get_sonar_value();
printf("min value %d\n", min);
while(angle<=1000)
{
rotate_right_dir(150);
sensor_val=get_sonar_value();
time_dur+=150;
if(sensor_val<min-10)
{
min=sensor_val;
min_angle=angle;
time = time_dur;
}
printf("For %d angle, value is %d\n",angle,sensor_val);
sleep(1);
angle+=200;
}
printf("Minimum angle %d\n",min_angle);
printf("Minimum value %d\n\n",min);
printf("time duration %d\n", time);
int res=1000-min_angle;
printf("res %d\n", res);
rotate_left_dir(750-time);
sleep(2);
return time;
}

void throw_ball(int val){
int max_speed2;
get_tacho_max_speed(sn3,&max_speed2);
set_tacho_stop_action_inx(sn3, TACHO_COAST);
set_tacho_speed_sp(sn3,-max_speed2);
set_tacho_time_sp(sn3, val);
set_tacho_command_inx(sn3, TACHO_RUN_TIMED);
printf("lifting up \n");
}

void back_to_position(int val){
set_tacho_stop_action_inx(sn3, TACHO_COAST);
set_tacho_speed_sp(sn3,max_speed);
set_tacho_time_sp(sn3, val);
set_tacho_command_inx(sn3, TACHO_RUN_TIMED);
printf("lifting down \n");
}

void find_postion(){
set_tacho_speed_sp(sn, -250);
set_tacho_speed_sp(sn1, -250);
set_tacho_time_sp(sn,10100);
set_tacho_time_sp(sn1,10100);
set_tacho_command_inx(sn, TACHO_RUN_TIMED);
set_tacho_command_inx(sn1, TACHO_RUN_TIMED);
printf("going forward");
while(1){
      value = get_sonar_value();
      printf( "\r(%f) \n", value);
      if(value<125.00000){
      printf("APplying breaks\n");
      int val = (int)value;
      printf("value from ultrasonic sensor: %d\n",val);
      printf("MOving forward\n");
      set_tacho_command_inx(sn, TACHO_STOP);
      set_tacho_command_inx(sn1, TACHO_STOP);
      sleep(2);;
      pickup(4000);
      sleep(2);
      break;
}
      fflush( stdout );
}
}


void rotate_left(int val){
set_tacho_stop_action_inx(sn, TACHO_COAST);
set_tacho_stop_action_inx(sn1, TACHO_COAST);
set_tacho_speed_sp(sn,-max_speed);
set_tacho_speed_sp(sn1,max_speed);
set_tacho_time_sp(sn,val);
set_tacho_time_sp(sn1, val);
set_tacho_command_inx(sn,TACHO_RUN_TIMED);
set_tacho_command_inx(sn1, TACHO_RUN_TIMED);
printf("turing left\n");
}

void go_forward(int val){

set_tacho_speed_sp(sn,-800);
set_tacho_speed_sp(sn1,-800);
set_tacho_time_sp(sn,val);
set_tacho_time_sp(sn1, val);
set_tacho_command_inx(sn,TACHO_RUN_TIMED);
set_tacho_command_inx(sn1, TACHO_RUN_TIMED);
printf("forwarding\n");
}

void go_backward(int val){
set_tacho_speed_sp(sn,max_speed);
set_tacho_speed_sp(sn1,max_speed);
set_tacho_time_sp(sn,2000);
set_tacho_time_sp(sn1, 2000);
set_tacho_command_inx(sn,TACHO_RUN_TIMED);
set_tacho_command_inx(sn1, TACHO_RUN_TIMED);
printf("reverse\n");
}

void adjust_direction(int val){
set_tacho_speed_sp(sn, -80);
set_tacho_speed_sp(sn1, 80);
set_tacho_time_sp(sn,20000);
set_tacho_time_sp(sn1,20000);
set_tacho_command_inx(sn,TACHO_RUN_TIMED);
set_tacho_command_inx(sn1, TACHO_RUN_TIMED);
while(1){
printf("entered\n");
get_sensor_value0(sn_compass, &v );
int valu = (int)v;
printf("intital value: %d\n",valu);
printf("adjusting");
if(valu==val||valu==val+1||valu==val-1){
set_tacho_command_inx(sn, TACHO_STOP);
set_tacho_command_inx(sn1, TACHO_STOP);
break;}
}
}

int main( void )
{

ev3_sensor_init();
ev3_search_sensor(LEGO_EV3_US, &sn_sonar,0);
ev3_search_sensor(HT_NXT_COMPASS, &sn_compass,0);

//****************************************************//

  while ( ev3_tacho_init() < 1 ) Sleep( 1000 );

  printf( "*** ( EV3 ) Hello! ***\n" );

  printf( "Found tacho motors:\n" );

  int port1=65;
  int port2=68;
  int port3=67;
  int port4=66;

  ev3_search_tacho_plugged_in(port1,0, &sn, 0 );
  ev3_search_tacho_plugged_in(port2,0, &sn1, 0);
  ev3_search_tacho_plugged_in(port3,0, &sn2, 0);
  ev3_search_tacho_plugged_in(port4,0, &sn3, 0);

//***********************************************************//

  get_sensor_value0(sn_compass, &v );
  int initial = (int)v;
  printf("Initial position: %d\n", initial);

//*********************************************************//
//Going to middle 

/* pickdown(2000);
 sleep(5);
 go_forward(3000);
 sleep(5);
 pickup(7400);
 sleep(6);
 pickdown(4000);*/

//turn right
// rotate_right(initial);

//throw the ball

 throw_ball(100);
 sleep(5);
 back_to_position(1300);
 sleep(3);
 back_to_position(100);

//***********************************************************//

  ev3_uninit();
  printf( "*** ( EV3 ) Bye! ***\n" );

  return ( 0 );
}

