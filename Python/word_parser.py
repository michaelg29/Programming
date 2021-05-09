if __name__ == "__main__":
    while True:
        innum = str(input("> "))
        printWords(innum)


def printWords(innum):
    l = len(innum)

    numwords = l // 9
    print("{ ", end="")

    i = l
    while i > 0:
        startIdx = max([0, i - 9])
        print(innum[startIdx:i], end="")
        if startIdx != 0:
            print(", ", end="")
        i = startIdx

    print(" }")


def printN(innum):
    print(innum)
    printWords(str(innum))
