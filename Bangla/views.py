from django.shortcuts import render
from django.http import HttpResponse
import requests
# Create your views here.

def index(request):
    return render(request, "home.html")

def submit(request):
    language1 = request.POST.get('lang1')
    language2 = request.POST.get('lang2')
    language3 = request.POST.get('lang3')
    enOrde = request.POST.get('enordn')
    text1 = request.POST.get('inputText1')
    text2 = request.POST.get('inputText2')
    text3 = request.POST.get('inputText3')

    print language1,language2,language3
    print text1, text2, text3

    if language2 == "English":
        if enOrde == "Encode":
            x = "DOMAIN=SPORT&password=guest&TAGERROR=NO&username=UNL_guest&conversion=true&language=en&data=%s&outputmode=text&coding=utf-8" % (text2)
            print x

            r = requests.post('http://unl.ru/etap-cgi/cgiunl.exe', data=x)
            print r.text
            ans = ''

            for i in range(len(r.text)):
                if r.text[i] == '\n':
                    ans += "<br/>"
                else:
                    ans += r.text[i]

            print ans

            return render(request, "submit.html",{'rule' : HttpResponse(r.text)})

        else:
            x = "DOMAIN=SPORT&password=guest&TAGERROR=NO&username=UNL_guest&conversion=false&language=en&data=%s&outputmode=text&coding=utf-8" % (
            text2)
            r = requests.post('http://unl.ru/etap-cgi/cgiunl.exe', data=x)

            return render(request, "submit.html",{'rule' : HttpResponse(r.text)})