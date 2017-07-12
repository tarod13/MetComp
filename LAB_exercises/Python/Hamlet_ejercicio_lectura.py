#1. Cargar y leer 
file = open('pg1524.txt','r')
lines = file.readlines()
file.close()

#2. Separar en palabras
words = list()
for line in lines:
    words_in_line = line.split()    
    words = words + words_in_line

#3. Contar palabras de 9 letras
nine_characters_words = 0
for word in words:
    number_characters = len(word)
    if number_characters == 9:
        nine_characters_words = nine_characters_words + 1

print("Hay %i palabras con 9 letras en el texto\n" % nine_characters_words)

#4. Contar palabras de n letras
for n in range(1,11):
    n_characters_words = 0
    for word in words:
        number_characters = len(word)    
        if number_characters == n:
            n_characters_words = n_characters_words + 1
    print n,n_characters_words
