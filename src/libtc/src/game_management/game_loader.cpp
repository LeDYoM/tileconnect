export module tile_connect:game_loader;

import agloader;

namespace tc::gm
{
export class GameLoader
{
    bool loadGame(std::string const& /*directory*/,
                  std::string const& /*fileName*/)
    {
        return true;
    }
};
}  // namespace tc::gm
