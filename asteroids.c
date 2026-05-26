#include <SDL2/SDL.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

#define LARGURA 800
#define ALTURA 600
#define MAX_ASTEROIDES 8
#define PONTOS_ASTEROIDE 14
#define PI 3.14159265358979f
#define MAX_TIROS 4

typedef struct 
{
    float x,y;
    float vx,vy;
    float angulo;
} Nave;

typedef struct {
    float x, y;
    float vx, vy;
    float raio;
    float raios[PONTOS_ASTEROIDE];
    int ativo;
} Asteroide;

typedef struct
{
    float x,y;
    float vx, vy;
    int ativo;
    int vida;
    float angulo;
} Tiro;


Asteroide asteroides[MAX_ASTEROIDES];
Tiro tiros[MAX_TIROS];

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

void spawn_asteroid(Asteroide* asteroides){

    for (int i = 0; i < MAX_ASTEROIDES; i++)
    {
        int borda = rand() % 4;
        float rand_0_1 = (float)rand() / RAND_MAX;
        float rand_1_0_1 = ((float)rand() / RAND_MAX) * 2.0f - 1.0f;

        float velocidade = 1.5f;
        asteroides[i].ativo=1;
        asteroides[i].raio = 10 + (40*rand_0_1);

        for (int j = 0; j < PONTOS_ASTEROIDE; j++)
        {
            float variacao = 0.8f + ((float)rand() / RAND_MAX) * 0.4f;
            asteroides[i].raios[j] = asteroides[i].raio * variacao;
        }
        

        switch (borda)
        {
        case 0:
            asteroides[i].x = LARGURA * rand_0_1;
            asteroides[i].y = -20;
            asteroides[i].vy = velocidade * rand_0_1;
            asteroides[i].vx = velocidade * rand_1_0_1;
            break;
        case 1:
            asteroides[i].x = LARGURA * rand_0_1;
            asteroides[i].y = ALTURA + 20;
            asteroides[i].vx = velocidade * rand_1_0_1;
            asteroides[i].vy = velocidade * rand_0_1 * -1;
            break;
        case 2:
            asteroides[i].x = -20;
            asteroides[i].y = ALTURA*rand_0_1;
            asteroides[i].vy = velocidade * rand_1_0_1;
            asteroides[i].vx = velocidade * rand_0_1;
            break;
        case 3:
            asteroides[i].x = LARGURA + 20;
            asteroides[i].y = ALTURA*rand_0_1;
            asteroides[i].vy = velocidade * rand_1_0_1;
            asteroides[i].vx = velocidade * rand_0_1*-1;
            break;
        
        default:
            break;
        }
    }

}

void desenha_asteroides(SDL_Renderer* renderer, Asteroide* asteroides) {

    float angulo = (2* PI /PONTOS_ASTEROIDE);

    for (int i = 0; i < MAX_ASTEROIDES; i++) {
        if (asteroides[i].ativo) {
            // atualiza posição
            asteroides[i].x += asteroides[i].vx;
            asteroides[i].y += asteroides[i].vy;
            asteroides[i].x = fmod(asteroides[i].x + LARGURA, LARGURA);
            asteroides[i].y = fmod(asteroides[i].y + ALTURA, ALTURA);

            // desenha
            for (int j = 0; j < PONTOS_ASTEROIDE; j++)
            {
                int proximo = j+1;
                if (proximo >= PONTOS_ASTEROIDE)
                {
                    proximo = 0;
                }
                
                
                float angulo_j = j * angulo;
                float angulo_proximo = (proximo)*angulo;

                float ponto_x = asteroides[i].x + asteroides[i].raios[j] * cos(angulo_j);
                float ponto_y = asteroides[i].y + asteroides[i].raios[j] * sin(angulo_j);

                float ponto_x_proximo = asteroides[i].x + asteroides[i].raios[proximo] * cos(angulo_proximo);
                float ponto_y_proximo = asteroides[i].y + asteroides[i].raios[proximo] * sin(angulo_proximo);

                SDL_RenderDrawLine(renderer, ponto_x, ponto_y, ponto_x_proximo, ponto_y_proximo);
            }
            
        }
    }
}

void inicia_tiros(Tiro* tiros){
    for (int i = 0; i < MAX_TIROS; i++)
    {
        tiros[i].ativo = 0;
    }
}

void atira(Nave* nave, Tiro* tiros){
    for (int i = 0; i < MAX_TIROS; i++) {
        tiros[i].angulo = nave->angulo;
        if (!tiros[i].ativo) {
            tiros[i].ativo = 1;
            tiros[i].vida = 60;
            tiros[i].vx = cos(nave->angulo) * 8.0f;
            tiros[i].vy = sin(nave->angulo) * 8.0f;
            tiros[i].x = nave->x + cos(nave->angulo) * 20;
            tiros[i].y = nave->y + sin(nave->angulo) * 20;
            break;
        }
    }
}

void desenha_tiro(SDL_Renderer* renderer, Tiro* tiros, Nave* nave){
    for (int i = 0; i < MAX_TIROS; i++) {
        if (tiros[i].ativo) {
            tiros[i].x += tiros[i].vx;
            tiros[i].y += tiros[i].vy;
            tiros[i].vida--;
            if (tiros[i].vida <= 0) tiros[i].ativo = 0;
            SDL_RenderDrawLine(renderer, tiros[i].x, tiros[i].y, tiros[i].x + cos(tiros[i].angulo)*5, 
            tiros[i].y + sin(tiros[i].angulo)*5);
        }
    }
}

int main() {
    
    srand(time(NULL));
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
    int cooldown_tiro = 0;
    float velocidade = 0;

    spawn_asteroid(asteroides);
    inicia_tiros(tiros);
    

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

        velocidade = sqrt(nave.vx * nave.vx + nave.vy * nave.vy);
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
        
        if (cooldown_tiro > 0) cooldown_tiro--;

        if (cooldown_tiro > 0) cooldown_tiro--;

        if (teclado[SDL_SCANCODE_SPACE] && cooldown_tiro == 0) {
            atira(&nave, tiros);
            cooldown_tiro = 15; // espera 15 frames (~0.25s) antes do próximo
        }
        
        
        SDL_SetRenderDrawColor(renderer, 10,10,30,255);
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        desenha_nave(renderer, &nave);
        desenha_asteroides(renderer, asteroides);
        desenha_tiro(renderer, tiros, &nave);
        SDL_RenderPresent(renderer);

        SDL_Delay(16); // 1000ms / 60fps ≈ 16ms por frame
     }
     

    return 0;
}