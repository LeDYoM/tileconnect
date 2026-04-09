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
        BoardInt board{BoardInt::SizeTuple{0U, 0U}};

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
                static_cast<int>(board.from_coords(x, y)))};
            board.push(x, y, std::move(t));
            CHECK(nullptr == t);
        }
    }

    for (BoardInt::SizeType y{0U}; y < Size64k; ++y)
    {
        for (BoardInt::SizeType x{0U}; x < Size64k; ++x)
        {
            CHECK(static_cast<int>(board.from_coords(x, y)) ==
                  *(board.get(x, y)));
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
    BoardDummyInts board{BoardDummyInts::SizeTuple(Size64k, Size64k)};

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

    BoardDummyInts::ConstTileContent tile;
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

    auto token0{board.get(Size64k - 1, Size64k - 1U)};
    auto token1{std::as_const(board).get(Size64k - 1, Size64k - 1U)};
    auto token2{board.get(Size64k - 1, Size64k - 1U)};

    CHECK(DummyInts{43, 124} == *token0);
    CHECK(DummyInts{43, 124} == *token1);
    CHECK(DummyInts{43, 124} == *token2);
    CHECK(token0.get() == token1.get());
    CHECK(token1.get() == token2.get());

    STATIC_CHECK(std::is_const_v<decltype(token0)::element_type>);
    STATIC_CHECK(std::is_const_v<decltype(token1)::element_type>);
    STATIC_CHECK(std::is_const_v<decltype(token2)::element_type>);
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

TEST_CASE("Board::shared_from_this", "[Board][Token]")
{
    auto board{BoardInt::createTBoard(6U, 7U)};
    CHECK(nullptr != board);
    auto board2{board->shared_from_this()};
    CHECK(board.get() == board2.get());
}

TEST_CASE("Board::sizes", "[Board][Token]")
{
    constexpr BoardInt::SizeTuple BoardSize{128U, 256U};
    auto board{BoardInt::createTBoard(BoardSize)};

    CHECK(BoardSize == board->size());
    CHECK(BoardSize.x == board->sizex());
    CHECK(BoardSize.y == board->sizey());

    CHECK(129U == board->from_coords(1U, 1U));
    CHECK(258U == board->from_coords(2U, 2U));

    CHECK((BoardSize.x * BoardSize.y) == board->cells());
}

TEST_CASE("Board::swap_board", "[Board][Token]")
{
    constexpr BoardInt::SizeTuple BoardSize32x32{128U, 128U};

    BoardInt board32x32{BoardSize32x32};
    board32x32.emplace(0U, 0U, 56);
    board32x32.emplace(50U, 124U, 1024);
    CHECK(56 == *(board32x32.get(0U, 0U)));
    CHECK(1024 == *(board32x32.get(50U, 124U)));

    constexpr BoardInt::SizeTuple BoardSize32x32_2{126U, 126U};

    BoardInt board32x32_2{BoardSize32x32_2};
    board32x32_2.emplace(0U, 0U, 56);
    board32x32_2.emplace(50U, 124U, 2024);
    board32x32_2.emplace(60U, 124U, 24);
    CHECK(56 == *(board32x32_2.get(0U, 0U)));
    CHECK(2024 == *(board32x32_2.get(50U, 124U)));
    CHECK(24 == *(board32x32_2.get(60U, 124U)));

    CHECK(board32x32.swap_board(board32x32_2, {0U, 0U}));
    CHECK(56 == *(board32x32.get(0U, 0U)));
    CHECK(56 == *(board32x32_2.get(0U, 0U)));

    CHECK(board32x32.swap_board(board32x32_2, {50U, 124U}));
    CHECK(2024 == *(board32x32.get(50U, 124U)));
    CHECK(1024 == *(board32x32_2.get(50U, 124U)));

    CHECK(board32x32_2.swap_board(board32x32, {60U, 124U}));
    CHECK(24 == *(board32x32.get(60U, 124U)));
    CHECK(nullptr == board32x32_2.get(60U, 124U));

    CHECK(board32x32_2.swap_board(board32x32, {80U, 125U}));
    CHECK(nullptr == board32x32.get(80U, 125U));
    CHECK(nullptr == board32x32_2.get(80U, 125U));

    CHECK_FALSE(board32x32_2.swap_board(board32x32, {80U, 126U}));
    CHECK_FALSE(board32x32.swap_board(board32x32_2, {200U, 126U}));

    CHECK_FALSE(board32x32.swap_board(board32x32, {50U, 124U}));
    CHECK(2024 == *(board32x32.get(50U, 124U)));
    CHECK(1024 == *(board32x32_2.get(50U, 124U)));
}

TEST_CASE("Board::copy_and_moves", "[Board][Token]")
{
    constexpr BoardInt::SizeTuple BoardSize32x32{128U, 128U};
    BoardInt board32x32{BoardSize32x32};
    board32x32.emplace(0U, 0U, 56);
    board32x32.emplace(50U, 124U, 1024);
    CHECK(56 == *(board32x32.get(0U, 0U)));
    CHECK(1024 == *(board32x32.get(50U, 124U)));

    BoardInt board32x32_copy{board32x32};
    CHECK(56 == *(board32x32_copy.get(0U, 0U)));
    CHECK(1024 == *(board32x32_copy.get(50U, 124U)));

    board32x32.emplace(2U, 3U, 9);
    CHECK(9 == *(board32x32.get(2U, 3U)));
    CHECK(nullptr == board32x32_copy.get(2U, 3U));

    board32x32_copy.emplace(5U, 6U, 12);
    CHECK(12 == *(board32x32_copy.get(5U, 6U)));
    CHECK(nullptr == board32x32.get(5U, 6U));

    BoardInt board32x32_copy2{BoardSize32x32};
    CHECK(nullptr == board32x32_copy2.get(0U, 0U));
    CHECK(nullptr == board32x32_copy2.get(2U, 3U));
    CHECK(nullptr == board32x32_copy2.get(5U, 6U));
    CHECK(nullptr == board32x32_copy2.get(50U, 124U));

    board32x32_copy2 = board32x32;
    CHECK(56 == *(board32x32_copy2.get(0U, 0U)));
    CHECK(9 == *(board32x32_copy2.get(2U, 3U)));
    CHECK(nullptr == board32x32_copy2.get(5U, 6U));
    CHECK(1024 == *(board32x32_copy2.get(50U, 124U)));

    BoardInt board32x32_{board32x32};

    BoardInt board32x32_moved{std::move(board32x32_)};
    CHECK(56 == *(board32x32_moved.get(0U, 0U)));
    CHECK(9 == *(board32x32_moved.get(2U, 3U)));
    CHECK(nullptr == board32x32_moved.get(5U, 6U));
    CHECK(1024 == *(board32x32_moved.get(50U, 124U)));

    CHECK(nullptr == board32x32_.get(0U, 0U));
    CHECK(nullptr == board32x32_.get(2U, 3U));
    CHECK(nullptr == board32x32_.get(5U, 6U));
    CHECK(nullptr == board32x32_.get(50U, 124U));

    BoardInt board32x32_move_assigned{128U, 128U};
    board32x32_move_assigned = std::move(board32x32);
    CHECK(56 == *(board32x32_move_assigned.get(0U, 0U)));
    CHECK(9 == *(board32x32_move_assigned.get(2U, 3U)));
    CHECK(nullptr == board32x32_move_assigned.get(5U, 6U));
    CHECK(1024 == *(board32x32_move_assigned.get(50U, 124U)));

    CHECK(nullptr == board32x32.get(0U, 0U));
    CHECK(nullptr == board32x32.get(2U, 3U));
    CHECK(nullptr == board32x32.get(5U, 6U));
    CHECK(nullptr == board32x32.get(50U, 124U));
}

TEST_CASE("Board::begin_and_end", "[Board][Token]")
{
    constexpr BoardInt::SizeTuple BoardSize32x32{128U, 128U};
    BoardInt board32x32{BoardSize32x32};
    board32x32.emplace(0U, 0U, 56);
    board32x32.emplace(1U, 0U, 243);
    board32x32.emplace(127U, 127U, 1024);

    auto start(board32x32.begin());
    CHECK(*start == board32x32.get(0U, 0U));
    CHECK(56 == **start);
    auto cstart(board32x32.cbegin());
    CHECK(start == cstart);

    auto latest(std::prev(board32x32.end()));
    CHECK(*latest == board32x32.get(127U, 127));
    CHECK(1024 == **latest);
    auto clatest(std::prev(board32x32.cend()));
    CHECK(latest == clatest);
}
