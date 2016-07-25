#include <Servo.h>

// create servo objects
Servo leftMotor;
Servo rightMotor;

const int serialPeriod = 250;       // only print to the serial console every 1/4 second
unsigned long timeSerialDelay = 0;

const int loopPeriod = 20;          // a period of 20ms = a frequency of 50Hz
unsigned long timeLoopDelay   = 0;

// specify the trig & echo pins used for the ultrasonic sensors
const int ultrasonic2TrigPin = 8;
const int ultrasonic2EchoPin = 9;
const int ultrasonic2TrigPinR = 10;
const int ultrasonic2EchoPinR = 11;
const int ultrasonic2TrigPinL = 2;
const int ultrasonic2EchoPinL = 3;


int ultrasonic2Distance;
int ultrasonic2Duration;
int ultrasonic2DistanceR;
int ultrasonic2DurationR;
int ultrasonic2DistanceL;
int ultrasonic2DurationL;

int numTurns=0;

// define the states
#define DRIVE_FORWARD             0
#define TURN_LEFT                 1
#define TURN_RIGHT                2
#define TURN_SLIGHTLY_LEFT        3
#define TURN_SLIGHTLY_RIGHT       4


int state = DRIVE_FORWARD; // 0 = drive forward (DEFAULT), 1 = turn left

void setup()
{
    Serial.begin(9600);
  
    // ultrasonic sensor pin configurations
    pinMode(ultrasonic2TrigPin, OUTPUT);
    pinMode(ultrasonic2EchoPin, INPUT);
    
        pinMode(ultrasonic2TrigPinR, OUTPUT);
    pinMode(ultrasonic2EchoPinR, INPUT);
    
        pinMode(ultrasonic2TrigPinL, OUTPUT);
    pinMode(ultrasonic2EchoPinL, INPUT);
    
    leftMotor.attach(13);
    rightMotor.attach(12);
}


void loop()
{


    debugOutput(); // prints debugging messages to the serial console
    
    if(millis() - timeLoopDelay >= loopPeriod)
    {
        readUltrasonicSensors(); // read and store the measured distances
        
        stateMachine();
        
        timeLoopDelay = millis();
    }
}


void stateMachine()
{
    if(state == DRIVE_FORWARD) // no obstacles detected
    {
        if(ultrasonic2Distance < 10 && ultrasonic2DistanceR < 10 && ultrasonic2DistanceL<10) // if there's nothing in front of us (note: ultrasonicDistance will be negative for some ultrasonics if there's nothing in range)
        {
            // drive forward
           rightMotor.write(180);
           leftMotor.write(65);
        }
        else if (ultrasonic2Distance>=10 && numTurns==0) // there's an object in front of us
        {
            numTurns++;
            state = TURN_LEFT;
        }
        else if (ultrasonic2Distance>=10 && numTurns==1)
        {
          state=TURN_RIGHT; 
          numTurns++;
        }
        else if (ultrasonic2DistanceR>=10)
        {
          state=TURN_SLIGHTLY_LEFT;
        }
        else if (ultrasonic2DistanceL>=10)
        {
          state=TURN_SLIGHTLY_RIGHT;
        }
        
        
    }
    else if(state == TURN_LEFT) // obstacle detected -- turn left
    {
        unsigned long timeToTurnLeft = 500; // it takes around .5 seconds to turn 90 degrees
        
        unsigned long turnStartTime = millis(); // save the time that we started turning

        while((millis()-turnStartTime) < timeToTurnLeft) // stay in this loop until timeToTurnLeft (.5 seconds) has elapsed
        {
            // turn left
            rightMotor.write(180);
            leftMotor.write(180);
        }
        
        state = DRIVE_FORWARD;
    }
    else if(state == TURN_RIGHT) // obstacle detected -- turn left
    {
        unsigned long timeToTurnRIGHT = 500; // it takes around .5 seconds to turn 90 degrees
        
        unsigned long turnStartTime = millis(); // save the time that we started turning

        while((millis()-turnStartTime) < timeToTurnRIGHT) // stay in this loop until timeToTurnRIGHT (.5 seconds) has elapsed
        {
            // turn right
            rightMotor.write(0);
            leftMotor.write(0);
        }
        
        state = DRIVE_FORWARD;
    }
    else if (state == TURN_SLIGHTLY_LEFT)
    {
               unsigned long timeToTurnLeft = 100; // it takes around .5 seconds to turn 90 degrees
        
        unsigned long turnStartTime = millis(); // save the time that we started turning

        while((millis()-turnStartTime) < timeToTurnLeft) // stay in this loop until timeToTurnLeft (.5 seconds) has elapsed
        {
            // turn left
            rightMotor.write(180);
            leftMotor.write(180);
        }
        
        state = DRIVE_FORWARD;
    }
    else if (state==TURN_SLIGHTLY_RIGHT)
    {
              unsigned long timeToTurnRIGHT = 100; // it takes around .5 seconds to turn 90 degrees
        
        unsigned long turnStartTime = millis(); // save the time that we started turning

        while((millis()-turnStartTime) < timeToTurnRIGHT) // stay in this loop until timeToTurnRIGHT (.5 seconds) has elapsed
        {
            // turn right
            rightMotor.write(0);
            leftMotor.write(0);
        }
        
        state = DRIVE_FORWARD;
    }
}


void readUltrasonicSensors()
{
    // ultrasonic 2
    digitalWrite(ultrasonic2TrigPin, HIGH);
    delayMicroseconds(10);                  // must keep the trig pin high for at least 10us
    digitalWrite(ultrasonic2TrigPin, LOW);
    
    ultrasonic2Duration = pulseIn(ultrasonic2EchoPin, HIGH);
    ultrasonic2Distance = (ultrasonic2Duration/2)/29;
    
        // ultrasonic 2 R
    digitalWrite(ultrasonic2TrigPinR, HIGH);
    delayMicroseconds(10);                  // must keep the trig pin high for at least 10us
    digitalWrite(ultrasonic2TrigPinR, LOW);
    
    ultrasonic2DurationR = pulseIn(ultrasonic2EchoPinR, HIGH);
    ultrasonic2DistanceR = (ultrasonic2DurationR/2)/29;
    
            // ultrasonic 2 L
    digitalWrite(ultrasonic2TrigPinL, HIGH);
    delayMicroseconds(10);                  // must keep the trig pin high for at least 10us
    digitalWrite(ultrasonic2TrigPinL, LOW);
    
    ultrasonic2DurationL = pulseIn(ultrasonic2EchoPinL, HIGH);
    ultrasonic2DistanceL = (ultrasonic2DurationL/2)/29;
}
//

void debugOutput()
{
    if((millis() - timeSerialDelay) > serialPeriod)
    {
        Serial.print("ultrasonic2Distance: ");
        Serial.print(ultrasonic2DistanceL);
        Serial.print("cm ");
        Serial.println(numTurns);
        
        timeSerialDelay = millis();
    }
}
