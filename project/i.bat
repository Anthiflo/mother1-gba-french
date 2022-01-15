copy m12.gba test.gba
@introconv.exe
@echo  Inserting new code
@xkas test.gba m12.asm
@insert.exe
