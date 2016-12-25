from django.shortcuts import render
from django.http import HttpResponse
import requests
import subprocess, os

# Create your views here.

#def index(request):
    #return render(request, "home.html")

def index(request):
    language1 = request.POST.get('lang1')
    language2 = request.POST.get('lang2')
    language3 = request.POST.get('lang3')
    enorde1 = request.POST.get('enordn1')
    enorde2 = request.POST.get('enordn2')
    enorde3 = request.POST.get('enordn3')
    text1 = request.POST.get('inputText1')
    text2 = request.POST.get('inputText2')
    text3 = request.POST.get('inputText3')

    print language1,language2,language3
    print text1, text2, text3
    
    if language1 == "Bangla":
        if enorde1 == "Encode":
            writeFile = open('UNLTest\\EnInput.txt','w')
            writeFile.writelines(text1)
            writeFile.close()
            
            FNULL = open(os.devnull,'w')
            arg = 'UNLtest\\EnCol33.exe "UNLtest\\WD.dic" "UNLtest\\EnRules.txt" "UNLtest\\EnInput.txt" "UNLtest\\EnOutput.txt" -go -l4'

            subprocess.call(arg, stdout=FNULL, stderr=FNULL, shell = False)
            
            readFile = open('UNLtest\\EnOutput.txt','r')
            
            lines = []
            
            while True:
                x = readFile.readline()
                
                if not x:
                    break
                    
                lines.append(x)
            
            readFile.close()
            
            ans = []
            
            Length = len(lines)
            id = Length-1
            
            while lines[id].find('{unl}') == -1:
                id = id - 1
                #print id
            
            while id < Length:
                ans.append( lines[id] )
                
                if lines[id].find('{/unl}') != -1:
                    break
                    
                id = id + 1
            
            return render(request, "home.html", {'rule': (ans)})
            
        else:
            writeFile = open('UNLTest\\DeInput.txt','w')
            writeFile.writelines(text1)
            writeFile.close()
            
            FNULL = open(os.devnull,'w')
            arg = 'UNLtest\\DeCol27.exe "UNLtest\\WD.dic" "UNLtest\\DeRules.txt" "UNLtest\\DeInput.txt" "UNLtest\\DeOutput.txt" -go -w -l4'

            subprocess.call(arg, stdout=FNULL, stderr=FNULL, shell = False)
            
            readFile = open('UNLtest\\DeOutput.txt','r')
            
            lines = []
            
            while True:
                x = readFile.readline()
                
                if not x:
                    break
                    
                lines.append(x)
                
            ans = []
            
            ans.append( lines[-3] );
            readFile.close()
            
            return render(request, "home.html", {'rule': (ans)})
            
    
    if language2 == "English":
        if enorde2 == "Encode":
            x = "DOMAIN=SPORT&password=guest&TAGERROR=NO&username=UNL_guest&conversion=true&language=en&data=%s&outputmode=text&coding=utf-8" % (text2)
            print x

            r = requests.post('http://unl.ru/etap-cgi/cgiunl.exe', data=x)
            print r.text
            ans1 = r.text[r.text.find('{unl'):r.text.find('{/unl}')+6]
            ans = []
            anss = ''
            for i in range(len(ans1)):
                if ans1[i] == '\n':
                    if anss != '':
                        ans.append(anss)

                    anss = ''
                else:
                    anss += ans1[i]
            ans.append(anss)

            print ans

            #return render(request, "home.html",{'rule' : r.text[r.text.find('{unl'):r.text.find('{/unl}')+6]})
            return render(request, "home.html", {'rule': (ans)})

        else:
            x = "DOMAIN=SPORT&password=guest&TAGERROR=NO&username=UNL_guest&conversion=false&language=en&data=%s&outputmode=text&coding=utf-8" % (
            text2)
            r = requests.post('http://unl.ru/etap-cgi/cgiunl.exe', data=x)

            return render(request, "home.html",{'rule' : HttpResponse(r.text[5:])})

    return render(request, "home.html")
