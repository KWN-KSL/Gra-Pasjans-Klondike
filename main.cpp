#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <queue>
#include <stack>
#include <algorithm>
#include <windows.h>
#include <string>
#include <map>
using namespace std;

struct Symbol {
    string symbol;
    int weight;
};

class Card {
public:
    Symbol symbol;
    string color;
    bool isFaceUp;
    Card(Symbol symbol, string color)
    {
        this->symbol = symbol;
        this->color = color;
        this->isFaceUp = false;
    }
    void flip()
    {
        if (this->isFaceUp == false)
            this->isFaceUp = true;
    }
};

void start(vector<vector<Card>> board, queue<Card> cardQueue, vector<stack<Card>> cardStack);
void program(vector<vector<Card>> board, queue<Card> cardQueue, vector<stack<Card>> cardStack);
void checkIfCardCanBeRemove(vector<vector<Card>>& board, queue<Card>& cardQueue, vector<stack<Card>>& cardStack, int fromField, int toField, string card);
void checkIfCardCanBePlace(vector<vector<Card>>& board, queue<Card>& cardQueue, vector<stack<Card>>& cardStack, int fromField, int toField, Card card);
void moveCard(vector<vector<Card>>& board, queue<Card>& cardQueue, vector<stack<Card>>& cardStack, int fromField, int toField, Card& card);
void move_Queue(queue<Card>& cardQueue);
void undoWriteToFile(vector<vector<Card>> board, queue<Card> cardQueue, vector<stack<Card>> cardStack);
void undoReadFromFile(vector<vector<Card>>& board, queue<Card>& cardQueue, vector<stack<Card>>& cardStack);
void generateDeck(vector<Card>& deck);
void shuffleDeck(vector<Card>& deck);
void writeDeck(vector<Card> deck);
void fillTheBoard(vector<Card>& deck, vector<vector<Card>>& board);
int maxCardInVector(vector<vector<Card>> board);
void printCard(Card card);
void printBoard(vector<vector<Card>> board, queue<Card>cardQueue, vector<stack<Card>> cardStack);
void fillTheQueue(vector<Card>& deck, queue<Card>& cardQueue);
void show_vectors(vector<vector<Card>> board);
void showQueue(queue<Card> cardQueue);
void checkIfGameIsEnded(vector<vector<Card>> board, queue<Card>cardQueue, vector<stack<Card>> cardStack);

string COLORS[] = { "h", "s", "d", "c" };
Symbol SYMBOLS[] = { {"A", 1}, {"2", 2}, {"3", 3}, {"4", 4}, {"5", 5}, {"6", 6}, {"7", 7}, {"8", 8}, {"9", 9}, {"10", 10}, {"J", 11}, {"Q", 12}, {"K", 13} };
map<char, int> SYMBOLVALUES = { {'A', 1}, {'2', 2}, {'3', 3}, {'4', 4}, {'5', 5}, {'6', 6}, {'7', 7}, {'8', 8}, {'9', 9}, {'1', 10}, {'J', 11}, {'Q', 12}, {'K', 13} };
map<char, bool> COLORSVALUES = { {'h', true}, {'s', false}, {'d', true}, {'c', false} }; // true - czerwone, false - czarne
bool canUndo = false;

int main()
{
    vector<Card> deck; /// poczatkowa talia 
    vector<vector<Card>> board(7); /// plansza dwuwyamirowa zawierajaca 7 kolumn po x kart
    queue<Card> cardQueue;  /// klejka trzech kart do dobierania
    vector<stack<Card>> cardStack(4); /// wektor czterech stosow
    canUndo = false;
    generateDeck(deck);
    shuffleDeck(deck);
    fillTheBoard(deck, board);
    fillTheQueue(deck, cardQueue);
    start(board, cardQueue, cardStack);
    return 0;
}

void start(vector<vector<Card>> board, queue<Card> cardQueue, vector<stack<Card>> cardStack)
{
    cout << endl << endl;
    cout << "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=" << endl << endl;
    cout << setw(75) << "Projekt 2 - Pasjans Klondike" << endl << endl;
    cout << setw(69) << "Autorzy projektu:" << endl << endl;
    cout << setw(86) << "KEWIN KISIEL - 197866 i MATEUSZ KUCZEROWSKI - 197900" << endl << endl;
    cout << "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=" << endl << endl;
    cout << setw(73) << "PRESS ENTER TO CONTINUE: ";
    cin.ignore(); /// to mi zgarnia enterka
    system("cls");
    program(board, cardQueue, cardStack);
}

void program(vector<vector<Card>> board, queue<Card> cardQueue, vector<stack<Card>> cardStack)
{
    printBoard(board, cardQueue, cardStack);
    string key = "";

    cout << "0 = Zmiana kart w kolejce." << endl;
    cout << "1 = Pokazanie kart w wektorach." << endl;
    cout << "2 = Pokazanie kart w kolejce." << endl;
    cout << "1,2,3,4,5,6,7 = Wektory" << endl;
    cout << "8,9,10,11 = Stosy" << endl;
    cout << "R/r = Restart rozgrywki." << endl;
    cout << "U/u = Cofniecie ruchu." << endl;
    cout << "Q/q = Wyjscie z programu." << endl;

    while (getline(cin, key))
    {
        if (key.length() == 1)
        {
            switch (key[0])
            {
            case '0':
                undoWriteToFile(board, cardQueue, cardStack);
                system("cls");
                move_Queue(cardQueue);
                checkIfGameIsEnded(board, cardQueue, cardStack);
                break;
            case '1':
                show_vectors(board);
                break;
            case '2':
                showQueue(cardQueue);
                break;
            case 'R':
            case 'r':
                canUndo = false;
                system("cls");
                main();
                break;
            case 'U':
            case 'u':
                if (canUndo == true)
                {
                    undoReadFromFile(board, cardQueue, cardStack);
                    canUndo = false;
                    system("cls");
                    checkIfGameIsEnded(board, cardQueue, cardStack);
                }
                else cout << "Nie mozna cofnac ruchu!" << endl;
                break;
            case 'Q':
            case 'q':
                canUndo = false;
                cout << "Program zostal zamkniety!" << endl;
                for (int i = 0; i < 7; i++)
                {
                    board[i].clear();
                    board[i].shrink_to_fit();
                }
                while (!cardQueue.empty()) cardQueue.pop();
                for (int i = 0; i < 3; i++)
                {
                    while (!cardStack[i].empty()) cardStack[i].pop();
                }
                exit(0);
                break;
            default:
                cout << "Podano nieprawidlowe dane!" << endl;
                break;
            }
        }
        else if (key.length() >= 6)
        {
            undoWriteToFile(board, cardQueue, cardStack);
            string temp;
            int fromField, toField, spaceCharIndex;
            string card;
            temp = key.substr(0, key.find(' '));
            fromField = atoi(temp.c_str()); // 0
            temp = key.substr(key.find(' ') + 1, key.length()); // "10h 2"
            spaceCharIndex = temp.find(' ');
            card = temp.substr(0, spaceCharIndex);
            toField = atoi(temp.substr(spaceCharIndex + 1, key.length()).c_str());
            checkIfCardCanBeRemove(board, cardQueue, cardStack, fromField, toField, card);
        }
        else
        {
            cout << "Podano nieprawidlowe dane!" << endl;
        }
    }
}

void checkIfCardCanBeRemove(vector<vector<Card>>& board, queue<Card>& cardQueue, vector<stack<Card>>& cardStack, int fromField, int toField, string card)
{
    if (fromField == 0 and !cardQueue.empty())
    {
        char temp = cardQueue.front().symbol.symbol[0]; // zamiana na char
        if (card[0] == temp)
        {
            temp = cardQueue.front().color[0];
            if (card[card.length() - 1] == temp) {
                checkIfCardCanBePlace(board, cardQueue, cardStack, fromField, toField, cardQueue.front());
            }
        }
        else cout << "Nie znaleziono karty w podanym miejscu!" << endl;
    }
    else if (fromField == 0 and cardQueue.empty()) cout << "Nie mozna zabrac karty z pustej kolejki!" << endl;
    else if (fromField >= 8 and fromField <= 11)
    {
        fromField = fromField - 8;
        char temp = cardStack[fromField].top().symbol.symbol[0]; // zamiana na char
        if (cardStack[fromField].top().symbol.weight == 1)
        {
            cout << "Nie mozna zabrac asa ze stosu!" << endl;
        }
        else if (cardStack[fromField].top().symbol.weight == 13)
        {
            cout << "Nie mozna zabrac krola ze stosu!" << endl;
        }
        else if (temp == card[0])
        {
            temp = cardStack[fromField].top().color[0];
            if (card[card.length() - 1] == temp) {
                fromField = fromField + 8;
                checkIfCardCanBePlace(board, cardQueue, cardStack, fromField, toField, cardStack[fromField - 8].top());
            }
        }
        else cout << "Nie znaleziono karty w podanym miejscu!5" << endl;
    }
    else
    {
        fromField--; ///zamaina na poprawene indeksowanie
        toField--;
        bool findCard = false;
        for (int i = 0; i < board[fromField].size(); i++) {
            if (board[fromField][i].isFaceUp == true) {
                char temp = board[fromField][i].symbol.symbol[0];
                if (card[0] == temp) {
                    temp = board[fromField][i].color[0];
                    if (card[card.length() - 1] == temp) {
                        int temp = fromField;
                        fromField++;
                        toField++;
                        checkIfCardCanBePlace(board, cardQueue, cardStack, fromField, toField, board[temp][i]);
                        findCard = true;
                        break;
                    }
                }
            }
        }
        if (findCard == false)
        {
            cout << "Nie znaleziono karty w podanym miejscu!" << endl;
        }
    }
}

void checkIfCardCanBePlace(vector<vector<Card>>& board, queue<Card>& cardQueue, vector<stack<Card>>& cardStack, int fromField, int toField, Card card)
{
    if ((toField >= 8 and toField <= 11) and (fromField >= 8 and fromField <= 11))
    {
        std::cout << " Nie mozna przenosic kart miedzy stosami!" << std::endl;
    }
    else if (toField >= 8 and toField <= 11)
    {
        toField = toField - 8;
        fromField--;
        if (cardStack[toField].empty()) {
            if (fromField == -1 and (cardQueue.front().symbol.weight == 1)) {
                cardStack[toField].push(cardQueue.front());
                cardQueue.pop();
                system("cls");
                checkIfGameIsEnded(board, cardQueue, cardStack);
            }
            else if (fromField == -1 and (cardQueue.front().symbol.weight != 1))
            {
                cout << "Nie mozna przeniesc karty." << endl;
            }
            else
            {
                if (board[fromField].back().symbol.weight == 1) {
                    cardStack[toField].push(board[fromField].back());
                    board[fromField].erase(board[fromField].end() - 1, board[fromField].end());
                    if (board[fromField].size() >= 1)
                    {
                        board[fromField].back().flip();
                    }
                    system("cls");
                    checkIfGameIsEnded(board, cardQueue, cardStack);
                }
                else cout << "Nie mozna przeniesc karty." << endl;
            }
        }
        else if (!cardStack[toField].empty())
        {
            if (((cardStack[toField].top().symbol.weight + 1) == card.symbol.weight) and (cardStack[toField].top().color == card.color))
            {
                toField = toField + 8;
                fromField++;
                moveCard(board, cardQueue, cardStack, fromField, toField, card);
            }
            else cout << "Nie mozna przeniesc karty." << endl;
        }
        else cout << "Nie mozna przeniesc karty." << endl;
    }
    else if (toField >= 1 and toField <= 7)
    {
        toField--;
        if (!board[toField].empty() and (((board[toField].back().symbol.weight - 1) == card.symbol.weight) and (COLORSVALUES[board[toField].back().color[0]] != COLORSVALUES[card.color[0]])))
        {
            toField++;
            moveCard(board, cardQueue, cardStack, fromField, toField, card);
        }
        else if (board[toField].empty() and (card.symbol.weight == 13)) //krol poloz na plansze
        {
            fromField--;
            if (fromField == -1 and (toField >= 0 and toField <= 6)) // z kolejki na wektor
            {
                board[toField].push_back(cardQueue.front());
                cardQueue.pop();
                fromField++;
            }
            else if ((fromField >= 0 and fromField <= 6) and (toField >= 0 and toField <= 6)) // z wektora na wektor
            {
                auto index = 0;
                while (true) {
                    if ((board[fromField][index].symbol.symbol == card.symbol.symbol) && (board[fromField][index].color == card.color)) break;
                    index++;
                }
                copy(board[fromField].begin() + index, board[fromField].end(), back_inserter(board[toField])); /// przenoszenie kart
                board[fromField].erase(board[fromField].begin() + index, board[fromField].end()); ///usuwanie kart
            }
            if (board[fromField].size() >= 1)
            {
                board[fromField].back().flip();
            }
            fromField++;
            system("cls");
            checkIfGameIsEnded(board, cardQueue, cardStack);
        }
        else cout << "Nie mozna przeniesc karty." << endl;
    }
    else cout << "Nie mozna przeniesc karty." << endl;
}

void moveCard(vector<vector<Card>>& board, queue<Card>& cardQueue, vector<stack<Card>>& cardStack, int fromField, int toField, Card& card)
{
    fromField--;
    toField--;
    if (fromField == -1 and (toField >= 0 and toField <= 6)) // z kolejki na wektor
    {
        board[toField].push_back(cardQueue.front());
        cardQueue.pop();
        system("cls");
        checkIfGameIsEnded(board, cardQueue, cardStack);
    }
    else if ((fromField >= 0 and fromField <= 6) and (toField >= 0 and toField <= 6)) // z wektora na wektor
    {
        auto index = 0;
        while (true) {
            if ((board[fromField][index].symbol.symbol == card.symbol.symbol) && (board[fromField][index].color == card.color)) break;
            index++;
        }
        copy(board[fromField].begin() + index, board[fromField].end(), back_inserter(board[toField])); /// przenoszenie kart
        board[fromField].erase(board[fromField].begin() + index, board[fromField].end()); ///usuwanie kart
        if (board[fromField].size() >= 1)
        {
            board[fromField].back().flip();
        }
        system("cls");
        checkIfGameIsEnded(board, cardQueue, cardStack);
    }
    else if (fromField == -1 and (toField >= 7 and toField <= 10)) // z kolejki na stos
    {
        toField = toField - 7;
        cardStack[toField].push(cardQueue.front());
        cardQueue.pop();
        system("cls");
        checkIfGameIsEnded(board, cardQueue, cardStack);
    }
    else if ((fromField >= 0 and fromField <= 6) and (toField >= 7 and toField <= 10)) // z wektora na stos
    {
        toField = toField - 7;
        cardStack[toField].push(board[fromField].back());
        board[fromField].erase(board[fromField].end() - 1, board[fromField].end());
        if (board[fromField].size() >= 1)
        {
            (board[fromField].back()).flip();
        }
        system("cls");
        checkIfGameIsEnded(board, cardQueue, cardStack);
    }
    else if ((fromField >= 7 and fromField <= 10) and (toField >= 0 and toField <= 6)) // ze stosu na wektor
    {
        fromField = fromField - 7;
        board[toField].push_back(cardStack[fromField].top());
        cardStack[fromField].pop();
        system("cls");
        checkIfGameIsEnded(board, cardQueue, cardStack);
    }
}

void move_Queue(queue<Card>& cardQueue) {

    if (cardQueue.size() > 1)
    {
        cardQueue.push(cardQueue.front()); /// dodaje pierwszy element na koniec kolejki
        cardQueue.pop(); //usuwa pierwszy element z kolejki
    }
    else if (cardQueue.size() == 1) cout << "Nie mozna przesunac kolejki, poniewaz jest tylko 1 karta!" << endl;
    else cout << "Nie mozna przesunac kolejki, poniewaz kolejka jest pusta!" << endl;
}

void undoWriteToFile(vector<vector<Card>> board, queue<Card> cardQueue, vector<stack<Card>> cardStack)
{
    ofstream file;
    file.open("undo.txt", ios::trunc);

    if (file.is_open()) {
        file << cardQueue.size() << std::endl;
        while (!cardQueue.empty()) {
            file << cardQueue.front().symbol.symbol << " ";
            file << cardQueue.front().symbol.weight << " ";
            file << cardQueue.front().color << " ";
            file << cardQueue.front().isFaceUp << " ";
            cardQueue.pop();
        }
        for (int i = 0; i < 7; i++)
        {
            file << std::endl << board[i].size() << std::endl;
            for (int j = 0; j < board[i].size(); j++)
            {
                file << board[i][j].symbol.symbol << " ";
                file << board[i][j].symbol.weight << " ";
                file << board[i][j].color << " ";
                file << board[i][j].isFaceUp << " ";
            }
        }
        for (int i = 0; i < 4; i++)
        {
            int stackSize = 0;
            stackSize = cardStack[i].size();
            file << std::endl << stackSize << endl;
            if (cardStack[i].empty())
            {
                file << " ";
            }
            else
            {
                for (int j = 0; j < stackSize; j++)
                {
                    file << cardStack[i].top().symbol.symbol << " ";
                    file << cardStack[i].top().symbol.weight << " ";
                    file << cardStack[i].top().color << " ";
                    file << cardStack[i].top().isFaceUp << " ";
                    cardStack[i].pop();
                }
            }
        }
        canUndo = true;
    }
    else cout << "Plik nie zostal otwarty!" << std::endl;
    file.close();
}

void undoReadFromFile(vector<vector<Card>>& board, queue<Card>& cardQueue, vector<stack<Card>>& cardStack)
{
    ifstream file;
    file.open("undo.txt");

    if (file.is_open()) {

        int capacity = 0; ///zmienna pomocnicza
        Card card({ "0",0 }, "0"); ///zmienna pomocnicza
        vector<Card> tab_cards;

        for (int i = 0; i < 7; i++) board[i].clear();
        while (!cardQueue.empty()) cardQueue.pop();
        for (int i = 0; i < 3; i++)
        {
            while (!cardStack[i].empty()) cardStack[i].pop();
        }

        file >> capacity; // wczytanie kolejki
        while (capacity--) {
            file >> card.symbol.symbol >> card.symbol.weight >> card.color >> card.isFaceUp;
            cardQueue.push(card);
        }

        capacity = 0; // wczytanie wektorow
        for (int i = 0; i < 7; i++)
        {
            file >> capacity;
            while (capacity--) {
                file >> card.symbol.symbol >> card.symbol.weight >> card.color >> card.isFaceUp;
                board[i].push_back(card);
            }
            //cout << endl;
        }

        capacity = 0; // wczytanie stosow
        for (int i = 0; i < 4; i++)
        {
            tab_cards.clear();
            file >> capacity;
            if (capacity == 0)
            {
                file.ignore('\n', 256);
                continue;
            }
            while (capacity--) {
                file >> card.symbol.symbol >> card.symbol.weight >> card.color >> card.isFaceUp;
                tab_cards.push_back(card);
            }
            int stackSize = 0;
            stackSize = tab_cards.size();
            for (int x = 0; x < stackSize; x++)
            {
                cardStack[i].push(tab_cards.back());
                tab_cards.erase(tab_cards.end() - 1, tab_cards.end());
            }
        }
    }
    file.close();
}

void generateDeck(vector<Card>& deck)
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 13; j++)
        {
            deck.push_back(Card(SYMBOLS[j], COLORS[i]));
        }
    }
}

void shuffleDeck(vector<Card>& deck)
{
    srand(time(NULL));
    for (int i = 0; i < deck.size(); i++)
    {
        int j = rand() % deck.size();
        swap(deck[i], deck[j]);
    }
}

void writeDeck(vector<Card> deck)
{
    for (int i = 0; i < deck.size(); i++)
    {
        cout << deck[i].symbol.symbol << deck[i].color << " - " << deck[i].symbol.weight << endl;
    }
}

void fillTheBoard(vector<Card>& deck, vector<vector<Card>>& board)
{
    for (int i = 0; i < 7; i++)
    {
        for (int j = 0; j < i + 1; j++)
        {
            board[i].push_back(deck[0]);
            deck.erase(deck.begin());
            if (j == i)
            {
                board[i][j].flip();
            }
        }
    }
}

int maxCardInVector(vector<vector<Card>> board)
{
    int maxCard = 0;
    for (int i = 0; i < 7; i++)
    {
        if (board[i].size() > maxCard)
        {
            maxCard = board[i].size();
        }
    }
    return maxCard;
}

void printCard(Card card)
{
    int cardMaxLength = 3;
    int cardLength = card.symbol.symbol.size() + card.color.size();
    if (card.isFaceUp)
    {
        if (cardLength < cardMaxLength)
        {
            cout << " ";
        }
        cout << card.symbol.symbol << card.color;
    }
    else
    {
        cout << "===";
    }
}

void printBoard(vector<vector<Card>> board, queue<Card>cardQueue, vector<stack<Card>> cardStack)
{
    std::cout << " 0" << setw(40) << "8  9  10 11" << std::endl;
    cout << "-------------------------------------------" << endl;
    if (cardQueue.empty()) cout << "0";
    else cout << " " << cardQueue.front().symbol.symbol << cardQueue.front().color;
    std::cout << setw(30);
    for (int i = 0; i < 4; i++) {
        if (cardStack[i].empty()) {
            std::cout << " 0 ";
        }
        else {
            std::cout << cardStack[i].top().symbol.symbol << cardStack[i].top().color << " ";
        }
    }
    std::cout << std::endl << "(" << cardQueue.size() << ")" << endl;
    cout << "-------------------------------------------" << endl << endl;
    for (int j = 0; j < maxCardInVector(board); j++)
    {
        for (int i = 0; i < 7; i++)
        {
            cout << "| ";
            if (j < board[i].size())
            {
                if (j == (board[i].size() - 1)) {
                    board[i][j].flip();
                }
                printCard(board[i][j]); /// j-wiersz i-kolumna(zamienic nazwy)
            }
            else
            {
                cout << "   ";
            }
            cout << " ";
        }
        cout << "|" << endl << endl;
    }
    for (int i = 0; i < 7; i++) {
        std::cout << "   " << i + 1 << "  ";
    }
    std::cout << std::endl << std::endl;;
}

void fillTheQueue(vector<Card>& deck, queue<Card>& cardQueue)
{
    int x = deck.size();
    for (int i = 0; i < x; i++)
    {
        deck[0].flip();
        cardQueue.push(deck[0]);
        deck.erase(deck.begin());
    }
}

void show_vectors(vector<vector<Card>> board)
{
    cout << "Elementy wektorow: " << endl;
    for (int i = 0; i < 7; i++) {
        for (int x = 0; x < board[i].size(); x++) {
            std::cout << board[i][x].symbol.symbol << board[i][x].color << " " << board[i][x].isFaceUp << " ";
        }
        std::cout << std::endl;
    }
    cout << endl;
}

void showQueue(queue<Card> cardQueue)
{
    std::cout << "Elementy kolejki: " << endl;
    while (!(cardQueue.empty()))
    {
        std::cout << cardQueue.front().symbol.symbol << cardQueue.front().color << " " << cardQueue.front().isFaceUp << " ";
        cardQueue.pop();
    }
    cout << endl;
}

void checkIfGameIsEnded(vector<vector<Card>> board, queue<Card>cardQueue, vector<stack<Card>> cardStack)
{
    string key = "";
    if ((cardStack[0].size() == 13) and (cardStack[1].size() == 13) and (cardStack[2].size() == 13) and (cardStack[3].size() == 13))
    {
        cout << "Gratulacje!!!" << endl << "Udalo ci sie ukonczyc gre!" << endl;
        cout << "Wpisz R/r aby zrestartowac rozgrywke lub Q/q aby wyjsc z programu" << endl;
        while (getline(cin, key))
        {
            switch (key[0])
            {
            case 'R':
            case 'r':
                canUndo = false;
                system("cls");
                main();
                break;
            case 'Q':
            case 'q':
                canUndo = false;
                cout << "Program zostal zamkniety!" << endl;
                exit(0);
                break;
            default:
                cout << "Podano nieprawidlowe dane!" << endl;
                break;
            }
        }
    }
    else program(board, cardQueue, cardStack);
}