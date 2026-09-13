#pragma once

#include <SFML/Graphics.hpp>
#include "piece.hpp"

/* player
- color to differentiate between players 
- controls for the player, since on same computer i thought we should mb store what the left right and punch controls are
and we can just initialize different keys in the constructor*/
struct Controls
{
//2d so only need left and right rlly, idnc ab jumping sorry
    sf::Keyboard::Key left;
    sf::Keyboard::Key right;
    sf::Keyboard::Key punch;
};

class Player {
public:
    Player(PieceColor c, Controls ctrl);

    PieceColor color; 
    Controls controls; 
};