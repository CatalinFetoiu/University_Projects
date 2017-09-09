function [page_rank] = Power(input, d, eps)
    %sunt citite datele din fisierul de input
    [A grades val1 val2] = GetInput(input);
    
    %este initializat vectorul de page-rank
    [n n] = size(A);
    y = ones(n, 1);
    
    %este calculata matricea M conform formulei din enunt
    for i = 1:n
        K(i, i) = 1/grades(i);
    endfor
    M = (K * A)';
    
    %matricea pentru care vectorul de page-rank este vector
    %propriu corespunzator valori proprii maxime
    P = d * M + ((1-d)/n) * ones(n);
    
    %se determina vectorul propriu cautat cu ajutorul 
    %metodei puterii directe
    while(true)
        y = P * y;
        y = y/norm(y);
        lambda = y' * P * y;
        
        if(norm(P * y - lambda * y) < eps)
            break
        endif
        
    endwhile
    y = y/norm(y, 1);
    
    %este returnat vectorul de page-rank
    page_rank = y;
endfunction
        
    