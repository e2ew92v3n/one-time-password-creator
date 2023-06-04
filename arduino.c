#include <Keypad.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2); // set the LCD address to 0x27 for a 16 chars and 2 line display
String pad;
String passwordList[50];
int passwordIndex = 0;
const byte numRows = 4;
const byte numCols = 4;
String password = "4321"; // 랜덤 1회용 비밀번호
char keypressed;
char keymap[numRows][numCols] =
{
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
//------------------------------------------------------------
byte rowPins[numRows] = {10, 9, 8, 7};
byte colPins[numCols] = {6, 5, 4, 3};
Keypad myKeypad = Keypad(makeKeymap(keymap), rowPins, colPins, numRows, numCols); // mapping keypad

const int redPin = 11;   // 빨간색 RGB LED에 연결된 핀
const int greenPin = 12; // 초록색 RGB LED에 연결된 핀
const int bluePin = 13;  // 파란색 RGB LED에 연결된 핀

void setup() {
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("ENTER");
  lcd.setCursor(0, 1);
  lcd.print("PASSWORD");
  delay(1000);
  lcd.clear();

  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  RGB_LED_OFF(); // 초기에는 RGB LED를 끄도록 설정합니다.
}

void loop() {
  readKeypad();
  pad = "";

  if (keypressed == '#') {
    // 비밀번호 배열에서 비밀번호가 존재하는지 체크하는 if문
    if (checkPassword(pad)) {
      lcd.setCursor(0, 1);
      lcd.print("DOOR OPEN");
      RGB_LED_GREEN();

      deletePassword(pad); // 비밀번호를 사용하면 삭제
      sendPassword(pad);

      delay(3000);
      RGB_LED_OFF();
    } else {
      lcd.setCursor(0, 1);
      lcd.print("WRONG PASSWORD");
      RGB_LED_RED();
      delay(3000);
      RGB_LED_OFF();
    }
  } else if (keypressed == '*') {
    pad = "";
    lcd.clear();
  }

  lcd.setCursor(0, 0);
  lcd.print(pad);
  delay(100);
}

void readKeypad() {
  keypressed = myKeypad.getKey(); // 키패드 입력 감지

  if (keypressed != '#' && keypressed != NO_KEY) {
    String konv = String(keypressed);
    pad += konv;
  }
}

// RGB LED 제어 함수
void RGB_LED_OFF() {
  digitalWrite(redPin, LOW);
  digitalWrite(greenPin, LOW);
  digitalWrite(bluePin, LOW);
}

void RGB_LED_RED() {
  digitalWrite(redPin, HIGH);
  digitalWrite(greenPin, LOW);
  digitalWrite(bluePin, LOW);
}

void RGB_LED_GREEN() {
  digitalWrite(redPin, LOW);
  digitalWrite(greenPin, HIGH);
  digitalWrite(bluePin, LOW);
}

// 비밀번호 배열을 조회해서 비밀번호가 있는지 체크하는 함수
bool checkPassword(String enteredPassword) {
  for (int i = 0; i < passwordIndex; i++) {
    if (passwordList[i] == enteredPassword) {
      return true;
    }
  }
  return false;
}

// 사용한 비밀번호를 배열에서 제거하는 함수
void deletePassword(String enteredPassword) {
  for (int i = 0; i < passwordIndex; i++) {
    if (passwordList[i] == enteredPassword) {
      for (int j = i; j < passwordIndex - 1; j++) {
        passwordList[j] = passwordList[j + 1];
      }
      passwordIndex--;
      break;
    }
  }
}

// 비밀번호를 사용하면 파이썬으로 전송함(파이썬에서도 삭제하기 위해)
void sendPassword(String passwordToSend) {
  Serial.println(passwordToSend);
}