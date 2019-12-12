from datetime import datetime

from django.shortcuts import render

# Create your views here.

from django.http import HttpResponse
from ipware import get_client_ip
from .models import Can


def index(request):
    client_ip, is_routable = get_client_ip(request)

    data = Can();
    data.status = request.GET['status']
    data.canid = request.GET['id']
    data.ip = client_ip
    data.level = request.GET['currentLevel']
    data.date = datetime.now()
    data.save()

    return HttpResponse("Success")


def view(request):
    latest = Can.objects.last();
    context = dict(ip=latest.ip, status=latest.status, level=float(latest.level), date=latest.date,canid=latest.canid)
    # return HttpResponse(datetime.now())
    return HttpResponse(render(request, "aaa.html", context))


def detail(request):
    latest = Can.objects.all().order_by('-date').values()
    latest = {
        "obj": latest
    }
    return HttpResponse(render(request, "details.html", latest))


def location(request):
    import requests

    url = "https://apility-io-ip-geolocation-v1.p.rapidapi.com/" + request.GET['ip']

    headers = {
        'x-rapidapi-host': "apility-io-ip-geolocation-v1.p.rapidapi.com",
        'x-rapidapi-key': "4e4506b3eemsh8c2b49b2f18e958p1df746jsn791bfba24c44",
        'accept': "application/json"
    }

    response = requests.request("GET", url, headers=headers)


    return HttpResponse(response)
