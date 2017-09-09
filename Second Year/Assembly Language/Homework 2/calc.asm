; decomment according to platform (Linux or Windows)
;%include "include/io.lin.inc"
%include "include/io.win.inc"

section .data
    firstLength: dd 0
    secondLength: dd 0
    firstLength1: dd 0
    secondLength1: dd 0
    big: dd 0
    small: dd 0
    length: dd 0
    maxLength: dd 0
    sign: dd 0
    change: dd 0
    negation: db 0
    final_sign: dd 0
    num_shifts: dd 0
    operation: db 0
    carry: db 0
    
    ; este alocata memorie pentru rezultatul inmultirii
    result: times 200 db 0
    adder: times 200 db 0
    mul_carry: db 0

section .text

global do_operation

; TODO dissasemble the main.o file. Be sure not to overwrite registers used
; by main.o that he does not save himself.
; If you see your program going nuts, consider looking in the main.o disassembly
; for the causes mentioned earlier.



and_or_function: 
    
    push ebp
    mov ebp, esp  
        
    ; este salvata in memorie operatia
    mov eax, dword [ebp + 16]
    mov bl, byte [eax]
    mov [operation], bl
        
    ; este retinuta adresa primului numar
    mov ebx, dword [ebp + 8]
    ; este retinuta adresa celui de-al doilea numar
    mov edx, dword [ebp + 12]
    
    ; sunt salvate lungimile celor doua numere        
    mov eax, dword [ebx + 4]
    mov [firstLength], eax
    
    mov eax, dword [edx + 4]
    mov [secondLength], eax
    
    ; este determinata lungimea minima     
    mov eax, [firstLength]
    cmp eax, dword [secondLength]
    jle second_greater
    mov ecx, [secondLength]
    mov [length], ecx
    
    mov ecx, 0
    jmp and_or_loop

    ; lungimea minima este retinuta in variabila
    ; length    
second_greater:

    mov ecx, [firstLength]
    mov [length], ecx
    
    mov ecx, 0
        
    ; in cadrul acestei bucle este realizata operatia and/or
    ; intre octetii de pe aceeasi pozitie din cele doua numere
and_or_loop:
    
    ; octetii curenti din cele doua numere
    mov al, byte [ebx + 8 + ecx]
    mov ah, byte [edx + 8 + ecx]
    
    cmp byte [operation], '&'
    je and_bytes
    jmp or_bytes
    
    ; este realizata operatia and
and_bytes:

    and al, ah
    mov byte [ebx + 8 + ecx], al
    jmp move_bytes_back
    
    ; este realizata operatia or
or_bytes:

    or al, ah
    
    ; rezultatul este plasat in primul numar
move_bytes_back:

    mov byte [ebx + 8 + ecx], al
    
    ; numerele sunt parcurse pana la lungimea minima
    inc ecx
    cmp ecx, [length]
    jl and_or_loop
    
    cmp byte [operation], '&'
    je and_operation
    jmp or_operation
    
    ; in cazul operatiei and este actualizata
    ; lungimea rezultatului cu lungimea minima
and_operation:

    mov ecx, [length]
    mov dword [ebx + 4], ecx   
    jmp finish_and_or
    
    ; in cazul operatiei or, daca al doilea numar este mai 
    ; mare sunt mutati octetii ramasi in acesta in primul numar
or_operation:

    mov ecx, [firstLength]
    cmp ecx, [secondLength]
    jge finish_and_or
    
    mov ecx, [length]
    
    ; sunt transferati octetii ramasi
transfer_bytes:

    cmp ecx, [secondLength]
    je finish_and_or
    
    mov al, byte [edx + 8 + ecx]
    mov byte [ebx + 8 + ecx], al
    
    inc ecx
    jmp transfer_bytes
    
finish_and_or:
    
    leave
    ret
  
    

left_shift_function:

    push ebp
    mov ebp, esp
        
    ; este determinat numarul necesar de shiftari
    mov ebx, dword [ebp + 12]
    mov ecx, dword [ebx + 8]
    mov [num_shifts], ecx
    
    ; adresa numarului
    mov ebx, dword [ebp + 8]
    
    mov esi, 0
    mov byte [carry], 0
   
   ; este realizata shiftarea de mai multe ori
shift_left_loop:

    ; lungimea numarului
    mov ecx, dword [ebx + 4]
    mov [length], ecx
    
    mov byte [carry], 0
    
    mov ecx, 0
shift_bytes_left:
    
    ; este shiftat fiecare octet din numar
    mov al, byte [ebx + 8 + ecx]
    mov dl, al
    shl al, 1
    mov byte [ebx + 8 + ecx], al
    
    ; este adunat carry-ul precedent
    mov dh, byte [carry]
    add byte [ebx + 8 + ecx], dh
    
    ; se determina carry-ul prin shiftarea la dreapta a octetului
    ; cu 7 pozitii, fiind verificata apoi egalitatea acestuia cu 1
    shr dl, 7
    cmp dl, 1
    je left_shift_carry
    mov byte [carry], 0
    jmp left_shift_no_carry
    
left_shift_carry:
    mov byte [carry], 1
    
left_shift_no_carry:
    
    inc ecx
    cmp ecx, [length]
    jl shift_bytes_left

    ; daca la final carry-ul este 1 este incrementata lungimea
    cmp byte [carry], 1
    je increase_length
    jmp left_shift_end
    
increase_length:
    add dword [ebx + 4], 1
    add byte [ebx + 8 + ecx], 1
    
left_shift_end:

    inc esi
    cmp esi, dword [num_shifts]
    jl shift_left_loop
    
    leave
    ret
    
    
    
right_shift_function:

    push ebp
    mov ebp, esp
    
    ; este determinat numarul de shiftari necesare
    mov ebx, dword [ebp + 12]
    mov ecx, dword [ebx + 8]
    mov [num_shifts], ecx
    
    ; adresa numarului
    mov ebx, dword [ebp + 8]
    
    mov esi, 0
    mov byte [carry], 0

shift_right_loop:

    ; lungimea numarului
    mov ecx, dword [ebx + 4]
    mov [length], ecx
    
    mov byte [carry], 0
    
    mov ecx, [length]
    
    ; sunt parcursi octetii in ordine inversa
shift_bytes_right:
    
    ; este shiftat fiecare octet din numar
    mov al, byte [ebx + 7 + ecx]
    mov dl, al
    shr al, 1
    mov byte [ebx + 7 + ecx], al
    
    mov dh, byte [carry]
    shl dh, 7
    add byte [ebx + 7 + ecx], dh
    
    ; se determina carry-ul prin shiftarea la stanga a octetului
    ; cu 7 pozitii, fiind verificata apoi egalitatea acestuia cu 128
    shl dl, 7
    cmp dl, 128
    je right_shift_carry
    mov byte [carry], 0
    jmp right_shift_no_carry
    
right_shift_carry:
    mov dword [carry], 1
    
right_shift_no_carry:
    
    dec ecx
    cmp ecx, 0
    jg shift_bytes_right

    ; daca la finalul shiftarii ultimul octet este 0 lungimea
    ; rezultatului este decrementata
    mov ecx, [length]
    cmp byte [ebx + 7 + ecx], 0    
    je decrease_length
    jmp right_shift_end
    
decrease_length:
    sub dword [ebx + 4], 1
    
right_shift_end:

    inc esi
    cmp esi, dword [num_shifts]
    jl shift_right_loop
    
    leave
    ret
 
    
    
addition_function:

    push ebp
    mov ebp, esp

    ; adresa primului numar
    mov ebx, dword [ebp + 8]
    ; adresa celui de-al doilea numar
    mov edx, dword [ebp + 12]
    
    ; esi retine adresa unde va fi pus rezultatul
    mov esi, ebx
        
    ; sunt salvate lungimile celor doua numere
    mov eax, dword [ebx + 4]
    mov [firstLength], eax
        
    mov eax, dword [edx + 4]
    mov [secondLength], eax
    
    ; este determinat numarul mai mic si cel mai
    ; mare, impreuna cu lungimile acestora
    mov ecx, [firstLength]
    cmp ecx, [secondLength]
    jle first_number_smaller
    
    mov ecx, [secondLength]
    mov [length], ecx
    mov [small], edx
    mov [big], ebx
    mov ecx, [firstLength]
    mov [maxLength], ecx
    jmp determine_result_sign
   
    ; impreuna cu lungimile sunt retinute adresa numarului
    ; mai mic si a celui mai mare
first_number_smaller:
    
    mov [length], ecx
    mov ecx, [secondLength]
    mov [maxLength], ecx
    mov [small], ebx
    mov [big], edx

determine_result_sign:

    ; daca lungimile celor doua numere sunt diferite inseamna
    ; ca semnul sumei este dat de numarul mai mare
    mov ecx, [length]
    cmp ecx, dword [maxLength]
    jne big_gives_sign
    
    jmp find_final_sign
    
big_gives_sign:
    
    mov ebx, [big]
    mov edx, dword [ebx]
    mov [final_sign], edx
    jmp final_sign_found

    ; in cazul lungimilor egale sunt parcursi octetii celor doua
    ; numere in ordine inversa pana sunt gasiti doi octeti diferiti,
    ; semnul fiind dat de numarul cu octetul mai mare
find_final_sign:
    
    mov ecx, [length]
    mov ebx, [small]
    mov edx, [big]

find_sign_loop:
    
    ; sunt comparati octetii de pe aceeasi pozitie
    mov al, byte [ebx + 7 + ecx]
    mov ah, byte [edx + 7 + ecx]
    
    cmp al, ah
    jl sign_of_big_number
    cmp al, ah
    jg sign_of_small_number
    
    dec ecx
    cmp ecx, 0
    jg find_sign_loop
     
    ; este salvat semnul rezultatului 
sign_of_big_number:
    
    mov ebx, [big]
    mov ecx, dword [ebx]
    mov dword [final_sign], ecx
    jmp final_sign_found

    ; este salvat semnul rezultatului
sign_of_small_number:

    mov ebx, [small]
    mov ecx, dword [ebx]
    mov dword [final_sign], ecx

final_sign_found:

    mov byte [negation], 0
    mov ebx, [small]
    mov edx, [big]

    ; sunt comparate semnele celor doua numere 
    mov ecx, dword [ebx]
    mov dword [sign], ecx
    mov ecx, dword [edx]
    cmp ecx, dword [sign]
    je same_sign
    
    ; daca semnele sunt diferite este trecut in complement fata de
    ; 2 numarul negativ, acesta fiind retinut de variabila change
    mov ecx, dword [ebx]
    cmp ecx, 0
    jne negate_first_number
    jmp negate_second_number
    
negate_first_number:
    mov dword [change], ebx
    jmp negate_number

negate_second_number:
    mov dword [change], edx
    
negate_number:
    mov byte [negation], 1
    mov ebx, dword [change]
    
    mov ecx, dword [ebx + 4]
    mov [length], ecx
    
    mov ecx, 0
    
    ; este negat fiecare octet din numar
change_to_2_complement:
    mov al, byte [ebx + 8 + ecx]
    not al
    mov byte [ebx + 8 + ecx], al
    
    inc ecx
    cmp ecx, [length]
    jl change_to_2_complement
    
    ; se aduna 1 la rezultat pentru a obtine complementul
    add byte [ebx + 8], 1
    
    ; bucla este folosita daca numerele au acelasi semn, sau dupa
    ; ce numarul negativ a fost trecut in complement fata de 2
same_sign:
    
    mov ecx, 0
    mov dword [carry], 0
    
    mov ebx, [small]
    mov edx, [big]
    
    ; sunt adunate numerele octet cu octet
add_numbers:
    
    mov ah, byte [edx + 8 + ecx]
    cmp ecx, [length]
    jge small_number_finished
    jmp small_number_not_finished
    
    ; daca primul numar s-a terminat se considera ca octetii
    ; sai in continuare sunt egali cu 0
small_number_finished:
    mov al, 0
    jmp add_bytes

small_number_not_finished:
    mov al, byte [ebx + 8 + ecx]
    
add_bytes:
    ; este adunat bitul de carry
    add al, byte [carry]
    
    ; sunt adunati octetii si este salvatul carry-ul rezultat
    clc
    add al, ah
    jc addition_carry
    jmp addition_not_carry
        
addition_carry:
    mov byte [carry], 1
    jmp carry_found

addition_not_carry:
    mov byte [carry], 0

carry_found:
    mov byte [esi + 8 + ecx], al
    
    ; sunt parcursi octetii pana la lungimea maxima
    inc ecx
    cmp ecx, [maxLength]
    jl add_numbers
    
    ; este setata lungimea rezultatului egala cu lungimea numarului
    ; mai mare
    mov ecx, [maxLength]
    mov dword [esi + 4], ecx
    
    ; daca la finalul adunarii carry-ul este 1 si numerele au
    ; acelasi semn este incrementata lungimea numarului rezultat
    cmp byte [carry], 1
    je increase_result_length
    jmp end_of_addition
    
increase_result_length:
    cmp byte [negation], 1
    je end_of_addition
     
    add byte [esi + 8 + ecx], 1
    add dword [esi + 4], 1

end_of_addition:
    ; este pus in rezultat semnul final determinat la inceput
    mov ecx, dword [final_sign]
    mov dword [esi], ecx
    
    ; daca semnul rezultatului este 0 si numerele initiale aveau semne
    ; diferite, rezultatul este trecut in complement fata de 2
    cmp ecx, 0
    jne restore_result
    jmp sign_of_result_restored
    
restore_result:
    ; daca numerele initiale aveau acelasi semn, rezultatul nu este negat
    cmp byte [negation], 0
    je sign_of_result_restored
    
    mov ecx, dword [esi + 4]
    mov dword [length], ecx
    mov ecx, 0
    
    ; este trecut rezultatul in complement fata de 2
change_result_in_2_complement:
    mov al, byte [esi + 8 + ecx]
    not al
    mov byte [esi + 8 + ecx], al
    
    inc ecx
    cmp ecx, [length]
    jl change_result_in_2_complement
    
    add byte [esi + 8], 1
    
sign_of_result_restored:

    mov ecx, dword [esi + 4]
    
    ; sunt sterse zerourile de la finalul rezultatului si este
    ; decrementata lungimea acestuia
delete_zeros:
    
    ; sunt parcursi octetii de la final spre inceput
    mov al, byte [esi + 7 + ecx]
    cmp al, 0
    je delete_zero_byte
    jne end_of_addition_function
    
    ; este decrementata lungimea
delete_zero_byte:
    sub dword [esi + 4], 1
    dec ecx
    cmp ecx, 1
    jg delete_zeros
    
end_of_addition_function:

    leave
    ret       
          
                

multiplication_function:
    
    push ebp
    mov ebp, esp
    
    ; adresa primului numar
    mov esi, dword [ebp + 8]
    ; adresa celui de-al doilea numar
    mov edi, dword [ebp + 12]
    
    ; sunt salvate lungimile celor doua numere    
    mov eax, dword [esi + 4]
    mov [firstLength1], eax
        
    mov eax, dword [edi + 4]
    mov [secondLength1], eax
    
    ; sunt initializate semnul si lungimea rezultatului
    mov dword [result], 0
    mov dword [result + 4], 1
    mov byte [result + 8], 0
    
    ; este retinuta pozitia curenta a offset-ului, inmultirea
    ; fiind tratata ca o serie de adunari repetate
    mov edx, 0
    
multiplication_loop:
    
    ; este initializat numarul adder, ce reprezinta rezultatul
    ; inmultirii octetului curent din al doilea numar cu primul numar
    mov dword [adder], 0
    
    ; lungimea lui adder este egala cu lungimea primului numar adunata
    ; cu offset-ul curent
    mov eax, [firstLength1]
    mov dword [adder + 4], eax
    add dword [adder + 4], edx
    mov eax, dword [adder + 4]
    
    mov ecx, 0
    
    ; sunt initializati cu 0 toti octetii din adder
initialize_adder:
    mov byte [adder + 8 + ecx], 0
    
    inc ecx
    cmp ecx, dword [adder + 4]
    jl initialize_adder

    ; este retinut octetul curent din al doilea numar
    mov bh, byte [edi + 8 + edx]
    mov byte [mul_carry], 0
    
    mov ecx, 0
    
create_adder:
    
    ; este inmultitul octetul retinut cu toti octetii din primul numar
    mov bl, byte [esi + 8 + ecx]
    mov al, bh
    
    mul bl
    
    ; este adunat octetul mai putin semnificativ din rezultat cu
    ; carry-ul, fiind apoi adunat la octetul curent din numar
    mov bl, byte [mul_carry]
    add byte [adder + 8 + edx + ecx], bl
    add byte [adder + 8 + edx + ecx], al
    jc multiplication_has_carry
   
    ; este setat bitul de carry in functie de rezultatul adunarii
multiplication_no_carry:
    mov byte [mul_carry], 0
    jmp multiplication_carry_set
    
multiplication_has_carry:
    mov byte [mul_carry], 1

multiplication_carry_set:
    ; este plasat octetul cel mai semnificativ rezultat din inmultire
    ; in pozitia urmatoare din numar
    add byte [adder + 9 + edx + ecx], ah
    
    inc ecx
    cmp ecx, [firstLength1]
    jl create_adder
    
    ; este adunat carry-ul final si este incrementata lungimea lui
    ; adder daca octetul de pe ultima pozitie a acestuia este nenul
    mov bl, byte [mul_carry]
    add byte [adder + 8 + edx + ecx], bl
    cmp byte [adder + 8 + edx + ecx], 0
    je length_not_changed
    
    inc dword [adder + 4]
    
length_not_changed:
    
    ; este adunatul rezultatul final cu numarul adder curent prin
    ; apelul functiei de adunare de la task-ul anterior
    pusha
    push adder
    push result
    call addition_function
    add esp, 8
    popa
    
    ; este deplasat offset-ul in cadrul celui de-al doilea numar
    inc edx
    cmp edx, [secondLength1]
    jl multiplication_loop
    
    ; este mutata la adresa primului numar lungimea rezultatului
    mov edx, dword [result + 4]
    mov dword [esi + 4], edx
    
    mov eax, dword [result + 4]
    mov ecx, 0
    
    ; sunt mutati toti octetii din rezultat la adresa primului numar
copy_result_back:
    mov bl, byte [result + 8 + ecx]
    mov byte [esi + 8 + ecx], bl
    
    inc ecx
    cmp ecx, eax
    jl copy_result_back
    
    ; este determinat semnul rezultatului
    mov ebx, dword [esi]
    mov edx, dword [edi]
    
    xor ebx, edx
    mov dword [esi], ebx
    
    leave
    ret
    


do_operation:
    
    push ebp
    mov ebp, esp
        
    ; este salvata in memorie operatia
    mov eax, dword [ebp + 16]
    mov bl, byte [eax]
    mov byte [operation], bl

    ; sunt luati parametrii functiei do_operation
    mov eax, dword [ebp + 8]
    mov ebx, dword [ebp + 12]
    mov ecx, dword [ebp + 16]
    
    ; sunt introdusi pe stiva parametrii
    push ecx
    push ebx
    push eax        
           
    ; este determinata operatia ceruta si este apelata functia
    ; corespunzatoare acesteia                                         
    cmp byte [operation], '&'
    je function_1
    cmp byte [operation], '|'
    je function_1
    cmp byte [operation], '<'
    je function_2
    cmp byte [operation], '>'
    je function_3
    cmp byte [operation], '+'
    je function_4
    cmp byte [operation], '*'
    je function_5
    
function_1:
    call and_or_function
    jmp end_of_operation

function_2:
    call left_shift_function
    jmp end_of_operation

function_3:
    call right_shift_function
    jmp end_of_operation

function_4:
    call addition_function
    jmp end_of_operation
    
function_5:
    call multiplication_function
    jmp end_of_operation

end_of_operation:
    
    add esp, 12
   
    leave
    ret
