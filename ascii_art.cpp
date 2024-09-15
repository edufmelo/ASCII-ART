#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct pixel { 		// struct para representar um pixel na imagem PPM
    unsigned char r; 	
    unsigned char g;
    unsigned char b;
};

void gerar_ascii_art(struct pixel **imagem, int linhas, int colunas) {  // imagem - matriz de ponteiros 
    char caracteres[] = {'#','&','@','$','X','O','o','*','.',';','!',' '};
    FILE *saida = fopen("ascii_art.txt", "w"); // abrir arquivo txt para visualizar ascii art

    if (saida == NULL) {
        printf("Erro na abertura do arquivo de saida\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < linhas; i++) {  	// percorre linhas da matriz
        for (int j = 0; j < colunas; j++) { 	// percorre colunas da matriz
            int media_intensidade = (imagem[i][j].r + imagem[i][j].g + imagem[i][j].b) / 3; 		// calcula a media dos componentes de cor (vermelho, verde e azul, respectivamente)

            // calcula o indice do caractere com base na media_intensidade ja calculada
            int indice = (media_intensidade * (sizeof(caracteres) - 1)) / 255; 		//  intensidade de cor vai de 0 a 255

            // escreve o caractere no console e no arquivo
            printf("%c", caracteres[indice]);
            fprintf(saida, "%c", caracteres[indice]);
        }
        printf("\n");
        fprintf(saida, "\n");
    }

    fclose(saida);
}

int main() {
    char arq[] = "19.ppm";		// nome do arquivo
    FILE *fp;
    char id[3];
    int colunas, linhas, maximo;
    struct pixel **imagem;

	fp = fopen(arq, "rb");
	
    fscanf(fp, "%s", id); 
    fscanf(fp, "%d", &colunas); // le numero de linhas da imagem
    fscanf(fp, "%d", &linhas);	// le numero de colunas da imagem
    fscanf(fp, "%d", &maximo); // le o valor maximo possivel para a intensidade de cor na imagem 
    fseek(fp, 1, SEEK_CUR); // SEEK_CUR = posicao atual do ponteiro do arquivo
    
    if (fp == NULL) {
        printf("Erro na abertura do arquivo <%s>", arq);	// verifica arquivo
        if (strcmp(id,"P6")){	// compara id do arquivo com formato ppm = P6
    		printf("\nO arquivo deve estar no formato PPM!");	// verifica formato arquivo
    		exit(EXIT_FAILURE);
    	}
	}

    printf("Tipo do arquivo <%s>: %s\n", arq, id);
    printf("Maximo: %d\n", maximo);
    printf("Imagem tem %d x %d pixels\n", linhas, colunas);
	
	//	basicamente, vai alocar dinamicamente memoria para representar a imagem na forma de uma matriz de pixels
    imagem = (struct pixel **) malloc(linhas * sizeof(struct pixel *)); 	// vetor de ponteiros (struct pixel *) - cada ponteiro vai apontar para um conjunto de pixels representando uma linha da imagem
    for (int i = 0; i < linhas; i++) {		
        imagem[i] = (struct pixel *) calloc(colunas, sizeof(struct pixel));	// alocando as colunas com calloc, zera a memoria
    }	

    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            fscanf(fp, "%c%c%c", &imagem[i][j].r, &imagem[i][j].g, &imagem[i][j].b);	// le as cores de cada pixel na imagem
        }
    }

    gerar_ascii_art(imagem, linhas, colunas);

    for (int i = 0; i < linhas; i++) {		// libera a memoria alocada
        free(imagem[i]);
    }
    free(imagem);

    fclose(fp);

    return 0;
}

