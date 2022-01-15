MOTHER 1+2 TRANSLATION TOOLS v1.0
Released April 29, 2011
http://mother12.net

============================================================================================

These are the tools, files, and source code I used to create the MOTHER 1+2 fan translation.
I'm releasing these to the public so that people can create their own translations or
simple game hacks. Keep in mind that I'm not responsible for anything these programs might
do to your system, your life, or the universe. Use at your own risk. If you're worried,
you can always check the source code files and compile the programs yourself.

NOTE: These tools were made for Windows/DOS, so if you have a Mac or something else, you
      might be out of luck. To use the fancy text editor/previewer, you'll probably need
      the Microsoft .NET framework. If your Windows PC is from the last five or ten years,
      then you probably already have this installed by now.

NOTE 2: As I didn't program the MOTHER 1 Funland program, I haven't included the source
        for that. If you're curious about the source code for the xkas assembler, check
        out http://byuu.org

============================================================================================

BASIC INFO:

Unzip the contents of the .zip file into a directory/folder. Then, place a Japanese
MOTHER 1+2 ROM into the same folder. Make sure it's unzipped. Name the file "m12.gba",
without the quotes. It MUST be named that.

After that, you're ready to go. Basically, you just edit the various .txt files, then
run the "i.bat" program. This will do all sorts of magic and create a new ROM called
"test.gba". Run this ROM in an emulator to see your changes. Ideally, you would run
"i.bat" from the command line rather than double-clicking it. That way you can see the
output of the program and see if things were successful or if any errors occurred.

For the most part, you shouldn't touch the .bin files or the .asm files. If you know what
you're doing, then go ahead.

============================================================================================

WORKING WITH THE MOTHER 1 STUFF:

The main text files associated with MOTHER 1 are:

  - m1_main_text.txt - this is THE main file. It has almost all of the text in the game.
                       You can edit this manually with a text editor, or you can use the
                       "MOTHER 1 Funland" program to edit the text and preview it in real
                       time. Also, be sure to save your changes before you close the
                       program!
                       
  - m1_misc_text.txt - this is all the other text in the game. Edit this with a normal
                       text editor. Oftentimes you can move text left or right by
                       adding or subtracting from a line's ROM address. If you don't
                       know what that means, then just ignore it.
                       
                       Note that some of the lines of text in this file might have
                       maximum length limits. I tried my best to make these as large
                       as possible, because I realize it often takes more letters to
                       say things in other languages. Still, if things act weird, you
                       might have to shorten or abbreviate some text here.
                      
  - m1_item_articles.txt - When you use an item or win an item in battle, a custom
                           control code will insert "a", "an", "some", or "the" as
                           is appropriate for the situation. The game knows which one to use
                           by checking this file. The numbers in this file match the with
                           the item names in m1_main_text.txt. Edit this file in a normal
                           text editor.
                           
                           If, for some reason, this is confusing to you or if you don't
                           care, then simply leave out the [03 F0] and [03 F1] parts found
                           on lines 833 and 85C.


IMPORTANT NOTES:

* If you're translating the game into your language but the tools don't seem to be inserting
  your accented letters (or other non-English letters) right, then the easy fix is to look
  up those letters in eng_table.txt, then write the letter code directly. So if the game isn't
  showing your "é" right, then just go into eng_table.txt, find that letter's hexadecimal code,
  which is "2F", and then enter "[2F]" into the text file you're editing, without the quotes,
  of course.
                           

* Main script text has to be manually formatted. Use the MOTHER 1 Funland text previewer to
  make this easier. Battle text DOESN'T need to be formatted, as I added an auto-formatter
  to the game's programming.


* Sometimes you will see text in the main script that is formatted to only go about halfway
  across the screen. This is because that text is used in smaller windows. This usually applies
  to text that is said by a doctor/nurse/healer, or by someone who is storing your items. You'll
  understand when you see those lines of text in action. It will take some trial and error to
  format this special text properly.


* If you're doing something advanced and want to directly enter more than one hex byte,
  keep in mind that if you're editing m1_misc_text.txt, each byte needs to be surrounded
  by brackets, like [EF][05][10]. m1_main_text.txt allows for more if you want, like [03 10],
  but I don't remember if it's limited to a max of two per bracket like that or not.


* When using MOTHER 1 Funland, the top box is the original text, the middle box is where
  you type your new txt, and the bottom line is where you can type a comment about that line
  for future reference. Some of the lines already have notes that I've left in.
  
  When moving from line to line, you don't need to hit "Apply Changes" each time; just press
  press the up or down button to move to a different line and it'll automatically save your
  changes into memory. Just remember to actually save the script before you close the program!

  Quote marks won't act "smart" in the previewer. Don't worry about this - the quotes will
  act right when you try the text in-game.

  This tool wasn't originally meant for public use, so you might find bugs or wish that there
  were more features. For example, if you try to go past the last line in the script, you'll
  get a big error message. If you run into problems like this, just grin and bear it if you
  can.

============================================================================================

WORKING WITH THE MOTHER 2 STUFF

The MOTHER 2 side only has basic menus, item names, enemy names, etc. translated, so there
isn't much to do here. Basically, just edit the text files that start with "m2_", then run
the "i.bat" batch file to insert the text into "test.gba".


IMPORTANT NOTES:

* Most of the file select and naming screens aren't translated. That's because the text
  for these are stored in multiple, very weird ways for who knows what reason.
  
* The MOTHER 2 translation uses the original MOTHER 2 font, so you can only use the English
  letters A-Z. You can't use lowercase letters.

* If you wish to build off of this translation, then you can always add your new changes
  to m2_misc_text.txt. The format is very simple, just enter a ROM address on a new line
  and then type the text (and any control codes) after it.
  
  How do you find what addresses to use? You'll have to figure that out on your own. It
  isn't necessarily hard, it's almost ROM hacking 101-level stuff. Just be warned, there is
  a TON of main script text, the text is less text and more like a giant, complex scripting
  language, and the text+control code format used by MOTHER 2 is different than the format
  used by EarthBound. All in all, it's a big mess.

============================================================================================

CHANGING THE INTRO SCREEN

To change the screen that appears when you first turn the game on, edit the "intro.bmp" file
with a paint program. It's important to keep these limitations in mind:

 - intro.bmp MUST be in 8-bit (256 color) format - you can usually make this change in any
   decent paint program
 - intro.bmp MUST be in uncompressed format - most paint programs save .BMP files
   uncompressed, so this probably won't be a problem for you
 - intro.bmp MUST be 240 pixels wide and 136 pixels tall
 - the 8x8 pixel block in the top-left area will be used as the background image for the
   rest of the screen when the image is displayed in-game. So, for best results, keep
   that 8x8 pixel area in the top left corner one color.
 - obviously, intro.bmp MUST be saved in the .BMP file format. Don't save a .PNG or .JPG
   file and then just rename it - that won't work.

============================================================================================

CHANGING OTHER STUFF

If you want to change the "Select a Game" graphics when choosing between MOTHER 1 and
MOTHER 2, edit the m12_gfx_blahblah.bin files using a tile editor like Tile Molestor.

If you want to change anything else than what's here, you'll have to figure out how to do
that on your own. Check http://romhacking.net for tutorials, tools, and all that good stuff
if you need to learn more.

============================================================================================

CREDITS

xkas cross assembler created by byuu (http://byuu.org/)
MOTHER 1 Funland editor created by Jeffman (http://jeff.erbrecht.ca/)
All else by Tomato (http://tomato.fobby.net)

