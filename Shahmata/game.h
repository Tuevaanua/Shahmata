#ifndef GAME_H
#define GAME_H

#include "chess.h"
#include <string>

// Класс для управления игровым процессом
class ChessGame {
private:
    ChessBoard board; // Шахматная доска

    // Вспомогательные методы
    Position parsePosition(const std::string& input) const; // Преобразование строки в позицию
    void printHelp() const; // Вывод справки по командам

public:
    void run(); // Основной игровой цикл
    void saveGame(const std::string& filename) const; // Сохранение игры
    bool loadGame(const std::string& filename);  // Загрузка игры
};

#endif // GAME_H