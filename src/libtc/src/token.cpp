export module token;

import <memory>;
import board;

namespace tc
{
export class Token
{
public:
    using ThisBoard      = TBoard<Token>;
    using BoardSPtr      = std::shared_ptr<ThisBoard>;
    using ConstBoardSPtr = std::shared_ptr<ThisBoard const>;
    using SizeTuple      = ThisBoard::SizeTuple;
    using SizeType       = ThisBoard::SizeType;
    using TokenValue     = int64_t;

    Token(BoardSPtr parent_board, SizeTuple const& position, TokenValue value) :
        m_parent_board{std::move(parent_board)},
        m_position{position},
        m_value{std::move(value)}
    {}

    Token(BoardSPtr parent_board,
          SizeType const x,
          SizeType const y,
          TokenValue value) :
        Token(std::move(parent_board), SizeTuple{x, y}, std::move(value))
    {}

    TokenValue get() const noexcept { return m_value; }

    BoardSPtr board() noexcept { return m_parent_board; }
    ConstBoardSPtr board() const noexcept { return m_parent_board; }
    ConstBoardSPtr cboard() const noexcept { return m_parent_board; }

    Token(Token const&)            = default;
    Token& operator=(Token const&) = default;

    Token(Token&&)            = default;
    Token& operator=(Token&&) = default;

private:
    BoardSPtr m_parent_board;
    SizeTuple m_position;
    TokenValue m_value;
};
}  // namespace tc
