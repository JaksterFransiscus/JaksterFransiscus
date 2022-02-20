//
// Parse incoming messages consisting of four decimal values followed by a carriage return
//  Example  "2 10000 6000 6100\n"
//  In TouchDesigner:     op('serial1').send("2 10000 6000 6100", terminator="\n")
//

#include <AccelStepper.h>

// Define pin connections
const int dirPin = 9;
const int stepPin = 8;

// Define a stepper and the pins it will use: ::DRIVER is used when you use a dedicated driver, 8 is step pin, 9 is direction
AccelStepper stepper(AccelStepper::DRIVER, stepPin, dirPin);

char buffer[32];   //maximum expected length
int len = 0; //necessary for the overflow.

bool newData, runallowed = false; // booleans for new data from serial, and runallowed flag


int movementGlobal; // 0 = stop motor, 1 = run at constant speed, 2 = run steps
int directionMotorGlobal; // 0 = CCW, 1 = CW
int speedMotorGlobal; //speed of the motor
int accelerationMotorGlobal; //acceleration of the motor
int stepsMotorGlobal; // number of steps

void setup()
{
  Serial.begin(19200); //baud rate. Should be the same in 'Serial' dat: baud rate.
  movementGlobal = 0;
  directionMotorGlobal = 0;
  speedMotorGlobal = 0;
  accelerationMotorGlobal = 0;
  stepsMotorGlobal = 0;

  stepper.setEnablePin(7); // Enable the 'Enable pin' on the driver. This will put off/on the power to the motor drive. 

  stepper.disableOutputs(); // Disable outputs, so no heating of the motor at idle

  stepper.setMaxSpeed(0); // MAX SPEED = steps/second
  stepper.setAcceleration(0); // ACCELERATION = steps/seconds²
}

void loop()
{
  checkSerial(); //check the serial communication and save variables
  controlMotor(); //run motor
}

void controlMotor() {
  if (movementGlobal == 2 && directionMotorGlobal == 1) {
    stepper.enableOutputs(); //enable output pins

    stepper.setMaxSpeed(speedMotorGlobal); // MAX SPEED = steps/second
    stepper.setAcceleration(accelerationMotorGlobal); // ACCELERATION = steps/seconds²

    stepper.moveTo(-stepsMotorGlobal); //move the motor CW to number of stepsMotorGlobal
    stepper.run();
  }
  else if (movementGlobal == 2 && directionMotorGlobal == 0) {
    stepper.enableOutputs(); //enable output pins

    stepper.setMaxSpeed(speedMotorGlobal); // MAX SPEED = steps/second
    stepper.setAcceleration(accelerationMotorGlobal); // ACCELERATION = steps/seconds²

    stepper.moveTo(stepsMotorGlobal); //move the motor CCW to number of stepsMotorGlobal
    stepper.run();
  }
  else if (movementGlobal == 1 && directionMotorGlobal == 1) {
    stepper.enableOutputs(); //enable output pins

    stepper.setAcceleration(accelerationMotorGlobal); // ACCELERATION = steps/seconds²
    stepper.setSpeed(-speedMotorGlobal); // MAX SPEED = steps/second with direction = CW

    stepper.run();
  }

  else if (movementGlobal == 1 && directionMotorGlobal == 0) {
    stepper.enableOutputs(); //enable output pins

    stepper.setAcceleration(accelerationMotorGlobal); // ACCELERATION = steps/seconds²
    stepper.setSpeed(speedMotorGlobal); // MAX SPEED = steps/second with direction = CCW

    stepper.run();
  }

  else {
    stepper.disableOutputs(); // Disable outputs, so no heating of the motor at idle

    stepper.setCurrentPosition(0); // reset position
    stepper.stop(); //stop motor
  }

}

void checkSerial() {

  if (Serial.available() > 0)
  {
    int incomingByte = Serial.read();
    buffer[len++] = incomingByte;
    newData = true; //flag
    //
    // check for overflow
    //
    if (len >= 32)
    {
      // overflow, resetting
      len = 0;
    }
    //
    // check for newline (end of message)
    //
    if (newData == true) {
      if (incomingByte == '\n')
      {
        int movement;
        int directionMotor;
        int speedMotor;
        int accelerationMotor;
        int stepsMotor;

        int n = sscanf(buffer, "%d %d %d %d %d", &movement, &directionMotor, &speedMotor, &accelerationMotor, &stepsMotor);
        if (n == 5)
        {
          movementGlobal = movement;
          directionMotorGlobal = directionMotor;
          speedMotorGlobal = speedMotor;
          accelerationMotorGlobal = accelerationMotor;
          stepsMotorGlobal = stepsMotor;

        }
        else
        {
          // parsing error, reject
        }
        len = 0; // reset buffer counter
      }
    }
  }
  newData = false;
}
