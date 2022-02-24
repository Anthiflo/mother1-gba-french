arch gba.thumb

//org $8FFFFFF; db $01 // TURN TRANSLATION DEBUG MODE ON


//========================================================================================
//                         MOTHER 1+2 HACKS NOT RELATED TO MOTHER 1
//========================================================================================

// alter select screen graphical text (where you choose between MOTHER 1 and MOTHER 2)
org $86DDC74; incbin m12_gfx_whichgame_a.bin
org $86E4F94; incbin m12_gfx_whichgame_b.bin


//========================================================================================
//                  SIMPLE HACKS TO GET MOTHER 2 MENU PATCH TO WORK RIGHT
//========================================================================================

// try to fix battle menu action highlighting in M2
org $80DC27C; lsl r1,r2,#0x4; nop
org $80DC2AC; lsl r1,r2,#0x4; nop


//========================================================================================
//                           MOTHER 1 SAVE FILE SELECT MENU HACKS
//========================================================================================

// Alter the file select menus
org $8FE5000; incbin m1_window_file_menu_1.bin // #OVERRIDDEN
org $8F0D208; dd $8FE5000

// move character info positions
org $8F0D138; db $03
// Level position
org $8F0D148; db $10
org $8F0D162; db $17
org $8FE6000; incbin m1_window_file_menu_2.bin // #OVERRIDDEN
org $8F0D2A4; dd $8FE6000
// File number position
org $8F0D236; db $10

// "Copy to where?" window
org $8FE7000; incbin m1_window_file_menu_3.bin // #OVERRIDDEN
org $8F0D37C; dd $8FE7000
// move "copy to" cursor when selecting a slot
org $8F2A0B6; db $00
org $8F2A0BA; db $00
org $8F2A0BE; db $00

// "Delete this file?" window
org $8FE8000; incbin m1_window_file_menu_4.bin // #OVERRIDDEN
org $8F0D3DC; dd $8FE8000

// "Override this file?" window (before copy)
org $8F27181; incbin m1_window_file_menu_5.bin // #OVERRIDDEN
// Move file window to clear the cursor correctly after copy (requires to relocate some code)
org $8F0D128; bl file_menu_code_reloc; mov r0,#0;

// lower box erasing stuff
org $8F2713D; db $01
org $8F27141; db $1C
org $8F27146; db $1C
org $8F2714B; db $1C
org $8F27150; db $1C
org $8F27155; db $1C


//========================================================================================
//                           MOTHER 1 NAMING SCREEN STUFF
//========================================================================================

// alter naming windows
org $8F0DE4C; dd $8FE7A00
org $8FE7A00; incbin m1_window_naming.bin // #OVERRIDDEN
org $8F0DDA8; db $02  // move desc. text up one row
org $8F0DDB4; db $05  // move name to be below the text
org $8F0DF7E; db $05
org $8F0DE78; db $05
org $8F0DF52; db $05
org $8F0DFA0; db $05

// repoint character text and resize it
org $8F0DBBC; dd $8FE7C00
org $8F0DBC4; dd $8FE7C30
org $8F0DBD0; dd $8FE7C60
org $8F0DBDC; dd $8FE7C90
org $8F0DBE8; dd $8FE7CC0

// change the question marks when typing a name to dots
org $8F0DF78; db $FC
org $8F0DF6A; db $FC
org $8F0DD72; db $FC
org $8F0E1FE; db $FC

// repoint list of unallowed names
org $8F0E2A0; dd $8FE80C0

// repoint "name not allowed" window
org $8F0E2A4; dd $8FE7200
org $8FE7200; incbin m1_window_name_not_allowed.bin // #OVERRIDDEN

// move the naming screen sprites up a few pixels
org $8F0DDE0; db $10

// repoint "Fav. Food" text on confirmation screen to allow longer text
org $8F0DBF0; dd $8FE79E0

// arrange and clear out the "Is this OK?" confirmation window
org $8F2741F; db $06
org $8F27425; db $10
org $8F2742C; db $01,$01,$01,$01,$01,$01,$01,$01,$01,$01,$01,$01,$01,$01,$01,$01	// #OVERRIDDEN
org $8F2743C; db $03,$0B,$02,$03,$0A
org $8F27441; db $01,$01,$01,$01,$01,$01,$01,$01,$01,$01,$01,$01,$01,$01,$01,$01	// #OVERRIDDEN
org $8F27451; db $03,$0B,$02,$03,$0C,$03,$04,$10,$FB,$03,$0D,$00


//========================================================================================
//                          MOTHER 1 PLAYER NAMING SCREEN STUFF
//========================================================================================

org $8F0BC62; db $FC
org $8F0BDD8; db $FC
org $8F0BDE6; db $FC
org $8F0BF88; db $FC


//========================================================================================
//                            MOTHER 1 BATTLE-RELATED HACKS
//========================================================================================

// alter character stats window in battle
org $8F0AEFC; dd $8FE7400
org $8FE7400; incbin m1_window_char_stats.bin // #OVERRIDDEN

// alter main battle text box design
org $8F275E6; db $01
org $8F275EC; db $1A
org $8F275F5; db $1A
org $8F275FE; db $1A
org $8F27607; db $1A
org $8F27610; db $1A
org $8F2761A; db $1A
org $8F27616; db $02

// repoint and set up "Can't use this" battle text
org $8F29F40; dd $8FE78A0
org $8FE78A0; incbin m1_window_cant_use_item.bin // #OVERRIDDEN

// repoint and set up "Can't equip this" battle text
org $8F29F44; dd $8FE7900
org $8FE7900; incbin m1_window_cant_equip_item.bin // #OVERRIDDEN

// repoint and set up "??? can't use this" battle text
org $8F29F50; dd $8FE7960
org $8FE7960; incbin m1_window_cant_use_other_item.bin // #OVERRIDDEN

// clear out text speed box
org $8F2789C; db $01,$01,$01,$01,$01,$01,$01,$01,$01,$01,$01,$01,$01,$01,$01,$01,$01,$01	//#OVERRIDDEN
org $8F278B3; db $01,$01,$01,$01,$01,$01,$01,$01,$01,$01,$01,$01,$01,$01,$01,$01,$01,$01	//#OVERRIDDEN

org $8F2761F; db $01   // make the game delete the battle box properly
org $8F27623; db $1C
org $8F27628; db $1C
org $8F2762D; db $1C
org $8F27632; db $1C
org $8F27637; db $1C

org $8F27770; db $0D   // move enemy name box left a little bit
org $8F27776; db $0F   // expand enemy name box in battle
org $8F2777F; db $0F
org $8F27788; db $0F
org $8F27791; db $0F
org $8F2779A; db $0F
org $8F277A3; db $0F
org $8F10F32; db $0E   // move cursor

// get pre-parsing stuff to work so we can do auto line wraps
org $8F0F226; bl copy_battle_line_to_ram

// add a space between the enemy name and the suffix letters if there are multiple enemies
org $8F0F2DA; bl add_space_to_enemy_name

// only undo auto-indenting if it's battle text
org $8F0C088; bl possibly_ignore_auto_indents


//========================================================================================
//                              MOTHER 1 OVERWORLD HACKS
//========================================================================================

// Alter the Command menu
org $8F0B290; dd $8FE4000
org $8FE4000; incbin m1_window_command_menu.bin // #OVERRIDDEN

// Alter the Status menu
org $8F0B188; dd $8FE4800
org $8FE4800; incbin m1_window_status_menu.bin // #OVERRIDDEN
org $8F0C6B4; db $04    // fix status menu number alignment
org $8F0CC78; mov r0,#0xC // empty status ailment = 8 characters, like the others

// alter main dialogue box
org $8F0CAE4; bl choose_text_window_type
org $8F0AE48; dd wide_text_box
org $8F7E100
wide_text_box:
   incbin m1_window_wide_text_box.bin
org $8F0CB1E; bl save_line_number_a
// keep game from updating character status on top of dialog boxes
org $8F0B01C; nop; nop;

// let capsule items be "swallowed" instead of "drank"
org $8F08576; bl swallow_item
org $8F08586; bl swallow_item
org $8F08596; bl swallow_item
org $8F085A6; bl swallow_item
org $8F085B6; bl swallow_item

// repoint yes/no main dialog options, make the game know when to choose which one
org $8FE7100; incbin m1_window_yes_no.bin // #OVERRIDDEN
org $8FE7140; incbin m1_window_yes_no_small.bin // #OVERRIDDEN
org $8F04FCE; bl choose_yes_no_size

// alter the item action menus
org $8F0B7C4; dd $8FE4B00
org $8FE4B00; incbin m1_window_item_action_menu.bin // #OVERRIDDEN
org $8F29FB6; db $14
org $8F29FBA; db $14
org $8F29FBE; db $14
org $8F29FC2; db $14
org $8F29FC6; db $14
org $8F29FCE; db $14
org $8F29FD2; db $14
org $8F29FD6; db $14

// expand store menu width
org $8F0BAC0; dd $8FE7800
org $8FE7800; incbin m1_window_shop_menu.bin
org $8F0BAD8; db $0E

// repoint and expand the "Who?" window
org $8F0B9F4; dd $8FE7560
org $8FE7560; incbin m1_window_who.bin // #OVERRIDDEN
// delete expanded "Who?" window properly
org $8F0B9B8; db $08
org $8F0B9CC; db $08
org $8F0B9E0; db $08

// change currency display in hand
org $8F0B03C; dd $8FE4400
org $8FE4400; incbin m1_window_money.bin

// change currency display in shops
org $8F0BAD8; bl change_dollar_sign_pos_shop; nop; nop; nop; nop

org $8F05A6A; bl yes_no_cursor
org $8F04FD8; bl yes_no_cursor
org $8F07FCE; bl yes_no_cursor

org $8F0C100; cmp r0,#0x7F
org $8F0C106; bl more_field_control_codes; nop; nop

// change position of status ailment and other info in status bar
//org $8F0AF48; mov r0,#1  // character name
//org $8F0AF6A; mov r1,#4  // HP
org $8F0AF86; mov r0,#0    // status effect
//org $8F0AFA6; mov r1,#9

// gender for status ailment effects
org $8F0AF8E; bl ailment_gender.call_from_status_bar
org $8F0C4BC; bl ailment_gender.call_from_status_screen

org $8F0CBE4; bl ailment_gender.text_line_with_gender
org $8F0CBF4; bl ailment_gender.text_line_with_gender
org $8F0CC0A; bl ailment_gender.text_line_with_gender
org $8F0CC1E; bl ailment_gender.text_line_with_gender
org $8F0CC32; bl ailment_gender.text_line_with_gender
org $8F0CC46; bl ailment_gender.text_line_with_gender
org $8F0CC5A; bl ailment_gender.text_line_with_gender
org $8F0CC6E; bl ailment_gender.text_line_with_gender

//========================================================================================
//                  FIXES TO BUGS IN THE ORIGINAL MOTHER 1 PROGRAMMING
//========================================================================================

org $8F66332; db $0A   // makes player attack sounds use proper sound
org $8F66308; db $01   // makes enemy attack sounds use proper sound

org $8F29E86; db $AC   // undo the programmers' nonsensical comma replacement stuff
org $8F29E84; db $A3   // undo programmers' weirdness to help get smart quotes working


//========================================================================================
//                                NEW MOTHER 1 GOODIES
//========================================================================================

// create item info for Easy Ring and place it in a box in Ninten's room
org $8F1B3C8; db $1B,$80,$3F,$9C,$02,$00,$02,$00
org $8F027B4; dd newobjecttable
org $8FE8200
newobjecttable:
  incbin m1_data_object_table_1.bin  // repointing a map object table to insert Easy Ring box

org $8F1258E; bl increaseexp
org $8F10350; bl increasemoney
org $8F09698; bl lowerencounterrate
//org $8F0E5E0; bl increase_offense  // turns the Easy Ring into the prank Hard Ring


//========================================================================================
//                               MOTHER 1 ENDING HACKS
//========================================================================================

// the ending runs via one long continuous script, need to repoint it so we can fix stuff
org $8FEA400; incbin m1_data_ending_script.bin // #OVERRIDDEN
org $8F0A500; dd $8FEA400
org $8FEBDB8; db $01,$02,$03,$04,$05,$00,$00,$00,$00,$00,$00,$00  // DIRECTOR 			#OVERRIDE
org $8FEBDE4; db $06,$07,$08,$09,$0A,$0B,$0C,$0D,$00,$00,$00,$00  // GAME DESIGNERS		#OVERRIDE
org $8FEBE05; db $0E,$0F,$10,$11,$12,$13,$14,$15,$16,$00,$00,$00  // MUSIC PRODUCERS	#OVERRIDE
org $8FEBE26; db $0E,$0F,$17,$18,$19,$1A,$1B,$1C,$00,$00,$00,$00  // MUSICAL EFFECTS	#OVERRIDE
org $8FEBE47; db $1D,$1E,$1F,$03,$0C,$20,$21,$22,$23,$15,$16,$00  // CHARACTER DESIGNERS#OVERRIDE
org $8FEBE68; db $24,$25,$26,$27,$28,$09,$29,$2A,$00,$00,$00,$00  // FIGURE MODELING	#OVERRIDE
org $8FEBE84; db $11,$2B,$2C,$2D,$2E,$2F,$30,$00,$00,$00,$00,$00  // PROGRAMMERS		#OVERRIDE
org $8FEBEAA; db $31,$32,$1E,$33,$34,$35,$36,$37,$38,$1C,$00,$00  // SCENARIO ASSISTANTS#OVERRIDE
org $8FEBED5; db $39,$3A,$3B,$3C,$3D,$04,$3E,$00,$00,$00,$00,$00  // COORDINATORS		#OVERRIDE
org $8FEBF0C; db $11,$12,$13,$14,$3F,$00,$00,$00,$00,$00,$00,$00  // PRODUCER			#OVERRIDE
org $8FEBF28; db $40,$41,$42,$43,$44,$45,$46,$47,$48,$49,$00,$00  // EXECUTIVE PRODUCER	#OVERRIDE
//org $8FEBDA9; db $1E // change music played here


//========================================================================================
//                             UNCENSORING MOTHER 1 STUFF
//========================================================================================

incsrc m1_uncensor.asm  // comment this out if you want to put the gfx censoring back in


//========================================================================================
//                               MOTHER 1 GRAPHIC HACKS
//========================================================================================

// insert new main font
org $8F2A5B0; incbin m1_gfx_font.bin

// alter the presented by/produced by screens
org $8F633EC; incbin m1_gfx_produced_by_a.bin //add "d by"
org $8F6350C; incbin m1_gfx_produced_by_b.bin //tail of "y"
org $8F6339C; incbin m1_gfx_produced_by_c.bin //"produce" without tail of "p"
org $8F6349C; incbin m1_gfx_produced_by_d.bin //"presents" + tail of "p"
org $8F0D5D0; bl producescreen1; b $8F0D5E8
org $8F0D63C; bl producescreen2; b $8F0D664
org $8F0D66E; nop
org $8F0D676; nop

// alter the title screen copyrights
org $8F2A216; incbin m1_title_copyrights_arrangement.bin
org $8F6374C; incbin m1_gfx_title_copyrights_1.bin
org $8F6386C; incbin m1_gfx_title_copyrights_2.bin

// change some of the graphical text used in the end credits
org $8F5FF2C; incbin m1_gfx_credits.bin

// insert translated map
org $8F5CB1C; incbin m1_gfx_map.bin

// translate dept store and hotel signs
org $8F2C720; incbin m1_gfx_dept_hotel_1.bin
org $8F2CF20; incbin m1_gfx_dept_hotel_2.bin

// translate stores categories
org $8F013EA; bl alt_tiles_pointers.check_alt_tilesets

define alt_tiles_table $8FFEE00
org {alt_tiles_table}; incbin alt_tiles_table.bin

org $8F325A0; incbin m1_gfx_store_1.bin 
define new_shop_tileset $8FFEA00
org {new_shop_tileset}; incbin m1_gfx_store_2.bin 

org $8F547A0; incbin m1_shop_arrangements.bin
org $8F53DA0; incbin m1_shop_subquads.bin

// change sport subquads
org $8F41DA1; db $6A; db $69
org $8F3D619; db $6A; db $69
org $8F3D5A9; db $6A; db $69

// change palettes for sport
org $8F55030; incbin m1_shop_sport_palette.bin
org $8F54F05; db $C4
org $8F54E46; db $C9


//========================================================================================
//                               MOTHER 1 MAP EDIT
//========================================================================================

// change some positions in order to display Youngtown correctly
org $8F5C81D; db $12,$13  // ADD BLANK BEFORE PODUNK
org $8F5C8D3; db $09,$0A  // ADD THE REST OF YOUNGTOWN


//========================================================================================
//                              NEW MOTHER 1 CODE HACKS
//========================================================================================

org $8FEC400; incsrc m1_new_code.asm
