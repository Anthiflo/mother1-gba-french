import re
import textwrap
import sys

LENGTH_CHAR_NAME = 6
LENGTH_ITEM_NAME = 12
LENGTH_ENEMY_NAME = 21
LENGTH_TEAM_PREFIX = 12
LENGTH_MONEY_AMOUNT = 5

#maxEnemyLength = 0
maxItemArticlesLength = []
maxEnemyArticlesLength = []
substitutions = []

def exampleStr(txt, length):
    if len(txt) > length:
        return txt[0:length]
    nbToAdd = length - len(txt)
    return txt + ("*" * nbToAdd)
    
#def checkMaxEnemyNameLength():
#    enemyFile = open("m1_enemy_long_names.txt","r",encoding="utf-8")
#    lines = enemyFile.readlines()   
#    res = 0
#
#    for line in lines:
#        match = re.match("^ENEMY-...-..: (.*)$", line)
#        if match:
#            if len(match.group(1)) > res:
#                res = len(match.group(1))
#    
#    enemyFile.close()
#    return res


def checkMaxArticleLength(filename1, filename2, prefix):
    classDefFile = open(filename1,"r",encoding="utf-8")
    lines = classDefFile.readlines()  
    res = [0] * 8
    lineNb = 0;
    
    classAttribFile = open(filename2,"r",encoding="utf-8")
    classAttribList = classAttribFile.read()
    classAttribFile.close()
    
    for line in lines:
        if (not line.startswith("//")):
            line = line.rstrip("\n")
            if (len(line) > res[lineNb%8]):
                classId = "{0:0{1}X}".format(lineNb//8,2)
                toFind = prefix + "-...-..: " + classId
                if (re.findall(toFind, classAttribList)):
                    res[lineNb%8] = len(line)
            lineNb += 1
    
    classDefFile.close()
    return res
    
def initSubstitutions():
    textFile = open(sys.argv[1],"r",encoding="utf-8")
    res = [""]*16
    lines = textFile.readlines()
    for line in lines:
        match = re.match("^(6[78](.)-E): (.*)$", line)
        if match and (len(res[int(match.group(2),base=16)]) < len(match.group(3))):
            res[int(match.group(2),base=16)] = match.group(3)
    return res

def performSubstitutions(line,lines):
    for i in range(16):
        line = re.sub("\[03 7" + "{:01X}".format(i) + "\]", substitutions[i], line)
    return line

def decodeLine(line):
    line = line.rstrip("\n")
    line = re.sub("@","", line)
    line = re.sub("\[END\]","", line)
    line = re.sub("\[..\]","*", line)
    line = re.sub("\[DOUBLEZERO\]", "*", line)
    line = re.sub("\[ALPHA\]", "α", line)
    line = re.sub("\[BETA\]", "β", line)
    line = re.sub("\[PIZ\]", "π", line)
    line = re.sub("\[GAMMA\]", "γ", line)
    line = re.sub("\[OMEGA\]", "Ω", line)
    
    line = re.sub("\[03 1[0123]\]",     exampleStr("HERO",LENGTH_CHAR_NAME), line)
    line = re.sub("\[03 1[6AB]\]",      exampleStr("HERO",LENGTH_CHAR_NAME), line)
    line = re.sub("\[03 17\]",          exampleStr("L’équipe de ",LENGTH_TEAM_PREFIX), line)
    line = re.sub("\[03 1C\]",          exampleStr("ITEM1",LENGTH_ITEM_NAME), line)
    line = re.sub("\[03 1D\]",          exampleStr("ITEM2",LENGTH_ITEM_NAME), line)
    line = re.sub("\[03 19\]( )*\$?",   exampleStr("MONEY",LENGTH_MONEY_AMOUNT) + "\\1$", line)
    line = re.sub("\[03 1E\]( )*\$?",   exampleStr("MONEY",LENGTH_MONEY_AMOUNT) + "\\1$", line)
    line = re.sub("\[03 3E\]",          exampleStr("HERO",LENGTH_CHAR_NAME), line)
    line = re.sub("\[03 5.\]",          exampleStr("e",2), line)
    line = re.sub("\[03 6.\]",          "e", line)
    line = re.sub("\[03 7E\]",          "", line)
    line = re.sub("\[03 7F\]",          "e", line)
    line = re.sub("\[03 C.\]",          " X", line)
    #line = re.sub("\[03 D.\]",          exampleStr("FIGHTERLONGNAME",LENGTH_ENEMY_NAME), line)
    line = re.sub("\[03 2[01]\]",      exampleStr("FIGHTERLONGNAME",LENGTH_ENEMY_NAME), line)
    for i in range(8):
        line = re.sub("\[03 4" + "{:01X}".format(i) + "\]", exampleStr("########", maxItemArticlesLength[i]), line)
        line = re.sub("\[03 4" + "{:01X}".format(i+8) + "\]", exampleStr("########", maxItemArticlesLength[i]), line)
        line = re.sub("\[03 E" + "{:01X}".format(i) + "\]", exampleStr("########", maxEnemyArticlesLength[i]), line)
        line = re.sub("\[03 E" + "{:01X}".format(i+8) + "\]", exampleStr("########", maxEnemyArticlesLength[i]), line)

    return line
    
def autowrapLines(lines, wdth):
    for idx,line in enumerate(lines):
        lines[idx] = "[BREAK]".join(textwrap.wrap(line, width=wdth,break_on_hyphens=False,break_long_words=False))
    return ("[BREAK]".join(lines)).split("[BREAK]")
    
nbTooLong = 0
maxLen = 0
maxLines = 0
extraLastLines = 0
autoWrap = False
forcedStrings = []
forcedLengths =[]

def initValues():
    #global maxEnemyLength
    global maxItemArticlesLength,maxEnemyArticlesLength,substitutions
    #maxEnemyLength = checkMaxEnemyNameLength()
    maxItemArticlesLength = checkMaxArticleLength("m1_item_classes.txt", "m1_item_articles.txt", "ITEM")
    maxEnemyArticlesLength = checkMaxArticleLength("m1_enemy_classes.txt", "m1_enemy_articles.txt", "ENEMY")
    substitutions = initSubstitutions()

initValues()

textFile = open(sys.argv[1],"r",encoding="utf-8")
lines = textFile.readlines()

for line in lines:
    match = re.match("^ *// .*#MAXLENGTH=(\d+).*$",line)
    if match:
        maxLen = int(match.group(1))
    match = re.match("^ *// .*#MAXLINES=(\d+).*$",line)
    if match:
        maxLines = int(match.group(1))
    match = re.match("^ *// .*#EXTRALASTLINES=(\d+).*$",line)
    if match:
        extraLastLines = int(match.group(1))
    match = re.match("^ *// .*#AUTOWRAP=(\d+).*$",line)
    if match:
        autoWrap = (int(match.group(1)) != 0)
    match = re.match("^ *// .*#FORCELENGTH \"(.*)\"=(\d+).*$",line)
    if match:
        forcedStrings.append(match.group(1))
        forcedLengths.append(int(match.group(2)))
    
    match = re.match("^(...-E): (.*)$", line)
    if match:
        lineId = match.group(1)
        line = match.group(2)
        
        for i,fs in enumerate(forcedStrings):
            line = line.replace(forcedStrings[i],"*"*forcedLengths[i])
            
        line = performSubstitutions(line,lines)
        
        line = decodeLine(line)
        
        pauseLines = line.split("[PAUSE]")
        
        for pauseLineIdx,pauseLine in enumerate(pauseLines):  
            curMaxlines = maxLines 
             
            lastLine = (pauseLineIdx == len(pauseLines) - 1)
            
            if lastLine:
                curMaxlines += extraLastLines

            pauseLine = re.sub("\[BREAK\]$", "", pauseLine) # Removing the BREAK at the very end
            breakLines = pauseLine.split("[BREAK]")
            
            if autoWrap:
                breakLines = autowrapLines(breakLines, maxLen)
                
            if maxLines != 0 and len(breakLines) > curMaxlines:
                print("WARNING! Too many lines at " + lineId + ": “" + "/".join(breakLines) + "”")
                nbTooLong += 1
            
            previousLineLength = maxLen
            
            for breakLine in breakLines:
                if (len(breakLine) > maxLen):
                    print("WARNING! Too long line at " + lineId + ": “" + breakLine + "”")
                    nbTooLong += 1
                
                if (len(sys.argv) > 2):
                    firstWord = breakLine.lstrip().split(" ")[0]
                    if (len(firstWord) > 0 and len(firstWord) + previousLineLength < maxLen
                    and (firstWord.isupper() or firstWord.islower())):
                        print("WARNING! At " + lineId + ": “" + firstWord + "” could fit before [BREAK]")
                        nbTooLong += 1
                
                    if (len(breakLine) > 0 and (breakLine[-1] == "," or breakLine[-1] == "." or breakLine[-1] == "?" or breakLine[-1] == "!")):
                        previousLineLength = maxLen
                    else:
                        previousLineLength = len(breakLine)
                    
        forcedStrings.clear()
        forcedLengths.clear()

textFile.close()

if (nbTooLong > 0):
    print("Text length issues: ", nbTooLong)

print("Text length check done!")