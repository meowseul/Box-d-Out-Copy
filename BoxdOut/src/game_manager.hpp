#pragma once

#include "piece.hpp"
#include "player.hpp"
#include "board.hpp"
#include "fight.hpp"
#include "PieceTextures.hpp"
#include <array>
#include <memory>
#include <optional>
#include <vector>
#include <SFML/System.hpp>

enum class GameState {
    Chess,
    Fight,
    Over
};

class GameManager {
    public:

    GameManager(); 
    ~GameManager();

    void update(sf::Vector2u windowSize);
    void render(sf::RenderWindow& window, const PieceTextures& pieceTextures, float tileSize);
    void handleClick(int r, int c);
    void switchPlayer();
    void initiateFight(int p1Id, int p2Id); 
    void endFight(); //impacts the board update
    bool loadTurnTextures(const std::string& directory = "assets/");
    bool loadWinTextures(const std::string& directory = "assets/board_pieces/");

    GameState getState() const; //check the state

    private:

    GameState state = GameState::Chess;

    sf::Texture whiteTurnTexture;
    sf::Texture blackTurnTexture;

    bool turnTexturesLoaded = false;

    sf::Clock turnIndicatorClock;
    static constexpr float turnIndicatorDuration = 1.0f;

    sf::Texture whiteWinTexture;
    sf::Texture blackWinTexture;
    bool winTexturesLoaded = false;

    PieceColor winner = PieceColor::None;
    
    Player p1;
    Player p2;

    Board board;
    
    std::array<Piece, 32> pieces; //bc 32 pieces on chess board

    Piece* fighter1 = nullptr; //the actual piece in fight game that refers to chess piece
    Piece* fighter2 = nullptr;

    //theres no fight when game starts so itll create one when it does
    std::unique_ptr<Fight> fight;
    //moved from Main, handles chess click
    std::optional<sf::Vector2i> selected;
    std::vector<sf::Vector2i> validMoves;

    bool p1Turn = true; //to track turns


    int fightFromR = -1;
    int fightFromC = -1;
    int fightToR = -1;
    int fightToC = -1;
};
