#include "fight.hpp"
#include "PieceTextures.hpp"
#include <algorithm>

namespace {
const float piece_width = 140;
const float piece_height = 180;
const float pieceY = 340;
const float health_width = 220;
const float health_height = 18;
const float health_top = 28;
const float health_side_padding = 24;
}

Fight::Fight(const Player& p1, const Player& p2, Piece& p1Piece, Piece& p2Piece)
    : p1(p1),
      p2(p2),
      p1_piece(p1Piece),
      p2_piece(p2Piece) {
        if (!backgroundTexture.loadFromFile("assets/board_pieces/FightBackground.png")) {
         // C: Throws at least two errors
         throw std::runtime_error("Failed to load FightBackground.png");
        }
      }
   
sf::FloatRect Fight::get_p1_hitbox() {
    return sf::FloatRect(sf::Vector2f(p1_x, pieceY), sf::Vector2f(piece_width, piece_height));
}

sf::FloatRect Fight::get_p2_hitbox() {
    return sf::FloatRect(sf::Vector2f(p2_x, pieceY), sf::Vector2f(piece_width, piece_height));
}

sf::FloatRect Fight::get_p1_punchbox() {
    const float punch_w = 80; //based on the rects i drew, easily adjustable for when we input sprites
    const float punch_h = 50;
    const float y = pieceY + (piece_height - punch_h) / 2; //put in middle of body
    const bool facingRight = (p1_x < p2_x); //determines which side facing
    const float x = facingRight ? (p1_x + piece_width) : (p1_x - punch_w);
    return sf::FloatRect(sf::Vector2f(x, y), sf::Vector2f(punch_w, punch_h));
}

sf::FloatRect Fight::get_p2_punchbox() {
    const float punch_w = 80; 
    const float punch_h = 50;
    const float y = pieceY + (piece_height - punch_h) / 2; 
    const bool facingLeft = (p1_x < p2_x);
    const float x = facingLeft ? (p2_x - punch_w) : (p2_x + piece_width);
    return sf::FloatRect(sf::Vector2f(x, y), sf::Vector2f(punch_w, punch_h));
}
FightStatus Fight::getStatus() const
{
    return status;
}

 void Fight::update(sf::Vector2u windowSize) {
    if(status != FightStatus::Playing) {
        return;
    }

     if(sf::Keyboard::isKeyPressed(p1.controls.left)){
        p1_x -= move_speed;
     }
     if(sf::Keyboard::isKeyPressed(p2.controls.left)){
        p2_x -= move_speed;
     }
     if(sf::Keyboard::isKeyPressed(p1.controls.right)){
        p1_x += move_speed;
     }
     if(sf::Keyboard::isKeyPressed(p2.controls.right)){
        p2_x += move_speed;
     }

     //need to check if the key is being held down so we can limit it to the first push
     bool p1_punch_pressed = sf::Keyboard::isKeyPressed(p1.controls.punch);
     bool p2_punch_pressed = sf::Keyboard::isKeyPressed(p2.controls.punch);

     //for rendering the punchboxes
     p1_punching = p1_punch_pressed;
     p2_punching = p2_punch_pressed;
     
     if(p1_punch_pressed && !p1_punch_was_pressed){

        if(get_p1_punchbox().findIntersection(get_p2_hitbox()).has_value()){
            p2_piece.health -= dmg;
            if(p2_piece.health <= 0){
                p2_piece.health = 0;
                status = FightStatus::white_Wins;
            }
        }
     }
   
     if(p2_punch_pressed && !p2_punch_was_pressed && status == FightStatus::Playing){
    
        if(get_p2_punchbox().findIntersection(get_p1_hitbox()).has_value()){
            p1_piece.health -= dmg;
            if(p1_piece.health <= 0){
                p1_piece.health = 0;
                status = FightStatus::black_Wins;
            }
        }
     }
     //keep same for next frame
     p1_punch_was_pressed = p1_punch_pressed;
     p2_punch_was_pressed = p2_punch_pressed;

     //to prevent bounds issue of running over screen 
     const float maxX = std::max(0.f, static_cast<float>(windowSize.x) - piece_width);
     p1_x = std::clamp(p1_x, 0.f, maxX);
     p2_x = std::clamp(p2_x, 0.f, maxX);
    }

void Fight::render(sf::RenderWindow& window, const PieceTextures& pieceTextures) {
    const sf::Vector2u windowSize = window.getSize();
    const sf::Texture& p1Texture = pieceTextures.get(p1_piece.color, p1_piece.type);
    const sf::Texture& p2Texture = pieceTextures.get(p2_piece.color, p2_piece.type);

    //to get the second player on the right side of the window
    if (!positions_initialized) {
      p2_x = static_cast<float>(windowSize.x) - piece_width - 64.f;
      positions_initialized = true;
    }

    // Background art!
    sf::Sprite background(backgroundTexture);
    sf::Vector2u bgTexSize = backgroundTexture.getSize();
    background.setScale(sf::Vector2f(
        static_cast<float>(windowSize.x) / static_cast<float>(bgTexSize.x),
        static_cast<float>(windowSize.y) / static_cast<float>(bgTexSize.y)
    ));
    window.draw(background);

    // p1 health bar
    {
        const float maxHealth = p1_piece.max_health > 0.f ? p1_piece.max_health : 1.f;
        const float healthVal = std::clamp(p1_piece.health, 0.f, maxHealth);
        const float fillW = health_width * (healthVal / maxHealth);
        sf::RectangleShape fill(sf::Vector2f(fillW, health_height));
        fill.setPosition(sf::Vector2f(health_side_padding, health_top));
        fill.setFillColor(sf::Color::Red);
        window.draw(fill);

        sf::RectangleShape outline(sf::Vector2f(health_width, health_height));
        outline.setPosition(sf::Vector2f(health_side_padding, health_top));
        outline.setFillColor(sf::Color::Transparent);
        outline.setOutlineColor(sf::Color::Blue);
        outline.setOutlineThickness(2);
        window.draw(outline);
    }

    // p2 health
    {
        const float p2x = static_cast<float>(windowSize.x) - health_side_padding - health_width;
        const float maxHealth = p2_piece.max_health > 0.f ? p2_piece.max_health : 1.f;
        const float healthVal = std::clamp(p2_piece.health, 0.f, maxHealth);
        const float fillW = health_width * (healthVal / maxHealth);
        sf::RectangleShape fill(sf::Vector2f(fillW, health_height));
   
        fill.setPosition(sf::Vector2f(p2x + (health_width - fillW), health_top));
        fill.setFillColor(sf::Color::Blue);
        window.draw(fill);

        sf::RectangleShape outline(sf::Vector2f(health_width, health_height));
        outline.setPosition(sf::Vector2f(p2x, health_top));
        outline.setFillColor(sf::Color::Transparent);
        outline.setOutlineColor(sf::Color::Blue);
        outline.setOutlineThickness(2);
        window.draw(outline);
    }

   /* square fighters
    auto drawFighter = [&](float x, sf::Color color) {
        sf::RectangleShape fighter(sf::Vector2f(piece_width, piece_height));
        fighter.setPosition(sf::Vector2f(x, pieceY));
        fighter.setFillColor(color);
        window.draw(fighter);
    }; 
    drawFighter(p1_x, sf::Color::White);
    drawFighter(p2_x, sf::Color::Black);
    */ 

   //had copilot assist with scaling
    sf::Sprite p1Sprite(p1Texture); //to be updated with newer battle sprites once baer is done with those
    sf::Sprite p2Sprite(p2Texture);

    p1Sprite.setScale(sf::Vector2f(piece_width / static_cast<float>(p1Texture.getSize().x), piece_height / static_cast<float>(p1Texture.getSize().y)));
    p2Sprite.setScale(sf::Vector2f(piece_width / static_cast<float>(p2Texture.getSize().x), piece_height / static_cast<float>(p2Texture.getSize().y)));
    
    p1Sprite.setPosition(sf::Vector2f(p1_x, pieceY));
    p2Sprite.setPosition(sf::Vector2f(p2_x, pieceY));

    window.draw(p1Sprite);
    window.draw(p2Sprite);

    /* OLD draw hitboxes & punchboxes
    auto drawHitbox = [&](const sf::FloatRect& r, sf::Color outline, sf::Color fill){
        sf::RectangleShape s(sf::Vector2f(r.size.x, r.size.y));
        s.setPosition(r.position);
        s.setFillColor(fill);
        s.setOutlineColor(outline);
        s.setOutlineThickness(2);
        window.draw(s);
    }; */
 
    if (p1_punching) {
       const sf::Texture& punchTexture = pieceTextures.get(p1_piece.type);

       sf::Sprite punchSprite(punchTexture);
       sf::FloatRect punchbox = get_p1_punchbox();

       const sf::Vector2u textureSize = punchTexture.getSize();
       punchSprite.setScale(sf::Vector2f(punchbox.size.x / static_cast<float>(textureSize.x), punchbox.size.y / static_cast<float>(textureSize.y)));
       punchSprite.setPosition(punchbox.position);
       window.draw(punchSprite);
    }

    if (p2_punching) {
         const sf::Texture& punchTexture = pieceTextures.get(p2_piece.type);

       sf::Sprite punchSprite(punchTexture);
       sf::FloatRect punchbox = get_p2_punchbox();

       const sf::Vector2u textureSize = punchTexture.getSize();
       punchSprite.setScale(sf::Vector2f(punchbox.size.x / static_cast<float>(textureSize.x), punchbox.size.y / static_cast<float>(textureSize.y)));
       punchSprite.setPosition(punchbox.position);
       window.draw(punchSprite);
    }
}