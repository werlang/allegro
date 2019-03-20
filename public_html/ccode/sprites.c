#include <allegro5/allegro.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_image.h>

#define FPS 60.0
#define LARGURA_TELA 800
#define ALTURA_TELA 380

ALLEGRO_DISPLAY *janela = NULL;
ALLEGRO_EVENT_QUEUE *fila_eventos = NULL;
ALLEGRO_TIMER *timer = NULL;
ALLEGRO_BITMAP *folha_sprite = NULL;
ALLEGRO_BITMAP *fundo = NULL;

void error_msg(char *text){
	al_show_native_message_box(janela,"ERRO",
		"Ocorreu o seguinte erro e o programa sera finalizado:",
		text,NULL,ALLEGRO_MESSAGEBOX_ERROR);
}

int inicializar(){
    if (!al_init()){
        error_msg("Falha ao inicializar a Allegro");
        return 0;
    }

    if (!al_init_image_addon()){
        error_msg("Falha ao inicializar o addon de imagens");
        return 0;
    }

    timer = al_create_timer(1.0 / FPS);
    if(!timer) {
        error_msg("Falha ao criar temporizador");
        return 0;
    }

    janela = al_create_display(LARGURA_TELA, ALTURA_TELA);
    if(!janela) {
        error_msg("Falha ao criar janela");
        al_destroy_timer(timer);
        return 0;
    }
    al_set_window_title(janela, "Sprites");


    fila_eventos = al_create_event_queue();
    if(!fila_eventos) {
        error_msg("Falha ao criar fila de eventos");
        al_destroy_timer(timer);
        al_destroy_display(janela);
        return 0;
    }

    //carrega a folha de sprites na variavel
    folha_sprite = al_load_bitmap("run2.bmp");
    if (!folha_sprite){
        error_msg("Falha ao carregar sprites");
        al_destroy_timer(timer);
        al_destroy_display(janela);
        al_destroy_event_queue(fila_eventos);
        return 0;
    }
    //usa a cor rosa como transparencia
    al_convert_mask_to_alpha(folha_sprite,al_map_rgb(255,0,255));

    //carrega o fundo
    fundo = al_load_bitmap("background.png");
    if (!fundo){
        error_msg("Falha ao carregar fundo");
        al_destroy_timer(timer);
        al_destroy_display(janela);
        al_destroy_event_queue(fila_eventos);
        al_destroy_bitmap(folha_sprite);
        return 0;
    }

    al_register_event_source(fila_eventos, al_get_display_event_source(janela));
    al_register_event_source(fila_eventos, al_get_timer_event_source(timer));
    al_start_timer(timer);

    return 1;
}

int main(void){
    int desenha = 1;
    int sair = 0;

    //largura e altura de cada sprite dentro da folha
    int altura_sprite=140, largura_sprite=108;
    //quantos sprites tem em cada linha da folha, e a atualmente mostrada
    int colunas_folha=4, coluna_atual=0;
    //quantos sprites tem em cada coluna da folha, e a atualmente mostrada
    int linha_atual=0, linhas_folha=2;
    //posicoes X e Y da folha de sprites que serao mostradas na tela
    int regiao_x_folha=0, regiao_y_folha=0;
    //quantos frames devem se passar para atualizar para o proximo sprite
    int frames_sprite=6, cont_frames=0;
    //posicao X Y da janela em que sera mostrado o sprite
    int pos_x_sprite=50, pos_y_sprite=150;
    //velocidade X Y que o sprite ira se mover pela janela
    int vel_x_sprite=4, vel_y_sprite=0;

    if (!inicializar()){
        return -1;
    }

    while(!sair){
        ALLEGRO_EVENT evento;
        al_wait_for_event(fila_eventos, &evento);

        /* -- EVENTOS -- */
        if(evento.type == ALLEGRO_EVENT_TIMER){
            //a cada disparo do timer, incrementa cont_frames
            cont_frames++;
            //se alcancou a quantidade de frames que precisa passar para mudar para o proximo sprite
            if (cont_frames >= frames_sprite){
                //reseta cont_frames
                cont_frames=0;
                //incrementa a coluna atual, para mostrar o proximo sprite
                coluna_atual++;
                //se coluna atual passou da ultima coluna
                if (coluna_atual >= colunas_folha){
                    //volta pra coluna inicial
                    coluna_atual=0;
                    //incrementa a linha, se passar da ultima, volta pra primeira
                    linha_atual = (linha_atual+1) % linhas_folha;
                    //calcula a posicao Y da folha que sera mostrada
                    regiao_y_folha = linha_atual * altura_sprite;
                }
                //calcula a regiao X da folha que sera mostrada
                regiao_x_folha = coluna_atual * largura_sprite;
            }
            //se o sprite estiver perto da borda direita ou esquerda da tela
            if ( pos_x_sprite + largura_sprite > LARGURA_TELA - 20 || pos_x_sprite < 20 ){
                //inverte o sentido da velocidade X, para andar no outro sentido
                vel_x_sprite = -vel_x_sprite;
            }

            //atualiza as posicoes X Y do sprite de acordo com a velocidade, positiva ou negativa
            pos_x_sprite += vel_x_sprite;
            pos_y_sprite += vel_y_sprite;

            desenha=1;
        }
        else if(evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            sair = 1;
        }

        /* -- ATUALIZA TELA -- */
        if(desenha && al_is_event_queue_empty(fila_eventos)) {

            //desenha o fundo na tela
            al_draw_bitmap_region(fundo,0,0,LARGURA_TELA,ALTURA_TELA,0,0,0);

            //velocidade positiva (movendo para direita)
            if (vel_x_sprite > 0)
                //desenha sprite na posicao X Y da janela, a partir da regiao X Y da folha
                al_draw_bitmap_region(folha_sprite,
                    regiao_x_folha,regiao_y_folha,
                    largura_sprite,altura_sprite,
                    pos_x_sprite,pos_y_sprite,0);
            else
                //desenha sprite, igual acima, com a excecao que desenha a largura negativa, ou seja, espelhado horizontalmente
                al_draw_scaled_bitmap(folha_sprite,
                    regiao_x_folha,regiao_y_folha,
                    largura_sprite,altura_sprite,
                    pos_x_sprite+largura_sprite,pos_y_sprite,
                    -largura_sprite,altura_sprite,0);

            al_flip_display();

            desenha = 0;
        }
    }

    al_destroy_bitmap(folha_sprite);
    al_destroy_bitmap(fundo);
    al_destroy_timer(timer);
    al_destroy_display(janela);
    al_destroy_event_queue(fila_eventos);

    return 0;
}
