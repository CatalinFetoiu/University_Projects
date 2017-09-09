%functia transforma trei vectori cu valori r, g, b in vectori
%cu valorile h, s, v corespunzatoare folosind algoritmul de conversie
%intr-un mod eficient
function [h s v] = r2h(r, g, b)
  	n = length(r);
	
  	r1 = double(r)/255;
  	g1 = double(g)/255;
  	b1 = double(b)/255;
  
	%se formeaza vectorii cu valorile Cmax, Cmin si delta corespunzatoare
	%fiecarui triplet (r, g, b);
  	Cmax = max(max(r1, g1), b1);
  	v = Cmax;
  	Cmin = min(min(r1, g1), b1);
  	delta = Cmax - Cmin;
  
  	h = zeros(1, n);
  	s = zeros(1, n);
	
  	%pe pozitiile unde delta este 0 h si s sunt egale cu 0
  	x = delta == 0;
  	h(x) = 0;
  	s(x) = 0; 
  
  	%pentru a evita ca pozitiile unde delta este egal cu 0 sa fie din nou
	%luate in considerare valorile corespunzatoare din Cmax sunt setate la
	%300 (> 255) (astfel se intra pe ramura else a conditiei if (delta == 0)
  	Cmax(delta == 0) = 300;
  
	%analog ca in cazul delta = 0 se convertesc toate pozitiile
	%din vectori care indeplinesc condiitile din algoritmul dat
  	x = Cmax == r1;
  	h(x) =  60 * mod((g1(x) - b1(x))./delta(x), 6);
  	x = Cmax == g1;
  	h(x) = 60 * ((b1(x) - r1(x))./delta(x) + 2);
  	x = Cmax == b1;
  	h(x) = 60 * ((r1(x) - g1(x))./delta(x) + 4);
  
  	x = Cmax == 0;
  	s(x) = 0;
  	x = Cmax != 0;
  	s(x) = delta(x)./Cmax(x);
  
  	%valorile din vectorii h, s, v sunt transformate in valori din
	%intervalul [0 100]
  	s = 100 * s;
  	v = 100 * v;
  	h = (h*100)/360;
  	v = v';
endfunction 
