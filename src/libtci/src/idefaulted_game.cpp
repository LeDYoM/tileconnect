module;

#include "tci_export.hpp"

export module tile_connect_interface:idefaulted_game;

import :ievent;
import :igame;

namespace tc::tci
{
export class TCI_CLASS_API IDefaultedGame : public IGame
{
public:
    IDefaultedGame() {}

    char const* name() const noexcept override { return "DefaultGameName"; }
    void onEvent(IEvent const&) noexcept override {}
};
}  // namespace tc::tci
