%functia este folosita pentru calculul produsului A = Hp * A
%intr-un mod stabil numeric
function [x] = ApplyHSToRandomColumn(x, v, p, beta)
    n = size(x, 1);
  
    t = sum(v(p:n) .* x(p:n)) / beta;
  
 	x(p:n) = x(p:n) - t * v(p:n);
endfunction
