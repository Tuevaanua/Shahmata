#ifndef GAME_H
#define GAME_H

#include "chess.h"
#include <string>

// ����� ��� ���������� ������� ���������
class ChessGame {
private:
    ChessBoard board; // ��������� �����

    // ��������������� ������
    Position parsePosition(const std::string& input) const; // �������������� ������ � �������
    void printHelp() const; // ����� ������� �� ��������

public:
    void run(); // �������� ������� ����
    void saveGame(const std::string& filename) const; // ���������� ����
    bool loadGame(const std::string& filename);  // �������� ����
};

#endif // GAME_H