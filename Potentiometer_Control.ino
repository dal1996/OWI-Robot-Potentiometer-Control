#define ADC_REF 5 //Reference voltage of ADC is 3.3v
#define FULL_ANGLE 270.0 //Full value of the rotary angle is 300 degrees


//----------------- Grip stuff --------------------
const int gripPot = 0;      // Brown wire
const int gripEnable = 43;        // Digital Pin number for grip enable // Blue
const int gripDirPin = 30;        // Pin number for grip direction pin  // Blue // HIGH = OPEN, LOW = CLOSE
int gripMax = 475;                  // Open
int gripMin = 270;                  // Closed

//----------------- Wrist stuff -------------------
const int m1Pot = 4;        // Green wire
const int wristEnable = 42;       // Digital Pin number for wrist enable // Green
const int wristDirPin = 28;       // Pin number for wrist direction pin // Purple // HIGH = UP , LOW = DOWN
int wristMax = 856;                 // Going down
int wristMin = 0;                   // Goin up

//------------------ Elbow stuff ------------------
const int m2Pot = 1;        // Red wire
const int elbowEnable = 41;       // Digital Pin for elbow enable // Gray
const int elbowDirPin = 26;       // Pin number for elbow direction pin // Gray // 
int elbowMax = 994;                 // Going down
int elbowMin = 124;                 // Going up

//------------------ Shoulder stuff ------------------
const int m3Pot = 3;        // Yellow wire
const int shoulderEnable = 40;    // Digital Pin for shoulder enable // Purple
const int shoulderDirPin = 24;    // Pin number for shoulder direction pin // White
int shoulderMax = 782;              // Going up
int shoulderMin = 176;              // Going down

//------------------- Waist stuff -------------------
const int m4Pot = 2;        // Orange wire
const int waistEnable = 39;       // Digital Pin for waist enable // Orange
const int waistDirPin = 22;       // Pin number for waist direction pin // Black
int waistMax = 825;                 // Going left
int waistMin = 90;                  // Going right

//------------------- LED stuff ----------------------
const int LEDEnable = 38;         // Choose 1 or 0 to turn on or off  // Yellow

// Raw Analog values
int gripVal = 0;
int m1Val = 0;
int m2Val = 0;
int m3Val = 0;
int m4Val = 0;

// Degree values from analogToDegree()
int gripDeg;
int m1Deg;
int m2Deg;
int m3Deg;
int m4Deg;

// Home base: wrist: 411 | elbow: 970 | Shoulder: 675 | Base: 422
// Grab: wrist: 790 | elbow: 899 | Shoulder: 469 | Base: 368
// Lift up: wrist: 698 | Elbow: 900 | : Shoulder: 604
// Rotate: waist: 253
// Place: Shoulder: 458
// Come up: wrist: 801 | elbow: 838 | Shoulder: 525 | waist: 253
// Back to base:
// Rinse and repeat

int count = 0;



void setup() {
  // Serial monitor
  Serial.begin(9600);
  
  // Enable Pins
  pinMode(gripEnable, OUTPUT);
  pinMode(wristEnable, OUTPUT);
  pinMode(elbowEnable, OUTPUT);
  pinMode(shoulderEnable, OUTPUT);
  pinMode(waistEnable, OUTPUT);
  pinMode(LEDEnable, OUTPUT);

  // Direction Pins
  pinMode(gripDirPin, OUTPUT);
  pinMode(wristDirPin, OUTPUT);
  pinMode(elbowDirPin, OUTPUT);
  pinMode(shoulderDirPin, OUTPUT);
  pinMode(waistDirPin, OUTPUT);
  
}

void loop() {
  
  gripVal = valAvg(gripPot);
  m1Val = valAvg(m1Pot);
  m2Val = valAvg(m2Pot);
  m3Val = valAvg(m3Pot);
  m4Val = valAvg(m4Pot);
  gripDeg = analogToDegree(gripVal,120,475, 275);
  m1Deg = analogToDegree(m1Val, 120, 856,0);
  m2Deg = analogToDegree(m2Val,180,994,124);
  m3Deg = analogToDegree(m3Val,270,782,176);
  m4Deg = analogToDegree(m4Val,180,825,90);

  while(count < 1)
  {
    homeBase();
    goPickUp();
    closeGrip();
    liftUp();
    moveWaist(584);
    dropOff();
    openGrip();
    liftUp();
    
    count++;
  }

//  Serial.print("Grip: ");
//  Serial.print(gripVal);
//  Serial.print(" | ");
//  
//  Serial.print("M1: ");
//  Serial.print(m1Val);
//  Serial.print(" | ");
//
//  Serial.print("M2: ");
//  Serial.print(m2Val);
//  Serial.print(" | ");
//
//  Serial.print("M3: ");
//  Serial.print(m3Val);
//  Serial.print(" | ");
// 
//  Serial.print("M4: ");
//  Serial.print(m4Val);
//  Serial.print(" | ");
//
//  Serial.print("\n");


  
}


// Get the averaged reading of which ever analog pin you want
int valAvg(const int pin)
{
  int i = 0;
  int sum = 0;
  int average = 0;
  while(i < 5)
  {
    sum += analogRead(pin);
    i++;
  }
  
  return average=sum/i;
}

void closeGrip()
{
  Serial.println("Closing grip");
  int currPosition = valAvg(gripPot);
  int lastPosition = currPosition;
  if(currPosition > 229)
  {
      while(currPosition > 229)
      {
        lastPosition = currPosition;
        gripVal = valAvg(gripPot);
        Serial.print("Grip:");
        Serial.print(gripVal);
        Serial.print(" | Closing");
        Serial.print("\n");
        digitalWrite(gripDirPin, LOW);
        digitalWrite(gripEnable, HIGH);
    
        delay(200);
        
        
        delay(500);
        currPosition = valAvg(gripPot);
        digitalWrite(gripEnable, LOW);
        
        if(currPosition == lastPosition)
        {
          
          break;
        }

        
      }
  }
  
}

void openGrip()
{
  Serial.println("Opening");
  int currPosition = valAvg(gripPot);
  if(currPosition < 385)
  {
      while(currPosition < 385)
      {
        gripVal = valAvg(gripPot);
        Serial.print("Grip:");
        Serial.print(gripVal);
        Serial.print(" | Opening");
        Serial.print("\n");
        digitalWrite(gripDirPin, HIGH);
        digitalWrite(gripEnable, HIGH);
    
        delay(200);
        digitalWrite(gripEnable, LOW);
        currPosition = valAvg(gripPot);
      }
  }
  
}

// Analog is 1023 max and 0 min
// Ratio of degMax / analogMax of joint
// 
int analogToDegree(int analogVal, int degMax, int analogMax, int analogMin)
{
  int potMax = 270;                               // 
  int potMin = 0;
  int degMin = 0; 
  int degreeMiddle = (degMax + degMin)/2;         // Middle degree position of pot
  int analogMiddle = (analogMax + analogMin)/2;   // 90 degree position (middle)
  int foundDegree;

  float analogToDegRatio = degMax / analogMax;
  foundDegree = analogVal*analogToDegRatio;
  return foundDegree;
}

//int getDegree(int val)
//{
//   // Read the raw sensor value
//   int sensor_value = val;
//
//   // Convert the sensor reading to degrees and return that value
//   int 
//   
//   
//   float voltage = (float)sensor_value * ADC_REF / 1023; 
//   float degrees = (voltage * FULL_ANGLE) / ADC_REF;
//   float realDegrees = degrees
//   return realDegrees;
//}

void moveWaist(int point)
{
    Serial.print("Moving Waist to: ");
    Serial.println(point);
    int currPosition = valAvg(m4Pot);
    if(currPosition < point)
    {
      
      // Moving waist left
      while(currPosition < point)  // Original 740
      {
        
        Serial.print("M4: ");
        Serial.print(currPosition);
        Serial.print(" | Left\n");
        digitalWrite(waistDirPin,LOW);
        digitalWrite(waistEnable,HIGH);
        delay(200);
        digitalWrite(waistEnable, LOW);
        
        currPosition = valAvg(m4Pot);
      }
      
      
    }

    else if(currPosition > point)
    {
      // Moving waist right
      while(currPosition > point)  // Original 100
      {
        
        Serial.print("M4: ");
        Serial.print(currPosition);
        Serial.print(" | Right\n");
        digitalWrite(waistDirPin,HIGH);
        digitalWrite(waistEnable,HIGH);
        delay(200);
        digitalWrite(waistEnable, LOW);
        currPosition = valAvg(m4Pot);

      }
    }
    
}


void moveShoulder(int point)
{
    Serial.print("Moving shoulder to: ");
    Serial.println(point);
    int currPosition = valAvg(m3Pot);
    if(currPosition < point)
    {
      
      // Moving Shoulder down
      while(currPosition < point)  // Original 740
      {
        
        Serial.print("M3: ");
        Serial.print(currPosition);
        Serial.print(" | UP\n");
        digitalWrite(shoulderDirPin,HIGH);
        digitalWrite(shoulderEnable,HIGH);
        delay(200);
        digitalWrite(shoulderEnable, LOW);

        currPosition = valAvg(m3Pot);
      }
      
      
    }
    // Moving Shoulder up
    else if(currPosition > point)
    {
      // Moving waist right
      while(currPosition > point)  // Original 100
      {
        
        Serial.print("M3: ");
        Serial.print(currPosition);
        Serial.print(" | DOWN\n");
        digitalWrite(shoulderDirPin,LOW);
        digitalWrite(shoulderEnable,HIGH);
        delay(200);
        digitalWrite(shoulderEnable, LOW);
        currPosition = valAvg(m3Pot);

      }
    }
    
}


void moveElbow(int point)
{
    Serial.print("Moving elbow to pos: ");
    Serial.println(point);
    int currPosition = valAvg(m2Pot);
    if(currPosition < point)
    {
      
      // Moving Elbow Down
      while(currPosition < point)  // Original 740
      {
        
        Serial.print("M2: ");
        Serial.print(currPosition);
        Serial.print(" | Down\n");
        digitalWrite(elbowDirPin,LOW);
        digitalWrite(elbowEnable,HIGH);
        delay(200);
        digitalWrite(elbowEnable, LOW);

        currPosition = valAvg(m2Pot);
      }
      
      
    }
    // Moving Elbow up
    else if(currPosition > point)
    {
      // Moving waist right
      while(currPosition > point)  // Original 100
      {
        
        Serial.print("M3: ");
        Serial.print(currPosition);
        Serial.print(" | UP\n");
        digitalWrite(elbowDirPin,HIGH);
        digitalWrite(elbowEnable,HIGH);
        delay(200);
        digitalWrite(elbowEnable, LOW);
        currPosition = valAvg(m2Pot);

      }
    }
    
}

void moveWrist(int point)
{
    Serial.print("Moving wrist to position: ");
    Serial.println(point);
    int currPosition = valAvg(m1Pot);
    if(currPosition < point)
    {
      
      // Moving Wrist Down
      while(currPosition < point)  // Original 740
      {
        
        Serial.print("M1: ");
        Serial.print(currPosition);
        Serial.print(" | DOWN\n");
        digitalWrite(wristDirPin,LOW);
        digitalWrite(wristEnable,HIGH);
        delay(200);
        digitalWrite(wristEnable, LOW);

        currPosition = valAvg(m1Pot);
      }
      
      
    }
    // Moving Wrist UP
    else if(currPosition > point)
    {
      // Moving wrist UP
      while(currPosition > point)  // Original 100
      {
        
        Serial.print("M1: ");
        Serial.print(currPosition);
        Serial.print(" | Up\n");
        digitalWrite(wristDirPin,HIGH);
        digitalWrite(wristEnable,HIGH);
        delay(200);
        digitalWrite(wristEnable, LOW);
        currPosition = valAvg(m1Pot);

      }
      
    }
    
}

void homeBase()
{
  Serial.println("Going to base");
  moveWaist(528);
  moveShoulder(649);
  moveElbow(944);
  moveWrist(579);
  
}

void goPickUp()
{
  Serial.println("Picking Up");
  moveWaist(446);
  moveWrist(274);
  moveElbow(929);
  moveShoulder(380);
  delay(1000);
}

void liftUp()
{
  Serial.println("Lifting");
  moveShoulder(561);
  moveElbow(944);
  moveWrist(579);
  delay(500);
}

void dropOff()
{
  Serial.println("Dropping");
  moveWaist(584);
  moveWrist(274);
  moveElbow(929);
  moveShoulder(390);
  delay(500);
}

