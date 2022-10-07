from django.urls import path
from . import views


urlpatterns = [
        path('', views.home, name='GUI-home'),
        path('about/', views.about, name='GUI-about'),

]
