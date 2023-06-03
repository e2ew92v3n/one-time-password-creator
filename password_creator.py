import random
import serial

class PasswordCreator:
    def __init__(self) -> None:
        # 비밀번호 리스트 생성
        self.password_list = []
        #self.arduino = serial.Serial('COM3', 9600)
        # 아두이노 연결안하면 오류발생
        
    def create_password(self):
        password = self.__new_password
        self.password_list.append(password) # 리스트에 비밀번호를 새로 생성해서 추가
        self.__send_password(password) # 아두이노에 비밀번호 전달하기
        
    def delete_password(self, password):
        # 비밀번호 사용 후 제거
        if password in self.password_list:
            self.password_list.remove(password)
            
    def my_password(self):
        # 생성한 비밀번호 목록 보기
        print(self.password_list)
                    
    def __send_password(self, password):
        if password in self.password_list:
            self.arduino.write(password.encode())
            
    def __new_password(self):
        temp_number = []
        for i in range(4):
            temp_number.append(random.randint(1, 9))
        password = ''.join(map(str, temp_number))
        return password