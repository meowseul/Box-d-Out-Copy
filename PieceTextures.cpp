#include "PieceTextures.hpp"


namespace {
    // Maps each PieceType to the name used in the png file
    const char* typeChar(PieceType t) {
        
        switch (t) {
            case PieceType::Pawn:   return "Pawn";
            case PieceType::Knight: return "Knight";
            case PieceType::Bishop: return "Bishop";
            case PieceType::Rook:   return "Rook";
            case PieceType::Queen:  return "Queen";
            case PieceType::King:   return "King";
        }

        throw std::invalid_argument("unknown PieceType");
    }
}

bool PieceTextures::loadAll(const std::string& directory) {
    const PieceColor colors[] = { PieceColor::White, PieceColor::Black };
    const PieceType types[] = {
        PieceType::Pawn, PieceType::Knight, PieceType::Bishop,
        PieceType::Rook, PieceType::Queen, PieceType::King
    };

    // For every color, load all six piece textures.
    for (PieceColor color : colors) {
        for (PieceType type : types) {
            std::string colorChar = (color == PieceColor::White) ? "white" : "black";

            // Full path to a given texture, e.g. "assets/board_pieces/Knight_white.png".
            std::string path = directory + typeChar(type) + colorChar + ".png";

            // Prevents copying
            sf::Texture& tex = textures[colorIndex(color)][typeIndex(type)];

            // Loads the pngs, throws error if one isn't present
            if (!tex.loadFromFile(path)) {
                return false;
            }
        }
    }
    // For every type, load all six punch textures.
    for (PieceType type : types) {

        // Full path to a given texture, e.g. "assets/board_pieces/KnightWeapon.png"
        std::string path = directory + typeChar(type) + "Weapon" + ".png";

        // Prevents copying
        sf::Texture& tex = punchTextures[typeIndex(type)];

        // Loads the pngs, throws error if one isn't present
        // C: Throws at least two errors
        if (!tex.loadFromFile(path)) {
            return false;
        }
    }

    // Load other miscellaneous textures
    const std::pair<std::string, std::string> miscFiles[] = {
        { "FightBackground", "assets/board_pieces/FightBackground.png" },
        { "BlackTurn",        "assets/board_pieces/BlackTurn.png" },
        { "WhiteTurn",        "assets/board_pieces/WhiteTurn.png" },
        { "BlackWin",        "assets/board_pieces/BlackWin.png" },
        { "WhiteWin",        "assets/board_pieces/WhiteWin.png" },
        { "Title",        "assets/board_pieces/Title.png" },
    };

    for (const auto& [key, path] : miscFiles) {
        sf::Texture tex;
        if (!tex.loadFromFile(path)) {
            return false;
        }
        miscTextures[key] = std::move(tex);
    }
    // Every texture loaded successfully
    return true; 
}

const sf::Texture& PieceTextures::get(PieceColor color, PieceType type) const {
    // Lookup into the pre-loaded array
    return textures[colorIndex(color)][typeIndex(type)];
}
const sf::Texture& PieceTextures::get(PieceType type) const {
    // Lookup into the pre-loaded array
    return punchTextures[typeIndex(type)];
}
const sf::Texture& PieceTextures::get(const std::string& key) const {
    // Lookup into the pre-loaded array
    return miscTextures.at(key);
}