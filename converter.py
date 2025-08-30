import os, sys, re

path = os.path.dirname(os.path.abspath(__file__))
#print(path)

def main():
    while True:
        index = 1
        files = []
        for file in os.listdir(os.path.dirname(os.path.abspath(__file__))):
            if file.endswith(".uvmsrc"):
                print(f"{index} | {file}")
                index += 1
                files.append(file)
        
        #print(files)
        index = input(">")
        
        try:
            #print(files[int(index)-1])
            convert(files[int(index)-1])
        except:
            if index == "*":
                for i in range(len(files)):
                    convert(files[i])
            elif index == "+":
                targets = []
                fail = False
                index2 = " "
                
                while index2 != "":
                    targetsShow = ""
                    for i in targets:
                        targetsShow += str(i+1)+" "
                    targetsShow = targetsShow[0:len(targetsShow)-1]
                        
                    index2 = input(f"[{targetsShow}] >")
                    try:
                        if int(index2)-1 >= 0 and int(index2)-1 <= len(files):
                            targets.append(int(index2)-1)
                    except:
                        if index2.casefold() == "x":
                            fail = True
                            break
                        else:
                            if index2 != "":
                                print("Invalid index")
                
                if not fail:
                    for i in targets:
                        convert(files[i])
            else:
                print("Invalid index")

def convert(filename):
    if len(sys.argv) < 2 and False:
        print("This converter requires a .uvmsrc source file to work, and no file was found. Press ENTER to terminate.")
        input()
    else:
        filepath, ext = os.path.splitext(filename) #sys.argv[1]
        #print(filepath, ext)
        
        if ext != ".uvmsrc":
            print(f"[WARNING!] You are compiling a {ext} file! While it may still compile, it is reccomended to stick to .uvmsrc files for clarity.")
            # and ext != ".txt":
            #print(f"This converter requires a .uvmsrc source file to work, and the provided file is a {ext} file. Press ENTER to terminate.")
            #input()
        #else:

        op2val = {
            "INC": 1,
            "DEC": 2,
            "JMP": 3,
            "JNZ": 4,
            "MOV": 5,
            "CPY": 6,
            "SET": 7,
            "OUT": 8,
            "INP": 9,
            "ADD": 10,
            "SUB": 11,
            "HLT": 255,  
        }
            
        lines = []
        labels = {}
        pos = 0
            
        fileIN = open(path+"/"+filepath+ext, "r")
        #print(fileIN)
        for line in fileIN:
            #print(line)
            s = line.replace(" ", "").replace("\n", "").replace("\t", "")
            if len(s) > 0:
                lines.append(s)

        #print(lines)
        
        for l in lines:
            if l[0] == "!":
                labels.update({l[1:]: hex(pos)[2:].zfill(4)})
            else:
                opcode = l[0:3]
                ops = l[3:]
                operands = [ops[i:i+2] for i in range(0, len(ops), 2)]
                #chars = 0
                t = ""
                    
                opcodeByte = hex(op2val[opcode.upper()])[2:].zfill(2)
                preData = opcodeByte+ops

                #for name, value in labels.items():
                #    preData = preData.replace(name, value)
                for name, value in labels.items():
                    preData = re.sub(f'\b{name}\b', '0000', preData)
                
                pos += len(preData)//2
        
        pos = 0
        with open(path+"/"+filepath+".uvm", "wb") as fileOUT:
            for i in range(len(lines)):
                if lines[i][0] !="!":
                    #print(lines[i])
                    for name, value in labels.items():
                        lines[i] = re.sub(f'{name}', value, lines[i])
                    #print(lines[i])
                    
                    l = lines[i]
                    opcode = l[0:3]
                    ops = l[3:]
                    operands = [ops[i:i+2] for i in range(0, len(ops), 2)]
                    #chars = 0
                    t = ""
                        
                    opcodeByte = hex(op2val[opcode.upper()])[2:].zfill(2)
                    preData = opcodeByte+ops

                    #for name, value in labels.items():
                    #    preData = preData.replace(name, value)
                    #print(preData, pos)
                    
                    pos += len(preData)//2
                    
                    data = bytes.fromhex(preData)
                    
                    fileOUT.write(data)
                    
                #print(labels)
        print(f"Successfully converted {filepath}{ext} into {filepath}.uvm!") # Press ENTER to finish!")
        #input()

main()
