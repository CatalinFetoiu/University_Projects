function [X t] = preprocess(name, tip, count_bins)
	%se concateneaza directoarele cats si not_cats caii date catre
	%directorul dataset
	cats = [name 'cats/'];
	not_cats = [name 'not_cats/'];
	%se concateneaza cailor catre directoarele cats si not_cats "*.jpg" pentru
	%a putea fi extrase doar fisiere cu extensia jpg din acestea
	cats1 = [cats '*.jpg'];
	not_cats1 = [not_cats '*.jpg'];
	
	%in cazul in care se doreste matricea histogramelor hsv
	if(strcmp(tip, 'HSV') == 1)
		%sunt extrase in structura files toate imaginile cu pisici cu ajutorul functiei dir
  		files = dir(cats1);
  		n = size(files, 1);
  
		%pentru fiecare imagine se concateneaza numele ei la calea catre directorul cats
		%si apoi histograma rezultata este alipita matricei X
  		X = hsvHistogram([cats files(1).name], count_bins);
  		for i = 2:n
   			v = hsvHistogram([cats files(i).name], count_bins);
    		X = [X; v];
  		endfor
  		
		%pozitiile corespunzatoare pozelor cu pisici din vectorul t sunt setate la 1
  	   	t1 = zeros(1, n) + 1; 
  
		%se procedeaza analog pentru pozele ce nu contin pisici
  		files = dir(not_cats1);
  		m = size(files, 1);
  
  		for i = 1:m
    		v = hsvHistogram([not_cats files(i).name], count_bins);
    		X = [X; v];
  		endfor
  
  		t2 = zeros(1, m) - 1;
  
		%vectorul t se obtine prin concatenarea vectorilor t1 si t2
  		t = [t1 t2];
  	   	t = t';
  	endif

	%in cazul in care se doreste matricea histogramelor rgb se procedeaza la fel
	%ca mai sus, fiind returnate matricea X si vectorul t
  	if(strcmp(tip, 'RGB') == 1)
 		files = dir(cats1);
  		n = size(files, 1);
  
  		X = rgbHistogram([cats files(1).name], count_bins);
  		for i = 2:n
    		v = rgbHistogram([cats files(i).name], count_bins);
    		X = [X; v];
  		endfor
  
  		t1 = zeros(1, n) + 1;
  
	  	files = dir(not_cats1);
  		m = size(files, 1);
  
  		for i = 1:m
    		v = rgbHistogram([not_cats files(i).name], count_bins);
    		X = [X; v];
  		endfor

  		t2 = zeros(1, m) - 1;	
  
  		t = [t1 t2];
  		t = t';
  	endif
endfunction
  
