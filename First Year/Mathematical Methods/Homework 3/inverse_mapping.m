function img_out = inverse_mapping(img_in, T)
    
    %este citita imaginea in matricea img
    img = imread(img_in);
    
    [m n] = size(img);
    
    %daca imaginea trebuie scalata
    if(T(1,2) == 0 && T(1,1) > 0)
        
        %este initializata noua imagine    
        m1 = floor(T(1,1) * m)- 2;
        n1 = floor(T(1,1) * n) - 2;
        new = zeros(m1, n1);
        
        %sunt create doua matrici ce contin perechile de indici cu
        %ajutorul carora este accesata noua matrice
        rows = 1:m1;
        columns = 1:n1;
        
        [mat1 mat2] = meshgrid(rows, columns);
        mat1 = mat1';
        mat2 = mat2';
        
        %sunt determinate pozitiile din imaginea initiala corespunzatoare
        %pozitiilor din noua imagine
        T = inv(T);
        index1 = T(1,1) * mat1 + 1;
        index2 = T(1,1) * mat2 + 1;
   
        %este creata matricea penru noua imagine prin calcularea intensi-
        %tatilor interpolate pentru pixelii sai
        new = mybilerp(img, index1, index2);
        img_out = new;
    
    
    %daca imaginea trebuie rotita
    else
    
        sin = T(2, 1);
        cos = T(1, 1);
        
        %este initializata noua imagine, noile dimensiuni fiind
        %determinate in functie de unghiul de rotatie
        m1 = round(abs(n * sin) + abs(m * cos)) - 2;
        n1 = round(abs(m * sin) + abs(n * cos)) - 2;
        new = zeros(m1, n1);
    
        %sunt create doua matrici ce contin perechile de indici cu
        %ajutorul carora este accesata matricea initiala
        rows = 1:m1;
        columns = 1:n1;
        [mat1 mat2] = meshgrid(rows, columns);
        
        mat1 = mat1';
        mat2 = mat2';

        %sunt calculate valorile minime pentru linii si coloane
        %obtinute in urma inmultirii cu matricea de rotatie
        
        %pentru linii
        padding(1) = T(1,1) * 1 + T(1,2) * 1;
        padding(2) = T(1,1) * 1 + T(1,2) * n;
        padding(3) = T(1,1) * m + T(1,2) * 1;
        padding(4) = T(1,1) * m + T(1,2) * n;
        minx = min(padding);
        
        %pentru coloane
        padding(1) = T(2,1) * 1 + T(2,2) * 1;
        padding(2) = T(2,1) * 1 + T(2,2) * n;
        padding(3) = T(2,1) * m + T(2,2) * 1;
        padding(4) = T(2,1) * m + T(2,2) * n;
        miny = min(padding);
        
        %la fel in ca in cazul functiei forward_mapping, daca exista valori 
        %negative pentru linii si coloane este adaugat padding-ul calculat 
        %anterior tuturor valorilor obtinute
        if(minx < 0)
            mat1 = mat1 - abs(minx) + 1;
        endif
        if(miny < 0)
            mat2 = mat2 - abs(miny) + 1;
        endif
    
        %sunt determinate pozitiile din imaginea initiala corespunzatoare
        %pozitiilor din noua imagine
        T = inv(T);
        index1 = T(1,1) * mat1 + T(1,2) * mat2 + 1;
        index2 = T(2,1) * mat1 + T(2,2) * mat2 + 1;
    
        %este creata imaginea corespunzatoare noii imaginii prin calcularea
        %intensitatilor interpolate ale pixelilor sai
        new = mybilerp(img, index1, index2);
        img_out = new;
   
    endif
    
endfunction
