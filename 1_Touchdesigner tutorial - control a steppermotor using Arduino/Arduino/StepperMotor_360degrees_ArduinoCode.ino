// Define stepper motor connections: DIR = direction of the stepper motor, PUL = Pulses that will corresponds with steps of the stepper motor. 
#define DIR 2
#define PUL 3

//constant from Touchdesigner
byte degreesTouchdesigner; //setpoint send by Touchdesigner

int actMotorDegrees; //actual motor position
int calMotorDegrees; //calculated motor position
int angle;
float stepPerAngle = 1.8;
int numOfSteps;

int speedMotor; //delay for a step


void setup() {
  //start serial port at 9600 bps:
  Serial.begin(9600);
  while (!Serial)
  {
    ;// wait for serial port to connect. Needed for native USB port only.
    }
  
  // Declare pins as output:
  pinMode(PUL, OUTPUT);
  pinMode(DIR, OUTPUT);

  // Setup the direction of the stepper motor: LOW = CCW, HIGH = CW: yellow signal in Touchdesigner
  digitalWrite(DIR, HIGH);

  degreesTouchdesigner = 0;
  actMotorDegrees = 0;
  calMotorDegrees = 0;
  speedMotor = 15;
}

void loop() {
  
  if(Serial.available() > 0)
  {
    degreesTouchdesigner = Serial.read(); // read the values from Touchdesigner
    if (actMotorDegrees != degreesTouchdesigner)
    {
      if (actMotorDegrees < degreesTouchdesigner)
      {
        digitalWrite(DIR,HIGH);
        calMotorDegrees = degreesTouchdesigner - actMotorDegrees;
        numOfSteps = round(calMotorDegrees/stepPerAngle);
       }
       
       else if (actMotorDegrees > degreesTouchdesigner)
       {
        digitalWrite(DIR,LOW);
        calMotorDegrees = actMotorDegrees - degreesTouchdesigner;
        numOfSteps = round(calMotorDegrees/stepPerAngle);
       }
       for (int x=0;numOfSteps; x++)
       {
        digitalWrite(PUL, HIGH);
        delayMicroseconds(speedMotor); // degreesTouchdesigner corresponds with value send from Touchdesigner
        digitalWrite(PUL, LOW);
        delayMicroseconds(speedMotor); // degreesTouchdesigner corresponds with value send from Touchdesigner
        }
        calMotorDegrees = actMotorDegrees;
      }
  }
}
