%functia realizeaza interpolare biliniara pentru matrice, 
%considerand punctele ce se afla pe aceleasi pozitii
function [value] = mybilerp(img, row, col)
    
    [height width] = size(img);
    
    %matricea valorilor returnate
    value = zeros(height, width);
    
    %indicii pentru care valorile row si col se incadreaza
    %in dimensiunile imaginii
    ind = row >= 1 & row <= height & col >= 1 & col <= width;
    
    %indicii sunt initializati cu 1
    x1 = ones(size(row));
    x2 = ones(size(row));
    y1 = ones(size(row));
    y2 = ones(size(row));
    
    %la fel ca in cazul functiei bilerp sunt determinate coordonatele
    %drepunghiurilor ce contin noile puncte de coordonate row si col
    x1(ind) = floor(double(row(ind)));
    x2(ind) = ceil(double(row(ind)));
    y1(ind) = floor(double(col(ind)));
    y2(ind) = ceil(double(col(ind)));
    
    ind1 = ones(size(row));
    ind2 = ones(size(row));
    ind3 = ones(size(row));
    ind4 = ones(size(row));
    
    [m n] = size(img);
    
    %sunt transformate perechile de indici pentru a putea
    %accesa matricele liniar
    ind1(ind) = m * (y1(ind)-1) + x1(ind);
    ind2(ind) = m * (y1(ind)-1) + x2(ind);
    ind3(ind) = m * (y2(ind)-1) + x1(ind);
    ind4(ind) = m * (y2(ind)-1) + x2(ind);
    
    %la fel in cazul functiei bilerp este realizata mai intai interpolarea
    %pe lungimile drepunghiurilor
    r1 = mylerp(x1, img(ind1), x2, img(ind2), row);
    r2 = mylerp(x1, img(ind3), x2, img(ind4), row);
    
    %este apoi realizata interpolarea liniara pe inaltimea drepunghiurilor
    value = mylerp(y1, r1, y2, r2, col);
    
    %valorile pe pozitiile unde row si col nu se incadreaza in dimensiunea
    %imaginii sunt egale cu 0
    value(!ind) = 0;

endfunction
