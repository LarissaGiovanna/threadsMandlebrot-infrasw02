- função que calcula de forma recursiva a funcao de mandlebrot (z_novo = z_atual² + num_imaginario) para cada ponto z

- guardar em um arquivo separado o tempo desse calculo que foi feito de formas diferentes usando as threads

## encontrar c
- largura vai ser referente a dividir o eixo x dos reais em colunas, e vai ser `3,0 / largura` (3,0 pq -2,0 ate 1,0 da 3,0)
- altura vai ser referente a dividir o eixo y dos imaginarios em linhas, e vai ser `3,0 / altura` (3,0 pq vai de -1,5 ate 1,5)
- imaginar uma matriz fatiada (largura x e altura y)
- c_real = inicio dos reais -2,0 + (coluna (j) * [3,0 / largura])
- c_imaginario = inicio dos imaginarios -1,5 + (linha(i) * [3,0 / altura])

- for das linhas i vai ate altura e for 2 das colunas j vai ate largura

## calcular formula
- calcular z_novo_real = z_real (inicialmente com 0)² - z_imag² + c_real
- calcular z_novo_imag = 2 * z_real * z_imag + c_imag
- vai resultar duas variaveis com valores diferentes
- ficar fazendo isso repetidas vezes ate que z_real² + z_imag² > 4 (se der maior que 4, o numero explodiu)
- se ele explodiu, nao faz parte do conjunto
- se nao aconteceu nada, ele faz parte do conjunto


#### openmp
#pragma omp parallel num_threads(num_threads recebido do args){
    #pragma omp for private(i)
        for ...
}