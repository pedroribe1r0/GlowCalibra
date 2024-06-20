import cv2
import numpy as np
from random import randint as rnd

#tamanho da imagem 423x353
#a matriz da imagem tem o primeiro indice sendo as linhas e o segundo as colunas, ao contrario da representacao 423x253
#imagem2.size/imagem2[0].size = altura
#imagem2[0].size = largura
#imagem2.size = area total

class Coords:
    def __init__(self, x, y):
        self.x = x
        self.y = y


imagem = cv2.imread("Imagens/imagem.jpeg")
imagem2 = cv2.imread("Imagens/imagem.jpeg")
imagem = cv2.cvtColor(imagem, cv2.COLOR_BGR2GRAY) 

limit = 105 #0 <= lim <= 255
area = imagem.size
cont_pixeis = 0
cont_gotas = 0
stack = []

#pixeis já visitados = 256, pixeis terminados = 255, pixeis abaixo do limite ficam pretos
for i in range(0, 353):
    for j in range(0, 423):
        if imagem[i][j] > limit:
            stack.append(Coords(i, j))
            while(stack):  # Corrigido aqui
                coords = stack.pop()
                imagem[coords.x][coords.y] = 255
                cont_pixeis += 1
                for k in range(-1, 2):  
                    for h in range(-1, 2): 
                        nx, ny = coords.x - k, coords.y - h
                        if 0 <= nx < 353 and 0 <= ny < 423:  
                            if imagem[nx][ny] > limit and imagem[nx][ny] < 255:
                                stack.append(Coords(nx, ny))
                                imagem[nx][ny] = 255
                            elif imagem[nx][ny] <= limit:
                                imagem[nx][ny] = 0
            cont_gotas += 1
        #else:
            #imagem[i][j] = 0
                



print(cont_gotas, cont_pixeis)


while 1:
    if cv2.waitKey(20) == 8:
        break
    cv2.imshow("ImagemBW", imagem)
    cv2.imshow("ImagemOriginal", imagem2)
