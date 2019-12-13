from django.utils import timezone

from django.db import models


class Can(models.Model):
    ip = models.CharField(max_length=255, blank=None)
    canid = models.CharField(max_length=255)
    status = models.IntegerField(blank=None)
    level = models.FloatField(blank=None)
    date = models.DateTimeField(auto_now_add=True, blank=True)
    lat = models.CharField(max_length=255, blank=None)
    lon = models.CharField(max_length=255, blank=None)
    accuracy = models.CharField(max_length=255, blank=None)


class remoteOpen(models.Model):
    status = models.IntegerField(default=0)
    ip = models.CharField(max_length=255, blank=None)
