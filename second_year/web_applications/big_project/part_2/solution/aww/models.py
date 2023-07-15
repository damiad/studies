from django.db import models
from django.contrib.auth.models import AbstractBaseUser, BaseUserManager

class UserManager(BaseUserManager):
    def create_user(self, name, username, password=None):
        if not name:
            raise ValueError('Name must be set')
        if not username:
            raise ValueError('Username must be set')
        if not password:
            raise ValueError('Password must be set')
        user = self.model(
            name=name,
            username=username,
            password=password,
        )
        user.set_password(password)
        user.save(using=self._db)
        return user

class User(AbstractBaseUser):
    name = models.CharField(max_length=100)
    username = models.CharField(max_length=100, unique=True)
    password = models.CharField(max_length=128)

    objects = UserManager()

    USERNAME_FIELD = 'username'
    REQUIRED_FIELDS = ['name']

    def __str__(self):
        return self.username

class Catalog(models.Model):
    name = models.CharField(max_length=100)
    description = models.TextField(blank=True, null=True)
    created_at = models.DateTimeField(auto_now_add=True)
    owner = models.ForeignKey(User, on_delete=models.CASCADE)
    is_available = models.BooleanField(default=True)
    delated_at = models.DateTimeField(blank=True, null=True)
    last_modified = models.DateTimeField(auto_now=True)
    parent = models.ForeignKey('self', related_name='subcatalogs', on_delete=models.CASCADE, null=True, blank=True)

class File(models.Model):
    name = models.CharField(max_length=100)
    description = models.TextField(blank=True, null=True)
    created_at = models.DateTimeField(auto_now_add=True)
    owner = models.ForeignKey(User, on_delete=models.CASCADE)
    is_available = models.BooleanField(default=True)
    delated_at = models.DateTimeField(blank=True, null=True)
    last_modified = models.DateTimeField(auto_now=True)
    catalog = models.ForeignKey(Catalog, related_name='files', null=True, blank=True, on_delete=models.CASCADE)

class FileSection(models.Model):
    name = models.CharField(max_length=100, blank=True, null=True)
    description = models.TextField(blank=True, null=True)
    created_at = models.DateTimeField(auto_now_add=True)
    start_line = models.PositiveIntegerField()
    end_line = models.PositiveIntegerField()
    PROCEDURE = 'PRC'
    COMMENT = 'CMT'
    COMPILER_DIRECTIVE = 'CD'
    VARIABLE_DECLARATION = 'VAR'
    ASSEMBLY_INSERTION = 'ASM'
    SECTION_TYPE_CHOICES = [
        (PROCEDURE, 'Procedura'),
        (COMMENT, 'Komentarz'),
        (COMPILER_DIRECTIVE, 'Dyrektywny kompilatora'),
        (VARIABLE_DECLARATION, 'Deklaracje zmiennych'),
        (ASSEMBLY_INSERTION, 'Wstawka asemblerowa')
    ]
    section_type = models.CharField(max_length=3, choices=SECTION_TYPE_CHOICES)
    STATUS_CHOICES = [
        ('WW', 'Kompiluje się z ostrzeżeniami'),
        ('NW', 'Kompiluje się bez ostrzeżeń'),
        ('NC', 'Nie kompiluje się'),
    ]
    status = models.CharField(max_length=2, choices=STATUS_CHOICES, blank=True, null=True)
    status_data = models.TextField(blank=True, null=True)
    content = models.TextField()
    file = models.ForeignKey(File, on_delete=models.CASCADE, related_name='sections')


