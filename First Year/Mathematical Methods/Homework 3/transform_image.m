function img_out = transform_image(img_in, k)
    
    %este creat un stack de imagini ce va contine nivelul
    %dat ca parametru
    stack = image_stack(img_in, ceil(k));
    
    %este citita imaginea data ca parametru
    img = imread(img_in);
    [m n] = size(img);
    
    %nivelele intre care se afla nivelul primit ca parmetru
    lev1 = floor(k);
    lev2 = ceil(k);
    
    %este realizata interpolare liniara pe inaltime, matrice asociata noii
    %imaginii fiind o combinatie liniara a matricilor de pe nivelele adiacente
    if(lev1 == lev2)
        img_out = stack(:, :, lev1);
    else
        img_out = (k-lev1) * stack(:, :, lev2) + (lev2-k) * stack(:, :, lev1);
    endif
           
endfunction
