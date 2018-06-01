// Red, Anode, Blue, Green
class LED {
  public:
    LED(int ID, int * pins) {
      id = ID; // Pin value of the anode
      currColor = -1; // Off
      pinsInUse = pins;
      calcPins();
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
        if (id + 2 > pinsInUse[3] + 1)
          pins[2] = pinsInUse[1];
        else 
          pins[2] = pinsInUse[0];
         
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

class LEDMatrix {
public:
  LEDMatrix(LEDColumn** columns, int sz) {
    this->columns = columns;
    this->sz = sz;
  }

private:
  LEDColumn** columns;
  int sz;

public:
  void draw(int col, int led, int color) {
    columns[col]->setLED(led, color);
    columns[col]->updateLEDs();
  }

  // Simple display to see if matrix is working correctly
  void testv1() {
    int dly = 50;
    // Color
    for(int color = 0; color < 3; ++color) {
      // Column
      for (int col = 0; col < sz; ++col) {
        // LED
        for (int led = 0; led < 4; ++led) {
          // Delay
          for (int i = 0; i < dly; ++i) {
            draw(col, led, color);
            delay(1);
          }
        }
      }
    }
  }

  
  void testv2() {
    int dly = 50;
    // Color
    for(int color = 0; color < 3; ++color) {
        // LED
        for (int led = 0; led < 4; ++led) {
          // Delay
          for (int i = 0; i < dly; ++i) {
            draw(0, led, color);
            draw(1, led, color);
            draw(2, led, color);
            draw(3, led, color);
            delay(1);
          }
        }
    }
  }
};

int incomingByte = 0;
LEDMatrix * matrix;
void setup() {
  int colmunPins_0[4] = {4,5,6,7};
  int colmunPins_1[4] = {8,9,10,11};
  int colmunPins_2[4] = {12,13,14,15};
  int colmunPins_3[4] = {16,17,18,19};
  LEDColumn * ledcolumn_0 = new LEDColumn(colmunPins_0);
  LEDColumn * ledcolumn_1 = new LEDColumn(colmunPins_1);
  LEDColumn * ledcolumn_2 = new LEDColumn(colmunPins_2);
  LEDColumn * ledcolumn_3 = new LEDColumn(colmunPins_3);

  LEDColumn* columns[4] = {ledcolumn_0, ledcolumn_1, ledcolumn_2, ledcolumn_3};
  matrix = new LEDMatrix(columns, 4);
  
  Serial.begin(9600);
}

void loop() { 
  // send data only when you receive data:
  if (Serial.available() > 0) {
      // xxxx   xx  xx
      // column led color
      incomingByte = Serial.read();
  }

  unsigned char colmun = incomingByte >> 4;
  unsigned char currLed = (incomingByte >> 2) & 0x03;
  unsigned char color = incomingByte & 0x03;
  
  matrix->draw(colmun, currLed, color);
}
