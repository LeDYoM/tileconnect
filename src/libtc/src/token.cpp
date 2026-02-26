export module token;

import <memory>;

namespace tc
{
class Board;
export class Token
{
public:
    explicit Token(std::shared_ptr<Board> parent_board) :
        m_parent_board{std::move(parent_board)}
    {}

private:
    std::shared_ptr<Board> m_parent_board;
};
}  // namespace tc
