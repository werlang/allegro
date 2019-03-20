#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

#include <stdio.h>
#include <stdbool.h>

bool inicializar();

const int LARGURA_TELA = 640;
const int ALTURA_TELA = 480;

ALLEGRO_DISPLAY *janela = NULL;

int main(void)
{
    if (!inicializar())
    {
        return -1;
    }

    // Linha: x1, y1, x2, y2, cor, espessura
    al_draw_line(20.0, 40.0, 40.0, 60.0, al_map_rgb(255, 0, 0), 1.0);
    al_flip_display();

    al_rest(2.0);

    al_clear_to_color(al_map_rgb(0, 0, 0));
    al_flip_display();

    al_rest(0.5);

    // Triângulo: x1, y1, x2, y2, x3, y3, cor, espessura
    al_draw_triangle(70.0, 30.0, 20.0, 55.0, 110.0, 250.0, al_map_rgb(255, 255, 255), 5.0);
    al_flip_display();

    al_rest(2.0);

    al_clear_to_color(al_map_rgb(0, 0, 0));
    al_flip_display();

    al_rest(0.5);

    // Triângulo preenchido: x1, y1, x2, y2, x3, y3, cor
    al_draw_filled_triangle(40.0, 90.0, 120.0, 246.0, 400.0, 23.0, al_map_rgb(255, 255, 0));
    al_flip_display();

    al_rest(2.0);

    al_clear_to_color(al_map_rgb(0, 0, 0));
    al_flip_display();

    al_rest(0.5);

    // Retângulo: x1, y1, x2, y2, cor, espessura
    al_draw_rectangle(70.0, 30.0, 110.0, 250.0, al_map_rgb(255, 0, 255), 6.0);
    al_flip_display();

    al_rest(2.0);

    al_clear_to_color(al_map_rgb(0, 0, 0));
    al_flip_display();

    al_rest(0.5);

    // Retângulo preenchido: x1, y1, x2, y2, cor
    al_draw_filled_rectangle(88.0, 10.0, 340.0, 77.0, al_map_rgb(0, 255, 255));
    al_flip_display();

    al_rest(2.0);

    al_clear_to_color(al_map_rgb(0, 0, 0));
    al_flip_display();

    al_rest(0.5);

    // Elipse: x1, y1, raio x, raio y, cor, espessura
    al_draw_ellipse(70.0, 90.0, 20.0, 55.0, al_map_rgb(255, 255, 255), 5.0);
    al_flip_display();

    al_rest(2.0);

    al_clear_to_color(al_map_rgb(0, 0, 0));
    al_flip_display();

    al_rest(0.5);

    // Elipse preenchido: x1, y1, raio x, raio y, cor
    al_draw_filled_ellipse(98.0, 145.0, 25.0, 15.0, al_map_rgb(128, 255, 128));
    al_flip_display();

    al_rest(2.0);

    al_clear_to_color(al_map_rgb(0, 0, 0));
    al_flip_display();

    al_rest(0.5);

    // Círculo: x1, y1, raio, cor, espessura
    al_draw_circle(250.0, 300.0, 70.0, al_map_rgb(128, 0, 0), 2.0);
    al_flip_display();

    al_rest(2.0);

    al_clear_to_color(al_map_rgb(0, 0, 0));
    al_flip_display();

    al_rest(0.5);

    // Círculo preenchido: x1, y1, raio, cor
    al_draw_filled_circle(350.0, 50.0, 43.0, al_map_rgb(0, 0, 255));
    al_flip_display();

    al_rest(2.0);

    al_clear_to_color(al_map_rgb(0, 0, 0));
    al_flip_display();

    al_rest(0.5);

    al_destroy_display(janela);

    return 0;
}

bool inicializar()
{
    if (!al_init())
    {
        fprintf(stderr, "Falha ao inicializar a biblioteca Allegro.\n");
        return false;
    }

    if (!al_init_primitives_addon())
    {
        fprintf(stderr, "Falha ao inicializar add-on de primitivas.\n");
        return false;
    }

    janela = al_create_display(LARGURA_TELA, ALTURA_TELA);
    if (!janela)
    {
        fprintf(stderr, "Falha ao criar janela.\n");
        return false;
    }

    al_set_window_title(janela, "Testando allegro_primitives");

    return true;
}
