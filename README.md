# Prog1
Repozytorium na projekt Programowanie 1 PJATK 2019

Program tworzy tabelę o ilości wierszy i kolumn podanych przez użytkownika,
następnie wypełnia ją losowymi liczbami naturalnymi z zakresu 1-25.
Po wygenerowaniu tabeli, liczy najkrótszą ścieżkę pomiędzy dowolną komórką
pierwszej kolumny, a dowolną komórką ostatniej kolumny, gdzie symboliczna 
odległość pomiędzy komórkami jest wyznaczona przez liczbę w komórce, która
jest komórką docelową danego przejścia. Przejście jest możliwe wyłącznie
pomiędzy sąsiadującymi komórkami. Za sąsiadujące komórki uznaje się takie,
które leżą w sąsiadujących kolumnach w tym samym wierszu ("obok") lub 
w wierszu o indeksie o jeden większym i mniejszym ("na ukos"). Punktem 
początkowym jest abstrakcyjny punkt o wartości 0, który leży na lewo od
pierwszej kolumny i sąsiaduje ze wszystkimi komórkami tejże. Po wyliczeniu
wartości najkrótszej trasy, program wyświetla sumę odległości wszystkich 
kroków oraz jej składniki.
