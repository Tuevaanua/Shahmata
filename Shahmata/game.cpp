#include "game.h"
#include <iostream>
#include <sstream>
#include <cctype>

// Преобразование строки (например, "e2") в позицию на доске
Position ChessGame::parsePosition(const std::string& input) const {
    if (input.length() != 2) return Position(-1, -1); // Неправильный формат

    char file = tolower(input[0]); // Буква (a-h)
    char rank = input[1]; // Цифра (1-8)

    // Проверка допустимости координат
    if (file < 'a' || file > 'h' || rank < '1' || rank > '8') {
        return Position(-1, -1);
    }

    // Преобразование в координаты доски (0-7)
    return Position(file - 'a', rank - '1');
}

// Вывод списка доступных команд
void ChessGame::printHelp() const {
    std::cout << "Команды\n"
        << "move <откуда> <куда> - Переместить фигуру (Пример: move e2 e4)\n"
        << "save <имя файла>     - Сохранение игры\n"
        << "load <имя файла>     - Загрузка сохранения\n"
        << "help                 - Показать справку\n"
        << "exit                 - Выход из игры\n";
}

// Основной игровой цикл
void ChessGame::run() {
    std::cout << "Добро пожаловать в шахматы!\n";
    printHelp();

    std::string input;
    while (!board.isGameOver()) {
        board.printBoard();
        std::cout << (board.getCurrentTurn() == Color::WHITE ? "White" : "Black") << "'s Ход: ";
        std::getline(std::cin, input);

        std::istringstream iss(input);
        std::string command;
        iss >> command;

        if (command == "move") {
            std::string fromStr, toStr;
            iss >> fromStr >> toStr;

            // Преобразуем введенные позиции
            Position from = parsePosition(fromStr);
            Position to = parsePosition(toStr);

            if (!from.isValid() || !to.isValid()) {
                std::cout << "Неправильный ввод. Используйте формат: e2 e4\n";
                continue;
            }

            // Пытаемся выполнить ход
            if (!board.movePiece(from, to)) {
                std::cout << "Недопустимый ход\n";
            }
        }
        else if (command == "save") {
            std::string filename;
            iss >> filename;
            saveGame(filename);
            std::cout << "Игра сохранена в " << filename << "\n";
        }
        else if (command == "load") {
            std::string filename;
            iss >> filename;
            if (loadGame(filename)) {
                std::cout << "Игра загружена из " << filename << "\n";
            }
            else {
                std::cout << "Не удалось загрузить игру из " << filename << "\n";
            }
        }
        else if (command == "help") {
            printHelp();
        }
        else if (command == "exit") {
            break;
        }
        else {
            std::cout << "Неизвестная команда. Введите 'help' для справки.\n";
        }
    }

    // Если игра закончилась (мат), выводим финальное сообщение
    if (board.isGameOver()) {
        board.printBoard();
        std::cout << "Игра окончена\n";
    }
}

// Сохранение игры в файл
void ChessGame::saveGame(const std::string& filename) const {
    board.saveGame(filename);
}

// Загрузка игры из файла
bool ChessGame::loadGame(const std::string& filename) {
    return board.loadGame(filename);
}