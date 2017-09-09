%functia realizeaza interpolare liniara pentru matrice, 
%considerand punctele ce se afla pe aceleasi pozitii
function [value] = mylerp(x1, y1, x2, y2, x)

    %in cadrul acestei functii y_i reprezinta valorile in punctele
    %din matricea x_i, interpolarea fiind realizata considerand
    %pozitia lui x fata de x1 si x2

    %matricea valorilor returnate
    value = zeros(size(x));
    
    %pentru pozitiile unde valorile x sunt egale
    value(x1 == x2) = y1(x1 == x2);
    
    %pentru restul pozitiilor este realizata interpolarea la
    %fel ca si in cazul functiei lerp
    t = zeros(size(x));
    ind = x1 != x2;
    t(ind) = (x(ind) - x1(ind)) ./ (x2(ind) - x1(ind));
    
    value(ind) = t(ind) .* y2(ind) + (1-t(ind)) .* y1(ind);
    
endfunction
    