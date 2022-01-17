// Define stepper motor connections: DIR = direction of the stepper motor, PUL = Pulses that will corresponds with steps of the stepper motor. 
#define DIR 2
#define PUL 3

//constant from Touchdesigner
byte delayTouchdesigner;

int steps = 1000;

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

  delayTouchdesigner = 0;
}

void loop() {
 
  if(Serial.available() > 0)
  {
    delayTouchdesigner = Serial.read(); // read the values from Touchdesigner
    
    }

    for (int i=0; i <= steps; i++)
    {
   // make a pulse signal: red signal in Touchdesigner
    digitalWrite(PUL, HIGH);
    delayMicroseconds(15*delayTouchdesigner); // delayTouchdesigner corresponds with value send from Touchdesigner
    digitalWrite(PUL, LOW);
    delayMicroseconds(15*delayTouchdesigner);
    }
  
}
