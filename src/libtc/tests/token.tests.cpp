#include "catch_include.hpp"

import tile_connect;

using BoardOfTokens = tc::TBoard<tc::Token>;

TEST_CASE("Token::Token", "[Token][Board]")
{
    std::shared_ptr<BoardOfTokens> board{
        BoardOfTokens::createTBoard(128U, 128U)};

    tc::Token token{board, BoardOfTokens::SizeTuple{0U, 0U}, 0U};
    (void)(token);
}

TEST_CASE("Token::board", "[Token][Board]")
{
    std::shared_ptr<BoardOfTokens> board{
        BoardOfTokens::createTBoard(128U, 128U)};

    tc::Token token{board, 0U, 0U, 1};
    CHECK(token.board().get() == board.get());

    tc::Token const cToken{std::as_const(token)};
    auto cBoard{cToken.board()};
    STATIC_CHECK(std::is_const_v<decltype(cBoard)::element_type>);

    auto cBoard2{token.cboard()};
    STATIC_CHECK(std::is_const_v<decltype(cBoard2)::element_type>);
}

TEST_CASE("Token::get", "[Token][Board]")
{
    std::shared_ptr<BoardOfTokens> board{
        BoardOfTokens::createTBoard(128U, 128U)};

    tc::Token token{board, 0U, 0U, 5};
    tc::Token token2{board, 1U, 1U, 30};
    tc::Token token3{board, 2U, 0U, 23};
    tc::Token token4{board, 2U, 1U, 189};

    CHECK(5 == token.get());
    CHECK(30 == token2.get());
    CHECK(23 == token3.get());
    CHECK(189 == token4.get());
}

TEST_CASE("Token::copy", "[Token][Board]")
{
    std::shared_ptr<BoardOfTokens> board{
        BoardOfTokens::createTBoard(128U, 128U)};

    tc::Token token{board, 0U, 0U, 5};
    tc::Token token2{token};
    CHECK(token.board() == token2.board());
    CHECK(token.get() == token2.get());
    CHECK(5 == token2.get());
}
