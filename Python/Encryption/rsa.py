import random
import string
from math import sqrt

def isPrime(n):
    if n <= 1:
        return False

    if n == 2 or n == 3:
        return True

    # if n is even
    if n % 2 == 0:
        return False

    # test each odd number from 3 to sqrt(n)
    for i in range(3, int(sqrt(n)) + 1, 2):
        if n % i == 0:
            return False

    # n is probably prime
    return True

# primeNums = [i for i in range(int(9e9) + 1, int(1e10), 2) if isPrime(i)]
# print('\a')

# def getPrimeNumber():
#     return primeNums[random.randint(0, len(primeNums) - 1)]

dict = {}
for idx, c in enumerate(string.printable):
    dict[c] = idx

def coPrime(p, q):
    return gcd(p, q) == 1

def gcd(p, q):
    while q:
        p, q = q, p % q
    return p

def encrypt(e, n, m):
    m = ""
    for c in msg:
        m += str(dict[c] ** e % n) + ' '
    return m

def decrypt(d, n, c):
    parts = c.split(' ')
    ret = ""
    for part in parts:
        if part:
            ret += string.printable[int(part) ** d % n]
    
    return ret

    #return (c ** d) % n

if __name__ == "__main__":
    print("RSA Algorithm")

    # get message
    # msg = input("Input message to be encrypted:")
    msg = ''.join([string.printable[random.randint(0, 99)] for i in range(30)])
    print("Message:", msg)

    print("Generating key pairs")

    # ================================ GENERATE KEYS ======================================#
    # find prime numbers p and q
    # p = getPrimeNumber()
    # q = getPrimeNumber()

    p = 173
    q = 149

    # find co-prime numbers to phi(n)
    N = p * q # becomes modulus in encryption/decryption, p and q only factors
    phiN = (p - 1) * (q - 1) # number of co-prime numbers 1 <= x <= N

    # choose e (encryption key)
    # 1 < e < phiN && e co-prime with N, phiN
    # e must be odd and co-prime with phiN
    e = 0
    for i in range(2, phiN):
        if coPrime(i, phiN):
            e = i
            break

    # ================================ ENCRYPT ======================================#
    print("Encryption Key: (", e, ", ", N, ")", sep='')

    # choose d (decryption key)
    # (d * e) % phiN = 1
    d = int((2 * phiN + 1) / e)

    # ================================ DECRYPT ======================================#
    print("Decryption Key: (", d, ", ", N, ")", sep='')

    c = encrypt(e, N, msg)
    print("Encrypted:", c)
    print("Decrypted:", decrypt(d, N, c))
    

