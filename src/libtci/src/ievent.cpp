module;

#include "tci_export.hpp"

export module tile_connect_interface:ievent;

namespace tc::tci
{
export class TCI_CLASS_API IEvent
{
    virtual ~IEvent() {}
};
}  // namespace tc::tci
