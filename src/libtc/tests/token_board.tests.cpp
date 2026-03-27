#include "catch_include.hpp"

import tile_connect;

TEST_CASE("TokenBoard::TokenBoard", "[Token][Board]")
{
    tc::TokenBoard board{64U, 64U};

    // Add Token
    {
        auto newToken{board.addToken({0U, 0U}, 1U)};
        CHECK(nullptr != newToken);
        auto newToken2{board.addToken({4U, 4U}, 6U)};
        CHECK(nullptr != newToken2);
    }

    // Swap tokens
    {
        CHECK(board.swapTokens({0U, 0U}, {4U, 4U}));
        CHECK_FALSE(board.swapTokens({0U, 0U}, {5U, 5U}));
        CHECK_FALSE(board.swapTokens({2U, 2U}, {6U, 6U}));
    }

    // Try to add tokens where there are tokens already
    {
        auto oldToken{board.addToken({0U, 0U}, 20U)};
        CHECK(nullptr == oldToken);
        auto oldToken2{board.addToken({4U, 4U}, 10345U)};
        CHECK(nullptr == oldToken2);
    }

    // Add more tokens and read them
    {
        auto newToken{board.addToken({2U, 2U}, 9U)};
        CHECK(nullptr != newToken);
        auto newToken2{board.addToken({9U, 9U}, 10U)};
        CHECK(nullptr != newToken2);

        CHECK(6U == board.get({0U, 0U})->get());
        CHECK(6U == board.get({0U, 0U})->get());
        CHECK(1U == board.get({4U, 4U})->get());
        CHECK(1U == board.get({4U, 4U})->get());
        CHECK(9U == board.get({2U, 2U})->get());
        CHECK(9U == board.get({2U, 2U})->get());
        CHECK(10U == board.get({9U, 9U})->get());
        CHECK(10U == board.get({9U, 9U})->get());

        CHECK(nullptr == board.get({1U, 1U}));
        CHECK(nullptr == board.get({1U, 1U}));
        CHECK(nullptr == board.get({60U, 60U}));
        CHECK(nullptr == board.get({60U, 60U}));
        CHECK(nullptr == board.get({128U, 128U}));
        CHECK(nullptr == board.get({128U, 128U}));
    }
}
