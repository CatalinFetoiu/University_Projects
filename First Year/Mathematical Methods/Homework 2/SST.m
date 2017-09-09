%functia rezolva un sistem superior triunghiular
function [x] = SST(A, b)
    [n n] = size(A);
  
    %sunt determinate componentele solutiei 
    x(n) = b(n) / A(n, n);
    for i = (n-1):-1:1
        aux = sum(A(i, (i+1):n) .* x((i+1):n));
        
        x(i) = (b(i) - aux)/A(i, i);
    endfor
endfunction