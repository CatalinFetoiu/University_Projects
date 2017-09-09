function [procent] = evaluate(name, w, tip, count_bins)
	%se concateneaza directoarele cats si not_cats caii date catre
	%directorul dataset
	cats = [name 'cats/'];
	not_cats = [name 'not_cats/'];
	%se concateneaza cailor catre directoarele cats si not_cats "*.jpg" pentru
	%a putea fi extrase doar fisiere cu extensia jpg din acestea
	cats1 = [cats '*.jpg'];
	not_cats1 = [not_cats '*.jpg'];

	%se calculeaza acuratetea pentru w obtinut din histogramele hsv
  	if(strcmp(tip, 'HSV') == 1)
		%sunt extrase in structura files toate imaginile cu pisici cu ajutorul functiei dir
  		files = dir(cats1);
  		n = size(files, 1);
  
  		countok = 0;
		%pentru fiecare imagine din cats se verifica daca predictia facuta cu ajutorul lui 
		%w si al histogramei este corecta si se incrementeaza contorul in caz afirmativ
  		for i = 1:n
    		x = [hsvHistogram([cats files(i).name], count_bins) 1];
    		if((w * x') >= 0)
      			countok += 1;
    		endif
  		endfor
  
		%sunt extrase in structura files toate imaginile ce nu contin pisici
  		files = dir(not_cats1);
  		m = size(files, 1);
  
		%pentru fiecare imagine din not_cats se verifica daca predictia facuta cu ajutorul lui 
		%w si al histogramei este corecta si se incrementeaza contorul in caz afirmativ
  		for i = 1:m
    		x = [hsvHistogram([not_cats files(i).name], count_bins) 1];
    		if((w * x') < 0)
      			countok += 1;
    		endif
  		endfor
  
		%se returneaza procentul in functie de valoarea contorului
 		procent = (countok/(n+m))*100;
  	endif

	%se calculeaza acuratetea pentru w obtinut din histogramele rgb la fel ca mai sus
  	if(strcmp(tip, 'RGB') == 1)
  		files = dir(cats1);
		n = size(files, 1);
  
  		countok = 0;
  		for i = 1:n    
    		x = [rgbHistogram([cats files(i).name], count_bins) 1];
    		if((w * x') >= 0)
      			countok += 1;
    		endif
  		endfor
  
  		files = dir(not_cats1);
  		m = size(files, 1);
  
  		for i = 1:m
    		x = [rgbHistogram([not_cats files(i).name], count_bins) 1];
    		if((w * x') < 0)
      			countok += 1;
    		endif
  		endfor
  
  		procent = (countok/(n+m))*100;
  	endif

endfunction

  
  
  
    
