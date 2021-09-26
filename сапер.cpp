#include <iostream>
#include <stdlib.h>
#include <string>
#include <time.h>
#include <fstream>
#include <conio.h>

using namespace std;

class Bombs {
    int** bombs;

    bool open(int x, int y) {
        if (bombs[x][y] == 1) {
            return true;
        }
        return false;
    }

    friend class Field;
    friend class Links;
    friend class Game;
    Bombs() {}

    Bombs(int width, int height, int bombs_amount, int* arr) {
        bombs = new int* [height];
        for (int i = 0; i < height; i++) {
            bombs[i] = new int[width];
            for (int j = 0; j < width; j++) {
                bombs[i][j] = 0;
            }
        }
        for (int i = 0; i < bombs_amount; i++) {
            bombs[arr[2 * i]][arr[2 * i + 1]] = 1;
        }
    }

    Bombs(int width, int height, int* bombs_amount) {
        string BOMBS;
        int success = 0;
        while (!success) {
            try {
                cout << "Введите количество бомб: ";
                cin >> BOMBS;
                *bombs_amount = stoi(BOMBS);
                if (*bombs_amount > width * height) {
                    cout << "Пожалуйста, введите количество < " << width * height << "." << endl;
                    continue;
                }
                if (*bombs_amount <= 0) {
                    cout << "Пожалуйста, введите количество > 0." << endl;
                    continue;
                }
                else {
                    success = 1;
                }
            }
            catch (...) {
                cout << "Некорректное количество бомб! Попробуйте снова." << endl;
            }
        }
        srand(time(NULL));
        bombs = new int* [height];
        for (int i = 0; i < height; i++) {
            bombs[i] = new int[width];
            for (int j = 0; j < width; j++) {
                bombs[i][j] = 0;
            }
        }
        int bomb;
        for (int i = 0; i < *bombs_amount; i++) {
            success = 0;
            while (!success) {
                bomb = rand() % (height * width);
                if (bombs[bomb / width][bomb % width] == 0) {
                    bombs[bomb / width][bomb % width] = 1;
                    success = 1;
                }
            }
        }
    }
};

class Field {
    friend class Bombs;
    friend class Links;
    friend class Game;
    int** field;
    Field() {}

    Field(int width, int height) {
        field = new int* [height];
        for (int i = 0; i < height; i++) {
            field[i] = new int[width];
            for (int j = 0; j < width; j++) {
                field[i][j] = -1;
            }
        }
    }

    Field(int width, int height, int* arr) {
        field = new int* [height];
        for (int i = 0; i < height; i++) {
            field[i] = new int[width];
            for (int j = 0; j < width; j++) {
                field[i][j] = arr[i * width + j];
            }
        }
    }

    void show(int width, int height) {
        cout << endl;
        int razr_width = 1;
        int razr_height = 1;
        int delitel_width = 10;
        int delitel_height = 10;
        int copy_height = height;
        int copy_width = width;
        while (copy_width / delitel_width != 0) {
            razr_width += 1;
            delitel_width *= 10;
        }
        while (copy_height / delitel_height != 0) {
            razr_height += 1;
            delitel_height *= 10;
        }
        delitel_width = 10;
        delitel_height = 10;
        int razr1_w = razr_width - 1;
        int razr1_h = razr_height - 1;
        for (int j = 0; j < razr_height + 1; j++) {
            cout << " ";
        }
        for (int i = 0; i < width; i++) {
            if ((i + 1) / delitel_width == i / delitel_width + 1) {
                razr1_w -= 1;
                delitel_width *= 10;
            }
            for (int j = 0; j < razr1_w; j++) {
                cout << " ";
            }
            cout << i + 1 << " ";
        }
        cout << endl;
        delitel_width = 10;
        razr1_w = razr_width - 1;
        for (int i = 0; i < 2 * height + 1; i++) {
            if (i % 2 == 0) {
                for (int j = 0; j < razr_height; j++) {
                    cout << " ";
                }
                for (int j = 0; j < width; j++) {
                    cout << " ";
                    for (int k = 0; k < razr_width; k++) {
                        cout << "_";
                    }
                }
                cout << endl;
            }
            else {
                if ((i / 2 + 1) / delitel_height == (i / 2) / delitel_height + 1) {
                    razr1_h -= 1;
                    delitel_height *= 10;
                }
                for (int j = 0; j < razr1_h; j++) {
                    cout << " ";
                }
                cout << i / 2 + 1;
                for (int j = 0; j < width; j++) {
                    cout << "|";
                    for (int k = 0; k < (razr_width - 1) / 2; k++) {
                        cout << " ";
                    }
                    if (field[i / 2][j] == -1) {
                        cout << "*";
                    }
                    if (field[i / 2][j] == -2) {
                        cout << "f";
                    }
                    if (field[i / 2][j] == -3) {
                        cout << "o";
                    }
                    if (field[i / 2][j] == 0) {
                        cout << " ";
                    }
                    if (field[i / 2][j] > 0) {
                        cout << field[i / 2][j];
                    }
                    for (int k = 0; k < razr_width / 2; k++) {
                        cout << " ";
                    }
                }
                cout << "|" << endl;
            }
        }
    }
    void reveal_bombs(int width, int height, Bombs bombs) {
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                if (bombs.bombs[i][j] == 1) {
                    field[i][j] = -3;
                }
            }
        }
    }
};

class Links {
    friend class Bombs;
    friend class Field;
    friend class Game;
    int** links;
    Links() {};
    Links(int width, int height, Bombs bombs) {
        links = new int* [height];
        for (int i = 0; i < height; i++) {
            links[i] = new int[width];
            for (int j = 0; j < width; j++) {
                int bombs_check = 0;
                int x = i + 1;
                int y = j + 1;
                if (x > 1) {
                    bombs_check += bombs.bombs[x - 2][y - 1];
                }
                if (y > 1) {
                    bombs_check += bombs.bombs[x - 1][y - 2];
                }
                if (x < height) {
                    bombs_check += bombs.bombs[x][y - 1];
                }
                if (y < width) {
                    bombs_check += bombs.bombs[x - 1][y];
                }
                if (x > 1 && y > 1) {
                    bombs_check += bombs.bombs[x - 2][y - 2];
                }
                if (x > 1 && y < width) {
                    bombs_check += bombs.bombs[x - 2][y];
                }
                if (x < height && y < width) {
                    bombs_check += bombs.bombs[x][y];
                }
                if (x < height && y > 1) {
                    bombs_check += bombs.bombs[x][y - 2];
                }
                links[i][j] = bombs_check;
            }
        }
    }
};

class Game {
    int success = 0;
    int processing = 1;
    int win = 0;
    int height, width, bombs_amount;
    Bombs bombs;
    Field field;
    Links links;
    int flags = 0;
    string filename = "сапер.txt";

    bool try_resume_last_game() {
        string unit, answer;
        ifstream fin(filename);
        fin >> unit;
        fin.close();
        if (unit != "") {
            cout << "Хотите продолжить старую игру? y/n ";
            cin >> answer;
            while (answer != "y" && answer != "n") {
                cout << "Ваш ответ не распознан. y/n ";
                cin >> answer;
            }
            if (answer == "y") {
                return true;
            }
            else {
                return false;
            }
        }
        return false;
    }

    void read_file() {
        ifstream fin(filename);
        fin >> height >> width >> bombs_amount;
        height = ~height;
        width = ~width;
        bombs_amount = ~bombs_amount;
        int* arr_field = new int[height * width];
        for (int i = 0; i < height * width; i++) {
            fin >> arr_field[i];
            arr_field[i] = ~arr_field[i];
        }
        Field field1(width, height, arr_field);
        field = field1;
        delete[] arr_field;
        int* arr_bombs = new int[bombs_amount * 2];
        for (int i = 0; i < 2 * bombs_amount; i++) {
            fin >> arr_bombs[i];
            arr_bombs[i] = ~arr_bombs[i];
        }
        Bombs bombs1(width, height, bombs_amount, arr_bombs);
        bombs = bombs1;
        delete[] arr_bombs;
        fin >> flags;
        flags = ~flags;
        fin.close();
        Links links1(width, height, bombs);
        links = links1;
    }

    void input_game_parameters() {
        ifstream fin(filename, ofstream::out | ofstream::trunc);
        fin.close();
        string WIDTH, HEIGHT;
        while (!success) {
            try {
                cout << "Введите ширину поля: ";
                cin >> WIDTH;
                width = stoi(WIDTH);
                if (width <= 0) {
                    cout << "Пожалуйста, введите ширину >0." << endl;
                    continue;
                }
                else {
                    success = 1;
                }
            }
            catch (...) {
                cout << "Некорректная ширина! Попробуйте снова." << endl;
            }
        }
        success = 0;
        while (!success) {
            try {
                cout << "Введите высоту поля: ";
                cin >> HEIGHT;
                height = stoi(HEIGHT);
                if (height <= 0) {
                    cout << "Пожалуйста, введите высоту > 0." << endl;
                    continue;
                }
                else {
                    success = 1;
                }
            }
            catch (...) {
                cout << "Некорректная высота! Попробуйте снова." << endl;
            }
        }
        Bombs bombs1(width, height, &bombs_amount);
        bombs = bombs1;
        Field field1(width, height);
        field = field1;
        Links links1(width, height, bombs);
        links = links1;
    }

    void write_in_the_file() {
        ofstream fout(filename, ofstream::out | ofstream::trunc);
        fout << ~height << " " << ~width << " " << ~bombs_amount << " ";
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                fout << ~field.field[i][j] << " ";
            }
        }
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                if (bombs.bombs[i][j] == 1) {
                    fout << ~i << " " << ~j << " ";
                }
            }
        }
        fout << ~flags;
        fout.close();
    }

    void turn_input(int* x, int* y, string* Action) {
        string X, Y;
        int success = 0;
        while (success != 3) {
            success = 0;
            cout << "строка, столбец, действие (Flag или Open): ";
            cin >> X >> Y >> *Action;
            try {
                *x = stoi(X);
                if (*x < 1 || *x > height) {
                    cout << "Некорректная строка! Попробуйте снова." << endl;
                    continue;
                }
                else {
                    success += 1;
                }
            }
            catch (...) {
                cout << "Некорректная строка! Попробуйте снова." << endl;
                continue;
            }
            try {
                *y = stoi(Y);
                if (*y < 1 || *y > width) {
                    cout << "Некорректный столбец! Попробуйте снова." << endl;
                    continue;
                }
                else {
                    success += 1;
                }
            }
            catch (...) {
                cout << "Некорректный столбец! Попробуйте снова." << endl;
                continue;
            }
            if (*Action != "Flag" && *Action != "Open") {
                cout << "Некорректное действие! Попробуйте снова." << endl;
                continue;
            }
            else {
                success += 1;
            }
        }
    }

    void player_turn() {
        string Action;
        int x, y;
        turn_input(&x, &y, &Action);
        if (Action == "Open") {
            if (bombs.bombs[x - 1][y - 1]) {
                processing = 0;
            }
            if (field.field[x - 1][y - 1] > -1) {
                cout << "Эта клетка уже открыта." << endl << endl;
            }
            if (processing) {
                open(x - 1, y - 1);
                if (field.field[x - 1][y - 1] == -2) {
                    flags -= 1;
                }
            }
        }
        else {
            flags += 1;
            if (field.field[x - 1][y - 1] == -2) {
                cout << "Клетка уже помечена флагом." << endl << endl;
            }
            if (field.field[x - 1][y - 1] == -1) {
                field.field[x - 1][y - 1] = -2;
                if (bombs.bombs[x - 1][y - 1] == 1) {
                    bombs_amount -= 1;
                }
            }
            if (field.field[x - 1][y - 1] >= 0) {
                cout << "Нельзя пометить флагом открытую клетку." << endl << endl;
            }
        }
    }

    void open(int line_tag, int column_tag) {
        if (links.links[line_tag][column_tag] != 0 && field.field[line_tag][column_tag] < 0) {
            field.field[line_tag][column_tag] = links.links[line_tag][column_tag];
        }
        else {
            field.field[line_tag][column_tag] = links.links[line_tag][column_tag];
            if (line_tag > 0 && field.field[line_tag - 1][column_tag] == -1) {
                open(line_tag - 1, column_tag);
            }
            if (line_tag < height - 1 && field.field[line_tag + 1][column_tag] == -1) {
                open(line_tag + 1, column_tag);
            }
            if (column_tag > 0 && field.field[line_tag][column_tag - 1] == -1) {
                open(line_tag, column_tag - 1);
            }
            if (column_tag < width - 1 && field.field[line_tag][column_tag + 1] == -1) {
                open(line_tag, column_tag + 1);
            }
            if (line_tag > 0 && column_tag > 0 && field.field[line_tag - 1][column_tag - 1] == -1) {
                open(line_tag - 1, column_tag - 1);
            }
            if (line_tag > 0 && column_tag < width - 1 && field.field[line_tag - 1][column_tag + 1] == -1) {
                open(line_tag - 1, column_tag + 1);
            }
            if (line_tag < height - 1 && column_tag < width - 1 && field.field[line_tag + 1][column_tag + 1] == -1) {
                open(line_tag + 1, column_tag + 1);
            }
            if (line_tag < height - 1 && column_tag > 0 && field.field[line_tag + 1][column_tag - 1] == -1) {
                open(line_tag + 1, column_tag - 1);
            }
        }
    }

    void process() {
        win = 0;
        int initial_bombs = bombs_amount;
        while (processing) {
            write_in_the_file();
            cout << "Осталось бомб: " << bombs_amount << endl;
            cout << "Ваш ход:" << endl;
            player_turn();
            if (bombs_amount == 0 && flags == initial_bombs) {
                processing = 0;
                win = 1;
            }
            if (processing) {
                field.show(width, height);
            }
        }
        ifstream fin(filename, ofstream::out | ofstream::trunc);
        fin.close();
    }

    void ending() {
        if (win) {
            field.show(width, height);
            cout << "Вы выиграли!" << endl;
        }
        else {
            field.reveal_bombs(width, height, bombs);
            field.show(width, height);
            cout << "Игра окончена!" << endl;
        }
    }
public:
    Game() {
        setlocale(LC_ALL, "rus");
        bool old_game = try_resume_last_game();
        if (old_game) {
            read_file();
        }
        else {
            input_game_parameters();
        }
        field.show(width, height);
        process();
        ending();
        _getch();
    }
};

//коды, лежащие в клетках field
//-1 - клетка не открыта
//-2 - клетка помечена флагом
//x>0 - клетка открыта и в смежных клетках х бомб

int main() {
    Game game;
    return 0;
}
