""" Solution to Problem E MICS 2014 - Hardest Hangman
"""
import sys

alphabet = "abcdefghijklmnopqrstuvwxyz"

def main():
    dictionaryList = []
    maxWordLength = 0
    letterCountDict = {}
    for letter in alphabet:
        letterCountDict[letter] = 0
    
    while True:
        word = sys.stdin.readline().strip().lower()
        if word == "*":
            break
        dictionaryList.append(word)
        if len(word) > maxWordLength:
            maxWordLength = len(word)

        for letter in word:
            letterCountDict[letter] += 1
            
    for length in range(1, maxWordLength+1):
        sys.stdout.write("Hardest word of length "+str(length)+": ")
        minCount = length*len(dictionaryList)
        minWord = ""
        for word in dictionaryList:
            if len(word) == length:
                letterCountSum = 0
                for letter in word:
                    letterCountSum += letterCountDict[letter]
                if letterCountSum < minCount:
                     minCount = letterCountSum
                     minWord = word
        if minWord == "":
            sys.stdout.write("No word of that length is in the dictionary!\n")
        else:
            sys.stdout.write(minWord+"\n")
    

main()
