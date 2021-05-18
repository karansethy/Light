#ifndef TOWERLIGHT_H
#define TOWERLIGHT_H
#include "mbed.h"
#include "PinNames.h"
#include "comm.h"
#include "mbed_retarget.h"
#include "us_ticker_api.h"
#include <ratio>
#include <string>
#include <vector>
#include <cmath>

#ifndef M_PI
#define M_PI 3.1415
#endif


namespace towerlight
{

    //Light data connector uses P18 on adapter board.
    //Pin 3 direction1, Pin 4 direction2, Pin 6 PWM

    PwmOut brightness(PB_7); //PWM
    DigitalOut direction1(PA_13); //set L298N direction pin
    DigitalOut direction2(PA_14); //set L298N direction pin

    //READ DOCUMENTATION ON NOTION TO UNDERSTAND MATH MULTIPLIERS
    float i=0.0; //map variable
    float g=0.0; //Sine wave modifier
    float x=1.0; //Sine wave multiplier
    float h=0.0;  //Exponent Modifier
    float a=4.3;  //Exponent Multiplier
    float b=-7.8; //Exponent Multiplier
    float k=0.0; //Superposition of exponent and sine function

    float intersect1=1.615;
    float intersect2=3.52;

    float delay = 4000000.0; //Time for one 'breath' or cycle of light in seconds^6
    long current_time = 0;


    float map(float var,float x1,float x2,float y1,float y2)
    {
        return (((var-x1)/(x2-x1))*(y2  -y1)) + y1;
    }

    void init_light() {
        direction1 = 0;
        direction2 = 1;
        brightness.period(0.0001);
    }

    void enable_light() {
        brightness.write(0.95);
    }

    void disable_light() {
        brightness.write(0.0);
    }

    void update_lights()
    {
        current_time = us_ticker_read(); //Change to correct funciton
        i = map(fmod(current_time,delay),0,delay,0,intersect2);
        g = (sin(i*x) + 1.000)/2.000; // Add atleast 1.0 to ensure all values are positive; Divide by 2 to scale it down to PWM values

        if(i<=intersect1)
        {
            brightness.write(g);
            return;
        }
        else
        {
        h=(exp(-(a*i+b))+1.630)/2.000; //Inverse Exponent function; Divide by 2 to scale it down to PWM values
        brightness.write(h-g);
        return;
        }
    }
}
#endif
