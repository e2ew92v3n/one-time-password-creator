import os
import random
import glob
from cryptography.hazmat.primitives import serialization, hashes
from cryptography.hazmat.primitives.asymmetric import rsa, padding
from cryptography.hazmat.backends import default_backend

def generate_key(user) -> None:
    # 공개키 비밀키 생성하기
    private_key = rsa.generate_private_key(
        public_exponent = 65537,
        key_size = 2048,
        backend = default_backend()
    )
    
    public_key = private_key.public_key()
    private_key_file = f"private_key_{user}".pem
    public_key_file = f"public_key_{user}".pem
    
    with open(private_key_file, "wb") as f:
        f.write(private_key.private_bytes(
            encoding = serialization.Encoding.PEM,
            format = serialization.PrivateFormat.PKCS8,
            encryption_algorithm = serialization.NoEncryption()
        ))
        
    with open(public_key_file, "wb") as f:
        f.write(public_key.public_bytes(
            encoding = serialization.Encoding.PEM,
            format = serialization.PublicFormat.SubjectPublicKeyInfo
        ))
    
class PasswordListNode:
    def __init__(self, name, password, next) -> None:
        self.name = name
        self.password = password
        self.next = next

class PasswordLinkedList:
    def __init__(self) -> None:
        self.__head = PasswordListNode("dummy", "dummy", None)
        self.__num_password = 0
        
    def __get_node(self, index):
        # 인덱스의 노드 값 반환
        curr = self.__head
        for i in range(index + 1):
            curr = curr.next
        return curr
    
    def __find_node(self, password):
        # 리스트의 노드 찾기
        prev = self.__head
        curr = prev.next
        while curr is not None:
            if curr.password == password:
                return(prev, curr)
            else:
                prev = curr
                curr = curr.next
        return None, None
    
    def append(self, name):
        # 리스트에 암호화된 비밀번호 추가
        password = self.__new_password()
        prev = self.__get_node(self.__num_password - 1)
        new_node = PasswordListNode(name, password, prev.next)
        prev.next = new_node
        self.__num_password += 1
    
    def remove(self, password):
        # 리스트에서 암호화된 비밀번호 삭제
        (prev, curr) = self.__find_node(password)
        if curr is not None:
            prev.next = curr.next
            self.__num_password -= 1
        else:
            return None
        
    def password_list(self) -> None:
        # 비밀번호 발급 목록 출력
        pw = []
        curr = self.__head.next
        while curr is not None:
            pw.append(curr.name)
            curr = curr.next
        print(pw)
        
    def get_public_key_list():
        # 가지고 있는 공개키의 목록 출력
        user = []
        public_key_files = glob.glob("public_key_*.pem")
        for public_key_file in public_key_files:
            name = os.path.splitext(public_key_file)[0].replace("public_key_", "")
            user.append(name)
        return user
    
    @staticmethod
    def find_user_public_key(user):
        # 출입자(이름)으로 암호화하기 위해 공개키 찾기
        public_key_file = f"public_key_{user}.pem"
        with open(public_key_file, "rb") as f:
            public_key = serialization.load_pem_public_key(
                f.read(),
                backend = default_backend()
            )
        return public_key
    
    @staticmethod
    def __new_password(user):
        # 비밀번호 생성후 user의 공개키로 암호화
        public_key = PasswordLinkedList.find_user_public_key(user)
        temp_number = []
        for i in range(10):
            temp_number.append(random.randint(1, 9))
        password = ''.join(map(str, temp_number))
        encrypted_password = public_key.encrypt(
            password.encode(),
            padding.OAEP(
                mgf = padding.MGF1(algorithm = hashes.SHA256()),
                algorithm = hashes.SHA256(),
                label = None
            )
        )
        return encrypted_password