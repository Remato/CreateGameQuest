#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl/include/curl/curl.h>

void Get_Link(FILE* arq){

    int c;
    int num_links = 0;
    int contador = 0;
    char* link = (char*)malloc(1*sizeof(char));
    c = fgetc(arq);

    if(c == '"'){
        while((c = fgetc(arq)) != '"'){
            link[contador] = c;
            contador++;
            link = (char*)realloc(link,(contador+1)*sizeof(char));
        }
    }

    printf("%s\n", link);
    free(link);
    num_links++;
}

int main()
{


    CURL *section = curl_easy_init();
    char Links[] = "<li><a href="; // aqui ele procura as tag's dentro de outras tags
    int pos = 0;
    int encontrei = 0;
    int caractere;

    curl_easy_setopt(section, CURLOPT_URL, "http://pt.wikipedia.org/wiki/Categoria:Armas_japonesas_antigas");
    FILE * arquivo = fopen("arq.txt", "w");

    curl_easy_setopt(section,  CURLOPT_WRITEDATA, arquivo);
    curl_easy_setopt(section,  CURLOPT_WRITEFUNCTION, fwrite);
    curl_easy_perform(section);

    //depois de entrar numa pagina de arma e salvar o conteudo, verificar uma proxima arma. com o "Brain"
    //após isso verifica com a url do google, www.google.com/pesquisa=excalibur, se no texto salvo dessa pagina
    //adquirir a palavra "espada" "sword" "lenda", valida a referencia, e passa para a proxima.

    fclose(arquivo);
    curl_easy_cleanup(section);
    fopen("arq.txt", "r");

    if(arquivo != NULL){

        while((caractere = fgetc(arquivo)) != EOF){

            if((caractere == Links[0]) && (!encontrei)){
                encontrei = 1; // podemos continuar a pesquisa a partir daqui
                pos = 0;
            }

            if(encontrei){

                if(caractere == Links[pos]){
                    encontrei = 1; // continua a pesquisa
                    pos++; // incrementa a posição de busca

                    if(pos == strlen(Links)){
                        //achou os outros links
                        Get_Link(arquivo);
                    }

                }
                else
                  encontrei = 0;
            }

        }

        fclose(arquivo);
    }

    return 0;

}
