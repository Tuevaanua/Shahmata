#include "game.h"

int main() {
    // Устанавливаем русскую локаль для корректного вывода сообщений
    setlocale(LC_ALL, "Russian");

    // Создаем и запускаем игру
    ChessGame game;
    game.run();

    return 0;
}