#include "game_manager.hpp"
#include <cmath>

// D: A class with a constructor that initializes member variables using an initializer list
GameManager::GameManager()
: state(GameState::Chess), 
p1(PieceColor::White, {sf::Keyboard::Key::A, sf::Keyboard::Key::D,  sf::Keyboard::Key::Space}), 
p2(PieceColor::Black, {sf::Keyboard::Key::Left, sf::Keyboard::Key::Right, sf::Keyboard::Key::Enter}), 
p1Turn(true)
{
    for(int r = 0; r < Board::SIZE; ++r) {
        for(int c = 0; c < Board::SIZE; ++c) {
            const Square& square = board.at(r, c); //the current square

            if(square.pieceId == -1) {
                continue; 
            }
            pieces[square.pieceId] = Piece(square.pieceId, square.piece, square.color);
        }
    }
}
    GameManager::~GameManager() {}

    bool GameManager::loadTurnTextures(const std::string& directory){
        if (!whiteTurnTexture.loadFromFile(directory + "WhiteTurn.png")) {
            return false;
        }
        if (!blackTurnTexture.loadFromFile(directory + "BlackTurn.png")) {
            return false;
        }
        turnTexturesLoaded = true;
        return true;
    }

    bool GameManager::loadWinTextures(const std::string& directory)
    {
        if (!whiteWinTexture.loadFromFile(directory + "WhiteWin.png")) {
            return false;
        }
        if (!blackWinTexture.loadFromFile(directory + "BlackWin.png")) {
            return false;
        }
        winTexturesLoaded = true;
        return true;
    }

    void GameManager::handleClick(int r, int c)
    {
        if(state != GameState::Chess) {
            return;
        }

        Square& clicked = board.at(r,c);

        if(!selected.has_value()) {
            if(clicked.color == (p1Turn ? PieceColor::White : PieceColor::Black)) {
                selected = sf::Vector2i(c,r);
                validMoves = board.getValidMoves(r,c);
            }
            return;
        }
        sf::Vector2i from = selected.value();

        //click same piece unselects it
        if(from.x == c && from.y == r) {
            selected.reset();
            validMoves.clear();
            return;
        }
        //if clicking on a square that has a piece of the same color, select that piece
        if(!clicked.isEmpty() && clicked.color == (p1Turn ? PieceColor::White : PieceColor::Black)) {
            selected = sf::Vector2i(c,r);
            validMoves = board.getValidMoves(r,c);
            return;
        }

        //validate (sorry i changed the name)
        bool validMove = false;
        for(const auto& move : validMoves) {
            if(move.x == c && move.y == r) {
                validMove = true;
                break;
            }
        }
        if(!validMove) {
            return;
        }

        Square& fromSQ = board.at(from.y, from.x);   
        Square& toSQ = board.at(r, c);


        int attackerID = fromSQ.pieceId;
        Piece& attacker = pieces[attackerID];
        //if clicking on a square that has an enemy piece
        if(!toSQ.isEmpty() && toSQ.color != fromSQ.color) 
        {
            int targetID = toSQ.pieceId; 
            Piece& target = pieces[targetID];
            
            fightFromR = from.y;
            fightFromC = from.x;
            fightToR = r;
            fightToC = c;

            //if the values of the pieces are withing +-1 of each other than initiate fight
            if(std::abs(attacker.value - target.value) <= 1) 
            {
                initiateFight(attackerID, targetID);
                selected.reset();
                validMoves.clear();
                return;
            }
            //otherwise if one piece is greater in value move it
            // Also handle the case where the target is a king so game will be over!
            bool capturedKing = (toSQ.piece == PieceType::King);
            PieceColor movingColor = fromSQ.color;

            board.movePiece(from.y, from.x, r, c);

            if (capturedKing) {
                winner = movingColor;
                state = GameState::Over;
                selected.reset();
                validMoves.clear();
                return;
            }
            switchPlayer();
            selected.reset();
            validMoves.clear();
            return;
        }

         //if square is empty and u want to move just move
        if(toSQ.isEmpty()) {
            board.movePiece(from.y, from.x, r, c);
            switchPlayer();
            selected.reset();
            validMoves.clear();
            return;
        }
    }
    void GameManager::initiateFight(int p1Id, int p2Id) 
    {   //need to make sure white is always p1 and black are p2 so its consistent positioning
        Piece& piece1 = pieces[p1Id];

        if(piece1.color == PieceColor::White) {
            fighter1 = &pieces[p1Id]; //white
            fighter2 = &pieces[p2Id];
        } else {
            fighter1 = &pieces[p2Id]; //white
            fighter2 = &pieces[p1Id];
        } 

        // I: A smart pointer that owns and manages memory
        fight = std::make_unique<Fight>(p1,p2,*fighter1,*fighter2);
        state = GameState::Fight;
    }

    void GameManager::update(sf::Vector2u windowSize)
    {   if(state == GameState::Chess) {
           return;
        }
        else if (state == GameState::Fight) {
            fight->update(windowSize);

            if(fight->getStatus() != FightStatus::Playing) {
                endFight();
            }
            return;
        }
        else if(state == GameState::Over) {
            // Update game over logic
        }
    }

    void GameManager::endFight()
    {
        FightStatus status = fight->getStatus();
        Piece* loser = nullptr;
        Piece* winningPiece = nullptr;

        if(status == FightStatus::white_Wins) {
            winningPiece = fighter1;
            loser = fighter2;
        } else if(status == FightStatus::black_Wins) {
            winningPiece = fighter2;
            loser = fighter1;
        }

        if(loser != nullptr && winningPiece != nullptr) 
        {
            bool attackerWon = (winningPiece->id == board.at(fightFromR, fightFromC).pieceId);

            bool loserWasKing = (loser->type == PieceType::King);
            PieceColor winningColor = winningPiece->color;

            board.removePiece(loser->id);
            winningPiece->value += 1.0f;
            if(attackerWon) {
                board.movePiece(fightFromR, fightFromC, fightToR, fightToC);
            }

            fighter1 = nullptr;
            fighter2 = nullptr;
            fight.reset();

            if (loserWasKing) {
                winner = winningColor;
                state = GameState::Over;
                return;
            }

            switchPlayer();
            state = GameState::Chess;
            return;
        }

        fighter1 = nullptr;
        fighter2 = nullptr;
        fight.reset();
        switchPlayer();
        state = GameState::Chess;
    }

    GameState GameManager::getState() const
        {
            return state;
        }
    
    void GameManager::render(sf::RenderWindow& window,const PieceTextures& pieceTextures, float tileSize)
    {
        if(state == GameState::Chess || state == GameState::Over) {
            board.draw(window, pieceTextures, tileSize, sf::Vector2f(0, 0), selected, validMoves);
        }

        if(state == GameState::Fight) {
            fight->render(window, pieceTextures);
        }

        if(state == GameState::Over) {
            if (winTexturesLoaded && winner != PieceColor::None) {
                const sf::Texture& winTex = (winner == PieceColor::White) ? whiteWinTexture : blackWinTexture;
                sf::Sprite sprite(winTex);
                sf::Vector2u texSize = winTex.getSize();
                float padding = 0.95f;
                sf::Vector2u winSize = window.getSize();
                float scaleX = (winSize.x) / static_cast<float>(std::max(texSize.x, texSize.y));
                float scaleY = (winSize.y) / static_cast<float>(std::max(texSize.x, texSize.y));
                sprite.setScale(sf::Vector2f(scaleX, scaleY));

                
                //sf::FloatRect bounds = sprite.getLocalBounds();
                //sprite.setOrigin(bounds.position + bounds.size / 2.f);
                //sprite.setPosition(sf::Vector2f(pos.x + tileSize / 2.f, pos.y + tileSize / 2.f));

                window.draw(sprite);
                /*const sf::Texture& winTex = (winner == PieceColor::White) ? whiteWinTexture : blackWinTexture;
                sf::Sprite winSprite(winTex);

                sf::Vector2u texSize = winTex.getSize();
                sf::Vector2u winSize = window.getSize();


                float padding = 0.95f;
                float x = (static_cast<float>(winSize.x) - static_cast<float>(texSize.x)) / 2.f;
                float y = (static_cast<float>(winSize.y) - static_cast<float>(texSize.y)) / 2.f;
                winSprite.setPosition(sf::Vector2f(x, y));

                window.draw(winSprite); */
            }
        }

        // --- Turn indicator: centered flash for ~2 seconds after a turn switch ---
        if(state != GameState::Over && turnTexturesLoaded && turnIndicatorClock.getElapsedTime().asSeconds() < turnIndicatorDuration) {
            const sf::Texture& turnTex = p1Turn ? whiteTurnTexture : blackTurnTexture;
            sf::Sprite turnSprite(turnTex);

            sf::Vector2u texSize = turnTex.getSize();
            sf::Vector2u winSize = window.getSize();

            float x = (static_cast<float>(winSize.x) - static_cast<float>(texSize.x)) / 2.f;
            float y = (static_cast<float>(winSize.y) - static_cast<float>(texSize.y)) / 2.f;
            turnSprite.setPosition(sf::Vector2f(x, y));

            window.draw(turnSprite);
        }
    }

    void GameManager::switchPlayer() 
    {
        p1Turn = !p1Turn;
        turnIndicatorClock.restart();
    }