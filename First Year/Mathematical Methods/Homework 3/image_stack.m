function stack = image_stack(img_in, num_levels)
    
    %este citita imaginea in matricea img
    img = imread(img_in);
    
    %matricea de blurare
    K = 1/9 * ones(3);
    
    %este initializat stack-ul de imagini
    [m n] = size(img);
    stack = zeros(m, n, num_levels);
    
    %sunt create nivelele stack-ului prin blurarea succesiva
    %a imaginii de pe nivelul precedent
    for i = 1:num_levels
        if(i == 1)
            stack(:, :, i) = conv2(img, K, 'same');
        else
            stack(:, :, i) = conv2(stack(:, :, i-1), K, 'same');
        endif
    endfor
    
endfunction
