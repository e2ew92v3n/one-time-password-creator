#include <SoftwareSerial.h>
#include <Keypad.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>

SoftwareSerial HC06(2, 3);
int passwordArray[4];
int i = 0;
LiquidCrystal_I2C lcd(0x27, 16, 2); // LCD 주소를 0x27로 설정, 16열 2행 디스플레이
String pad;
String receivedString; // 임시비밀번호 저장
const byte numRows = 4;
const byte numCols = 4;
String password; // 랜덤 4자리 비밀번호
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
Keypad myKeypad = Keypad(makeKeymap(keymap), rowPins, colPins, numRows, numCols); // 키패드 매핑


const int redPin = 11;   // 빨간색 RGB LED 핀
const int greenPin = 12; // 초록색 RGB LED 핀
const int bluePin = 13;  // 파란색 RGB LED 핀

void setup() {
  Serial.begin(9600);
  HC06.begin(9600);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("ENTER");
  lcd.setCursor(0, 1);
  lcd.print("PASSWORD");
  delay(1000);
  lcd.clear();
  Serial.begin(9600);

  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  RGB_LED_OFF(); // 초기에는 RGB LED를 끄도록 설정

  HC06.begin(9600); // 블루투스 모듈과의 시리얼 통신 속도 설정


}

void loop() {
  readKeypad();
  if (HC06.available()) {
    char receivedChar = HC06.read();
    
    // 숫자인지 확인
    if (isDigit(receivedChar)) {
      // 문자를 숫자로 변환하여 배열에 저장
      passwordArray[i] = receivedChar - '0';
      Serial.print(passwordArray[i]);
      i++;
      
      // 4자리 숫자를 모두 받았을 때
      if (i == 4) {
        password = passwordArrayToString(passwordArray);
        Serial.print(" Converted: ");
        Serial.println(password);
        
        i = 0; // 다음 숫자를 위해 인덱스 초기화
      }
    }
  }
  if (keypressed == '#') {
    Serial.println(pad);
    Serial.print(password);
      if (pad == password) {
        lcd.setCursor(0, 1);
        lcd.print("DOOR OPEN");
        RGB_LED_GREEN();
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
  keypressed = myKeypad.getKey(); // 키패드 입력 읽어오기
  if (keypressed != NO_KEY && keypressed != '#') {
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

String passwordArrayToString(int arr[]) {
  String password = "";
  
  for (int j = 0; j < 4; j++) {
    password += String(arr[j]);
  }
  
  return password;
}
