--Fetoiu Catalin-Emil 324CB

module Interpreter
  (
    -- * Types
    Prog,
    Asgn,

    -- * Functions
    evalRaw,
    evalAdt,
  ) where

import Data.Char

-------------------------------------------------------------------------------
--------------------------------- The Expr ADT  -------------------------------
-------------------------------------------------------------------------------
data Expr = Add Expr Expr
          | Sub Expr Expr
          | Mult Expr Expr
          | Equal Expr Expr
          | Smaller Expr Expr
          | Symbol String
          | Value Int deriving (Show, Read)


-------------------------------------------------------------------------------
---------------------------------- The Prog ADT -------------------------------
-------------------------------------------------------------------------------
data Asgn = Asgn String Expr deriving (Show, Read)

data Prog = Eq Asgn
          | Seq Prog Prog
          | If Expr Prog Prog
          | For Asgn Expr Asgn Prog
          | Loop Expr Expr Prog
          | Assert Expr
          | Return Expr deriving (Show, Read)


-------------------------------------------------------------------------------
-------------------------------- The Interpreter ------------------------------
-------------------------------------------------------------------------------

--Tipul folosit pentru a stoca perechi de forma (variabila, valoare).
type Dictionary a = [(String, a)] 

--Tipul folosit pentru a detecta erorile in timpul evaluarii arborelui de sintaxa al programului
data Result a = Val a | Error

--Verifica daca un element apartine unei liste
contains :: (Eq a) => a -> [a] -> Bool
contains elem list = foldr (||) False (map (\x -> (x == elem)) list)

--Adauga o pereche (variabila, valoare) intr-o tabela de hash, pentru variabilele deja existente,
--valoarea lor fiind suprascrisa
addKey :: String -> a -> Dictionary a -> Dictionary a
addKey key value hash = case (contains key (map (\pair -> (fst pair)) hash)) of
    False -> hash ++ [(key, value)]
    True -> map (\(k, v) -> if (k == key) then (k, value)
                            else (k, v)) hash

--Intoarce valoarea unei variabile stocata intr-o tabela de hash
valueOf :: String -> Dictionary Int -> Result Int
valueOf key hash = case (filter (\(k, value) -> (k == key)) hash) of
    [] -> Error
    [x] -> Val (snd x)


--Erorile ce pot aparea in timpul evaluarii
initialisationError = "Uninitialized variable"
assertError = "Assert failed"
returnError = "Missing return"

--Evalueaza arborele unei expresii ce contine doar operatori binari (operatorii aritmetici impreuna
--cu operatorii "<" si "=="). Este folosita o tabela de hash pentru a retine valoarea curenta a
--fiecarei variabile din expresie. Poate returna un String reprezentand o eroare de initializare, o
--valoare intreaga sau o valoare booleana.
evalExpr :: Dictionary Int -> Expr -> Either String (Either Bool Int)

--Pentru un nod de tip constanta, este returnata valoarea stocata in acesta
evalExpr hash (Value x) = Right (Right x)

--Pentru nodurile de tip variabila se verifica daca acestea au fost initializate, fiind returnata
--o eroare in caz negativ, altfel fiind returnata valoarea curenta
evalExpr hash (Symbol var) = case (valueOf var hash) of
    Error -> Left initialisationError
    (Val x) -> Right (Right x)

--Pentru nodurile de tip operator binar, sunt mai intai evaluati subarborii stang si drept ai
--acestora, fiind apoi aplicat operatorul asupra celor doua rezultate
evalExpr hash (Add left right) = case (evalExpr hash left, evalExpr hash right) of
    (Right (Right v1), Right (Right v2)) -> Right (Right (v1 + v2))
    _ -> Left initialisationError
evalExpr hash (Sub left right) = case (evalExpr hash left, evalExpr hash right) of
    (Right (Right v1), Right (Right v2)) -> Right (Right (v1 - v2))
    _ -> Left initialisationError
evalExpr hash (Mult left right) = case (evalExpr hash left, evalExpr hash right) of
    (Right (Right v1), Right (Right v2)) -> Right (Right (v1 * v2))
    _ -> Left initialisationError
evalExpr hash (Equal left right) = case (evalExpr hash left, evalExpr hash right) of
    (Right (Right v1), Right (Right v2)) -> Right (Left (v1 == v2))
    _ -> Left initialisationError
evalExpr hash (Smaller left right) = case (evalExpr hash left, evalExpr hash right) of
    (Right (Right v1), Right (Right v2)) -> Right (Left (v1 < v2))
    _ -> Left initialisationError                                                                                                                                               


--Functia auxiliara folosita pentru evaluarea unui program. Aceasta primeste ca parametrii o
--tabela de hash a variabilelor curente si un arbore de sintaxa. Este returnat un tuplu format
--din tabela de hash actualizata, rezultatul evaluarii (ce poate fi o eroare sau un numar intreg)
--si o valoare booleana ce indica daca programul contine o instructiune return.
eval :: Dictionary Int -> Prog -> (Dictionary Int, Either String Int, Bool)

--Pentru un nod de tip Eq, este adaugata in tabela de hash perechea (variabila, valoare) din nod
eval hash (Eq (Asgn var expr)) = case (evalExpr hash expr) of
    (Left error) -> (hash, Left error, False)
    (Right (Right x)) -> (addKey var x hash, Right 1, False)

--Pentru un nod de tip Seq, este evaluat mai intai primul subprogram, in cazul in care acesta
--contine un return, fiind intoarsa valoarea returnata de acesta. Altfel este evaluat si cel
--de-al doilea subprogram
eval hash (Seq prog1 prog2) = case (eval hash prog1) of
    (_, Left error, _) -> (hash, Left error, False)
    (hash1, result, True) -> (hash1, result, True)
    (hash1, _, _) -> case (eval hash1 prog2) of
        (_, Left error, _) -> (hash1, Left error, False)
        val -> val

--Pentru un nod de tip Assert, este evaluata expresia booleana continuta in acesta, iar daca aceasta
--este falsa este returnata eroarea corespunzatoare
eval hash (Assert expr) = case (evalExpr hash expr) of
    (Left error) -> (hash, Left error, False)
    (Right (Left res)) -> if (not res) then (hash, Left assertError, False)
                          else (hash, Right 1, False)

--Pentru un nod de tip Return, este intors rezultatul evaluarii expresiei continute in acesta
eval hash (Return expr) = case (evalExpr hash expr) of
    (Left error) -> (hash, Left error, False)
    (Right (Right x)) -> (hash, (Right x), True)

--Pentru un nod de tip If, este evaluata mai intai expresia booleana continuta in acesta, fiind
--evaluata apoi ramura corespunzatoare rezultatului
eval hash (If expr prog1 prog2) = case (evalExpr hash expr) of
    (Left error) -> (hash, Left error, False)
    (Right (Left res)) -> if res then (eval hash prog1)
                          else (eval hash prog2)

--Pentru un nod de tip For, este folosita functia auxiliara evalFor ce evalueaza mai intai primul
--Assign (corespunzator initializarii), fiind apoi evaluat nodul program impreuna cu nodul
--corespunzator incrementarii pana cand rezultatul evaluarii nodului conditie este fals
eval hash (For asgn1 expr asgn2 prog) = case (eval hash (Eq asgn1)) of
    (_, Left error, _) -> (hash, Left error, False)
    (hash1, _, _) -> evalFor hash1 expr asgn2 prog

evalFor :: (Dictionary Int) -> Expr -> Asgn -> Prog -> (Dictionary Int, Either String Int, Bool)
evalFor h exp asgn program = case (evalExpr h exp) of
    (Left error) -> (h, Left error, False)
    (Right (Left res)) -> if (not res) then (h, Right 1, False)
                          else case (eval h program) of
                                    (_, Left error, _) -> (h, Left error, False)
                                    (h1, _, _) -> case (eval h1 (Eq asgn)) of
                                        (_, Left error, _) -> (h1, Left error, False)
                                        (h2, _, _) -> evalFor h2 exp asgn program


--Functia principala de evaluare a unui program. Aceasta foloseste functia auxiliara eval, apelata
--avand ca parametru o tabela de hash vida. Se verifica daca a fost intoarsa o eraore si daca
--programul contine instructiunea return, altfel este returnat rezultatul intreg al evaluarii
evalAdt :: Prog -> Either String Int
evalAdt prog = let (hash, result, hasReturn) = eval [] prog in
               case result of
                    (Left error) -> (Left error)
                    _ -> if (not hasReturn) then (Left returnError)
                         else case result of
                                   (Right x) -> (Right x)
                                   _ -> (Left returnError)



--Tipul functiilor folosite pentru a parsa diferite elemente ale unui program
type Parser a = String -> Maybe (a, String)

--Combinatorul de redirectare pentru parsere
(.>>.) :: (Parser a) -> (a -> Parser b) -> (Parser b)
parser .>>. f = \s -> case (parser s) of
    Just (x, s') -> (f x) s'
    _ -> Nothing

--Combinatorul de "sau" intre parsere. Este returnat un parser ce incearca sa extraga elemente
--dintr-un string folosind pe rand cele doua parsere initiale
(.||.) :: (Parser a) -> (Parser a) -> (Parser a)
p .||. p' = \s -> case (p s) of
    Nothing -> (p' s)
    res -> res

--Creeaza un parser ce returneza o pereche formata din rezultatele obtinute folosind succesiv
--parserele initiale asupra unui String. Este util pentru inlantuirea mai multor parsere, fiind
--returnat un tuplu cu rezultatele extrase
(.^.) :: (Parser a) -> (Parser b) -> (Parser (a, b))
p .^. p' = \s -> case (p s) of
    Nothing -> Nothing
    Just (res1, rest) -> case (p' rest) of
        Nothing -> Nothing
        Just(res2, rest') -> Just ((res1, res2), rest')

--Combinatorul de parsere este asemanator cu cel de mai sus, diferenta fiind ca este ignorat
--rezultatul celei de-a doua parsari. Este util in inlanutuirea mai multor parsere, daca printre
--acestea se gasesc parsere ce extrag doar cuvinte cheie precum for, if din program, acestea
--nefiind folosite ulterior
(.^^.) :: (Parser a) -> (Parser b) -> (Parser a)
p .^^. p' = \s -> case (p s) of
    Nothing -> Nothing
    Just (res1, rest) -> case (p' rest) of
        Nothing -> Nothing
        Just(_, rest') -> Just (res1, rest')


--Elimina spatiile, tab-urile si caracterele '\n' dintr-un program. Singurele spatii ce nu
--sunt eliminate sunt cele ce vin dupa o instructiune return sau assert
remove_ws :: String -> String
remove_ws [] = []
remove_ws ('r':'e':'t':'u':'r':'n':' ':rest) = 'r':'e':'t':'u':'r':'n':' ':(remove_ws rest)
remove_ws ('a':'s':'s':'e':'r':'t':' ':rest) = 'a':'s':'s':'e':'r':'t':' ':(remove_ws rest)
remove_ws ('\n':rest) = remove_ws rest
remove_ws ('\t':rest) = remove_ws rest
remove_ws (' ':rest) = remove_ws rest
remove_ws (c:rest) = c:(remove_ws rest)

--Creaza un parser ce extrage string-ul primit ca parametru dintr-un string dat
reserved :: String -> Parser String
reserved operator = \s -> if ((take (length operator) s) == operator)
    then Just (operator, drop (length operator) s)
    else Nothing

--Extrage un nume de variabila din program
variable :: Parser Expr
variable [] = Nothing
variable (h:t) = if (not (isLetter h)) then Nothing else case (variable t) of
    Just (Symbol s, rest) -> Just (Symbol (h:s), rest)
    Nothing -> Just (Symbol (h:[]), t)

--Extrage un numar intreg dintr-un sir
extractNumber :: String -> String
extractNumber s = case s of
    [] -> []
    (h:t) -> if (not (isDigit h)) then []
             else h:(extractNumber t)

--Extrage o constanta dintr-un program, folosind functia extractNumber
constant :: Parser Expr
constant [] = Nothing
constant s = case (extractNumber s) of
    (h:t) -> Just (Value (read (h:t) :: Int), drop (length (h:t)) s)
    [] -> Nothing

--Extrage un operand dintr-un program, acesta putand fi o variabila sau o constanta
operand :: Parser Expr
operand = variable .||. constant

--Extrage o expresie ce contine doar inmultiri. Este extras mai intai un operand, iar in cazul
--in care primul caracter din string-ul ramas este '*', acesta este parsat in continuare
parseProduct :: Parser Expr
parseProduct s = case (operand s) of
    Just (expr1, '*':rest) -> case (parseProduct rest) of
        Just (expr2, rest') -> Just (Mult expr1 expr2, rest')
        Nothing -> Nothing
    res -> res

--Extrage o expresie ce contine doar inmultiri sau adunari. Este extrasa mai intai o expresie
--ce contine doar inmultiri, iar in cazul in care primul caracter din string-ul ramas este '+',
--acesta este parsat in continuare
parseProductSum :: Parser Expr
parseProductSum s = case (parseProduct s) of
    Just (expr1, '+':rest) -> case (parseProductSum rest) of
        Just (expr2, rest') -> Just (Add expr1 expr2, rest')
        Nothing -> Nothing
    res -> res

--Functia este folosita pentru a schimba semnele unei expresii dintr-un program, pentru a putea
--fi respectata precedenta operatorului '-'. Schimbarea se face pana cand este intalnita prima
--paranteza sau primul ';', pentru a nu afecta structura programului ce urmeaza dupa expresia ce
--este parsata
revSign :: String -> String
revSign [] = []
revSign (';':rest) = ';':rest
revSign (')':rest) = ')':rest
revSign ('+':rest) = '-':(revSign rest)
revSign ('-':rest) = '+':(revSign rest)
revSign (c:rest) = c:(revSign rest) 

--Parseaza o expresie matematica. Mai intai este extrasa o expresie ce contine doar adunari si
--scaderi. Daca au mai ramas caractere de parsat, iar primul dintre acestea este '-', sunt
--schimbate semnele expresiei ce urmeaza dupa minus, fiind apoi continuata parsarea.
parseMath :: Parser Expr
parseMath s = case (parseProductSum s) of
    Just (expr1, '-':rest) -> case (parseMath (revSign rest)) of
        Just (expr2, rest') -> Just (Sub expr1 expr2, rest')
        Nothing -> Nothing
    res -> res

--Extrage un operator de tip "<" sau "==" dintr-un program
opParser :: Parser String
opParser ('<':rest) = Just (['<'], rest)
opParser ('=':'=':rest) = Just ("==", rest)
opParser _ = Nothing

--Extrage operatorul "=" dintr-un program
parseEqual :: Parser String
parseEqual ('=':rest) = Just ("=", rest)
parseEqual _ = Nothing

--Parseaza o instructiune de comparare (de tip "<" sau "==") prin combinarea a doua parsere
--de expresii matematice cu un parser de tip operator
parseSmallerEqual :: Parser Expr
parseSmallerEqual = ((parseMath .^. opParser) .^. parseMath) .>>.
    (\((expr1, op), expr2) -> (\s -> if (op == "<") then Just (Smaller expr1 expr2, s)
                                     else Just (Equal expr1 expr2, s)))

--Parseaza o instructiune de tip assert prin extragerea succesiva a numelui instructiunii si
--a expresiei corespunzatoare
parseAssert :: Parser Prog
parseAssert = (reserved "assert ") .>>. (\x -> parseSmallerEqual) .>>.
    (\expr -> (\s -> Just (Assert expr, s)))

--Parseaza o instructiune de tip return prin extragerea succesiva a numelui instructiunii si
--a expresiei ce urmeaza sa fie returnata
parseReturn :: Parser Prog
parseReturn = (reserved "return ") .>>. (\x -> parseMath) .>>.
    (\x -> (\s -> Just (Return x, s)))

--Parseaza o instructiune de tip assign prin extragerea succesiva a numelui variabilei, a
--operatorul egal si a expresiei ce urmeaza sa ii fie asignata variabilei
parseAssign :: Parser Prog
parseAssign = ((variable .^. parseEqual) .^. parseMath) .>>.
    (\((Symbol var, op), expr) -> (\s -> Just (Eq (Asgn var expr), s)))

--Parseaza o instructiune de tip return, assert sau assign, impreuna cu delimitatorul ';'
parseBlock :: Parser Prog
parseBlock = ((parseReturn .||. parseAssert) .||. parseAssign) .^^. (reserved ";")

--Parseaza o secventa de subprograme. Mai intai este extrasa o instructiune de tip block (return,
--assert sau assign) sau o instructiune de tip if sau for. Daca s-a ajuns la finalul programului
--sau daca primul caracter din programul ramas este o acolada (ce semnifica terminarea unui
--subprogram corespunzator corpului unui for sau unei ramuri a unui if), parsarea se opreste. In
--caz contrar, sunt extrase in continuare subprograme pana cand este intalnita o eroare de sintaxa
--sau se ajunge la finalul programului
parseSeq :: Parser Prog
parseSeq s = case ((parseBlock .||. parseIf .||. parseFor .||. parseLoop) s) of
    Nothing -> Nothing
    Just (prog1, "") -> Just (prog1, [])
    Just (prog1, '}':rest) -> Just (prog1, '}':rest)
    Just (prog1, rest) -> case (parseSeq rest) of
        Nothing -> Nothing
        Just (prog2, rest') ->  Just (Seq prog1 prog2, rest')

--Parsarea unei instructiuni de tip if presupune parsarea conditiei, impreuna cu parsarea
--subprogramelor corespunzatoare celor doua ramuri ale instructiunii, acestea fiind combinate
--intr-un nod de tip If al arborelui de sintaxa. Parsarea intregii instructiuni este realizata
--si cu ajutorul parserelor ce extrag parantezele rotunde si acoladele, precum si a celor ce extrag
--cuvintele cheie if, then si else
parseIf :: Parser Prog
parseIf = (((((((((((((reserved "if") .^^. (reserved "(")) .^. parseSmallerEqual))
    .^^. (reserved ")")) .^^. (reserved "then")) .^^. (reserved "{")) .^. parseSeq)
    .^^. (reserved "}")) .^^. (reserved "else")) .^^. (reserved "{")) .^. parseSeq)
    .^^. (reserved "}"))
    .>>. (\((("if", expr), prog1), prog2) -> (\s -> Just (If expr prog1 prog2, s)))

--Parsarea unei instructiuni de tip For presupune parsarea initializarii, a conditiei de oprire,
--a incrementarii si a subprogramului corespunzator corpului instructiunii, acestea fiind combinate
--intr-un nod de tip For al arborelui de sintaxa. Parsarea intregii instructiuni este realizata
--si cu ajutorul parserelor ce extrag parantezele rotunde si acoladele, precum si a celui ce
--extrage cuvantul cheie for
parseFor :: Parser Prog
parseFor = (((((((((((reserved "for") .^^. (reserved "(")) .^. parseAssign) .^^. (reserved ";"))
    .^. parseSmallerEqual) .^^. (reserved ";")) .^. parseAssign) .^^. (reserved ")"))
    .^^. (reserved "{")) .^. parseSeq) .^^. (reserved "}"))
    .>>. (\(((("for", Eq init), cond), Eq asgn), prog) -> (\s -> Just (For init cond asgn prog, s)))

parseLoop :: Parser Prog
parseLoop = (((((((((reserved "loop") .^^. (reserved "(")) .^. variable) .^^. (reserved ";"))
    .^. parseSmallerEqual) .^^. (reserved ")")) .^^. (reserved "{")) .^. parseSeq) .^^. (reserved "}"))
    .>>. (\((("loop", Symbol var), cond), prog) -> (\s -> Just (For (Asgn var (Value 0)) cond (Asgn var (Add (Symbol var) (Value 1))) prog, s)))
    


--Functia ce parseaza un intreg program, folosind parserele specifice instructiunilor din program.
--Inainte de a se incepe parsarea, sunt eliminate spatiile, tab-urile si caracterele '\n' din
--cadrul programului
parse :: String -> Maybe Prog
parse s = case (parseSeq (remove_ws s)) of
    Nothing -> Nothing
    Just (prog, rest) -> Just prog

--Functia ce evalueaza un program dat sub forma de string, prin compunerea functiilor parse
--si evalAdt
evalRaw :: String -> Either String Int
evalRaw rawProg =
    case parse rawProg of
        Just prog -> evalAdt prog
        Nothing   -> Left "Syntax error"

















