from django.shortcuts import render
from django.http import HttpResponse

def home(request):
    return render(request, 'GUI/home.html')

def about(request):
    return HttpResponse('<h1>GUI About</h1>')


