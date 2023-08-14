#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <stdbool.h>
#include <stdio.h>
#define resx 800
#define resy 600
#define ancho 40
#define movespeed_p 8
#define movespeed_e 1
#define NUM_ENEMIGOS 6
#define MAX_BULLETS 10 // Número máximo de disparos en pantalla

float velocidad_salto = -10.0; // La velocidad inicial del salto
float gravedad = 1.0;          // La fuerza de gravedad que afecta al personaje

struct perso
{
    int posx = 50;
    int posy = 50;
    int vidas = 3;
    int suelo;
};

struct enemy
{
    int posxe = 100;
    int posye = 100;
    bool dir = 0;
    int hp = 100;
};

/*typedef struct
{
    float x; // Posición x del disparo
    float y; // Posición y del disparo
    float angle; // Ángulo del disparo
    bool active; // Estado del disparo (activo o no)
} Bullet;*/

void dibujarmapa(int mapa[resy / ancho][resx / ancho], ALLEGRO_BITMAP *bloque , ALLEGRO_BITMAP *puerta);
/*void decoraciones(int mapa[resy / ancho][resx / ancho], ALLEGRO_BITMAP *puerta);*/
void dibujarenemigos(struct enemy enemigos[], int num_enemigos, ALLEGRO_BITMAP *imagen_enemigos);
void acciones_de_enemigos(struct enemy enemigos[], int num_enemigos, int mapa[resy / ancho][resx / ancho]);
void inicializar_enemigos(struct enemy enemigos[], int num_enemigos, int mapa[resy / ancho][resx / ancho]);
void moverpj(ALLEGRO_EVENT_QUEUE *queue, struct perso *p, bool *menu, bool *juego, bool *derecha, bool *izquierda, int mapa[resy / ancho][resx / ancho]);
// void init_bullets();
// void fire_bullet(float x, float y, int direction);
// void draw_bullets();

int main()
{
    al_init();
    al_install_keyboard();
    al_init_image_addon();
    al_install_mouse();

    int x, y;
    int mx, my;
    int opcion;
    int botones[] = {0, 0};

    bool menu = false;
    bool juego = false;
    bool redraw = true;

    bool derecha = false;
    bool izquierda = false;
    bool saltando = false;

    struct perso p;
    struct enemy enemigos[NUM_ENEMIGOS];
    // inicializar_enemigos(enemigos, NUM_ENEMIGOS);

    int mapa[resy / ancho][resx / ancho];
    // Bullet bullets[MAX_BULLETS];

    ALLEGRO_TIMER *timer = al_create_timer(1.0 / 30.0);
    ALLEGRO_DISPLAY *disp = al_create_display(resx, resy);
    ALLEGRO_EVENT_QUEUE *queue = al_create_event_queue();
    ALLEGRO_FONT *font = al_create_builtin_font();
    ALLEGRO_BITMAP *personaje = al_load_bitmap("imagenes_1/PJ1.png");
    ALLEGRO_BITMAP *bloque = al_load_bitmap("brick.png");
    ALLEGRO_BITMAP *puerta = al_load_bitmap("imagenes_1/puerta2.png");
    ALLEGRO_BITMAP *fondo = al_load_bitmap("imagenes_1/fondo2.png");
    ALLEGRO_BITMAP *imagen_enemigos = al_load_bitmap("imagenes_1/imagen_enemigo.png");
    // ALLEGRO_BITMAP *bullet = al_load_bitmap("iamgenes_1/bala.png");
    // ALLEGRO_SAMPLE *musica = al_load_sample("nombre_del_archivo.wav");
    //  menu
    ALLEGRO_BITMAP *menu_null = al_load_bitmap("imagenes_1/menu_null.png");
    ALLEGRO_BITMAP *menu_jugar = al_load_bitmap("imagenes_1/menu_jugar.png");
    ALLEGRO_BITMAP *menu_salir = al_load_bitmap("imagenes_1/menu_salir.png");
    // ALLEGRO_BITMAP *imagen_enemigo = al_load_bitmap("mono2.jpg");

    al_convert_mask_to_alpha(personaje, al_map_rgb(0xFF, 0x00, 0xFF));
    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(disp));
    al_register_event_source(queue, al_get_timer_event_source(timer));
    al_register_event_source(queue, al_get_mouse_event_source());

    al_start_timer(timer);

    dibujarmapa(mapa, bloque, puerta);
    inicializar_enemigos(enemigos, NUM_ENEMIGOS, mapa);
    /*decoraciones(mapa, puerta);*/

    while (!menu)
    {
        ALLEGRO_EVENT events;
        al_wait_for_event(queue, &events);
        al_clear_to_color(al_map_rgb(0, 0, 0));
        al_draw_bitmap(menu_null, 0, 0, 0);
        switch (events.keyboard.keycode)
        {
        case ALLEGRO_KEY_1:
            while (!juego)
            {
                moverpj(queue, &p, &menu, &juego, &derecha, &izquierda, mapa);
                
                // fire_bullet(x, y, direction);

                /*ALLEGRO_EVENT event;
                al_wait_for_event(queue, &event);*/

                // else if ((events.type == ALLEGRO_EVENT_DISPLAY_CLOSE)) break;

                if (redraw && al_is_event_queue_empty(queue))
                {
                    al_clear_to_color(al_map_rgb(0, 0, 0));
                    al_draw_bitmap(fondo, 0, 0, 0);
                    dibujarmapa(mapa, bloque, puerta);
                    dibujarenemigos(enemigos, NUM_ENEMIGOS, imagen_enemigos);
                    //acciones_de_enemigos(enemigos, NUM_ENEMIGOS);
                    acciones_de_enemigos(enemigos, NUM_ENEMIGOS, mapa);
                    /*decoraciones(mapa, puerta);*/
                    al_draw_bitmap(personaje, p.posx, p.posy, 0);

                    al_flip_display();
                    redraw = true;
                }
            }
            break;

        case ALLEGRO_KEY_2:
            menu = true;
            break;
        }
        if (redraw && al_is_event_queue_empty(queue))
        {
            al_clear_to_color(al_map_rgb(0, 0, 0));
            al_draw_bitmap(menu_null, 0, 0, 0);
            // al_draw_text(font, al_map_rgb(255, 255, 255), 0, 0, 0, "Hola mundo");

            al_flip_display();
            redraw = true;
        }
    }
    al_destroy_display(disp);
    al_destroy_event_queue(queue);
    al_destroy_timer(timer);
    al_destroy_bitmap(personaje);
    al_destroy_bitmap(bloque);
    al_destroy_bitmap(puerta);
    al_destroy_bitmap(fondo);
    al_destroy_bitmap(menu_null);
    al_destroy_bitmap(menu_jugar);
    al_destroy_bitmap(menu_salir);
    al_destroy_font(font);

    return 0;
}

void dibujarpersonaje(ALLEGRO_BITMAP *personaje, struct perso jugador)
{
    float pixelPosX = jugador.posx;
    float pixelPosY = jugador.posy;

    al_draw_bitmap(personaje, pixelPosX, pixelPosY, 0);
}

void moverpj(ALLEGRO_EVENT_QUEUE *queue, struct perso *p, bool *menu, bool *juego, bool *derecha, bool *izquierda, int mapa[resy / ancho][resx / ancho])
{
    static bool en_aire = false;
    static float velocidad_y = 0.0;

    ALLEGRO_EVENT events;
    al_wait_for_event(queue, &events);

    // printf("%d, %d\n", p->posy/ancho, p->posx/ancho);
    // printf("%d, %d\n", mapa[(p->posy+ancho)/ancho][(p->posx)/ancho], mapa[(p->posy+ancho)/ancho][(p->posx+ancho)/ancho]);
    // printf("%d\n", resy/ancho);
    /*for (int i = 0; i < resy / ancho; i++)
    {
        for (int j = 0; j < resx / ancho; j++)
        {
            printf("[%d]", mapa[i][j]);
        }
        printf("\n");
    }*/

    if (events.type == ALLEGRO_EVENT_TIMER)
    {
        // Aplicacion de la gravedad
        if (en_aire)
        {
            velocidad_y += gravedad;
            p->posy += velocidad_y;
        }

        // Control de colisiones con el suelo ?????????
        // if (p->posy + ancho >= resy && (mapa[(p->posy+ancho)/ancho][(p->posx)/ancho] != '1' || mapa[(p->posy+ancho)/ancho][(p->posx+ancho)/ancho] != '1'))
        // if (p->posy + ancho <= resy && (mapa[(p->posy+ancho)/ancho][(p->posx)/ancho] == '1' || mapa[(p->posy+ancho)/ancho][(p->posx+ancho)/ancho] == '1'))
        // printf("%d, %d\n", mapa[(p->posy+ancho)/ancho][(p->posx)/ancho], mapa[(p->posy+ancho)/ancho][(p->posx+ancho)/ancho]);  //IMPRIMES LA POSICION DEL JUGADOR (LOS PIES) EN EL ARREGLO, PERO IMPRIME UN 100 (???????)

        if (p->posy + ancho >= resy || (mapa[(p->posy + ancho + 4) / ancho][(p->posx + 6) / ancho] == 1 || mapa[(p->posy + ancho + 4) / ancho][(p->posx + ancho - 6) / ancho] == 1))
        {
            en_aire = false;
            velocidad_y = 0.0;
            p->posy -= velocidad_y;
        }

        // Movimiento horizontal hacia la derecha
        // if (*derecha && p->posx + ancho < resx && mapa[(p->posy)/ancho][(p->posx+ancho)/ancho] == '1')
        if (*derecha && p->posx + ancho < resx && (mapa[(p->posy + 10) / ancho][(p->posx + ancho + 2) / ancho] != 1 && mapa[(p->posy + ancho - 2) / ancho][(p->posx + ancho + 2) / ancho] != 1))
        // if (*derecha && [p->posx + ancho < resx] <- BORDES DE LA PANTALLA && [BLOQUES])
        {
            // Comprobar si hay colisión con algún bloque en la siguiente posición.............
            p->posx += movespeed_p;
            if (mapa[(p->posy + ancho) / ancho][(p->posx + 6) / ancho] == 0 && mapa[(p->posy + ancho) / ancho][(p->posx + ancho - 6) / ancho] == 0)
            {
                en_aire = true;
            }
        }
        // Movimiento horizontal hacia la izquierda
        else if (*izquierda && p->posx + ancho < resx && (mapa[(p->posy + 2) / ancho][(p->posx - 2) / ancho] != 1 && mapa[(p->posy + ancho - 2) / ancho][(p->posx - 2) / ancho] != 1))
        {
            // Comprobar si hay colisión con algún bloque en la siguiente posición......
            p->posx -= movespeed_p;
            if (mapa[(p->posy + ancho) / ancho][(p->posx + 6) / ancho] == 0 && mapa[(p->posy + ancho) / ancho][(p->posx + ancho - 6) / ancho] == 0)
            {
                en_aire = true;
            }
        }
    }
    else if (events.type == ALLEGRO_EVENT_KEY_DOWN) // Se presionó una tecla
    {
        switch (events.keyboard.keycode)
        {
        case ALLEGRO_KEY_D: // Tecla para ir a la derecha
            *derecha = true;
            break;

        case ALLEGRO_KEY_A: // Tecla para ir a la izquierda
            *izquierda = true;
            break;

        case ALLEGRO_KEY_SPACE: // Tecla para saltar
            if (!en_aire && (mapa[(p->posy - 2) / ancho][(p->posx + 6) / ancho] != 1 && mapa[(p->posy - 2) / ancho][(p->posx + ancho - 6) / ancho] != 1))
            {
                velocidad_y = velocidad_salto;
                en_aire = true;
            }
            break;

        case ALLEGRO_KEY_ESCAPE: // Tecla para salir
            *menu = true;
            *juego = true;
            break;
        }
    }
    else if (events.type == ALLEGRO_EVENT_KEY_UP) // Se soltó una tecla
    {
        switch (events.keyboard.keycode)
        {
        case ALLEGRO_KEY_D: // Se soltó la tecla D
            *derecha = false;
            break;

        case ALLEGRO_KEY_A: // Se soltó la tecla A
            *izquierda = false;
            break;
        }
    }
}

/*void init_bullets()
{
    bullets[MAX_BULLETS];
    // Recorrer el arreglo de disparos
    for (int i = 0; i < MAX_BULLETS; i++) {
        // Asignarle una imagen al disparo
        bullets[i].bitmap = al_load_bitmap("bala.png");

        // Asignarle una posición inicial al disparo fuera de la pantalla
        bullets[i].x = -100;
        bullets[i].y = -100;

        // Asignarle una dirección inicial al disparo
        bullets[i].direction = 0;

        // Asignarle una velocidad al disparo
        bullets[i].speed = 10;

        // Asignarle un estado inicial al disparo como inactivo
        bullets[i].active = 0;
    }
}*/

/*void fire_bullet(float x, float y, int direction)
{
    // Recorrer el arreglo de disparos
    for (int i = 0; i < MAX_BULLETS; i++) {
        // Buscar un disparo inactivo
        if (bullets[i].active == 0) {
            // Activar el disparo con los valores dados
            bullets[i].x = x;
            bullets[i].y = y;
            bullets[i].direction = direction;
            bullets[i].active = 1;

            // Salir del bucle
            break;
        }
    }
}*/

/*void draw_bullets()
{
    // Recorrer el arreglo de disparos
    for (int i = 0; i < MAX_BULLETS; i++) {
        // Verificar si el disparo está activo
        if (bullets[i].active == 1) {
            // Dibujar el disparo según su posición
            al_draw_bitmap(bullets[i].bitmap, bullets[i].x, bullets[i].y, 0);
        }
    }
}*/

void dibujarmapa(int mapa[resy / ancho][resx / ancho], ALLEGRO_BITMAP *bloque, ALLEGRO_BITMAP *puerta)
{
    FILE *map;

    if ((map = fopen("mapa.txt", "r")) == NULL)
    {
        printf("Error al abrir archivo");
    }

    for (int i = 0; i < resy / ancho; i++)
    {
        for (int j = 0; j < resx / ancho; j++)
        {
            fscanf(map, "%d", &mapa[i][j]);
        }
    }
    for (int i = 0; i < resy / ancho; i++)
    {
        for (int j = 0; j < resx / ancho; j++)
        {
            if (mapa[i][j] == 1)
            {
                al_draw_bitmap(bloque, j * ancho, i * ancho, 0);
            }

            if (mapa[i][j] == 2)
            {
                al_draw_bitmap(puerta, j * ancho, i * ancho, 0);
            }
            /*if (mapa[i][j] == 3)
            {
                al_draw_bitmap(puerta_abierta, j * ancho, i * ancho, 0);
            }*/

        }
    }
    fclose(map);
}

/*void decoraciones(mapa[resy / ancho][resx / ancho], ALLEGRO_BITMAP *puerta)
{
    FILE *map;

    if ((map = fopen("mapa.txt", "r")) == NULL)
    {
        printf("Error al abrir archivo");
    }

    for (int i = 0; i < resy / ancho; i++)
    {
        for (int j = 0; j < resx / ancho; j++)
        {
            fscanf(map, "%d", &mapa[i][j]);
        }
    }

    for (int i = 0; i < resy / ancho; i++)
    {
        for (int j = 0; j < resx / ancho; j++)
        {

            if (mapa[i][j] == 2)
            {
                al_draw_bitmap(puerta, j * ancho, i * ancho, 0);
            }

        }
    }

    fclose(map);
}*/

void dibujarenemigos(struct enemy enemigos[], int num_enemigos, ALLEGRO_BITMAP *imagen_enemigos)
{
    for (int i = 0; i < num_enemigos; i++)
    {
        al_draw_bitmap(imagen_enemigos, enemigos[i].posxe, enemigos[i].posye, 0);
    }
}

void acciones_de_enemigos(struct enemy enemigos[], int num_enemigos, int mapa[resy / ancho][resx / ancho])
{
    for (int i = 0; i < num_enemigos; i++)
    {
        if (enemigos[i].dir == 0)
        {
            enemigos[i].posxe = enemigos[i].posxe + 8; // Desplazarse a la derecha
            // Cambiar la dirección si hay colisión con un bloque
            if (enemigos[i].posxe >= resx - 50 || mapa[enemigos[i].posye / ancho][(enemigos[i].posxe + ancho) / ancho] == 1)
            {
                enemigos[i].dir = 1; // Cambiar dirección
            }
        }
        else
        {
            enemigos[i].posxe -= movespeed_e; // Desplazarse a la izquierda
            // Cambiar la dirección si hay colisión con un bloque
            if (enemigos[i].posxe <= 50 || mapa[enemigos[i].posye / ancho][(enemigos[i].posxe - ancho) / ancho] == 1)
            {
                enemigos[i].dir = 0; // Cambiar dirección
            }
        }
    }
}


void inicializar_enemigos(struct enemy enemigos[], int num_enemigos, int mapa[resy / ancho][resx / ancho])
{
    int contador_enemigos = 0;
    
    for (int i = 0; i < resy / ancho; i++)
    {
        for (int j = 0; j < resx / ancho; j++)
        {
            if (mapa[i][j] == 4) // Verificar el valor del bloque en la posición (i, j)
            {
                enemigos[contador_enemigos].posxe = j * ancho; // Multiplica la columna por el tamaño del bloque para obtener la posición en x
                enemigos[contador_enemigos].posye = i * ancho; // Multiplica la fila por el tamaño del bloque para obtener la posición en y
                enemigos[contador_enemigos].dir = 0; // Esto establecerá la dirección inicial de todos los enemigos.
                enemigos[contador_enemigos].hp = 100; // Esto establecerá los puntos de vida iniciales de todos los enemigos.
                
                contador_enemigos++;
                
                if (contador_enemigos == num_enemigos)
                {
                    return; // Si se han inicializado todos los enemigos, se sale de la función
                }
            }
        }
    }
}



/*

// Función que inicializa el sistema de audio y el addon de codecs
void init_audio() {
    // Inicializar el sistema de audio
    if (!al_install_audio()) {
        printf("Error al inicializar el audio\n");
        exit(1);
    }

    // Inicializar el addon de codecs
    if (!al_init_acodec_addon()) {
        printf("Error al inicializar el addon de codecs\n");
        exit(1);
    }
}

// Función que carga la música
void load_music(ALLEGRO_SAMPLE *musica) {
    // Cargar el archivo de sonido
    musica = al_load_sample("musica.wav");
    if (!musica) {
        printf("Error al cargar la música\n");
        exit(1);
    }
}

// Función que reproduce la música
void play_music(ALLEGRO_SAMPLE *musica) {
    // Reproducir la música en bucle
    if (!al_play_sample(musica, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, NULL)) {
        printf("Error al reproducir la música\n");
        exit(1);
    }
}

// Función que detiene la música
void stop_music() {
    // Detener todos los samples
    al_stop_samples();
}

// Función que libera la memoria del sample
void destroy_music(ALLEGRO_SAMPLE *musica) {
    // Destruir el sample
    al_destroy_sample(musica);
}

*/