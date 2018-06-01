// Red, Anode, Blue, Green
class LED {
  public:
    LED(int ID, int * pins) {
      id = ID; // Pin value of the anode
      currColor = -1; // Off
      calcPins();
      pinsInUse = pins;
    }

  private:
    int id;
    int pins[3];
    int currColor;
    int * pinsInUse; // Just for context of the pin range, ie {4,5,6,7}

    // Loops around the pins to find color pins based on location of anode
    void calcPins() {
      if (id - 1 < pinsInUse[0]) {
        pins[0] =  pinsInUse[3];
      } else {
        pins[0] = id - 1;
      }
      
      if (id + 1 > pinsInUse[3]) {
        pins[1] = pinsInUse[0];
      } else {
        pins[1] = id + 1;
      }
      
      if (id + 2 > pinsInUse[3]) {
        pins[2] = pinsInUse[1];
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
        leds[i] = new LED(pins[i], pins);
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
LEDColumn * ledcolumn_0;
LEDColumn * ledcolumn_1;
LEDColumn * ledcolumn_2;
LEDColumn * ledcolumn_3;
void setup() {
  int colmunPins_0[4] = {4,5,6,7};
  int colmunPins_1[4] = {8,9,10,11};
  int colmunPins_2[4] = {12,13,14,15};
  int colmunPins_3[4] = {16,17,18,19};
  
  ledcolumn_0 = new LEDColumn(colmunPins_0);
  ledcolumn_1 = new LEDColumn(colmunPins_1);
  ledcolumn_2 = new LEDColumn(colmunPins_2);
  ledcolumn_3 = new LEDColumn(colmunPins_3);
  
  Serial.begin(9600);
}

void loop() { 
  // send data only when you receive data:
  /*if (Serial.available() > 0) {
      // xxxx   xx  xx
      // column led color
      incomingByte = Serial.read();
  }

  //unsigned char colmun
  unsigned char currLed = (incomingByte >> 2) & 0x03;
  unsigned char color = incomingByte & 0x03;
  ledcolumn->setLED(currLed, 2);
  ledcolumn->updateLEDs();*/
  ledcolumn_0->setLED(3, 2);
  ledcolumn_1->setLED(3, 2);
  ledcolumn_2->setLED(3, 2);
  ledcolumn_3->setLED(3, 2);

  ledcolumn_0->updateLEDs();
  ledcolumn_1->updateLEDs();
  ledcolumn_2->updateLEDs();
  ledcolumn_3->updateLEDs();
}
