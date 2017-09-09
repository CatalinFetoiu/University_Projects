function [R1 R2 R3] = PageRank(input, d, eps)
    %sunt determinati vectorii de page-rank cu ajutorul
    %celor 3 metode
    R1 = Iterative(input, d, eps);
    R2 = Algebraic(input, d);
    R3 = Power(input, d, eps);
    
    %sunt citite valorile val1 si val2
    [A grades val1 val2] = GetInput(input);
    %se adauga extensia ".out" fisierului de input
    output = [input ".out"];
    
    %este folosit vectorul returnat de functia Algebraic
    R = R2;
    n = size(R2, 1);
    index = 1:n;
   
    %este sortat vectorul in functie de indicele page-rank, totodata fiind
    %retinute poziitiile initiale ale site-urilor in vectorul index
    for i = 1:n
        for j = (i+1):n
            if(R(i) < R(j)- eps)
                aux = R(i);
                R(i) = R(j);
                R(j) = aux;
                
                aux = index(i);
                index(i) = index(j);
                index(j) = aux;
             endif
        endfor
    endfor
    
    %este determinat gradul de apartenenta pentru fiecare site
    for i = 1:n
        R(i) = Apartenenta(R(i), val1, val2);
    endfor
    
    %este scris numarul de site-uri in fisierul de output 
    f = fopen(output, 'w');
    fprintf(f, "%d\n", n);
    fprintf(f, "\n");
    
    %sunt scrisi in fisier vectorii returnati de cele 3 functii
    for i = 1:n
        fprintf(f, "%d\n", R1(i));
    endfor
    fprintf(f, "\n");
    
    for i = 1:n
        fprintf(f, "%d\n", R2(i));
    endfor
    fprintf(f, "\n");
    
    for i = 1:n
        fprintf(f, "%d\n", R3(i));
    endfor
    fprintf(f, "\n");
    
    %sunt scrisi in fisier indicii site-urilor impreuna cu gradul de
    %apartenenta al fiecaruia
    for i = 1:n
        fprintf(f, "%d %d %d\n", i, index(i), R(i));
    endfor
    
    fclose(f);
endfunction
    
    
                
                
                
                