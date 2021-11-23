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

/** 
 * SUPPORT: 
 * - ULN2003 Driver + 28BYJ-48 stepper
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

/** SAMPLE
 * Stepper28BYJ48 st(8, 9, 10, 11);
 * // Các chân ứng với IN1, IN2, IN3, IN4
 * 
 * Method: run(float speed, float round)
 * speed = 0..1 (1 là tốc độ max, 0.5 là 50% tốc độ)
 * round = là số kiểu float:
 *  0 là không xoay
 *  0.1 xoay 10% vòng tròn
 *  0.11 xoay 11% vòng tròn
 * -0.11 là xoay 11% vòng tròn (ngược chiều kim đồng hồ)
 *  1 xoay đúng 1 vòng tròn
 *  2 xoay đúng 2 vòng tròn
 * -2 xoay đúng 2 vòng tròn (ngược chiều kim đồng hồ)