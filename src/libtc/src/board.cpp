export module board;

import <vector>;
import <memory>;
import <cstddef>;
import <concepts>;

namespace tc
{
struct OutOfBoundsPolicy
{
    [[nodiscard]] static constexpr bool check(auto const x,
                                              auto const y,
                                              auto const maxx,
                                              auto const maxy)
    {
        return x < maxx && y < maxy;
    }
};

export template <typename T, typename OOBPolocy = OutOfBoundsPolicy>
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

    TBoard(TBoard const&)            = default;
    TBoard& operator=(TBoard const&) = default;

    TBoard(TBoard&& rhs) noexcept :
        m_size{rhs.m_size}, m_tokens{std::move(rhs.m_tokens)}
    {
        rhs.m_size = {0U, 0U};
    }

    TBoard& operator=(TBoard&& rhs)
    {
        std::swap(m_size, rhs.m_size);
        std::swap(m_tokens, rhs.m_tokens);
        return *this;
    }

    [[nodiscard]] constexpr SizeType from_coords(
        SizeType const x,
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
        return valid_coords(x, y) ? (*this)[x, y] : nullptr;
    }

    [[nodiscard]] ConstTileContent get(SizeTuple const& size) const noexcept
    {
        return valid_coords(size.x, size.y) ? (*this)[size] : nullptr;
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

    [[nodiscard]] SizeType cells() const noexcept { return sizex() * sizey(); }

    void push(SizeTuple const& size, TileContent content)
    {
        push(size.x, size.y, std::move(content));
    }

    void push(SizeType const x, SizeType const y, TileContent element)
    {
        if (OOBPolocy::check(x, y, m_size.x, m_size.y))
        {
            (*this)[x, y] = std::move(element);
        }
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
        if (OOBPolocy::check(x, y, m_size.x, m_size.y))
        {
            std::swap(content, (*this)[x, y]);
        }
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
        if (OOBPolocy::check(x, y, m_size.x, m_size.y))
        {
            (*this)[x, y] = std::make_shared<T>(std::forward<Args>(args)...);
        }
    }

    template <typename... Args>
    void emplace(SizeTuple const& size, Args&&... args)
    {
        emplace(size.x, size.y, std::forward<Args>(args)...);
    }

    template <typename... Args>
    [[nodiscard]] ConstTileContent emplace_swap(SizeType const x,
                                                SizeType const y,
                                                Args&&... args)
    {
        TileContent content{std::make_shared<T>(std::forward<Args>(args)...)};
        std::swap(content, (*this)[x, y]);
        return content;
    }

    template <typename... Args>
    [[nodiscard]] ConstTileContent emplace_swap(SizeTuple const& size,
                                                Args&&... args)
    {
        return emplace_swap(size.x, size.y, std::forward<Args>(args)...);
    }

    void swap_tiles(SizeTuple const& lpos, SizeTuple const& rpos)
    {
        if (lpos != rpos &&
            OOBPolocy::check(lpos.x, lpos.y, m_size.x, m_size.y) &&
            OOBPolocy::check(rpos.x, rpos.y, m_size.x, m_size.y))
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

    template <typename T2, typename OOBPolocy2>
    bool swap_board(TBoard<T2, OOBPolocy2>& rhs, SizeTuple const& position)
    {
        if (this != std::addressof(rhs) &&
            OOBPolocy::check(position.x, position.y, m_size.x, m_size.y) &&
            OOBPolocy2::check(position.x, position.y, rhs.m_size.x,
                              rhs.m_size.y))
        {
            std::swap((*this)[position.x, position.y],
                      rhs[position.x, position.y]);
            return true;
        }
        return false;
    }

    [[nodiscard]] bool valid_coords(SizeType const x,
                                    SizeType const y) const noexcept
    {
        return x < m_size.x && y < m_size.y;
    }

    [[nodiscard]] bool valid_coords(SizeTuple const& size) const
    {
        return valid_coords(size.x, size.y);
    }

    void clear() noexcept
    {
        for (auto& token : m_tokens)
        {
            token = nullptr;
        }
    }

    [[nodiscard]] BoardContent::const_iterator begin() const noexcept
    {
        return m_tokens.begin();
    }

    [[nodiscard]] BoardContent::const_iterator cbegin() const noexcept
    {
        return m_tokens.cbegin();
    }

    [[nodiscard]] BoardContent::const_iterator end() const noexcept
    {
        return m_tokens.end();
    }

    [[nodiscard]] BoardContent::const_iterator cend() const noexcept
    {
        return m_tokens.cend();
    }

    [[nodiscard]] static std::shared_ptr<TBoard<T>> createTBoard(
        SizeType const x,
        SizeType const y)
    {
        return std::make_shared<TBoard<T>>(x, y);
    }

    [[nodiscard]] static std::shared_ptr<TBoard<T>> createTBoard(
        SizeTuple const size)
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
