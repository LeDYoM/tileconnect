export module token_board;

import <memory>;
import token;
import board;

namespace tc
{
export class TokenBoard
{
public:
    using InnerBoard_t          = TBoard<Token>;
    using InnerBoardSharedPtr_t = std::shared_ptr<InnerBoard_t>;
    using TileContent           = TBoard<Token>::TileContent;
    using ConstTileContent      = TBoard<Token>::ConstTileContent;
    using SizeTuple             = InnerBoard_t::SizeTuple;
    using SizeType              = InnerBoard_t::SizeType;

    explicit TokenBoard(SizeTuple const& size) :
        m_board{InnerBoard_t::createTBoard(size)}
    {}

    TokenBoard(SizeType const x, SizeType const y) : TokenBoard{SizeTuple{x, y}}
    {}

    /**
     * @brief Add a Token object to the board
     * If the position already contains a token, no token will be added
     * @param position The position where to add the token
     * @return TileContent Containing a copy of the added Token or nullptr if a
     * Token was already in the position
     */
    TileContent addToken(SizeTuple const& position, Token::TokenValue value)
    {
        if (m_board->get(position) == nullptr)
        {
            m_board->emplace(position, m_board, SizeTuple{0U, 0U},
                             std::move(value));
            return m_board->get(position);
        }
        return nullptr;
    }

    bool swapTokens(SizeTuple const& lhPosition,
                    SizeTuple const& rhPosition) noexcept
    {
        if (m_board->get(lhPosition) != nullptr &&
            m_board->get(rhPosition) != nullptr)
        {
            m_board->swap_tiles(lhPosition, rhPosition);
            return true;
        }
        return false;
    }

    TileContent get(SizeTuple const& position) noexcept
    {
        return m_board->get(position);
    }

    ConstTileContent get(SizeTuple const& position) const noexcept
    {
        return m_board->get(position);
    }

    ConstTileContent cget(SizeTuple const& position) const noexcept
    {
        return m_board->cget(position);
    }

private:
    InnerBoardSharedPtr_t m_board;
};
}  // namespace tc
