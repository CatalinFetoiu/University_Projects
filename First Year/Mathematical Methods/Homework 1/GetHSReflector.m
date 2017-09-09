%functia intoarce vectorul v folosit in aflarea reflectorului
%Householder al unei coloana si numerele sigma si beta folosite
%in descompunere
function [vp, sigma, beta] = GetHSReflector(x, p)
    n = size(x, 1);
  
    s = sum(x(p:n).^2);
    sigma = sign(x(p)) * sqrt(s);
  
    vp = zeros(n, 1);
    vp(p) = x(p) + sigma;
    vp((p+1):n) = x((p+1):n);
  
    beta = sigma * vp(p);
endfunction

  
