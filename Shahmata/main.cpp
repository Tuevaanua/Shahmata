#include "game.h"

int main() {
    // ������������� ������� ������ ��� ����������� ������ ���������
    setlocale(LC_ALL, "Russian");

    // ������� � ��������� ����
    ChessGame game;
    game.run();

    return 0;
}