export module board;

import <vector>;
import <memory>;
import <cstddef>;
import <concepts>;

namespace tc
{
export template <typename T>
class TBoard : public std::enable_shared_from_this<TBoard<T>>
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

    explicit TBoard(SizeTuple const size) : TBoard{size.x, size.y} {}

    [[nodiscard]] constexpr SizeType from_coords(SizeType const x,
                                                SizeType const y) const noexcept
    {
        return (m_size.x * y) + x;
    }

    [[nodiscard]] constexpr SizeType from_coords(
        SizeTuple const& size) const noexcept
    {
        return from_coords(size.x, size.y);
    }

    [[nodiscard]] ConstTileContent get(SizeType const x,
                                       SizeType const y) const noexcept
    {
        return (*this)[x, y];
    }

    [[nodiscard]] ConstTileContent get(SizeTuple const& size) const noexcept
    {
        return (*this)[size];
    }

    [[nodiscard]] TileContent get(SizeType const x, SizeType const y) noexcept
    {
        return (*this)[x, y];
    }

    [[nodiscard]] TileContent get(SizeTuple const& size) noexcept
    {
        return (*this)[size];
    }

    [[nodiscard]] ConstTileContent cget(SizeType const x,
                                        SizeType const y) const noexcept
    {
        return get(x, y);
    }

    [[nodiscard]] ConstTileContent cget(SizeTuple const& size) const noexcept
    {
        return get(size);
    }

    [[nodiscard]] TileContent extract(SizeType const x,
                                      SizeType const y) noexcept
    {
        return std::move((*this)[x, y]);
    }

    [[nodiscard]] TileContent extract(SizeTuple size) noexcept
    {
        return std::move((*this)[size.x, size.y]);
    }

    [[nodiscard]] SizeType sizex() const noexcept { return m_size.x; }

    [[nodiscard]] SizeType sizey() const noexcept { return m_size.y; }

    [[nodiscard]] SizeTuple size() const noexcept { return {sizex(), sizey()}; }

    [[nodiscard]] SizeType cells() const noexcept
    {
        return sizex() * sizey();
    }

    void push(SizeTuple const& size, TileContent content)
    {
        push(size.x, size.y, std::move(content));
    }

    void push(SizeType const x, SizeType const y, TileContent element)
    {
        (*this)[x, y] = std::move(element);
    }

    void set(SizeType const x, SizeType const y, T element)
    {
        push(x, y, std::make_shared<T>(std::move(element)));
    }

    void set(SizeTuple const& size, T element)
    {
        set(size.x, size.y, std::move(element));
    }

    [[nodiscard]] TileContent push_swap(SizeTuple const& size,
                                        TileContent content)
    {
        return push_swap(size.x, size.y, std::move(content));
    }

    [[nodiscard]] TileContent push_swap(SizeType const x,
                                        SizeType const y,
                                        TileContent content)
    {
        std::swap(content, (*this)[x, y]);
        return content;
    }

    [[nodiscard]] TileContent set_swap(SizeType const x,
                                       SizeType const y,
                                       T element)
    {
        return push_swap(x, y, std::make_shared<T>(std::move(element)));
    }

    [[nodiscard]] TileContent set_swap(SizeTuple const& size, T element)
    {
        return set_swap(size.x, size.y, std::move(element));
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

    template <typename... Args>
    [[nodiscard]] TileContent emplace_swap(SizeType const x,
                                           SizeType const y,
                                           Args&&... args)
    {
        TileContent content{std::make_shared<T>(std::forward<Args>(args)...)};
        std::swap(content, (*this)[x, y]);
        return content;
    }

    template <typename... Args>
    [[nodiscard]] TileContent emplace_swap(SizeTuple const& size,
                                           Args&&... args)
    {
        return emplace_swap(size.x, size.y, std::forward<Args>(args)...);
    }

    void swap_tiles(SizeTuple const& lpos, SizeTuple const& rpos)
    {
        if (lpos != rpos)
        {
            std::swap((*this)[lpos.x, lpos.y], (*this)[rpos.x, rpos.y]);
        }
    }

    void swap_tiles(SizeType const lx,
                    SizeType const ly,
                    SizeType const rx,
                    SizeType const ry)
    {
        swap_tiles(SizeTuple{lx, ly}, SizeTuple{rx, ry});
    }

    static std::shared_ptr<TBoard<T>> createTBoard(SizeType const x,
                                                   SizeType const y)
    {
        return std::make_shared<TBoard<T>>(x, y);
    }

    static std::shared_ptr<TBoard<T>> createTBoard(SizeTuple const size)
    {
        return std::make_shared<TBoard<T>>(size);
    }

private:
    [[nodiscard]] ConstTileContent operator[](SizeType const x,
                                              SizeType const y) const noexcept
    {
        return m_tokens[from_coords(x, y)];
    }

    [[nodiscard]] ConstTileContent operator[](
        SizeTuple const& size) const noexcept
    {
        return (*this)[size.x, size.y];
    }

    [[nodiscard]] decltype(auto) operator[](SizeType const x,
                                            SizeType const y) noexcept
    {
        return m_tokens[from_coords(x, y)];
    }

    [[nodiscard]] decltype(auto) operator[](SizeTuple const& size) noexcept
    {
        return (*this)[size.x, size.y];
    }

    SizeTuple m_size;
    BoardContent m_tokens;
};

}  // namespace tc
