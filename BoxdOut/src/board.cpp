#include "board.hpp"
#include "PieceTextures.hpp"
#include <cassert>

namespace {
    bool inBounds(int r, int c) {
        return r >= 0 && r < Board::SIZE && c >= 0 && c < Board::SIZE;
    }
}

Board::Board() {
    setupStartingPosition();
}

Square& Board::at(int row, int col) {
    assert(row >= 0 && row < SIZE && col >= 0 && col < SIZE);
    // J: An STL container that isn't vector or string.
    return grid[row][col];
}

const Square& Board::at(int row, int col) const {
    assert(row >= 0 && row < SIZE && col >= 0 && col < SIZE);
    return grid[row][col];
}

void Board::setupStartingPosition() {
    // Clear everything first.
    for (auto& row : grid) {
        for (auto& square : row) {
            square.piece = PieceType::None;
            square.color = PieceColor::None;
            square.pieceId = -1;
        }
    }

    // Back rank piece order, left to right.
    const std::array<PieceType, SIZE> backRank = {
        PieceType::Rook, PieceType::Knight, PieceType::Bishop, PieceType::Queen,
        PieceType::King, PieceType::Bishop, PieceType::Knight, PieceType::Rook
    };

    int id = 0;

    for (int col = 0; col < SIZE; ++col) {
        // Row 0 = Black's back rank, Row 7 = White's back rank.
        grid[0][col] = { backRank[col], PieceColor::Black, id++ };
        grid[7][col] = { backRank[col], PieceColor::White, id++ };

        // Row 1 = Black pawns, Row 6 = White pawns.
        grid[1][col] = { PieceType::Pawn, PieceColor::Black, id++ };
        grid[6][col] = { PieceType::Pawn, PieceColor::White, id++ };
    }
}

void Board::movePiece(int fromRow, int fromCol, int toRow, int toCol) {
    Square& from = at(fromRow, fromCol);
    Square& to   = at(toRow, toCol);
    to = from;            
    from.piece = PieceType::None;
    from.color = PieceColor::None;
    from.pieceId = -1;
}

std::vector<sf::Vector2i> Board::getValidMoves(int row, int col) const {
    std::vector<sf::Vector2i> moves;
    const Square& square = grid[row][col];
    if (square.isEmpty()) return moves;

    PieceColor color = square.color;

    // Adds (r,c) as a valid destination if in bounds; returns true if a
    // sliding piece can continue past it.
    auto tryAdd = [&](int r, int c) -> bool {
        if (!inBounds(r, c)) return false;
        const Square& target = grid[r][c];
        if (target.isEmpty()) {
            moves.push_back(sf::Vector2i(c, r));
            return true;
        } else if (target.color != color) {
            moves.push_back(sf::Vector2i(c, r)); // capture, then stop
            return false;
        }
        return false; // blocked by own piece
    };

    switch (square.piece) {
        case PieceType::Pawn: {
            int dir = (color == PieceColor::White) ? -1 : 1;
            int startRow = (color == PieceColor::White) ? 6 : 1;

            if (inBounds(row + dir, col) && grid[row + dir][col].isEmpty()) {
                moves.push_back(sf::Vector2i(col, row + dir));
                if (row == startRow && grid[row + 2 * dir][col].isEmpty()) {
                    moves.push_back(sf::Vector2i(col, row + 2 * dir));
                }
            }
            for (int dc : {-1, 1}) {
                int r = row + dir, c = col + dc;
                if (inBounds(r, c) && !grid[r][c].isEmpty() && grid[r][c].color != color) {
                    moves.push_back(sf::Vector2i(c, r));
                }
            }
            break;
        }
        case PieceType::Knight: {
            const int offsets[8][2] = {
                {-2,-1},{-2,1},{-1,-2},{-1,2},{1,-2},{1,2},{2,-1},{2,1}
            };
            for (auto& off : offsets) {
                int r = row + off[0], c = col + off[1];
                if (inBounds(r, c)) {
                    const Square& target = grid[r][c];
                    if (target.isEmpty() || target.color != color) {
                        moves.push_back(sf::Vector2i(c, r));
                    }
                }
            }
            break;
        }
        case PieceType::Bishop: {
            const int dirs[4][2] = {{-1,-1},{-1,1},{1,-1},{1,1}};
            for (auto& d : dirs) {
                int r = row + d[0], c = col + d[1];
                while (tryAdd(r, c)) { r += d[0]; c += d[1]; }
            }
            break;
        }
        case PieceType::Rook: {
            const int dirs[4][2] = {{-1,0},{1,0},{0,-1},{0,1}};
            for (auto& d : dirs) {
                int r = row + d[0], c = col + d[1];
                while (tryAdd(r, c)) { r += d[0]; c += d[1]; }
            }
            break;
        }
        case PieceType::Queen: {
            const int dirs[8][2] = {{-1,-1},{-1,1},{1,-1},{1,1},{-1,0},{1,0},{0,-1},{0,1}};
            for (auto& d : dirs) {
                int r = row + d[0], c = col + d[1];
                while (tryAdd(r, c)) { r += d[0]; c += d[1]; }
            }
            break;
        }
        case PieceType::King: {
            const int dirs[8][2] = {{-1,-1},{-1,0},{-1,1},{0,-1},{0,1},{1,-1},{1,0},{1,1}};
            for (auto& d : dirs) {
                int r = row + d[0], c = col + d[1];
                if (inBounds(r, c)) {
                    const Square& target = grid[r][c];
                    if (target.isEmpty() || target.color != color) {
                        moves.push_back(sf::Vector2i(c, r));
                    }
                }
            }
            break;
        }
        default:
            break;
    }

    return moves;
}

    void Board::removePiece(int id) 
    {
        for(auto& r : grid) {
            for(auto& square : r) {
                if (square.pieceId == id) {
                    square.pieceId = -1;
                    square.piece = PieceType::None;
                    square.color = PieceColor::None;
                    return;
                }
            }
        }
    }

void Board::draw(sf::RenderWindow& window,
                const PieceTextures& pieceTextures,
                float tileSize,
                sf::Vector2f origin,
                std::optional<sf::Vector2i> selected,
                const std::vector<sf::Vector2i>& validMoves) const {
    const sf::Color lightSquare(240, 217, 181);
    const sf::Color darkSquare(181, 136, 99);
    const sf::Color selectedColor(255, 235, 100);   // warm yellow
    const sf::Color validMoveColor(140, 200, 255);  // light blue

    sf::RectangleShape tile(sf::Vector2f(tileSize, tileSize));

    for (int row = 0; row < SIZE; ++row) {
        for (int col = 0; col < SIZE; ++col) {
            // Converting the grid to pixel coords
            sf::Vector2f pos = origin + sf::Vector2f(col * tileSize, row * tileSize);

            // Draw the checkerboard square, changing color if selected or a valid move.
            tile.setPosition(pos);

            bool isSelected = selected.has_value() && selected->x == col && selected->y == row;
            bool isValidMove = false;
            for (const auto& mv : validMoves) {
                if (mv.x == col && mv.y == row) { isValidMove = true; break; }
            }

            if (isSelected) {
                tile.setFillColor(selectedColor);
            } else if (isValidMove) {
                tile.setFillColor(validMoveColor);
            } else {
                tile.setFillColor(((row + col) % 2 == 0) ? lightSquare : darkSquare);
            }

            window.draw(tile);

            // Draw the piece sprite, if any.
            const Square& square = grid[row][col];
            if (!square.isEmpty()) {
                // Find the correct texture for this piece
                const sf::Texture& tex = pieceTextures.get(square.color, square.piece);
                sf::Sprite sprite(tex);
                sf::Vector2u texSize = tex.getSize();
                float padding = 0.95f;
                float scale = (tileSize * padding) / static_cast<float>(std::max(texSize.x, texSize.y));
                sprite.setScale(sf::Vector2f(scale, scale));
                sf::FloatRect bounds = sprite.getLocalBounds();

                // Centers the sprite
                sprite.setOrigin(bounds.position + bounds.size / 2.f);
                sprite.setPosition(sf::Vector2f(pos.x + tileSize / 2.f, pos.y + tileSize / 2.f));

                window.draw(sprite);
            }
        }
    }
}