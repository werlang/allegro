#include <allegro5/allegro.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

#define LARGURA_TELA 640
#define ALTURA_TELA 480

ALLEGRO_DISPLAY *janela = NULL;
//samples que guardam os efeitos sonoros
ALLEGRO_SAMPLE *som_azul=NULL;
ALLEGRO_SAMPLE *som_verm=NULL;
//musica de fundo
ALLEGRO_AUDIO_STREAM *musica = NULL;
ALLEGRO_BITMAP *bitmap_azul = NULL;
ALLEGRO_BITMAP *bitmap_verm = NULL;
ALLEGRO_EVENT_QUEUE *fila_eventos = NULL;

void error_msg(char *text){
	al_show_native_message_box(NULL,"ERRO",
		"Ocorreu o seguinte erro e o programa sera finalizado:",
		text,NULL,ALLEGRO_MESSAGEBOX_ERROR);
}

int inicializar(){
    if(!al_init()){
        error_msg("Falha ao inicializar a Allegro");
        return 0;
    }

    //addon de audio
    if(!al_install_audio()){
        error_msg("Falha ao inicializar o audio");
        return 0;
    }

    //addon que da suporte as extensoes de audio
    if(!al_init_acodec_addon()){
        error_msg("Falha ao inicializar o codec de audio");
        return 0;
    }

    //cria o mixer (e torna ele o mixer padrao), e adciona 5 samples de audio nele
    if (!al_reserve_samples(5)){
        error_msg("Falha ao reservar amostrar de audio");
        return 0;
    }

    //carrega os samples
    som_verm = al_load_sample( "wololo.ogg" );
    if (!som_verm){
        error_msg( "Audio nao carregado" );
        return 0;
    }
    som_azul = al_load_sample( "aiao.ogg" );
    if (!som_azul){
        error_msg( "Audio nao carregado" );
        al_destroy_sample(som_verm);
        return 0;
    }

    //carrega o stream
    musica = al_load_audio_stream("soundtrack.ogg", 4, 1024);
    if (!musica)
    {
        error_msg( "Audio nao carregado" );
        al_destroy_sample(som_azul);
        al_destroy_sample(som_verm);
        return 0;
    }
    //liga o stream no mixer
    al_attach_audio_stream_to_mixer(musica, al_get_default_mixer());
    //define que o stream vai tocar no modo repeat
    al_set_audio_stream_playmode(musica, ALLEGRO_PLAYMODE_LOOP);

    janela = al_create_display(LARGURA_TELA, ALTURA_TELA);
    if(!janela){
        error_msg("Falha ao criar janela");
        al_destroy_sample(som_azul);
        al_destroy_sample(som_verm);
        al_destroy_audio_stream(musica);
        return 0;
    }

    if (!al_install_mouse()){
        error_msg("Falha ao inicializar o mouse");
        al_destroy_display(janela);
        return -1;
    }

    bitmap_azul = al_create_bitmap(30, 80);
    if (!bitmap_azul){
        error_msg("Falha ao criar bitmap");
        al_destroy_sample(som_azul);
        al_destroy_sample(som_verm);
        al_destroy_audio_stream(musica);
        al_destroy_display(janela);
        return 0;
    }
    al_set_target_bitmap(bitmap_azul);
    al_clear_to_color(al_map_rgb(0, 0, 255));

    bitmap_verm = al_create_bitmap(30, 80);
    if (!bitmap_verm){
        error_msg("Falha ao criar bitmap");
        al_destroy_sample(som_azul);
        al_destroy_sample(som_verm);
        al_destroy_audio_stream(musica);
        al_destroy_display(janela);
        al_destroy_bitmap(bitmap_azul);
        return 0;
    }
    al_set_target_bitmap(bitmap_verm);
    al_clear_to_color(al_map_rgb(255, 0, 0));

    al_set_target_bitmap(al_get_backbuffer(janela));

    fila_eventos = al_create_event_queue();
    if(!fila_eventos) {
        error_msg("Falha ao criar fila de eventos");
        al_destroy_sample(som_azul);
        al_destroy_sample(som_verm);
        al_destroy_audio_stream(musica);
        al_destroy_display(janela);
        al_destroy_bitmap(bitmap_azul);
        al_destroy_bitmap(bitmap_verm);
        return 0;
    }

    al_register_event_source(fila_eventos, al_get_mouse_event_source());
    al_register_event_source(fila_eventos, al_get_display_event_source(janela));
    al_clear_to_color(al_map_rgb(0,0,0));
    al_flip_display();

    return 1;
}

int main(){
    int sair=0;
    //vai guardar quando o mouse esta sobre a area de algum dos bitmaps
    //0=nenhum, 1=azul, 2=vermelho
    int area=0;

    if (!inicializar()){
        return -1;
    }

    al_clear_to_color(al_map_rgb(0,0,0));

    //desenha bitmaps que serao clicados
    al_draw_bitmap(bitmap_azul,50,50,0);
    al_draw_bitmap(bitmap_verm,150,50,0);

    al_flip_display();

    while (!sair){
        ALLEGRO_EVENT evento;
        al_wait_for_event(fila_eventos, &evento);

        if (evento.type == ALLEGRO_EVENT_MOUSE_AXES){
            //y entre 50 e 130 significa na altura dos bitmaps
            if (evento.mouse.y >= 50 && evento.mouse.y <= 130){
                //x entre 50 e 80 representa a area do bitmap azul
                if (evento.mouse.x >= 50 && evento.mouse.x <= 80)
                    area=1;
                //x entre 150 e 180 representa a area do bitmap vermelho
                else if (evento.mouse.x >= 150 && evento.mouse.x <= 180)
                    area=2;
                else
                    area=0;
            }
            else
                area=0;
        }
        else if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP){
            if (area==1)
                //toca som_azul uma vez
                al_play_sample(som_azul, 1.0, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);

            if (area==2)
                //toca som_verm uma vez
                al_play_sample(som_verm, 1.0, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);

        }
        else if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE){
            sair = 1;
        }
    }

    al_destroy_sample(som_azul);
    al_destroy_sample(som_verm);
    al_destroy_audio_stream(musica);
    al_destroy_display(janela);
    al_destroy_bitmap(bitmap_azul);
    al_destroy_bitmap(bitmap_verm);
    al_destroy_event_queue(fila_eventos);
    return 0;
}
