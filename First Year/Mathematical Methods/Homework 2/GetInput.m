%functia returneaza matricea de adiacenta a grafului, vectorul cu
%numarul de link-uri si valorile folosite in functia Apartenenta
function [A grades val1 val2] = GetInput(input)
    f = fopen(input, 'r');
    
    %numarul de site-uri
    n = fscanf(f, "%d", 1);
    %matricea de adiacenta a grafului
    A = zeros(n);
    %vectorul cu numarul de link-uri
    grades = zeros(1, n);
    
    %pentru fiecare din cele n site-uri este citit numarul de
    %linkuri si apoi fiecare site catre care acesta are link
    for i = 1:n
        index = fscanf(f, "%d", 1);
        links_number = fscanf(f, "%d", 1);
        links = fscanf(f, "%d", links_number);
        
        %se verifica daca site-ul are link catre el insusi
        links = links(links != index);
        
        %sunt completate matricea A si vectorul grades
        A(index, links) = 1;
        grades(i) = size(links, 1);
    endfor
    
    %sunt citite valorile folosite in functia Apartenenta
    val1 = fscanf(f, "%f", 1);
    val2 = fscanf(f, "%f", 1);
    
    fclose(f);
endfunction