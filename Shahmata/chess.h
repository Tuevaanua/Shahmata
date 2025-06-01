#ifndef CHESS_H
#define CHESS_H

#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <fstream>

// ���� ����� (�����/������)
enum class Color { WHITE, BLACK };

// ��������� ��� ������������� ������� �� ��������� �����
struct Position {
    int x; // ����������� (0-7, ������������� a-h)
    int y; // ��������� (0-7, ������������� 1-8)

    Position(int x = -1, int y = -1) : x(x), y(y) {}
    bool operator==(const Position& other) const { return x == other.x && y == other.y; }
    bool isValid() const { return x >= 0 && x < 8 && y >= 0 && y < 8; }
    std::string toString() const {
        return std::string(1, 'a' + x) + std::to_string(y + 1);
    }
};

// ������� ����� ��� ���� ��������� �����
class Piece {
protected:
    Color color;    // ���� ������
    Position position; // ������� �������
    char symbol;    // ������ ��� ����������� (��������, 'P' ��� ����� �����)

public:
    Piece(Color color, Position position, char symbol)
        : color(color), position(position), symbol(symbol) {
    }
    virtual ~Piece() = default;

    // ������� � �������
    Color getColor() const { return color; }
    Position getPosition() const { return position; }
    void setPosition(Position pos) { position = pos; }
    char getSymbol() const { return symbol; }

    // ����������� ������, ������� ������ ���� ����������� � ����������� �������
    virtual bool isValidMove(Position newPos, const std::vector<std::unique_ptr<Piece>>& pieces) const = 0;
    virtual std::unique_ptr<Piece> clone() const = 0;

    // ����� ������ ��� ���� �����
    bool isPathClear(Position newPos, const std::vector<std::unique_ptr<Piece>>& pieces) const;
    Piece* getPieceAt(Position pos, const std::vector<std::unique_ptr<Piece>>& pieces) const;
};

// ������ ��� ���������� ����� (����������� �� Piece)
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

// �����, �������������� ��������� ����� � ������� ������
class ChessBoard {
private:
    std::vector<std::unique_ptr<Piece>> pieces; // ��� ������ �� �����
    Color currentTurn; // ��� ������ ���
    bool gameOver; // ���� ��������� ����

    // ��������������� ������
    void initializePieces(); // ������������� ��������� ����������� �����
    Piece* getPieceAt(Position pos) const; // �������� ������ �� �������
    bool isCheck(Color kingColor) const; // ��������, ��������� �� ������ ��� �����
    bool isCheckmate(Color kingColor); // �������� �� ���
    bool isPositionUnderAttack(Position pos, Color attackingColor) const; // ��� ������ �� �������
    Position getKingPosition(Color color) const; // �������� ������� ������

public:
    ChessBoard();

    // �������� ������ ��� ���������� �����
    bool movePiece(Position from, Position to); // ������� ���
    void printBoard() const; // ���������� �����
    bool isGameOver() const { return gameOver; } // ���������, �������� �� ����
    Color getCurrentTurn() const { return currentTurn; } // ��� ������ ���

    // ������ ��� ����������/�������� ����
    bool saveGame(const std::string& filename) const;
    bool loadGame(const std::string& filename);
};

#endif // CHESS_H