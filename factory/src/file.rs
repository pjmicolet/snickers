use std::fs;

/// Take a file and return a vector of strings (one per line)
pub fn open_file_as_string_vec( file : String ) -> Vec<String> {
    let data = fs::read_to_string( file ).expect( "Unable to read file" );
    // Not sure this is really what I should be doing but I think it makes sense
    // we can't make str since we don't know it at compile time, it doesn't look very fast but who
    // knows
    let vec = data.split("\n").map(str::to_string).collect::<Vec<String>>();//Jesus this symbol soup lol
    return vec;
}
