module;

#include "tc_export.hpp"

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

    TokenBoard(TokenBoard&&)            = default;
    TokenBoard& operator=(TokenBoard&&) = default;

    TokenBoard& operator=(TokenBoard const&) = default;
    TokenBoard(TokenBoard const&)            = default;

    [[nodiscard]] TokenBoard clone()
    {
        InnerBoard_t inner_token_board_copy(*m_board);
        InnerBoardSharedPtr_t shared_ptr_copy{
            std::make_shared<InnerBoard_t>(std::move(inner_token_board_copy))};

        /*
                for (SizeType y{0U}; y < m_board->sizey(); ++y)
                {
                    for (SizeType x{0U}; x < m_board->sizex(); ++x)
                    {
                        SizeTuple const position{x,y};
                        auto const copy_value{m_board->get(position)->value()};
                        (void)(token_board_copy.addToken(position, copy_value));
                    }
                }
        */

        TokenBoard token_board_copy{std::move(shared_ptr_copy)};

        for (auto const& element : *m_board)
        {
            if (std::shared_ptr<Token const> const token_copy{element};
                token_copy != nullptr)
            {
                SizeTuple const position{token_copy->position()};
                token_board_copy.updateToken(position,
                                             token_copy.get()->value());
            }
        }

        return token_board_copy;
    }

    /**
     * @brief Add a Token object to the board
     * If the position already contains a token, no token will be added
     * @param position The position where to add the token
     * @return TileContent Containing a copy of the added Token or nullptr if a
     * Token was already in the position
     */
    ConstTileContent addToken(SizeTuple const& position,
                              Token::TokenValue value)
    {
        if (m_board->get(position) == nullptr)
        {
            m_board->emplace(position, m_board, position, std::move(value));
            return m_board->get(position);
        }
        return nullptr;
    }

    /**
     * @brief Set a Token value in an already occupied board position.
     * If the position does not contain a token, no token will be added.
     * @param position The position where the Token to update is
     * @return TileContent Containing a copy of the updated Token or nullptr if
     * no Token was in the position.
     */
    ConstTileContent updateToken(SizeTuple const position,
                                 Token::TokenValue value)
    {
        if (auto const oldToken{m_board->get(position)}; oldToken != nullptr)
        {
            m_board->emplace(position, m_board, position, std::move(value));
            return oldToken;
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

    [[nodiscard]] ConstTileContent get(SizeTuple const& position) const noexcept
    {
        return m_board->get(position);
    }

private:
    explicit TokenBoard(InnerBoardSharedPtr_t&& board) noexcept :
        m_board{std::move(board)}
    {}

    InnerBoardSharedPtr_t m_board;
};
}  // namespace tc
