#include "catch_include.hpp"

import tile_connect;

using BoardOfTokens = tc::TBoard<tc::Token>;

TEST_CASE("Token::Token", "[Token][Board]")
{
    std::shared_ptr<BoardOfTokens> board{
        BoardOfTokens::createTBoard(128U, 128U)};

    tc::Token token{board};
    (void)(token);
}

TEST_CASE("Token::board", "[Token][Board]")
{
    std::shared_ptr<BoardOfTokens> board{
        BoardOfTokens::createTBoard(128U, 128U)};

    tc::Token token{board};
    CHECK(token.board().get() == board.get());

    tc::Token const cToken{std::as_const(token)};
    auto cBoard{cToken.board()};
    static_assert(std::is_const_v<decltype(cBoard)::element_type>);
}
