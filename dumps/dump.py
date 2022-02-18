import re
import sys
import os

POINTER_ADDR = "30010" #"F27A90"
NB_POINTERS = "87C"
ROM_OFFSET = "0" #"8000000"
TEXT_OFFSET = "60010" #"0"
ADDR_SIZE = 3 #4
ENDIAN = "little"
CC_BYTE = 3

def initCharTable(filename):
    table = {}
    charFile = open(filename,"r",encoding="utf-8")
    charLines = charFile.readlines()   
    for charLine in charLines:
        lineMatch = re.match("^(..) (.*)$", charLine)
        if(lineMatch):
            charCode = int(lineMatch.group(1),16)
            char = lineMatch.group(2)
            charMatch = re.match("\[.*\]", char)
            if (len(char) > 1 and not charMatch):
                char = "[" + char + "]"
            table[charCode] = char
    charFile.close()
    return table

def initCCList():
    lst = {}
    lst[2]="PAUSE"
    return lst

def dumpPointers(file):
    file.seek(int(POINTER_ADDR,16))
    pointers = []
    for i in range(int(NB_POINTERS,16)):
        addr = int.from_bytes(file.read(ADDR_SIZE), ENDIAN)
        pointers.append(addr - int(ROM_OFFSET,16) + int(TEXT_OFFSET,16))
    return pointers

def dumpLines(file, pointers, charTable, ccList):
    lines = []
    for i in range(int(NB_POINTERS,16)):
        dumpedLine = ""
        if (pointers[i] > 0):
            file.seek(pointers[i])
            curByte = int.from_bytes(file.read(1), ENDIAN)
            isCC = False
            while(curByte != 0):
                if (isCC):
                    if (curByte in ccList):
                        newChar = ccList[curByte]
                    else:
                        newChar =  "{0:02X}".format(CC_BYTE) + " "
                        newChar += "{0:02X}".format(curByte)
                    newChar = "[" + newChar + "]"
                    isCC = False
                else:
                    if (curByte in charTable):
                        newChar = charTable[curByte]
                    else:
                        newChar = "[" + "{0:02X}".format(curByte) + "]"
                    isCC = (curByte == CC_BYTE)
                if (not isCC):
                    dumpedLine += newChar
                curByte = int.from_bytes(file.read(1), ENDIAN)
        lines.append(dumpedLine)
    return lines

def outputToFile(filename,lines):
    output = open(filename,"w",encoding="utf-8")
    for idx,line in enumerate(lines):
        output.write("{0:03X}".format(idx) + ":" + line + "\n")
    output.close()

def outputToScreen(lines):
    for idx,line in enumerate(lines):
        print("{0:03X}".format(idx) + ":" + line + "\n")
   
def displayHelp():
    print("Syntax: " + os.path.basename(sys.argv[0]) + " rom_file text_table [output_file]")
   
def main():
    charTable = initCharTable(sys.argv[2])
    ccList = initCCList()
    file = open(sys.argv[1],"rb")
    dumpedPointers = dumpPointers(file)    
    dumpedLines = dumpLines(file, dumpedPointers, charTable, ccList)
    file.close()

    if (len(sys.argv) > 3):
        outputToFile(sys.argv[3], dumpedLines)
    else:
        outputToScreen(dumpedLines)
        
if (len(sys.argv) < 3):
    displayHelp()
else:
    main()

