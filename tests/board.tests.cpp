#include "catch_include.hpp"

#include <limits>
import tile_connect;

TEST_CASE("Board::construct")
{
    using BoardInt = tc::TBoard<int>;

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
        constexpr auto BIntMax{std::numeric_limits<BoardInt::SizeType>::max()};
        BoardInt board{BIntMax, BIntMax};

        CHECK(BIntMax == board.size().x);
        CHECK(BIntMax == board.size().y);
        CHECK(BoardInt::SizeTuple{BIntMax, BIntMax} == board.size());
    }

    // Access all tokens by index.
}
