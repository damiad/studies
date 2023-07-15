from django.test import TestCase, Client, RequestFactory
from django.contrib.auth.models import User
from .models import Catalog, File, FileSection
from django.urls import reverse
from django.contrib.auth.forms import AuthenticationForm
from django.template.loader import render_to_string
from .views import get_file_tree
import json


class CatalogModelTestCase(TestCase):
    def setUp(self):
        self.user = User.objects.create_user(username='testuser', password='testpassword')

    def test_catalog_creation(self):
        catalog = Catalog.objects.create(name='Test Catalog', owner=self.user)
        self.assertEqual(catalog.name, 'Test Catalog')
        self.assertEqual(catalog.owner, self.user)
        self.assertEqual(catalog.is_available, True)

class FileModelTestCase(TestCase):
    def setUp(self):
        self.user = User.objects.create_user(username='testuser', password='testpassword')
        self.catalog = Catalog.objects.create(name='Test Catalog', owner=self.user)

    def test_file_creation(self):
        file = File.objects.create(name='Test File', owner=self.user, catalog=self.catalog)
        self.assertEqual(file.name, 'Test File')
        self.assertEqual(file.owner, self.user)
        self.assertEqual(file.catalog, self.catalog)
        self.assertEqual(file.is_available, True)

class FileSectionModelTestCase(TestCase):
    def setUp(self):
        self.user = User.objects.create_user(username='testuser', password='testpassword')
        self.catalog = Catalog.objects.create(name='Test Catalog', owner=self.user)
        self.file = File.objects.create(name='Test File', owner=self.user, catalog=self.catalog)

    def test_file_section_creation(self):
        file_section = FileSection.objects.create(
            name='Test Section',
            start_line=1,
            end_line=10,
            section_type='PRC',
            content='Test Content',
            file=self.file,
            status='NW',
            status_data='Test Status Data'
        )
        self.assertEqual(file_section.name, 'Test Section')
        self.assertEqual(file_section.section_type, 'PRC')
        self.assertEqual(file_section.file, self.file)
        self.assertEqual(file_section.status, 'NW')
        self.assertEqual(file_section.status_data, 'Test Status Data')
        # Zmiana wartości pola status
        file_section.status = 'WW'
        file_section.save()
        self.assertEqual(file_section.status, 'WW')


class HomeViewTestCase(TestCase):
    def test_home_view(self):
        client = Client()
        response = client.get(reverse('home'))
        self.assertEqual(response.status_code, 200)
        self.assertTemplateUsed(response, 'aww/home.html')


class LoginViewTestCase(TestCase):
    def setUp(self):
        self.credentials = {'username': 'testuser', 'password': 'testpassword'}
        User.objects.create_user(**self.credentials)

    def test_login_view_get(self):
        client = Client()
        response = client.get(reverse('login'))
        self.assertEqual(response.status_code, 200)
        self.assertTemplateUsed(response, 'aww/login.html')

    def test_login_view_post_valid(self):
        client = Client()
        response = client.post(reverse('login'), data=self.credentials)
        self.assertEqual(response.status_code, 302)  # Redirect status code
        self.assertRedirects(response, reverse('main'))

    def test_login_view_post_invalid(self):
        client = Client()
        response = client.post(reverse('login'), data={'username': 'invalid', 'password': 'wrong'})
        self.assertEqual(response.status_code, 200)
        self.assertTemplateUsed(response, 'aww/login.html')


class RegisterViewTestCase(TestCase):
    def test_register_view_get(self):
        client = Client()
        response = client.get(reverse('register'))
        self.assertEqual(response.status_code, 200)
        self.assertTemplateUsed(response, 'aww/register.html')

    def test_register_view_post_valid(self):
        client = Client()
        response = client.post(reverse('register'), data={
            'name': 'Test User',
            'username': 'testuser',
            'password1': 'testpassword',
            'password2': 'testpassword'
        })
        self.assertEqual(response.status_code, 302)  # Redirect status code
        self.assertRedirects(response, reverse('home'))

    def test_register_view_post_invalid(self):
        client = Client()
        response = client.post(reverse('register'), data={
            'name': 'Test User',
            'username': 'testuser',
            'password1': 'testpassword',
            'password2': 'differentpassword'
        })
        self.assertEqual(response.status_code, 200)
        self.assertTemplateUsed(response, 'aww/register.html')


class MainViewTestCase(TestCase):
    def setUp(self):
        self.user = User.objects.create_user(username='testuser', password='testpassword')
        self.catalog = Catalog.objects.create(name='Test Catalog', owner=self.user)
        self.file = File.objects.create(name='Test File', owner=self.user)

    def test_main_view_authenticated(self):
        client = Client()
        client.login(username='testuser', password='testpassword')
        response = client.get(reverse('main'))
        self.assertEqual(response.status_code, 200)
        self.assertTemplateUsed(response, 'aww/main.html')
        self.assertIn('root_catalogs', response.context)
        self.assertIn('root_files', response.context)

    def test_main_view_unauthenticated(self):
        client = Client()
        response = client.get(reverse('main'))
        self.assertEqual(response.status_code, 302)  # Redirect status code


class GetFileTreeViewTestCase(TestCase):
    def setUp(self):
        self.factory = RequestFactory()

    def test_get_file_tree(self):
        # Tworzenie użytkownika i danych dla testu
        user = User.objects.create_user(username='testuser', password='testpass')
        catalog = Catalog.objects.create(name='Root', owner=user, is_available=True)
        file = File.objects.create(name='Test File', owner=user, is_available=True)
        # Tworzenie żądania
        request = self.factory.get('/path/to/endpoint')
        request.user = user
        # Wywołanie funkcji widoku
        response = get_file_tree(request)
        # Sprawdzenie, czy odpowiedź jest poprawna
        self.assertEqual(response.status_code, 200)
        response_data = json.loads(response.content)
        self.assertIn('success', response_data)
        # Porównanie generowanego HTML z oczekiwanym
        expected_html = render_to_string('aww/file_tree_main.html', {'root_catalogs': [catalog], 'root_files': [file]})
        self.assertEqual(response_data['success'], True)
        self.assertEqual(response_data['html'], expected_html)


class AddCatalogFormViewTestCase(TestCase):
    def setUp(self):
        self.client = Client()
        self.user = User.objects.create_user(username='testuser', password='testpassword')

    def test_add_catalog_form_view(self):
        self.client.login(username='testuser', password='testpassword')
        response = self.client.post(reverse('add_catalog_form'))

        self.assertEqual(response.status_code, 200)
        self.assertTemplateUsed(response, 'aww/add_catalog.html')

        # Sprawdzenie, czy odpowiedź zawiera oczekiwane dane JSON
        json_response = response.json()
        self.assertTrue(json_response['success'])
        self.assertIn('html', json_response)

        # Sprawdzenie, czy wygenerowany HTML jest prawidłowy
        expected_html = render_to_string('aww/add_catalog.html', {'catalogs': Catalog.objects.filter(owner=self.user, is_available=True).order_by('name')})
        self.assertEqual(json_response['html'], expected_html)

class AddCatalogViewTestCase(TestCase):
    def setUp(self):
        self.client = Client()
        self.user = User.objects.create_user(username='testuser', password='testpassword')

    def test_add_catalog_view(self):
        self.client.login(username='testuser', password='testpassword')
        parent_catalog = Catalog.objects.create(name='Parent Catalog', owner=self.user)

        data = {
            'name': 'New Catalog',
            'description': 'Catalog description',
            'parent': parent_catalog.id
        }
        response = self.client.post(reverse('add_catalog'), data=data)

        self.assertEqual(response.status_code, 200)

        # Sprawdzenie, czy odpowiedź zawiera oczekiwane dane JSON
        json_response = response.json()
        self.assertTrue(json_response['success'])
        self.assertIn('html', json_response)

        # Sprawdzenie, czy katalog został poprawnie utworzony
        created_catalog = Catalog.objects.get(name='New Catalog', owner=self.user)
        self.assertEqual(created_catalog.description, 'Catalog description')
        self.assertEqual(created_catalog.parent, parent_catalog)

        # Sprawdzenie, czy wywołany został widok get_file_tree
        self.assertTemplateUsed('aww/file_tree_main.html')

class RemoveCatalogFormViewTestCase(TestCase):
    def setUp(self):
        self.client = Client()
        self.user = User.objects.create_user(username='testuser', password='testpassword')

    def test_remove_catalog_form_view(self):
        self.client.login(username='testuser', password='testpassword')
        parent_catalog = Catalog.objects.create(name='Parent Catalog', owner=self.user)

        response = self.client.post(reverse('remove_catalog_form'))

        self.assertEqual(response.status_code, 200)

        # Sprawdzenie, czy odpowiedź zawiera oczekiwane dane JSON
        json_response = response.json()
        self.assertTrue(json_response['success'])
        self.assertIn('html', json_response)

        # Sprawdzenie, czy odpowiedni szablon jest używany
        self.assertTemplateUsed(response, 'aww/delete_catalog.html')

class DeleteCatalogViewTestCase(TestCase):
    def setUp(self):
        self.client = Client()
        self.user = User.objects.create_user(username='testuser', password='testpassword')

    def test_delete_catalog_view(self):
        self.client.login(username='testuser', password='testpassword')
        catalog = Catalog.objects.create(name='Test Catalog', owner=self.user)

        response = self.client.post(reverse('delete_catalog'), {'catalog_id': catalog.id})

        self.assertEqual(response.status_code, 200)

        # Sprawdzenie, czy katalog został poprawnie usunięty
        catalog.refresh_from_db()
        self.assertFalse(catalog.is_available)
        self.assertIsNotNone(catalog.deleted_at)

        # Sprawdzenie, czy odpowiedź zawiera oczekiwane dane z funkcji get_file_tree
        json_response = response.json()
        self.assertTrue(json_response['success'])
        self.assertIn('html', json_response)

class RemoveFileFormViewTestCase(TestCase):
    def setUp(self):
        self.client = Client()
        self.user = User.objects.create_user(username='testuser', password='testpassword')

    def test_remove_file_form_view(self):
        self.client.login(username='testuser', password='testpassword')
        file = File.objects.create(name='Test File', owner=self.user)

        response = self.client.post(reverse('remove_file_form'))

        self.assertEqual(response.status_code, 200)

        # Sprawdzenie, czy odpowiedź zawiera oczekiwane dane
        json_response = response.json()
        self.assertTrue(json_response['success'])
        self.assertIn('html', json_response)

class DeleteFileViewTestCase(TestCase):
    def setUp(self):
        self.client = Client()
        self.user = User.objects.create_user(username='testuser', password='testpassword')

    def test_delete_file_view(self):
        self.client.login(username='testuser', password='testpassword')
        file = File.objects.create(name='Test File', owner=self.user)

        response = self.client.post(reverse('delete_file'), {'file_id': file.id})

        self.assertEqual(response.status_code, 200)

        # Sprawdzenie, czy plik został poprawnie usunięty
        file.refresh_from_db()
        self.assertFalse(file.is_available)
        self.assertIsNotNone(file.deleted_at)

        # Sprawdzenie, czy odpowiedź zawiera oczekiwane dane z funkcji get_file_tree
        json_response = response.json()
        self.assertTrue(json_response['success'])
        self.assertIn('html', json_response)

class AddFileFormViewTestCase(TestCase):
    def setUp(self):
        self.client = Client()
        self.user = User.objects.create_user(username='testuser', password='testpassword')
        self.catalog = Catalog.objects.create(name='Test Catalog', owner=self.user)

    def test_add_file_form_view(self):
        self.client.login(username='testuser', password='testpassword')

        response = self.client.post(reverse('add_file_form'))

        self.assertEqual(response.status_code, 200)

        # Sprawdzenie, czy odpowiedź zawiera oczekiwane dane
        json_response = response.json()
        self.assertTrue(json_response['success'])
        self.assertIn('html', json_response)


class SaveSectionsViewTestCase(TestCase):
    def setUp(self):
        self.client = Client()
        self.user = User.objects.create_user(username='testuser', password='testpassword')

    def test_save_sections_view(self):
        self.client.login(username='testuser', password='testpassword')
        file = File.objects.create(name='Test File', owner=self.user)

        sections_data = [
            {
                'start_line': 1,
                'end_line': 10,
                'section_type': 'PRC',
                'name': 'Section 1',
                'description': 'Section 1 description',
                'content': 'Section 1 content',
                'file_id': file.id
            },
            {
                'start_line': 11,
                'end_line': 20,
                'section_type': 'OTH',
                'name': 'Section 2',
                'description': 'Section 2 description',
                'content': 'Section 2 content',
                'file_id': file.id
            }
        ]

        response = self.client.post(reverse('save_sections'), json.dumps(sections_data), content_type='application/json')

        self.assertEqual(response.status_code, 200)

        # Sprawdzenie, czy sekcje zostały poprawnie zapisane w bazie danych
        file_sections = FileSection.objects.filter(file=file)
        self.assertEqual(file_sections.count(), 2)

        # Sprawdzenie, czy odpowiedź zawiera oczekiwane dane z funkcji get_file_tree
        json_response = response.json()
        self.assertTrue(json_response['success'])
        self.assertIn('html', json_response)


from django.core.files.uploadedfile import SimpleUploadedFile

class AddFileViewTestCase(TestCase):
    def setUp(self):
        self.client = Client()
        self.user = User.objects.create_user(username='testuser', password='testpassword')
        self.catalog = Catalog.objects.create(name='Test Catalog', owner=self.user)

    def test_add_file_view(self):
        self.client.login(username='testuser', password='testpassword')
        file_content = b'Test file content'
        file_data = {
            'name': 'Test File',
            'description': 'Test File description',
            'catalog': self.catalog.id,
            'file': SimpleUploadedFile('test_file.txt', file_content),
        }

        response = self.client.post(reverse('add_file'), file_data)

        self.assertEqual(response.status_code, 200)

        # Sprawdzenie, czy plik został poprawnie zapisany w bazie danych
        file = File.objects.get(name='Test File')
        self.assertEqual(file.owner, self.user)
        self.assertEqual(file.catalog, self.catalog)

        # Sprawdzenie, czy odpowiedź zawiera oczekiwane dane
        json_response = response.json()
        self.assertTrue(json_response['success'])
        self.assertEqual(json_response['file_id'], file.id)
        self.assertIn('html', json_response)

class GetFileSectionsViewTestCase(TestCase):
    def setUp(self):
        self.client = Client()
        self.user = User.objects.create_user(username='testuser', password='testpassword')
        self.catalog = Catalog.objects.create(name='Test Catalog', owner=self.user)
        self.file = File.objects.create(name='Test File', owner=self.user, catalog=self.catalog)

    def test_get_file_sections_view(self):
        self.client.login(username='testuser', password='testpassword')

        # Dodaj sekcje do pliku
        section1 = FileSection.objects.create(start_line=1, end_line=5, content='Section 1', file=self.file)
        section2 = FileSection.objects.create(start_line=6, end_line=10, content='Section 2', file=self.file)

        url = reverse('get_file_sections', args=[self.file.id])
        response = self.client.get(url)

        self.assertEqual(response.status_code, 200)

        # Sprawdzenie, czy odpowiedź zawiera oczekiwane dane
        json_response = response.json()
        self.assertIn('section_texts', json_response)
        section_texts = json_response['section_texts']
        self.assertEqual(len(section_texts), 2)
        self.assertIn(section1.content, section_texts)
        self.assertIn(section2.content, section_texts)


from django.test import TestCase, Client
from django.urls import reverse

class CompileFileViewTestCase(TestCase):
    def setUp(self):
        self.client = Client()
        self.user = User.objects.create_user(username='testuser', password='testpassword')
        # Dodaj odpowiednie obiekty do bazy danych (File, Section itp.) w celu wykonania testów.

    def test_compile_file_view(self):
        self.client.login(username='testuser', password='testpassword')
        # create example file
        file = File.objects.create(name='Test File', owner=self.user)
        section1 = FileSection.objects.create(start_line=1, end_line=5, section_type='Code', content='int main() {', file=file)
        section2 = FileSection.objects.create(start_line=6, end_line=10, section_type='Code', content='    return 0;', file=file)
        section3 = FileSection.objects.create(start_line=11, end_line=15, section_type='Code', content='}', file=file)

        file_id = file.id
        c_standard = 'c11'
        procesor = 'mcs51'
        opt = 0
        rel = 2

        url = reverse('compile_file', args=[file_id, c_standard, procesor, opt, rel])
        response = self.client.get(url)

        # Sprawdź oczekiwane wartości w odpowiedzi JSON
        self.assertEqual(response.status_code, 200)
        json_response = response.json()
        self.assertTrue(json_response['success'])
        self.assertIn('kompilacja', json_response)
        self.assertIn('text', json_response)

        # Sprawdzanie oczekiwanych statusów sekcji
        section1.refresh_from_db()
        section2.refresh_from_db()
        section3.refresh_from_db()
        self.assertEqual(section1.status, 'NW')
        self.assertEqual(section2.status, 'NW')
        self.assertEqual(section3.status, 'NW')

        # Sprawdzanie oczekiwanych wyników kompilacji
        kompilacja = json_response['kompilacja']
        text = json_response['text']
        asm_content=""";--------------------------------------------------------
; File Created by SDCC : free open source ANSI-C Compiler
; Version 4.0.0 #11528 (Linux)
;--------------------------------------------------------
	.module try
	.optsdcc -mmcs51 --model-medium
	
;--------------------------------------------------------
; Public variables in this module
;--------------------------------------------------------
	.globl _main
;--------------------------------------------------------
; special function registers
;--------------------------------------------------------
	.area RSEG    (ABS,DATA)
	.org 0x0000
;--------------------------------------------------------
; special function bits
;--------------------------------------------------------
	.area RSEG    (ABS,DATA)
	.org 0x0000
;--------------------------------------------------------
; overlayable register banks
;--------------------------------------------------------
	.area REG_BANK_0	(REL,OVR,DATA)
	.ds 8
;--------------------------------------------------------
; internal ram data
;--------------------------------------------------------
	.area DSEG    (DATA)
;--------------------------------------------------------
; overlayable items in internal ram 
;--------------------------------------------------------
;--------------------------------------------------------
; Stack segment in internal ram 
;--------------------------------------------------------
	.area	SSEG
__start__stack:
	.ds	1

;--------------------------------------------------------
; indirectly addressable internal ram data
;--------------------------------------------------------
	.area ISEG    (DATA)
;--------------------------------------------------------
; absolute internal ram data
;--------------------------------------------------------
	.area IABS    (ABS,DATA)
	.area IABS    (ABS,DATA)
;--------------------------------------------------------
; bit data
;--------------------------------------------------------
	.area BSEG    (BIT)
;--------------------------------------------------------
; paged external ram data
;--------------------------------------------------------
	.area PSEG    (PAG,XDATA)
;--------------------------------------------------------
; external ram data
;--------------------------------------------------------
	.area XSEG    (XDATA)
;--------------------------------------------------------
; absolute external ram data
;--------------------------------------------------------
	.area XABS    (ABS,XDATA)
;--------------------------------------------------------
; external initialized ram data
;--------------------------------------------------------
	.area XISEG   (XDATA)
	.area HOME    (CODE)
	.area GSINIT0 (CODE)
	.area GSINIT1 (CODE)
	.area GSINIT2 (CODE)
	.area GSINIT3 (CODE)
	.area GSINIT4 (CODE)
	.area GSINIT5 (CODE)
	.area GSINIT  (CODE)
	.area GSFINAL (CODE)
	.area CSEG    (CODE)
;--------------------------------------------------------
; interrupt vector 
;--------------------------------------------------------
	.area HOME    (CODE)
__interrupt_vect:
	ljmp	__sdcc_gsinit_startup
;--------------------------------------------------------
; global & static initialisations
;--------------------------------------------------------
	.area HOME    (CODE)
	.area GSINIT  (CODE)
	.area GSFINAL (CODE)
	.area GSINIT  (CODE)
	.globl __sdcc_gsinit_startup
	.globl __sdcc_program_startup
	.globl __start__stack
	.globl __mcs51_genXINIT
	.globl __mcs51_genXRAMCLEAR
	.globl __mcs51_genRAMCLEAR
	.area GSFINAL (CODE)
	ljmp	__sdcc_program_startup
;--------------------------------------------------------
; Home
;--------------------------------------------------------
	.area HOME    (CODE)
	.area HOME    (CODE)
__sdcc_program_startup:
	ljmp	_main
;	return from main will return to caller
;--------------------------------------------------------
; code
;--------------------------------------------------------
	.area CSEG    (CODE)
;------------------------------------------------------------
;Allocation info for local variables in function 'main'
;------------------------------------------------------------
;	try.c:1: int main() {
;	-----------------------------------------
;	 function main
;	-----------------------------------------
_main:
	ar7 = 0x07
	ar6 = 0x06
	ar5 = 0x05
	ar4 = 0x04
	ar3 = 0x03
	ar2 = 0x02
	ar1 = 0x01
	ar0 = 0x00
;	try.c:2: return 0;
	mov	dptr,#0x0000
;	try.c:3: }
	ret
	.area CSEG    (CODE)
	.area CONST   (CODE)
	.area XINIT   (CODE)
	.area CABS    (ABS,CODE)
"""
        if kompilacja:
            text = text.strip().split('\n')
            asm_content = asm_content.strip().split('\n')
            self.assertEqual(text[1], asm_content[1])
            self.assertEqual(text[5], asm_content[5])
            self.assertEqual(text[17], asm_content[17])
            self.assertEqual(text[41], asm_content[41])
        else:
            self.assertEqual(text, 'Error: Invalid syntax on line 3\n')


