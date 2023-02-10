# TODO

height = 0
while (height < 1 or height > 8):
    val = input("Height: ")
    while not val.isdigit():
        val = input("Height: ")
    height = int(val)

for i in range(1, height+1):
    for j in range(0, height-i):
        print(" ", end="")

    for j in range(0, i):
        print("#", end="")

    print("  ", end="")

    for j in range(0, i):
        print("#", end="")

    print("")