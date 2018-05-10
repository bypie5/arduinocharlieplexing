// Red, Anode, Blue, Green
class LED {
  public:
    LED(int ID) {
      id = ID; // Pin value of the anode
      currColor = -1; // Off
      calcPins();
    }

  private:
    int id;
    int pins[3];
    int currColor;

    // Loops around the pins to find color pins based on location of anode
    void calcPins() {
      if (id - 1 < 4) {
        pins[0] = 7;
      } else {
        pins[0] = id - 1;
      }

      if (id + 1 > 7) {
        pins[1] = 4;
      } else {
        pins[1] = id + 1;
      }

      if (id + 2 > 7) {
        pins[2] = 4;
      } else {
        pins[2] = id + 2;
      }    
    }

    void turnOff() {
      pinMode(id, INPUT);
      digitalWrite(id, LOW);
    }

  public:
    int getId() {
      return id;
    }
  
    int getPin(int index) {
      return pins[index];
    }  

    void setColor(int color) { 
      switch (color) {
        case -1:
          currColor = -1;
        break;
        
        case 0:
          currColor = pins[0];
        break;
        
        case 1:
          currColor = pins[1];
        break;
        
        case 2:
          currColor = pins[2];
        break;
      }
    }

    void draw() {
      if (currColor != -1) {
        pinMode(id, OUTPUT);
        digitalWrite(id, HIGH);
        pinMode(currColor, OUTPUT);
        digitalWrite(currColor, LOW);
        delay(1);
        pinMode(id, INPUT);
        digitalWrite(id, LOW);
        pinMode(currColor, INPUT);
        digitalWrite(currColor, HIGH);
        delay(1);
      } else {
        turnOff();
      }
    }
};

class LEDColumn {
  public:
    LEDColumn(int * pinsInUse) {
      pins = pinsInUse;
      for (int i = 0; i < 4; i++) {
        leds[i] = new LED(pins[i]);
        pinMode(pins[i], INPUT);
      }  
    }
  
  private:
    LED * leds[4];
    int * pins;

  public:
    void setLED(int index, int color) {
      leds[index]->setColor(color);
    }
  
    void updateLEDs() {
      for (int i = 0; i < 4; ++i) {
        leds[i]->draw();
        leds[i]->setColor(-1);
      }
    }

    LED** getLEDs() {
      return leds;
    }
    
};

// HC-SR04 sonar distance sensor
class Sensor {
  public:
    Sensor(int * pins) {
      trigger = pins[0];
      echo = pins[1];
    
      pinMode(trigger, OUTPUT);
      digitalWrite(trigger, LOW);
      
      pinMode(echo, INPUT);
    }

   private:
    int trigger;
    int echo;

   public:
    float maxDist = 25;
    float minDist = 3;
   
    int getDistance() {
      digitalWrite(trigger, LOW);
      delayMicroseconds(2);
      
      // 10 microseconds high
      digitalWrite(trigger, HIGH);
      delayMicroseconds(10);
      digitalWrite(trigger, LOW);
      
      long duration = pulseIn(echo, HIGH);

      int distance = duration*0.034/2;

      // Boundary conditions 
      if (distance > maxDist) {
        distance = maxDist;
      } else if (distance < minDist) {
        distance = minDist;
      }
      
      return distance;
    }
};

LEDColumn * ledcolumn;
Sensor * sensor;
void setup() {
  Serial.begin(9600);
  int colmunPins[4] = {4,5,6,7};
  ledcolumn = new LEDColumn(colmunPins);

  int sensorPins[2] = {11,12};
  sensor = new Sensor(sensorPins);
}


// int color = 0;
void loop() { 
  // Closer: low, Farther: high
  float yPos = sensor->getDistance();
  float steps = (sensor->maxDist - sensor->minDist)/4;

  // Sample granularity of 50ms
  for (int i = 0; i < 50; i++) {
    if ((yPos >= sensor->minDist || yPos >= 0) && yPos < steps) {
      ledcolumn->setLED(0, 0);
    } else if (yPos >= steps && yPos < steps*2) {
      ledcolumn->setLED(1, 0);
    } else if (yPos >= steps*2 && yPos < steps*3) {
      ledcolumn->setLED(2, 0);
    } else if (yPos >= steps*3 && yPos <= sensor->maxDist) {
      ledcolumn->setLED(3, 0);
    } else {
      // Debugging case
      ledcolumn->setLED(3, 1);
    }
    
    ledcolumn->updateLEDs();
    delay(1);
  }
}
