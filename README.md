


### UDP Test cases
| Input | Output | Status |
|-------|--------| ------ |
| b"\000\007(+ 1 2)" | b"\x01\x00\x013" | Passed |
| b"\000\007(* 3 4)" | b"\x01\x00\x0212" | Passed |
| b"\000\010(- 10 4)" | b"\x01\x00\x016" | Passed |
| b"\000\010(/ 12 3)" | b"\x01\x00\x014" | Passed |
| b"\000\017(+ 1 (* 2 3))" | b"\x01\x00\x017" | Passed |
| b"\x01\x01" | b"\x01\x01\x05Error" | Passed |
| b"\x00\x07(+ 1 2)345" | b"\x01\x00\x013" | Passed |
| b"\x00\x07(+ 1 2" | b"\x01\x01\x1aCouldn't parse expression!" | Passed |

### TCP Test cases
| Input | Output | Status |
|-------|--------| ------ |
| b"HELLO\nSOLVE (+ 1 2)\nBYE\n" | b"HELLO\nRESULT 3\nBYE\n" | Passed |
| b"HELLO\nSOLVE (* 3 4)\nBYE\n" | b"HELLO\nRESULT 12\nBYE\n" | Passed |
| b"HELLO\nSOLVE (- 10 4)\nBYE\n" | b"HELLO\nRESULT 6\nBYE\n" | Passed |
| b"HELLO\nSOLVE (/ 12 3)\nBYE\n" | b"HELLO\nRESULT 4\nBYE\n" | Passed |
| b"HELLO\nSOLVE (+ 1 (* 2 3))\nBYE\n" | b"HELLO\nRESULT 7\nBYE\n" | Passed |
| b"hell\nSOLVE\n" | b"BYE\n" | Passed |
| b"HELLO\nSOLVE (+ 1 2\n" | b"HELLO\nBYE\n" | Passed |
| b"HELLO\nSOLVE (/ 1 0\n" | b"HELLO\nBYE\n" | Passed |
| b"HELLO\nSOLVE (+ (+ 3 9) (+ (+ 2 (+ 6 1) 4) 9 7) 5)\nBYE\n" | b"HELLO\nRESULT 46\nBYE\n" | Passed |
| b"HELLO\nSOLVE (- (- 3 9) (- (- 2 (- 6 1) 4) 9 7) 5)\nBYE\n" | b"HELLO\nRESULT 12\nBYE\n" | Passed |
| b"HELLO\nSOLVE (* (* 3 9) (* (* 2 (* 6 1) 4) 9 7) 5)\nBYE\n" | b"HELLO\nRESULT 408240\nBYE\n" | Passed |
| b"HELLO\nSOLVE (/ (/ 7000 9) (/ (/ 86207 (/ 16 1) 4) 2 7) 5)\nBYE\n" | b"HELLO\nRESULT 1\nBYE\n" | Passed |
| b"HELLO\nSOLVE (+ 1 (* 45 (+ 3 4)) (* 3 6) (/ 7 8) (/ 21 4))\nBYE\n" | b"HELLO\nRESULT 339\nBYE\n" | Passed |
| b"HELLO\nSOLVE (/ (* (- 64 4) (+ 3 2)) (- 12 4) (* (+ (/ 10 2) (- 7 4)) (/ (- 8 2) 3)))\nBYE\n" | b"HELLO\nRESULT 2\nBYE\n" | Passed |
