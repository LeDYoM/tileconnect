export module board;

import token;
import <vector>;
import <memory>;
import <cstddef>;

namespace tc
{
export template <typename T>
class TBoard : public std::enable_shared_from_this<T>
{
public:
    using TileContent      = typename std::shared_ptr<T>;
    using ConstTileContent = typename std::shared_ptr<T const>;
    using BoardContent     = std::vector<TileContent>;
    using SizeType         = BoardContent::size_type;

    struct SizeTuple
    {
        SizeType x;
        SizeType y;
        bool operator==(SizeTuple const&) const = default;
    };

    TBoard(SizeType const x, SizeType const y) :
        m_size{x, y}, m_tokens(static_cast<std::size_t>(x * y), nullptr)
    {}

    constexpr SizeType fromCoords(SizeType const x,
                                  SizeType const y) const noexcept
    {
        return (m_size.x * y) + x;
    }

    ConstTileContent operator[](SizeType const x,
                                SizeType const y) const noexcept
    {
        return m_tokens[fromCoords(x, y)];
    }

    TileContent operator[](SizeType const x, SizeType const y) noexcept
    {
        return m_tokens[fromCoords(x, y)];
    }

    SizeType sizex() const noexcept { return m_size.x; }

    SizeType sizey() const noexcept { return m_size.y; }

    SizeTuple size() const noexcept { return {sizex(), sizey()}; }

private:
    SizeTuple m_size;
    BoardContent m_tokens;
};

}  // namespace tc
