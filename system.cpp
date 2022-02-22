#define _CRT_SECURE_NO_WARNINGS

#include<iostream>
#include<vector>
#include<fstream>
#include<string>
#include <algorithm>
#include<ctime>
using namespace std;


#define N 9

class sudoku_generator : public vector<vector<int>> {
	//Pomocniczy wiersz
	vector<int> line;
	//Pomocnicza kolumna
	vector<int> column;
	//Pomocnicza plansza do sudoku wykorzystywana do ulatwienia sobie sortowania elementow obiektu
	vector<vector<int>> sorted;

public:

	//Konstruktor ma za zadanie stworzyc obiekt w ktorym kazdy wiersz zawiera wartosci od 1 do 9 i zadna z tyc wartosci sie nie powtarza
	sudoku_generator() {
		for (int i = 0; i < N; i++) {
			//Ta petla ma za zadanie wypelnienie wiersza wartosciami od 1 do 9
			for (int j = 0; j < N; j++) {
				line.push_back(j+1);
			}
			//W tym momencie wiersz jest sortowany w sposob losowy tak aby ulatwic sobie pozniejsze sortowanie
			random_shuffle(line.begin(), line.end());
			push_back(line);
			//Po dodaniu wiersza do obiektu czyszcze wiersz pomocniczy 'line'
			line.clear();
			//Dla pewnosci sprawdzam czy 'line' jest puste, aby wartosci sie nie powtarzly
			if (!line.empty()) {
				cout << "Linia nie zostala opozniona!" << endl;
			}
		}
		sorting_sudoku();
		int dif = set_difficulty();
		hide_values(dif);
		show_sudoku();
	}
	

	//Metoda ma za zadanie posortowac sudoku tak, aby spelnialy wymagania gry
	void sorting_sudoku() {
		//Petla zewnetrzna (k) ma za zadanie odnosic sie do kolejnych kolumn planszy
		for (int k = 0; k < N; k++) {
			//Petla wewnetrzna (i) ma za zadanie odnosic sie do kolejnych wierszy
			for (int i = 0; i < N;i++) {
				//Debug
				//cout << i + 1 << ") ";

				//Tworze iterator na kolejne wiersze planszy
				vector<vector<int>>::iterator beg_line = begin() + i;
				//Pobieram wiersz
				line = *beg_line;
				//Tworze iterator na kolejne elementy wiersza
				vector<int>::iterator beg = line.begin() + k;

				//Sprawdzam czy vector column jest pusta. Pusty vector oznacza ze bedzie to pierwszy element vectora
				if (column.empty()) {
					//Debug
					//cout << "\tDobrze" << endl;

					column.push_back(*beg);
				}
				else {
					//Flaga ma sygnalizowac czy wszystkie operacje wewnatrz petli while sa spelnione
					bool flag = true;
					int move = 1;

					while (flag) {
						//Flaga zostaje od razu wylaczona. Jesli wszystko jest dobrze petla sie od razu zakonczy
						flag = false;
						for (int j = 0; j < i; j++) {
							//Iterator wskazuje na kolejne elementy vectora ktory sprawdzamy. Nie chcemy aby zadna wartosc sie powtarzala
							vector<int>::iterator check = column.begin() + j;
							//Sprawdzam czy badany element *beg powtarza sie juz w naszym vektorze
							//Ponadto jesli badamy ostatnia kolumne, to nie ma potrzeby sprawdzania czy wartosci sie powtarzaja bo nie ma kolejnej kolumny
							//z ktora mozna zamienic wartosci
							if (*beg == *check && k != N - 1) {
								//Tworze zmienna pomocnicza aby zamienic dwie wartosci ze soba
								int pom = *beg;

								//Debug
								//cout << "\tMOVE: " << move << "\t";

								//Upewniam sie czy nie wychodzimy juz poza ostatnia kolumne
								if (move + k < N) {
									//Zamieniam ze soba dwie wartosci
									*beg = *(beg + move);
									*(beg + move) = pom;

									//Debug
									//cout << "| Zamieniam " << pom << " z " << *beg;

									//Program syganlizuje ze zostaly wprowadzone poprawki i nalezy sprawdzic czy wszytko jest odbrze wiec powtarzamy petle
									flag = true;
									//Move zostaje zwiekszone, na wypadek gdyby sie okazalo ze zamiana wartosci nie podzialala. Bez tego wpadlibysmy w petle nieskonczona
									//zamieniajac ze soba stale te same dwie wartosci
									move++;
								}
								break;
							}
						}
					}

					//Debug
					//cout << "\tDobrze" << endl;

					//Zamieniam poprzedni wiersz na nowy, posortowany
					*beg_line = line;
					column.push_back(*beg);
				}

			}

			//Debug
			//cout << "Wielkosc column: " << column.size() << endl;

			sorted.push_back(column);
			//Czyszcze vector aby wartosci sie nie powtarzaly
			column.clear();
		}
		
	}

	//Ta metoda ma za zadanie wyœwietliæ zawartoœæ obiektu w taki sposob zeby wygladalo jak sudoku czyli w tabeli 9x9
	void show_sudoku() {
		//Iteratory wskazuja na wiersze w sudoku
		vector<vector<int>>::iterator beg = begin();
		vector<vector<int>>::iterator en = end();
		while (beg != en) {
			cout << "-------------------------------------" << endl;

			//'Line' ma za zadanie pobrac jeden wiersz na ktorym bede dalej pracowal
			line = *beg;

			//Iteratory wskazujace na pojedyncze elementy sudoku
			vector<int>::iterator b = line.begin();
			vector<int>::iterator e = line.end();

			//W tej petli wyswietlam kolejne elementy sudoku
			while (b != e) {
				if (*b == 0)
				{
					cout << "| " << "  ";
				}
				else {
					cout << "| " << *b << " ";
				}
				b++;
			}
			cout << "|" << endl;

			//Przechodze do kolejnego wiersza
			beg++;
		}
		cout << "-------------------------------------" << endl;
	}


	//Metoda ma za zadanie ukryc czesc wartosci sudoku
	void hide_values(int difficulty) {
		//czyszcze zawartosc obiektu poniewaz chce w niej zapisac ostateczny wyglad sudoku
		this->clear();

		vector<vector<int>>::iterator beg = sorted.begin();
		vector<vector<int>>::iterator en = sorted.end();
		while (beg != en) {
			line = *beg;
			vector<int>::iterator b = line.begin();
			vector<int>::iterator e = line.end();
			
			while (b != e) {

				//w sposob randomowy chce pozbyc sie czesci wartosci
				if (random_bool(difficulty)) {
					*b = 0;
				}
				b++;
			}
			push_back(line);
			beg++;
		}
	}
	
	bool random_bool(int dif) {
		int n;
		switch (dif)
		{
		case 1:
			n = 2;
			break;
		case 2:
			n = 5;
			break;
		case 3:
			n = 3;
			break;
		default:
			n = 2;
			break;
		}

		int x = (rand() % n) + 1;

		if (x % 2 == 0) {
			return false;
		}
		return true;
	}

	int set_difficulty() {
		int dif;
		cout << "Wybierz poziom trdnosci:" << endl;
		cout << "Easy - 1" << endl;
		cout << "Medium - 2" << endl;
		cout << "Hard - 3" << endl;
		cout << "Wybor: ";
		cin >> dif;
		return dif;
	}
	
};

int main(){
	srand(time(NULL));
	sudoku_generator gen;

	cout << "Koniec progrmu" << endl;
}

