//autor: Błażej Bałęczny @PJATK 2019

/* Program tworzy tabelę o ilości wierszy i kolumn podanych przez użytkownika,
 * następnie wypełnia ją losowymi liczbami naturalnymi z zakresu 1-25. 
 * Po wygenerowaniu tabeli, liczy najkrótszą ścieżkę pomiędzy dowolną komórką
 * pierwszej kolumny, a dowolną komórką ostatniej kolumny, gdzie symboliczna 
 * odległość pomiędzy komórkami jest wyznaczona przez liczbę w komórce, która
 * jest komórką docelową danego przejścia. Przejście jest możliwe wyłącznie
 * pomiędzy sąsiadującymi komórkami. Za sąsiadujące komórki uznaje się takie,
 * które leżą w sąsiadujących kolumnach w tym samym wierszu ("obok") lub 
 * w wierszu o indeksie o jeden większym i mniejszym ("na ukos"). Punktem 
 * początkowym jest abstrakcyjny punkt o wartości 0, który leży na lewo od
 * pierwszej kolumny i sąsiaduje ze wszystkimi komórkami tejże. Po wyliczeniu
 * wartości najkrótszej trasy, program wyświetla sumę odległości wszystkich 
 * kroków oraz ich poszczególne wartości w kolejności, w jakiej przeszedł je
 * program.
 */
                                                                                

#include <iostream>
#include <time.h>
#include <vector>
#include <numeric>
#include <algorithm>

/* Funkcja tworząca i zwracająca dwuwymiarową tabelę, którą zapełniła liczbami
 * naturalnymi z zakresu 1-25.
 */
std::vector<std::vector<int>> makeArray(int rows, int columns) {
	
	/* Użycie vectorów jest podyktowane koniecznością dynamicznego przypisania
	 * wielkości tabeli przez użytkownika w trakcie działania programu 
	 */
	std::vector<std::vector<int>> numberMatrix; // Ten vector zwraca funkcja
	srand(time(NULL));

	for (int row = 0; row < rows; row++) {
		
		// vector będący wierszem tabeli w jej graficznym przedstawieniu
		std::vector<int> rowVector; 
		
		for (int column = 0; column < columns; column++) {
			rowVector.push_back(rand() % 25 + 1);
		}
		numberMatrix.push_back(rowVector);
	}

	return numberMatrix;
}

// Funkcja wypisująca na ekran graficzne przedstawienie zawartości tabeli.
void drawArray(std::vector<std::vector<int>> numbers) {
	std::cout << "TABELA: \n";
	for (std::vector<int> row : numbers) {
		for (int number : row) {
			std::cout << number << '\t';
		}
		std::cout << '\n';
	}

	std::cout << '\n';
}

/* Funkcja obliczajaca najkrótszą drogę od pierwszej do ostatniej kolumny dla
 * danej tabeli.
 */
std::vector<int> shortestPath(std::vector<std::vector<int>> numbers) {

	/* Vector cost (w graficznym przedstawieniu tabela liczb) zawiera odległości
	 * dojścia do wszystkich kroków pośrednich. Jest zbudowana analogicznie, do
	 * tabeli wejściowej numbers, jednak każda komórka zawiera dodatkowo zestaw
	 * odległości, przez które funkcja przeszła do tej pory, szukając 
	 * najkrótszej ścieżki. 
	 */
	std::vector<std::vector<std::vector<int>>> cost;

	/* Poniższa pętla wypełnia vector cost danymi początkowymi. W graficznym
	 * przedstawieniu, pierwsza kolumna jest zapełniana tymi samymi liczbami,
	 * co pierwsza kolumna tabeli wejściowej numbers. Pozostałe kolumny są
	 * zapełniane pustymi vectorami liczb.
	 */
	for (int row = 0; row < numbers.size(); row++) {
		
		// Lokalna zmienna będąca wierszem tabeli cost
		std::vector<std::vector<int>> rowVector;

		for (int column = 0; column < numbers[row].size(); column++) {

			// Lokalna zmienna będąca komórką tabeli cost
			std::vector<int> cellVector;

			if (column == 0) cellVector.push_back(numbers[row][column]);
			rowVector.push_back(cellVector);
		}
		cost.push_back(rowVector);
	}

	// Zmienna przechowująca deltę indeksu komórki "na ukos w górę"
	int indexA{ -1 };
	// Zmienna przechowująca deltę indeksu komórki "obok"
	int indexB{ 0 };
	// Zmienna przechowująca deltę indeksu komórki "na ukos w dół"
	int indexC{ 1 };

	/* Pętla sprawdza i zapełnia po kolei kolumny tabeli cost, pomijając 
	 * pierwszą kolumnę, która została już zapłeniona.
	 */
	for (int column = 1; column < cost[0].size(); column++) {
		for (int row = 0; row < cost.size(); row++) {
			
			// jeżeli wiersz jest pierwszy od góry, to nie sprawdzamy indeksu -1
			if (row == 0) {  
				indexA = 0 ;
			}

			// jeżeli wiersz jest ostatni, to nie sprawdzamy wiersza po nim
			else if (row == cost.size() - 1) {  
				indexC = 0;
			}

			/* Liczymy sumy kosztów dotarcia od komórek sąsiadujących z komórką
			 * docelową od lewej strony.
			 */
			// Zmienna przechowująca sumę kosztów dotarcia z komórki z góry
			int a{ std::accumulate(cost[row + indexA][column - 1].begin(),
									cost[row + indexA][column - 1].end(), 0) 
			};
			// Zmienna przechowująca sumę kosztów dotarcia z komórki obok
			int b{ std::accumulate(cost[row + indexB][column - 1].begin(),
									cost[row + indexB][column - 1].end(), 0) 
			};
			// Zmienna przechowująca sumę kosztów dotarcia z komórki z dołu
			int c{ std::accumulate(cost[row + indexC][column - 1].begin(),
									cost[row + indexC][column - 1].end(), 0) 
			};

			// Zmienna na najmniejszą sumę kosztów dotarcia do komórki
			int smallest{ std::min(a, std::min(b, c)) };

			// Szukamy najmniejszej z policzonych sum i wskazujemy jej indeks.
			int index;
			if (smallest == a) {
				index = indexA;
			}
			else if (smallest == b) {
				index = indexB;
			}
			else {
				index = indexC;
			}

			// Uzupełnamy komórkę kosztami najkrótszej drogi do niej prowadzącej
			for (int number : cost[row + index][column - 1]) {
				cost[row][column].push_back(number);
			}
			cost[row][column].push_back(numbers[row][column]);

			// Przywracamy wszystkie delty do domyślnych wartości
			indexA = -1;
			indexC = 1;
		}
	}
	/* Zmienna na najniższą odkrytą drogę rozwiązania. Zainicjalizowana na
	 * najwyższą możliwą wartość.
	 */
	unsigned long long low_sum{ 25 * cost[0].size() };

	std::vector<int> func_result;

	/* Pętla sprawdza po kolei komórki ostatniej kolumny i szuka takiej o
	 * najniższej sumie wartości.
	 */
	for (int i = 0; i < cost.size(); i++) {
		int vectorSum{ std::accumulate(cost[i][cost[i].size() - 1].begin(),
									   cost[i][cost[i].size() - 1].end(), 0)
		};
		if (vectorSum < low_sum) {
			low_sum = vectorSum;
			func_result = cost[i][cost[i].size() - 1];
		}
	}
	return func_result;
}

// Funkcja wywołuje najważniejsze funkcje programu i odpowiada za interfejs.
void interface() {
	int rows;
	int columns;
	std::cout << "Podaj liczbe rzedow: ";
	std::cin >> rows;
	std::cout << "Podaj liczbe kolumn: ";
	std::cin >> columns;
	std::vector<std::vector<int>> numbers = makeArray(rows, columns);
	drawArray(numbers);
	std::vector<int> result = shortestPath(numbers);
	
	// Wyświetlamy wynik
	for (int number : result) {
		std::cout << number << ' ';
	}
	std::cout << "\nSUMA: " << std::accumulate(result.begin(), result.end(), 0) 
			  << '\n';
}


int main() {
	interface();
	return 0;
}