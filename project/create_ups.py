import os
print("---------------------------------------------------------")
print("Creating UPS file (mother1vf.ups)")
print("---------------------------------------------------------")
os.system("ups diff -b m12.gba -m test.gba -o mother1vf.ups")
os.system("pause")
