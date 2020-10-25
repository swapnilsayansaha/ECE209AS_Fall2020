// What: Emulate classic T9 keypad (without text prediction) on Arduino
// Where: https://github.com/ZulNs/MultitapKeypad
// Why: The code is heavily modified to be used with Python for old-school texting
// with our developed hardware
// Modifying existing code saves time for 
// developing the major components of the system


#include <MultitapKeypad.h>


const byte ROW0 = 2;
const byte ROW1 = 3;
const byte ROW2 = 4;
const byte ROW3 = 5;
const byte COL0 = 8;
const byte COL1 = 9;
const byte COL2 = 10;
const byte COL3 = 11;

const byte CHR_BOUND = 3;
const byte BACKSPACE = 8;
const byte CLEARSCREEN = 12;
const byte CARRIAGE_RETURN = 13;
const byte CAPSLOCK_ON = 17;
const byte CAPSLOCK_OFF = 18;
const byte NUMLOCK_ON = 19;
const byte NUMLOCK_OFF = 20;

const char SYMBOL[] PROGMEM = {
  '\b', '\b', '\b', '\b', '\b',
  '\b', '\b', '\b','\b', '\b',
  '\b', '\b', '\b', '\b', '\b',
  '\b', '\b', '\b', '\b', '\b',
  '\b', '\b', '\b', '\b', CHR_BOUND
};
const char ALPHABET[] PROGMEM = {
  'A', 'B', 'C', CHR_BOUND, CHR_BOUND,
  'D', 'E', 'F', CHR_BOUND, CHR_BOUND,
  'G', 'H', 'I', CHR_BOUND, CHR_BOUND,
  'J', 'K', 'L', CHR_BOUND, CHR_BOUND,
  'M', 'N', 'O', CHR_BOUND, CHR_BOUND,
  'P', 'Q', 'R', 'S',       CHR_BOUND,
  'T', 'U', 'V', CHR_BOUND, CHR_BOUND,
  'W', 'X', 'Y', 'Z',       CHR_BOUND
};

boolean alphaMode = true;
boolean upperCaseMode = true;
boolean isEndOfDisplay = false;
byte startCursorPos;
byte endCursorPos;
byte cursorPos;
byte chrCtr;
char strBuffer[16];

MultitapKeypad kpd(ROW0, ROW1, ROW2, ROW3, COL0, COL1, COL2);
Key key;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);
  char chr;
  chr = '1';
  getString(21, 31);
  getString(21, 26);
}

void loop() {
  
}



void getString(byte startPos, byte endPos) {
  char chr;
  byte strSize = endPos - startPos + 1;
  startCursorPos = startPos;
  endCursorPos = endPos;
  cursorPos = startPos;
  chrCtr = 0;
  boolean isCompleted = false;
  while (true) {
    key = kpd.getKey();
    switch (key.state) {
      case KEY_DOWN:
      case MULTI_TAP:
        digitalWrite(LED_BUILTIN, HIGH);
        switch (key.code) {
          case KEY_1:
            chr = alphaMode ? getSymbol(key.tapCounter) : '1';
            break;
          case KEY_ASTERISK:
            chr = '*';
            break;
          case KEY_0:
            chr = alphaMode ? ' ' : '0';
            break;
          case KEY_NUMBER_SIGN:
            Serial.print('#');
            if (alphaMode) {
              chr = upperCaseMode ? CAPSLOCK_ON : CAPSLOCK_OFF;
            }
            else
              chr = key.character;
            break;
          case KEY_A:
            chr = BACKSPACE;
            break;
          case KEY_B:
          case KEY_C:
            chr = 0;
            break;
          case KEY_D:
            strBuffer[chrCtr] = 0;
            isCompleted = true;
            break;
          default:
            chr = alphaMode ? getAlphabet(key.character, key.tapCounter) : key.character;
        }
        if (isCompleted)
          break;
        if (!upperCaseMode && chr >= 'A')
          chr += 32; // makes lower case
        if (key.state == MULTI_TAP && alphaMode && key.character >= '1' && key.character <= '9') {
          Serial.print('\b');
          printToLcd(BACKSPACE);
          chrCtr--;
        }
        if (chr > 0) {
          if (chr == BACKSPACE)
            chrCtr--;
          if (chr >= ' ') {
            strBuffer[chrCtr] = chr;
            if (chrCtr < strSize)
              chrCtr++;
          }
          printToLcd(chr);
        }
        break;
      case LONG_TAP:
        switch (key.code) {
          case KEY_A:
            chr = CLEARSCREEN;
            break;
          case KEY_NUMBER_SIGN:
            if (alphaMode)
              upperCaseMode = !upperCaseMode;
            alphaMode = !alphaMode;
            chr = alphaMode ? NUMLOCK_OFF : NUMLOCK_ON;
            break;
          default:
            chr = key.character;
        }
        if (chr < ' ' || alphaMode && chr >= '0' && chr <= '9') {
          if (chr >= '0' || chr == NUMLOCK_OFF) {
            Serial.print('\b');
            printToLcd(BACKSPACE);
            chrCtr--;
          }
          if (chr == CLEARSCREEN)
            chrCtr = 0;
          if (chr >= ' ') {
            strBuffer[chrCtr] = chr;
            if (chrCtr < strSize)
              chrCtr++;
          }
          printToLcd(chr);
        }
        break;
      case MULTI_KEY_DOWN:
        digitalWrite(LED_BUILTIN, LOW);
        break;
      case KEY_UP:
        digitalWrite(LED_BUILTIN, LOW);
        if (isCompleted)
          return;
    }
  }
}

void printToLcd(char chr) {
  switch (chr) {
    case BACKSPACE:
      if (cursorPos > startCursorPos) {
        isEndOfDisplay = false;
      }
      break;
    case CLEARSCREEN:
      while (cursorPos > startCursorPos) {
        Serial.print(F(" "));
      }
      break;
    case CAPSLOCK_ON:
    case CAPSLOCK_OFF:
    case NUMLOCK_ON:
    case NUMLOCK_OFF:
      break;
    default:
      if (cursorPos == endCursorPos)
        isEndOfDisplay = true;
      Serial.print(chr);
  }
}


byte getSymbol(byte ctr) {
  byte sym = pgm_read_byte_near(SYMBOL + ctr);
  if (sym == CHR_BOUND) {
    sym = pgm_read_byte_near(SYMBOL);
    kpd.resetTapCounter();
  }
  return sym;
}

byte getAlphabet(byte chr, byte ctr) {
  chr = (chr - '2') * 5;
  byte alpha = pgm_read_byte_near(ALPHABET + chr + ctr);
  if (alpha == CHR_BOUND) {
    alpha = pgm_read_byte_near(ALPHABET + chr);
    kpd.resetTapCounter();
  }
  return alpha;
}
