mod file;
fn main() {
    let path = "./test.txt";
    let file = file::open_file_as_string_vec( path.to_string() );
    for string in file {
        println!( "{}", string );
    }
    println!("We bake (?) snickers here")
}
