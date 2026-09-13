#pragma once
#include <SFML/Graphics.hpp>
//#include <array>
#include "piece.hpp"


class PieceTextures {
    public:
        // Loads the pngs for every piece
        bool loadAll(const std::string& directory = "assets/board_pieces/");
        const sf::Texture& get(PieceColor color, PieceType type) const;
        const sf::Texture& get(PieceType type) const;
        const sf::Texture& get(const std::string& key) const;

    private:
        // Creates an array of the different piece textures
        std::array<std::array<sf::Texture, 6>, 2> textures;
        //for punchboxes, color doesnt matter
        std::array<sf::Texture, 6> punchTextures;


        // For misc textures
        std::unordered_map<std::string, sf::Texture> miscTextures;


        // Converts the colors into a number for the array
        static int colorIndex(PieceColor c) { return c == PieceColor::White ? 0 : 1; }

        // Converts the piece type into numbers for the array
        // Converts the piece type into numbers for the array
        static int typeIndex(PieceType t) {
            switch (t) {
                case PieceType::Pawn:   return 0;
                case PieceType::Knight: return 1;
                case PieceType::Bishop: return 2;
                case PieceType::Rook:   return 3;
                case PieceType::Queen:  return 4;
                case PieceType::King:   return 5;
                default: throw std::invalid_argument("no texture index for this PieceType");
            }
        }
};