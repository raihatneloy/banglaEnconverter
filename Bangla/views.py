from django.shortcuts import render, render_to_response
from django.http import HttpResponse
from django.contrib import messages
import requests
import subprocess, os

# Create your views here.

#def index(request):
    #return render(request, "home.html")

def index(request):
    language1 = request.GET.get('lang1')
    language2 = request.GET.get('lang2')
    language3 = request.GET.get('lang3')
    enorde1 = request.GET.get('enordn1')
    enorde2 = request.GET.get('enordn2')
    enorde3 = request.GET.get('enordn3')
    text11 = request.GET.get('inputText11')
    text12 = request.GET.get('inputText12')
    text2 = request.GET.get('inputText2')
    text3 = request.GET.get('inputText3')

    #print language1,language2,language3
    print text11, text2, text3
    #print request.GET.get('bBut'),
    #print request.GET.get('eBut'),
    #print request.GET.get('tBut')
    
    ans = None
    
    if request.GET.get('bBut') == 'ban':
        if enorde1 == "Encode":
            text11 = text11.encode( "utf8" )
            print text11
            if len(text11.split()) < 3:
                messages.error(request, 'The Given sentence has no complete expectation (Akangkha)');
                return render(request, "home.html");
                
            else:
                writeFile = open('UNLTest\\EnInput.txt','w')
                writeFile.writelines(text11)
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
                    
                    if x.find('[W]') != -1:
                        messages.error(request, 'The Given sentence has error - Proximity (Joggota)');
                        return render(request, "home.html");
                    
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
                print ans[1]
                if len(ans) > 2:
                    if ans[1] == '[W]':
                        messages.error(request, 'The Given sentence has error - Proximity (Joggota)');
                        return render(request, "home.html");
                
                request.GET.text1 = 'AvzG'
                
                return render(request, "home.html", {'rule': (ans)})
                
        else:
            text12 = text12.encode( "utf8" )
            writeFile = open('UNLTest\\DeInput.txt','w')
            writeFile.writelines(text12)
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
            
    
    elif request.GET.get('eBut') == 'eng':
        text2 = text2.encode("utf8")
        if enorde2 == "Encode":
            x = "DOMAIN=SPORT&password=guest&TAGERROR=NO&username=UNL_guest&conversion=true&language=en&data=%s&outputmode=text&coding=utf-8" % (text2)
            print x

            r = requests.post('http://unl.ru/etap-cgi/cgiunl.exe', data=x)
            #print r.text
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

            #print '!!!' +  ans

            #return render(request, "home.html",{'rule' : r.text[r.text.find('{unl'):r.text.find('{/unl}')+6]})
            return render(request, "home.html", {'rule': (ans)})

        else:
            x = "DOMAIN=SPORT&password=guest&TAGERROR=NO&username=UNL_guest&conversion=false&language=en&data=%s&outputmode=text&coding=utf-8" % (
            text2)
            r = requests.post('http://unl.ru/etap-cgi/cgiunl.exe', data=x)

            return render(request, "home.html",{'rule' : HttpResponse(r.text[5:])})
    else:
        return render(request, "home.html")
