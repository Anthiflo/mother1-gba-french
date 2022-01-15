copy m12.gba test.gba
@python converttext.py
@introconv.exe
@python check_overlap.py m12.asm
@echo  Inserting new code
@xkas test.gba m12.asm
@insert.exe
@del m1_main_text.txt
