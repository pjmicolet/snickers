use std::fmt;

static MATH_OPS : [ char ; 4 ] = [ '+', '-', '*', '/' ];
static CHARS : [ char ; 62 ] = [ 'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z','0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z' ];
static OPEN_BRACKETS : [ char ; 3 ]= [ '[', '(', '<' ];
static CLOSED_BRACKETS : [ char ; 3 ] = [ ']', ')', '>' ];

#[derive(Debug)]
pub struct LexerError {
    details: String
}

impl LexerError {
    fn new( msg : &str ) -> LexerError {
        LexerError{ details : msg.to_string() }
    }
}

impl fmt::Display for LexerError {
    fn fmt( &self, f : &mut fmt::Formatter ) -> fmt::Result {
        write!( f, "Lexer Exception {}", self.details )
    }
}

pub enum TokenType {
    ID,
    Math,
    OpenBrace,
    CloseBrace,
    OpenSqbr,
    CloseSqbr,
    OpenAngbr,
    CloseAngbr
}

pub struct Token {
    str_form: String,
    token_type : TokenType
}

impl fmt::Display for Token {
    fn fmt( &self, f : &mut fmt::Formatter ) -> fmt::Result {
        let ttype = match self.token_type {
            TokenType::ID => "ID",
            TokenType::Math => "MATH",
            TokenType::OpenBrace => "OPEN_NRACE",
            TokenType::CloseBrace => "CloseBrace",
            TokenType::OpenSqbr => "OpenSqbr",
            TokenType::CloseSqbr => "CloseSqbr",
            TokenType::OpenAngbr => "OpenAngbr",
            TokenType::CloseAngbr => "CloseAngbr",
        };
        write!( f, "str_form {}, token_type {}", self.str_form, ttype )
    }
}

pub fn create_tokens( line : String ) -> Result< Vec<Token>, LexerError > {
    let potential_tokens = line.split(" ");
    let mut tokens = Vec::new();
    for p_token in potential_tokens {
        let mut token_stack : String = "".to_string();
        for c in p_token.chars() {
            if CHARS.contains( &c ) {
                token_stack.push( c );
            }
            else if MATH_OPS.contains( &c ) {
                if !token_stack.is_empty() {
                    tokens.push( Token { str_form:token_stack.to_string(), token_type: TokenType::ID } );
                    token_stack.clear();
                }
                tokens.push( Token { str_form:c.to_string(), token_type: TokenType::Math } );
            }
            else if OPEN_BRACKETS.contains( &c ) || CLOSED_BRACKETS.contains( &c ) {
                if !token_stack.is_empty() {
                    tokens.push( Token { str_form:token_stack.to_string(), token_type: TokenType::ID } );
                    token_stack.clear();
                }
                match c {
                    '[' => tokens.push( Token { str_form:c.to_string(), token_type: TokenType::OpenSqbr } ),
                    '(' => tokens.push( Token { str_form:c.to_string(), token_type: TokenType::OpenBrace } ),
                    '<' => tokens.push( Token { str_form:c.to_string(), token_type: TokenType::OpenAngbr } ),
                    ']' => tokens.push( Token { str_form:c.to_string(), token_type: TokenType::CloseSqbr } ),
                    ')' => tokens.push( Token { str_form:c.to_string(), token_type: TokenType::CloseBrace } ),
                    '>' => tokens.push( Token { str_form:c.to_string(), token_type: TokenType::CloseAngbr } ),
                    _ => return Err( LexerError::new( "Compiler should know what's avail..." ) ),
                };
            } else {
               return Err( LexerError::new( "Incorrect synax!" ) );
            }
        }
        if !token_stack.is_empty() {
            tokens.push( Token{ str_form:token_stack, token_type:TokenType::ID } );
        }
    }
    Ok( tokens )
}


