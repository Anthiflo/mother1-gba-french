import re
import sys
import os
import json

START_ADDR = "start_addr"
HAS_POINTERS = "has_pointers"
NB_ENTRIES = "nb_entries"
TEXT_OFFSET = "text_offset"
ADDR_SIZE = "addr_size"
ENDIAN = "endian"
CHAR_TABLE = "char_table"
END_BYTE = "end_byte"
CC_BYTES = "cc_bytes"
CC_ARGUMENTS = "arguments"
KNOWN_COMMANDS = "known_commands"

def initConfig(filename):
    cfgFile = open(filename,"r",encoding="utf-8")
    cfg = json.load(cfgFile)
    return cfg

def initCharTable(filename):
    table = {}
    charFile = open(filename,"r",encoding="utf-8")
    charLines = charFile.readlines()   
    for charLine in charLines:
        lineMatch = re.match("^(..) (.+)$", charLine)
        if(lineMatch):
            charCode = int(lineMatch.group(1),16)
            char = lineMatch.group(2)
            charMatch = re.match("\[.*\]", char)
            if (len(char) > 1 and not charMatch):
                char = "[" + char + "]"
            table[charCode] = char
    charFile.close()
    return table

def dumpPointers(file, cfg):
    pointers = []
    if (cfg[HAS_POINTERS]):
        file.seek(int(cfg[START_ADDR],16))
        for i in range(int(cfg[NB_ENTRIES],16)):
            addr = int.from_bytes(file.read(cfg[ADDR_SIZE]), cfg[ENDIAN])
            pointers.append(addr + int(cfg[TEXT_OFFSET],16))
    return pointers
    
def dumpOneLine(file, offset, charTable, cfg):
    dumpedLine = ""
    if (offset > 0):
        file.seek(offset)
    stopRead = False
    while(not stopRead):
        curByte = int.from_bytes(file.read(1), cfg[ENDIAN])
        if (curByte == cfg[END_BYTE]):
            stopRead = True
        if (curByte in charTable):
            dumpedLine += charTable[curByte]
        else:
            hexByte = "{0:02X}".format(curByte)
            bytesStr = hexByte
            if (hexByte in cfg[CC_BYTES]):
                for i in range(cfg[CC_BYTES][hexByte][CC_ARGUMENTS]):
                     paramByte = int.from_bytes(file.read(1), cfg[ENDIAN])
                     hexParamByte = "{0:02X}".format(paramByte)
                     bytesStr += " " + hexParamByte
            if (bytesStr in cfg[KNOWN_COMMANDS]):
                bytesStr = cfg[KNOWN_COMMANDS][bytesStr]
            dumpedLine += "[" + bytesStr + "]"
    return dumpedLine

def dumpLines(file, pointers, charTable, cfg):
    lines = []
    if (cfg[HAS_POINTERS]):
        for i in range(int(cfg[NB_ENTRIES],16)):
            dumpedLine = ""
            if (pointers[i] > 0):
                dumpedLine = dumpOneLine(file, pointers[i],charTable,cfg)
            lines.append(dumpedLine)
    else:
        offset = int(cfg[START_ADDR],16)
        for i in range(int(cfg[NB_ENTRIES],16)):
            dumpedLine = dumpOneLine(file, offset,charTable,cfg)
            lines.append(dumpedLine)
            offset = -1
            
    return lines

def outputToFile(filename,lines,addPrefix):
    output = open(filename,"w",encoding="utf-8")
    if (addPrefix):
        for idx,line in enumerate(lines):
            output.write("{0:03X}".format(idx) + ":" + line + "\n")
    else:
        output.write("\n???:".join(lines))
    output.close()

def outputToScreen(lines,addPrefix):
    if (addPrefix):
        for idx,line in enumerate(lines):
            print("{0:03X}".format(idx) + ":" + line + "\n")
    else:
        print("\n???:".join(lines))
   
def displayHelp():
    print("Syntax: " + os.path.basename(sys.argv[0]) + " rom_file cfg_file [output_file]")
   
def main():
    cfg = initConfig(sys.argv[2])
    charTable = initCharTable(cfg[CHAR_TABLE])
    file = open(sys.argv[1],"rb")
    dumpedPointers = dumpPointers(file, cfg)    
    dumpedLines = dumpLines(file, dumpedPointers, charTable, cfg)
    file.close()

    if (len(sys.argv) > 3):
        outputToFile(sys.argv[3], dumpedLines, cfg[HAS_POINTERS])
    else:
        outputToScreen(dumpedLines, cfg[HAS_POINTERS])
        
if (len(sys.argv) < 3):
    displayHelp()
else:
    main()

