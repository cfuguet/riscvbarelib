#!/usr/bin/env python3
##
#  Copyright 2023,2024 CEA*
#  Commissariat a l'Energie Atomique et aux Energies Alternatives
#
#  Licensed under the Apache License, Version 2.0 (the "License");
#  you may not use this file except in compliance with the License.
#  You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
#  Unless required by applicable law or agreed to in writing, software
#  distributed under the License is distributed on an "AS IS" BASIS,
#  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#  See the License for the specific language governing permissions and
#  limitations under the License.
##
##
#  @file   scripts/bin2mem.py
#  @author Cesar Fuguet
#  @date   February, 2020
##
import binascii
import sys
import os
import argparse

def bin2hex(binfile, hexfile, wordbytes):
    word = binfile.read(wordbytes)
    fmt = "{:0>" + str(wordbytes*2) + "}\n"
    while word != b"":
        # byte swap the read word
        word = word[::-1]

        # convert the read word into hexadecimal
        word = bytes.decode(binascii.hexlify(word))

        # write into the output hexadecimal file
        hexfile.write(fmt.format(word))

        # read the next word
        word = binfile.read(wordbytes)

def main(args):
    # check that the input file exists
    if not os.path.lexists(args.infile):
        print('error: input file does not exist... ')
        sys.exit(1)

    # start converting the input binary file into hexadecimal
    with open(args.infile, "rb") as binfile:
        with open(args.outfile, "w") as hexfile:
            bin2hex(binfile, hexfile, args.outwidth)

if __name__ == '__main__':
    parser = argparse.ArgumentParser(
        prog='bin2mem',
        usage='%(prog)s [options]',
        description='Convert binary file into hex file'
    )

    parser.add_argument('--in', dest='infile', type=str, action='store',
        help='Input binary file')
    parser.add_argument('--out', dest='outfile', type=str, action='store',
        help='Output hex file')
    parser.add_argument('--outw', dest='outwidth', type=int, action='store',
        help='Output width (in number of bytes) of the hex file')

    main(parser.parse_args())
