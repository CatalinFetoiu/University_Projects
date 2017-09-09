function [value] = lerp(values, x)
    
    n = length(values);
    
    %daca noua valoare este mai mare decat capetele intervalului
    if(x < 1 || x > n)
        value = 0;
        return;
    endif
    
    %capetele intervalului de lungime 1 in care se afla noua valoare
    left = floor(x);
    right = ceil(x);
    
    %daca valoarea este intreaga
    if(left == right)
        value = values(left);
        return;
    endif
    
    %in caz contrar se realizeaza interpolarea liniara
    t = (x-left)/(right-left);
    value = t * values(right) + (1-t) * values(left);
    
endfunction