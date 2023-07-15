from django.urls import path, include
from django.contrib.auth import views as auth_views
from django.contrib.auth.decorators import login_required

from . import views

urlpatterns = [
    path("", views.home, name="home"),
    path('login/', views.login_view, name='login'),
    path('register/', views.register_view, name='register'),
    path("main/", login_required(views.main), name="main"),
    path('add_catalog_form/', views.add_catalog_form, name='add_catalog_form'),
    path('remove_catalog_form/', views.remove_catalog_form, name='remove_catalog_form'),
    path('add_file_form/', views.add_file_form, name='add_file_form'),
    path('save_sections/', views.save_sections, name='save_sections'),
    path('remove_file_form/', views.remove_file_form, name='remove_file_form'),
    path('add_catalog/', views.add_catalog, name='add_catalog'),
    path('delete_catalog/', views.delete_catalog, name='delete_catalog'),
    path('add_file/', views.add_file, name='add_file'),
    path('delete_file/', views.delete_file, name='delete_file'),
    path('main/get_file_sections/<int:file_id>/', views.get_file_sections, name='get_file_sections'),
    path('main/compile_file/<int:file_id>/<str:c_standard>/<str:procesor>/<int:opt>/<int:rel>/', views.compile_file, name='compile_file'),
    path('main/save_asm_file/', views.save_asm_file, name='save_asm_file'),
]