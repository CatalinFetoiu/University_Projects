function [w] = learn(X, t)
  	[m n] = size(X);
	%matricei X i se adauga o coloana de 1
 	X(:, n+1) = 1;
	
	%se inmulteste matricea X cu X' pentru a putea aplica
	%descompunerea Householder pe o matricea patratica(X' * X)
  	t = X' * t;
  	X = X' * X;
  
	%se descompune matricea A in produsul Q * R si se determina
	%vectorul de invatare w tinand cont de faptul ca R este
	%superior triunghiulara si Q este ortogonala
    [Q R] = Householder(X);
  	t = Q' * t;
  	w = SST(R, t);
endfunction
