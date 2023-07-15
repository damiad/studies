from datetime import datetime
from django.contrib.auth.forms import AuthenticationForm
from django.contrib.auth import login, authenticate
from django.shortcuts import render, redirect
from django.contrib.auth.decorators import login_required
from .models import User, Catalog, File, FileSection
from django.contrib import messages
from django.http import JsonResponse


def home(request):
    return render(request, 'aww/home.html')

def login_view(request):
    if request.method == 'POST':
        form = AuthenticationForm(data=request.POST)
        if form.is_valid():
            username = form.cleaned_data.get('username')
            password = form.cleaned_data.get('password')
            user = authenticate(username=username, password=password)
            if user is not None:
                login(request, user)
                return redirect('main')
    else:
        form = AuthenticationForm()
    return render(request, 'aww/login.html', {'form': form})

def register_view(request):
    if request.method == 'POST':
        name = request.POST['name']
        username = request.POST['username']
        password1 = request.POST['password1']
        password2 = request.POST['password2']
        if password1 != password2:
            return render(request, 'aww/register.html', {'error': 'Passwords must match'})
        try:
            user = User.objects.create_user(name=name, username=username, password=password1)
            login(request, user)
            return redirect('home')
        except ValueError as e:
            return render(request, 'aww/register.html', {'error': str(e)})
    else:
        return render(request, 'aww/register.html')

@login_required(login_url='home')
def main(request):
    root_catalogs = Catalog.objects.filter(owner=request.user, parent=None, is_available=True).order_by('name')
    root_files = File.objects.filter(owner=request.user, catalog=None, is_available=True).order_by('name')
    return render(request, 'aww/main.html', {'root_catalogs': root_catalogs, 'root_files': root_files})

@login_required
def add_catalog(request):
    if request.method == 'POST':
            name = request.POST['name']
            description = request.POST['description']
            parent_id = request.POST['parent']
            if parent_id:
                parent = Catalog.objects.get(id=parent_id)
            else:
                parent = None
            print(request.user.name + " " + request.user.username + " " + str(request.user.id))
            catalog = Catalog(name=name, description=description, owner=request.user, parent=parent)
            catalog.save()
            messages.success(request, 'Katalog został dodany!')
            return redirect('main')
    else:
        catalogs = Catalog.objects.filter(owner=request.user, is_available=True).order_by('name')
    return render(request, 'aww/add_catalog.html', {'catalogs': catalogs})

@login_required
def delete_catalog(request):
    if request.method == 'POST':
        catalog_id = request.POST.get('catalog_id')
        catalog = Catalog.objects.get(id=catalog_id, owner=request.user)
        catalog.is_available = False
        catalog.deleted_at = datetime.now()
        catalog.save()
        messages.success(request, f'Katalog "{catalog.name}" został usunięty.')
        return redirect('main')
    else:
        catalogs = Catalog.objects.filter(owner=request.user, is_available=True)
        return render(request, 'aww/delete_catalog.html', {'catalogs': catalogs})
    
@login_required
def delete_file(request):
    if request.method == 'POST':
        file_id = request.POST.get('file_id')
        file = File.objects.get(id=file_id, owner=request.user)
        file.is_available = False
        file.deleted_at = datetime.now()
        file.save()
        messages.success(request, f'Plik "{file.name}" został usunięty.')
        return redirect('main')
    else:
        files = File.objects.filter(owner=request.user, is_available=True)
        return render(request, 'aww/delete_file.html', {'files': files})

@login_required
def add_file(request):
    if request.method == 'POST':
        # odczytanie przesłanych danych z formularza
        name = request.POST['name']
        description = request.POST['description']
        catalog_id = request.POST['catalog']
        if catalog_id:
            catalog = Catalog.objects.get(id=catalog_id)
        else:
            catalog = None
        # zapisanie pliku do bazy danych
        file = File(name=name, description=description, owner=request.user, catalog=catalog)
        file.save()

        # Read the uploaded file as bytes and decode to string
        file_content = request.FILES['file'].read().decode('utf-8')
        sections = parse_file_content(file_content)

        # zapisanie sekcji do bazy danych
        for section in sections:
            file_section = FileSection(
                start_line=section['start_line'],
                end_line=section['end_line'],
                section_type=section['type'],
                content=section['content'],
                file=file
            )
            file_section.save()

        return redirect('main')
    else:
        catalogs = Catalog.objects.filter(owner=request.user, is_available=True).order_by('name')
        return render(request, 'aww/add_file.html', {'catalogs': catalogs})


def parse_file_content(content):
    lines = content.split('\n')
    sections = []
    current_section = None
    current_type = None
    current_start_line = None
    
    for i, line in enumerate(lines):
        if line.startswith('//') or line.startswith('#'):
            if current_section is None:
                current_section = ''
                current_type = FileSection.COMPILER_DIRECTIVE
                current_start_line = i + 1
            current_section += line + '\n'
        elif line.startswith('/*'):
            if current_section is None:
                current_section = ''
                current_type = FileSection.COMMENT
                current_start_line = i + 1
            current_section += line + '\n'
            while not line.endswith('*/'):
                i += 1
                line = lines[i]
                current_section += line + '\n'
        elif line.startswith('class '):
            if current_section is not None:
                sections.append({
                    'content': current_section,
                    'type': current_type,
                    'start_line': current_start_line,
                    'end_line': i
                })
            current_section = line + '\n'
            current_type = FileSection.PROCEDURE
            current_start_line = i + 1
        elif line.startswith('var '):
            if current_section is not None:
                sections.append({
                    'content': current_section,
                    'type': current_type,
                    'start_line': current_start_line,
                    'end_line': i
                })
            current_section = line + '\n'
            current_type = FileSection.VARIABLE_DECLARATION
            current_start_line = i + 1
        elif line.startswith('__asm__'):
            if current_section is not None:
                sections.append({
                    'content': current_section,
                    'type': current_type,
                    'start_line': current_start_line,
                    'end_line': i
                })
            current_section = line + '\n'
            current_type = FileSection.ASSEMBLY_INSERTION
            current_start_line = i + 1
        elif line.strip() == '':
            if current_section is not None:
                sections.append({
                    'content': current_section,
                    'type': current_type,
                    'start_line': current_start_line,
                    'end_line': i
                })
            current_section = None
            current_type = None
            current_start_line = None
        else:
            if current_section is None:
                current_section = ''
                current_type = FileSection.PROCEDURE
                current_start_line = i + 1
            current_section += line + '\n'
    
    if current_section is not None:
        sections.append({
            'content': current_section,
            'type': current_type,
            'start_line': current_start_line,
            'end_line': len(lines)
        })
    
    return sections

def get_file_sections(request, file_id):
    if request.method == 'GET':
        file = File.objects.get(id=file_id)
        sections = file.sections.all().order_by('start_line')
        section_texts = [section.content for section in sections]
        return JsonResponse({'section_texts': section_texts})

import tempfile, os
from django.http import HttpResponse
def compile_file(request, file_id, c_standard, procesor, opt, rel):
    if request.method == 'GET':
        file = File.objects.get(id=file_id)
        sections = file.sections.all().order_by('start_line')

        temp_file = tempfile.NamedTemporaryFile(suffix='.c', delete=False)
        with open(temp_file.name, 'w') as f:
            for section in sections:
                f.write(section.content)
        
        result = compile(temp_file.name, c_standard, procesor, opt, rel)
        errors = parse_errors(result)
        asm_file = temp_file.name.replace('.c', '.asm')
        #! usuwa początek /tmp/ pliku asm_file (być może zależne od sdcc w systemie)
        asm_file = asm_file[5:]
        os.remove(temp_file.name)

        # każdej sekcji przypisujemy status na NW
        for section in sections:
            section.status = 'NW'
            section.save()
        # przypisanie statusów sekcjom z błędami
        for error in errors:
            line_num = error[0]
            section = get_section_for_line_num(line_num, sections)
            if error[1].startswith('warning'):
                section.status = 'WW'
            else:
                section.status = 'NC'
            section.status_data = error[1]

        if any('error' in error[1].lower() for error in errors):
            return HttpResponse("Nie udało się skompilować pliku.\n\n" + ''.join([f'{error[0]}: {error[1]}\n' for error in errors]), content_type='text/plain')   
        else:
            with open(asm_file, 'r') as f:
                asm_content = f.read()
            os.remove(asm_file)
            return HttpResponse(asm_content, content_type='text/plain')

options = [
  { "name": "-fomit-frame-pointer", "value": 1 },
  { "name": "-finline-functions", "value": 2 },
  { "name": "-fno-reorder-blocks", "value": 4 },
  { "name": "-fno-jump-tables", "value": 8 },
  { "name": "-fpack-struct", "value": 16 },
]

def get_optimization_string(value):
    optimization_list = []
    for option in reversed(options):
        if value >= option["value"]:
            optimization_list.append(option["name"])
            value -= option["value"]
    return " ".join(optimization_list)

def get_relative_string(procesor, rel):
    if procesor == 'mcs51':
        if rel == 1:
            return '--model-small'
        elif rel == 2:
            return '--model-medium'
        elif rel == 3:
            return '--model-large'
        elif rel == 4:
            return '--model-huge'
    elif procesor == 'z80':
        if rel == 1:
            return '--small'
        elif rel == 2:
            return '--medium'
        elif rel == 3:
            return '--large'
    elif procesor == 'stm8':
        if rel == 1:
            return '--fmem-none'
        elif rel == 2:
            return '--fmem-sm'
        elif rel == 3:
            return '--fmem-lg'
    return ''


import subprocess
def compile(file_path, c_standard, procesor, opt, rel):
    opt_string = get_optimization_string(opt) 
    rel_string = get_relative_string(procesor, rel)
    try:
        if opt_string:
            if rel_string:
                result = subprocess.check_output(['sdcc', opt_string ,f'-m{procesor}', rel_string ,f'-S{c_standard}', file_path], stderr=subprocess.STDOUT)
            else:
                result = subprocess.check_output(['sdcc', opt_string ,f'-m{procesor}' ,f'-S{c_standard}', file_path], stderr=subprocess.STDOUT)
        else:
            if rel_string:
                result = subprocess.check_output(['sdcc', f'-m{procesor}', rel_string ,f'-S{c_standard}', file_path], stderr=subprocess.STDOUT)
            else:
                result = subprocess.check_output(['sdcc', f'-m{procesor}',f'-S{c_standard}', file_path], stderr=subprocess.STDOUT)
        return result.decode('utf-8')
    except subprocess.CalledProcessError as e:
        return e.output.decode('utf-8')

import re
def parse_errors(result):
    error_pattern = re.compile(r'^.+:(\d+): (warning|error) (\d+): (.+)$', re.MULTILINE)
    errors = error_pattern.findall(result)
    return [(int(line), f"{severity} {code}: {message}") for (line, severity, code, message) in errors]

def get_section_for_line_num(line_num, sections):
    for section in sections:
        if line_num >= section.start_line and line_num <= section.end_line:
            return section
    return None


from django.views.decorators.csrf import csrf_exempt
@csrf_exempt
def save_asm_file(request):
    if request.method == 'POST':
        asm_code = request.POST.get('asm_code', '')
        file_path = 'file.asm'
        with open(file_path, 'w') as f:
            f.write(asm_code)
        return JsonResponse({'status': 'success'})
    else:
        return HttpResponse('Only POST requests are allowed')