function [page_rank] = Iterative(input, d, eps)
    %sunt citite datele din fisierul de input
    [A grades val1 val2] = GetInput(input);
    %este initializat vectorul de page-rank
    [n n] = size(A);
    R0 = ones(n, 1) * 1/n;
    
    %este calculata matricea M conform formulei din enunt
    K = zeros(n);
    for i = 1:n
        K(i, i) = 1/grades(i);
    endfor
    M = (K * A)';
    
    %este modificat vectorul pana cand se atinge
    %precizia numerica dorita
    while(true)
        R1 = d * M * R0 + ((1-d)/n) * ones(n, 1);
        if(norm(R1 - R0) < eps)
            break;
        endif
        
        R0 = R1;
    endwhile
    
    %este returnat vectorul de page-rank
    page_rank = R0;
endfunction

