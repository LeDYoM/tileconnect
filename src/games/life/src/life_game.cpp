export module life_game:game;

import tile_connect_interface;

namespace lg
{
class LifeGame : public tc::tci::IDefaultedGame
{};
}  // namespace lg

extern "C"
{
    tc::tci::IGame* createGame()
    {
        return new lg::LifeGame();
    }

    void destroyGame(tc::tci::IGame* game)
    {
        delete game;
    }
}