def menu():
    print("-----------------")
    print(" 비밀번호 관리자")
    print("-----------------")
    print("1. 임시 비밀번호 생성하기")
    print("2. 내 비밀번호 보기")
    print("3. 비밀번호 사용내역")
    print("4. 종료")
    select = input("번호선택 : ")
    return int(select)