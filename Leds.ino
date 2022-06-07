#include <Adafruit_NeoPixel.h>
     
    #define PIN 6
    #define DIODE_COUNT 234
    #define ROWS 13
    #define COLUMNS 18
    #define MAX_OPTION 3
    #define MIN_OPTION 0

    #define DEBOUNCE_TIME 50

    //buttons
    #define BTN_LEFT 0
    #define BTN_UP 1
    #define BTN_RIGHT 2
    #define BTN_DOWN 3
    #define BTN_1 4
    #define BTN_2 5

    //FunctionsIDs
    #define FACE_ID 0
    #define LINES_ID 1
    #define SNAKE_ID 2
    #define ARKANOID_ID 3

    byte sprites[4][30] = {
      {0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000011, 0b00000011, 0b00010010, 0b00010010, 0b01001000, 
      0b01001000, 0b11000000, 0b11000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00010000, 0b00100000, 
      0b01000000, 0b10000000, 0b00000000, 0b00000010, 0b00000001, 0b11110000, 0b00000011, 0b00000000, 0b00000000, 0b00000000},
      
      {0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00011110, 0b00011110, 0b01001000, 
      0b01001000, 0b11000000, 0b11000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00010000, 0b00100000, 
      0b01000000, 0b10000000, 0b00000000, 0b00000010, 0b00000001, 0b11110000, 0b00000011, 0b00000000, 0b00000000, 0b00000000},
      
      {0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00010010, 0b00010010, 0b01111000, 
      0b01111000, 0b11000000, 0b11000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00010000, 0b00100000, 
      0b01000000, 0b10000000, 0b00000000, 0b00000010, 0b00000001, 0b11110000, 0b00000011, 0b00000000, 0b00000000, 0b00000000},
      
      {0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00010010, 0b00010010, 0b01001000, 
      0b01001000, 0b11000000, 0b11000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00010000, 0b00100000, 
      0b01000000, 0b10000000, 0b00000000, 0b00000010, 0b00000001, 0b11110000, 0b00000011, 0b00000000, 0b00000000, 0b00000000}
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

      //dla Sanke game
        int snakeSgt[DIODE_COUNT][2]; //wspolne dla arkanoid
        int head = 0;
        int tail_len = 3;
        int snake_dir = 0;  //0 - right, 1 - down, 2 - left, 3 - up
        int fruit_pos[2];
        int snake_mode = 0; //-1 dead, 0 static, 1 playing
        int snake_delay = 30; //30 okresow po 100 milisekund czyli 3 sekundy
        bool snake_visible = true;
      //

      //dla Arkanoid game
        int paletteSize = 3;
        int palettePosOnStart = (COLUMNS / 2) - (paletteSize / 2) - 1;
        int palettePos = palettePosOnStart;
        int maxPalettePos = COLUMNS - paletteSize;
        float ballPosition[2];
        float ballDirection[2];
        float ballSpeed = 0.45f;
        int arkanoid_mode = 0; //-1 dead, 0 static, 1 playing
        int arkanoid_lives = 3;
        int max_respawn_time = 40;  //40 okresow po 50 milisekund czyli 2 sekundy
        int respawn_time = 0;
      //

      //dla ogolnego menu
        int mainOption = 0; //główne menu
        bool isMainOpt = true;  //czy jestesmy w glownym menu
        int option = 0;     //menu podrzędne dla "aplikacji"
        byte flags_oneClick = 0b00000000;
        byte flags_holdClick = 0b00000000;
      //
     
    Adafruit_NeoPixel pixels = Adafruit_NeoPixel(DIODE_COUNT, PIN, NEO_GRB + NEO_KHZ800);
    
    byte switches = 0;  //left, up, right, down, btn1, btn2
    unsigned long lastIntMillis = 0;
    int colors[16][3] = {{0, 0, 0}, {0, 0, 255}, {0, 128, 128}, {0, 255, 0}, {128, 128, 0}, {255, 0, 0}, {128, 0, 128}, {85, 85, 85}, 
                        {51, 102, 102}, {102, 51, 102}, {102, 102, 51}, {0, 85, 170}, {0, 170, 85}, {85, 170, 0}, {170, 85, 0}, {170, 0, 85}};
    int colorsRGB[3] = {1, 3, 5};

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

    float fract(float x) { return x - int(x); }

    float mix(float a, float b, float t) { return a + (b - a) * t; }

    void hsv2rgb(float h, float s, float v, int* rgb) 
    {
      rgb[0] = v * mix(1.0, constrain(abs(fract(h + 1.0) * 6.0 - 3.0) - 1.0, 0.0, 1.0), s) * 255;
      rgb[1] = v * mix(1.0, constrain(abs(fract(h + 0.6666666) * 6.0 - 3.0) - 1.0, 0.0, 1.0), s) * 255;
      rgb[2] = v * mix(1.0, constrain(abs(fract(h + 0.3333333) * 6.0 - 3.0) - 1.0, 0.0, 1.0), s) * 255;
    }

    void blinkingEyes(int nrColor)
    { //mrugam oczkami z losowa czestotliwoscia
      if(actDelay <= 0)
      {
        refresh = true;
        if(spriteNr % 3 == 0)
          spriteInc = -spriteInc;

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

    void spawn_fruit()
    {
      bool isGood = true;

      do
      {
        isGood = true;
          
        fruit_pos[0] = random() % COLUMNS;
        fruit_pos[1] = random() % ROWS;

        for(int i = 0; i < tail_len; i++)
        {
          if(snakeSgt[i][0] == fruit_pos[0] && snakeSgt[i][1] == fruit_pos[1])
          {
            isGood = false;
            break;
          }
        }
      }while(isGood == false);
    }

    void reset_snake()
    {
        snakeSgt[0][0] = (int)(COLUMNS / 2);
        snakeSgt[0][1] = (int)(ROWS / 2);
        snakeSgt[1][0] = snakeSgt[0][0] - 1;
        snakeSgt[1][1] = snakeSgt[0][1];
        snakeSgt[2][0] = snakeSgt[1][0] - 1;
        snakeSgt[2][1] = snakeSgt[0][1];
        head = 0;
        tail_len = 3;
        snake_dir = 0;  //0 - right, 1 - down, 2 - left, 3 - up
        colorSingle(fruit_pos[1] * COLUMNS + fruit_pos[0], colors[0], 100); //zamaluj poprzedniego
        spawn_fruit();
        snake_mode = 0; //set static
        snake_delay = 30;
        snake_visible = true;

      for(int i = 0; i < tail_len; i++)
        colorSingle(snakeSgt[i][1] * COLUMNS + snakeSgt[i][0], colors[1], 100);
        
      delay(100);
    }

    void snakeGame()
    {
      if(snake_mode == 1)
      {
        //zapamietaj poprzednia pozycje glowy
        int prevHead[2];
        prevHead[0] = snakeSgt[head][0];
        prevHead[1] = snakeSgt[head][1];
  
        //glowa w miejscu ogona zawsze
        head--;
        if(head < 0)
          head = tail_len - 1;
  
        //zapamietaj poprzednie ulozenie ogona
        int prevTail[2];
        prevTail[0] = snakeSgt[head][0];
        prevTail[1] = snakeSgt[head][1];
  
        int movX = 0;
        int movY = 0;
        switch(snake_dir)
        {
          case 0: movX = 1; break;
          case 1: movY = 1; break;
          case 2: movX = -1; break;
          case 3: movY = -1; break;
          default: break;
        }
  
        snakeSgt[head][0] = prevHead[0] + movX;
        snakeSgt[head][1] = prevHead[1] + movY;
  
        if(snakeSgt[head][0] < 0)
          snakeSgt[head][0] = COLUMNS - 1;
        if(snakeSgt[head][0] >= COLUMNS)
          snakeSgt[head][0] = 0;
        if(snakeSgt[head][1] < 0)
          snakeSgt[head][1] = ROWS - 1;
        if(snakeSgt[head][1] >= ROWS)
          snakeSgt[head][1] = 0;

      if(snake_visible)
      {
        for(int i = 0; i < tail_len; i++)
        { //maluj weza
          int tmpColor[3];
          hsv2rgb((float) i / tail_len, 1, 1, tmpColor);
          colorSingle(snakeSgt[i][1] * COLUMNS + snakeSgt[i][0], tmpColor, 50);
        }
      }
      else
      {
        for(int i = 0; i < tail_len; i++) //usuwaj weza
          colorSingle(snakeSgt[i][1] * COLUMNS + snakeSgt[i][0], colors[0], 100);
      }

      colorSingle(fruit_pos[1] * COLUMNS + fruit_pos[0], colors[7], 100);
      
        //zdobywanie punktu
        if(snakeSgt[head][0] == fruit_pos[0] && snakeSgt[head][1] == fruit_pos[1])
        {
          tail_len++;
          spawn_fruit();
  
          for(int i = tail_len; i > head; i--)
          {
            snakeSgt[i][0] = snakeSgt[i - 1][0];
            snakeSgt[i][1] = snakeSgt[i - 1][1];
          }
          
          snakeSgt[head][0] = prevTail[0];
          snakeSgt[head][1] = prevTail[1];
  
          head++;
        }
  
        //smierc przy zderzeniu z samym soba
        for(int i = 0; i < tail_len; i++)
        {
          if(i == head)
            continue;
          if(snakeSgt[head][0] == snakeSgt[i][0] && snakeSgt[head][1] == snakeSgt[i][1])
          {
            snake_mode = -1;
            break;
          }
        }
      
        delay(100);
        colorSingle(prevTail[1] * COLUMNS + prevTail[0], colors[0], 100); //zamaluj za ogonem (oszczednosc obliczen :3)
      }
      else
      {
        if(snake_visible)
        {
          for(int i = 0; i < tail_len; i++)
          { //maluj weza
            int tmpColor[3];
            hsv2rgb((float) i / tail_len, 1, 1, tmpColor);
            colorSingle(snakeSgt[i][1] * COLUMNS + snakeSgt[i][0], tmpColor, 50);
          }
        }
        else
        {
          for(int i = 0; i < tail_len; i++) //usuwaj weza
            colorSingle(snakeSgt[i][1] * COLUMNS + snakeSgt[i][0], colors[0], 100);
        }
        if(snake_mode == -1)
        {
          snake_visible = !snake_visible;
          snake_delay--;
          if(snake_delay <= 0)
            reset_snake();
        }
          
        delay(100);
      }
    }

    void normalize(float pos[2])
    {
      float magnitude = sqrt(pow(pos[0], 2) + pow(pos[1], 2));
      pos[0] /= magnitude;
      pos[1] /= magnitude;
    }

    float myTan(float oppositeAngle, float nextToAngle)
    {
      return oppositeAngle / nextToAngle;
    }

    void check45degAngle(float sidesLength[2])
    {
      float tangens = myTan(sidesLength[1], abs(sidesLength[0]));
      if(tangens > -1.0f)
      {
        if(sidesLength[0] < 0.0f)
          sidesLength[0] = -0.71f;
        else
          sidesLength[0] = 0.71f;
        sidesLength[1] = -0.71f;
      }
      else
      {
        normalize(sidesLength);
      }
    }

    void reset_arkanoid_ball()
    {
      //usuwanie poprzedniej paletki
      for(int i = 0; i < paletteSize; i++)
        colorSingle((ROWS - 1) * COLUMNS + palettePos + i, colors[0], 100);

      palettePos = palettePosOnStart;
      respawn_time = 0;
      ballPosition[0] = (COLUMNS / 2) - 1;
      ballPosition[1] = ROWS - 2;

      ballDirection[0] = (float)random(-100, 101) / 100.0f;
      ballDirection[1] = (float)random(-100, 1) / 100.0f;
      check45degAngle(ballDirection);

      if(arkanoid_lives > 0)
        arkanoid_mode = 0;
      else
        arkanoid_mode = -1;
    }

    void reset_arkanoid()
    {
      arkanoid_lives = paletteSize;

      reset_arkanoid_ball();

      for(int i = 0; i < DIODE_COUNT; i++)
        snakeSgt[i][0] = 0;

      for(int i = 1; i < COLUMNS - 1; i++)
        for(int j = 1; j < ROWS / 2; j++)
          snakeSgt[j * COLUMNS + i][0] = 1;

      for(int i = 2; i < COLUMNS - 2; i++)
        for(int j = 2; j < ROWS / 2 - 1; j++)
          snakeSgt[j * COLUMNS + i][0] = 2;

      for(int i = 3; i < COLUMNS - 3; i++)
        for(int j = 3; j < ROWS / 2 - 2; j++)
          snakeSgt[j * COLUMNS + i][0] = 3;
    }

    void arkanoidGame()
    {
      //poruszanie paletki
      for(int i = 0; i < paletteSize; i++)
      {
        if(i < arkanoid_lives)
          colorSingle((ROWS - 1) * COLUMNS + palettePos + i, colors[3], 100);
        else
          colorSingle((ROWS - 1) * COLUMNS + palettePos + i, colors[5], 100);
      }

      if(palettePos > 0)
        colorSingle((ROWS - 1) * COLUMNS + palettePos - 1, colors[0], 100);

      if(palettePos < maxPalettePos)
        colorSingle((ROWS - 1) * COLUMNS + palettePos + paletteSize, colors[0], 100);
      //______________________

      //elementy zniszczalne
      for(int i = 0; i < DIODE_COUNT; i++)
      {
        if(snakeSgt[i][0] > 0)
          colorSingle(i, colors[colorsRGB[snakeSgt[i][0] - 1]], 25);
      }
      //______________________

      //poruszanie pileczki
      int actPosition = (int)(ballPosition[1] + 0.5f) * COLUMNS + (int)(ballPosition[0] + 0.5f);
      int nextPosition = (int)(ballPosition[1] + ballDirection[1] * ballSpeed + 0.5f) * COLUMNS + (int)(ballPosition[0] + ballDirection[0] * ballSpeed + 0.5f);
      colorSingle(actPosition, colors[0], 100); //zamalowywanie poprzedniej

      if(snakeSgt[nextPosition][0] == 0 && arkanoid_mode == 1)
      { //czy nie najeżdżam na statyczny obiekt po odbiciu
        ballPosition[0] += ballDirection[0] * ballSpeed;
        ballPosition[1] += ballDirection[1] * ballSpeed;

        ballPosition[0] = min(max(ballPosition[0], 0.0f), COLUMNS - 1);
        ballPosition[1] = min(max(ballPosition[1], 0.0f), ROWS - 1);
      }

      actPosition = (int)(ballPosition[1] + 0.5f) * COLUMNS + (int)(ballPosition[0] + 0.5f);

      if(ballPosition[0] <= 0 || ballPosition[0] >= (COLUMNS - 1))
        ballDirection[0] = -ballDirection[0];

      if(ballPosition[1] <= 0 || ballPosition[1] >= (ROWS - 1))
        ballDirection[1] = -ballDirection[1];

      if(ballPosition[1] >= (ROWS - 1))
      {
        arkanoid_lives--;
        reset_arkanoid_ball();
        respawn_time = max_respawn_time;
        return;
      }

      //czy uderzam w klocki zniszczalne

      //zabezpieczenie przed wyjscie poza tablicę, bo czasami nawet gry przełączał
      int bounce = max(min(actPosition + 1, DIODE_COUNT - 1), 0);
      if(snakeSgt[bounce][0] > 0 && ballDirection[0] > 0)
      { //prawo odbicie
        snakeSgt[bounce][0]--;
        colorSingle(bounce, colors[0], 100);
        ballDirection[0] = -ballDirection[0];
      }
      else
      {
        bounce = max(min(actPosition - 1, DIODE_COUNT - 1), 0);
        if(snakeSgt[bounce][0] > 0 && ballDirection[0] < 0)
        { //lewo odbicie
          snakeSgt[bounce][0]--;
          colorSingle(bounce, colors[0], 100);
          ballDirection[0] = -ballDirection[0];
        }
        else 
        {
          bounce = max(min(actPosition + COLUMNS, DIODE_COUNT - 1), 0);
          if(snakeSgt[bounce][0] > 0 && ballDirection[1] > 0)
          { //dół odbicie
            snakeSgt[bounce][0]--;
            colorSingle(bounce, colors[0], 100);
            ballDirection[1] = -ballDirection[1];
          }
          else 
          {
            bounce = max(min(actPosition - COLUMNS, DIODE_COUNT - 1), 0);
            if(snakeSgt[bounce][0] > 0 && ballDirection[1] < 0)
            { //góra odbicie
              snakeSgt[bounce][0]--;
              colorSingle(bounce, colors[0], 100);
              ballDirection[1] = -ballDirection[1];
            }
            else
            { //dowolne uderzenie po przekątnej (rzadka sprawa)
              nextPosition = (int)(ballPosition[1] + ballDirection[1] * ballSpeed + 0.5f) * COLUMNS + (int)(ballPosition[0] + ballDirection[0] * ballSpeed + 0.5f);
              nextPosition = max(min(nextPosition, DIODE_COUNT - 1), 0);
              if(snakeSgt[nextPosition][0] > 0)
              {
              snakeSgt[nextPosition][0]--;
              colorSingle(nextPosition, colors[0], 100);
              ballDirection[0] = -ballDirection[0];
              ballDirection[1] = -ballDirection[1];
              }
            }
          }
        }
      }

      //Odbicie od paletki
      if(ballDirection[1] > 0)
      {
        for(int i = 0; i < paletteSize; i++)
        {
          if(actPosition + COLUMNS == (ROWS - 1) * COLUMNS + palettePos + i)
          {
            ballDirection[1] = -ballDirection[1];
            float center = palettePos + (paletteSize - 1) / 2;
            ballDirection[0] += (ballPosition[0] - center) * 0.5f;
            check45degAngle(ballDirection);
          }
        }
      }

      colorSingle(actPosition, colors[14], 100);
      //______________________
      
      if(respawn_time > 0)
        respawn_time--;

      delay(50);
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
      
    if(bitRead(flags_oneClick, BTN_LEFT))
    { //left
      if(!isMainOpt)
      {
        if(mainOption == FACE_ID)
        { //twarz
        option--;
        if(option < 0)
          option = 4;
        }
        if(mainOption == SNAKE_ID)
        { //snakeGame
          if(snake_dir != 0 && snake_mode == 1)
            snake_dir = 2;
          if(snake_mode == 0)
            snake_mode = 1;
        }
      }
      bitClear(flags_oneClick, BTN_LEFT);
    }

    if(bitRead(flags_holdClick, BTN_LEFT))
    {
      if(!isMainOpt)
      {
        if(mainOption == ARKANOID_ID)
        {
          if(palettePos > 0 && arkanoid_mode != -1 && respawn_time <= 0)
            palettePos--;
          if(arkanoid_mode == 0 && respawn_time <= 0)
            arkanoid_mode = 1;
        }
      }
    }
    
    if(bitRead(flags_oneClick, BTN_UP))
    { //up
      if(isMainOpt)
      {
        mainOption--;
        if(mainOption < MIN_OPTION)
          mainOption = MAX_OPTION;
      }
      else
      {
        if(mainOption == SNAKE_ID)
        { //snakeGame
          if(snake_dir != 1  && snake_mode == 1)
            snake_dir = 3;
          if(snake_mode == 0)
            snake_mode = 1;
        }
      }
      bitClear(flags_oneClick, BTN_UP);
    }
    if(bitRead(flags_oneClick, BTN_RIGHT))
    { //right
      if(!isMainOpt)
      {
        if(mainOption == FACE_ID)
        { //twarz
        option++;
        if(option > 4)
          option = 0;
        }
        if(mainOption == SNAKE_ID)
        { //snakeGame
          if(snake_dir != 2 && snake_mode == 1)
            snake_dir = 0;
          if(snake_mode == 0)
            snake_mode = 1;
        }
      }
      bitClear(flags_oneClick, BTN_RIGHT);
    }

    if(bitRead(flags_holdClick, BTN_RIGHT))
    { //right
      if(!isMainOpt)
      {
        if(mainOption == ARKANOID_ID)
        {
          if(palettePos < maxPalettePos && arkanoid_mode != -1 && respawn_time <= 0)
            palettePos++;
          if(arkanoid_mode == 0 && respawn_time <= 0)
            arkanoid_mode = 1;
        }
      }
    }
    
    if(bitRead(flags_oneClick, BTN_DOWN))
    { //down
      if(isMainOpt)
      {
        mainOption++;
        if(mainOption > MAX_OPTION)
          mainOption = MIN_OPTION;
      }
      else
      {
        if(mainOption == SNAKE_ID)
        { //snakeGame
          if(snake_dir != 3 && snake_mode == 1)
            snake_dir = 1;
          if(snake_mode == 0)
            snake_mode = 1;
        }
      }
      bitClear(flags_oneClick, BTN_DOWN);
    }
    if(bitRead(flags_oneClick, BTN_1))
    { //btn1 wlacza aplikacje
      if(isMainOpt)
      {
        isMainOpt = false;
        pixels.clear();
        
        if(mainOption == SNAKE_ID)
          reset_snake();
        
        if(mainOption == ARKANOID_ID)
          reset_arkanoid();
      }
      bitClear(flags_oneClick, BTN_1);
    }
    if(bitRead(flags_oneClick, BTN_2))
    { //btn2 wylacza aplikacje
      if(!isMainOpt && (mainOption >= MIN_OPTION && mainOption <= MAX_OPTION))
        isMainOpt = true;
      bitClear(flags_oneClick, BTN_2);
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
      if(mainOption == FACE_ID)
        blinkingEyes(option + 1);

      if(mainOption == LINES_ID)
        drawLines();

      if(mainOption == SNAKE_ID)
        snakeGame();

      if(mainOption == ARKANOID_ID)
        arkanoidGame();
    }

    pixels.show();
    }

    ISR(PCINT2_vect)
    {
      switches = PINK; // get PORTK value
      refresh = true;

      //buttony na click
      if(millis() - lastIntMillis > DEBOUNCE_TIME)
      {
        if(!bitRead(switches, BTN_LEFT)) //left
          bitSet(flags_oneClick, BTN_LEFT);
          
        if(!bitRead(switches, BTN_UP)) //up
          bitSet(flags_oneClick, BTN_UP);
          
        if(!bitRead(switches, BTN_RIGHT)) //right
          bitSet(flags_oneClick, BTN_RIGHT);
          
        if(!bitRead(switches, BTN_DOWN)) //down
          bitSet(flags_oneClick, BTN_DOWN);
          
        if(!bitRead(switches, BTN_1)) //btn1
          bitSet(flags_oneClick, BTN_1);
          
        if(!bitRead(switches, BTN_2)) //btn2
          bitSet(flags_oneClick, BTN_2);
      }

      //buttony na przytrzymanie
        if(!bitRead(switches, BTN_LEFT)) //left
          bitSet(flags_holdClick, BTN_LEFT);
        else
          bitClear(flags_holdClick, BTN_LEFT);
          
        if(!bitRead(switches, BTN_UP)) //up
          bitSet(flags_holdClick, BTN_UP);
        else
          bitClear(flags_holdClick, BTN_UP);
          
        if(!bitRead(switches, BTN_RIGHT)) //right
          bitSet(flags_holdClick, BTN_RIGHT);
        else
          bitClear(flags_holdClick, BTN_RIGHT);
          
        if(!bitRead(switches, BTN_DOWN)) //down
          bitSet(flags_holdClick, BTN_DOWN);
        else
          bitClear(flags_holdClick, BTN_DOWN);
          
        if(!bitRead(switches, BTN_1)) //btn1
          bitSet(flags_holdClick, BTN_1);
        else
          bitClear(flags_holdClick, BTN_1);
          
        if(!bitRead(switches, BTN_2)) //btn2
          bitSet(flags_holdClick, BTN_2);
        else
          bitClear(flags_holdClick, BTN_2);
      
      lastIntMillis = millis();
      Serial.println(switches, BIN);
    }
