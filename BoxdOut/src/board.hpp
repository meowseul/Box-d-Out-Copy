#pragma once

#include <SFML/Graphics.hpp>
#include <array>
#include <string>
#include <vector>
#include <optional>
#include "piece.hpp"
#include "PieceTextures.hpp"


struct Square {
    PieceType  piece = PieceType::None;
    PieceColor color = PieceColor::None;
    int pieceId = -1;

    bool isEmpty() const { return piece == PieceType::None; }
};

class Board {
public:
    static constexpr int SIZE = 8;

    Board();

    Square&       at(int row, int col);
    const Square& at(int row, int col) const;

    // Resets the grid to the standard chess starting position.
    void setupStartingPosition();

    // Returns all legal destination squares for the piece at (row, col),
    // as (col, row) pairs, given standard chess movement rules.
    std::vector<sf::Vector2i> getValidMoves(int row, int col) const;

    // Draws the checkerboard + piece labels into the given window.
    // tileSize is the pixel size of one square; origin is the top-left
    // pixel where the board should start (e.g. {0,0} or some margin).
    void draw(sf::RenderWindow& window,
                const PieceTextures& pieceTextures,
                float tileSize,
                sf::Vector2f origin,
                std::optional<sf::Vector2i> selected = std::nullopt,
              const std::vector<sf::Vector2i>& validMoves = {}) const;
    
    // Moves whatever is at (fromRow, fromCol) to (toRow, toCol), clearing the source square.
    void movePiece(int fromRow, int fromCol, int toRow, int toCol);
    void removePiece(int id);

private:
    std::array<std::array<Square, SIZE>, SIZE> grid;
    //static std::string pieceLetter(PieceType type);
};
