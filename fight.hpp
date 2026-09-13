#pragma once

#include <SFML/Graphics.hpp>
#include "piece.hpp"
#include "player.hpp"
#include "PieceTextures.hpp"


enum class FightStatus {
    Playing,
    white_Wins,
    black_Wins
};

class Fight {
public:
    Fight(const Player& p1, const Player& p2, Piece& p1_piece, Piece& p2_piece);
    void update(sf::Vector2u windowSize);
    void render(sf::RenderWindow& window, const PieceTextures& pieceTextures);
    FightStatus getStatus() const;

private:
    const Player& p1;
    const Player& p2;
    Piece& p1_piece;
    Piece& p2_piece;
    float p1_x = 64.f; 
    float p2_x;
    float move_speed = 1.f; //for now
    float dmg = 25.f;
    bool positions_initialized = false;
    bool p1_punching = false;
    bool p2_punching = false;
    bool p1_punch_was_pressed = false; //bc of fps dmg is being dealt faster so have to fix sigh
    bool p2_punch_was_pressed = false;

    FightStatus status = FightStatus::Playing;
    sf::FloatRect get_p1_hitbox(); //hittable area
    sf::FloatRect get_p2_hitbox(); 
    sf::FloatRect get_p1_punchbox(); //where u punching
    sf::FloatRect get_p2_punchbox();

    sf::Texture backgroundTexture;
};
