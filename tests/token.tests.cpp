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

    tc::Token token{board, 0U, 0U, 0U};
    CHECK(token.board().get() == board.get());

    tc::Token const cToken{std::as_const(token)};
    auto cBoard{cToken.board()};
    STATIC_CHECK(std::is_const_v<decltype(cBoard)::element_type>);

    auto cBoard2{token.cboard()};
    STATIC_CHECK(std::is_const_v<decltype(cBoard2)::element_type>);
}
