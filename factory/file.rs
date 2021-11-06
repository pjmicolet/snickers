use std::fs;

pub fn open_file_as_string( file : String ) -> String {
    let data = fs::read_to_string( file ).expect( "Unable to read file" );
    return data;
}
