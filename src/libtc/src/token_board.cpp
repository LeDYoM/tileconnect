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

    explicit TokenBoard(SizeTuple const& size) :
        m_board{InnerBoard_t::createTBoard(size)}
    {}

    /**
     * @brief Create a Token object
     * If the position already contains a token, no token will be added
     * @param position The position where to add the token
     * @return TileContent Containing a copy of the added Token or nullptr if a
     * Token was already in the position
     */
    TileContent createToken(SizeTuple const& position, Token::TokenValue /*value*/)
    {
        if (m_board->get(position) == nullptr)
        {
//            std::shared_ptr<tc::Token> t{std::make_shared<tc::Token>(m_board, SizeTuple{0U, 0U}, 0U)};
//            InnerBoardSharedPtr_t t2{std::make_shared<InnerBoard_t>(m_board, position, 0)};
            m_board->emplace(position, m_board, SizeTuple{0U, 0U}, 0U);
        }
        return nullptr;
    }

private:
    InnerBoardSharedPtr_t m_board;
};
}  // namespace tc
