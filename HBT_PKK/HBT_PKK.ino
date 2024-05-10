#include <PS2X_lib.h>

#include<Servo.h>

#include <AFMotor.h>  



#include <AFMotor.h>  

Servo servo1;
Servo servo2;
PS2X ps2; 

#define PS2_DAT A0  
#define PS2_CMD A1
#define PS2_SEL A2
#define PS2_CLK A4

bool oneHandMode = true;

AF_DCMotor motor1(1);  // Bắn bóng
AF_DCMotor motor2(2);  // Vơ bóng
AF_DCMotor motor3(3);  // Phải 
AF_DCMotor motor4(4);  // Trái

void setup() {
  ps2.config_gamepad(PS2_CLK, PS2_CMD, PS2_SEL, PS2_DAT); 

  motor1.setSpeed(255);
  motor2.setSpeed(255);
  motor3.setSpeed(255);
  motor4.setSpeed(255);

  servo1.attach(9);
  servo2.attach(8);
}

void loop() {
  ps2.read_gamepad(); 
  delay(50);
  
  // Đổi chế độ lái khi nút L1 được nhấn
  if (ps2.ButtonPressed(PSB_L1)) {
    oneHandMode = !oneHandMode;  
    delay(100);  
  }
  
  // one hand mode
  if (oneHandMode) {
    // Chế độ lái một tay: Sử dụng joystick bên trái
    int16_t joyY = ps2.Analog(PSS_LY);  
   
    int motorSpeed = map(abs(joyY), 0, 255, 0, 255);
    int motor3Speed = joyY > 0 ? motorSpeed : -motorSpeed;
    int motor4Speed = joyY > 0 ? motorSpeed : -motorSpeed;

    // Điều khiển motor
    motor3.run(joyY > 0 ? FORWARD : BACKWARD);
    motor4.run(joyY > 0 ? FORWARD : BACKWARD);
    motor3.setSpeed(abs(motor3Speed));
    motor4.setSpeed(abs(motor4Speed));
  } else {
    // Chế độ lái hai tay: Sử dụng joystick bên phải và bên trái
    int16_t joyRightX = ps2.Analog(PSS_RX);  
    int16_t joyLeftY = ps2.Analog(PSS_LY);  

    int motorSpeedRight = map(abs(joyRightX), 0, 255, 0, 255);
    int motorSpeedLeft = map(abs(joyLeftY), 0, 255, 0, 255);
    int motor1Speed = joyRightX > 0 ? motorSpeedRight : -motorSpeedRight;
    int motor2Speed = joyLeftY > 0 ? motorSpeedLeft : -motorSpeedLeft;

    // Điều khiển motor
    motor1.run(joyRightX > 0 ? FORWARD : BACKWARD);
    motor2.run(joyLeftY > 0 ? FORWARD : BACKWARD);
    motor1.setSpeed(abs(motor1Speed));
    motor2.setSpeed(abs(motor2Speed));
  }

  // Điều khiển motor 1 bắn bóng và vơ bóng
  if (ps2.ButtonPressed(PSB_PAD_UP)) {
    motor1.run(FORWARD);
    motor1.setSpeed(255);
  } else if (ps2.ButtonPressed(PSB_PAD_DOWN)) {
    motor1.run(RELEASE); 
  }

  if (ps2.ButtonPressed(PSB_RED)) {
    motor2.run(BACKWARD);
  } else if (ps2.ButtonPressed(PSB_PINK)) {
    motor2.run(FORWARD);
  } else if (ps2.ButtonPressed(PSB_GREEN)) {
    motor2.run(RELEASE);  
  }

  // Điều khiển servo 1
  if (ps2.ButtonPressed(PSB_R1)) {
    servo1.write(90);
  } else if (ps2.ButtonPressed(PSB_R2)) {
    servo1.write(0);
  }

  // Điều khiển servo 2
  if (ps2.ButtonPressed(PSB_PAD_RIGHT)) {
    servo2.write(180);
  } else if (ps2.ButtonPressed(PSB_PAD_LEFT)) {
    servo2.write(0);
  }

  delay(50);
}
