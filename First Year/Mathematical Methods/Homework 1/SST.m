%functia rezolva un sistem superior triunghiular
function [v] = SST(A, b)
    [n n] = size(A);

	%sunt determinate elementele lui v
    v(n) = b(n) / A(n, n);
	for i = (n-1):-1:1
      	aux = sum(A(i, (i+1):n) .* v((i+1):n));
    	v(i) = (b(i) - aux)/A(i, i);
    endfor
endfunction
