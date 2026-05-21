module;

#include "tci_export.hpp"

export module tile_connect_interface:igame;

import :ievent;

namespace tc::tci
{
export class TCI_CLASS_API IGame
{
public:
    IGame() = default;
    virtual char const* name() const noexcept = 0;

    virtual void onEvent(IEvent const& event) noexcept = 0;

    IGame(IGame const&) = default;
    IGame& operator=(IGame const&) = default;

    virtual ~IGame() {}
};
}  // namespace tc::tci
