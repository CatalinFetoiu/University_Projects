%functia este folosita pentru efectuarea produsului
%A = Hp * A intr-un mod stabil numeric
function [x] = ApplyHSToPColumn(x, p, sigma)
   x(p) = -sigma;
   n = size(x, 1);
   x((p+1):n) = 0;
endfunction
