# 🃏 Pasjans Klondike
Projekt realizowany w języku C++ w ramach 2. semestru studiów we współpracy w dwuosobowym zespole.  
Program umożliwia układanie klasycznego pasjansa Klondike, znanego m.in. z systemu Windows. Gra została w pełni zaimplementowana w konsoli z zachowaniem oryginalnych zasad oraz z użyciem kontenerów STL: vector, stack oraz queue.
## 🧭 Menu początkowe
![image](https://github.com/user-attachments/assets/da540d19-0c15-4317-981f-e7b9c99f093b)
## 🃏 Przykład początkowej rozgrywki
![image](https://github.com/user-attachments/assets/4776854c-c19e-46d1-8e2f-67c055f11258)
## 🎯 Cel gry
Celem gry jest przeniesienie wszystkich kart na cztery stosy (tzw. fundamenty), każdy posortowany rosnąco od Asa do Króla, zgodnie z kolorem (♠ ♣ ♥ ♦).
## 🧩 Zasady gry (Klondike)
Karty rozdawane są do 7 kolumn w układzie rosnącym (od 1 do 7 kart w kolumnie), gdzie widoczna jest tylko ostatnia karta.
Pozostałe karty trafiają do talii głównej (queue).
Dostępne są cztery fundamenty (stack), na które można przenosić karty zgodnie z zasadami:
* Zgodny kolor (np. same ♠).
* Rosnąca wartość (A → K).
* W kolumnach można przenosić karty w kolejności malejącej, naprzemiennie kolorami (np. czerwona 9 → czarny 8).
* Na puste kolumny można przenieść tylko Króla.
* Możliwość przeglądania talii i cofania się do początku.
## 🧠 Obsługa programu:
7 kolumn (wektory) – numerowane od 1 do 7  
4 stosy końcowe (fundamenty) – sloty 8–11  
Talia główna (queue) – slot 0  
Przykładowy system wpisywania ruchów:  
* >>źródło karta cel  
>>0 Kd 1  
>>2 10h 4  
