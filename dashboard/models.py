import datetime

from django.db import models
from django.utils import timezone 


class Recipe(models.Model):
    recipe_name = models.CharField(max_length=30)
    author = models.CharField(max_length=30)
    create_date = models.DateTimeField('date created')
    mfc_1 = models.DecimalField(max_digits=4, decimal_places=2) 
    mfc_2 = models.DecimalField(max_digits=4, decimal_places=2) 
    mfc_3 = models.DecimalField(max_digits=4, decimal_places=2) 
    mfc_4 = models.DecimalField(max_digits=4, decimal_places=2) 
