    #include <Adafruit_NeoPixel.h>
     
    #define PIN 6
    #define DIODE_COUNT 234
    #define ROWS 13
    #define COLUMNS 18

    #define DEBOUNCE_TIME 50

    byte sprites[4][30] = {
      {0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000011, 0b00000011, 0b00010010, 0b00010010, 0b01001000, 
      0b01001000, 0b11000000, 0b11000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00010000, 0b00100011, 
      0b01000000, 0b10001100, 0b00000000, 0b01111010, 0b00000001, 0b00110000, 0b00000011, 0b00000000, 0b00000000, 0b00000000},
      
      {0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00011110, 0b00011110, 0b01001000, 
      0b01001000, 0b11000000, 0b11000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00010000, 0b00100011, 
      0b01000000, 0b10001100, 0b00000000, 0b01111010, 0b00000001, 0b00110000, 0b00000011, 0b00000000, 0b00000000, 0b00000000},
      
      {0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00010010, 0b00010010, 0b01111000, 
      0b01111000, 0b11000000, 0b11000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00010000, 0b00100011, 
      0b01000000, 0b10001100, 0b00000000, 0b01111010, 0b00000001, 0b00110000, 0b00000011, 0b00000000, 0b00000000, 0b00000000},
      
      {0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00010010, 0b00010010, 0b01001000, 
      0b01001000, 0b11000000, 0b11000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00010000, 0b00100011, 
      0b01000000, 0b10001100, 0b00000000, 0b01111010, 0b00000001, 0b00110000, 0b00000011, 0b00000000, 0b00000000, 0b00000000}
      };

      //dla spriteów
      bool refresh = true;  //flaga odśeiżania obrazka
      int spriteNr = 0;
      int spriteInc = -1;
      int actDelay = 0;
      //

      //dla drawLines
        int act[2] = {0, 0};
        int dest[2] = {0, 0};
        int colorNr = 1;
        int dir = 0;
      //

      //dla ogolnego menu
        int mainOption = 0; //główne menu
        bool isMainOpt = true;  //czy jestesmy w glownym menu
        int option = 0;     //menu podrzędne dla "aplikacji"
        byte flags = 0b00000000;
      //
     
    Adafruit_NeoPixel pixels = Adafruit_NeoPixel(DIODE_COUNT, PIN, NEO_GRB + NEO_KHZ800);
    
    byte switches = 0;  //left, up, right, down, btn1, btn2
    unsigned long lastIntMillis = 0;
    int colors[16][3] = {{0, 0, 0}, {0, 0, 255}, {0, 128, 128}, {0, 255, 0}, {128, 128, 0}, {255, 0, 0}, {128, 0, 128}, {85, 85, 85}, 
                        {51, 102, 102}, {102, 51, 102}, {102, 102, 51}, {0, 85, 170}, {0, 170, 85}, {85, 170, 0}, {170, 85, 0}, {170, 0, 85}};

    void colorSingleAdd(int nr, int *color, int sat)
    {
      int tmp = ((nr / COLUMNS) % 2 == 0) ? ((DIODE_COUNT - 1) - nr) : ((DIODE_COUNT - 1) - COLUMNS * (nr / COLUMNS) - (COLUMNS - 1) + nr % COLUMNS);

        unsigned long gColor = pixels.getPixelColor(tmp);
        byte gColors[3] = {0, 0, 0};
        for(int j = 0; j < 3; j++)
        {
          for(int k = 0; k < 8; k++)
          {
            bitWrite(gColors[j], k, bitRead(gColor, j * 8 + k));
          }
        }
      
      pixels.setPixelColor(tmp, constrain(*(color) * sat / 100 + gColors[2], 0, 255), constrain(*(color + 1) * sat / 100 + gColors[1], 0, 255), constrain(*(color + 2) * sat / 100 + gColors[0], 0, 255));
    }

    void colorSingle(int nr, int *color, int sat)
    {
      int tmp = ((nr / COLUMNS) % 2 == 0) ? ((DIODE_COUNT - 1) - nr) : ((DIODE_COUNT - 1) - COLUMNS * (nr / COLUMNS) - (COLUMNS - 1) + nr % COLUMNS);
      pixels.setPixelColor(tmp, *(color) * sat / 100, *(color + 1) * sat / 100, *(color + 2) * sat / 100);
    }

    void colorHEX(byte *sprite, int *color, int sat, int *bgColor, int bgSat)
    {
      for(int i = 0; i < DIODE_COUNT; i++)
      {
        int tmp = ((i / COLUMNS) % 2 == 0) ? ((DIODE_COUNT - 1) - i) : ((DIODE_COUNT - 1) - COLUMNS * (i / COLUMNS) - (COLUMNS - 1) + i % COLUMNS);
        int tmp2 = tmp / 8;
        int tmp3 = tmp % 8;
        if(bitRead(*(sprite + tmp2), tmp3))
        pixels.setPixelColor(i, *(color) * sat / 100, *(color + 1) * sat / 100, *(color + 2) * sat / 100);
        else
        pixels.setPixelColor(i, *(bgColor) * bgSat / 100, *(bgColor + 1) * bgSat / 100, *(bgColor + 2) * bgSat / 100);
      }
    }

    void blinkingEyes(int nrColor)
    { //mrugam oczkami z losowa czestotliwoscia
      if(actDelay <= 0)
      {
        refresh = true;
        if(spriteNr % 3 == 0)
        {
        spriteInc = -spriteInc;
        }
        spriteNr += spriteInc;
        if(spriteNr != 0)
          actDelay = 100;
        else
          actDelay = (rand() % 5 + 3) * 1000;
      }

      if(refresh)
      {
      colorHEX(sprites[spriteNr], colors[nrColor], 50, colors[10], 2);
      refresh = false;
      }
      delay(20);
      actDelay -= 20;
    }

    void drawLines()
    { 
      if((act[0] == dest[0] && act[1] == dest[1]) || act[0] >= COLUMNS || act[0] < 0 || act[1] >= ROWS || act[1] < 0)
      {
        colorNr++;
        if(colorNr >= 16)
          colorNr = 1;
        if(random() % 2 == 0)
        {
          act[0] = (random() % 2) * (COLUMNS - 1);
          act[1] = random() % (ROWS - 4) + 2;
          dest[0] = random() % (COLUMNS - 4) + 2;
          dest[1] = (random() % 2) * (ROWS - 1);
          if(act[0] == 0)
            dir = 1;
          else
            dir = -1;
        }
        else
        {
          act[0] = random() % (COLUMNS - 4) + 2;
          act[1] = (random() % 2) * (ROWS - 1);
          dest[0] = (random() % 2) * (COLUMNS - 1);
          dest[1] = random() % (ROWS - 4) + 2;
          if(act[1] == 0)
            dir = 2;
          else
            dir = -2;
        }
      }
      else
      {
        if(abs(dir) == 1 && act[0] == dest[0])
        {
          if(dest[1] == 0)
            dir = -2;
          else
            dir = 2;
        }
  
        if(abs(dir) == 2 && act[1] == dest[1])
        {
          if(dest[0] == 0)
            dir = -1;
          else
            dir = 1;
        }
  
        if(dir == 1)
          act[0] = act[0] + 1;
        if(dir == -1)
          act[0] = act[0] - 1;
        if(dir == 2)
          act[1] = act[1] + 1;
        if(dir == -2)
          act[1] = act[1] - 1;
      }

      for(int i = 0; i < DIODE_COUNT; i++)
      {
        unsigned long gColor = pixels.getPixelColor(i);
        byte gColors[3] = {0, 0, 0};
        for(int j = 0; j < 3; j++)
        {
          for(int k = 0; k < 8; k++)
          {
            bitWrite(gColors[j], k, bitRead(gColor, j * 8 + k));
          }
        }
       
        pixels.setPixelColor(i, gColors[2]/1.05,  gColors[1]/1.05,  gColors[0]/1.05);
      }
      
      colorSingleAdd(act[1] * COLUMNS + act[0], colors[colorNr], 100);
      delay(100);
    }
     
    void setup()
    {
      pixels.begin(); // Inicjalizacja biblioteki

      cli();
      PCICR |= 0b00000100;
      PCMSK2 |= 0b00111111;    // turn on pins PD0, PD1, PD2, PD3, PD4, PD5, PCINT16, PCINT17, PCINT18, PCINT19, PCINT20, PCINT21 
      sei();
      Serial.begin(9600);
      randomSeed(analogRead(A7));
      DDRK = 0; // set PORTK to input
      digitalWrite(A8, HIGH);
      digitalWrite(A9, HIGH);
      digitalWrite(A10, HIGH);
      digitalWrite(A11, HIGH);
      digitalWrite(A12, HIGH);
      digitalWrite(A13, HIGH);
      digitalWrite(A14, LOW);
      digitalWrite(A15, LOW);
    }
     
    void loop()
    {
    if(isMainOpt)
      delay(20);
      
    if(bitRead(flags, 0))
    {
      if(!isMainOpt)
      {
        if(mainOption == 0)
        {
        option--;
        if(option < 0)
          option = 4;
        }
      }
      bitClear(flags, 0);
    }
    if(bitRead(flags, 1))
    {
      if(isMainOpt)
        mainOption--;
      bitClear(flags, 1);
    }
    if(bitRead(flags, 2))
    {
      if(!isMainOpt)
      {
        if(mainOption == 0)
        {
        option++;
        if(option > 4)
          option = 0;
        }
      }
      bitClear(flags, 2);
    }
    if(bitRead(flags, 3))
    {
      if(isMainOpt)
        mainOption++;
      bitClear(flags, 3);
    }
    if(bitRead(flags, 4))
    {
      if(isMainOpt)
        isMainOpt = false;
      bitClear(flags, 4);
    }
    if(bitRead(flags, 5))
    {
      if(!isMainOpt)
      {
        if(mainOption == 0 || mainOption == 1)
        {
        isMainOpt = true;
        }
      }
      bitClear(flags, 5);
    }

    if(refresh && isMainOpt)
    {
      pixels.clear();
      for(int i = 0; i < 3; i++)
      colorSingle(i + COLUMNS * mainOption, colors[mainOption + 1], 100);
      refresh = false;
    }

    if(!isMainOpt)
    {
      if(mainOption == 0)
      {
        blinkingEyes(option + 1);
      }
      if(mainOption == 1)
      {
        drawLines();
      }
    }

    pixels.show();
    }

    ISR(PCINT2_vect)
    {
      switches = PINK; // get PORTK value
      refresh = true;

      if(millis() - lastIntMillis > DEBOUNCE_TIME)
      {
      if(!bitRead(switches, 0)) //left
        bitSet(flags, 0);
      if(!bitRead(switches, 1)) //up
        bitSet(flags, 1);
      if(!bitRead(switches, 2)) //right
        bitSet(flags, 2);
      if(!bitRead(switches, 3)) //down
        bitSet(flags, 3);
      if(!bitRead(switches, 4)) //btn1
        bitSet(flags, 4);
      if(!bitRead(switches, 5)) //btn2
        bitSet(flags, 5);
      }
             lastIntMillis = millis();
      Serial.println(switches, BIN);
    }
