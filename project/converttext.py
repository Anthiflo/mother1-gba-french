import io
import re
import shutil
import sys
import os

# Replaces text in a string
def replace(text, old_text, new_text):
    text_pattern = re.compile(re.escape(old_text), flags=re.U)
    return text_pattern.sub(new_text, text)
    
def convert(input_file, output_file, table_file):
    # We store the content of the main text into a variable before editing it
    with io.open(input_file, "r", encoding="utf-8") as file:
        file_contents = file.read()

    # We store the content of the character table
    with io.open(table_file, "r", encoding="ansi") as file:
        table_lines = file.readlines()

    # We iterate through the character table to replace every entry with its code within the main text
    for line_index, table_line in enumerate(table_lines):
        table_entry = table_line.rstrip().split(" ")
        character = table_entry[1]

        # If it’s a single character and it’s not ascii
        if (len(character) == 1 and not character.isascii()):
            value = "[" + table_entry[0] + "]"
            file_contents = replace(file_contents, character, value)

    # We write the result into the output file
    with io.open(output_file, "w+", encoding="utf-8") as file:
        file.seek(0)
        file.truncate()
        file.write(file_contents)
        
#convert("m1_main_text.txt","m1_main_text_converted.txt","eng_table.txt")
#convert("m1_enemy_long_names.txt", "m1_enemy_long_names_converted.txt", "eng_table.txt")

if (len(sys.argv) > 3):
    convert(sys.argv[1],sys.argv[3],sys.argv[2])
else:
    print("Syntax: " + os.path.basename(sys.argv[0]) + " input_text char_table [output_text]")

