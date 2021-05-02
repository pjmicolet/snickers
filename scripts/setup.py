import os
import pathlib

def generate_build_folder_and_compile_json():
  base_path=input("Please specify snickers base: ")
  path=input("Please specify where your build will be: ")
  pathlib.Path(path).mkdir(parents=True,exist_ok=True)
  os.chdir(path)
  os.popen("cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON " + base_path)
  os.popen("ln -s " + path + "/" + "compile_commands.json " + " " + base_path)
  
generate_build_folder_and_compile_json()
