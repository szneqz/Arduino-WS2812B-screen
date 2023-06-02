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
    #define TETRIS_ID 4

    //SpritesStaticValues
    #define SPRITE_ANIMATED_EYES 0
    #define SPRITE_COLORFUL_CAT 1
    #define MAX_SPRITE_NR 7

    //DrawLines
    #define LINES_AMOUNT 10

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

    byte colorfulCatSprite[][30] = {
      {0b00000100, 0b10000000, 0b00110000, 0b00000000, 0b01000011, 0b00000001, 0b00001010, 0b11010001, 0b00100010, 0b00000100, 
      0b10000011, 0b00001000, 0b00000000, 0b00100100, 0b00000000, 0b00010000, 0b00000000, 0b00000000, 0b00000010, 0b00000000, 
      0b00000001, 0b00000000, 0b01000000, 0b00000000, 0b00001000, 0b00000110, 0b00011000, 0b11100000, 0b00011111, 0b00000000},

      {0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b10000000, 0b00000000, 0b00000100, 0b00101000, 0b00000101, 0b11000000, 
      0b00001100, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b10000000, 0b00000001, 0b01100000, 0b00000000, 0b00000000, 
      0b00111000, 0b00000000, 0b00000111, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000},

      {0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 
      0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b10000110, 0b00000001, 0b00011000, 0b00000110, 0b00000000, 0b00000000, 
      0b10000000, 0b01001100, 0b00000000, 0b11001100, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000},

      //sprite2
      {0b00000000, 0b00000000, 0b00111000, 0b00000000, 0b01000111, 0b00000001, 0b00001010, 0b11010001, 0b00100010, 0b00000100, 
      0b10000011, 0b00001000, 0b00000000, 0b00100100, 0b00000000, 0b00010000, 0b00000000, 0b00000000, 0b00000010, 0b00000000, 
      0b00000001, 0b00000000, 0b01000000, 0b00000000, 0b00001000, 0b00000110, 0b00011000, 0b11100000, 0b00011111, 0b00000000},

      {0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b10000000, 0b00000000, 0b00000100, 0b00101000, 0b00000101, 0b11000000, 
      0b00001100, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b10000000, 0b00000001, 0b01100000, 0b00000000, 0b00000000, 
      0b00111000, 0b00000000, 0b00000111, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000},

      {0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 
      0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000010, 0b00000001, 0b00010100, 0b00001010, 0b00000000, 0b00000000, 
      0b00000000, 0b00110011, 0b00000000, 0b01111000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000},

      //sprite3 only eyes blink :3
      {0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 
      0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00011100, 0b00001110, 0b00000000, 0b00000000, 
      0b10000000, 0b01001100, 0b00000000, 0b11001100, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000}
    };

    byte menuSprites[][30] = {
      {0b00001111, 0b00000000, 0b00100000, 0b11111110, 0b10001111, 0b00001000, 0b00100000, 0b00100010, 0b10010001, 0b10001000, 
      0b00000000, 0b00100010, 0b01000010, 0b10001000, 0b00001111, 0b00100001, 0b10100000, 0b10100000, 0b10000000, 0b01111100, 
      0b00000010, 0b00000010, 0b00001000, 0b11111000, 0b00111111, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000},

      {0b00000000, 0b01000010, 0b00111100, 0b00001000, 0b10001111, 0b00100000, 0b00000000, 0b10000010, 0b00000011, 0b00001000, 
      0b11000000, 0b00100001, 0b00000000, 0b10000001, 0b00010111, 0b00000101, 0b01000000, 0b00010100, 0b00000000, 0b01010001, 
      0b00000000, 0b01001100, 0b00000000, 0b00000000, 0b00000001, 0b00000000, 0b00000000, 0b00000000, 0b00111100, 0b00000000},

      {0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b11110000, 0b11100000, 0b00001111, 0b10000010, 0b00000000, 0b00001000, 
      0b00000010, 0b00100000, 0b11111000, 0b10000111, 0b00000111, 0b00010000, 0b00000000, 0b01000000, 0b00000000, 0b00100000, 
      0b00000001, 0b00000000, 0b00000100, 0b10000000, 0b00010000, 0b00000000, 0b01111110, 0b00000000, 0b00000000, 0b00000000},

      {0b00000000, 0b00000000, 0b00000000, 0b10111100, 0b00000111, 0b11010000, 0b11011111, 0b01000011, 0b01001110, 0b00001000, 
      0b00000000, 0b00100000, 0b00000000, 0b10000000, 0b00000111, 0b00000000, 0b00000000, 0b00001000, 0b00000000, 0b00000000, 
      0b00000000, 0b00000000, 0b00000000, 0b11000000, 0b00000001, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000},

      {0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000100, 0b00000000, 0b00011000, 0b00001111, 
      0b00100000, 0b00100000, 0b00000000, 0b10000000, 0b00000111, 0b00000000, 0b00000000, 0b00000100, 0b00000000, 0b10010011, 
      0b00000001, 0b01111000, 0b00001111, 0b11110000, 0b00111111, 0b11000000, 0b10011111, 0b00000000, 0b11011110, 0b00000011}

    };

    byte staticSprites[][30] = {
      {0b01111000, 0b01111000, 0b11110000, 0b11110011, 0b11100011, 0b11111100, 0b10011100, 0b11100001, 0b01100001, 0b00000110, 
      0b10000011, 0b00111001, 0b00000000, 0b11000111, 0b00000001, 0b00001110, 0b00001110, 0b00011100, 0b01110000, 0b00111000, 
      0b10000000, 0b01110011, 0b00000000, 0b11111100, 0b00000000, 0b11100000, 0b00000001, 0b00000000, 0b00000011, 0b00000000},

      {0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b10000000, 0b00000011, 0b00000111, 0b00001010, 0b00010100, 0b00111000, 
      0b01110000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 
      0b00100000, 0b00000000, 0b10000001, 0b11111111, 0b00000111, 0b11111110, 0b00011111, 0b00000000, 0b00000000, 0b00000000},

      {0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b11111100, 
      0b11111100, 0b00110000, 0b00110000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00010000, 
      0b10000000, 0b00111111, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000},

      {0b10000000, 0b00000111, 0b10000000, 0b01100001, 0b00000000, 0b00110001, 0b00000010, 0b11000010, 0b00010000, 0b00001000, 
      0b01000011, 0b00010000, 0b00001100, 0b01000010, 0b00110000, 0b00001000, 0b11000001, 0b00100000, 0b00001000, 0b01000000, 
      0b00100000, 0b00001100, 0b00000001, 0b00110001, 0b00000010, 0b00011000, 0b00000110, 0b10000000, 0b00000111, 0b00000000},

      {0b01000000, 0b00000000, 0b00010001, 0b10011100, 0b00000000, 0b11111001, 0b00000000, 0b01110000, 0b10000111, 0b11000000, 
      0b01011000, 0b01001000, 0b01100000, 0b00000000, 0b11000000, 0b00000000, 0b11000000, 0b01001011, 0b00010000, 0b00000011, 
      0b00000000, 0b00000000, 0b01000000, 0b00110100, 0b00000001, 0b11000000, 0b00000000, 0b00100000, 0b10000000, 0b00000000}

      }; 

      //dla spriteów
      bool refresh = true;  //flaga odśeiżania obrazka
      int spriteNr = 0;
      int staticSpriteNr = 0;
      int spriteInc = -1;
      int actDelay = 0;

      //catSprite
      bool isHappy = false;
      //

      //dla DrawLines
        int act[LINES_AMOUNT][2] = {{0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}};
        int dest[LINES_AMOUNT][2] = {{0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}};
        int colorNr[LINES_AMOUNT] = {1, 2, 3, 5, 7, 8, 9, 10, 13, 15};
        int nextColorNr = 1;
        int dir[LINES_AMOUNT] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
      //

      //dla Sanke game
        int snakeSgt[DIODE_COUNT][2]; //wspolne dla snake, arkanoid, tetris
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

      //dla Tetris game
        int figurePosX = 0;
        int figurePosY = 0;
        const int figurePosXStart = 3;
        int figureRot = 0; //4 rotacje
        const int figureMaxRot = 4;
        int tetris_colors[10] = {1, 2, 3, 5, 7, 8, 9, 10, 13, 15};
        int figures[7][4][4] = {
        {{1, 5, 9, 13}, {4, 5, 6, 7}, {1, 5, 9, 13}, {4, 5, 6, 7}},   // linia
        {{4, 5, 9, 10}, {2, 6, 5, 9}, {4, 5, 9, 10}, {2, 6, 5, 9}},   // Z
        {{6, 7, 9, 10}, {1, 5, 6, 10}, {6, 7, 9, 10}, {1, 5, 6, 10}}, // Z odwrotne
        {{1, 2, 5, 9}, {0, 4, 5, 6}, {1, 5, 9, 8}, {4, 5, 6, 10}},    // L odwrotne
        {{1, 2, 6, 10}, {5, 6, 7, 9}, {2, 6, 10, 11}, {3, 5, 6, 7}},  // L
        {{1, 4, 5, 6}, {1, 4, 5, 9}, {4, 5, 6, 9}, {1, 5, 6, 9}},     // |-
        {{1, 2, 5, 6}, {1, 2, 5, 6}, {1, 2, 5, 6}, {1, 2, 5, 6}}      // kwadrat
        };
        int actualFigure = 0;
        int actualFigureColor = 0;
        bool randomizeFigure = false;  //czy wylosowac nowy blok
        const int tetris_game_width = 10; //standardowa rozgrywka
        int tetris_mode = 0;  //-1 dead, 0 static, 1 playing
        int block_delay = 20; //20 okresów po 50 milisekund czyli 1 sekunda
        const int movement_block_delay = 20;
        const int wholeLine_block_delay = 60;
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

    void ColorSingleAdd(int nr, int *color, int sat)
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

    void ColorSingle(int nr, int *color, int sat)
    {
      int tmp = ((nr / COLUMNS) % 2 == 0) ? ((DIODE_COUNT - 1) - nr) : ((DIODE_COUNT - 1) - COLUMNS * (nr / COLUMNS) - (COLUMNS - 1) + nr % COLUMNS);
      pixels.setPixelColor(tmp, *(color) * sat / 100, *(color + 1) * sat / 100, *(color + 2) * sat / 100);
    }

    void ColorHEX(byte *sprite, int *color, int sat, int *bgColor, int bgSat, bool useBg = true)
    {
      for(int i = 0; i < DIODE_COUNT; i++)
      {
        int tmp = ((i / COLUMNS) % 2 == 0) ? ((DIODE_COUNT - 1) - i) : ((DIODE_COUNT - 1) - COLUMNS * (i / COLUMNS) - (COLUMNS - 1) + i % COLUMNS);
        int tmp2 = tmp / 8;
        int tmp3 = tmp % 8;
        if(bitRead(*(sprite + tmp2), tmp3))
        pixels.setPixelColor(i, *(color) * sat / 100, *(color + 1) * sat / 100, *(color + 2) * sat / 100);
        else if(useBg)
        pixels.setPixelColor(i, *(bgColor) * bgSat / 100, *(bgColor + 1) * bgSat / 100, *(bgColor + 2) * bgSat / 100);
      }
    }

    float Fract(float x) { return x - int(x); }

    float Mix(float a, float b, float t) { return a + (b - a) * t; }

    void HSVtoRGB(float h, float s, float v, int* rgb) 
    {
      rgb[0] = v * Mix(1.0, constrain(abs(Fract(h + 1.0) * 6.0 - 3.0) - 1.0, 0.0, 1.0), s) * 255;
      rgb[1] = v * Mix(1.0, constrain(abs(Fract(h + 0.6666666) * 6.0 - 3.0) - 1.0, 0.0, 1.0), s) * 255;
      rgb[2] = v * Mix(1.0, constrain(abs(Fract(h + 0.3333333) * 6.0 - 3.0) - 1.0, 0.0, 1.0), s) * 255;
    }

    void BlinkingEyes(int nrColor)
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
      ColorHEX(sprites[spriteNr], colors[nrColor], 50, colors[0], 2);
      refresh = false;
      }
      delay(20);
      actDelay -= 20;
    }

    void ColorfulCat()
    {
      if(refresh)
      {
        int offset = 0;
        int blinkOffset = 0;

        if(isHappy)
          offset = 3;
        
        if(actDelay <= 0 && !isHappy)
          blinkOffset = 4;

        ColorHEX(colorfulCatSprite[0 + offset], colors[option + 1], 30, colors[0], 2);
        ColorHEX(colorfulCatSprite[1 + offset], colors[option + 5], 30, colors[0], 2, false);
        ColorHEX(colorfulCatSprite[2 + offset + blinkOffset], colors[option + 9], 30, colors[0], 2, false);
        refresh = false;
      }

      if(actDelay <= -500)
      {
        actDelay = random(3000, 8000);
        refresh = true;
      }
      else
        actDelay -= 20;

      if(actDelay < 0 && actDelay >= -20)
        refresh = true;

      delay(20);
    }

    void StaticSprite()
    {
      int actSprite = staticSpriteNr - 2; //-2 bo są dwa niestatyczne obrazy
      if(refresh)
      {
        ColorHEX(staticSprites[actSprite], colors[option + 1], 30, colors[0], 2);
        refresh = false;
      }
      delay(20);
    }

    void DrawLines()
    { 
      for(int i = 0; i < LINES_AMOUNT; i++)
      {
        if((act[i][0] == dest[i][0] && act[i][1] == dest[i][1]) || act[i][0] >= COLUMNS || act[i][0] < 0 || act[i][1] >= ROWS || act[i][1] < 0)
        {
          nextColorNr++;
          if(nextColorNr >= 16)
            nextColorNr = 1;
          colorNr[i] = nextColorNr;

          if(random() % 2 == 0)
          {
            act[i][0] = (random() % 2) * (COLUMNS - 1);
            act[i][1] = random() % (ROWS - 4) + 2;
            dest[i][0] = random() % (COLUMNS - 4) + 2;
            dest[i][1] = (random() % 2) * (ROWS - 1);
            if(act[i][0] == 0)
              dir[i] = 1;
            else
              dir[i] = -1;
          }
          else
          {
            act[i][0] = random() % (COLUMNS - 4) + 2;
            act[i][1] = (random() % 2) * (ROWS - 1);
            dest[i][0] = (random() % 2) * (COLUMNS - 1);
            dest[i][1] = random() % (ROWS - 4) + 2;
            if(act[i][1] == 0)
              dir[i] = 2;
            else
              dir[i] = -2;
          }
        }
        else
        {
          if(abs(dir[i]) == 1 && act[i][0] == dest[i][0])
          {
            if(dest[i][1] == 0)
              dir[i] = -2;
            else
              dir[i] = 2;
          }
    
          if(abs(dir[i]) == 2 && act[i][1] == dest[i][1])
          {
            if(dest[i][0] == 0)
              dir[i] = -1;
            else
              dir[i] = 1;
          }
    
          if(dir[i] == 1)
            act[i][0] = act[i][0] + 1;
          if(dir[i] == -1)
            act[i][0] = act[i][0] - 1;
          if(dir[i] == 2)
            act[i][1] = act[i][1] + 1;
          if(dir[i] == -2)
            act[i][1] = act[i][1] - 1;
        }
        
        ColorSingleAdd(act[i][1] * COLUMNS + act[i][0], colors[colorNr[i]], 100);
      }

        for(int j = 0; j < DIODE_COUNT; j++)
        {
          unsigned long gColor = pixels.getPixelColor(j);
          byte gColors[3] = {0, 0, 0};
          for(int k = 0; k < 3; k++)
          {
            for(int l = 0; l < 8; l++)
            {
              bitWrite(gColors[k], l, bitRead(gColor, k * 8 + l));
            }
          }
        
          pixels.setPixelColor(j, gColors[2]/2,  gColors[1]/2,  gColors[0]/2);
        }
      delay(20);
    }

    void SpawnFruit()
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

    void ResetSnake()
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
        ColorSingle(fruit_pos[1] * COLUMNS + fruit_pos[0], colors[0], 100); //zamaluj poprzedniego
        SpawnFruit();
        snake_mode = 0; //set static
        snake_delay = 30;
        snake_visible = true;

      for(int i = 0; i < tail_len; i++)
        ColorSingle(snakeSgt[i][1] * COLUMNS + snakeSgt[i][0], colors[1], 100);
        
      delay(100);
    }

    void SnakeGame()
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
          HSVtoRGB((float) i / tail_len, 1, 1, tmpColor);
          ColorSingle(snakeSgt[i][1] * COLUMNS + snakeSgt[i][0], tmpColor, 50);
        }
      }
      else
      {
        for(int i = 0; i < tail_len; i++) //usuwaj weza
          ColorSingle(snakeSgt[i][1] * COLUMNS + snakeSgt[i][0], colors[0], 100);
      }

      ColorSingle(fruit_pos[1] * COLUMNS + fruit_pos[0], colors[7], 100);
      
        //zdobywanie punktu
        if(snakeSgt[head][0] == fruit_pos[0] && snakeSgt[head][1] == fruit_pos[1])
        {
          tail_len++;
          SpawnFruit();
  
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
        ColorSingle(prevTail[1] * COLUMNS + prevTail[0], colors[0], 100); //zamaluj za ogonem (oszczednosc obliczen :3)
      }
      else
      {
        if(snake_visible)
        {
          for(int i = 0; i < tail_len; i++)
          { //maluj weza
            int tmpColor[3];
            HSVtoRGB((float) i / tail_len, 1, 1, tmpColor);
            ColorSingle(snakeSgt[i][1] * COLUMNS + snakeSgt[i][0], tmpColor, 50);
          }
        }
        else
        {
          for(int i = 0; i < tail_len; i++) //usuwaj weza
            ColorSingle(snakeSgt[i][1] * COLUMNS + snakeSgt[i][0], colors[0], 100);
        }
        if(snake_mode == -1)
        {
          snake_visible = !snake_visible;
          snake_delay--;
          if(snake_delay <= 0)
            ResetSnake();
        }
          
        delay(100);
      }
    }

    void Normalize(float pos[2])
    {
      float magnitude = sqrt(pow(pos[0], 2) + pow(pos[1], 2));
      pos[0] /= magnitude;
      pos[1] /= magnitude;
    }

    float MyTan(float oppositeAngle, float nextToAngle)
    {
      return oppositeAngle / nextToAngle;
    }

    void Check45DegAngle(float sidesLength[2])
    {
      float tangens = MyTan(sidesLength[1], abs(sidesLength[0]));
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
        Normalize(sidesLength);
      }
    }

    void ResetArkanoidBall()
    {
      //usuwanie poprzedniej paletki
      for(int i = 0; i < paletteSize; i++)
        ColorSingle((ROWS - 1) * COLUMNS + palettePos + i, colors[0], 100);

      palettePos = palettePosOnStart;
      respawn_time = 0;
      ballPosition[0] = (COLUMNS / 2) - 1;
      ballPosition[1] = ROWS - 2;

      ballDirection[0] = (float)random(-100, 101) / 100.0f;
      ballDirection[1] = (float)random(-100, 1) / 100.0f;
      Check45DegAngle(ballDirection);

      if(arkanoid_lives > 0)
        arkanoid_mode = 0;
      else
        arkanoid_mode = -1;
    }

    void ResetArkanoid()
    {
      arkanoid_lives = paletteSize;

      ResetArkanoidBall();

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

    void ArkanoidGame()
    {
      //poruszanie paletki
      for(int i = 0; i < paletteSize; i++)
      {
        if(i < arkanoid_lives)
          ColorSingle((ROWS - 1) * COLUMNS + palettePos + i, colors[3], 100);
        else
          ColorSingle((ROWS - 1) * COLUMNS + palettePos + i, colors[5], 100);
      }

      if(palettePos > 0)
        ColorSingle((ROWS - 1) * COLUMNS + palettePos - 1, colors[0], 100);

      if(palettePos < maxPalettePos)
        ColorSingle((ROWS - 1) * COLUMNS + palettePos + paletteSize, colors[0], 100);
      //______________________

      //elementy zniszczalne
      for(int i = 0; i < DIODE_COUNT; i++)
      {
        if(snakeSgt[i][0] > 0)
          ColorSingle(i, colors[colorsRGB[snakeSgt[i][0] - 1]], 25);
      }
      //______________________

      //poruszanie pileczki
      int actPosition = (int)(ballPosition[1] + 0.5f) * COLUMNS + (int)(ballPosition[0] + 0.5f);
      int nextPosition = (int)(ballPosition[1] + ballDirection[1] * ballSpeed + 0.5f) * COLUMNS + (int)(ballPosition[0] + ballDirection[0] * ballSpeed + 0.5f);
      ColorSingle(actPosition, colors[0], 100); //zamalowywanie poprzedniej

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
        ResetArkanoidBall();
        respawn_time = max_respawn_time;
        return;
      }

      //czy uderzam w klocki zniszczalne

      //zabezpieczenie przed wyjscie poza tablicę, bo czasami nawet gry przełączał
      int bounce = max(min(actPosition + 1, DIODE_COUNT - 1), 0);
      if(snakeSgt[bounce][0] > 0 && ballDirection[0] > 0)
      { //prawo odbicie
        snakeSgt[bounce][0]--;
        ColorSingle(bounce, colors[0], 100);
        ballDirection[0] = -ballDirection[0];
      }
      else
      {
        bounce = max(min(actPosition - 1, DIODE_COUNT - 1), 0);
        if(snakeSgt[bounce][0] > 0 && ballDirection[0] < 0)
        { //lewo odbicie
          snakeSgt[bounce][0]--;
          ColorSingle(bounce, colors[0], 100);
          ballDirection[0] = -ballDirection[0];
        }
        else 
        {
          bounce = max(min(actPosition + COLUMNS, DIODE_COUNT - 1), 0);
          if(snakeSgt[bounce][0] > 0 && ballDirection[1] > 0)
          { //dół odbicie
            snakeSgt[bounce][0]--;
            ColorSingle(bounce, colors[0], 100);
            ballDirection[1] = -ballDirection[1];
          }
          else 
          {
            bounce = max(min(actPosition - COLUMNS, DIODE_COUNT - 1), 0);
            if(snakeSgt[bounce][0] > 0 && ballDirection[1] < 0)
            { //góra odbicie
              snakeSgt[bounce][0]--;
              ColorSingle(bounce, colors[0], 100);
              ballDirection[1] = -ballDirection[1];
            }
            else
            { //dowolne uderzenie po przekątnej (rzadka sprawa)
              nextPosition = (int)(ballPosition[1] + ballDirection[1] * ballSpeed + 0.5f) * COLUMNS + (int)(ballPosition[0] + ballDirection[0] * ballSpeed + 0.5f);
              nextPosition = max(min(nextPosition, DIODE_COUNT - 1), 0);
              if(snakeSgt[nextPosition][0] > 0)
              {
              snakeSgt[nextPosition][0]--;
              ColorSingle(nextPosition, colors[0], 100);
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
            Check45DegAngle(ballDirection);
          }
        }
      }

      ColorSingle(actPosition, colors[14], 100);
      //______________________
      
      if(respawn_time > 0)
        respawn_time--;

      delay(50);
    }

    void ResetTetris()
    {
      for(int i = 0; i < DIODE_COUNT; i++)
        snakeSgt[i][0] = 0;
      
      randomizeFigure = true;
      tetris_mode = 0;

      for(int i = 0; i < ROWS; i++) //pionowa biala kreska oddzielajaca gre
        ColorSingle(i * COLUMNS + tetris_game_width, colors[7], 100);
    }

    int GetFigureBlockPos(int i, int myFigPosX = -1, int myFigPosY = -1)
    {
      if(myFigPosX == -1) myFigPosX = figurePosX;
      if(myFigPosY == -1) myFigPosY = figurePosY;
      return (myFigPosY + (figures[actualFigure][figureRot][i] / 4)) * COLUMNS + myFigPosX + (figures[actualFigure][figureRot][i] % 4);
    } 

    void DrawFigure(int lastPosX, int lastPosY, int lastRot = -1)
    {
      if(lastRot == -1)
        lastRot = figureRot;

      for(int i = 0; i < 4; i++)
      {
        int figureBlockPos = GetFigureBlockPos(i, lastPosX, lastPosY);
        ColorSingle(figureBlockPos, colors[0], 100);
      }

      for(int i = 0; i < 4; i++)
      {
        int figureBlockPos = GetFigureBlockPos(i, figurePosX, figurePosY);
        ColorSingle(figureBlockPos, colors[actualFigureColor], 100);
      }
    }

    void RotateTetrisFigure()
    {

    }

    void MoveTetrisLeftRight(int dir)
    {
      if(tetris_mode == 1)
      {
        bool canMove = true;

        for(int i = 0; i < 4; i++)
        {
          int figureBlockPos = GetFigureBlockPos(i);
          if((figureBlockPos % COLUMNS) + dir < 0 || (figureBlockPos % COLUMNS) + dir >= tetris_game_width || snakeSgt[figureBlockPos + dir][0] != 0)
          { //jezeli blok w bok to juz wyjechanie za mape lub blok w bok zawiera juz inny blok
            canMove = false;
            break;
          }
        }

        if(canMove)
        {
          figurePosX += dir;
          DrawFigure(figurePosX - dir, figurePosY);
        }
      }
    }

    void TetrisGame()
    {
      if(randomizeFigure)
      {
        actualFigure = random(0,7);
        actualFigureColor = random(0,10);
        figurePosX = figurePosXStart;
        figurePosY = 0;
        figureRot = 0;
        randomizeFigure = false;
        block_delay = movement_block_delay * 2; //na poczatku niech ma 2 sekundy czekanka
      }

      if(block_delay > 0)
        block_delay--;

      if(block_delay <= 0 && tetris_mode == 1)
      {
          for(int i = 0; i < 4; i++)
          {
            int figureBlockPos = GetFigureBlockPos(i);
            if(figureBlockPos + COLUMNS > DIODE_COUNT || snakeSgt[figureBlockPos + COLUMNS][0] != 0) 
            { //jezeli blok nizej to juz wyjechanie za mape lub blok nizej zawiera juz inny blok
              randomizeFigure = true;
              break;
            }
          }

        if(!randomizeFigure)
        {
          figurePosY++; //ruch bloku w dol
          DrawFigure(figurePosX, figurePosY - 1);
          block_delay = movement_block_delay;
        }
        else
        { //jezeli nie moze sie ruszyc to zapisz informacje o kolorze na tablicy
          randomizeFigure = true;
          for(int i = 0; i < 4; i++)
          {
            int figureBlockPos = (figurePosY + (figures[actualFigure][figureRot][i] / 4)) * COLUMNS + figurePosX + (figures[actualFigure][figureRot][i] % 4);
            snakeSgt[figureBlockPos][0] = actualFigureColor;
          }
        }
      }
      //TODO:
      //dodaj obracanie figur!
      //przyspieszanie w dol
      //jezeli mamy juz na spawnie zderzenie to koniec gry
      //testuj gre :3
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
        refresh = true;
        if(option < 0)
          option = 4;
        }
        if(mainOption == SNAKE_ID)
        { //SnakeGame
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
        if(mainOption == TETRIS_ID)
        {
          if(figurePosX > 0 && tetris_mode != -1)
            MoveTetrisLeftRight(-1);
          if(tetris_mode == 0)
            tetris_mode = 1;
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
        if(mainOption == FACE_ID)
        {
          staticSpriteNr--;
          refresh = true;
          if(staticSpriteNr < 0)
            staticSpriteNr = MAX_SPRITE_NR - 1;
        }

        if(mainOption == SNAKE_ID)
        { //SnakeGame
          if(snake_dir != 1  && snake_mode == 1)
            snake_dir = 3;
          if(snake_mode == 0)
            snake_mode = 1;
        }

        if(mainOption == TETRIS_ID)
        {
          RotateTetrisFigure();
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
        refresh = true;
        if(option > 4)
          option = 0;
        }
        if(mainOption == SNAKE_ID)
        { //SnakeGame
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
        if(mainOption == TETRIS_ID)
        {
          if(figurePosX < (tetris_game_width - 4) && tetris_mode != -1)
            MoveTetrisLeftRight(1);
          if(tetris_mode == 0)
            tetris_mode = 1;
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
        if(mainOption == FACE_ID)
        {
          staticSpriteNr++;
          refresh = true;
          if(staticSpriteNr >= MAX_SPRITE_NR)
            staticSpriteNr = 0;
        }
        if(mainOption == SNAKE_ID)
        { //SnakeGame
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
        refresh = true;  //resetowanie ustawienia sprite
        pixels.clear();
        
        if(mainOption == SNAKE_ID)
          ResetSnake();
        
        if(mainOption == ARKANOID_ID)
          ResetArkanoid();

        if(mainOption == TETRIS_ID)
          ResetTetris();
      }
      bitClear(flags_oneClick, BTN_1);
    }

    if(bitRead(flags_holdClick, BTN_1))
    { //btn1
      if(!isMainOpt)
      {
        if(mainOption == FACE_ID)
        {
          if(staticSpriteNr == SPRITE_COLORFUL_CAT)
            isHappy = true;
        }
      }
    }
    else
    {
      if(!isMainOpt)
      {
        if(mainOption == FACE_ID)
        {
          if(staticSpriteNr == SPRITE_COLORFUL_CAT)
            isHappy = false;
        }
      }
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
      ColorHEX(menuSprites[mainOption], colors[mainOption + 1], 30, colors[0], 2);
      refresh = false;
    }

    if(!isMainOpt)
    {
      if(mainOption == FACE_ID)
      {
      if(staticSpriteNr == SPRITE_ANIMATED_EYES)
        BlinkingEyes(option + 1);
      else if (staticSpriteNr == SPRITE_COLORFUL_CAT)
        ColorfulCat();
      else
        StaticSprite();
      }

      if(mainOption == LINES_ID)
        DrawLines();

      if(mainOption == SNAKE_ID)
        SnakeGame();

      if(mainOption == ARKANOID_ID)
        ArkanoidGame();

      if(mainOption == TETRIS_ID)
        TetrisGame();
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
