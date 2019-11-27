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
 * kroków oraz jej składniki.
 */
                                                                                

#include <iostream>
#include <time.h>
#include <vector>
#include <numeric>
#include <algorithm>

/* Funkcja zwracająca wskaźnik do dwuwymiarowej tablicy, którą zapełniła 
 * losowymi liczbami naturalnymi z zakresu 1-25.
 */
int** make_array(int rows, int columns) {
	srand(time(NULL));

	// Wskaźnik do dwuwymiarowej tablicy o dynamicznie alokowanej wielkości
	int** number_matrix{ new int* [rows] };
	for (int i = 0; i < rows; i++) number_matrix[i] = new int[columns];
	
	// Pętle wypełniają tablicę losowymi liczbami
	for (int row = 0; row < rows; row++){
		for (int column = 0; column < columns; column++) {
			number_matrix[row][column] = rand() % 25 + 1;
		}
	}
	return number_matrix;
}

// Funkcja wypisująca na ekran graficzne przedstawienie zawartości tabeli.
void draw_array(int** numbers, int rows, int columns) {
	std::cout << "TABELA: \n";
	for (int row = 0; row < rows; row++) {
		for (int column = 0; column < columns; column++) {
			std::cout << numbers[row][column] << '\t';
		}
		std::cout << '\n';
	}
	std::cout << '\n';
}

/* Funkcja obliczajaca najkrótszą drogę od pierwszej do ostatniej kolumny dla
 * danej tablicy.
 */
std::vector<int> find_shortest_path(int** numbers, int rows, int columns) {

	/* Vector distance (w graficznym przedstawieniu tabela liczb) zawiera odległości
	 * dojścia do wszystkich kroków pośrednich. Jest zbudowany analogicznie, do
	 * tabeli wejściowej numbers, jednak każda komórka będzie zawierała 
	 * dodatkowo zestaw odległości, przez które funkcja przeszła do tej pory,
	 * szukając najkrótszej ścieżki. 
	 */
	std::vector<std::vector<std::vector<int>>> distance;

	/* Poniższa pętla wypełnia vector distance danymi początkowymi. W graficznym
	 * przedstawieniu, pierwsza kolumna jest zapełniana tymi samymi liczbami,
	 * co pierwsza kolumna tabeli wejściowej numbers. Pozostałe kolumny są
	 * zapełniane pustymi vectorami liczb.
	 */
	for (int row = 0; row < rows; row++) {
		
		std::vector<std::vector<int>> row_vector;

		for (int column = 0; column < columns; column++) {

			std::vector<int> cell_vector;

			if (column == 0) cell_vector.push_back(numbers[row][column]);
			row_vector.push_back(cell_vector);
		}
		distance.push_back(row_vector);
	}
	int indexA{ -1 }; // Delta indeksu komórki "na ukos w górę"	
	int indexB{ 0 }; // Delta indeksu komórki "obok"
	int indexC{ 1 }; // Delta indeksu komórki "na ukos w dół"

	/* Pętla sprawdza i zapełnia po kolei kolumny tabeli distance, pomijając 
	 * pierwszą kolumnę, która została już zapełniona.
	 */
	for (int column = 1; column < distance[0].size(); column++) {
		for (int row = 0; row < distance.size(); row++) {
			
			// jeżeli wiersz jest pierwszy, to nie sprawdzamy wiersza powyżej
			if (row == 0) {  
				indexA = 0 ;
			}
			// jeżeli wiersz jest ostatni, to nie sprawdzamy wiersza po nim
			if (row == distance.size() - 1) {  
				indexC = 0;
			}

			/* Liczymy sumy odległości od komórek sąsiadujących z komórką
			 * docelową z lewej strony.
			 */
			// Zmienna przechowująca sumę odległości z komórki z góry
			int a{ std::accumulate(distance[row + indexA][column - 1].begin(),
								   distance[row + indexA][column - 1].end(), 0) 
			};
			// Zmienna przechowująca sumę odległości z komórki obok
			int b{ std::accumulate(distance[row + indexB][column - 1].begin(),
								   distance[row + indexB][column - 1].end(), 0) 
			};
			// Zmienna przechowująca sumę odległości z komórki z dołu
			int c{ std::accumulate(distance[row + indexC][column - 1].begin(),
								   distance[row + indexC][column - 1].end(), 0) 
			};
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

			// Uzupełnamy komórkę najkrótszą drogą do niej prowadzącą
			for (int number : distance[row + index][column - 1]) {
				distance[row][column].push_back(number);
			}
			distance[row][column].push_back(numbers[row][column]);

			// Przywracamy wszystkie delty do domyślnych wartości
			indexA = -1;
			indexC = 1;
		}
	}
	/* Zmienna na najkrótszą odkrytą drogę rozwiązania. Zainicjalizowana na
	 * najwyższą możliwą wartość.
	 */
	unsigned long long lowest_sum{ 25 * distance[0].size() };

	std::vector<int> func_result;

	/* Pętla sprawdza po kolei komórki ostatniej kolumny i szuka takiej o
	 * najniższej sumie wartości.
	 */
	for (int i = 0; i < distance.size(); i++) {
		int vectorSum{ std::accumulate(distance[i][distance[i].size() - 1].begin(),
									   distance[i][distance[i].size() - 1].end(), 0)
		};
		if (vectorSum < lowest_sum) {
			lowest_sum = vectorSum;
			func_result = distance[i][distance[i].size() - 1];
		}
	}
	return func_result;
}

// Funkcja wywołuje najważniejsze funkcje programu i odpowiada za interfejs.
void interface() {
	int rows;
	int columns;
	std::cout << "Podaj liczbe wierszy: ";
	std::cin >> rows;
	std::cout << "Podaj liczbe kolumn: ";
	std::cin >> columns;
	int** numbers{ make_array(rows, columns) };
	draw_array(numbers, rows, columns);
	std::vector<int> result{ find_shortest_path(numbers, rows, columns) };
	
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