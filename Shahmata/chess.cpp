#include "chess.h"
#include <cmath>
#include <algorithm>

// ��������, �������� �� ���� ����� ������� �������� � ����� ��������
bool Piece::isPathClear(Position newPos, const std::vector<std::unique_ptr<Piece>>& pieces) const {
    // ��������� ������� �� x � y
    int dx = newPos.x - position.x;
    int dy = newPos.y - position.y;

    // ���������� ���������� ����� (������������ �� dx � dy �� ������)
    int steps = std::max(std::abs(dx), std::abs(dy));

    // ���������� ����������� �������� �� x � y
    int xStep = dx == 0 ? 0 : (dx > 0 ? 1 : -1);
    int yStep = dy == 0 ? 0 : (dy > 0 ? 1 : -1);

    // ��������� ��� ������������� �������
    for (int i = 1; i < steps; ++i) {
        Position intermediate(position.x + i * xStep, position.y + i * yStep);
        if (getPieceAt(intermediate, pieces) != nullptr) {
            return false; // ���� �� ��������
        }
    }
    return true; // ���� ��������
}

// �������� ������ � ��������� �������
Piece* Piece::getPieceAt(Position pos, const std::vector<std::unique_ptr<Piece>>& pieces) const {
    for (const auto& piece : pieces) {
        if (piece->getPosition() == pos) {
            return piece.get(); // ���������� ��������� ������
        }
    }
    return nullptr; // ������ �� �������
}

// ���������� ������� ��� �����
Pawn::Pawn(Color color, Position position) : Piece(color, position, color == Color::WHITE ? 'P' : 'p') {}

bool Pawn::isValidMove(Position newPos, const std::vector<std::unique_ptr<Piece>>& pieces) const {
    if (!newPos.isValid()) return false;

    // ����������� �������� ����� (����� ��� �����, ���� ��� ������)
    int direction = (color == Color::WHITE) ? 1 : -1;
    // ��������� ����� ��� ����� (������ ��� �����, ������� ��� ������)
    int startRow = (color == Color::WHITE) ? 1 : 6;

    // �������� ������
    if (newPos.x == position.x) {
        // ���� ��� ������
        if (newPos.y == position.y + direction && getPieceAt(newPos, pieces) == nullptr) {
            return true;
        }
        // ��� ���� ������ � ��������� �������
        if (position.y == startRow && newPos.y == position.y + 2 * direction &&
            getPieceAt(newPos, pieces) == nullptr &&
            getPieceAt(Position(position.x, position.y + direction), pieces) == nullptr) {
            return true;
        }
    }
    // ������ ������ �� ���������
    else if (abs(newPos.x - position.x) == 1 && newPos.y == position.y + direction) {
        Piece* target = getPieceAt(newPos, pieces);
        if (target != nullptr && target->getColor() != color) {
            return true;
        }
    }

    return false; // ������������ ���
}

std::unique_ptr<Piece> Pawn::clone() const {
    return std::make_unique<Pawn>(*this);
}

// ���������� ������� ��� �����
Rook::Rook(Color color, Position position) : Piece(color, position, color == Color::WHITE ? 'R' : 'r') {}

bool Rook::isValidMove(Position newPos, const std::vector<std::unique_ptr<Piece>>& pieces) const {
    if (!newPos.isValid()) return false;

    // ����� ����� ��������� ������ �� ������
    if (newPos.x != position.x && newPos.y != position.y) return false;

    // ���������, �������� �� ����
    if (!isPathClear(newPos, pieces)) return false;

    // ���������, ����� �� ����� ������ � �������� �������
    Piece* target = getPieceAt(newPos, pieces);
    return target == nullptr || target->getColor() != color;
}

std::unique_ptr<Piece> Rook::clone() const {
    return std::make_unique<Rook>(*this);
}

// ���������� ������� ��� ����
Knight::Knight(Color color, Position position) : Piece(color, position, color == Color::WHITE ? 'N' : 'n') {}

bool Knight::isValidMove(Position newPos, const std::vector<std::unique_ptr<Piece>>& pieces) const {
    if (!newPos.isValid()) return false;

    // ��������� ������� �� x � y
    int dx = abs(newPos.x - position.x);
    int dy = abs(newPos.y - position.y);

    // ���� ����� ������ "�" - 2 � ���� ������� � 1 � ������
    if (!((dx == 1 && dy == 2) || (dx == 2 && dy == 1))) return false;

    // ���������, ����� �� ����� ������ � �������� �������
    Piece* target = getPieceAt(newPos, pieces);
    return target == nullptr || target->getColor() != color;
}

std::unique_ptr<Piece> Knight::clone() const {
    return std::make_unique<Knight>(*this);
}

// ���������� ������� ��� �����
Bishop::Bishop(Color color, Position position) : Piece(color, position, color == Color::WHITE ? 'B' : 'b') {}

bool Bishop::isValidMove(Position newPos, const std::vector<std::unique_ptr<Piece>>& pieces) const {
    if (!newPos.isValid()) return false;

    // ���� ����� ������ �� ��������� (������� �� x � y ������ ���� ����������)
    if (abs(newPos.x - position.x) != abs(newPos.y - position.y)) return false;

    // ���������, �������� �� ����
    if (!isPathClear(newPos, pieces)) return false;

    // ���������, ����� �� ����� ������ � �������� �������
    Piece* target = getPieceAt(newPos, pieces);
    return target == nullptr || target->getColor() != color;
}

std::unique_ptr<Piece> Bishop::clone() const {
    return std::make_unique<Bishop>(*this);
}

// ���������� ������� ��� �����
Queen::Queen(Color color, Position position) : Piece(color, position, color == Color::WHITE ? 'Q' : 'q') {}

bool Queen::isValidMove(Position newPos, const std::vector<std::unique_ptr<Piece>>& pieces) const {
    if (!newPos.isValid()) return false;

    // ����� ����� ��� ����� ��� ���� (�� ������ ��� �� ���������)
    if (newPos.x != position.x && newPos.y != position.y &&
        abs(newPos.x - position.x) != abs(newPos.y - position.y)) {
        return false;
    }

    // ���������, �������� �� ����
    if (!isPathClear(newPos, pieces)) return false;

    // ���������, ����� �� ����� ������ � �������� �������
    Piece* target = getPieceAt(newPos, pieces);
    return target == nullptr || target->getColor() != color;
}

std::unique_ptr<Piece> Queen::clone() const {
    return std::make_unique<Queen>(*this);
}

// ���������� ������� ��� ������
King::King(Color color, Position position) : Piece(color, position, color == Color::WHITE ? 'K' : 'k') {}

bool King::isValidMove(Position newPos, const std::vector<std::unique_ptr<Piece>>& pieces) const {
    if (!newPos.isValid()) return false;

    // ������ ����� ������ ������ �� ���� ������ � ����� �����������
    if (abs(newPos.x - position.x) > 1 || abs(newPos.y - position.y) > 1) return false;

    // ���������, ����� �� ����� ������ � �������� �������
    Piece* target = getPieceAt(newPos, pieces);
    return target == nullptr || target->getColor() != color;
}

std::unique_ptr<Piece> King::clone() const {
    return std::make_unique<King>(*this);
}

// ������������� ��������� �����
ChessBoard::ChessBoard() : currentTurn(Color::WHITE), gameOver(false) {
    initializePieces();
}

// ��������� ����������� �����
void ChessBoard::initializePieces() {
    // ����� ������
    pieces.push_back(std::make_unique<Rook>(Color::WHITE, Position(0, 0)));
    pieces.push_back(std::make_unique<Knight>(Color::WHITE, Position(1, 0)));
    pieces.push_back(std::make_unique<Bishop>(Color::WHITE, Position(2, 0)));
    pieces.push_back(std::make_unique<Queen>(Color::WHITE, Position(3, 0)));
    pieces.push_back(std::make_unique<King>(Color::WHITE, Position(4, 0)));
    pieces.push_back(std::make_unique<Bishop>(Color::WHITE, Position(5, 0)));
    pieces.push_back(std::make_unique<Knight>(Color::WHITE, Position(6, 0)));
    pieces.push_back(std::make_unique<Rook>(Color::WHITE, Position(7, 0)));
    // ����� �����
    for (int i = 0; i < 8; ++i) {
        pieces.push_back(std::make_unique<Pawn>(Color::WHITE, Position(i, 1)));
    }

    // ������ ������
    pieces.push_back(std::make_unique<Rook>(Color::BLACK, Position(0, 7)));
    pieces.push_back(std::make_unique<Knight>(Color::BLACK, Position(1, 7)));
    pieces.push_back(std::make_unique<Bishop>(Color::BLACK, Position(2, 7)));
    pieces.push_back(std::make_unique<Queen>(Color::BLACK, Position(3, 7)));
    pieces.push_back(std::make_unique<King>(Color::BLACK, Position(4, 7)));
    pieces.push_back(std::make_unique<Bishop>(Color::BLACK, Position(5, 7)));
    pieces.push_back(std::make_unique<Knight>(Color::BLACK, Position(6, 7)));
    pieces.push_back(std::make_unique<Rook>(Color::BLACK, Position(7, 7)));
    // ������ �����
    for (int i = 0; i < 8; ++i) {
        pieces.push_back(std::make_unique<Pawn>(Color::BLACK, Position(i, 6)));
    }
}

// �������� ������ �� �������
Piece* ChessBoard::getPieceAt(Position pos) const {
    for (const auto& piece : pieces) {
        if (piece->getPosition() == pos) {
            return piece.get();
        }
    }
    return nullptr;
}

// ��������, ��������� �� ������ ��� �����
bool ChessBoard::isCheck(Color kingColor) const {
    // ������� ������� ������
    Position kingPos = getKingPosition(kingColor);
    if (!kingPos.isValid()) return false;

    // ���������, ����� �� �����-���� ������ ���������� ��������� ������
    for (const auto& piece : pieces) {
        if (piece->getColor() != kingColor && piece->isValidMove(kingPos, pieces)) {
            return true; // ���
        }
    }
    return false; // ��� ����
}

// �������� �� ���
bool ChessBoard::isCheckmate(Color kingColor) {
    // ���� ��� ����, �� � ���� ���� �� �����
    if (!isCheck(kingColor)) return false;

    Position kingPos = getKingPosition(kingColor);
    if (!kingPos.isValid()) return true; // ������ �� ������ (���������� ��� ���)

    // ��������� ��� ��������� ���� ������
    for (int dx = -1; dx <= 1; ++dx) {
        for (int dy = -1; dy <= 1; ++dy) {
            if (dx == 0 && dy == 0) continue; // ���������� ������� �������

            Position newPos(kingPos.x + dx, kingPos.y + dy);
            if (!newPos.isValid()) continue; // ���������� ������������ �������

            // ���������� �������, ������� ������ ��������
            Piece* target = getPieceAt(newPos);
            if (target && target->getColor() == kingColor) continue;

            // �������� ���������� ������ � ���������, ��������� �� �� ��� �����
            auto kingPiece = getPieceAt(kingPos);
            auto temp = kingPiece->clone();
            kingPiece->setPosition(newPos);

            bool stillInCheck = isCheck(kingColor);

            // ���������� ������ �� �����
            kingPiece->setPosition(kingPos);

            if (!stillInCheck) {
                return false; // ���� ���� �� ���� ���, ��������� ��� - �� ���
            }
        }
    }

    return true; // ��� ���������� �����, ��������� ��� - ���
}

// ��������, ��������� �� ������� ��� ������ ����� ���������� �����
bool ChessBoard::isPositionUnderAttack(Position pos, Color attackingColor) const {
    for (const auto& piece : pieces) {
        if (piece->getColor() == attackingColor && piece->isValidMove(pos, pieces)) {
            return true;
        }
    }
    return false;
}

// �������� ������� ������ ���������� �����
Position ChessBoard::getKingPosition(Color color) const {
    for (const auto& piece : pieces) {
        if (piece->getSymbol() == (color == Color::WHITE ? 'K' : 'k')) {
            return piece->getPosition();
        }
    }
    return Position(-1, -1); // ������ �� ������
}

// �������� ����� ��� ���������� ����
bool ChessBoard::movePiece(Position from, Position to) {
    if (gameOver) return false; // ���� ��� ��������

    // ���������, ���� �� ������ � ��������� ������� � ����������� �� ��� �������� ������
    Piece* piece = getPieceAt(from);
    if (!piece || piece->getColor() != currentTurn) return false;

    // ���������, �������� �� ��� ��� ���� ������
    if (!piece->isValidMove(to, pieces)) return false;

    // ������� ��������� ����� ������ ��� ��������
    auto temp = piece->clone();
    Piece* capturedPiece = getPieceAt(to);

    // �������� ��������� ���
    piece->setPosition(to);
    bool inCheck = isCheck(currentTurn);

    // �������� ��������� ���
    piece->setPosition(from);

    // ���� ��� ������ ������ ��� ���, �� ����������
    if (inCheck) {
        std::cout << "��� �������� ������ ��� ���" << std::endl;
        return false;
    }

    // ��������� ��� ������������
    piece->setPosition(to);

    // ������� ��������� ������, ���� ��� ����
    if (capturedPiece) {
        pieces.erase(
            std::remove_if(pieces.begin(), pieces.end(),
                [to](const std::unique_ptr<Piece>& p) {
                    return p->getPosition() == to;
                }),
            pieces.end()
        );
    }

    // ���������, �� ��������� �� �� ��� ����������
    Color opponentColor = currentTurn == Color::WHITE ? Color::BLACK : Color::WHITE;
    if (isCheckmate(opponentColor)) {
        gameOver = true;
        std::cout << (currentTurn == Color::WHITE ? "White" : "Black") << " ��� " << std::endl;
    }
    else if (isCheck(opponentColor)) {
        std::cout << (opponentColor == Color::WHITE ? "White" : "Black") << " ��� " << std::endl;
    }

    // �������� ��� ������� ������
    currentTurn = opponentColor;
    return true;
}

// ����������� ��������� �����
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

// ���������� ���� � ����
bool ChessBoard::saveGame(const std::string& filename) const {
    std::ofstream out(filename);
    if (!out) return false;

    // ����������, ��� ������ ���
    out << (currentTurn == Color::WHITE ? "white" : "black") << "\n";

    // ���������� ��� ������ � �� �������
    for (const auto& piece : pieces) {
        out << piece->getSymbol() << " "
            << piece->getPosition().x << " "
            << piece->getPosition().y << "\n";
    }

    return true;
}

// �������� ���� �� �����
bool ChessBoard::loadGame(const std::string& filename) {
    std::ifstream in(filename);
    if (!in) return false;

    // ������� ������� ������
    pieces.clear();

    // ������, ��� ���
    std::string turn;
    in >> turn;
    currentTurn = (turn == "white") ? Color::WHITE : Color::BLACK;

    // ������ ������ � �� �������
    char symbol;
    int x, y;
    while (in >> symbol >> x >> y) {
        Color color = isupper(symbol) ? Color::WHITE : Color::BLACK;
        Position pos(x, y);

        // ������� ������ � ������������ � ������������ �������
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