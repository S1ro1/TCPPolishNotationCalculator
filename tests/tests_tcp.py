import pytest
import socket

HOST = "127.0.0.1"
PORT = 2023

TEST_DATA_TCP = [
    (b"HELLO\nSOLVE (+ 1 2)\nBYE\n", b"HELLO\nRESULT 3\nBYE\n"),
    (b"HELLO\nSOLVE (* 3 4)\nBYE\n", b"HELLO\nRESULT 12\nBYE\n"),
    (b"HELLO\nSOLVE (- 10 4)\nBYE\n", b"HELLO\nRESULT 6\nBYE\n"),
    (b"HELLO\nSOLVE (/ 12 3)\nBYE\n", b"HELLO\nRESULT 4\nBYE\n"),
    (b"HELLO\nSOLVE (+ 1 (* 2 3))\nBYE\n", b"HELLO\nRESULT 7\nBYE\n"),
    (b"hell\nSOLVE\n", b"BYE\n"),
    (b"HELLO\nSOLVE (+ 1 2\n", b"HELLO\nBYE\n"),
    (b"HELLO\nSOLVE (/ 1 0\n", b"HELLO\nBYE\n"),
    (b"HELLO\nSOLVE (+ (+ 3 9) (+ (+ 2 (+ 6 1) 4) 9 7) 5)\nBYE\n", b"HELLO\nRESULT 46\nBYE\n"),
    (b"HELLO\nSOLVE (- (- 3 9) (- (- 2 (- 6 1) 4) 9 7) 5)\nBYE\n", b"HELLO\nRESULT 12\nBYE\n"),
    (b"HELLO\nSOLVE (* (* 3 9) (* (* 2 (* 6 1) 4) 9 7) 5)\nBYE\n", b"HELLO\nRESULT 408240\nBYE\n"),
    (b"HELLO\nSOLVE (/ (/ 7000 9) (/ (/ 86207 (/ 16 1) 4) 2 7) 5)\nBYE\n", b"HELLO\nRESULT 1\nBYE\n"),
    (b"HELLO\nSOLVE (+ 1 (* 45 (+ 3 4)) (* 3 6) (/ 7 8) (/ 21 4))\nBYE\n", b"HELLO\nRESULT 339\nBYE\n"),
    (b"HELLO\nSOLVE (/ (* (- 64 4) (+ 3 2)) (- 12 4) (* (+ (/ 10 2) (- 7 4)) (/ (- 8 2) 3)))\nBYE\n", b"HELLO\nRESULT 2\nBYE\n")
]

@pytest.mark.parametrize('message, expected', TEST_DATA_TCP)
def test_tcp(message, expected):
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.connect((HOST, PORT))
    s.sendall(message)
    response = b""
    while True:
        data = s.recv(1024)
        print(data)
        if not data:
            break
        response += data
    s.close()

    assert response == expected
