from random import *

class Bombs:

    def __init__ (self, width, height, bombs_amount, arr):
        if (len(arr)) == 0:
            success = 0
            while (not success):
                try:
                    print("Введите количество бомб: ", end = "")
                    bombs_amount[0] = int(input())
                    if (bombs_amount[0] > width * height):
                        print("Пожалуйста, введите количество < ", width * height, ".")
                        continue
                    if (bombs_amount[0] <= 0):
                        print("Пожалуйста, введите количество > 0.")
                        continue
                    else:
                        success = 1
                except ValueError:
                    print("Некорректное количество бомб! Попробуйте снова.")
            self.bombs = [0]*height;
            for i in range (height):
                self.bombs[i] = [0]*width
            for i in range (bombs_amount[0]):
                success = 0;
                while (not success):
                    bomb = randint(0, height*width - 1)
                    if (self.bombs[bomb // width][bomb % width] == 0):
                        self.bombs[bomb // width][bomb % width] = 1
                        success = 1
        else:
            self.bombs = [0]*height
            for i in range (height):
                self.bombs[i] = [0]*width
            for i in range (bombs_amount[0]):
                self.bombs[arr[2 * i]][arr[2 * i + 1]] = 1
    
    def open(self, x, y):
        if (self.bombs[x][y] == 1):
            return True
        return false
        

class Field:
    def __init__ (self, width, height, arr):
        self.field = [-1] * height
        for i in range (height):
            self.field[i] = [-1] * width
            if (len(arr) != 0):
                for j in range (width):
                    self.field[i][j] = arr[i * width + j]
                
    def show(self, width, height):
        print()
        razr_width = 1;
        razr_height = 1;
        delitel_width = 10;
        delitel_height = 10;
        copy_height = height;
        copy_width = width;
        while (copy_width // delitel_width != 0):
            razr_width += 1
            delitel_width *= 10
        while (copy_height // delitel_height != 0):
            razr_height += 1
            delitel_height *= 10
        delitel_width = 10
        delitel_height = 10
        razr1_w = razr_width - 1
        razr1_h = razr_height - 1
        for j in range (razr_height+1):
            print(" ", end = "")
        for i in range (width):
            if ((i + 1) // delitel_width == i // delitel_width + 1):
                razr1_w -= 1
                delitel_width *= 10
            for j in range (razr1_w):
                print(" ", end = "")
            print(i + 1, " ", sep = "", end = "")
        print()
        delitel_width = 10;
        razr1_w = razr_width - 1;
        for i in range (2*height + 1):
            if (i % 2 == 0):
                for j in range (razr_height):
                    print(" ", end = "")
                for j in range (width):
                    print(" ", end = "")
                    for k in range (razr_width):
                        print("_", end = "")
                print()
            else:
                if ((i // 2 + 1) // delitel_height == (i // 2) // delitel_height + 1):
                    razr1_h -= 1
                    delitel_height *= 10
                for j in range (razr1_h):
                    print(" ", end = "")
                print(i // 2 + 1, end = "")
                for j in range (width):
                    print("|", end = "")
                    for k in range ((razr_width - 1) // 2):
                        print(" ", end = "")
                    if (self.field[i // 2][j] == -1):
                        print("*", end = "")
                    if (self.field[i // 2][j] == -2):
                        print("f", end = "")
                    if (self.field[i // 2][j] == -3):
                        print("o", end = "")
                    if (self.field[i // 2][j] == 0):
                        print(" ", end = "")
                    if (self.field[i // 2][j] > 0):
                        print(self.field[i//2][j], end = "")
                    for k in range (razr_width // 2):
                        print(" ", end = "")
                print("|")
                
    def reveal_bombs(self, width, height, bombs):
        for i in range (height):
            for j in range (width):
                if (bombs.bombs[i][j] == 1):
                    self.field[i][j] = -3;

class Links:
    def __init__ (self, width, height, bombs):
        self.links = [0]*height
        for i in range (height):
            self.links[i] = [0]*width
            for j in range (width):
                bombs_check = 0;
                x = i + 1;
                y = j + 1;
                if (x > 1):
                    bombs_check += bombs.bombs[x - 2][y - 1]
                if (y > 1):
                    bombs_check += bombs.bombs[x - 1][y - 2]
                if (x < height):
                    bombs_check += bombs.bombs[x][y - 1]
                if (y < width):
                    bombs_check += bombs.bombs[x - 1][y]
                if (x > 1 and y > 1):
                    bombs_check += bombs.bombs[x - 2][y - 2]
                if (x > 1 and y < width):
                    bombs_check += bombs.bombs[x - 2][y]
                if (x < height and y < width):
                    bombs_check += bombs.bombs[x][y]
                if (x < height and y > 1):
                    bombs_check += bombs.bombs[x][y - 2]
                self.links[i][j] = bombs_check

class Game:
    def try_resume_last_game(self):
        self.processing = 1
        self.win = 0
        self.bombs_amount = [0]
        self.flags = 0
        self.filename = "сапер.txt"
        try:
            fin = open(self.filename, 'r')
            unit = fin.read(1)
            fin.close();
            if (unit != ""):
                print("Хотите продолжить старую игру? y/n ", end = "")
                answer = str(input())
                while (answer != "y" and answer != "n"):
                    print("Ваш ответ не распознан. y/n ")
                    answer = str(input())
                if (answer == "y"):
                    return True
                else:
                    return False
        except FileNotFoundError:
            return False

    def read_file(self):
        fin = open(self.filename, 'r')
        file = fin.read().split(" ")
        self.height, self.width, self.bombs_amount[0] = list(map(int, [file[0], file[1], file[2]]))
        index = 3
        self.height = ~self.height;
        self.width = ~self.width;
        self.bombs_amount[0] = ~self.bombs_amount[0];
        self.initial_bombs = self.bombs_amount[0]
        arr_field = [0]*(self.height * self.width)
        for i in range (self.height*self.width):
            arr_field[i] = int(file[index])
            index += 1
            arr_field[i] = ~arr_field[i];
        self.field = Field(self.width, self.height, arr_field);
        arr_bombs = [0]*(self.bombs_amount[0] * 2)
        for i in range (2*self.bombs_amount[0]):
            arr_bombs[i] = int(file[index])
            index += 1
            arr_bombs[i] = ~arr_bombs[i];
        self.bombs = Bombs(self.width, self.height, self.bombs_amount, arr_bombs);
        self.flags = int(file[index])
        self.flags = ~self.flags
        fin.close()
        self.links = Links(self.width, self.height, self.bombs)

    def input_game_parameters(self):
        fin = open(self.filename, 'w')
        fin.close()
        success = 0
        while (not success):
            try:
                print("Введите ширину поля: ", end = "")
                width = int(input())
                if (width <= 0):
                    print("Пожалуйста, введите ширину >0.")
                    continue;
                else:
                    success = 1
            except ValueError:
                print("Некорректная ширина! Попробуйте снова.")
                
        success = 0
        while (not success):
            try:
                print("Введите высоту поля: ", end = "")
                height = int(input())
                if (height <= 0):
                    print("Пожалуйста, введите высоту > 0.")
                    continue
                else:
                    success = 1
            except ValueError:
                print("Некорректная высота! Попробуйте снова.")
        self.width = width
        self.height = height
        self.bombs = Bombs(self.width, self.height, self.bombs_amount, [])
        self.field = Field(self.width, self.height, [])
        self.links = Links(self.width, self.height, self.bombs)
        self.initial_bombs = self.bombs_amount[0]

    def write_in_the_file(self):
        fout = open(self.filename, 'w')
        fout.write(str(~self.height) + " " + str(~self.width) + " " + str(~self.bombs_amount[0]) + " ")
        for i in range (self.height):
            for j in range (self.width):
                fout.write(str(~self.field.field[i][j]) + " ")

        for i in range (self.height):
            for j in range (self.width):
                if (self.bombs.bombs[i][j] == 1):
                    fout.write(str(~i) + " " + str(~j) + " ")

        fout.write(str(~self.flags))
        fout.close()

    def turn_input(self, x, y, Action):
        success = 0;
        while (success != 4):
            success = 0;
            print("строка, столбец, действие (Flag или Open): ", end = "")
            try:
                X, Y , Action[0] = input().split(" ")
                success += 1
            except ValueError:
                print("Пожалуйста, пишите в одну строку.")
                continue
            try:
                x[0] = int(X)
                if (x[0] < 1 or x[0] > self.height):
                    print("Некорректная строка! Попробуйте снова.")
                    continue
                else:
                    success += 1
            except ValueError:
                print("Некорректная строка! Попробуйте снова.")
                continue
            try:
                y[0] = int(Y)
                if (y[0] < 1 or y[0] > self.width):
                    print("Некорректный столбец! Попробуйте снова.")
                    continue
                else:
                    success += 1
            except ValueError:
                print("Некорректный столбец! Попробуйте снова.")
                continue
            if (Action[0] != "Flag" and Action[0] != "Open"):
                print("Некорректное действие! Попробуйте снова.")
                continue
            else:
                success += 1

    def player_turn(self):
        arr_x = [0]
        arr_y = [0]
        arr_Action = ["0"]
        self.turn_input(arr_x, arr_y, arr_Action)
        x = arr_x[0]
        y = arr_y[0]
        Action = arr_Action[0]
        if (Action == "Open"):
            if (self.bombs.bombs[x - 1][y - 1]):
                self.processing = 0
            if (self.field.field[x - 1][y - 1] > -1):
                print("Эта клетка уже открыта.", "\n")
            if (self.processing):
                self.open(x - 1, y - 1)
                if (self.field.field[x - 1][y - 1] == -2):
                    self.flags -= 1
        else:
            self.flags += 1
            if (self.field.field[x - 1][y - 1] == -2):
                print("Клетка уже помечена флагом.", "\n")
            if (self.field.field[x - 1][y - 1] == -1):
                self.field.field[x - 1][y - 1] = -2
                if (self.bombs.bombs[x - 1][y - 1] == 1):
                    self.bombs_amount[0] -= 1
            if (self.field.field[x - 1][y - 1] >= 0):
                print("Нельзя пометить флагом открытую клетку.", "\n")

    def open(self, line_tag, column_tag):
        height = self.height
        width = self.width
        if (self.links.links[line_tag][column_tag] != 0 and self.field.field[line_tag][column_tag] < 0):
            self.field.field[line_tag][column_tag] = self.links.links[line_tag][column_tag]
        else:
            self.field.field[line_tag][column_tag] = self.links.links[line_tag][column_tag]
            if (line_tag > 0 and self.field.field[line_tag - 1][column_tag] == -1):
                self.open(line_tag - 1, column_tag)
            if (line_tag < height - 1 and self.field.field[line_tag + 1][column_tag] == -1):
                self.open(line_tag + 1, column_tag)
            if (column_tag > 0 and self.field.field[line_tag][column_tag - 1] == -1):
                self.open(line_tag, column_tag - 1)
            if (column_tag < width - 1 and self.field.field[line_tag][column_tag + 1] == -1):
                self.open(line_tag, column_tag + 1)
            if (line_tag > 0 and column_tag > 0 and self.field.field[line_tag - 1][column_tag - 1] == -1):
                self.open(line_tag - 1, column_tag - 1)
            if (line_tag > 0 and column_tag < width - 1 and self.field.field[line_tag - 1][column_tag + 1] == -1):
                self.open(line_tag - 1, column_tag + 1)
            if (line_tag < height - 1 and column_tag < width - 1 and self.field.field[line_tag + 1][column_tag + 1] == -1):
                self.open(line_tag + 1, column_tag + 1)
            if (line_tag < height - 1 and column_tag > 0 and self.field.field[line_tag + 1][column_tag - 1] == -1):
                self.open(line_tag + 1, column_tag - 1)

    def process(self):
        self.win = 0;
        initial_bombs = self.bombs_amount[0]
        while (self.processing):
            self.write_in_the_file()
            print("Осталось бомб: ", self.bombs_amount[0], sep = "")
            print("Ваш ход:")
            self.player_turn()
            if (self.bombs_amount[0] == 0 and self.flags == initial_bombs):
                self.processing = 0
                self.win = 1
            if (self.processing):
                self.field.show(self.width, self.height)
        fin = open(self.filename, 'w')
        fin.close()

    def ending(self):
        if (self.win):
            self.field.show(self.width, self.height)
            print("Вы выиграли!")
        else :
            self.field.reveal_bombs(self.width, self.height, self.bombs)
            self.field.show(self.width, self.height)
            print("Игра окончена!")
            
    def __init__ (self):
        old_game = self.try_resume_last_game()
        if (old_game):
            self.read_file()
        else:
            self.input_game_parameters()
        self.field.show(self.width, self.height)
        self.process()
        self.ending()

game = Game()
a = input()
