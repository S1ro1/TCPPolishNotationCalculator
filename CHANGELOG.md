### Known limitations
- Only linux supported
- In case of TCP receiving message f.e. "HELLO\nSOLVE (+1" , then second part of the message (SOLVE ...) will be ignored, because it doesn't contain newline character, and server will wait for another message with \n
- Only set order of arguments is supported


Author: S1ro1 <matej.sirovatka@gmail.com>
Date:   Mon Apr 17 17:01:20 2023 +0200

    Add readme

Author: S1ro1 <matej.sirovatka@gmail.com>
Date:   Mon Apr 17 16:00:12 2023 +0200

    Fix error in tcp connection, add simple tests, add charts to readme

Author: S1ro1 <matej.sirovatka@gmail.com>
Date:   Mon Apr 17 14:15:16 2023 +0200

    Throw error with negative results, check for unwanted exits and replace with exception throws

Author: S1ro1 <matej.sirovatka@gmail.com>
Date:   Mon Apr 17 02:19:31 2023 +0200

    Add support for one query in multiple messages, small refactoring, fix format of tcp reply

Author: S1ro1 <matej.sirovatka@gmail.com>
Date:   Mon Apr 17 01:39:25 2023 +0200

    Add support for one message in multiple queries, small refactoring, fix format of tcp reply

Author: S1ro1 <matej.sirovatka@gmail.com>
Date:   Mon Apr 17 00:31:44 2023 +0200

    Small fixes in udp

Author: S1ro1 <matej.sirovatka@gmail.com>
Date:   Sun Apr 16 23:59:23 2023 +0200

    Small refactor, fix address in use error

Author: S1ro1 <matej.sirovatka@gmail.com>
Date:   Sun Apr 16 22:19:55 2023 +0200

    Add support for multiple queries in 1 message for tcp

Author: S1ro1 <matej.sirovatka@gmail.com>
Date:   Sun Apr 16 21:58:22 2023 +0200

    Add Sigint handler for tcp connection, small refactor

Author: S1ro1 <matej.sirovatka@gmail.com>
Date:   Sun Apr 16 20:53:10 2023 +0200

    First version of TCP added

Author: S1ro1 <matej.sirovatka@gmail.com>
Date:   Sun Apr 16 14:24:06 2023 +0200

    Add handling of division by zero

Author: S1ro1 <matej.sirovatka@gmail.com>
Date:   Sat Apr 15 00:45:15 2023 +0200

    Change parser to throw exceptions, refactor code and fix small bugs in udp

Author: S1ro1 <matej.sirovatka@gmail.com>
Date:   Sat Apr 15 00:25:24 2023 +0200

    Connect udp to parser, base version of udp working

Author: S1ro1 <matej.sirovatka@gmail.com>
Date:   Fri Apr 14 23:56:37 2023 +0200

    Add udp listen

Author: S1ro1 <matej.sirovatka@gmail.com>
Date:   Fri Apr 14 22:39:24 2023 +0200

    Small refactoring, add better logging and exit function

Author: S1ro1 <matej.sirovatka@gmail.com>
Date:   Fri Apr 14 03:18:59 2023 +0200

    Add parsing of expressions
