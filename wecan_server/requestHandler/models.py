from django.db import models

# Create your models here.
class Can(models.Model):
    ip = models.CharField(max_length=255)
    canid = models.CharField(max_length=255)
    status = models.CharField(max_length=255)
    level = models.CharField(max_length=255)
    date = models.DateTimeField(auto_now_add=True,blank=True)
