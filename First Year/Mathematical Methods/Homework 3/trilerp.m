function value = trilerp(stack, row, col, level)
    
    %nivelele intre care se afla nivelul primit ca parmetru
    lev1 = floor(level);
    lev2 = ceil(level);
    
    %este realizata interpolarea biliniara pentru row si col
    %pe cele doua nivele determinate mai sus
    val1 = bilerp(stack(:, :, lev1), row, col);
    val2 = bilerp(stack(:, :, lev2), row, col);
    
    %este realizata interpolarea liniara pe inaltime
    v = zeros(1, lev2);
    v(lev1) = val1;
    v(lev2) = val2;
    value = lerp(v, level);
endfunction
