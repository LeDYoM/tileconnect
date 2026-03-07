#include "catch_include.hpp"

#include <limits>
import tile_connect;

using BoardInt = tc::TBoard<int>;

TEST_CASE("Board::construct", "[Board]")
{
    {
        BoardInt board{3U, 4U};

        CHECK(3U == board.size().x);
        CHECK(4U == board.size().y);
        CHECK(BoardInt::SizeTuple{3U, 4U} == board.size());
    }

    {
        BoardInt board{0U, 0U};

        CHECK(0U == board.size().x);
        CHECK(0U == board.size().y);
        CHECK(BoardInt::SizeTuple{0U, 0U} == board.size());
    }

    {
        constexpr auto BIntMax{std::numeric_limits<uint16_t>::max()};
        BoardInt board{BIntMax, 1U};

        CHECK(BIntMax == board.size().x);
        CHECK(1U == board.size().y);
        CHECK(BoardInt::SizeTuple{BIntMax, 1U} == board.size());
    }
}

TEST_CASE("Board::Access Tokens by index", "[Board][Token]")
{
    constexpr BoardInt::SizeType Size64k{512U};
    BoardInt board{Size64k, Size64k};

    for (BoardInt::SizeType y{0U}; y < Size64k; ++y)
    {
        for (BoardInt::SizeType x{0U}; x < Size64k; ++x)
        {
            CHECK(nullptr == board.get(x, y));
        }
    }

    for (BoardInt::SizeType y{0U}; y < Size64k; ++y)
    {
        for (BoardInt::SizeType x{0U}; x < Size64k; ++x)
        {
            BoardInt::TileContent t{std::make_shared<int>(
                static_cast<int>(board.fromCoords(x, y)))};
            board.push(x, y, std::move(t));
            CHECK(nullptr == t);
        }
    }

    for (BoardInt::SizeType y{0U}; y < Size64k; ++y)
    {
        for (BoardInt::SizeType x{0U}; x < Size64k; ++x)
        {
            CHECK(board.fromCoords(x, y) == *(board.get(x, y)));
        }
    }
}

class DummyInts
{
public:
    DummyInts(int a, int b) : m_a{a}, m_b{b} {}

    constexpr bool operator==(DummyInts const&) const = default;

    int m_a;
    int m_b;
};

TEST_CASE("Board::emplace", "[Board][Token]")
{
    using BoardDummyInts = tc::TBoard<DummyInts>;
    constexpr BoardDummyInts::SizeType Size64k{512U};
    BoardDummyInts board{Size64k, Size64k};

    CHECK(nullptr == board.get(0U, 0U));
    board.emplace(0U, 0U, 3, 4);
    CHECK(DummyInts{3, 4} == *(board.get(0U, 0U)));

    CHECK(nullptr == board.get(Size64k - 1, Size64k - 1U));
    board.emplace({Size64k - 1U, Size64k - 1U}, 43, 124);
    CHECK(DummyInts{43, 124} == *(board.get(Size64k - 1, Size64k - 1U)));
}

TEST_CASE("Board::emplace_swap", "[Board][Token]")
{
    using BoardDummyInts = tc::TBoard<DummyInts>;
    constexpr BoardDummyInts::SizeType Size64k{512U};
    BoardDummyInts board{Size64k, Size64k};

    CHECK(nullptr == board.get(0U, 0U));
    board.emplace(0U, 0U, 3, 4);
    CHECK(DummyInts{3, 4} == *(board.get(0U, 0U)));

    CHECK(nullptr == board.get(Size64k - 1, Size64k - 1U));
    board.emplace(Size64k - 1U, Size64k - 1U, 43, 124);
    CHECK(DummyInts{43, 124} == *(board.get(Size64k - 1, Size64k - 1U)));

    BoardDummyInts::TileContent tile;
    CHECK(nullptr == tile);

    tile = board.emplace_swap(0U, 0U, 12, 25);
    CHECK(nullptr != tile);
    CHECK(DummyInts{3, 4} == *tile);
    CHECK(DummyInts{12, 25} == *(board.get(0U, 0U)));

    tile = board.emplace_swap({Size64k - 1, Size64k - 1U}, 101, 256);
    CHECK(nullptr != tile);
    CHECK(DummyInts{43, 124} == *tile);
    CHECK(DummyInts{101, 256} == *(board.get(Size64k - 1, Size64k - 1U)));
    CHECK(DummyInts{12, 25} == *(board.get(0U, 0U)));
}

TEST_CASE("Board::set", "[Board][Token]")
{
    using BoardDummyInts = tc::TBoard<DummyInts>;
    constexpr BoardDummyInts::SizeType Size64k{512U};
    BoardDummyInts board{Size64k, Size64k};

    CHECK(nullptr == board.get(0U, 0U));
    board.set(0U, 0U, DummyInts{3, 4});
    CHECK(DummyInts{3, 4} == *(board.get(0U, 0U)));

    CHECK(nullptr == board.get(Size64k - 1, Size64k - 1U));
    board.set({Size64k - 1U, Size64k - 1U}, DummyInts{43, 124});
    CHECK(DummyInts{43, 124} == *(board.get(Size64k - 1, Size64k - 1U)));
}

TEST_CASE("Board::set_swap", "[Board][Token]")
{
    using BoardDummyInts = tc::TBoard<DummyInts>;
    constexpr BoardDummyInts::SizeType Size64k{512U};
    BoardDummyInts board{Size64k, Size64k};

    CHECK(nullptr == board.get(0U, 0U));
    board.set(0U, 0U, DummyInts{3, 4});
    CHECK(DummyInts{3, 4} == *(board.get(0U, 0U)));

    CHECK(nullptr == board.get(Size64k - 1, Size64k - 1U));
    board.set(Size64k - 1U, Size64k - 1U, DummyInts{43, 124});
    CHECK(DummyInts{43, 124} == *(board.get(Size64k - 1, Size64k - 1U)));

    BoardDummyInts::TileContent tile;
    CHECK(nullptr == tile);

    tile = board.set_swap(0U, 0U, DummyInts{12, 25});
    CHECK(nullptr != tile);
    CHECK(DummyInts{3, 4} == *tile);
    CHECK(DummyInts{12, 25} == *(board.get(0U, 0U)));

    tile = board.set_swap(BoardDummyInts::SizeTuple{Size64k - 1, Size64k - 1U},
                          DummyInts{101, 256});
    CHECK(nullptr != tile);
    CHECK(DummyInts{43, 124} == *tile);
    CHECK(DummyInts{101, 256} == *(board.get(Size64k - 1, Size64k - 1U)));
    CHECK(DummyInts{12, 25} == *(board.get(0U, 0U)));
}

TEST_CASE("Board::push", "[Board][Token]")
{
    using BoardDummyInts = tc::TBoard<DummyInts>;
    constexpr BoardDummyInts::SizeType Size64k{512U};
    BoardDummyInts board{Size64k, Size64k};

    CHECK(nullptr == board.get(0U, 0U));
    board.push(0U, 0U, std::make_shared<DummyInts>(3, 4));
    CHECK(DummyInts{3, 4} == *(board.get(0U, 0U)));

    CHECK(nullptr == board.get(Size64k - 1, Size64k - 1U));
    board.push({Size64k - 1U, Size64k - 1U},
               std::make_shared<DummyInts>(43, 124));
    CHECK(DummyInts{43, 124} == *(board.get(Size64k - 1, Size64k - 1U)));
}

TEST_CASE("Board::push_swap", "[Board][Token]")
{
    using BoardDummyInts = tc::TBoard<DummyInts>;
    constexpr BoardDummyInts::SizeType Size64k{512U};
    BoardDummyInts board{Size64k, Size64k};

    CHECK(nullptr == board.get(0U, 0U));
    board.push(0U, 0U, std::make_shared<DummyInts>(3, 4));
    CHECK(DummyInts{3, 4} == *(board.get(0U, 0U)));

    CHECK(nullptr == board.get(Size64k - 1, Size64k - 1U));
    board.push(Size64k - 1U, Size64k - 1U,
               std::make_shared<DummyInts>(43, 124));
    CHECK(DummyInts{43, 124} == *(board.get(Size64k - 1, Size64k - 1U)));

    BoardDummyInts::TileContent tile;
    CHECK(nullptr == tile);

    tile = board.push_swap(0U, 0U, std::make_shared<DummyInts>(12, 25));
    CHECK(nullptr != tile);
    CHECK(DummyInts{3, 4} == *tile);
    CHECK(DummyInts{12, 25} == *(board.get(0U, 0U)));

    tile = board.push_swap(BoardDummyInts::SizeTuple{Size64k - 1, Size64k - 1U},
                           std::make_shared<DummyInts>(101, 256));

    CHECK(nullptr != tile);
    CHECK(DummyInts{43, 124} == *tile);
    CHECK(DummyInts{101, 256} == *(board.get(Size64k - 1, Size64k - 1U)));
    CHECK(DummyInts{12, 25} == *(board.get(0U, 0U)));
}

TEST_CASE("Board::swap_tile", "[Board][Token]")
{
    using BoardDummyInts = tc::TBoard<DummyInts>;
    using SizeType       = BoardDummyInts::SizeType;
    using SizeTuple      = BoardDummyInts::SizeTuple;
    constexpr SizeType Size64k{512U};
    BoardDummyInts board{Size64k, Size64k};

    board.emplace(SizeTuple{0U, 0U}, 77, 45);
    board.emplace(SizeTuple{255U, 255U}, 1024, 2045);
    CHECK(DummyInts{77, 45} == *(board.get(SizeTuple{0U, 0U})));
    CHECK(DummyInts{1024, 2045} == *(board.get(SizeTuple{255U, 255U})));

    board.swap_tiles(0U, 0U, 255U, 255U);
    CHECK(DummyInts{1024, 2045} == *(board.get(0U, 0U)));
    CHECK(DummyInts{77, 45} == *(board.get(255U, 255U)));

    board.swap_tiles(SizeTuple{0U, 0U}, SizeTuple{254U, 254U});
    CHECK(nullptr == board.get(0U, 0U));
    CHECK(DummyInts{1024, 2045} == *(board.get(254U, 254U)));
}

TEST_CASE("Board::extract", "[Board][Token]")
{
    using BoardDummyInts = tc::TBoard<DummyInts>;
    using SizeType       = BoardDummyInts::SizeType;
    using SizeTuple      = BoardDummyInts::SizeTuple;
    constexpr SizeType Size64k{512U};
    BoardDummyInts board{Size64k, Size64k};

    board.emplace(SizeTuple{0U, 0U}, 77, 45);
    board.emplace(SizeTuple{255U, 255U}, 1024, 2045);
    CHECK(DummyInts{77, 45} == *(board.get(SizeTuple{0U, 0U})));
    CHECK(DummyInts{1024, 2045} == *(board.get(SizeTuple{255U, 255U})));

    auto ZeroZero{board.extract(0U, 0U)};
    CHECK(DummyInts{77, 45} == *ZeroZero);
    auto Other{board.extract(255U, 255U)};
    CHECK(DummyInts{1024, 2045} == *Other);

    ZeroZero = board.extract(0U, 0U);
    CHECK(nullptr == ZeroZero);
    Other = board.extract(255U, 255U);
    CHECK(nullptr == Other);
}
