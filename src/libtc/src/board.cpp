export module board;

import token;
import<vector>;
import<memory>;
import<cstddef>;
import<concepts>;

namespace tc
{
export template <typename T>
requires std::movable<T> class TBoard : public std::enable_shared_from_this<T>
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

    ConstTileContent operator[](SizeTuple const& size) const noexcept
    {
        return (*this)[size.x, size.y];
    }

    TileContent operator[](SizeTuple const& size) noexcept
    {
        return (*this)[size.x, size.y];
    }

    TileContent extract(SizeType const x, SizeType const y) noexcept
    {
        return std::move(this[x, y]);
    }

    TileContent extract(SizeTuple size) noexcept
    {
        return std::move(this[size.x, size.y]);
    }

    SizeType sizex() const noexcept { return m_size.x; }

    SizeType sizey() const noexcept { return m_size.y; }

    SizeTuple size() const noexcept { return {sizex(), sizey()}; }

    TileContent swap_element(SizeType const x, SizeType const y, T element)
    {
        TileContent content{std::make_shared<T>(std::move(element))};
        std::swap(content, (*this)[x, y]);
        return content;
    }

    TileContent swap_element(SizeTuple const& size, T element)
    {
        return swap_element(size.x, size.y, std::move(element));
    }

    template <typename... Args>
    TileContent swap_emplace(SizeType const x, SizeType const y, Args&&... args)
    {
        TileContent content{std::make_shared<T>(std::forward<Args>(args)...)};
        std::swap(content, (*this)[x, y]);
        return content;
    }

    template <typename... Args>
    TileContent swap_emplace(SizeTuple const& size, Args&&... args)
    {
        return swap_emplace(size.x, size.y, std::forward<Args>(args)...);
    }

    template <typename... Args>
    void emplace(SizeType const x, SizeType const y, Args&&... args)
    {
        (*this)[x, y] = std::make_shared<T>(std::forward<Args>(args)...);
    }

    template <typename... Args>
    void emplace(SizeTuple const& size, Args&&... args)
    {
        emplace(size.x, size.y, std::forward<Args>(args)...);
    }

    void emplace(SizeType const x, SizeType const y, Args&&... args)
    {
        (*this)[x, y] = std::make_shared<T>(std::forward<Args>(args)...);
    }

    void emplace(SizeTuple const& size, Args&&... args)
    {
        emplace(size.x, size.y, std::forward<Args>(args)...);
    }

private:
    SizeTuple m_size;
    BoardContent m_tokens;
};

}  // namespace tc
