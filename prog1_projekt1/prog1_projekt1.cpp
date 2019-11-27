//autor: Błażej Bałęczny @PJATK 2019

/* Program tworzy tabelę o ilości rzędów i kolumn podanych przez użytkownika,
 * następnie wypełnia ją losowymi liczbai naturalnymi z zakresu 1-25. Po wygene-
 * rowaniu tabeli, liczy najkrótszą ścieżkę pomiędzy dowolną komórką pierwszej
 * kolumny, a dowolną komórką ostatniej kolumny, gdzie symboliczna odległość 
 * pomiędzy komórkami jest wyznaczona przez liczbę w komórce, która jest
 * komórką docelową danego przejścia. Przejście jest możliwe wyłącznie pomiędzy
 * sąsiadującymi komórkami. Za sąsiadujące komórki uznaje się takie, które leżą
 * w sąsiadujących kolumnach w tym samym wierszu ("obok") lub w wierszu o 
 * indeksie o jeden większym i mniejszym ("na ukos"). Punktem początkowym jest
 * abstrakcyjny punkt o wartości 0, który leży na lewo od pierwszej kolumny i 
 * sąsiaduje ze wszystkimi komórkami tejże. Po wyliczeniu wartości najkrótszej
 * trasy, program wyświetla sumę odległości wszystkich kroków oraz ich 
 * poszczególne wartości w kolejności, w jakiej przeszedł je program.
 */
                                                                                

#include <iostream>
#include <time.h>
#include <vector>
#include <numeric>

/* Funkcja makeArray() tworzy i zwraca dwuwymiarową tabelę, którą zapełniła 
 * liczbami naturalnymi z zakresu 1-25.
 */

std::vector<std::vector<int>> makeArray(int rows, int columns) {
	
	/* Użycie vectorów jest podyktowane koniecznością dynamicznego przypisania
	 * wielkości tabeli przez użytkownika w trakcie działania programu 
	 */
	std::vector<std::vector<int>> numberMatrix; // Ten vector zwraca funkcja
	srand(time(NULL));

	for (int row = 0; row < rows; row++) {
		
		// vector będący rzędem tabeli w jej graficznym przedstawieniu
		std::vector<int> rowVector; 
		
		for (int column = 0; column < columns; column++) {
			rowVector.push_back(rand() % 25 + 1);
		}
		numberMatrix.push_back(rowVector);
	}

	return numberMatrix;
}

/* Funkcja drawArray() wypisuje na ekran graficzne przedstawienie zawartości
 * tabeli.
 */

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
		std::vector<std::vector<int>> outerVector;
		for (int column = 0; column < numbers[row].size(); column++) {
			std::vector<int> innerVector;

			if (column == 0) innerVector.push_back(numbers[row][column]);

			//dla pozostalych kolumn jest to pusty vector
			outerVector.push_back(innerVector);
		}
		cost.push_back(outerVector);
	}

	int a; //glowna funkcjonalnosc polega na sprawdzaniu sum kosztow krokow, lezacych po lewej stronie od sprawdzanego
	int b; //miejsca w tabeli. Funkcja sprawdza trzy miejsca lezace po lewej stronie w przypadku elementow ze srodka kolumny, dwa w przypadku
	int c; //pierwszego i ostatniego miejsca w tabeli. Zostana one zapisane do zmiennych po lewej.
	int indexA = -1;
	int indexB = 0;
	int indexC = 1;
	int index;

	//petla sprawdza i zapelnia po kolei kolumny tabeli cost, pomijajac pierwsza kolumne, ktora zostala juz zapleniona.
	for (int column = 1; column < cost[0].size(); column++) {
		for (int row = 0; row < cost.size(); row++) {
			if (row == 0) {  // jezeli rzad ma indeks 0 (pierwszy rzad od gory), to nie sprawdzamy indeksu -1
				indexA = 0;
			}
			else if (row == cost.size() - 1) {  // jezeli rzad ma ostatni indeks, to nie sprawdzamy wiersza po nim
				indexC = 0;
			}

			// liczymy sumy kosztow dotarcia do trzech (badz dwoch) miejsc leacych w dostepnych komorkach po lewej stronie od komorki docelowej
			a = std::accumulate(cost[row + indexA][column - 1].begin(), cost[row + indexA][column - 1].end(), 0);
			b = std::accumulate(cost[row + indexB][column - 1].begin(), cost[row + indexB][column - 1].end(), 0);
			c = std::accumulate(cost[row + indexC][column - 1].begin(), cost[row + indexC][column - 1].end(), 0);

			//szukamy najmniejszej z policzonych sum i wskazujemy jej indeks wzgledem aktualnie sprawdzanej komorki
			//wyobrazajac sobie tabele graficznie, indeks A=-1 bedzie na skos do gory po lewej, indeks B=0 bezposrednio po lewej
			//zas indeks C=1 na skos w dol.
			if (a <= b && a <= c) {
				index = indexA;
			}
			else if (b <= a && b <= c) {
				index = indexB;
			}
			else if (c <= a && c <= b) {
				index = indexC;
			}

			//petla uzupelnia aktualnie sprawdzana komorke danymi z komorki lezacej po lewej stronie, do ktorej
			//prowadzila najkrotsza droga dojscia
			for (int number : cost[row + index][column - 1]) {
				cost[row][column].push_back(number);
			}
			cost[row][column].push_back(numbers[row][column]);

			indexA = -1; //przywracamy indeksy do domyslnych wartosci przed kolejnym krokiem petli
			indexC = 1;
		}
	}

	std::vector<int> result;

	unsigned long long temp = 25 * cost[0].size(); //zmienna przechowuje maksymalna mozliwa droge dojscia.

	//petla szuka najmniejszej sumy wartosci w ostatniej kolumnie, nastepnie przekazuje ja do zmiennej result
	for (int i = 0; i < cost.size(); i++) {
		int vectorSum = std::accumulate(cost[i][cost[i].size() - 1].begin(), cost[i][cost[i].size() - 1].end(), 0);
		if (vectorSum < temp) {
			temp = vectorSum;
			result = cost[i][cost[i].size() - 1];
		}
	}
	return result;
}

//funkcja wywoluje najwazniejsze funkcje programu i odpowiada za kontakt z uzytkownikiem
void interface() {
	int rows;
	int columns;
	//pobieramy dane wejsciowe
	std::cout << "Podaj liczbe rzedow: ";
	std::cin >> rows;
	std::cout << "Podaj liczbe kolumn: ";
	std::cin >> columns;
	//tworzymy tabele na podstawie danych wejsciowych
	std::vector<std::vector<int>> numbers = makeArray(rows, columns);
	//wyswietlamy tablice dla uzytkownika
	drawArray(numbers);
	//liczymy wynik
	std::vector<int> result = shortestPath(numbers);
	//wyswietlamy wynik
	for (int number : result) {
		std::cout << number << ' ';
	}
	std::cout << "\nSUMA: " << std::accumulate(result.begin(), result.end(), 0) << '\n';
}


int main() {
	interface();
	return 0;
}