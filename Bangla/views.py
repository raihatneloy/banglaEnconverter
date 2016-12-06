from django.shortcuts import render
from django.http import HttpResponse
import requests
# Create your views here.

def index(request):
    return render(request, "home.html")

def submit(request):
    language = request.POST.get('lang')
    enOrde = request.POST.get('enordn')
    text = request.POST.get('inputText')


    if language == "English":
        if enOrde == "Encode":
            x = "DOMAIN=SPORT&password=guest&TAGERROR=NO&username=UNL_guest&conversion=true&language=en&data=%s&outputmode=text&coding=utf-8" % (text)
            r = requests.post('http://unl.ru/etap-cgi/cgiunl.exe', data=x)
            print r.text
            ans = ''

            for i in range(len(r.text)):
                if r.text[i] == '\n':
                    ans += "<br/>"
                else:
                    ans += r.text[i]

            return render(request, "submit.html",{'rule' : HttpResponse(r.text)})

        else:
            x = "DOMAIN=SPORT&password=guest&TAGERROR=NO&username=UNL_guest&conversion=false&language=en&data=%s&outputmode=text&coding=utf-8" % (
            text)
            r = requests.post('http://unl.ru/etap-cgi/cgiunl.exe', data=x)

            return HttpResponse(r.text)