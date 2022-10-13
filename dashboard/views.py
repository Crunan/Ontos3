from django.shortcuts import get_object_or_404, render
from django.http import HttpResponse
from django.views import generic
from django.urls import reverse
from django.utils import timezone

from .models import Question, Choice

class IndexView(generic.ListView):
    template_name = 'dashboard/index.html'
    context_object_name = 'latest_question_list'

    def get_queryset(self):
        """
        Return the last five published questions (not including those set to be
        published in the future. 
        """
        return Question.objects.filter(
                pub_date__lte=timezone.now()
        ).order_by('-pub_date')[:5]

class DetailView(generic.DetailView):
    model = Question
    template_name = 'dashboard/detail.html'

class ResultsView(generic.DetailView):
    model = Question
    template_name = 'dashboard/results.html'

def vote(request, question_id):
    pass
    
