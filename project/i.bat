copy m12.gba test.gba
@python converttext.py
@introconv.exe
@echo  Inserting new code
@xkas test.gba m12.asm
@insert.exe 1
@echo.
@python check_overlap.py m12.asm insert_report.txt
@python check_text_lengths.py m1_main_text.txt
@del m1_main_text_converted.txt
@del m1_enemy_long_names_converted.txt
@echo off
@for %%x in (%cmdcmdline%) do if /i "%%~x"=="/c" set DOUBLECLICKED=1
@if defined DOUBLECLICKED pause