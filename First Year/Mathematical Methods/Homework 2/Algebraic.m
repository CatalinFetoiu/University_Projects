function [page_rank] = Algebraic(input, d)
    %sunt citite datele din fisierul de input
    [A grades val1 val2] = GetInput(input);
    
    %este calculata matricea M conform formulei din enunt
    [n n] = size(A);
    for i = 1:n
        K(i, i) = 1/grades(i);
    endfor
    M = (K * A)';
    
    %matricea ce trebuie inversata
    T = eye(n) - d * M;
    aux = ((1-d)/n) * ones(n, 1);
    
    %este descompusa QR matricea T folosind algoritmul
    %Gram-Schmidt
    [Q R] = Gram_Schmidt(T);
    
    %matricea inversa a lui T
    inv_T = zeros(n);
    I = eye(n);
    
    %pentru fiecare coloana din matricea T este determinata coloana
    %corespunzatoare din matricea inversa rezolvand sistemul superior
    %triunghiular obtinut prin inmultirea cu matricea ortogonala Q
    for i = 1:n
        b = I(:, i);
        b = Q' * b;
        
        inv_T(:, i) = SST(R, b);
    endfor
    
    %este returnat vectorul de page-rank
    page_rank = inv_T * aux;
endfunction
    
    
    