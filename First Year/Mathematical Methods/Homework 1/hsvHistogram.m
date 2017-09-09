function [histogram] = hsvHistogram(image, count_bins)
  	img = imread(image);
  	[n m u] = size(img);
  
	%lungimea fiecarui interval
  	i = 101/count_bins;
  
  	histogramH = zeros(1, count_bins);
  	histogramS = zeros(1, count_bins);
  	histogramV = zeros(1, count_bins);
  
	%se extrag din matricea de pixeli valorile r, g, b in trei vectori
  	r = img(:,:,1)(:);
  	g = img(:,:,2)(:);
  	b = img(:,:,3)(:);
  
	%valorile din vectorii r, g, b sunt convertite la valori h, s, v cu 
	%ajutorul functiei r2h
  	[H S V] = r2h(r, g, b);
  
	%la fel ca in cazul functie rgbHistogram valorile sunt impartite la lungimea
	%intervalului cu ajutorul functie idivide  
  	H = idivide(H, i);
  	S = idivide(S, i);
  	V = idivide(V, i);
  
	%pentru fiecare interval i se numara cate valori se afla in intervalul respectiv
	%numarand cate valori din vectorii obtinuti prin impartire sunt egale cu i
  	for i = 0:(count_bins-1)
    	histogramH(i+1) = length(H(H == i));
   		histogramS(i+1) = length(S(S == i));
    	histogramV(i+1) = length(V(V == i));
  	endfor
  
	%cele 3 histograme sunt concatenate in histograma hsv
  	histogram = [histogramH histogramS histogramV];
endfunction
  
  
  
