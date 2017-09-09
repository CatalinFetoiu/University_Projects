function [value] = bilerp(img, row, col)
    
    [height width] = size(img);
    
    %daca noua linie este mai mare decat dimensiunile imaginii
    if(row > height || row < 1)
        value = 0;
        return;
    endif
    
    %capetele intervalului de lungime 1 in care se afla noua
    %valoare a liniei
    x1 = floor(row);
    x2 = ceil(row);
    
    %interpolare liniara pentru lungimile dreptunghiului in
    %care se afla noul punct
    r1 = lerp(img(x1, :), col);
    r2 = lerp(img(x2, :), col); 
    
    %interpolare liniara pe inaltimea aceluiasi dreptunghi
    v = zeros(1, height);
    v(x1) = r1;
    v(x2) = r2;
    value = lerp(v, row);

endfunction
