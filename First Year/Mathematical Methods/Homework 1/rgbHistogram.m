function [histogram] = rgbHistogram(image, count_bins)
    img = imread(image);
  
	%lungimea unui interval
    i = 256/count_bins;
  
    histogramR = zeros(1, count_bins);
    histogramG = zeros(1, count_bins);
    histogramB = zeros(1, count_bins);
  
	%fiecare valoare R, G sau B din pixelii imaginii este impartita
	%la i cu functia idivide, astfel conditia ca o valoare a unei
	%culori sa apartina intervalului cu numarul k devine echivalenta
	%cu conditia ca partea intreaga a valorii sa fie k
    img = idivide(img, i);
  
	%pentru fiecare interval se determina cate valori RGB se afla in acesta 
    for i = 0:(count_bins-1)
        histogramR(i+1) = size(img(img(:,:,1) == i), 1); 
        histogramG(i+1) = size(img(img(:,:,2) == i), 1);
        histogramB(i+1) = size(img(img(:,:,3) == i), 1);
    endfor
  
	%histograma returnata este obtinuta prin concatenarea
	%histogramelor pentru cele 3 culori
    histogram = [histogramR histogramG histogramB];
endfunction
