mod file;
mod lexer;
fn main() {
    let path = "./test.txt";
    let file = file::open_file_as_string_vec( path.to_string() );
    for string in file {
        let data = lexer::create_tokens( string );
        let data = match data {
            Ok( vec ) => vec,
            Err( error ) => panic!("yeah {}", error )
        };
        for datum in data {
            println!( "{}", datum );
        }
    }
    println!("We bake (?) snickers here")
}
