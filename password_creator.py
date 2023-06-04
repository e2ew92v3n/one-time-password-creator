import random
import serial
import os
import time

class PasswordCreator:
    def __init__(self) -> None:
        # 비밀번호 리스트 생성
        self.password_list = []
        self.arduino = serial.Serial('COM3', 9600)
        self.delete_list = []
        
    def create_password(self):
        password = self.__new_password()
        self.password_list.append(password) # 리스트에 비밀번호를 새로 생성해서 추가
        self.__send_password(password) # 아두이노에 비밀번호 전달하기
        print("-----------------")
        print("비밀번호 생성 완료")
        print("-----------------")
        time.sleep(1)
        
    def delete_password(self):
        # 비밀번호 사용 후 제거
        for password in self.delete_list:
            if password in self.password_list:
                self.password_list.remove(password)
        self.delete_list.clear()
            
    def my_password(self):
        # 생성한 비밀번호 목록 보기
        os.system('cls' if os.name == 'nt' else 'clear')
        print("-----------------")
        print("내 비밀번호")
        print("-----------------")
        for i, password in enumerate(self.password_list):
            print(f"{i + 1}. {password} - {self.__send_password(password)}")
        print("-----------------")
        print("1. 뒤로가기")
        select = input("번호선택 : ")
        return int(select)
                    
    def __send_password(self, password):
        if password in self.password_list:
            self.arduino.write(password.encode())
            return "delivered"
        else:
            return "fail"
            
    def __new_password(self):
        temp_number = []
        for i in range(4):
            temp_number.append(random.randint(1, 9))
        password = ''.join(map(str, temp_number))
        return password

def menu():
    os.system('cls' if os.name == 'nt' else 'clear')
    print("-----------------")
    print(" 비밀번호 관리자")
    print("-----------------")
    print("1. 임시 비밀번호 생성하기")
    print("2. 내 비밀번호 보기")
    print("3. 비밀번호 사용내역")
    print("4. 종료")
    select = input("번호선택 : ")
    return int(select)