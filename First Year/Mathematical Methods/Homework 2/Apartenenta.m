function [y] = Apartenenta(x, val1, val2)
    %sunt determinate valorile a si b pentru care 
    %functia este continua
    a = 1/(val2 - val1);
    b = 1 - a * val2;
    
    %este returnata valoarea functiei in punctul x in
    %functie de intervalul in care se afla acesta
    if((0 <= x) && (x < val1))
        y = 0;
    endif
    
    if((val1 <= x) && (x <= val2))
        y = a * x + b;
    endif
    
    if((val2 < x) && (x <= 1))
        y = 1;
    endif
endfunction
        
     