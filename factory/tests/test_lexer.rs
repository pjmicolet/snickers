use lexer;

#[test]
fn basic_lexing() {
    let token_1 = lexer::create_tokens( &"a = b".to_string() ).expect( "Failure" );
    assert_eq!(3, token_1.len());
    matches!( token_1[0].token_type, lexer::TokenType::ID );
    matches!( token_1[1].token_type, lexer::TokenType::Assign );
    matches!( token_1[2].token_type, lexer::TokenType::ID );

    let token_2 = lexer::create_tokens( &"\"This is a string\"".to_string() ).expect( "Failure" );
    assert_eq!(1, token_2.len());
    matches!( token_2[0].token_type, lexer::TokenType::StringLiteral );
}
