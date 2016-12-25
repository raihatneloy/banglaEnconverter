import subprocess, os

FNULL = open(os.devnull,'w')
arg = 'UNLtest\\EnCol33.exe "UNLtest\\WD.dic" "UNLtest\\EnRules.txt" "UNLtest\\EnInput.txt" "UNLtest\\EnOutput.txt" -go -l4'

subprocess.call(arg, stdout=FNULL, stderr=FNULL, shell = False)

lines = []
readFile = open('UNLtest\\EnOutput.txt','r')

while True:
    x = readFile.readline()
    
    if not x:
        break
        
    print x,
