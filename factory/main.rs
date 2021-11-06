mod file;
fn main() {
    let path = "./test.txt";
    let file = file::open_file_as_string( path.to_string() );
    println!( "{}", file );
    println!("We bake (?) snickers here")
}
