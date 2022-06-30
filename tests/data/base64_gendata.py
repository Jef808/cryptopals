#!/usr/bin/env python

import base64
import random

if __name__ == '__main__':
    for i in range(0, 3):
        test_input = bytearray(random.randint(1, 255) for _ in range(1022 + i))# + b'\x00'
        #test_input = b"".join(bytes(chr(random.randint(0, 255)), encoding="utf-8") for _ in range (1020 + i)) + b"\x00"
        test_expected_output = str(base64.standard_b64encode(test_input), encoding="utf-8")
        with open(f"test_base64_input{i+1}", "wb+") as file:
            file.write(test_input)
        with open(f"test_base64_expected_output{i+1}.txt", "w+") as file:
            file.write(test_expected_output)
