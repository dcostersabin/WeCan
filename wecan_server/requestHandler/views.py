from datetime import datetime

from django.shortcuts import render, redirect

# Create your views here.

from django.http import HttpResponse
from django.http import HttpResponseRedirect
from ipware import get_client_ip
from .models import Can, remoteOpen
from geopy.geocoders import GoogleV3
import json


def index(request):
    # return HttpResponse("success")

    client_ip, is_routable = get_client_ip(request)

    data = Can();
    data.status = request.GET['status']
    data.canid = request.GET['id']
    data.ip = client_ip
    data.level = request.GET['currentLevel']
    data.date = datetime.now()
    data.lon = request.GET['lon']
    data.lat = request.GET['lat']
    data.accuracy = request.GET['acc']
    if  not (remoteOpen.objects.filter(ip=client_ip).exists()):
        add = remoteOpen()
        add.ip = client_ip
        add.status = 0
        add.save()
        return add

    data.save()
    check = remoteOpen.objects.last()
    if check.status == 0 and int (request.GET['openSignalCheck'])== 0:
        return HttpResponse("Success")

    elif check.status == 1 and int( request.GET['openSignalCheck']) == 0 :
        return HttpResponse("Open")

    elif check.status == 0 and int(request.GET['openSignalCheck']) == 1:
        return HttpResponse("Close")


def view(request):
    latest = Can.objects.last()
    check = remoteOpen.objects.last()
    context = dict(ip=latest.ip, status=latest.status, level=float(latest.level), date=latest.date, canid=latest.canid, stat = check.status,lon=latest.lon , lat=latest.lat)
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
    #
    # url = "https://apility-io-ip-geolocation-v1.p.rapidapi.com/" + request.GET['ip']
    #
    # headers = {
    #     'x-rapidapi-host': "apility-io-ip-geolocation-v1.p.rapidapi.com",
    #     'x-rapidapi-key': "4e4506b3eemsh8c2b49b2f18e958p1df746jsn791bfba24c44",
    #     'accept': "application/json"
    # }
    #
    # response = requests.request("GET", url, headers=headers)
    # # return HttpResponse(response,content_type="application/json")
    # obj = response.json()
    # latitude = obj['ip']['latitude']
    # longitude = obj['ip']['longitude']
    # return HttpResponse("lon" + str(longitude )+ "lat" + str(latitude))

    lon = request.GET['lat']
    lat = request.GET['lon']
    point = lon + ',' + lat
    geo = GoogleV3(api_key='')
    address = (geo.reverse(point))
    # return(HttpResponse(address[0]))
    url = "https://maps.googleapis.com/maps/api/distancematrix/json?units=imperial&origins=Prime College, Pahikwo Sadak, Kathmandu&destinations="+str(address[0])+"&key=AIzaSyBncA1-AoVovjUmp1mDBstZs6PKbBxKnKA"
    response = requests.request("GET",url)
    response = response.json()
    # return HttpResponse(str(response))
    # return  HttpResponse()
    context = {'dist':response['destination_addresses'][0],"ori":response['origin_addresses'][0],"distance":response['rows'][0]['elements'][0]['distance']['text'],'duration':response['rows'][0]['elements'][0]['duration']['text']}
    return HttpResponse(render(request,"distance.html",context))



def openLid(request):
    if remoteOpen.objects.filter(ip=request.GET['ip']).exists():
        check = remoteOpen.objects.get(ip=request.GET['ip'])
        check.status = 1
        check.save()
    return redirect('view')


def closeLid(request):
    if remoteOpen.objects.filter(ip=request.GET['ip']).exists():
        check = remoteOpen.objects.get(ip=request.GET['ip'])
        check.status = 0
        check.save()
        view(request)
    return redirect('view')


def locationDetail(request):
    return 0
