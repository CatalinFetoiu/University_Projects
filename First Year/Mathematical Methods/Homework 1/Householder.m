%functia aplica algoritmul de factorizare Householder
%matricei A
function [Q R] = Householder(A)
    [m n] = size(A);
    H = eye(m);
	
	%sunt determinati reflectorii Householder pentru fiecare
	%coloana din A
    for p = 1:min(m-1, n)
        [v sigma beta] = GetHSReflector(A(:,p), p);
    	H1 = eye(m) - 2 * v * v' / (v' * v);
    
		%inmultirea A = Hp * A se realizeaza mai stabil numeric
		%tinand cont de forma lui Hp
        A(:,p) = ApplyHSToPColumn(A(:,p), p, sigma);
        for j = (p+1):n
            A(:,j) = ApplyHSToRandomColumn(A(:,j), v, p, beta);
        endfor

        H = H1 * H;
    
    endfor
  
    Q = H';
    R = A;
endfunction
  
