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

pub struct Token {
    str_form: String,
    token_type : String
}

impl fmt::Display for Token {
    fn fmt( &self, f : &mut fmt::Formatter ) -> fmt::Result {
        write!( f, "str_form {}, token_type {}", self.str_form, self.token_type )
    }
}

pub fn create_tokens( line : String ) -> Result< Vec<Token>, LexerError > {
    let potential_tokens = line.split(" ");
    let mut tokens = Vec::new();
    for p_token in potential_tokens {
        for c in p_token.chars() {
            if MATH_OPS.contains( &c ) || CHARS.contains( &c ) || OPEN_BRACKETS.contains( &c ) || CLOSED_BRACKETS.contains( &c ) {
                continue;
            } else {
               return Err( LexerError::new( "Incorrect synax!" ) );
            }
        }
        tokens.push( Token{ str_form:p_token.to_string(), token_type:"bla".to_string() } );
    }
    Ok( tokens )
}


