from password_creator import *
from menu import *
import serial
import time

def main():
    port = "COM3"
    baudrate = 9600
    my_password = PasswordCreator()
    while True:
        # 메뉴 선택시 함수 작동
        select = menu()
        if select == 1:
            continue
        elif select == 2:
            continue
        elif select == 3:
            continue
        elif select == 4:
            print("종료")
            break

if __name__ == "__main__":
    main()