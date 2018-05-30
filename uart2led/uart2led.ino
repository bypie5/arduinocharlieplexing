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

int incomingByte = 0;
LEDColumn * ledcolumn;
void setup() {
  int colmunPins[4] = {4,5,6,7};
  ledcolumn = new LEDColumn(colmunPins);
  
  Serial.begin(9600);
}

void loop() {

  ledcolumn->setLED(0, 0);
  ledcolumn->updateLEDs();
  
  // send data only when you receive data:
  if (Serial.available() > 0) {
      // read the incoming byte:
      incomingByte = Serial.read();

      // say what you got:
      Serial.print("I received: ");
      Serial.println(incomingByte, DEC);
  }
}
