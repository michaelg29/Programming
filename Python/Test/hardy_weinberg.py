import random

p = q = 0
generations = 1
offspring = 5
dominant = 'A'
recessive = 'a'

'''
p = float(input('Input value of p (must be between 0 and 1, if outside of range, gets random value): '))
if p < 0 or p > 1:
    p = random.random()

q = 1 - p

generations = int(input('Input number of generations to be simulated (0 < g < 20): '))
while generations <= 0 or generations > 20:
    generations = int(input('Try again, input must be greater than zero and less than 20: '))

offspring = int(input('Input number of offspring in each generation (0 <= o): '))
while offspring <= 0:
    offspring = int(input('Try again, input must be greater than zero: '))

dominant = input('Input symbol for dominant allele: ')
recessive = input('Input symbol for recessive allele: ')
'''

# cut out
p = random.random()
generations = 10
q = 1 - p
# end cut

for i in range(generations):
    hom_dom_count = het_count = hom_rec_count = 0
    print('=' * 10, 'Generation', i, '=' * 10)
    print('p = ', p, '; q = ', q, sep='')
    print('=' * 10)
    print('%10s%10s%10s' % ('Gamete 1', 'Gamete 2', 'Zygote')

    for j in range(offspring)
        gamete_1 = gamete_2 = zygote = ''
        if random.random() <= p:
            gamete_1 = dominant
        else:
            gamete_1 = recessive

        if random.random() <= p:
            gamete_2 = dominant
        else:
            gamete2 += recessive

        zygote = gamete_1 + gamete_2

        print('%10s%10s%10s' % (gamete_1, gamete_2, zygote))

        if zygote = dominant * 2:
            hom_dom_count += 1
        elif zygote == recessive * 2:
            hom_rec_count += 1
        else:
            het_count += 1