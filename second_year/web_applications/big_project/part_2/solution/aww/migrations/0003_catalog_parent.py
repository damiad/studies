# Generated by Django 4.1.7 on 2023-05-04 14:40

from django.db import migrations, models
import django.db.models.deletion


class Migration(migrations.Migration):

    dependencies = [
        ('aww', '0002_user_last_login_alter_user_password_and_more'),
    ]

    operations = [
        migrations.AddField(
            model_name='catalog',
            name='parent',
            field=models.ForeignKey(blank=True, null=True, on_delete=django.db.models.deletion.CASCADE, related_name='subcatalogs', to='aww.catalog'),
        ),
    ]
