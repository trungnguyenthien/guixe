/**
 * @file SUltraSonic.h
 * @author TrungNguyenThien
 * @brief 
 * @version 0.1
 * @date 2021-11-21
 * 
 * @copyright Copyright (c) 2021
 * 
 */

// SUPPORT:
// HC-SR04 Ultrasonic Sensor Module
// US-015 Ultrasonic Sensor Module

class SUltraSonic
{
private:
    byte p_trig;
    byte p_echo;
    int distance;

public:
    SUltraSonic(byte trig, byte echo)
    {
        this->p_trig = trig;
        this->p_echo = echo;
    }

    void setup()
    {
        pinMode(this->p_trig, OUTPUT); // chân trig sẽ phát tín hiệu
        pinMode(this->p_echo, INPUT);  // chân echo sẽ nhận tín hiệu
    }

    void check()
    {
        unsigned long duration; // biến đo thời gian
        int distance;           // biến lưu khoảng cách

        byte trig = this->p_trig;
        byte echo = this->p_echo;

        /* Phát xung từ chân trig */
        digitalWrite(trig, 0); // tắt chân trig
        delayMicroseconds(2);
        digitalWrite(trig, 1); // phát xung từ chân trig
        delayMicroseconds(5);  // xung có độ dài 5 microSeconds
        digitalWrite(trig, 0); // tắt chân trig

        /* Tính toán thời gian */
        duration = pulseIn(echo, HIGH);        // Đo độ rộng xung HIGH ở chân echo.
        distance = int(duration / 2 / 29.412); // Tính khoảng cách đến vật.

        /* In kết quả ra Serial Monitor */
        Serial.print(distance);
        Serial.println("cm");

        this->distance = distance;
    }

    int distanceByCm()
    {
        return this->distance;
    }
};

/**
   Để đo khoảng cách, ta sẽ phát 1 xung rất ngắn (5 microSeconds - ú) từ chân Trig.
   Sau đó, cảm biến sẽ tạo ra 1 xung HIGH ở chân Echo cho đến khi nhận lại được sóng phản xạ ở pin này.
   Chiều rộng của xung sẽ bằng với thời gian sóng siêu âm được phát từ cảm biển và quay trở lại.

   Tốc độ của âm thanh trong không khí là 340 m/s (hằng số vật lý),
   tương đương với 29,412 microSeconds/cm (106 / (340*100)).

   Khi đã tính được thời gian, ta sẽ chia cho 29,412 để nhận được khoảng cách.
*/
