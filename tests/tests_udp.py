import socket
import pytest

HOST = "127.0.0.1"
PORT = 2023

TEST_DATA_UDP = [
    (b"\000\007(+ 1 2)", b"\x01\x00\x013"),
    (b"\000\007(* 3 4)", b"\x01\x00\x0212"),
    (b"\000\010(- 10 4)", b"\x01\x00\x016"),
    (b"\000\010(/ 12 3)", b"\x01\x00\x014"),
    (b"\000\017(+ 1 (* 2 3))", b"\x01\x00\x017"),
    (b"\x01\x01", b"\x01\x01\x05Error"),
    (b"\x00\x07(+ 1 2)345", b"\x01\x00\x013",),
    (b"\x00\x07(+ 1 2", b"\x01\x01\x1aCouldn't parse expression!"),
]

@pytest.mark.parametrize('message, expected', TEST_DATA_UDP)
def test_udp(message, expected):
    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    s.sendto(message, (HOST, PORT))
    response = s.recv(1024)
    print(response)
    s.close()

    assert response == expected
