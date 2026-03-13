export module token;

import <memory>;
import board;

namespace tc
{
export class Token
{
public:
    using BoardSPtr = std::shared_ptr<TBoard<Token>>;
    using ConstBoardSPtr = std::shared_ptr<TBoard<Token> const>;

    explicit Token(BoardSPtr parent_board) :
        m_parent_board{std::move(parent_board)}
    {}

    BoardSPtr board() noexcept { return m_parent_board; }
    ConstBoardSPtr board() const noexcept { return m_parent_board; }

private:
    BoardSPtr m_parent_board;
};
}  // namespace tc
