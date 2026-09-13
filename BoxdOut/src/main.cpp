#include <SFML/Graphics.hpp>
#include "game_manager.hpp"
#include "PieceTextures.hpp"

int main() {
    const float tileSize = 80.f;
    const int boardPixels = static_cast<int>(tileSize) * Board::SIZE;
    //std::optional<sf::Vector2i> selected;
    //std::vector<sf::Vector2i> validMoves;

    // SFML 3's VideoMode takes a Vector2u for size instead of two ints.
    sf::RenderWindow window(sf::VideoMode(sf::Vector2u(boardPixels, boardPixels)), "Chess Board");

    /* SFML 3 renamed loadFromFile -> openFromFile.
    sf::Font font;
    if (!font.openFromFile("assets/KidpixiesRegular-p0Z1.ttf")) {
        return -1; // Font failed to load — check the path.
    } */

    // Loading all the sprites
    PieceTextures pieceTextures;
    if (!pieceTextures.loadAll("assets/board_pieces/")) {
        return -1;
    }

    /* moved all this to game manager duties
    Board board; // Constructor already sets up the starting position.
    Piece whitePiece;
    Piece blackPiece;
    Player p1 (PieceColor::White, {sf::Keyboard::Key::A, sf::Keyboard::Key::D, sf::Keyboard::Key::Space});
    Player p2(PieceColor::Black, {sf::Keyboard::Key::Left, sf::Keyboard::Key::Right, sf::Keyboard::Key::Enter});
    Fight fight(p1, p2, whitePiece, blackPiece);
    
    enum class Screen {
        Chess,
        Fight
    };

   Screen screen = Screen::Chess;
    PieceColor currentTurn = PieceColor::White;
    bool fKeyHeld = false; */

    // Example mutation: directly change a square through the mutable reference.
    // board.at(4, 4).piece = PieceType::Pawn;
    // board.at(4, 4).color = PieceColor::White;
     
    GameManager gm;
    if (!gm.loadTurnTextures("assets/board_pieces/")) {
        return -1;
    }

    if (!gm.loadWinTextures("assets/board_pieces/")) {
        return -1;
    }

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent())
        {   
            //close window
            if(event->is<sf::Event::Closed>()) {
                window.close();
            }

            //mouse stuff
            if(const auto* mouseEvent = event->getIf<sf::Event::MouseButtonPressed>()) {
                if(mouseEvent->button == sf::Mouse::Button::Left) {
                    sf::Vector2i px = mouseEvent->position;

                    int col = static_cast<int>(px.x / tileSize);
                    int row = static_cast<int>(px.y / tileSize);

                    if(row >= 0 && row < Board::SIZE && col >= 0 && col < Board::SIZE) {
                        gm.handleClick(row, col);
                    }
                }
            }
        }

        gm.update(window.getSize());
        window.clear();
        gm.render(window,pieceTextures,tileSize);
        window.display();
    }
    return 0;
}

        /* moved behavior into game manager, also removed f key press bc that was for testing
         if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::F)) {
            if (!fKeyHeld) {
                screen = (screen == Screen::Chess) ? Screen::Fight : Screen::Chess;
                fKeyHeld = true;
            }
        } else {
            fKeyHeld = false;
        } 

        window.clear();

        if (screen == Screen::Chess) {

            board.draw(window, pieceTextures, tileSize, sf::Vector2f(0.f, 0.f), selected, validMoves);
        } else {
            fight.update(window.getSize());
            fight.render(window);
        }

        window.display();
    }
}
    */