export module tile_connect_interface:igame;

import :ievent;

namespace tc::interf
{
    export class IGame
    {
    public:
        virtual char const* const name() const noexcept = 0;

        virtual void onEvent();

        virtual ~IGame() {}
    };
}  // namespace tc: interf
