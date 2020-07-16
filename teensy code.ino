
#include <ILI9341_t3.h>
#include <font_Arial.h> // from ILI9341_t3
#include <XPT2046_Touchscreen.h>
#include <Bounce.h>
#include <Adafruit_NeoPixel.h>

#define orange 0xF2C4
#define neon  0xEFE2
#define green  0x1FC8
#define chaba  0xF9FF
#define red   0xF800
#define alien   0x1783
#define white   0xFE36

#define BACKLIGHT_PIN   10
#define TFT_DC          20
#define TFT_CS          21
#define TFT_RST         255  // 255 = unused. connect to 3.3V
#define TFT_MOSI        11
#define TFT_SCLK        14
#define TFT_MISO        12
#define CS_PIN  9    //9
#define TIRQ_PIN  8   //1

class Strip
{
  public:
    uint8_t   effect;
    uint8_t   effects;
    uint16_t  effStep;
    unsigned long effStart;
    Adafruit_NeoPixel strip;
    Strip(uint16_t leds, uint8_t pin, uint8_t toteffects, uint16_t striptype) : strip(leds, pin, striptype) {
      effect = -1;
      effects = toteffects;
      Reset();
    }
    void Reset() {
      effStep = 0;
      effect = (effect + 1) % effects;
      effStart = millis();
    }
};

struct Loop
{
  uint8_t currentChild;
  uint8_t childs;
  bool timeBased;
  uint16_t cycles;
  uint16_t currentTime;
  Loop(uint8_t totchilds, bool timebased, uint16_t tottime) {
    currentTime = 0;
    currentChild = 0;
    childs = totchilds;
    timeBased = timebased;
    cycles = tottime;
  }
};

Strip strip_0(16, 38, 16, NEO_GRB + NEO_KHZ800);
struct Loop strip0loop0(1, false, 1);



int statCom = 0;
ILI9341_t3 tft = ILI9341_t3(TFT_CS, TFT_DC, TFT_RST, TFT_MOSI, TFT_SCLK, TFT_MISO);
XPT2046_Touchscreen ts(CS_PIN, TIRQ_PIN);
boolean wastouched = true;
int x;
int bpm = 140;
int roomSize = 1;
int oct;
int cvtToTft;
int cvtNeo;
float bar = 1.1;
int roomNow;
int roomRec;
int colorTft[8] = {0xF800, 0x07DF, 0xFFE0, 0x04FF, 0xF800, 0x07DF, 0xFFE0, 0x04FF};

int colorBpm[16] =
{ tft.color565(0, 71, 189),
  tft.color565(2, 138, 217),
  tft.color565(7, 185, 252),
  tft.color565(0, 149, 67),
  tft.color565(0, 171, 56),
  tft.color565(154, 240, 0),
  tft.color565(255, 179, 0),
  tft.color565(255, 206, 0),
  tft.color565(255, 230, 59),
  tft.color565(234, 0, 52),
  tft.color565(253, 71, 3),
  tft.color565(253, 130, 42),
  tft.color565(130, 0, 172),
  tft.color565(182, 16, 191),
  tft.color565(204, 114, 245),
  tft.color565(204, 114, 245)
};

bool btnLoopState[8] = {false, false, false, false, false, false, false, false};
bool buttonStatePrevious[8] = {false, false, false, false, false, false, false, false};
bool buttonStateLongPress[8] = {false, false, false, false, false, false, false, false};
bool buttonStateShortPress[8]  = {false, false, false, false, false, false, false, false};

unsigned long buttonPressDuration[8];
unsigned long buttonLongPressMillis[8];
const unsigned long minButtonLongPressDuration = 1000;


float mut[8] = {0, 0, 0, 0, 0, 0, 0, 0};
float clearLoop[8] = {false, false, false, false, false, false, false, false};
String loopStation[8];

String instName[16] = {"electro drum", "acoustic drum", "dj mustard kit", "x.shulz lofi", "lead synth1", "lead synth2", "lead synth3", "synth pad1", "synth pad2", "synth pad3", "synth bass1", "synth bass2", "synth bass3", "synth keys1", "synth keys2", "synth keys3"};
String nowInst = "electro drum";

Bounce pushbutton[2] = {Bounce(15, 5), Bounce(16, 5)};
Bounce pushbutton2[2] = {Bounce(19, 5), Bounce(18, 5)};
bool selectInst = false;
bool selectMetro = false;
bool buttonState[2] = {false, false};
bool buttonState2[2] = {false, false};

bool serialStarted = false;
bool serialEnded = false;
char inData[80];
int serIndex;

#define PIN            38
#define NUMPIXELS      16
#define PIN2            39
#define NUMPIXELS2      8
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixels2 = Adafruit_NeoPixel(NUMPIXELS2, PIN2, NEO_GRB + NEO_KHZ800);

#define MAX_DEBOUNCE (1)

#define NUM_BTN_COLUMNS (4)
#define NUM_BTN_ROWS (1)

#define NUM_BTN_COLUMNS1 (4)
#define NUM_BTN_ROWS1 (1)

#define NUM_BTN_COLUMNS2 (4)
#define NUM_BTN_ROWS2 (4)

#define muxPin0 0  // Digital Pin 2
#define muxPin1 1  // Digital Pin 3
#define muxPin2 2  // Digital Pin 4
#define muxPin3 3  // Digital Pin 5

const int muxIn[2] =  {A21, A22}; //mux

uint32_t neoColorArray[15] {
  pixels.Color(243, 0, 183),
  pixels.Color(0, 150, 39),
  pixels.Color(244, 165, 44),
  pixels.Color(163, 0, 241),
  pixels.Color(57, 247, 240),     //now
  pixels.Color(243, 0, 183),
  pixels.Color(0, 150, 39),
  pixels.Color(244, 165, 44),
  pixels.Color(163, 0, 241),
  pixels.Color(57, 247, 240),
  pixels.Color(243, 0, 183),
  pixels.Color(0, 150, 39),
  pixels.Color(244, 165, 44),
  pixels.Color(163, 0, 241),
  pixels.Color(57, 247, 240)
};

uint32_t neoColorLoop[8] {
  pixels2.Color(237, 28, 36),
  pixels2.Color(0, 255, 255),
  pixels2.Color(237, 28, 36),
  pixels2.Color(0, 255, 255),
  pixels2.Color(229, 233, 0),
  pixels2.Color(0, 89, 255),
  pixels2.Color(229, 233, 0),
  pixels2.Color(0, 89, 255)
};
uint32_t neoColorOff = pixels.Color(0, 0, 0);
uint32_t neoColorOff2 = pixels2.Color(0, 0, 0);
// Global variables
static int32_t next_scan[3];
static const uint8_t btncolumnpins[NUM_BTN_COLUMNS] = {28, 29, 30, 31};
static const uint8_t btnrowpins[NUM_BTN_ROWS]       = {32};
static const uint8_t btncolumnpins1[NUM_BTN_COLUMNS1] = {33, 34, 35, 36};
static const uint8_t btnrowpins1[NUM_BTN_ROWS1]       = {37};

static const uint8_t btncolumnpins2[NUM_BTN_COLUMNS2] = {4, 5, 6, 7}; //btn sel ,gnd
static const uint8_t btnrowpins2[NUM_BTN_ROWS2]       = {24, 25 , 26, 27}; //btn read , switch

static int8_t debounce_count[NUM_BTN_COLUMNS][NUM_BTN_ROWS];
static int8_t debounce_count1[NUM_BTN_COLUMNS1][NUM_BTN_ROWS1];
static int8_t debounce_count2[NUM_BTN_COLUMNS2][NUM_BTN_ROWS2];


static void setuppins()
{
  uint8_t i;
  // button columns
  for (i = 0; i < NUM_BTN_COLUMNS; i++)
  {
    pinMode(btncolumnpins[i], OUTPUT);
    digitalWrite(btncolumnpins[i], HIGH);
  }

  // button row input lines
  for (i = 0; i < NUM_BTN_ROWS; i++)
  {
    pinMode(btnrowpins[i], INPUT_PULLUP);
  }
  // Initialize the debounce counter array
  for (uint8_t i = 0; i < NUM_BTN_COLUMNS; i++)
  {
    for (uint8_t j = 0; j < NUM_BTN_ROWS; j++)
    {
      debounce_count[i][j] = 0;
    }
  }
}

static void setuppins1()
{
  uint8_t i;
  // button columns
  for (i = 0; i < NUM_BTN_COLUMNS1; i++)
  {
    pinMode(btncolumnpins1[i], OUTPUT);
    digitalWrite(btncolumnpins1[i], HIGH);
  }

  // button row input lines
  for (i = 0; i < NUM_BTN_ROWS1; i++)
  {
    pinMode(btnrowpins1[i], INPUT_PULLUP);
  }
  // Initialize the debounce counter array
  for (uint8_t i = 0; i < NUM_BTN_COLUMNS1; i++)
  {
    for (uint8_t j = 0; j < NUM_BTN_ROWS1; j++)
    {
      debounce_count1[i][j] = 0;
    }
  }
}

void setupButtonPad4_4() {
  uint8_t i;
  // button columns
  for (i = 0; i < NUM_BTN_COLUMNS2; i++) //set btn gnd
  {
    pinMode(btncolumnpins2[i], OUTPUT);

    // with nothing selected by default
    digitalWrite(btncolumnpins2[i], HIGH);
  }

  // button row input lines
  for (i = 0; i < NUM_BTN_ROWS2; i++)  //set btn input
  {
    pinMode(btnrowpins2[i], INPUT_PULLUP);
  }

  for (uint8_t i = 0; i < NUM_BTN_COLUMNS2; i++)   //set debounce =0
  {
    for (uint8_t j = 0; j < NUM_BTN_ROWS2; j++)
    {
      debounce_count2[i][j] = 0;
    }
  }
}


static void scan()
{
  static uint8_t current = 0;
  uint8_t val;
  uint8_t i, j;

  // Select current columns
  digitalWrite(btncolumnpins[current], LOW);
  // pause a moment
  delay(1);
  // Read the button inputs

  for ( j = 0; j < NUM_BTN_ROWS; j++)
  {
    val = digitalRead(btnrowpins[j]);
    if (val == LOW)
    {
      if ( debounce_count[current][j] < MAX_DEBOUNCE)
      {
        debounce_count[current][j]++;
        if ( debounce_count[current][j] == MAX_DEBOUNCE )
        {
          btnLoopState[(current * NUM_BTN_ROWS) + j] = true;
          //btnLoopState2[(current * NUM_BTN_ROWS) + j] = ! btnLoopState2[(current * NUM_BTN_ROWS) + j];
          //                              Serial.print("btnLoop");
          //                              Serial.print((current * NUM_BTN_ROWS) + j);
          //                              Serial.print(":");
          //                              Serial.println(" 1");
          //                              Serial.println( btnLoopState2[(current * NUM_BTN_ROWS) + j]);

        }
      }
    }
    else
    {
      // otherwise, button is released
      if ( debounce_count[current][j] > 0)
      {
        debounce_count[current][j]--;
        if ( debounce_count[current][j] == 0 )
        {
          btnLoopState[(current * NUM_BTN_ROWS) + j] = false;

          //                    Serial.print("btnLoop");
          //                    Serial.print((current * NUM_BTN_ROWS) + j);
          //                    Serial.print(":");
          //                    Serial.println(" 0");
        }
      }
    }
  }// for j = 0 to 3;

  delay(1);
  digitalWrite(btncolumnpins[current], HIGH);

  current++;
  if (current >= NUM_BTN_COLUMNS)
  {
    current = 0;
  }
}

static void scan1()
{
  static uint8_t current = 0;
  uint8_t val;
  uint8_t i, j;

  // Select current columns
  digitalWrite(btncolumnpins1[current], LOW);
  // pause a moment
  delay(1);
  // Read the button inputs

  for ( j = 0; j < NUM_BTN_ROWS1; j++)
  {
    val = digitalRead(btnrowpins1[j]);
    if (val == LOW)
    {
      if ( debounce_count1[current][j] < MAX_DEBOUNCE)
      {
        debounce_count1[current][j]++;
        if ( debounce_count1[current][j] == MAX_DEBOUNCE )
        {
          btnLoopState[(current * NUM_BTN_ROWS) + j + 4] = true;
          //btnLoopState2[(current * NUM_BTN_ROWS) + j + 4] = ! btnLoopState2[(current * NUM_BTN_ROWS) + j + 4];
          //btnLoopState[(current * NUM_BTN_ROWS) + j + 4] = !btnLoopState[(current * NUM_BTN_ROWS) + j + 4];
          //                              Serial.print("btnLoop");
          //                              Serial.print(((current * NUM_BTN_ROWS1) + j) + 4);
          //                              Serial.print(":");
          //                              Serial.println(" 1");


        }
      }
    }
    else
    {
      // otherwise, button is released
      if ( debounce_count1[current][j] > 0)
      {
        debounce_count1[current][j]--;
        if ( debounce_count1[current][j] == 0 )
        {
          btnLoopState[(current * NUM_BTN_ROWS) + j + 4] = false;
          //                    Serial.print("btnLoop");
          //                    Serial.print(((current * NUM_BTN_ROWS1) + j) + 4);
          //                    Serial.print(":");
          //                    Serial.println(" 0");
        }
      }
    }
  }// for j = 0 to 3;

  delay(1);
  digitalWrite(btncolumnpins1[current], HIGH);

  current++;
  if (current >= NUM_BTN_COLUMNS1)
  {
    current = 0;
  }
}

void scan4_4() {
  uint8_t val;
  static uint8_t current = 0;
  uint8_t i, j;

  digitalWrite(btncolumnpins2[current], LOW);
  delay(1);
  for ( j = 0; j < NUM_BTN_ROWS2; j++)
  {
    val = digitalRead(btnrowpins2[j]);

    if (val == LOW)
    {
      // active low: val is low when btn is pressed
      if ( debounce_count2[current][j] < MAX_DEBOUNCE)
      {
        debounce_count2[current][j]++;
        if ( debounce_count2[current][j] == MAX_DEBOUNCE )
        {

          if (selectInst == true) {
            tft.fillRect(20, 60, 175, 25, ILI9341_BLACK);
            nowInst = instName[(current * NUM_BTN_ROWS2) + j];
            Serial.print("nowInst: ");
            Serial.println((current * NUM_BTN_ROWS2) + j);
          }

          if (selectMetro == true)  {
            if ((current * NUM_BTN_ROWS2) + j == 12) {
              Serial.println("metroSpeed: 0");
            }
            if ((current * NUM_BTN_ROWS2) + j == 13) {
              Serial.println("metroSpeed: 1");
            }
          }

          if (selectInst == false && selectMetro == false) {
            Serial.print("btn");
            Serial.print((current * NUM_BTN_ROWS2) + j);
            Serial.print(":");
            Serial.println(" 1");
          }
          // Do whatever you want to with the button press here:
          // toggle the current LED state
          //if (selectInstru == true) checkInstru = (current * NUM_BTN_ROWS) + j;
        }
      }
    }
    else
    {

      if ( debounce_count2[current][j] > 0)
      {
        debounce_count2[current][j]--;
        if ( debounce_count2[current][j] == 0 )
        {
          if (selectInst == false && selectMetro == false) {
            Serial.print("btn");
            Serial.print((current * NUM_BTN_ROWS2) + j);
            Serial.print(":");
            Serial.println(" 0");
          }
        }
      }
    }
  }// for j = 0 to 3;
  delay(1);

  digitalWrite(btncolumnpins2[current], HIGH);
  current++;
  if (current >= NUM_BTN_COLUMNS2)
  {
    current = 0;
  }
}


void muxSetup() {
  pinMode(muxPin0, OUTPUT);
  pinMode(muxPin1, OUTPUT);
  pinMode(muxPin2, OUTPUT);
  pinMode(muxPin3, OUTPUT);
}
int readMux(int channel, int muxSig)
{
  digitalWrite(muxPin0, bitRead(channel, 0));
  digitalWrite(muxPin1, bitRead(channel, 1));
  digitalWrite(muxPin2, bitRead(channel, 2));
  digitalWrite(muxPin3, bitRead(channel, 3));

  // read from the selected  channel (A0)
  int muxValue = analogRead(muxSig);
  // return the  analog value
  return muxValue;
}

void muxValue() {
  static unsigned long previousMillis[2] = {0, 0};
  const long millis200 = 200;
  static int rawSlideVal[8];
  static int rawVolVal[11];
  static int newSlideVal[8];
  static int newVolVal[11];
  static int oldSlideVal[8] = {0, 0, 0, 0, 0, 0, 0, 0};
  static int oldVolVal[11] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  //  if ( millis() - previousMillis >= millis500) {
  //    previousMillis = millis();
  for (int i = 0; i < 8 ; i++)
  {

    rawSlideVal[i] = readMux(i, muxIn[0]); // read from a mux channel
    if (rawSlideVal[i] < 20) {
      newSlideVal[i] = 0;
    }
    else if (rawSlideVal[i] > 1000) {
      newSlideVal[i] = 1023;
    }
    else {
      newSlideVal[i] = rawSlideVal[i];
    }

    if (newSlideVal[i] < (oldSlideVal[i] - 10) || newSlideVal[i] > (oldSlideVal[i] + 10) ) {
      if ( millis() - previousMillis[0] >= millis200) {
        previousMillis[0] = millis();
        Serial.print("slide");
        Serial.print(i);
        Serial.print(": ");
        Serial.println(newSlideVal[i]);
        oldSlideVal[i] = newSlideVal[i];
      }
    }
    delayMicroseconds(50);
  }
  for (int i = 0; i < 11 ; i++)
  {
    rawVolVal[i] = readMux(i, muxIn[1]); // read from a mux channel
    if (rawVolVal[i] < 20) {
      newVolVal[i] = 0;
    }
    else if (rawVolVal[i] > 1000) {
      newVolVal[i] = 1023;
    }
    else {
      newVolVal[i] = rawVolVal[i];
    }
    if (newVolVal[i] < (oldVolVal[i] - 10) || newVolVal[i] > (oldVolVal[i] + 10) ) {
      if ( millis() - previousMillis[1] >= millis200) {
        previousMillis[1] = millis();
        Serial.print("vol");
        Serial.print(i);
        Serial.print(": ");
        Serial.println(newVolVal[i]);
        oldVolVal[i] = newVolVal[i];
      }
    }
    delayMicroseconds(50);
  }
}




void setupAllbutton() {
  for (int i = 0; i < 3; i++) {
    next_scan[i] = millis() + 1;
  }
  setuppins();
  setuppins1();
  setupButtonPad4_4();
}

void ScanAllbutton() {
  if (millis() >= next_scan[0])
  {
    next_scan[0] = millis() + 1;
    scan();
  }
  if (millis() >= next_scan[1])
  {
    next_scan[1] = millis() + 1;
    scan1();
  }
  if (millis() >= next_scan[2])
  {
    next_scan[2] = millis() + 1;
    scan4_4();
  }
}

void buttonSetup() {
  pinMode(15, INPUT);
  pinMode(16, INPUT);
  pinMode(19, INPUT);
  pinMode(18, INPUT);

}
void buttonPush() {
  for (int i = 0; i < 2; i++) {
    if (pushbutton[i].update()) {
      if (pushbutton[i].risingEdge()) {
        buttonState[i] = true;
        //        Serial.print("buttonState");
        //        Serial.print(i);
        //        Serial.print(": ");
        //        Serial.println(buttonState[i]);
      }
      else {
        buttonState[i] = false;
        //        Serial.print("buttonState");
        //        Serial.print(i);
        //        Serial.println(buttonState[i]);
      }
    }
  }
  for (int i = 0; i < 2; i++) {
    if (pushbutton2[i].update()) {
      if (pushbutton2[i].risingEdge()) {
        buttonState2[i] = true;
        Serial.print("stopClear");
        Serial.print(i);
        Serial.print(": ");
        Serial.println(buttonState2[i]);
      }
      else {
        buttonState2[i] = false;
        //        Serial.print("buttonState");
        //        Serial.print(i);
        //        Serial.println(buttonState[i]);
      }
    }
  }

}


void buttonCheckState() {
  if (buttonState[0] == true)
  {

    selectInst = true;
  }
  else {
    selectInst = false;
  }
  if (buttonState[1] == true)
  {

    selectMetro = true;
  }
  else {
    selectMetro = false;
  }
}

void changeColor() {
  if ( selectInst == true) {
    for (int i = 0; i < 15; i++) {
      if (nowInst == instName[i]  ) {
        for (int j = 0; j < NUMPIXELS; j++) {
          pixels.setPixelColor(j, neoColorArray[i]);
        }
      }
    }
  }
}
void setTFT() {
  pinMode(BACKLIGHT_PIN, OUTPUT );
  analogWrite(BACKLIGHT_PIN, 255); // 0 to 255
  Serial.begin(38400);
  tft.begin();
  tft.setRotation(1);
  tft.fillScreen(ILI9341_BLACK);
  ts.begin();
  ts.setRotation(1);
}


void drawTest() {
  static bool clearRoom  = true;
  for (roomNow  = 0; roomNow < roomSize; roomNow++) {
    if (roomSize == 1 && clearRoom  == true) {
      tft.fillRect(0, 0, 320, 20, ILI9341_BLACK);
      clearRoom  = false;
    }
    tft.fillRect(roomNow * 20, 0, 18, 20, colorBpm[roomNow]);
    if (roomSize == 16) {
      clearRoom  = true;
    }
  }
  tft.setFont(Arial_18);
  //  tft.setTextColor(neon);
  //  tft.setCursor(20, 30);
  //  tft.print("BPM : ");
  //
  //  tft.setTextColor(green);
  //  tft.print(bpm);

  //    tft.print("  ");
  tft.setTextColor(chaba);
  tft.setCursor(20, 60);
  tft.print(nowInst);
  //  tft.setTextColor(alien);
  //  tft.setCursor(200, 60);
  //  tft.print("oct  : ");
  //  tft.print("10");
  //  tft.setTextColor(red);
  //  tft.setCursor(20, 90);
  //  tft.print("Recording");

  for (int i = 0; i < 8; i++) {
    if (i < 4) {
      tft.drawRoundRect((i * 60) + 20, 120, 50, 50, 5, colorTft[i]);
    }
    else {
      tft.drawRoundRect(((i - 4) * 60) + 20, 180, 50, 50, 5, colorTft[i]);
    }
  }

}

void touchScreen() {
  static unsigned long previousMillis = 0;
  const long millis400 = 400;
  boolean istouched = ts.touched();
  //   Serial.println(istouched);
  if ( millis() - previousMillis >= millis400) {
    previousMillis = millis();
    if (istouched) {
      TS_Point p = ts.getPoint();
      if (!wastouched) {
      }
      x = map(p.x, 450, 4095, 1, 3);
      Serial.print("xpos: ");
      Serial.println(x);
      wastouched = istouched;
    }
  }
}

void drawWait() {
  tft.setFont(Arial_18);
  tft.setTextColor(neon);
  tft.setCursor(20, 30);
  tft.print("Waiting..");
}

void setup() {
  Serial.begin(9600);
  strip_0.strip.begin();
  pixelStart();
  buttonSetup();
  setupAllbutton();
  setTFT();
  muxSetup();
}

void loop() {
  serialPuredata();
  if (statCom == 0) {
    strips_loop();
    drawWait();
  }
  if (statCom == 1) {
    ScanAllbutton();
    buttonLongPress();
    pixels.show();
    buttonPush();
    buttonCheckState();
    changeColor();
    drawTest();
    loopBtn();
    muxValue();
    touchScreen();
  }
}


void pixelStart() {
  pixels.begin();
  pixels2.begin();
  for (int i = 0; i < NUMPIXELS2; i++) {
    pixels2.setPixelColor(i, neoColorOff2);
    pixels2.show();
  }
  for (int i = 0; i < NUMPIXELS; i++) {
    pixels.setPixelColor(i, neoColorArray[0]);
  }
}


void loopBtn() {
  static bool is_led[8] = {false, false, false, false, false, false, false, false};
  static unsigned long remTime[8] = {0, 0, 0, 0, 0, 0, 0, 0};
  unsigned long  period = 500;
  for (int i = 0; i < 8; i++) {
    if (loopStation[i] == 'p') {
      if (millis() - remTime[i] >= period ) {
        remTime[i] = millis() ;
        if (!is_led[i]) {
          is_led[i] = true;
          pixels2.setPixelColor(convertNeo(i), neoColorLoop[convertNeo(i)]); // Moderately bright green color.
          pixels2.show(); // This sends the updated pixel color to the hardware.
        } else {
          is_led[i] = false;
          pixels2.setPixelColor(convertNeo(i), neoColorOff2); // Moderately bright green color.
          pixels2.show(); // This sends the updated pixel color to the hardware.
        }
      }
      tft.setTextColor(ILI9341_YELLOW, ILI9341_BLACK);
      tft.setCursor(20, 90);
      tft.print("Pending");
      if (i < 4) {
        tft.fillRoundRect(((i * 60) + 20) + 5, 120 + 5, 40, 40, 5, colorTft[i]);
      } else {
        tft.fillRoundRect(((i - 4) * 60) + 25, 180 + 5, 40, 40, 5, colorTft[i]);
      }
    }
    if (loopStation[i] == 'r') {
      pixels2.setPixelColor(convertNeo(i), neoColorLoop[convertNeo(i)]);
      pixels2.show();
      tft.fillRect(20, 90, 300, 25, ILI9341_BLACK);
      tft.setTextColor(red, ILI9341_BLACK);
      tft.setCursor(20, 90);
      tft.print("Recording");
    }
    if (loopStation[i] == 'd') {
      tft.fillRect(20, 90, 300, 25, ILI9341_BLACK);
      if (mut[i] == 0) {
        if (i < 4) {
          tft.fillRoundRect(((i * 60) + 20) + 5, 120 + 5, 40, 40, 5, ILI9341_BLACK);
          tft.fillCircle(((i * 60) + 20) + 25, (120 + 5) + 20, 10, 0xFE36);
        } else {
          tft.fillRoundRect(((i - 4) * 60) + 25, 180 + 5, 40, 40, 5, ILI9341_BLACK);;
          tft.fillCircle(((i - 4) * 60) + 45, (180 + 5) + 20, 10, 0xFE36);
        }
        pixels2.setPixelColor(convertNeo(i), neoColorOff2);
        pixels2.show();
        mut[i] = 3;
      }
      if (mut[i] == 1) {
        if (i < 4) {
          tft.fillRoundRect(((i * 60) + 20) + 5, 120 + 5, 40, 40, 5, colorTft[i]);
          tft.fillCircle(((i * 60) + 20) + 25, (120 + 5) + 20, 10, 0xFE36);
        } else {
          tft.fillRoundRect(((i - 4) * 60) + 25, 180 + 5, 40, 40, 5, colorTft[i]);
          tft.fillCircle(((i - 4) * 60) + 45, (180 + 5) + 20, 10, 0xFE36);
        }
        pixels2.setPixelColor(convertNeo(i), neoColorLoop[convertNeo(i)]);
        pixels2.show();
        mut[i] = 3;
      }
      if (mut[i] == 2) {
        if (i < 4) {
          tft.fillRoundRect(((i * 60) + 20) + 5, 120 + 5, 40, 40, 5, ILI9341_BLACK);
        } else {
          tft.fillRoundRect(((i - 4) * 60) + 25, 180 + 5, 40, 40, 5, ILI9341_BLACK);;
        }
        pixels2.setPixelColor(convertNeo(i), neoColorOff2);
        pixels2.show();
        mut[i] = 3;
      }
    }
  }
}
int convertNeo(int i) {
  if (i == 0)cvtNeo = 2;
  if (i == 1)cvtNeo = 3;
  if (i == 2)cvtNeo = 6;
  if (i == 3)cvtNeo = 7;
  if (i == 4)cvtNeo = 0;
  if (i == 5)cvtNeo = 1;
  if (i == 6)cvtNeo = 4;
  if (i == 7)cvtNeo = 5;
  return cvtNeo;
}



void buttonLongPress() {
  for (int i = 0; i < 8; i++) {
    if (btnLoopState[i] == true && buttonStatePrevious[i] == false && !buttonStateLongPress[i]) {
      buttonLongPressMillis[i] = millis();
      buttonStatePrevious[i] = true;
      //Serial.println("Button press");
    }
    buttonPressDuration[i] = millis() - buttonLongPressMillis[i];


    if (btnLoopState[i] == true && !buttonStateLongPress[i] && buttonPressDuration[i] >= minButtonLongPressDuration) {
      buttonStateLongPress[i] = true;
      //      Serial.println("Button long pressed");
      Serial.print("btnClear");
      Serial.print(i);
      Serial.println(": 1");
      //      Serial.println(buttonStateLongPress[i]);

    }
    if (btnLoopState[i] == false && buttonStatePrevious[i] == true) {
      buttonStatePrevious[i] = false;
      buttonStateLongPress[i] = false;
      // Serial.println("Button released");

      if (buttonPressDuration[i] < minButtonLongPressDuration) {
        // Serial.println("Button pressed shortly");
        //        btnLoopState2[i] = !btnLoopState2[i];
        Serial.print("btnLoop");
        Serial.print(i);
        Serial.println(": 1");

      }
    }
  }
}

int cvtTft(int i) {
  if (i == 0)cvtToTft = 4;
  if (i == 1)cvtToTft = 5;
  if (i == 2)cvtToTft = 0;
  if (i == 3)cvtToTft = 1;
  if (i == 4)cvtToTft = 6;
  if (i == 5)cvtToTft = 7;
  if (i == 6)cvtToTft = 2;
  if (i == 7)cvtToTft = 3;
  return cvtToTft;
}

void strips_loop() {
  if (strip0_loop0() & 0x01)
    strip_0.strip.show();
}

uint8_t strip0_loop0() {
  uint8_t ret = 0x00;
  switch (strip0loop0.currentChild) {
    case 0:
      ret = strip0_loop0_eff0(); break;
  }
  if (ret & 0x02) {
    ret &= 0xfd;
    if (strip0loop0.currentChild + 1 >= strip0loop0.childs) {
      strip0loop0.currentChild = 0;
      if (++strip0loop0.currentTime >= strip0loop0.cycles) {
        strip0loop0.currentTime = 0;
        ret |= 0x02;
      }
    }
    else {
      strip0loop0.currentChild++;
    }
  };
  return ret;
}

uint8_t strip0_loop0_eff0() {
  // Strip ID: 0 - Effect: Rainbow - LEDS: 16
  // Steps: 60 - Delay: 20
  // Colors: 3 (255.0.0, 0.255.0, 0.0.255)
  // Options: rainbowlen=60, toLeft=true,
  if (millis() - strip_0.effStart < 20 * (strip_0.effStep)) return 0x00;
  float factor1, factor2;
  uint16_t ind;
  for (uint16_t j = 0; j < 16; j++) {
    ind = strip_0.effStep + j * 1;
    switch ((int)((ind % 60) / 20)) {
      case 0: factor1 = 1.0 - ((float)(ind % 60 - 0 * 20) / 20);
        factor2 = (float)((int)(ind - 0) % 60) / 20;
        strip_0.strip.setPixelColor(j, 255 * factor1 + 0 * factor2, 0 * factor1 + 255 * factor2, 0 * factor1 + 0 * factor2);
        break;
      case 1: factor1 = 1.0 - ((float)(ind % 60 - 1 * 20) / 20);
        factor2 = (float)((int)(ind - 20) % 60) / 20;
        strip_0.strip.setPixelColor(j, 0 * factor1 + 0 * factor2, 255 * factor1 + 0 * factor2, 0 * factor1 + 255 * factor2);
        break;
      case 2: factor1 = 1.0 - ((float)(ind % 60 - 2 * 20) / 20);
        factor2 = (float)((int)(ind - 40) % 60) / 20;
        strip_0.strip.setPixelColor(j, 0 * factor1 + 255 * factor2, 0 * factor1 + 0 * factor2, 255 * factor1 + 0 * factor2);
        break;
    }
  }
  if (strip_0.effStep >= 60) {
    strip_0.Reset();
    return 0x03;
  }
  else strip_0.effStep++;
  return 0x01;
}





void serialPuredata() {
  while (Serial.available() > 0)
  {
    char c = Serial.read();
    if (c == '-') {
      serIndex = 0;
      inData[serIndex] = '\0';
      serialStarted = true;
    } else if (c == '/') {
      serialEnded = true;
      break;
    } else {
      if (serIndex < 79) {
        inData[serIndex] = c;
        serIndex++;
        inData[serIndex] = '\0';
      }
    }

  }
  if (serialStarted && serialEnded) {
    String PrintString = String(inData);

    int commaIndex = PrintString.indexOf('_');
    int secondCommaIndex = PrintString.indexOf('_', commaIndex + 1);
    int thirdCommaIndex = PrintString.indexOf('_', secondCommaIndex + 1);


    String part1 = PrintString.substring(0, commaIndex);
    String part2 = PrintString.substring(commaIndex + 1, secondCommaIndex);
    String part3 = PrintString.substring(secondCommaIndex + 1, thirdCommaIndex);


    if (part1.equals("bpm")) {
      bpm = part2.toFloat();
      tft.fillRect(20, 30, 120, 30, ILI9341_BLACK);
      tft.setCursor(20, 30);
      tft.setTextColor(neon);
      tft.print("BPM : ");
      tft.setTextColor(green);
      tft.print(bpm);
    }
    if (part1.equals("room")) {
      roomSize = part2.toFloat();
    }
    if (part1.equals("bar")) {
      bar = part2.toFloat();
      tft.fillRect(200, 30, 50, 30, ILI9341_BLACK);
      tft.setCursor(200, 30);
      tft.setTextColor(white);
      tft.printf("%.1f", bar);
    }
    if (part1.equals("start")) {
      statCom = 1;
    }
    if (part1.equals("oct")) {
      oct = part2.toFloat();
      tft.fillRect(200, 60, 100, 30, ILI9341_BLACK);
      tft.setCursor(200, 60);
      tft.setTextColor(alien);
      tft.print("Oct :");
      tft.print(oct);
    }
    if (part1.equals("stop")) {
      if (part2.toFloat() == 1)
        tft.fillRect(0, 0, 320, 20, ILI9341_BLACK);
    }
    if (part1.equals("mut")) {
      mut[part2.toInt() - 1] = part3.toFloat();
    }
    if (part1.equals("loop")) {
      if (part2.equals("p")) {
        loopStation[part3.toInt() - 1] = 'p';
      }
      if (part2.equals("r")) {
        loopStation[part3.toInt() - 1] = 'r';
      }
      if (part2.equals("d")) {
        loopStation[part3.toInt() - 1] = 'd';
      }
    }
    //    if (part1.equals("clear")) {
    //      clearLoop[part2.toInt() - 1] = part3.toFloat();
    //    }
  }
  serialStarted = false;
  serialEnded = false;
  serIndex = 0;
  for (int i = 0; i < 80; i++) {
    inData[i] = '\0';
  }
}
