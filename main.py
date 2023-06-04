from password_creator import *

def main():
    my_password = PasswordCreator()
    time.sleep(2)
    password_to_delete = my_password.arduino.readline().decode().strip()
    
    my_password.delete_list.append(password_to_delete)
    
    while True:
        # 메뉴 선택시 함수 작동
        select = menu()
        if select == 1:
            my_password.create_password()
        elif select == 2:
            if my_password.my_password() == 1:
                continue
            for password in my_password.delete_list:
                my_password.delete_password(password)
        elif select == 3:
            continue
        elif select == 4:
            print("종료")
            break
    
    my_password.arduino.close()

if __name__ == "__main__":
    main()