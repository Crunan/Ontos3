from django.shortcuts import get_object_or_404, render
from django.http import HttpResponse

from .models import Recipe 

def Index(request):
        return render(request, 'dashboard/index.html') 

