// Os arquivos de cabeçalho
#include <allegro5/allegro.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

// Atributos da tela
#define LARGURA_TELA 640
#define ALTURA_TELA 480

//função que mostra mensagem de erro em uma caixa de texto nativa
//a função recebe a string da mensagem como parâmetro
void error_msg(char *text){
    al_show_native_message_box(NULL,"ERRO",
        "Ocorreu o seguinte erro e o programa sera finalizado:",
        text,NULL,ALLEGRO_MESSAGEBOX_ERROR);
}

int main(void){
    // A nossa janela
    ALLEGRO_DISPLAY *janela = NULL;

    // O nosso arquivo de fonte
    ALLEGRO_FONT *fonte = NULL;

    //tenta inciar a biblioteca, caso retorne false, mostra mensagem e encerra o programa
    if (!al_init()){
        error_msg("Falha ao inicializar a Allegro");
        return -1;
    }

    // Inicialização do add-on para uso de fontes
    al_init_font_addon();

    // Inicialização do add-on para uso de fontes True Type
    if (!al_init_ttf_addon()){
        error_msg("Falha ao inicializar add-on allegro_ttf");
        return -1;
    }

    // Criação da nossa janela
    janela = al_create_display(LARGURA_TELA, ALTURA_TELA);
    if (!janela){
        error_msg("Falha ao criar janela");
        return -1;
    }

    // Carregando o arquivo de fonte
    fonte = al_load_font("arial.ttf", 48, 0);
    if (!fonte){
        al_destroy_display(janela);
        error_msg("Falha ao carregar fonte");
        return -1;
    }

    // Preenchemos a tela com a cor branca
    al_clear_to_color(al_map_rgb(255, 255, 255));

    // Texto alinhado à esquerda
    al_draw_text(fonte, al_map_rgb(255, 0, 0), 10, 10, ALLEGRO_ALIGN_LEFT, "Esquerda");

    // Texto alinhado à direita
    al_draw_text(fonte, al_map_rgb(0, 0, 255), LARGURA_TELA - 10, 50, ALLEGRO_ALIGN_RIGHT, "Direita");

    // Texto centralizado
    al_draw_text(fonte, al_map_rgb(0, 255, 0), LARGURA_TELA / 2, 90, ALLEGRO_ALIGN_CENTRE, "Centro");

    // Exemplo de impressão de valores variáveis
    int i = 4;
    char texto[50] = "formatado";
    al_draw_textf(fonte, al_map_rgb(0, 0, 0), LARGURA_TELA / 2, 250, ALLEGRO_ALIGN_CENTRE, "Texto %d: %s", i, texto);

    // Atualizamos a tela
    al_flip_display();

    // E aguardamos 10 segundos
    al_rest(10.0);

    // Desalocação da fonte e da janela
    al_destroy_font(fonte);
    al_destroy_display(janela);

    return 0;
}
