//autor: Błażej Bałęczny @PJATK 2019

/* Program znajduje najkrótszą ścieżkę, poruszając się po tabeli o wymiarach
 * podanych przez użytkownika, której komórki wypełnione są losowymi liczbami
 * naturalnymi z zakresu 1-25. Ścieżka szukana jest od pierwszej do ostatniej
 * kolumny. Odległość pomiędzy komórkami jest równa liczbie znajdującej się
 * w komórce docelowej. Przejście jest możliwe wyłącznie pomiędzy sąsiadującymi
 * komórkami. Za sąsiadujące komórki uznaje się takie, które leżą
 * w sąsiadujących kolumnach w tym samym wierszu ("obok") lub w wierszu
 * o indeksie o jeden większym i mniejszym ("na ukos"). Punktem początkowym jest
 * abstrakcyjny punkt o wartości 0, który leży na lewo od pierwszej kolumny
 * i sąsiaduje ze wszystkimi komórkami tejże. Po wyliczeniu wartości najkrótszej
 * trasy, program wyświetla odległości poszczególnych kroków oraz ich sumę.
 */


#include <iostream>
#include <time.h>
#include <vector>
#include <numeric>
#include <algorithm>

 /* Funkcja zwracająca wskaźnik do dwuwymiarowej tablicy, którą zapełniła
  * losowymi liczbami naturalnymi z zakresu 1-25.
  */
int** makeArray(int rows, int columns) {
    srand(time(NULL));

    // Wskaźnik do dwuwymiarowej tablicy o dynamicznie alokowanej wielkości
    int** numberMatrix{ new int* [rows] };
    for (int i = 0; i < rows; i++) numberMatrix[i] = new int[columns];

    // Pętle wypełniają tablicę losowymi liczbami
    for (int row = 0; row < rows; row++) {
        for (int column = 0; column < columns; column++) {
            numberMatrix[row][column] = rand() % 25 + 1;
        }
    }
    return numberMatrix;
}

// Funkcja wypisująca na ekran graficzne przedstawienie zawartości tabeli.
void printArray(int** numbers, int rows, int columns) {
    std::cout << "TABELA: \n";
    for (int row = 0; row < rows; row++) {
        for (int column = 0; column < columns; column++) {
            std::cout << numbers[row][column] << ' ';
            if (numbers[row][column] / 10 == 0) std::cout << ' ';
        }
        std::cout << '\n';
    }
    std::cout << '\n';
}

/* Funkcja obliczajaca najkrótszą drogę od pierwszej do ostatniej kolumny dla
 * danej tablicy. Zwraca wektor odległości poszczególnych kroków wyniku.
 */
std::vector<int> findPath(int** numbers, int rows, int columns) {

    /* Vector distance to vector o trzech poziomach zagnieżdżenia. Jego wielkość
     * jest tożsama z ilością rzędów tablicy numbers. Ilość vectorów w */
    std::vector<std::vector<std::vector<int>>> distance;
    for (int row = 0; row < rows; row++) {
        std::vector<std::vector<int>> rowVector;
        for (int column = 0; column < columns - 1; column++) {
            std::vector<int> cellVector;
            rowVector.push_back(cellVector);
        }
        distance.push_back(rowVector);
    }
    
    std::vector<int> result;
    int resultSum{ 25 * (columns - 1) };

    for (int column = 0; column < columns; column++) {
        for (int row = 0; row < rows; row++) {

            if (column == 0)
                distance[row][column].push_back(numbers[row][column]);
            else {
                int indexA{ row == 0 ? 0 : -1 };
                int indexB{ row == rows - 1 ? 0 : 1 };
                std::vector<int> smallestDistance;
                for (int index = indexA; index <= indexB; index++) {
                    std::vector<int> testCase { 
                        distance[row + index][column - 1] 
                    };
                    if (index == indexA) {
                        smallestDistance = testCase;
                    }
                    else {
                        int testCaseSum{ std::accumulate(testCase.begin(),
                            testCase.end(), 0) };
                        int smallestSum{ std::accumulate(smallestDistance.begin(),
                            smallestDistance.end(), 0) };
                        if (testCaseSum < smallestSum) smallestDistance = testCase;
                    }
                }
                smallestDistance.push_back(numbers[row][column]);
                if (column != columns - 1)
                distance[row][column] = smallestDistance;
                else {
                    int lastCellSum{ std::accumulate(smallestDistance.begin(),smallestDistance.end(),0) };
                    if (lastCellSum < resultSum) {
                        resultSum = lastCellSum;
                        result = smallestDistance;
                    }
                }
            }
        }
    }
    return result;
}

// Funkcja wywołuje najważniejsze funkcje programu i odpowiada za interfejs.
void interface() {
    int rows;
    int columns;
    std::cout << "Podaj liczbe wierszy: ";
    std::cin >> rows;
    std::cout << "Podaj liczbe kolumn: ";
    std::cin >> columns;
    int** numbers{ makeArray(rows, columns) };
    printArray(numbers, rows, columns);
    std::vector<int> result{ findPath(numbers, rows, columns) };

    // Wyświetlenie wyniku
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