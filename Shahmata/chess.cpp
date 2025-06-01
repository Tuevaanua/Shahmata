#include "chess.h"
#include <cmath>
#include <algorithm>

// Проверка, свободен ли путь между текущей позицией и новой позицией
bool Piece::isPathClear(Position newPos, const std::vector<std::unique_ptr<Piece>>& pieces) const {
    // Вычисляем разницу по x и y
    int dx = newPos.x - position.x;
    int dy = newPos.y - position.y;

    // Определяем количество шагов (максимальное из dx и dy по модулю)
    int steps = std::max(std::abs(dx), std::abs(dy));

    // Определяем направление движения по x и y
    int xStep = dx == 0 ? 0 : (dx > 0 ? 1 : -1);
    int yStep = dy == 0 ? 0 : (dy > 0 ? 1 : -1);

    // Проверяем все промежуточные позиции
    for (int i = 1; i < steps; ++i) {
        Position intermediate(position.x + i * xStep, position.y + i * yStep);
        if (getPieceAt(intermediate, pieces) != nullptr) {
            return false; // Путь не свободен
        }
    }
    return true; // Путь свободен
}

// Получить фигуру в указанной позиции
Piece* Piece::getPieceAt(Position pos, const std::vector<std::unique_ptr<Piece>>& pieces) const {
    for (const auto& piece : pieces) {
        if (piece->getPosition() == pos) {
            return piece.get(); // Возвращаем найденную фигуру
        }
    }
    return nullptr; // Фигура не найдена
}

// Реализация методов для пешки
Pawn::Pawn(Color color, Position position) : Piece(color, position, color == Color::WHITE ? 'P' : 'p') {}

bool Pawn::isValidMove(Position newPos, const std::vector<std::unique_ptr<Piece>>& pieces) const {
    if (!newPos.isValid()) return false;

    // Направление движения пешки (вверх для белых, вниз для черных)
    int direction = (color == Color::WHITE) ? 1 : -1;
    // Стартовая линия для пешки (вторая для белых, седьмая для черных)
    int startRow = (color == Color::WHITE) ? 1 : 6;

    // Движение вперед
    if (newPos.x == position.x) {
        // Один шаг вперед
        if (newPos.y == position.y + direction && getPieceAt(newPos, pieces) == nullptr) {
            return true;
        }
        // Два шага вперед с начальной позиции
        if (position.y == startRow && newPos.y == position.y + 2 * direction &&
            getPieceAt(newPos, pieces) == nullptr &&
            getPieceAt(Position(position.x, position.y + direction), pieces) == nullptr) {
            return true;
        }
    }
    // Взятие фигуры по диагонали
    else if (abs(newPos.x - position.x) == 1 && newPos.y == position.y + direction) {
        Piece* target = getPieceAt(newPos, pieces);
        if (target != nullptr && target->getColor() != color) {
            return true;
        }
    }

    return false; // Недопустимый ход
}

std::unique_ptr<Piece> Pawn::clone() const {
    return std::make_unique<Pawn>(*this);
}

// Реализация методов для ладьи
Rook::Rook(Color color, Position position) : Piece(color, position, color == Color::WHITE ? 'R' : 'r') {}

bool Rook::isValidMove(Position newPos, const std::vector<std::unique_ptr<Piece>>& pieces) const {
    if (!newPos.isValid()) return false;

    // Ладья может двигаться только по прямой
    if (newPos.x != position.x && newPos.y != position.y) return false;

    // Проверяем, свободен ли путь
    if (!isPathClear(newPos, pieces)) return false;

    // Проверяем, можно ли взять фигуру в конечной позиции
    Piece* target = getPieceAt(newPos, pieces);
    return target == nullptr || target->getColor() != color;
}

std::unique_ptr<Piece> Rook::clone() const {
    return std::make_unique<Rook>(*this);
}

// Реализация методов для коня
Knight::Knight(Color color, Position position) : Piece(color, position, color == Color::WHITE ? 'N' : 'n') {}

bool Knight::isValidMove(Position newPos, const std::vector<std::unique_ptr<Piece>>& pieces) const {
    if (!newPos.isValid()) return false;

    // Вычисляем разницу по x и y
    int dx = abs(newPos.x - position.x);
    int dy = abs(newPos.y - position.y);

    // Конь ходит буквой "Г" - 2 в одну сторону и 1 в другую
    if (!((dx == 1 && dy == 2) || (dx == 2 && dy == 1))) return false;

    // Проверяем, можно ли взять фигуру в конечной позиции
    Piece* target = getPieceAt(newPos, pieces);
    return target == nullptr || target->getColor() != color;
}

std::unique_ptr<Piece> Knight::clone() const {
    return std::make_unique<Knight>(*this);
}

// Реализация методов для слона
Bishop::Bishop(Color color, Position position) : Piece(color, position, color == Color::WHITE ? 'B' : 'b') {}

bool Bishop::isValidMove(Position newPos, const std::vector<std::unique_ptr<Piece>>& pieces) const {
    if (!newPos.isValid()) return false;

    // Слон ходит только по диагонали (разница по x и y должна быть одинаковой)
    if (abs(newPos.x - position.x) != abs(newPos.y - position.y)) return false;

    // Проверяем, свободен ли путь
    if (!isPathClear(newPos, pieces)) return false;

    // Проверяем, можно ли взять фигуру в конечной позиции
    Piece* target = getPieceAt(newPos, pieces);
    return target == nullptr || target->getColor() != color;
}

std::unique_ptr<Piece> Bishop::clone() const {
    return std::make_unique<Bishop>(*this);
}

// Реализация методов для ферзя
Queen::Queen(Color color, Position position) : Piece(color, position, color == Color::WHITE ? 'Q' : 'q') {}

bool Queen::isValidMove(Position newPos, const std::vector<std::unique_ptr<Piece>>& pieces) const {
    if (!newPos.isValid()) return false;

    // Ферзь ходит как ладья или слон (по прямой или по диагонали)
    if (newPos.x != position.x && newPos.y != position.y &&
        abs(newPos.x - position.x) != abs(newPos.y - position.y)) {
        return false;
    }

    // Проверяем, свободен ли путь
    if (!isPathClear(newPos, pieces)) return false;

    // Проверяем, можно ли взять фигуру в конечной позиции
    Piece* target = getPieceAt(newPos, pieces);
    return target == nullptr || target->getColor() != color;
}

std::unique_ptr<Piece> Queen::clone() const {
    return std::make_unique<Queen>(*this);
}

// Реализация методов для короля
King::King(Color color, Position position) : Piece(color, position, color == Color::WHITE ? 'K' : 'k') {}

bool King::isValidMove(Position newPos, const std::vector<std::unique_ptr<Piece>>& pieces) const {
    if (!newPos.isValid()) return false;

    // Король может ходить только на одну клетку в любом направлении
    if (abs(newPos.x - position.x) > 1 || abs(newPos.y - position.y) > 1) return false;

    // Проверяем, можно ли взять фигуру в конечной позиции
    Piece* target = getPieceAt(newPos, pieces);
    return target == nullptr || target->getColor() != color;
}

std::unique_ptr<Piece> King::clone() const {
    return std::make_unique<King>(*this);
}

// Инициализация шахматной доски
ChessBoard::ChessBoard() : currentTurn(Color::WHITE), gameOver(false) {
    initializePieces();
}

// Начальная расстановка фигур
void ChessBoard::initializePieces() {
    // Белые фигуры
    pieces.push_back(std::make_unique<Rook>(Color::WHITE, Position(0, 0)));
    pieces.push_back(std::make_unique<Knight>(Color::WHITE, Position(1, 0)));
    pieces.push_back(std::make_unique<Bishop>(Color::WHITE, Position(2, 0)));
    pieces.push_back(std::make_unique<Queen>(Color::WHITE, Position(3, 0)));
    pieces.push_back(std::make_unique<King>(Color::WHITE, Position(4, 0)));
    pieces.push_back(std::make_unique<Bishop>(Color::WHITE, Position(5, 0)));
    pieces.push_back(std::make_unique<Knight>(Color::WHITE, Position(6, 0)));
    pieces.push_back(std::make_unique<Rook>(Color::WHITE, Position(7, 0)));
    // Белые пешки
    for (int i = 0; i < 8; ++i) {
        pieces.push_back(std::make_unique<Pawn>(Color::WHITE, Position(i, 1)));
    }

    // Черные фигуры
    pieces.push_back(std::make_unique<Rook>(Color::BLACK, Position(0, 7)));
    pieces.push_back(std::make_unique<Knight>(Color::BLACK, Position(1, 7)));
    pieces.push_back(std::make_unique<Bishop>(Color::BLACK, Position(2, 7)));
    pieces.push_back(std::make_unique<Queen>(Color::BLACK, Position(3, 7)));
    pieces.push_back(std::make_unique<King>(Color::BLACK, Position(4, 7)));
    pieces.push_back(std::make_unique<Bishop>(Color::BLACK, Position(5, 7)));
    pieces.push_back(std::make_unique<Knight>(Color::BLACK, Position(6, 7)));
    pieces.push_back(std::make_unique<Rook>(Color::BLACK, Position(7, 7)));
    // Черные пешки
    for (int i = 0; i < 8; ++i) {
        pieces.push_back(std::make_unique<Pawn>(Color::BLACK, Position(i, 6)));
    }
}

// Получить фигуру по позиции
Piece* ChessBoard::getPieceAt(Position pos) const {
    for (const auto& piece : pieces) {
        if (piece->getPosition() == pos) {
            return piece.get();
        }
    }
    return nullptr;
}

// Проверка, находится ли король под шахом
bool ChessBoard::isCheck(Color kingColor) const {
    // Находим позицию короля
    Position kingPos = getKingPosition(kingColor);
    if (!kingPos.isValid()) return false;

    // Проверяем, может ли какая-либо фигура противника атаковать короля
    for (const auto& piece : pieces) {
        if (piece->getColor() != kingColor && piece->isValidMove(kingPos, pieces)) {
            return true; // Шах
        }
    }
    return false; // Нет шаха
}

// Проверка на мат
bool ChessBoard::isCheckmate(Color kingColor) {
    // Если нет шаха, то и мата быть не может
    if (!isCheck(kingColor)) return false;

    Position kingPos = getKingPosition(kingColor);
    if (!kingPos.isValid()) return true; // Король не найден (технически это мат)

    // Проверяем все возможные ходы короля
    for (int dx = -1; dx <= 1; ++dx) {
        for (int dy = -1; dy <= 1; ++dy) {
            if (dx == 0 && dy == 0) continue; // Пропускаем текущую позицию

            Position newPos(kingPos.x + dx, kingPos.y + dy);
            if (!newPos.isValid()) continue; // Пропускаем недопустимые позиции

            // Пропускаем позиции, занятые своими фигурами
            Piece* target = getPieceAt(newPos);
            if (target && target->getColor() == kingColor) continue;

            // Временно перемещаем короля и проверяем, останется ли он под шахом
            auto kingPiece = getPieceAt(kingPos);
            auto temp = kingPiece->clone();
            kingPiece->setPosition(newPos);

            bool stillInCheck = isCheck(kingColor);

            // Возвращаем короля на место
            kingPiece->setPosition(kingPos);

            if (!stillInCheck) {
                return false; // Есть хотя бы один ход, убирающий шах - не мат
            }
        }
    }

    return true; // Нет допустимых ходов, убирающих шах - мат
}

// Проверка, находится ли позиция под атакой фигур указанного цвета
bool ChessBoard::isPositionUnderAttack(Position pos, Color attackingColor) const {
    for (const auto& piece : pieces) {
        if (piece->getColor() == attackingColor && piece->isValidMove(pos, pieces)) {
            return true;
        }
    }
    return false;
}

// Получить позицию короля указанного цвета
Position ChessBoard::getKingPosition(Color color) const {
    for (const auto& piece : pieces) {
        if (piece->getSymbol() == (color == Color::WHITE ? 'K' : 'k')) {
            return piece->getPosition();
        }
    }
    return Position(-1, -1); // Король не найден
}

// Основной метод для выполнения хода
bool ChessBoard::movePiece(Position from, Position to) {
    if (gameOver) return false; // Игра уже окончена

    // Проверяем, есть ли фигура в начальной позиции и принадлежит ли она текущему игроку
    Piece* piece = getPieceAt(from);
    if (!piece || piece->getColor() != currentTurn) return false;

    // Проверяем, допустим ли ход для этой фигуры
    if (!piece->isValidMove(to, pieces)) return false;

    // Создаем временную копию фигуры для проверки
    auto temp = piece->clone();
    Piece* capturedPiece = getPieceAt(to);

    // Временно выполняем ход
    piece->setPosition(to);
    bool inCheck = isCheck(currentTurn);

    // Отменяем временный ход
    piece->setPosition(from);

    // Если ход ставит короля под шах, он недопустим
    if (inCheck) {
        std::cout << "Ход поставит короля под шах" << std::endl;
        return false;
    }

    // Выполняем ход окончательно
    piece->setPosition(to);

    // Удаляем съеденную фигуру, если она есть
    if (capturedPiece) {
        pieces.erase(
            std::remove_if(pieces.begin(), pieces.end(),
                [to](const std::unique_ptr<Piece>& p) {
                    return p->getPosition() == to;
                }),
            pieces.end()
        );
    }

    // Проверяем, не поставили ли мы мат противнику
    Color opponentColor = currentTurn == Color::WHITE ? Color::BLACK : Color::WHITE;
    if (isCheckmate(opponentColor)) {
        gameOver = true;
        std::cout << (currentTurn == Color::WHITE ? "White" : "Black") << " МАТ " << std::endl;
    }
    else if (isCheck(opponentColor)) {
        std::cout << (opponentColor == Color::WHITE ? "White" : "Black") << " ШАХ " << std::endl;
    }

    // Передаем ход другому игроку
    currentTurn = opponentColor;
    return true;
}

// Отображение шахматной доски
void ChessBoard::printBoard() const {
    std::cout << "  a b c d e f g h" << std::endl;
    for (int y = 7; y >= 0; --y) {
        std::cout << y + 1 << " ";
        for (int x = 0; x < 8; ++x) {
            Piece* piece = getPieceAt(Position(x, y));
            if (piece) {
                std::cout << piece->getSymbol() << " ";
            }
            else {
                std::cout << ". ";
            }
        }
        std::cout << y + 1 << std::endl;
    }
    std::cout << "  a b c d e f g h" << std::endl;
}

// Сохранение игры в файл
bool ChessBoard::saveGame(const std::string& filename) const {
    std::ofstream out(filename);
    if (!out) return false;

    // Записываем, чей сейчас ход
    out << (currentTurn == Color::WHITE ? "white" : "black") << "\n";

    // Записываем все фигуры и их позиции
    for (const auto& piece : pieces) {
        out << piece->getSymbol() << " "
            << piece->getPosition().x << " "
            << piece->getPosition().y << "\n";
    }

    return true;
}

// Загрузка игры из файла
bool ChessBoard::loadGame(const std::string& filename) {
    std::ifstream in(filename);
    if (!in) return false;

    // Очищаем текущие фигуры
    pieces.clear();

    // Читаем, чей ход
    std::string turn;
    in >> turn;
    currentTurn = (turn == "white") ? Color::WHITE : Color::BLACK;

    // Читаем фигуры и их позиции
    char symbol;
    int x, y;
    while (in >> symbol >> x >> y) {
        Color color = isupper(symbol) ? Color::WHITE : Color::BLACK;
        Position pos(x, y);

        // Создаем фигуры в соответствии с прочитанными данными
        switch (tolower(symbol)) {
        case 'p': pieces.push_back(std::make_unique<Pawn>(color, pos)); break;
        case 'r': pieces.push_back(std::make_unique<Rook>(color, pos)); break;
        case 'n': pieces.push_back(std::make_unique<Knight>(color, pos)); break;
        case 'b': pieces.push_back(std::make_unique<Bishop>(color, pos)); break;
        case 'q': pieces.push_back(std::make_unique<Queen>(color, pos)); break;
        case 'k': pieces.push_back(std::make_unique<King>(color, pos)); break;
        default: break;
        }
    }

    gameOver = false;
    return true;
}