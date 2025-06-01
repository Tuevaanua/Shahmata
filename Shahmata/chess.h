#ifndef CHESS_H
#define CHESS_H

#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <fstream>

// Цвет фигур (белые/черные)
enum class Color { WHITE, BLACK };

// Структура для представления позиции на шахматной доске
struct Position {
    int x; // Горизонталь (0-7, соответствует a-h)
    int y; // Вертикаль (0-7, соответствует 1-8)

    Position(int x = -1, int y = -1) : x(x), y(y) {}
    bool operator==(const Position& other) const { return x == other.x && y == other.y; }
    bool isValid() const { return x >= 0 && x < 8 && y >= 0 && y < 8; }
    std::string toString() const {
        return std::string(1, 'a' + x) + std::to_string(y + 1);
    }
};

// Базовый класс для всех шахматных фигур
class Piece {
protected:
    Color color;    // Цвет фигуры
    Position position; // Текущая позиция
    char symbol;    // Символ для отображения (например, 'P' для белой пешки)

public:
    Piece(Color color, Position position, char symbol)
        : color(color), position(position), symbol(symbol) {
    }
    virtual ~Piece() = default;

    // Геттеры и сеттеры
    Color getColor() const { return color; }
    Position getPosition() const { return position; }
    void setPosition(Position pos) { position = pos; }
    char getSymbol() const { return symbol; }

    // Виртуальные методы, которые должны быть реализованы в производных классах
    virtual bool isValidMove(Position newPos, const std::vector<std::unique_ptr<Piece>>& pieces) const = 0;
    virtual std::unique_ptr<Piece> clone() const = 0;

    // Общие методы для всех фигур
    bool isPathClear(Position newPos, const std::vector<std::unique_ptr<Piece>>& pieces) const;
    Piece* getPieceAt(Position pos, const std::vector<std::unique_ptr<Piece>>& pieces) const;
};

// Классы для конкретных фигур (наследуются от Piece)
class Pawn : public Piece {
public:
    Pawn(Color color, Position position);
    bool isValidMove(Position newPos, const std::vector<std::unique_ptr<Piece>>& pieces) const override;
    std::unique_ptr<Piece> clone() const override;
};

class Rook : public Piece {
public:
    Rook(Color color, Position position);
    bool isValidMove(Position newPos, const std::vector<std::unique_ptr<Piece>>& pieces) const override;
    std::unique_ptr<Piece> clone() const override;
};

class Knight : public Piece {
public:
    Knight(Color color, Position position);
    bool isValidMove(Position newPos, const std::vector<std::unique_ptr<Piece>>& pieces) const override;
    std::unique_ptr<Piece> clone() const override;
};

class Bishop : public Piece {
public:
    Bishop(Color color, Position position);
    bool isValidMove(Position newPos, const std::vector<std::unique_ptr<Piece>>& pieces) const override;
    std::unique_ptr<Piece> clone() const override;
};

class Queen : public Piece {
public:
    Queen(Color color, Position position);
    bool isValidMove(Position newPos, const std::vector<std::unique_ptr<Piece>>& pieces) const override;
    std::unique_ptr<Piece> clone() const override;
};

class King : public Piece {
public:
    King(Color color, Position position);
    bool isValidMove(Position newPos, const std::vector<std::unique_ptr<Piece>>& pieces) const override;
    std::unique_ptr<Piece> clone() const override;
};

// Класс, представляющий шахматную доску и игровую логику
class ChessBoard {
private:
    std::vector<std::unique_ptr<Piece>> pieces; // Все фигуры на доске
    Color currentTurn; // Чей сейчас ход
    bool gameOver; // Флаг окончания игры

    // Вспомогательные методы
    void initializePieces(); // Инициализация начальной расстановки фигур
    Piece* getPieceAt(Position pos) const; // Получить фигуру по позиции
    bool isCheck(Color kingColor) const; // Проверка, находится ли король под шахом
    bool isCheckmate(Color kingColor); // Проверка на мат
    bool isPositionUnderAttack(Position pos, Color attackingColor) const; // Под атакой ли позиция
    Position getKingPosition(Color color) const; // Получить позицию короля

public:
    ChessBoard();

    // Основные методы для управления игрой
    bool movePiece(Position from, Position to); // Сделать ход
    void printBoard() const; // Отобразить доску
    bool isGameOver() const { return gameOver; } // Проверить, окончена ли игра
    Color getCurrentTurn() const { return currentTurn; } // Чей сейчас ход

    // Методы для сохранения/загрузки игры
    bool saveGame(const std::string& filename) const;
    bool loadGame(const std::string& filename);
};

#endif // CHESS_H