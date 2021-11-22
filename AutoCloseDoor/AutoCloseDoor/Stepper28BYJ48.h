/**
 * @file Stepper28BYJ48.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2021-11-21
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <Stepper.h>

class Stepper28BYJ48
{
private:
    const int maxSpeed = 15;
    const int stepsPerRevolution = 2048;
    Stepper myStepper = Stepper(0, 0, 0, 0, 0);

public:
    Stepper28BYJ48(
        int dIn1,
        int dIn2,
        int dIn3,
        int dIn4)
    {
        myStepper = Stepper(stepsPerRevolution, dIn1, dIn3, dIn2, dIn4);
    }

    void run(float speed, float round)
    {
        if (speed > 1)
        {
            speed = 1;
        }
        myStepper.setSpeed(speed * maxSpeed);
        myStepper.step(stepsPerRevolution * round);
    }
};