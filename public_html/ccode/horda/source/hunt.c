#include <allegro5/allegro.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <string.h>

#define FPS 60.0q
#define TILE_SIZE 32
#define TECLA_W 0
#define TECLA_S 1
#define TECLA_A 2
#define TECLA_D 3
#define COORD_X 0
#define COORD_Y 1
#define MAX_FLECHAS 20
#define MAX_INIMIGOS 200
#define VEL_FLECHA 20
#define DANO_FLECHA 80.0
#define FORCA_INIMIGO 20
#define ESTADO_PRE_MENU 0
#define ESTADO_MENU 1
#define ESTADO_PRE_JOGO 2
#define ESTADO_JOGO 3
#define ESTADO_PRE_SCORE 4
#define ESTADO_SCORE 5
#define ESTADO_FIM 6

struct sprite {
    ALLEGRO_BITMAP *folha_sprite;
    int largura_sprite, altura_sprite;
    int largura_hitbox, altura_hitbox;
    int regiao_x_folha, regiao_y_folha;
    int frames_sprite, cont_frames;
    int colunas_sprite, coluna_atual;
    int linhas_sprite, linha_atual;
    float pos_x, pos_y;
    float velocidade;
    int inverte_sprite;
    float angulo;
    int vida;
};

struct scores {
    char nome[10];
    int pontos;
};

ALLEGRO_DISPLAY *janela = NULL;
ALLEGRO_EVENT_QUEUE *fila_eventos = NULL;
ALLEGRO_TIMER *timer = NULL;
ALLEGRO_BITMAP *buffer_janela = NULL;
ALLEGRO_BITMAP *gui = NULL;
ALLEGRO_BITMAP *barra_vida = NULL;
ALLEGRO_BITMAP *barra_mira = NULL;
ALLEGRO_BITMAP *fundo = NULL;
ALLEGRO_BITMAP *orc = NULL;
ALLEGRO_BITMAP *archer = NULL;
ALLEGRO_BITMAP *animacao_inicial;
ALLEGRO_BITMAP *flecha_titulo;
ALLEGRO_FONT *titulo = NULL;
ALLEGRO_FONT *fonte = NULL;
ALLEGRO_SAMPLE *som_morte=NULL;
ALLEGRO_SAMPLE *som_acerta_machado=NULL;
ALLEGRO_SAMPLE *som_acerta_flecha=NULL;
ALLEGRO_SAMPLE *som_erra_flecha=NULL;
ALLEGRO_SAMPLE *som_atira_flecha=NULL;
ALLEGRO_AUDIO_STREAM *musica = NULL;
ALLEGRO_COLOR cor_titulo;

int tbm_pressionado = 0; //tempo que o botao do mouse esta presionado
int inimigo_spawn_time = 150; //tempo que um novo inimigo aparece
float taxa_aumento_spawn = 0.95; //taxa que reduz tempo de respawn do inimigo
int cont_tempo=0;
int mortes = 0; //pontos do jogo
int largura_tela, altura_tela; //dimensoes da tela do jogo
int largura_janela, altura_janela; //dimensoes da janela criada pelo programa
float ratio_x, ratio_y; //proporcao entre janela/tela
float dpi_scale; //calcula proporcao entre resolucao obtida e pixels da tela
float ratio_tela, tamanho_letterbox; //proporcao entre janela e tela do jogo, e tamanho das barras pretas em cima e em baixo.
int tecla_direcao[4]={0,0,0,0}; //vetor que guarda quando cada uma das 4 direcoes foi pressionada
int estado = ESTADO_PRE_MENU; //estado do jogo. cada etapa do jogo significa um estado
int desenha=1;
struct sprite heroi; //heroi
struct sprite inimigos[MAX_INIMIGOS]; //orcs
struct sprite flechas[MAX_FLECHAS]; //flechas
int nflechas=0, iflechas=0; //numero de flechas preenchidas no vetor, e posicao da ultima flechas
int ninimigos=0, iinimigos=0; //numero de ocs iniciados no vetor, e posicao do ultimo orc criado
struct scores top[10]; //guarda os top 10 scores que puxa do arquivo


//graus para radianos
float g_to_r(float ang){
//180 x = pi ang
    return M_PI * ang / 180;
}

//radianos para graus
float r_to_g(float rad){
//180 rad = pi x
    return rad * 180 / M_PI;
}

/*
    encontra dados triginometricos. considerando um triangulo retangulo,recebe:
    catetos x e y, hipotenusa, angulo entre y e hipotenusa.
    recebe tudo por referencia. 2 variavel precisam ter valor 0, dai a funcao calcula e preenche
*/
void encontra_trig(float *x, float *y, float *hip, float *ang){
    if (*x!=0 && *y!=0){
        if (hip != NULL)
            *hip = sqrt( pow(*x,2) + pow(*y,2) );
        if (ang != NULL)
            *ang = r_to_g(atan((*x)/(*y)));
        return;
    }
    if(*x!=0 && *hip!=0){
        if (y != NULL)
            *y = sqrt( pow(*hip,2) - pow(*x,2) );
        if (ang != NULL)
            *ang = r_to_g(asin((*x)/(*hip)));
        return;
    }
    if (*x!=0 && *ang!=0){
        if (y != NULL)
            *y = (*x) / tan(g_to_r(*ang));
        if (hip != NULL)
            *hip = *x / sin(g_to_r(*ang));
        return;
    }
    if(*y!=0 && *hip!=0){
        if (x != NULL)
            *x = sqrt( pow(*hip,2) - pow(*y,2) );
        if (ang != NULL)
            *ang = r_to_g(acos((*y)/(*hip)));
        return;
    }
    if (*y!=0 && *ang!=0){
        if (x != NULL)
            *x = (*y) * tan(g_to_r(*ang));
        if (hip != NULL)
            *hip = *y / cos(g_to_r(*ang));
        return;
    }
    if (*hip!=0 && *ang!=0){
        if (x != NULL)
            *x = sin(g_to_r(*ang)) * (*hip);
        if (y != NULL)
            *y = cos(g_to_r(*ang)) * (*hip);
        return;
    }
}

//simplificacao do encontra_trig para achar somente angulo baseado em dois pontos cartesianos
float calcula_angulo(float xa, float ya, float xb, float yb){
    float dx = xb-xa;
    float dy = yb-ya;
    float hip=0, ang=0;
    encontra_trig(&dx, &dy, &hip, &ang);
    if (dy<0)
        ang+=180;
    else if (dx<0 && dy>0)
        ang+=360;
    return ang;
}

//simplificacao do encontra_trig para achar hipotenusa baseado em dois pontos cartesianos
float calcula_distancia(float xa, float ya, float xb, float yb){
    float dx = xb-xa;
    float dy = yb-ya;
    float hip=0, ang=0;
    encontra_trig(&dx, &dy, &hip, &ang);
    return hip;
}

//verifica se os hitboxes do sprite a estao colidindo com b
int hitbox_colidindo(struct sprite *a, struct sprite *b){
    float centro_a_x = a->pos_x + a->largura_sprite/2;
    float centro_a_y = a->pos_y + a->altura_sprite/2;
    float centro_b_x = b->pos_x + b->largura_sprite/2;
    float centro_b_y = b->pos_y + b->altura_sprite/2;

    float direita_a = centro_a_x + a->largura_hitbox/2;
    float esquerda_a = centro_a_x - a->largura_hitbox/2;
    float baixo_a = centro_a_y + a->altura_hitbox/2;
    float cima_a = centro_a_y - a->altura_hitbox/2;

    float direita_b = centro_b_x + b->largura_hitbox/2;
    float esquerda_b = centro_b_x - b->largura_hitbox/2;
    float baixo_b = centro_b_y + b->altura_hitbox/2;
    float cima_b = centro_b_y - b->altura_hitbox/2;

    if ((direita_a > esquerda_b && direita_a < direita_b) || (esquerda_a > esquerda_b && esquerda_a < direita_b)){
        if ((cima_a < baixo_b && cima_a > cima_b) || (baixo_a > cima_b && baixo_a < baixo_b))
            return 1;
    }
    return 0;
}

void destroy(){
    if (janela)
        al_destroy_display(janela);
    if (fonte)
        al_destroy_font(fonte);
    if (titulo)
        al_destroy_font(titulo);
    if (timer)
        al_destroy_timer(timer);
    if (fundo)
        al_destroy_bitmap(fundo);
    if (gui)
        al_destroy_bitmap(gui);
    if (barra_mira)
        al_destroy_bitmap(barra_mira);
    if (barra_vida)
        al_destroy_bitmap(barra_vida);
    if (flecha_titulo)
        al_destroy_bitmap(flecha_titulo);
    if (orc)
        al_destroy_bitmap(orc);
    if (archer)
        al_destroy_bitmap(archer);
    if (fila_eventos)
        al_destroy_event_queue(fila_eventos);
    if (som_morte)
        al_destroy_sample(som_morte);
    if (som_atira_flecha)
        al_destroy_sample(som_atira_flecha);
    if (som_acerta_flecha)
        al_destroy_sample(som_acerta_flecha);
    if (som_acerta_machado)
        al_destroy_sample(som_acerta_machado);
    if (som_erra_flecha)
        al_destroy_sample(som_erra_flecha);
    if (musica)
        al_destroy_audio_stream(musica);
}

void error_msg(char *text){
    if (janela)
        al_show_native_message_box(janela,"ERRO",
            "Ocorreu o seguinte erro e o programa sera finalizado:",
            text,NULL,ALLEGRO_MESSAGEBOX_ERROR);
    else
        al_show_native_message_box(NULL,"ERRO",
            "Ocorreu o seguinte erro e o programa sera finalizado:",
            text,NULL,ALLEGRO_MESSAGEBOX_ERROR);
}

int inicializar(){
    if (!al_init()){
        error_msg("Falha ao inicializar a Allegro");
        return 0;
    }

    al_set_new_display_flags(ALLEGRO_FULLSCREEN_WINDOW); //seta tela para fullscreen
    janela = al_create_display(0, 0); //cria display. o tamanho vai ser automatico
    if(!janela) {
        error_msg("Falha ao criar janela");
        destroy();
        return 0;
    }
    al_set_window_title(janela, "Horda Orc");
    largura_janela = al_get_display_width(janela); //captura tamanho da janela criada
    altura_janela = al_get_display_height(janela);

    largura_tela = 1067; //tamanho da tela do jogo. independe da janela criada
    altura_tela = 600;
    ratio_tela = (float)largura_tela/altura_tela; //proporcao entre largura/altura da tela do jogo
    //caso aspect ratio do jogo para o monitor seja diferente, calcula tamanho da faixa preta
    tamanho_letterbox = altura_janela - largura_janela/ratio_tela;
    altura_janela = largura_janela / ratio_tela;
    ratio_x = (float)largura_tela/largura_janela;
    ratio_y = (float)altura_tela/altura_janela;
    buffer_janela = al_create_bitmap(largura_tela, altura_tela); //buffer que seja escalado de acordo com o tamanho da janela
    dpi_scale=0;

    al_init_primitives_addon();

    if (!al_init_image_addon()){
        error_msg("Falha ao inicializar o addon de imagens");
        destroy();
        return 0;
    }

    fundo = al_load_bitmap("img/tile.png");
    if (!fundo){
        error_msg("Falha ao carregar o fundo");
        destroy();
        return 0;
    }

    orc = al_load_bitmap("img/orc_spritesheet.png");
    archer = al_load_bitmap("img/archer_spritesheet.png");
    if (!orc || !archer){
        error_msg("Falha ao carregar folha de sprites");
        destroy();
        return 0;
    }

    flecha_titulo = al_load_bitmap("img/arrow.png");
    gui = al_load_bitmap("img/gui.png");
    barra_vida = al_load_bitmap("img/red_bar.png");
    barra_mira = al_load_bitmap("img/yellow_bar.png");
    if (!flecha_titulo || !gui || !barra_mira || !barra_vida){
        error_msg("Falha ao carregar imagem");
        destroy();
        return 0;
    }

    al_init_font_addon();
    if (!al_init_ttf_addon()){
        error_msg("Falha ao inicializar add-on allegro_ttf");
        destroy();
        return 0;
    }

    fonte = al_load_font("manaspc.ttf", 32, 0);
    titulo = al_load_font("manaspc.ttf", 56, 0);
    if (!fonte || !titulo){
        error_msg("Falha ao carregar fonte");
        destroy();
        return 0;
    }

    if (!al_install_keyboard()){
        error_msg("Falha ao inicializar o teclado");
        destroy();
        return 0;
    }

    if (!al_install_mouse()){
        error_msg("Falha ao inicializar o mouse");
        destroy();
        return 0;
    }

    fila_eventos = al_create_event_queue();
    if(!fila_eventos) {
        error_msg("Falha ao criar fila de eventos");
        destroy();
        return 0;
    }

    timer = al_create_timer(1.0 / FPS);
    if(!timer) {
        error_msg("Falha ao criar temporizador");
        destroy();
        return 0;
    }

    if(!al_install_audio()){
        error_msg("Falha ao inicializar o audio");
        destroy();
        return 0;
    }

    if(!al_init_acodec_addon()){
        error_msg("Falha ao inicializar o codec de audio");
        destroy();
        return 0;
    }

    if (!al_reserve_samples(64)){
        error_msg("Falha ao reservar amostras de audio");
        destroy();
        return 0;
    }

    som_morte = al_load_sample( "sounds/orc_death.ogg" );
    som_atira_flecha = al_load_sample( "sounds/arrow_fire.wav" );
    som_acerta_flecha = al_load_sample( "sounds/arrow_hit.ogg" );
    som_erra_flecha = al_load_sample( "sounds/arrow_miss.ogg" );
    som_acerta_machado = al_load_sample( "sounds/axe_hit.ogg" );
    musica = al_load_audio_stream("sounds/musica.ogg", 4, 1024);
    if (!som_morte || !som_atira_flecha || !som_acerta_flecha || !som_erra_flecha || !som_acerta_machado || !musica){
        error_msg( "Audio nao carregado" );
        destroy();
        return 0;
    }
    al_attach_audio_stream_to_mixer(musica, al_get_default_mixer());
    al_set_audio_stream_playmode(musica, ALLEGRO_PLAYMODE_LOOP);
    al_set_audio_stream_gain(musica, 0.6);


    al_register_event_source(fila_eventos, al_get_display_event_source(janela));
    al_register_event_source(fila_eventos, al_get_mouse_event_source());
    al_register_event_source(fila_eventos, al_get_keyboard_event_source());
    al_register_event_source(fila_eventos, al_get_timer_event_source(timer));
    al_start_timer(timer);

    return 1;
}

int init_heroi(struct sprite *h){
    h->folha_sprite = archer;
    h->largura_sprite = 50;
    h->altura_sprite = 50;
    h->largura_hitbox = 30;
    h->altura_hitbox = 35;
    h->colunas_sprite = 5;
    h->coluna_atual = 0;
    h->linhas_sprite = 6;
    h->linha_atual = 2;
    h->frames_sprite = 8;
    h->cont_frames = 0;
    h->pos_x = largura_tela/2;
    h->pos_y = altura_tela/2;
    h->velocidade = 2;
    h->regiao_x_folha = 0;
    h->regiao_y_folha = 0;
    h->inverte_sprite = 0;
    h->angulo = 0;
    h->vida = 100;

    return 1;
}

int init_inimigo(struct sprite *i){
    i->folha_sprite = orc;
    i->largura_sprite = 75;
    i->altura_sprite = 75;
    i->largura_hitbox = 25;
    i->altura_hitbox = 35;
    i->colunas_sprite = 5;
    i->coluna_atual = 0;
    i->linhas_sprite = 6;
    i->linha_atual = 0;
    i->frames_sprite = 8;
    i->cont_frames = 0;
    i->velocidade = 1;
    i->regiao_x_folha = 0;
    i->regiao_y_folha = 0;
    i->inverte_sprite = 0;
    i->angulo = 0;
    i->vida = 100;

    //posicao aleatoria nas bordas da tela
    if (rand()%2){
        i->pos_x = rand()%largura_tela;
        if (rand()%2)
            i->pos_y = 50;
        else
            i->pos_y = altura_tela-50;
    }
    else{
        i->pos_y = rand()%altura_tela;
        if (rand()%2)
            i->pos_x = 50;
        else
            i->pos_x = largura_tela-50;
    }

    return 1;
}

int init_flecha(struct sprite *f, struct sprite *h){
    ALLEGRO_BITMAP *temp = NULL;
    temp = al_load_bitmap("img/archer_spritesheet.png");
    if (!temp){
        error_msg("Falha ao carregar o sprite archer_spritesheet.png");
        destroy();
        return 0;
    }

    f->largura_sprite = 50;
    f->altura_sprite = 50;
    f->largura_hitbox = 5;
    f->altura_hitbox = 30;
    f->colunas_sprite = 7;
    f->coluna_atual = 6;
    f->linhas_sprite = 6;
    f->linha_atual = 5;
    f->frames_sprite = 0;
    f->cont_frames = 0;
    f->inverte_sprite = 0;

    f->pos_x = h->pos_x + h->largura_sprite/2; //flecha inicia no centro do sprite do heroi
    f->pos_y = h->pos_y + h->altura_sprite/2;

    int max_mira = 30; //alcance maximo da flecha em frames
    if (tbm_pressionado < 10) //pressionou muito pouco,
        f->vida = 0;
    else if (tbm_pressionado < 100) //calcula alcance da flecha por uma equacao de 1o grau
        f->vida = 5 + ((max_mira-5)/100.0) * tbm_pressionado;
    else
        f->vida = max_mira;

    f->angulo = h->angulo; //angulo da flecha pega angulo que o heroi tava mirando na hora do disparo
    f->velocidade = VEL_FLECHA;

    f->folha_sprite = al_create_bitmap(f->largura_sprite, f->altura_sprite);
    if (!f->folha_sprite){
        error_msg("Falha ao criar bitmap");
        destroy();
        al_destroy_bitmap(temp);
        return 0;
    }

    f->regiao_x_folha = f->coluna_atual * f->largura_sprite;
    f->regiao_y_folha = f->linha_atual * f->altura_sprite;

    al_set_target_bitmap(f->folha_sprite);
    al_draw_bitmap_region(temp, f->regiao_x_folha, f->regiao_y_folha, f->largura_sprite, f->altura_sprite, 0, 0, 0);
    al_set_target_bitmap(al_get_backbuffer(janela));
    al_destroy_bitmap(temp);

    return 1;
}

void atualiza_heroi(struct sprite *h){
    //calcula direcoes x e y do heroi
    int dir_x = tecla_direcao[TECLA_A] + tecla_direcao[TECLA_D];
    int dir_y = tecla_direcao[TECLA_W] + tecla_direcao[TECLA_S];
    h->cont_frames++;
    if (h->cont_frames >= h->frames_sprite){
        h->cont_frames=0;

        //se tiver vivo
        if (h->vida > 0){
            if (tbm_pressionado>0) //se tiver pressionando o mouse
                h->coluna_atual = 5;
            else if (h->coluna_atual==5) //soltou o botao
                h->coluna_atual=6;
            else if (h->coluna_atual==6) //volta pro inicio da animacao
                h->coluna_atual=0;
            else if (dir_x==0 && dir_y==0) //se nao tiver nenhuma tecla de direcao pressionada
                h->coluna_atual=0;
            else
                h->coluna_atual = (h->coluna_atual + 1) % h->colunas_sprite; //proximo sprite
        }
        else { //morto
            if (h->linha_atual >= 0 && h->linha_atual <= 1) //animacao de morte
                h->coluna_atual = 0;
            if (h->linha_atual >= 2 && h->linha_atual <= 4)
                h->coluna_atual = 3;

            h->linha_atual = 5;
            h->colunas_sprite = 3;

            if (h->coluna_atual != 2 && h->coluna_atual != 5) //nao terminou animacao de morte
                h->coluna_atual = (h->coluna_atual + 1) % h->colunas_sprite; //passa pra proxima
        }


        h->regiao_x_folha = h->coluna_atual * h->largura_sprite;

        //se tiver vivo e pressionando o botao, verifica angulo do mouse para escolher o sprite adequado
        if (h->vida > 0){
            if (tbm_pressionado){
                if (h->angulo >= 337 || h->angulo < 22 ){
                    h->linha_atual = 4;
                    h->inverte_sprite = 0;
                }
                if (h->angulo >= 22 && h->angulo < 67){
                    h->linha_atual = 3;
                    h->inverte_sprite = 0;
                }
                if (h->angulo >= 67 && h->angulo < 112){
                    h->linha_atual = 2;
                    h->inverte_sprite = 0;
                }
                if (h->angulo >= 112 && h->angulo < 157){
                    h->linha_atual = 1;
                    h->inverte_sprite = 0;
                }
                if (h->angulo >= 157 && h->angulo < 202){
                    h->linha_atual = 0;
                    h->inverte_sprite = 0;
                }
                if (h->angulo >= 202 && h->angulo < 247){
                    h->linha_atual = 1;
                    h->inverte_sprite = 1;
                }
                if (h->angulo >= 247 && h->angulo < 292){
                    h->linha_atual = 2;
                    h->inverte_sprite = 1;
                }
                if (h->angulo >= 292 && h->angulo < 337){
                    h->linha_atual = 3;
                    h->inverte_sprite = 1;
                }
            }
            else{ //nao tem botao pressionado, verifica direcao para escolher sprite
                if (dir_x==0 && dir_y==-1){
                    h->linha_atual = 0;
                    h->inverte_sprite = 0;
                }
                if (dir_x==1 && dir_y==-1){
                    h->linha_atual = 1;
                    h->inverte_sprite = 0;
                }
                if (dir_x==1 && dir_y==0){
                    h->linha_atual = 2;
                    h->inverte_sprite = 0;
                }
                if (dir_x==1 && dir_y==1){
                    h->linha_atual = 3;
                    h->inverte_sprite = 0;
                }
                if (dir_x==0 && dir_y==1){
                    h->linha_atual = 4;
                    h->inverte_sprite = 0;
                }
                if (dir_x==-1 && dir_y==1){
                    h->linha_atual = 3;
                    h->inverte_sprite = 1;
                }
                if (dir_x==-1 && dir_y==0){
                    h->linha_atual = 2;
                    h->inverte_sprite = 1;
                }
                if (dir_x==-1 && dir_y==-1){
                    h->linha_atual = 1;
                    h->inverte_sprite = 1;
                }
            }
        }

    }
    h->regiao_y_folha = h->linha_atual * h->altura_sprite;

    if (h->vida > 0){
        float velocidade_mirando = 1;
        if (tbm_pressionado){
            velocidade_mirando = 0;
        }

        float c_x=0, c_y=0, ang=45, vel=1;
        //se tiver andando diagonal, calcula componente x e y baseado na velocidade
        if (dir_x!=0 && dir_y!=0){
            encontra_trig(&c_x, &c_y, &vel, &ang);
        }
        else{
            c_x = 1;
            c_y = 1;
        }

        h->pos_x += h->velocidade * dir_x * c_x * velocidade_mirando;
        h->pos_y += h->velocidade * dir_y * c_y * velocidade_mirando;

        //nao deixa passar das bordas da tela
        if (h->pos_x < 0)
            h->pos_x = 0;
        if (h->pos_x > largura_tela - h->largura_sprite)
            h->pos_x = largura_tela - h->largura_sprite;
        if (h->pos_y < 0)
            h->pos_y = 0;
        if (h->pos_y > altura_tela - h->altura_sprite)
            h->pos_y = altura_tela - h->altura_sprite;
    }
}

void atualiza_inimigo(struct sprite *i, struct sprite *h){
    //ta vivo ou em animacao de morte
    if ((i->vida > 0) || ( i->linha_atual != 5) || (i->coluna_atual != 2 && i->coluna_atual != 5)){
        float dx = h->pos_x - i->pos_x;
        float dy = h->pos_y - i->pos_y;
        if (dx==0) dx=1;
        if (dy==0) dy=1;

        float hip=0, ang=0;
        encontra_trig(&dx, &dy, &hip, &ang);
        float razao = i->velocidade / hip;
        float vel_x = razao * dx;
        float vel_y = razao * dy;

        //angulo do inimigo em relacao ao heroi
        i->angulo = calcula_angulo(i->pos_x, i->pos_y, h->pos_x, h->pos_y);

        i->cont_frames++;
        if (i->cont_frames >= i->frames_sprite){
            i->cont_frames=0;

            //linha da animacao, baseado no angulo
            if (i->vida > 0){
                if (i->angulo >= 337 || i->angulo < 22 ){
                    i->linha_atual = 4;
                    i->inverte_sprite = 0;
                }
                if (i->angulo >= 22 && i->angulo < 67){
                    i->linha_atual = 3;
                    i->inverte_sprite = 0;
                }
                if (i->angulo >= 67 && i->angulo < 112){
                    i->linha_atual = 2;
                    i->inverte_sprite = 0;
                }
                if (i->angulo >= 112 && i->angulo < 157){
                    i->linha_atual = 1;
                    i->inverte_sprite = 0;
                }
                if (i->angulo >= 157 && i->angulo < 202){
                    i->linha_atual = 0;
                    i->inverte_sprite = 0;
                }
                if (i->angulo >= 202 && i->angulo < 247){
                    i->linha_atual = 1;
                    i->inverte_sprite = 1;
                }
                if (i->angulo >= 247 && i->angulo < 292){
                    i->linha_atual = 2;
                    i->inverte_sprite = 1;
                }
                if (i->angulo >= 292 && i->angulo < 337){
                    i->linha_atual = 3;
                    i->inverte_sprite = 1;
                }
            }
            else {
                //morto, acha qual das duas animacoes de morte deve usar
                if (i->linha_atual >= 0 && i->linha_atual <= 1)
                    i->coluna_atual = 0;
                if (i->linha_atual >= 2 && i->linha_atual <= 4)
                    i->coluna_atual = 3;

                i->linha_atual = 5;
                i->colunas_sprite = 3;
            }

            i->coluna_atual++;
            //animacao de hit
            if (i->coluna_atual == 9){
                h->vida-=FORCA_INIMIGO;
                al_play_sample(som_acerta_machado, 1.0, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
            }
            //fim da coluna
            if (i->coluna_atual >= i->colunas_sprite ){
                i->coluna_atual = 0;
                i->colunas_sprite = 5;
            }
            i->regiao_x_folha = i->coluna_atual * i->largura_sprite;
            i->regiao_y_folha = i->linha_atual * i->altura_sprite;

        }

        if (dx < 0) dx = -dx;
        if (dy < 0) dy = -dy;
        if (hitbox_colidindo(i,h) && i->coluna_atual < 5){ //verifica se esta colidindo para entrar na animacao de hit
            i->coluna_atual = 5;
            i->colunas_sprite = 9;
        }

        //movimento
        if (i->vida > 0 && i->coluna_atual < 5){
            i->pos_y += vel_y;
            i->pos_x += vel_x;
            int j;
            float x=0,y=0,ang;
            //verifica se esta colidindo com outros orcs
            for (j=0 ; j<ninimigos ; j++){
                if (inimigos[j].vida > 0){
                    if (hitbox_colidindo(i, &inimigos[j])){
                        i->pos_y -= vel_y;
                        i->pos_x -= vel_x;
                        //calcula angulo entre os 2 colidindo, faz um vetor x y na direcao contraria, e anda sua velocidade nessa direcao
                        ang = calcula_angulo(i->pos_x, i->pos_y, inimigos[j].pos_x, inimigos[j].pos_y);
                        encontra_trig(&x, &y, &(i->velocidade), &ang);
                        i->pos_y -= y;
                        i->pos_x -= x;
                        break;
                    }
                }
            }
        }
    }
    else //se estiver morto, segue perdendo vida, para o corpo sumir
        i->vida--;
}

void atualiza_flechas(struct sprite flechas[], int n){
    int i;
    float vel_x=0, vel_y=0;
    for (i=0 ; i<n ; i++){
        flechas[i].vida--;
        if (flechas[i].vida > 0){
            //calcula vetores x y baseado na velocidade e angulo
            encontra_trig(&vel_x, &vel_y, &(flechas[i].velocidade), &(flechas[i].angulo));
            flechas[i].pos_y += vel_y;
            flechas[i].pos_x += vel_x;
        }
        if (flechas[i].vida == 0) //chegou alcancou distancia maxima
            al_play_sample(som_erra_flecha, 1.0, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
    }
}

int colisao_flecha(struct sprite flechas[], int nf, struct sprite inimigos[], int ni){
    int i,j;
    int ok_x=0, ok_y=0;
    float ponta_x, ponta_y;
    float vel_x=0, vel_y=0;
    for (i=0 ; i<nf ; i++){
        vel_x=0, vel_y=0;
        //calcula velocidade x y da flecha
        encontra_trig(&vel_x, &vel_y, &(flechas[i].velocidade), &(flechas[i].angulo));
        //calcula a coordenada da ponta de flecha
        ponta_x = (flechas[i].altura_hitbox * vel_x / VEL_FLECHA) + flechas[i].pos_x;
        ponta_y = (flechas[i].altura_hitbox * vel_y / VEL_FLECHA) + flechas[i].pos_y;
        for (j=0 ; j<ni ; j++){
            ok_x=0; ok_y=0;
            //calcula bordas esq/dir/cima/baixo do inimigo
            int h_x = inimigos[j].pos_x + inimigos[j].largura_sprite/2 - inimigos[j].largura_hitbox/2;
            int h_y = inimigos[j].pos_y + inimigos[j].altura_sprite/2 - inimigos[j].altura_hitbox/2;
            float direita_inimigo = h_x + inimigos[j].largura_hitbox;
            float esquerda_inimigo = h_x;
            float cima_inimigo = h_y;
            float baixo_inimigo = h_y + inimigos[j].altura_hitbox;

            //verifica se a ponta esta dentro do hitbox do inimigo
            if (ponta_x >= esquerda_inimigo && ponta_x <= direita_inimigo)
                ok_x = 1;
            if (ponta_y >= cima_inimigo && ponta_y <= baixo_inimigo)
                ok_y = 1;

            if (ok_x && ok_y && flechas[i].vida>0 && inimigos[j].vida>0){
                flechas[i].vida=-450;
                //tira vida do inimigo igual dano da flecha + 30% aleatorio
                inimigos[j].vida -= DANO_FLECHA + DANO_FLECHA * (rand()%31) / 100.0;
                al_play_sample(som_acerta_flecha, 1.0, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
                //se matou inimigo
                if (inimigos[j].vida <= 0){
                    mortes++;
                    al_play_sample(som_morte, 1.0, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
                    //a cada 5 mortes, diminui o tempo que um novo inimigo aparece
                    if (mortes%5 == 0)
                        inimigo_spawn_time *= taxa_aumento_spawn;
                }
                return 1;
            }
        }
    }
    return 0;
}

void desenha_flechas(struct sprite flechas[], int n, struct sprite *h){
    int i;
    for (i=0 ; i<n ; i++){
        //desenha sprite da flecha rotacionado para bater com o angulo de disparo
        if (flechas[i].vida > -500)
            al_draw_rotated_bitmap(flechas[i].folha_sprite, flechas[i].largura_sprite/2, flechas[i].altura_sprite/2, flechas[i].pos_x, flechas[i].pos_y, g_to_r(360-(flechas[i].angulo+180)), 0);
    }
}

int imenu=0, tam_menu=5;
//atribuicoes iniciais antes de mostrar o menu do jogo
void pre_menu(){
    animacao_inicial = al_create_bitmap(900,150); //cria animacao do menu
    init_heroi(&heroi);

    heroi.pos_x = al_get_bitmap_width(animacao_inicial)/2 - heroi.largura_sprite/2;
    heroi.pos_y = al_get_bitmap_height(animacao_inicial)/2 - heroi.altura_sprite/2;

    cor_titulo = al_map_rgb(rand()%256, rand()%256, rand()%256); //cor aleatoria do titulo do jogo

    estado++; //proximo estado
}

//dentro da animacao inicial, quando o heroi quer mirar uma flecha, -1 == esqueda, 1 == direita
void animacao_mira_flecha(int dir){
    tbm_pressionado++;
    if (dir == -1)
        heroi.angulo = 270;
    else
        heroi.angulo = 90;
}

//dentro da animacao inicial, quando o heroi atira flecha
void animacao_atira_flecha(){
    init_flecha(&flechas[0], &heroi);
    nflechas=1;
    tbm_pressionado=0;
    al_play_sample(som_atira_flecha, 1.0, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
}

//dentro da animacao inicial, quando um novo inimigo e criado
void animacao_cria_inimigo(int lado){
    init_inimigo(&inimigos[ninimigos]);
    inimigos[ninimigos].pos_y = al_get_bitmap_height(animacao_inicial)/2 - inimigos[0].altura_sprite/2;
    if (lado == -1)
        inimigos[ninimigos].pos_x = 10;
    else if (lado == 1)
        inimigos[ninimigos].pos_x = al_get_bitmap_width(animacao_inicial) - inimigos[ninimigos].largura_sprite - 10;
    ninimigos++;
}

//animacao inicial
void animacao_menu(){
    tecla_direcao[TECLA_A] = 0;
    tecla_direcao[TECLA_D] = 0;
    tecla_direcao[TECLA_W] = 0;
    tecla_direcao[TECLA_S] = 0;

    //abaixo segue o script da animacao. descreve o que acontece em cada tempo
    if (cont_tempo == 0){
        animacao_cria_inimigo(-1);
        animacao_cria_inimigo(1);
    }
    else if (cont_tempo < 70){
        tecla_direcao[TECLA_A] = -1;
    }
    else if (cont_tempo < 100){
        animacao_mira_flecha(-1);
        if (cont_tempo == 80){
            animacao_cria_inimigo(1);
        }
    }
    else if (cont_tempo==100){
        animacao_atira_flecha();
    }
    else if (cont_tempo < 130){
        animacao_mira_flecha(-1);
    }
    else if (cont_tempo==130){
        animacao_atira_flecha();
        animacao_cria_inimigo(1);
    }
    else if (cont_tempo < 200){
        animacao_mira_flecha(1);
    }
    else if (cont_tempo==200){
        animacao_atira_flecha();
    }
    else if (cont_tempo < 260){
        animacao_mira_flecha(1);
    }
    else if (cont_tempo==260){
        animacao_atira_flecha();
    }
    else if (cont_tempo < 340){
        tecla_direcao[TECLA_A] = -1;
    }
    else if (cont_tempo < 400){
        animacao_mira_flecha(1);
        if (cont_tempo == 360){
            animacao_cria_inimigo(1);
            animacao_cria_inimigo(-1);
        }
    }
    else if (cont_tempo==400){
        animacao_atira_flecha();
    }
    else if (cont_tempo < 450){ //foge
        tecla_direcao[TECLA_D] = 1;
    }
    else if (cont_tempo < 470){
        animacao_mira_flecha(-1);
    }
    else if (cont_tempo==470){
        animacao_atira_flecha();
    }
    else if (cont_tempo < 490){
        animacao_mira_flecha(-1);
    }
    else if (cont_tempo==490){
        animacao_atira_flecha();
    }
    else if (cont_tempo < 530){
        animacao_mira_flecha(1);
    }
    else if (cont_tempo==530){
        animacao_atira_flecha();
    }
    else if (cont_tempo < 560){
        animacao_mira_flecha(1);
    }
    else if (cont_tempo==560){
        animacao_atira_flecha();
    }
    else if (cont_tempo < 620){
        animacao_mira_flecha(1);
    }
    else if (cont_tempo==620){
        animacao_atira_flecha();
    }
    else if (cont_tempo < 700){
        animacao_mira_flecha(1);
    }
    else if (cont_tempo==700){
        animacao_atira_flecha();
        animacao_cria_inimigo(-1);
    }
    else if (cont_tempo < 750){
        tecla_direcao[TECLA_D] = 1;
    }
    else if (cont_tempo < 790){
        animacao_mira_flecha(1);
    }
    else if (cont_tempo==790){
        animacao_atira_flecha();
    }
    else if (cont_tempo < 1000){
        tecla_direcao[TECLA_D] = 1;
    }
    else if (cont_tempo < 1050){
        animacao_mira_flecha(-1);
    }
    else if (cont_tempo==1050){
        animacao_atira_flecha();
    }
    else if (cont_tempo < 1110){
        animacao_mira_flecha(-1);
    }
    else if (cont_tempo==1110){
        animacao_atira_flecha();
    }
    else if (heroi.pos_x > al_get_bitmap_width(animacao_inicial)/2 - heroi.largura_sprite/2){
        tecla_direcao[TECLA_A] = -1;
    }
    else{ //termina animacao, seta variaveis para voltar ao inicio da contagem de tempo, e resetar animacao
        cont_tempo=-1;
        ninimigos=0;
    }
}

float x,y, dpi_samples=0, xi, yi;
int pula_para_scores=0, mostra_instrucoes=0, mostra_creditos=0;
void menu(){
    int i,j;
    char op_menu[5][50] = {"INICIAR","INSTRUCOES","PONTUACOES","CREDITOS","SAIR"}; //texto das opcoes do menu
    float largura_flecha = 100; //flecha do menu, do ponteiro do mouse
    float altura_flecha = largura_flecha * al_get_bitmap_height(flecha_titulo)/al_get_bitmap_width(flecha_titulo);
    float y_flecha = al_get_font_ascent(fonte)/2 - altura_flecha/2;
    float x_flecha = al_get_text_width(fonte, op_menu[imenu])/2 + 20;
    //calcula o espaco y para os items do menu
    float y_menu = al_get_bitmap_height(animacao_inicial)+25 + 3 * (altura_tela - (al_get_bitmap_height(animacao_inicial)+25)) / (tam_menu+3);
    float x_menu = largura_tela/2;
    //quanto de espaco tem cada item
    float menu_inc = (altura_tela-y_menu)/tam_menu;
    ALLEGRO_COLOR cor_menu;
    ALLEGRO_EVENT evento;
    al_wait_for_event(fila_eventos, &evento);
    if(evento.type == ALLEGRO_EVENT_TIMER){
        for (i=0 ; i<ninimigos ; i++)
            atualiza_inimigo(&inimigos[i], &heroi); //atualiza os movimentos e animacoes de todos inimigos

        atualiza_flechas(flechas, nflechas); //e das flechas
        colisao_flecha(flechas, nflechas, inimigos, ninimigos); //verifica colisoes das flechas

        atualiza_heroi(&heroi); //atualiza movimento e animacao do heroi. tudo isso so e necessario no menu por causa da animacao inicial

        animacao_menu(); //verifica o script da animacao

        cont_tempo++;
        desenha = 1;
    }
    else if (evento.type == ALLEGRO_EVENT_KEY_DOWN){
        if (mostra_creditos){ //se estiver mostrando creditos, manda sumir a caixa de texto
            mostra_creditos=0;
        }
        else if (mostra_instrucoes){ //se estiver mostrando instrucoes, manda sumir a caixa de texto
            mostra_instrucoes=0;
        }
        else if (evento.keyboard.keycode == ALLEGRO_KEY_UP || evento.keyboard.keycode == ALLEGRO_KEY_W){
            imenu--; //sobe um item no menu
            if (imenu<0) //se apertar para cima no primeiro item, seleciona o ultimo
                imenu = tam_menu-1;
        }
        else if (evento.keyboard.keycode == ALLEGRO_KEY_DOWN || evento.keyboard.keycode == ALLEGRO_KEY_S){
            imenu = (imenu+1)%tam_menu; //desce item no menu
        }
        else if (evento.keyboard.keycode == ALLEGRO_KEY_ENTER){
            if (imenu == 0){ //inicia jogo
                estado++;
            }
            else if (imenu == 1){ //instrucoes
                mostra_instrucoes=1;
            }
            else if (imenu == 2){ //scores
                pula_para_scores=1;
                estado++;
            }
            else if (imenu == 3){ //creditos
                mostra_creditos=1;
            }
            else if (imenu == 4){ //sair
                estado = ESTADO_FIM;
            }
        }

    }
    else if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP){
        if (mostra_creditos){
            mostra_creditos=0;
        }
        else if (mostra_instrucoes){
            mostra_instrucoes=0;
        }
        else if (imenu == 0)
            estado++;
        else if (imenu == 1){
            mostra_instrucoes=1;
        }
        else if (imenu == 2){
            pula_para_scores=1;
            estado++;
        }
        else if (imenu == 3){
            mostra_creditos=1;
        }
        else if (imenu == 4)
            estado = ESTADO_FIM;
    }
    else if (evento.type == ALLEGRO_EVENT_MOUSE_AXES){
        x = evento.mouse.x;
        y = evento.mouse.y;
        if (dpi_samples < 10){ //espera alguns movimentos do mouse para calcular dpi da tela
            if (dpi_samples == 0){
                xi = x; //inicialmente guarda posicao inicial do mouse
                yi = y;
            }
            al_set_mouse_xy(janela, largura_janela, altura_janela); //vai para ultima posicao x y da janela
            dpi_scale = (float)largura_janela / x; //calcula a proporcao da coordenada do mouse com o valor que temos da janela
            dpi_samples++;
            if (dpi_samples == 10) //ja passou 10 vezes, volta o mouse para onde tava antes desses passos
                al_set_mouse_xy(janela, xi, yi);
        }
        x = x * ratio_x * dpi_scale; //novo x e y
        y = (y - tamanho_letterbox/2) * ratio_y * dpi_scale;

        //calcula o indice que sera o item do menu selecionado pelo mouse.
        int i = ((y - y_menu) / menu_inc) + 0.25;
        if (i < 0)
            i = 0;
        if (i >= tam_menu)
            i = tam_menu-1;

        //verifica se esta dentro da largura do item do menu
        int direcao = al_get_text_width(fonte, op_menu[i])/2 + 20;
        if (x >= x_menu - direcao && x <= x_menu + direcao)
            imenu = i;
    }
    else if(evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
        estado = ESTADO_FIM;
    }

    if(desenha && al_is_event_queue_empty(fila_eventos)) {
        al_set_target_bitmap(buffer_janela);
        al_clear_to_color(al_map_rgb(0,0,0));
        al_set_target_bitmap(animacao_inicial); //faz desenhos na animacao inicial
        for (i=0 ; i<altura_tela ; i+=TILE_SIZE)
            for (j=0 ; j<largura_tela ; j+=TILE_SIZE){
                al_draw_bitmap(fundo,j,i,0);  //desenha tiles do mapa
        }
        int flag_desenha=0;

        //desenha flechas com rotacao
        for (i=0 ; i<nflechas ; i++){
            if (flechas[i].vida > -500)
                al_draw_rotated_bitmap(flechas[i].folha_sprite, flechas[i].largura_sprite/2, flechas[i].altura_sprite/2, flechas[i].pos_x, flechas[i].pos_y, g_to_r(360-(flechas[i].angulo+180)), 0);
        }

        for (i=0 ; i<ninimigos ; i++){
            if (inimigos[i].vida > - 500){
                flag_desenha=0;
                if (inimigos[i].inverte_sprite) //verifica se o sprite precisa ser invertido
                    flag_desenha = ALLEGRO_FLIP_HORIZONTAL;
                //desenha inimigos
                al_draw_bitmap_region(inimigos[i].folha_sprite, inimigos[i].regiao_x_folha, inimigos[i].regiao_y_folha, inimigos[i].largura_sprite, inimigos[i].altura_sprite, inimigos[i].pos_x, inimigos[i].pos_y, flag_desenha);
            }
        }

        flag_desenha=0;
        //desenha heroi
        if (heroi.inverte_sprite)
            flag_desenha = ALLEGRO_FLIP_HORIZONTAL;
        al_draw_bitmap_region(heroi.folha_sprite, heroi.regiao_x_folha, heroi.regiao_y_folha, heroi.largura_sprite, heroi.altura_sprite, heroi.pos_x, heroi.pos_y, flag_desenha);

        al_set_target_bitmap(buffer_janela);

        //desenha retangulo marrom na tela
        al_draw_filled_rectangle(largura_tela/2 - al_get_bitmap_width(animacao_inicial)/2, 0, largura_tela/2 + al_get_bitmap_width(animacao_inicial)/2, altura_tela, al_map_rgb(50,30,15));
        //desenha animacao na tela
        al_draw_bitmap(animacao_inicial,largura_tela/2 - al_get_bitmap_width(animacao_inicial)/2, 25, 0);

        //al_draw_textf(fonte, al_map_rgb(255,255,255), 0, 0, ALLEGRO_ALIGN_LEFT, "%.1f %.1f", x, y);
        //al_draw_textf(fonte, al_map_rgb(255,255,255), 0, 50, ALLEGRO_ALIGN_LEFT, "%i %i", largura_janela, altura_janela);
        //al_draw_textf(fonte, al_map_rgb(255,255,255), 0, 100, ALLEGRO_ALIGN_LEFT, "%i %i", largura_tela, altura_tela);
        //al_draw_textf(fonte, al_map_rgb(255,255,255), 0, 150, ALLEGRO_ALIGN_LEFT, "%.2f", dpi_scale);

        //a cada 100 frames, atualiza cor do titulo
        if (cont_tempo%100==0)
            cor_titulo = al_map_rgb(rand()%256, rand()%256, rand()%256);
        al_draw_textf(titulo, cor_titulo, largura_tela/2, y_menu - menu_inc*2, ALLEGRO_ALIGN_CENTRE, "HORDA ORC");

        //flecha do menu
        al_draw_scaled_bitmap(flecha_titulo, 0, 0, al_get_bitmap_width(flecha_titulo), al_get_bitmap_height(flecha_titulo),
                               x_menu - x_flecha - largura_flecha, y_menu + imenu*menu_inc + y_flecha, largura_flecha, altura_flecha, 0);
        al_draw_scaled_bitmap(flecha_titulo, 0, 0, al_get_bitmap_width(flecha_titulo), al_get_bitmap_height(flecha_titulo),
                               x_menu + x_flecha, y_menu + imenu*menu_inc + y_flecha, largura_flecha, altura_flecha, ALLEGRO_FLIP_HORIZONTAL);

        //desenha item do menu
        for (i=0 ; i<tam_menu ; i++){
            if (i == imenu) //desenha item de cor vermelha, caso seja o selecionado
                cor_menu = al_map_rgb(255,0,0);
            else
                cor_menu = al_map_rgb(128,128,128);
            al_draw_textf(fonte, cor_menu, x_menu, y_menu + i*menu_inc, ALLEGRO_ALIGN_CENTRE, op_menu[i]);
        }

        //mostra a caixa com o texto dos creditos
        if (mostra_creditos){
            float w = 700;
            float h = 150;
            al_draw_filled_rectangle(largura_tela/2 - w/2 - 5, altura_tela/2 - h/2 - 5, largura_tela/2 + w/2, altura_tela/2 + h/2, al_map_rgb(192,192,192));
            al_draw_filled_rectangle(largura_tela/2 - w/2, altura_tela/2 - h/2, largura_tela/2 + w/2 + 5, altura_tela/2 + h/2 + 5, al_map_rgb(64,64,64));
            al_draw_filled_rectangle(largura_tela/2 - w/2, altura_tela/2 - h/2, largura_tela/2 + w/2, altura_tela/2 + h/2, al_map_rgb(128,128,128));
            al_draw_textf(fonte, al_map_rgb(0,0,0), largura_tela/2, altura_tela/2 - 40, ALLEGRO_ALIGN_CENTRE, "Desenvolvido por Pablo Werlang");
            al_draw_textf(fonte, al_map_rgb(0,0,0), largura_tela/2, altura_tela/2 + 5, ALLEGRO_ALIGN_CENTRE, "pswerlang@gmail.com");
        }
        //mostra a caixa com o texto das instrucoes
        if (mostra_instrucoes){
            float w = 850;
            float h = 380;
            al_draw_filled_rectangle(largura_tela/2 - w/2 - 5, altura_tela/2 - h/2 - 5, largura_tela/2 + w/2, altura_tela/2 + h/2, al_map_rgb(192,192,192));
            al_draw_filled_rectangle(largura_tela/2 - w/2, altura_tela/2 - h/2, largura_tela/2 + w/2 + 5, altura_tela/2 + h/2 + 5, al_map_rgb(64,64,64));
            al_draw_filled_rectangle(largura_tela/2 - w/2, altura_tela/2 - h/2, largura_tela/2 + w/2, altura_tela/2 + h/2, al_map_rgb(128,128,128));
            al_draw_textf(fonte, al_map_rgb(0,0,0), largura_tela/2, altura_tela/2 - 145, ALLEGRO_ALIGN_CENTRE, "Voce deve matar o maior numero");
            al_draw_textf(fonte, al_map_rgb(0,0,0), largura_tela/2, altura_tela/2 - 110, ALLEGRO_ALIGN_CENTRE, "de orcs que conseguir.");
            al_draw_textf(fonte, al_map_rgb(0,0,0), largura_tela/2, altura_tela/2 - 75, ALLEGRO_ALIGN_CENTRE, "Movimente-se usando as teclas W/S/A/D.");
            al_draw_textf(fonte, al_map_rgb(0,0,0), largura_tela/2, altura_tela/2 - 40, ALLEGRO_ALIGN_CENTRE, "Use o cursor do mouse para mirar.");
            al_draw_textf(fonte, al_map_rgb(0,0,0), largura_tela/2, altura_tela/2 - 5, ALLEGRO_ALIGN_CENTRE, "Segure o botao do mouse e solte");
            al_draw_textf(fonte, al_map_rgb(0,0,0), largura_tela/2, altura_tela/2 + 30, ALLEGRO_ALIGN_CENTRE, "para disparar uma flecha.");
            al_draw_textf(fonte, al_map_rgb(0,0,0), largura_tela/2, altura_tela/2 + 65, ALLEGRO_ALIGN_CENTRE, "Quanto mais tempo segurar o botao,");
            al_draw_textf(fonte, al_map_rgb(0,0,0), largura_tela/2, altura_tela/2 + 100, ALLEGRO_ALIGN_CENTRE, "mais longe o alcance.");
            al_draw_textf(fonte, al_map_rgb(0,0,0), largura_tela/2, altura_tela/2 + 135, ALLEGRO_ALIGN_CENTRE, "Pressione ESC para pausar/sair.");
        }

        al_set_target_bitmap(al_get_backbuffer(janela));
        al_clear_to_color(al_map_rgb(0,0,0));
        //desenha o buffer na tela, escalando o tamanho
        al_draw_scaled_bitmap(buffer_janela, 0, 0, largura_tela, altura_tela, 0, tamanho_letterbox/2, largura_janela, altura_janela, 0);
        al_flip_display();
        desenha = 0;
    }
}

//estado antes de comecar o jogo
void pre_jogo(){
    init_heroi(&heroi);
    cont_tempo=0;
    nflechas=0;
    ninimigos=0;
    tbm_pressionado=0;
    mortes=0;
    tecla_direcao[TECLA_A] = 0;
    tecla_direcao[TECLA_D] = 0;
    tecla_direcao[TECLA_W] = 0;
    tecla_direcao[TECLA_S] = 0;
    estado++;
    if (pula_para_scores)
        estado++;
}

int heroi_morreu=0, pause=0;
void jogo(){
    int i,j;
    ALLEGRO_EVENT evento;
    al_wait_for_event(fila_eventos, &evento);

    /* -- EVENTOS -- */
    if(evento.type == ALLEGRO_EVENT_TIMER){
        //se o jogo nao esta parado
        if (!pause){
            atualiza_heroi(&heroi); //atualiza heroi
            if (heroi.vida < 0){ //se heroi morreu fica 250 frames apanhando
                heroi_morreu++;
                if (heroi_morreu > 250)
                    estado++;
            }
            for (i=0 ; i<ninimigos ; i++)
                atualiza_inimigo(&inimigos[i], &heroi);
            atualiza_flechas(flechas, nflechas);
            colisao_flecha(flechas, nflechas, inimigos, ninimigos);
            if (tbm_pressionado)    tbm_pressionado++;

            if (cont_tempo%inimigo_spawn_time == 0){
                init_inimigo(&inimigos[iinimigos]);
                iinimigos = (iinimigos+1) % MAX_INIMIGOS;
                if (ninimigos < MAX_INIMIGOS)
                    ninimigos++;
            }

            cont_tempo++;
        }
        desenha=1;
    }
    else if (evento.type == ALLEGRO_EVENT_KEY_DOWN){
        if (pause){
            if (evento.keyboard.keycode == ALLEGRO_KEY_ESCAPE) //apertou esc com o jogo pausado
                estado = ESTADO_FIM;
            else
                pause=0;
        }
        //coloca +1 ou -1 nas posicoes do vetor direcao, indicando as direcoes x e y que estao sendo pressionadas no teclado
        else if (evento.keyboard.keycode == ALLEGRO_KEY_W){
            tecla_direcao[TECLA_W]=-1;
        }
        else if (evento.keyboard.keycode == ALLEGRO_KEY_S){
            tecla_direcao[TECLA_S]=1;
        }
        else if (evento.keyboard.keycode == ALLEGRO_KEY_A){
            tecla_direcao[TECLA_A]=-1;
        }
        else if (evento.keyboard.keycode == ALLEGRO_KEY_D){
            tecla_direcao[TECLA_D]=1;
        }
        else if (evento.keyboard.keycode == ALLEGRO_KEY_ESCAPE){//seta flag que pausa o jogo
            pause=1;
        }
    }
    else if (evento.type == ALLEGRO_EVENT_KEY_UP){
        if (evento.keyboard.keycode == ALLEGRO_KEY_W){
            tecla_direcao[TECLA_W]=0;
        }
        else if (evento.keyboard.keycode == ALLEGRO_KEY_S){
            tecla_direcao[TECLA_S]=0;
        }
        else if (evento.keyboard.keycode == ALLEGRO_KEY_A){
            tecla_direcao[TECLA_A]=0;
        }
        else if (evento.keyboard.keycode == ALLEGRO_KEY_D){
            tecla_direcao[TECLA_D]=0;
        }
    }
    else if (evento.type == ALLEGRO_EVENT_MOUSE_AXES){
        x = evento.mouse.x * ratio_x * dpi_scale * (1.01333 - 0.0133333*dpi_scale); //ajuste para dpi_scale x=1, y=1; x=1.75, y=0.99
        y = (evento.mouse.y - tamanho_letterbox/2) * ratio_y * dpi_scale * (1.01333 - 0.0133333*dpi_scale);

        //calcula angulo do heroi com o mouse
        heroi.angulo = calcula_angulo(heroi.pos_x + heroi.largura_sprite/2,
                                      heroi.pos_y + heroi.altura_sprite/2, x, y);
    }
    else if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN){
        if (!pause)
            tbm_pressionado = 1;
    }
    else if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP){
        //quando soltar o botao, cria uma nova flecha
        if (!pause){
            if (nflechas < MAX_FLECHAS)
                nflechas++;
            init_flecha(&flechas[iflechas], &heroi);
            iflechas = (iflechas+1) % MAX_FLECHAS; //quando chegar na ultima posicao do vetor, volta pra primeira
            tbm_pressionado = 0;
            al_play_sample(som_atira_flecha, 1.0, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
        }
    }
    else if(evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
        estado = ESTADO_FIM;
    }

    /* -- ATUALIZA TELA -- */
    if(desenha && al_is_event_queue_empty(fila_eventos)) {
        al_set_target_bitmap(buffer_janela);
        al_clear_to_color(al_map_rgb(0,0,0));
        int flag_desenha;
        for (i=0 ; i<altura_tela ; i+=TILE_SIZE){
            for (j=0 ; j<largura_tela ; j+=TILE_SIZE){
                al_draw_bitmap(fundo,j,i,0);
            }
        }

        desenha_flechas(flechas, nflechas, &heroi);

        for (i=0 ; i<ninimigos ; i++){
            if (inimigos[i].vida <= 0 && inimigos[i].vida > -500){
                flag_desenha=0;
                if (inimigos[i].inverte_sprite)
                    flag_desenha = ALLEGRO_FLIP_HORIZONTAL;
                al_draw_bitmap_region(inimigos[i].folha_sprite, inimigos[i].regiao_x_folha, inimigos[i].regiao_y_folha, inimigos[i].largura_sprite, inimigos[i].altura_sprite, inimigos[i].pos_x, inimigos[i].pos_y, flag_desenha);
            }
        }

        flag_desenha=0;
        if (heroi.inverte_sprite)
            flag_desenha = ALLEGRO_FLIP_HORIZONTAL;
        if (heroi.vida <= 0)
            al_draw_bitmap_region(heroi.folha_sprite, heroi.regiao_x_folha, heroi.regiao_y_folha, heroi.largura_sprite, heroi.altura_sprite, heroi.pos_x, heroi.pos_y, flag_desenha);

        for (i=0 ; i<ninimigos ; i++){
            if (inimigos[i].vida > 0){
                flag_desenha=0;
                if (inimigos[i].inverte_sprite)
                    flag_desenha = ALLEGRO_FLIP_HORIZONTAL;
                al_draw_bitmap_region(inimigos[i].folha_sprite, inimigos[i].regiao_x_folha, inimigos[i].regiao_y_folha, inimigos[i].largura_sprite, inimigos[i].altura_sprite, inimigos[i].pos_x, inimigos[i].pos_y, flag_desenha);
            }
        }

        flag_desenha=0;
        if (heroi.inverte_sprite)
            flag_desenha = ALLEGRO_FLIP_HORIZONTAL;
        if (heroi.vida > 0)
            al_draw_bitmap_region(heroi.folha_sprite, heroi.regiao_x_folha, heroi.regiao_y_folha, heroi.largura_sprite, heroi.altura_sprite, heroi.pos_x, heroi.pos_y, flag_desenha);
        al_draw_bitmap_region(buffer_janela, heroi.pos_x + heroi.largura_sprite/2 - 65, heroi.pos_y + heroi.altura_sprite/2 - 65, 130, 130, 16, 16, 0);

        al_draw_bitmap(gui, 0, 0, 0); //desenha a interface que mostra vida, e botao pressionado na tela
        if (heroi.vida > 0){
            al_draw_bitmap_region(barra_vida, 0, 0, 16, 20, 118,14, 0); //barra vermelha de vida
            al_draw_scaled_bitmap(barra_vida, 17, 0, al_get_bitmap_width(barra_vida)-16, 20, 118+16, 14, (al_get_bitmap_width(barra_vida)-16) * heroi.vida/100, 20, 0);
        }
        if (tbm_pressionado > 0){
            al_draw_bitmap_region(barra_mira, 0, 0, 12, 21, 138,33, 0);
            //desenha a barra amarela, de alcance do tiro
            if (tbm_pressionado > 100)
                al_draw_scaled_bitmap(barra_mira, 13, 0, al_get_bitmap_width(barra_mira)-13, 21, 138+12, 33, (al_get_bitmap_width(barra_mira)-13) , 21, 0);
            else
                al_draw_scaled_bitmap(barra_mira, 13, 0, al_get_bitmap_width(barra_mira)-13, 21, 138+12, 33, (al_get_bitmap_width(barra_mira)-13) * tbm_pressionado/100, 21, 0);
        }
        al_draw_textf(fonte, al_map_rgb(255,0,0), 165, 70, ALLEGRO_ALIGN_LEFT, "PONTOS: %i",mortes);

        if (pause){ //se pausou o jogo, aparece a caixa de texto
            float w = 700;
            float h = 150;
            al_draw_filled_rectangle(largura_tela/2 - w/2 - 5, altura_tela/2 - h/2 - 5, largura_tela/2 + w/2, altura_tela/2 + h/2, al_map_rgb(192,192,192));
            al_draw_filled_rectangle(largura_tela/2 - w/2, altura_tela/2 - h/2, largura_tela/2 + w/2 + 5, altura_tela/2 + h/2 + 5, al_map_rgb(64,64,64));
            al_draw_filled_rectangle(largura_tela/2 - w/2, altura_tela/2 - h/2, largura_tela/2 + w/2, altura_tela/2 + h/2, al_map_rgb(128,128,128));
            al_draw_textf(fonte, al_map_rgb(0,0,0), largura_tela/2, altura_tela/2 - 50, ALLEGRO_ALIGN_CENTRE, "JOGO PAUSADO");
            al_draw_textf(fonte, al_map_rgb(0,0,0), largura_tela/2, altura_tela/2 - 10, ALLEGRO_ALIGN_CENTRE, "Pressione ESC para sair, ou ");
            al_draw_textf(fonte, al_map_rgb(0,0,0), largura_tela/2, altura_tela/2 + 30, ALLEGRO_ALIGN_CENTRE, "outra tecla para continuar. ");
        }

        al_set_target_bitmap(al_get_backbuffer(janela));
        al_clear_to_color(al_map_rgb(0,0,0));
        al_draw_scaled_bitmap(buffer_janela, 0, 0, largura_tela, altura_tela, 0, tamanho_letterbox/2, largura_janela, altura_janela, 0);
        al_flip_display();
        desenha = 0;
    }
}

int entrou_no_top=-1;
char texto[10]="";
//carregar arquivo com scores
void pre_score(){
    FILE *arq = fopen("scores","rb");
    if (arq == NULL){ //se arquivo nao existe, cria
        arq = fopen("scores","wb");
        entrou_no_top=0;
        fclose(arq);
    }
    arq = fopen("scores","rb");
    int tam=0;
    while (!feof(arq)){ //carrega todas posicoes do arquivo na struct
        fread(&top[tam],sizeof(struct scores),1,arq);
        if (!feof(arq)){
            if (mortes > top[tam].pontos && entrou_no_top == -1) //verifica se pontuacao atual faz entrar no top10
                entrou_no_top = tam;
            tam++;
        }
        if (tam == 0) //se nenhum elemento no arquivo, entrou no top10
            entrou_no_top = tam;
    }
    fclose(arq);
    strcpy(texto, "");
    estado++;
}

void score(){
    int i,j;
    char c[2]="";
    ALLEGRO_EVENT evento;
    al_wait_for_event(fila_eventos, &evento);
    if(evento.type == ALLEGRO_EVENT_TIMER){
        heroi_morreu--; //torna obrigatorio a espera de uns segundo antes de fechar o jogo assim que heroi morreu
        desenha = 1;
    }
    else if (evento.type == ALLEGRO_EVENT_KEY_DOWN){
        if (entrou_no_top == -1){
            if (pula_para_scores){ //se chegou aqui atraves do menu inicial, volta o estado para o inicial
                pula_para_scores=0;
                estado=0;
            }
            else if (heroi_morreu <= 0)
                estado++;
        }
        else if (evento.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
            estado++;
        else if (evento.keyboard.keycode == ALLEGRO_KEY_ENTER){
            if (entrou_no_top != -1){ //apos digitar o nome, e deu enter salva conteudo na struct de volta
                FILE *arq = fopen("scores","wb");
                for (i=0 ; i<10 ; i++){
                    if (mortes > top[i].pontos){ //procura em qual posicao do vetor adicionar
                        for(j=9 ; j>i ; j--) //empurra os proximos elementos pra frente
                            top[j] = top[j-1];
                        top[i].pontos = mortes;
                        strcpy(top[i].nome, texto);
                        break;
                    }
                }
                fwrite(top, sizeof(struct scores), 10, arq); //esceve no arquivo
                fclose(arq);

                estado++;
            }
        }
    }
    else if (evento.type == ALLEGRO_EVENT_KEY_CHAR){ //ao digitar os caracteres do nome
        if (entrou_no_top != -1){
            *c = evento.keyboard.unichar; //captura caractere
            if (evento.keyboard.keycode == ALLEGRO_KEY_BACKSPACE) //backspace, volta uma posicao no texto
                texto[strlen(texto)-1]='\0';
            //se for letras ou numeros, insere no texto
            else if ( ((*c >= 'a' && *c <= 'z') || (*c >= 'A' && *c <= 'Z') || (*c >= '0' && *c <= '9') || (*c == ' ')) && strlen(texto)<10 )
                strcat(texto,c);
        }
    }
    else if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP){
        if (entrou_no_top == -1){
            if (pula_para_scores){
                pula_para_scores=0;
                estado=0;
            }
            else if (heroi_morreu <= 0)
                estado++;
        }
    }
    else if(evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
        estado = ESTADO_FIM;
    }

    if(desenha && al_is_event_queue_empty(fila_eventos)) {
        float y_menu = 5*altura_tela/15; // 10/15 da tela sera para os items do menu
        int menu_inc = (altura_tela-y_menu)/10;
        al_set_target_bitmap(buffer_janela);
        al_clear_to_color(al_map_rgb(0,0,0));
        al_draw_filled_rectangle(largura_tela/2 - al_get_bitmap_width(animacao_inicial)/2, 0, largura_tela/2 + al_get_bitmap_width(animacao_inicial)/2, altura_tela, al_map_rgb(50,30,15));

        if (entrou_no_top != -1){ //mostra texto se entrou no top
            al_draw_textf(titulo, al_map_rgb(0,255,255), largura_tela/2, y_menu - 4.5*menu_inc, ALLEGRO_ALIGN_CENTRE, "TOP 10 PONTUACOES");
            al_draw_textf(fonte, al_map_rgb(255,0,0), largura_tela/2, y_menu - 3*menu_inc, ALLEGRO_ALIGN_CENTRE, "DIGITE SEU NOME PARA ENTRAR PARA A");
            al_draw_textf(fonte, al_map_rgb(255,0,0), largura_tela/2, y_menu - 2*menu_inc, ALLEGRO_ALIGN_CENTRE, "HISTORIA COMO MATADOR DE ORCS");
        }
        else//ou so o titulo caso nao tenha entrado
            al_draw_textf(titulo, al_map_rgb(0,255,255), largura_tela/2, y_menu - 3.5*menu_inc, ALLEGRO_ALIGN_CENTRE, "TOP 10 PONTUACOES");

        al_draw_textf(fonte, al_map_rgb(0,128,255), largura_tela/2 - al_get_bitmap_width(animacao_inicial)/2 + 280, y_menu - menu_inc, ALLEGRO_ALIGN_LEFT, "PTS.");
        al_draw_textf(fonte, al_map_rgb(0,128,255), largura_tela/2 - al_get_bitmap_width(animacao_inicial)/2 + 430, y_menu - menu_inc, ALLEGRO_ALIGN_LEFT, "NOME");

        j=0;
        //desenha cada item na tela
        for (i=0 ; i<10 ; i++){
            if (i == entrou_no_top){
                al_draw_textf(fonte, al_map_rgb(255,0,0), largura_tela/2 - al_get_bitmap_width(animacao_inicial)/2 + 280, y_menu + i*menu_inc, ALLEGRO_ALIGN_LEFT, "%i",mortes);
                al_draw_textf(fonte, al_map_rgb(255,0,0), largura_tela/2 - al_get_bitmap_width(animacao_inicial)/2 + 430, y_menu + i*menu_inc, ALLEGRO_ALIGN_LEFT, "%s",texto);
                j=-1;
            }
            else if (top[i+j].pontos > 0){
                al_draw_textf(fonte, al_map_rgb(128,128,128), largura_tela/2 - al_get_bitmap_width(animacao_inicial)/2 + 280, y_menu + i*menu_inc, ALLEGRO_ALIGN_LEFT, "%i",top[i+j].pontos);
                al_draw_textf(fonte, al_map_rgb(128,128,128), largura_tela/2 - al_get_bitmap_width(animacao_inicial)/2 + 430, y_menu + i*menu_inc, ALLEGRO_ALIGN_LEFT, "%s",top[i+j].nome);
            }
        }

        al_set_target_bitmap(al_get_backbuffer(janela));
        al_clear_to_color(al_map_rgb(0,0,0));
        al_draw_scaled_bitmap(buffer_janela, 0, 0, largura_tela, altura_tela, 0, tamanho_letterbox/2, largura_janela, altura_janela, 0);
        al_flip_display();
        desenha = 0;
    }
}

int main(void){
    srand(time(NULL));

    if (!inicializar())
        return -1;

    while(estado != ESTADO_FIM){ //enquanto nao chegar no estado do fim de jogo, entra no estado adequado
        if (estado == ESTADO_PRE_MENU)
            pre_menu();
        else if (estado == ESTADO_MENU)
            menu();
        else if (estado == ESTADO_PRE_JOGO)
            pre_jogo();
        else if (estado == ESTADO_JOGO)
            jogo();
        else if (estado == ESTADO_PRE_SCORE)
            pre_score();
        else if (estado == ESTADO_SCORE)
            score();
    }

    destroy();
    return 0;
}
