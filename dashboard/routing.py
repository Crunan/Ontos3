from django.urls import re_path

from . import consumers

websocket_urlpatterns = [
        repath("/dashboard/", consumers.ChatConsumer.as_asgi()),
]
