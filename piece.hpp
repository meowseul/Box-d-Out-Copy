#pragma once

#include <SFML/Graphics.hpp>

//moved from board to here, also added on health & value
enum class PieceType {
    None,
    Pawn,
    Knight,
    Bishop,
    Rook,
    Queen,
    King
};

enum class PieceColor {
    None,
    White,
    Black
};

struct Piece
{
    int id = -1; //piece tracker
    PieceType type = PieceType::None;
    PieceColor color = PieceColor::None;
    // The value of the piece, which grows when they win combat (also what is used to determine if theres a fight)
    float value = 0;
    float health = 100;
    float max_health = 100;

    Piece() = default;
    Piece(int id, PieceType type, PieceColor color) 
    : id(id), type(type), color(color), value(getStartValue(type))
    {
    }

    static float getStartValue(PieceType type) { //autosuggested values, may change for balancing
        switch(type) {
            case PieceType::Pawn:
                return 1.0f;
            case PieceType::Knight:
                return 3.0f;
            case PieceType::Bishop:
                return 3.0f;
            case PieceType::Rook:
                return 5.0f;
            case PieceType::Queen:
                return 9.0f;
            case PieceType::King:
                return 100.0f;
            default:
                return 0.0f;
        }
    }
};