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

        CHECK(6U == board.get({0U, 0U})->value());
        CHECK(6U == board.get({0U, 0U})->value());
        CHECK(1U == board.get({4U, 4U})->value());
        CHECK(1U == board.get({4U, 4U})->value());
        CHECK(9U == board.get({2U, 2U})->value());
        CHECK(9U == board.get({2U, 2U})->value());
        CHECK(10U == board.get({9U, 9U})->value());
        CHECK(10U == board.get({9U, 9U})->value());

        CHECK(nullptr == board.get({1U, 1U}));
        CHECK(nullptr == board.get({1U, 1U}));
        CHECK(nullptr == board.get({60U, 60U}));
        CHECK(nullptr == board.get({60U, 60U}));
        CHECK(nullptr == board.get({128U, 128U}));
        CHECK(nullptr == board.get({128U, 128U}));
    }
}

TEST_CASE("TokenBoard::copy", "[Token][Board]")
{
    tc::TokenBoard board{64U, 64U};

    auto newToken{board.addToken({0U, 0U}, 6U)};
    CHECK(6U == board.get({0U, 0U})->value());
    auto newToken2{board.addToken({4U, 4U}, 4U)};
    CHECK(4U == board.get({4U, 4U})->value());

    tc::TokenBoard board2{board.clone()};
    auto newToken21{board2.get({0U, 0U})};
    CHECK(nullptr != newToken21);
    auto newToken22{board2.get({4U, 4U})};
    CHECK(nullptr != newToken22);

    auto newToken4{board.addToken({40U, 40U}, 40U)};
    CHECK(40U == board.get({40U, 40U})->value());
    CHECK(nullptr == board2.get({40U, 40U}));
}
