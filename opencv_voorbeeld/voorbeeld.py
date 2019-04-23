import math
import random

## return: xs in oplopende volgorde gesorteerd
def invoegsorteren(xs):
    res = []
    for x in xs:
        res = invoegen(x, res)
    return res

def test_invoegsorteren():
    return invoegsorteren([2, 1, 2, 5, 3]) == [1, 2, 2, 3, 5]

## xs is gesorteerd
## return: xs met daaraan toegevoegd x op de goede plaats
def invoegen(x, xs):
    length = len(xs)
    res = []
    k = 0
    while k < length and x >= xs[k]:
        res.append(xs[k])
        k += 1
    res.append(x)
    while k < length:
        res.append(xs[k])
        k += 1
    return res

def test_invoegen():
    return invoegen(4, [1, 3, 7, 8]) == [1 ,3, 4, 7, 8]

def invoegsorteren2(xs):
    xs.sort();
    return xs

def invoegen2(x, xs):
    length = len(xs)
    for k in range(length):
        if x < xs[k]:
            return xs[:k] + [x] + xs[k:]
    return xs + [x]

def test_random():
    getallen      = [random.randint(1, 10) for _ in range(100)]
    kwadraten     = [x**2                  for x in getallen]
    evenkwadraten = [x                     for x in kwadraten if x % 2 == 0]
    return invoegsorteren(evenkwadraten)

## wordt direct uitgevoerd
print test_random()
##sum(x**2 for x in range(10))

