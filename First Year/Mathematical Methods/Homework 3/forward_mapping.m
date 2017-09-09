function img_out = forward_mapping(img_in, T)
    
    %este citita imaginea in matricea img
    img = imread(img_in);
    
    [m n] = size(img);
    
    %daca imaginea trebuie scalata
    if(T(1,2) == 0 && T(1,1) > 0)
        
        %este initializata noua imagine
        m1 = round(T(1,1) * m);
        n1 = round(T(1,1) * n);
        new = zeros(m1, n1);
    
        %sunt create doua matrici ce contin perechile de indici cu
        %ajutorul carora este accesata matricea initiala
        rows = 1:m;
        columns = 1:n;
        [mat1 mat2] = meshgrid(rows, columns);
        
        %sunt determinate pozitiile in care vor fi plasatii vechii
        %pixeli in noua imagine
        index1 = T(1,1) * mat1 + T(1,2) * mat2;
        index2 = T(2,1) * mat1 + T(2,2) * mat2;
          
        index1 = round(index1);
        index2 = round(index2);
    
        %se elimina posibilitatea de a obtine pixeli morti in imagine
        %prin aducerea in intervalul dimensiunilor noii imagini a 
        %indexilor ce deviaza cu o unitate
        index1(index1 < 1) = 1;
        index2(index2 < 1) = 1;
        
        %sunt transformate perechile de indici pentru a putea
        %accesa imaginile liniar
        ind = sub2ind(size(img), mat1(:), mat2(:));
        newind = sub2ind(size(new), index1(:), index2(:));
        
        %este creata matricea pentru noua imagine
        new(newind) = img(ind);
        img_out = new;
    

    %daca imaginea trebuie rotita
    else
    
        sin = T(2, 1);
        cos = T(1, 1);
        
        %este initializata noua imagine, noile dimensiuni fiind
        %determinate in functie de unghiul de rotatie
        m1 = round(abs(n * sin) + abs(m * cos));
        n1 = round(abs(m * sin) + abs(n * cos));
        new = zeros(m1, n1);
        
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
        
        %sunt create doua matrici ce contin perechile de indici cu
        %ajutorul carora este accesata matricea initiala
        rows = 1:m;
        columns = 1:n;
        [mat1 mat2] = meshgrid(rows, columns);
        
        %sunt determinate pozitiile in care vor fi plasatii vechii
        %pixeli in noua imagine
        index1 = T(1,1) * mat1 + T(1,2) * mat2;
        index2 = T(2,1) * mat1 + T(2,2) * mat2;
        
        %daca exista valori negative pentru linii si coloane este adaugat
        %padding-ul calculat anterior tuturor valorilor obtinute
        if(minx < 0)
            index1 = index1 + abs(minx) + 1;
        endif
        if(miny < 0)
            index2 = index2 + abs(miny) + 1;
        endif
    
        index1 = round(index1);
        index2 = round(index2);
    
        %se elimina posibilitatea de a obtine pixeli morti in imagine
        %prin aducerea in intervalul dimensiunilor noii imagini a 
        %indexilor ce deviaza cu o unitate
        index1(index1 < 1) = 1;
        index2(index2 < 1) = 1;
        
        %sunt transformate perechile de indici pentru a putea
        %accesa imaginile liniar
        ind = sub2ind(size(img), mat1(:), mat2(:));
        newind = sub2ind(size(new), index1, index2);
        
        %este creata matricea pentru noua imagine
        new(newind) = img(ind);
        img_out = new;
        
    endif
          
endfunction
