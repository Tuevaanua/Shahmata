#include "game.h"
#include <iostream>
#include <sstream>
#include <cctype>

// �������������� ������ (��������, "e2") � ������� �� �����
Position ChessGame::parsePosition(const std::string& input) const {
    if (input.length() != 2) return Position(-1, -1); // ������������ ������

    char file = tolower(input[0]); // ����� (a-h)
    char rank = input[1]; // ����� (1-8)

    // �������� ������������ ���������
    if (file < 'a' || file > 'h' || rank < '1' || rank > '8') {
        return Position(-1, -1);
    }

    // �������������� � ���������� ����� (0-7)
    return Position(file - 'a', rank - '1');
}

// ����� ������ ��������� ������
void ChessGame::printHelp() const {
    std::cout << "�������\n"
        << "move <������> <����> - ����������� ������ (������: move e2 e4)\n"
        << "save <��� �����>     - ���������� ����\n"
        << "load <��� �����>     - �������� ����������\n"
        << "help                 - �������� �������\n"
        << "exit                 - ����� �� ����\n";
}

// �������� ������� ����
void ChessGame::run() {
    std::cout << "����� ���������� � �������!\n";
    printHelp();

    std::string input;
    while (!board.isGameOver()) {
        board.printBoard();
        std::cout << (board.getCurrentTurn() == Color::WHITE ? "White" : "Black") << "'s ���: ";
        std::getline(std::cin, input);

        std::istringstream iss(input);
        std::string command;
        iss >> command;

        if (command == "move") {
            std::string fromStr, toStr;
            iss >> fromStr >> toStr;

            // ����������� ��������� �������
            Position from = parsePosition(fromStr);
            Position to = parsePosition(toStr);

            if (!from.isValid() || !to.isValid()) {
                std::cout << "������������ ����. ����������� ������: e2 e4\n";
                continue;
            }

            // �������� ��������� ���
            if (!board.movePiece(from, to)) {
                std::cout << "������������ ���\n";
            }
        }
        else if (command == "save") {
            std::string filename;
            iss >> filename;
            saveGame(filename);
            std::cout << "���� ��������� � " << filename << "\n";
        }
        else if (command == "load") {
            std::string filename;
            iss >> filename;
            if (loadGame(filename)) {
                std::cout << "���� ��������� �� " << filename << "\n";
            }
            else {
                std::cout << "�� ������� ��������� ���� �� " << filename << "\n";
            }
        }
        else if (command == "help") {
            printHelp();
        }
        else if (command == "exit") {
            break;
        }
        else {
            std::cout << "����������� �������. ������� 'help' ��� �������.\n";
        }
    }

    // ���� ���� ����������� (���), ������� ��������� ���������
    if (board.isGameOver()) {
        board.printBoard();
        std::cout << "���� ��������\n";
    }
}

// ���������� ���� � ����
void ChessGame::saveGame(const std::string& filename) const {
    board.saveGame(filename);
}

// �������� ���� �� �����
bool ChessGame::loadGame(const std::string& filename) {
    return board.loadGame(filename);
}