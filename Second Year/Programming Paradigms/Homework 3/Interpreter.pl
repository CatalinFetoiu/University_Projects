% Fetoiu Catalin-Emil 324CB

valnum(X):- char_type(X, alnum), char_type(X, ascii).
vother(X):- member(X, [';','<','+','-','*','(',')','{','}']).
validc(X):- valnum(X) ; vother(X) ;  X == '='.

lparseq(['='|L],'==',L).
lparseq([X|L],'=',[X|L]):-dif(X,'=').
lparseq([],'=',[]).

lparsealn([X|L],L2,R,L3):- valnum(X), lparsealn(L, [X|L2], R, L3).
lparsealn([X|L],L2,R,[X|L]):- \+valnum(X), reverse(L2, L3), atom_chars(R, L3).
lparsealn([],L2,R,[]):- reverse(L2, L3), atom_chars(R, L3).

lparse2(['='|L],L2,L3):- lparseq(L,R,L4), lparse2(L4,[R|L2],L3).
lparse2([X|L],L2,L3):- valnum(X),lparsealn(L,[X],R,L4), lparse2(L4,[R|L2],L3).
lparse2([X|L],L2,L3):- vother(X), lparse2(L,[X|L2],L3).
lparse2([X|L],L2,L3):- \+validc(X), lparse2(L,L2,L3).
lparse2([],L2,L3):- reverse(L2,L3).

lparse(S, L):- atom_chars(S, L2), lparse2(L2,[],L),!.


% Evalueaza un program primit sub forma de lista de tokeni
parseInputAux(L, R) :- parseIO(L, Prog), evalAdt(Prog, R), !.
parseInputAux(L, 'e') :- not(parseIO(L, _)), !.

% Functia de baza a temei, ce evalueaza un program citit dintr-un fisier de intrare
parseInput(F,R):-read_file_to_string(F,S,[]), lparse(S,L), parseInputAux(L,R), !.


% Extrage o constanta dintr-un program
parseConstant([H|R], value(O), R) :- atom_number(H, O), !.

% Verifica daca un atom contine doar litere
isVariable([]).
isVariable([F|T]) :- char_type(F, alpha), isVariable(T).

% Extrage o variabila dintr-un program, folosind predicatul anterior, totodata verificandu-se ca numele variabilei
% sa nu fie un cuvant cheie
parseVariable([H|R], variable(H), R) :- atom_chars(H, F), isVariable(F),
                                        H \= 'for', H \= 'if', H \= 'else', H \= 'then',
                                        H \= 'assert', H \= 'return', !.

% Extrage un operand dintr-un program, acesta putand fi o constanta sau o variabila
parseOperand(L, O, R) :- (parseConstant(L, O, R) ; parseVariable(L, O, R)), !.

% Extrage dintr-un program cuvantul cheie dat ca prim parametru
reserved(Operator, [Operator|R], Operator, R).

% Extrage o expresie ce contine doar inmultiri. Este extras mai intai un operand, iar in cazul in care primul
% token ramas in lista este *, parsarea este efectuata in continuare
parseProduct(L, mult(X, Y), R) :- parseOperand(L, X, R1), reserved('*', R1, _, R2), parseProduct(R2, Y, R), !.
parseProduct(L, O, R) :- parseOperand(L, O, R).

% Extrage o expresie ce contine doar inmultiri sau adunari. Este extrasa mai intai o expresie ce contine doar inmultiri
% iar in cazul in care primul token ramas in lista este +, parsarea este efectuata in continuare
parseProductSum(L, add(X, Y), R) :- parseProduct(L, X, R1), reserved('+', R1, _, R2), parseProductSum(R2, Y, R), !.
parseProductSum(L, O, R) :- parseProduct(L, O, R).

% Predicatul este folosit pentru a schimba semnele unei expresii dintr-un program, pentru a putea fi respectata
% precedenta operatorului -. Schimbarea se face pana cand este intalnita prima paranteza, primul ; sau < sau ==
% pentru a nu afecta structura programului ce urmeaza dupa expresia ce este parsata
revSign([], []) :- !.
revSign([';'|Rest], [';'|Rest]) :- !.
revSign([')'|Rest], [')'|Rest]) :- !.
revSign(['<'|Rest], ['<'|Rest]) :- !.
revSign(['=='|Rest], ['=='|Rest]) :- !.
revSign(['+'|Rest1], ['-'|Rest2]) :- revSign(Rest1, Rest2), !.
revSign(['-'|Rest1], ['+'|Rest2]) :- revSign(Rest1, Rest2), !.
revSign([C|Rest1], [C|Rest2]) :- revSign(Rest1, Rest2), !.

% Extrage o expresie matematica folosind acelasi principiu ca si predicatele anterioare, fiind folosit si predicatul
% anterior pentru a schimba semnele din expresie
parseMath(L, sub(X, Y), R) :- parseProductSum(L, X, R1), reserved('-', R1, _, R2),
                              revSign(R2, R3), parseMath(R3, Y, R), !.
parseMath(L, O, R) :- parseProductSum(L, O, R).

% Extrage o expresie de tip <
parseSmaller(L, smaller(X, Y), R) :- parseMath(L, X, R1), reserved('<', R1, _, R2), parseSmaller(R2, Y, R), !.
parseSmaller(L, O, R) :- parseMath(L, O, R).

% Exrage o expresie de tip ==
parseSmallerEqual(L, equal(X, Y), R) :- parseSmaller(L, X, R1), reserved('==', R1, _, R2),
                                        parseSmallerEqual(R2, Y ,R), !.
parseSmallerEqual(L, O, R) :- parseSmaller(L, O, R).

% Instructiunile de tip assert, ret sau assign sunt parsate prin extragerea succesiva a numelui instructiunii
% si a expresiei corespunzatoare
parseAssert(L, assert(Expr), R) :- reserved('assert', L, _, R1), reserved('(', R1, _, R2),
                                   parseSmallerEqual(R2, Expr, R3), reserved(')', R3, _, R), !.
parseReturn(L, ret(Expr), R) :- reserved('return', L, _, R1), parseSmallerEqual(R1, Expr, R), !.
parseAssign(L, assign(Var, Expr), R) :- parseVariable(L, Var, R1), reserved('=', R1, _, R2),
                                        parseSmallerEqual(R2, Expr, R), !.5

% Parseaza o linie din program, linie ce se termina prin caracterul ;
parseBlock(L, O, R) :- (parseAssert(L, O, R1) ; (parseReturn(L, O, R1) ; parseAssign(L, O, R1))),
                       reserved(';', R1, _, R), !.
parseBlock(L, nop_statement, R) :- reserved(';', L, _, R), !.

tryParsers(L, O, R) :- ((parseBlock(L, O, R) ; parseIf(L, O, R)) ; parseFor(L, O, R)), !.

% Parseaza o secventa de subprograme
parseSeq(L, O, []) :- tryParsers(L, O, []), !.
parseSeq(L, O, ['}'|R]) :- tryParsers(L, O, ['}'|R]), !.
parseSeq(L, seq(Prog1, Prog2), R) :- tryParsers(L, Prog1, R1), parseSeq(R1, Prog2, R), !.

% Parsarea instructiunilor de tip if_statement si for_statement este realizata urmarind sintaxa exacta a acestora
parseIf(L, if_statement(Expr, Prog1, Prog2), R) :- reserved('if', L, _, R1), reserved('(', R1, _, R2),
                                                   parseSmallerEqual(R2, Expr, R3), reserved(')', R3, _, R4),
                                                   reserved('then', R4, _, R5), reserved('{', R5, _, R6),
                                                   parseSeq(R6, Prog1, R7), reserved('}', R7, _, R8), 
                                                   reserved('else', R8, _, R9), reserved('{', R9, _, R10),
                                                   parseSeq(R10, Prog2, R11), reserved('}', R11, _, R), !.

parseFor(L, for_statement(Init, Cond, Inc, Prog), R) :- reserved('for', L, _, R1), reserved('(', R1, _, R2),
                                                        parseAssign(R2, Init, R3), reserved(';', R3, _, R4),
                                                        parseSmallerEqual(R4, Cond, R5), reserved(';', R5, _, R6), 
                                                        parseAssign(R6, Inc, R7), reserved(')', R7, _, R8),
                                                        reserved('{', R8, _, R9),
                                                        parseSeq(R9, Prog, R10), reserved('}', R10, _, R), !.

% Predicatul indica faptul ca parsarea unui program a fost efectuata cu succes doar daca lista de tokeni ramasa
% la final este vida
parseIO(L, O) :- parseSeq(L, O, []), !.


% Verifica daca o pereche de tip (variabila, valoare) este continuta in tabela de simboluri asociata programului. 
% Aceasta tabela este retinuta ca o lista de perechi
getValue([pair(K, V) | _], K, V) :- !.
getValue([_ | Hash], K, V) :- getValue(Hash, K, V), !.

% Adauga o pereche (variabila, valoare) in tabela de simboluri
addKey([pair(K, _)|Hash], K, V1, [pair(K, V1)|Hash]) :- !.
addKey([], K, V, [pair(K, V)]) :- !.
addKey([H|T], K, V, R) :- addKey(T, K, V, Hash), R = [H|Hash], !.

% Predicatele sunt folosite pentru a converti valorile boolene returnate de operatorii < si == in valorile 0 si 1
convertSmaller(R1, R2, 1) :- R1 < R2, !.
convertSmaller(_, _, 0) :- !.

convertEqual(R1, R2, 1) :- R1 = R2, !.
convertEqual(_, _, 0) :- !.

% Evalueaza o expresie din program, in functie de operatorii continuti
evalExpr(_, value(X), X) :- !.
evalExpr(Hash, variable(X), R) :- getValue(Hash, X, R), !.
evalExpr(Hash, add(Left, Right), R) :- evalExpr(Hash, Left, R1), evalExpr(Hash, Right, R2), R is R1 + R2, !.
evalExpr(Hash, sub(Left, Right), R) :- evalExpr(Hash, Left, R1), evalExpr(Hash, Right, R2), R is R1 - R2, !.
evalExpr(Hash, mult(Left, Right), R) :- evalExpr(Hash, Left, R1), evalExpr(Hash, Right, R2), R is R1 * R2, !.
evalExpr(Hash, smaller(Left, Right), R) :- evalExpr(Hash, Left, R1), evalExpr(Hash, Right, R2),
                                           convertSmaller(R1, R2, R), !.
evalExpr(Hash, equal(Left, Right), R) :- evalExpr(Hash, Left, R1), evalExpr(Hash, Right, R2),
                                         convertEqual(R1, R2, R), !.

% Pentru nodurile de tip assign, ret sau assert din arborele de sintaxa, evaluarea presupune evaluarea
% expresiilor asociate acestora
evalProgram(Hash, assign(variable(Var), Expr), Hash1, right(1), false) :- evalExpr(Hash, Expr, Result),
                                                                          addKey(Hash, Var, Result, Hash1), !.
evalProgram(Hash, assign(variable(_), Expr), Hash, left('e'), false) :- not(evalExpr(Hash, Expr, _)), !.

evalProgram(Hash, assert(Expr), Hash, right(1), false) :- evalExpr(Hash, Expr, 1), !.
evalProgram(Hash, assert(Expr), Hash, left('a'), false) :- evalExpr(Hash, Expr, K), not(K = 1), !.
evalProgram(Hash, assert(Expr), Hash, left('e'), false) :- not(evalExpr(Hash, Expr, _)), !.

evalProgram(Hash, ret(Expr), Hash, right(R), true) :- evalExpr(Hash, Expr, R), !.
evalProgram(Hash, ret(Expr), Hash, left('e'), false) :- not(evalExpr(Hash, Expr, _)), !.

evalProgram(Hash, nop_statement, Hash, right(1), false) :- !.

% Evalueaza un nod de tip secventa de subprograme. Merita mentionat ca, in cazul unei erori de tip assert failed,
% se verifica mai intai daca subprogramul urmator nu contine erori, doar in acest caz fiind returnata eroare de
% assert, in caz contrar fiind returnata ultima eroare obtinuta
evalProgram(Hash, seq(Prog1, _), Hash1, right(R), true) :- evalProgram(Hash, Prog1, Hash1, right(R), true), !.
evalProgram(Hash, seq(Prog1, Prog2), H, R, Has) :- evalProgram(Hash, Prog1, Hash1, right(_), _),
                                                   evalProgram(Hash1, Prog2, H, R, Has), !.
evalProgram(Hash, seq(Prog1, Prog2), Hash, left(Err), false) :- (evalProgram(Hash, Prog1, Hash1, right(_), _),
                                                             evalProgram(Hash1, Prog2, _, left(Err), _)) ;
                                                             (evalProgram(Hash, Prog1, _, left(Err), _), Err \= 'a') ;
                                                             (evalProgram(Hash, Prog1, Hash1, left(Err), _), Err = 'a',
                                                             evalProgram(Hash1, Prog2, _, right(_), _)) ;
                                                             (evalProgram(Hash, Prog1, Hash1, left('a'), _),
                                                             evalProgram(Hash1, Prog2, _, left(Err), _)), !.

% Evalueaza un nod de tip if_statement
evalProgram(Hash, if_statement(Expr, Prog1, _), H, R, Has) :- evalExpr(Hash, Expr, 1), 
                                                              evalProgram(Hash, Prog1, H, R, Has), !.
evalProgram(Hash, if_statement(Expr, _, Prog2), H, R, Has) :- evalExpr(Hash, Expr, K), not(K = 1),
                                                              evalProgram(Hash, Prog2, H, R, Has), !.
evalProgram(Hash, if_statement(Expr, _, _), Hash, left('e'), false) :- not(evalExpr(Hash, Expr, _)), !.

% Evalueaza un nod de tip for_statement, folosind predicatul evalFor ce simuleaza comportamentul acestuia
evalProgram(Hash, for_statement(Init, Expr, Asgn, Prog), X, Y, Z) :- evalProgram(Hash, Init, Hash1, right(_), _),
                                                                     evalFor(Hash1, Expr, Asgn, Prog, X, Y, Z).
evalProgram(Hash, for_statement(Init, _, _, _), Hash, left(Err), false) :- evalProgram(Hash, Init, _, left(Err), _), !.

evalFor(Hash, Expr, Asgn, Prog, X, Y, Z) :- evalExpr(Hash, Expr, K), K = 1,
                                            evalProgram(Hash, Prog, Hash1, right(_), _),
                                            evalProgram(Hash1, Asgn, Hash2, right(_), _),
                                            evalFor(Hash2, Expr, Asgn, Prog, X, Y, Z), !.
evalFor(Hash, Expr, _, _, Hash, right(1), false) :- evalExpr(Hash, Expr, K), not(K = 1), !.
evalFor(Hash, Expr, _, Prog, _, left(Err), false) :- evalExpr(Hash, Expr, K), K = 1, 
                                                    evalProgram(Hash, Prog, _, left(Err), _), !.
evalFor(Hash, Expr, Asgn, Prog, _, left(Err), false) :- evalExpr(Hash, Expr, K), K = 1, 
                                                     evalProgram(Hash, Prog, Hash1, _, _),
                                                     evalProgram(Hash1, Asgn, _, left(Err), _), !.
evalFor(Hash, Expr, _, _, _, left('e'), false) :- not(evalExpr(Hash, Expr, _)), !.

% Evalueaza arborele de sintaxa asociat unui program. Verificarea conditiei ca un program sa contina o instructiune 
% ret este efectuata de acest predicat
evalAdt(Prog, R) :- evalProgram([], Prog, _, right(R), true), !.
evalAdt(Prog, 'x') :- evalProgram([], Prog, _, right(_), false), !.
evalAdt(Prog, Err) :- evalProgram([], Prog, _, left(Err), _), !.




























