#include <SDL2/SDL.h>
#include <math.h>
#define LARGURA 800
#define ALTURA 600

typedef struct 
{
    float x,y;
    float vx,vy;
    float angulo;
} Nave;

void desenha_nave(SDL_Renderer* renderer, Nave* nave){
    int r_nariz = 20;
    int r_asa = 12;

    float nariz_x = nave->x + r_nariz*cos(nave->angulo);
    float nariz_y = nave->y + r_nariz*sin(nave->angulo);

    float asa_esq_x = nave->x + r_asa*cos(nave->angulo + 2.5);
    float asa_esq_y = nave->y + r_asa*sin(nave->angulo + 2.5);

    float asa_dir_x = nave->x + r_asa*cos(nave->angulo - 2.5);
    float asa_dir_y = nave->y + r_asa*sin(nave->angulo - 2.5);

    SDL_RenderDrawLine(renderer, nariz_x, nariz_y, asa_esq_x, asa_esq_y);
    SDL_RenderDrawLine(renderer, nariz_x, nariz_y, asa_dir_x, asa_dir_y);
    SDL_RenderDrawLine(renderer, asa_esq_x, asa_esq_y, asa_dir_x, asa_dir_y);

}


int main() {

    SDL_Window* janela = NULL;
    SDL_Renderer* renderer = NULL;
    SDL_Init(SDL_INIT_VIDEO);

    janela = SDL_CreateWindow("ASTEROID", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, LARGURA, ALTURA,0);
    renderer = SDL_CreateRenderer(janela, -1, SDL_RENDERER_ACCELERATED);

    int rodando = 1;
    Nave nave;
    nave.x = LARGURA / 2;
    nave.y = ALTURA / 2;
    nave.vx = 0;
    nave.vy = 0;
    nave.angulo = 0;

     while (rodando)
     {
        SDL_Event evento;

        while (SDL_PollEvent(&evento)) {
            if (evento.type == SDL_QUIT) rodando = 0;
        }
        const Uint8* teclado = SDL_GetKeyboardState(NULL);

        if (teclado[SDL_SCANCODE_LEFT])  nave.angulo -= 0.03;
        if (teclado[SDL_SCANCODE_RIGHT]) nave.angulo += 0.03;

        if (teclado[SDL_SCANCODE_UP]) {
            nave.vx += cos(nave.angulo) * 0.05;
            nave.vy += sin(nave.angulo) * 0.05;
        }

        float velocidade = sqrt(nave.vx * nave.vx + nave.vy * nave.vy);
        if (velocidade > 5.0f) {
            nave.vx = (nave.vx / velocidade) * 5.0f;
            nave.vy = (nave.vy / velocidade) * 5.0f;
        }

        nave.vx *= 0.99f;
        nave.vy *= 0.99f;
        nave.x += nave.vx;
        nave.y += nave.vy;
        nave.x = fmod(nave.x + LARGURA, LARGURA);
        nave.y = fmod(nave.y + ALTURA, ALTURA);
        
        SDL_SetRenderDrawColor(renderer, 10,10,30,255);
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        desenha_nave(renderer, &nave);
        SDL_RenderPresent(renderer);

        SDL_Delay(16); // 1000ms / 60fps ≈ 16ms por frame
     }
     

    return 0;
}