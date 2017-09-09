%include "io.inc"

section .data
    %include "input.inc"
    
    ;sirul este folosit pentru a transforma resturile mai
    ;mari ca 9 in literele echivalente
    remainders db "0123456789abcdef", 0
    
    ;mesajul afisat in cazul in care baza este in afara
    ;intervalului [2, 16]
    error_message db "Baza incorecta", 0
    
    ;aceasta variabila este introdusa pe stiva inainte de
    ;a fi introduse resturile obtinute, fiind utila ca si
    ;conditie de oprire la afisare
    delimiter dd 17

section .text
global CMAIN
CMAIN:
    
    ;sunt initializati pointerii catre stiva
    push ebp
    mov ebp, esp

    ;este introdus delimitatorul pe stiva
    mov eax, delimiter 
    push eax
    
    ;este plasat deimpartitul in registrul eax si
    ;impartitorul in registrul ebx
    mov eax, [numar]
    mov ebx, [baza]
    
    ;se verifica daca baza se afla in intervalul [2, 16]
    ;si este afisat mesajul de eroare in caz contrar
    cmp ebx, 2
    jl error
    cmp ebx, 16
    jg error
    
    
compute_remainders:

    ;in cadrul acestui label sunt determinate resturile
    ;obtinute prin impartiri succesive ale numarului la
    ;baza data, acestea fiind introduse pe stiva
    mov edx, 0
    div ebx
    push edx
    
    ;impartirea se face cat timp catul este mai mare ca 0
    cmp eax, 0
    jg compute_remainders


print_number:
    
    ;sunt scoase de pe stiva resturile obtinute pana este
    ;intalnit delimitatorul pus la inceput, motivul folosirii
    ;stivei fiind de a le afisa in ordinea corecta din
    ;reprezentarea in baza data
    pop edx
    cmp edx, delimiter
    je end
    
    ;este afisat fiecare rest, pentru cele mai mari decat 9
    ;fiind afisata litera corespunzatoare
    PRINT_CHAR remainders + edx
    jmp print_number
    
    
error:

    ;este afisat mesajul de eroare
    PRINT_STRING error_message
    NEWLINE
    
end:

    leave
    ret