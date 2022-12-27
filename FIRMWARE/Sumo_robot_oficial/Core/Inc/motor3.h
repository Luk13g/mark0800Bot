#ifndef MOTOR3_H_
#define MOTOR3_H_

#include<stdbool.h>

    // enable/disable flipping of motors
   // void flipLeftMotor(bool flip);
   // void flipRightMotor(bool flip);

    // set speed for left, right, or both motors
    void motor_parada(void);
    void setLeftSpeed(int speed);
    void setRightSpeed(int speed);
    void setSpeeds(int leftSpeed, int rightSpeed);
    void motor3_init();

    static inline void init()
    {
      static bool initialized = false;

      if (!initialized)
      {
        initialized = true;
        motor3_init();
      }
    }



#endif /* MOTOR3_H_ */
