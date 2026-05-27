#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

#define LARGURA 800
#define ALTURA 600
#define MAX_ASTEROIDES 8
#define PONTOS_ASTEROIDE 14
#define PI 3.14159265358979f
#define MAX_TIROS 4
#define MAX_PARTICULAS 80
#define ESTADO_MENU 0
#define ESTADO_JOGANDO 1
#define ESTADO_GAME_OVER 2
#define ESTADO_PAUSADO 3
#define MAX_ESTRELAS 150
#define FONTE "assets/fonts/Lato-Heavy.ttf"


typedef struct 
{
    float x,y;
    float vx,vy;
    float angulo;
    int vidas;
    int invencivel;
} Nave;

typedef struct {
    float x, y;
    float vx, vy;
    float raio;
    float raios[PONTOS_ASTEROIDE];
    int ativo;
    int vida;
    int tamanho;
} Asteroide;

typedef struct
{
    float x,y;
    float vx, vy;
    int ativo;
    int vida;
    float angulo;
} Tiro;

typedef struct {
    float x, y;
    float vx, vy;
    int vida;
    int ativo;
} Particula;

typedef struct {
    float x, y;
    float velocidade;
    int brilho; // 100-255, estrelas distantes são mais fracas
} Estrela;


Estrela estrelas[MAX_ESTRELAS];
Particula particulas[MAX_PARTICULAS];
Asteroide asteroides[MAX_ASTEROIDES];
Tiro tiros[MAX_TIROS];

void desenha_nave(SDL_Renderer* renderer, Nave* nave){
    int r_nariz = 12;
    int r_asa = 12;
    float angulo_calda_x = nave->x + cos(nave->angulo);
    float angulo_calda_y = nave->y + sin(nave->angulo);


    float nariz_x = nave->x + r_nariz*cos(nave->angulo);
    float nariz_y = nave->y + r_nariz*sin(nave->angulo);

    float asa_esq_x = nave->x + r_asa*cos(nave->angulo + 2.5);
    float asa_esq_y = nave->y + r_asa*sin(nave->angulo + 2.5);

    float asa_dir_x = nave->x + r_asa*cos(nave->angulo - 2.5);
    float asa_dir_y = nave->y + r_asa*sin(nave->angulo - 2.5);

    SDL_RenderDrawLine(renderer, nariz_x, nariz_y, asa_esq_x, asa_esq_y);
    SDL_RenderDrawLine(renderer, nariz_x, nariz_y, asa_dir_x, asa_dir_y);
    SDL_RenderDrawLine(renderer, asa_esq_x, asa_esq_y, angulo_calda_x,angulo_calda_y);
    SDL_RenderDrawLine(renderer, angulo_calda_x, angulo_calda_y, asa_dir_x, asa_dir_y);
}

void inicia_asteroides(Asteroide* asteroides){
    for (int i = 0; i < MAX_ASTEROIDES; i++)
    {
        asteroides[i].ativo = 0;
    }
    
}

void spawn_asteroid(Asteroide* asteroides){
    
    for (int i = 0; i < MAX_ASTEROIDES; i++) {
        if (!asteroides[i].ativo) {
            int borda = rand() % 4;
            float rand_0_1 = (float)rand() / RAND_MAX;
            float rand_1_0_1 = ((float)rand() / RAND_MAX) * 2.0f - 1.0f;

            float velocidade = 1.5f;
            asteroides[i].ativo=1;
            asteroides[i].tamanho = 1 + rand() % 3; 
            asteroides[i].vida = asteroides[i].tamanho;

            if (asteroides[i].tamanho == 3) asteroides[i].raio = 35 + ((float)rand()/RAND_MAX) * 15;
            if (asteroides[i].tamanho == 2) asteroides[i].raio = 20 + ((float)rand()/RAND_MAX) * 14;
            if (asteroides[i].tamanho == 1) asteroides[i].raio = 8  + ((float)rand()/RAND_MAX) * 11;
            

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
            break;
        } 
    }
    

}

void spawna_fragmento(float x, float y, int tamanho, Asteroide* asteroides) {
    for (int i = 0; i < MAX_ASTEROIDES; i++) {
        if (!asteroides[i].ativo) {
            float angulo = ((float)rand() / RAND_MAX) * 2 * PI;
            float velocidade = 1.5f + ((float)rand() / RAND_MAX) * 1.5f;

            asteroides[i].ativo = 1;
            asteroides[i].tamanho = tamanho;
            asteroides[i].vida = tamanho;
            asteroides[i].x = x;
            asteroides[i].y = y;
            asteroides[i].vx = cos(angulo) * velocidade;
            asteroides[i].vy = sin(angulo) * velocidade;

            if (tamanho == 2) asteroides[i].raio = 20 + ((float)rand()/RAND_MAX) * 14;
            if (tamanho == 1) asteroides[i].raio = 8  + ((float)rand()/RAND_MAX) * 11;

            for (int j = 0; j < PONTOS_ASTEROIDE; j++) {
                float variacao = 0.8f + ((float)rand() / RAND_MAX) * 0.4f;
                asteroides[i].raios[j] = asteroides[i].raio * variacao;
            }
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

                if (asteroides[i].vida == asteroides[i].tamanho) {
                    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // intacto = branco
                } else if (asteroides[i].vida == 2) {
                    SDL_SetRenderDrawColor(renderer, 255, 200, 100, 255); // amarelado
                } else {
                    SDL_SetRenderDrawColor(renderer, 255, 80, 80, 255);   // vermelho = quase morto
                }

                SDL_RenderDrawLine(renderer, ponto_x, ponto_y, ponto_x_proximo, ponto_y_proximo);
            }
        }
    }
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
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

void spawna_explosao(float x, float y, Particula* particulas) {
    int count = 0;
    for (int i = 0; i < MAX_PARTICULAS && count < 10; i++) {
        if (!particulas[i].ativo) {
            float angulo = ((float)rand() / RAND_MAX) * 2 * PI;
            float velocidade = 1.0f + ((float)rand() / RAND_MAX) * 3.0f;
            particulas[i].ativo = 1;
            particulas[i].vida = 30;
            particulas[i].x = x;
            particulas[i].y = y;
            particulas[i].vx = cos(angulo) * velocidade;
            particulas[i].vy = sin(angulo) * velocidade;
            count++;
        }
    }
}

void verifica_colisao_tiro_asteroide(Tiro* tiros, Asteroide* asteroides, Particula* particulas, int* pontos, Mix_Chunk* som_explosao){
    float distancia;
    float dx;
    float dy;

    for (int i = 0; i < MAX_TIROS; i++)
    {
        if (tiros[i].ativo)
        {
            for (int j = 0; j < MAX_ASTEROIDES; j++)
            {
                if (asteroides[j].ativo)
                {
                    dx = tiros[i].x - asteroides[j].x;
                    dy = tiros[i].y - asteroides[j].y;
                    
                    distancia = sqrt(dx*dx + dy*dy);

                    
                    if (distancia < asteroides[j].raio)
                    {
                        asteroides[j].vida--;
                        tiros[i].ativo = 0;
                        if (asteroides[j].vida <= 0) 
                        {
                            asteroides[j].ativo = 0;
                            spawna_explosao(asteroides[j].x,asteroides[j].y, particulas);
                            Mix_PlayChannel(-1, som_explosao, 0);
                            if (asteroides[j].tamanho == 3)
                            {
                                int modo = rand() % 3;
                                if (modo == 0) { // dois tamanho 2
                                    spawna_fragmento(asteroides[j].x, asteroides[j].y, 2, asteroides);
                                    spawna_fragmento(asteroides[j].x, asteroides[j].y, 2, asteroides);
                                } else if (modo == 1) { // um 2 e um 1
                                    spawna_fragmento(asteroides[j].x, asteroides[j].y, 2, asteroides);
                                    spawna_fragmento(asteroides[j].x, asteroides[j].y, 1, asteroides);
                                } else { // três tamanho 1
                                    spawna_fragmento(asteroides[j].x, asteroides[j].y, 1, asteroides);
                                    spawna_fragmento(asteroides[j].x, asteroides[j].y, 1, asteroides);
                                    spawna_fragmento(asteroides[j].x, asteroides[j].y, 1, asteroides);
                                }
                            } else if (asteroides[j].tamanho == 2) 
                            {
                                spawna_fragmento(asteroides[j].x, asteroides[j].y, 1, asteroides);
                                spawna_fragmento(asteroides[j].x, asteroides[j].y, 1, asteroides);
                            }
                            (*pontos)++;
                        }
                    }
                    
                }
            }
            
        }
        
    }
    
}

void desenha_particulas(SDL_Renderer* renderer, Particula* particulas){
    for (int i = 0; i < MAX_PARTICULAS; i++)
    {
        if (particulas[i].ativo) {
            SDL_RenderDrawPoint(renderer, particulas[i].x, particulas[i].y);
            particulas[i].x += particulas[i].vx;
            particulas[i].y += particulas[i].vy;
            particulas[i].vida--;
            if (particulas[i].vida <= 0) particulas[i].ativo = 0;
        }
    }
}

int verifica_colisao_nave(Nave* nave, Asteroide* asteroides){
    if (nave->invencivel > 0) return 0;
    float distancia;
    float dx;
    float dy;

    for (int i = 0; i < MAX_ASTEROIDES; i++)
    {
        if (asteroides[i].ativo)
        {
            dx = nave->x - asteroides[i].x;
            dy = nave->y - asteroides[i].y;
            
            distancia = sqrt(dx*dx + dy*dy);  

            if (distancia < asteroides[i].raio + 11)
            {
                nave->vidas--;
                nave->invencivel = 120;
                return 1;
            }
        }
    }

    return 0;
}

void desenha_pontos_vidas(SDL_Renderer* renderer, TTF_Font* fonte ,int pontos, int vidas) {
    char texto_pontos[32];
    char texto_vidas[32];
    sprintf(texto_pontos, "Pontos: %d", pontos);
    sprintf(texto_vidas, "Vidas: %d", vidas);


    SDL_Color branco = {255, 255, 255, 255};
    SDL_Surface* surface_pontos = TTF_RenderText_Solid(fonte, texto_pontos, branco);
    SDL_Surface* surface_vidas = TTF_RenderText_Solid(fonte, texto_vidas, branco);

    SDL_Texture* texture_pontos = SDL_CreateTextureFromSurface(renderer, surface_pontos);
    SDL_Texture* texture_vidas = SDL_CreateTextureFromSurface(renderer, surface_vidas);

    SDL_FreeSurface(surface_pontos); // surface virou texture, pode liberar
    SDL_FreeSurface(surface_vidas);

    SDL_Rect dest_pontos = {15, 10, 100, 30}; // x, y, largura, altura
    SDL_Rect dest_vidas = {15, 50, 100, 20};

    SDL_RenderCopy(renderer, texture_pontos, NULL, &dest_pontos);
    SDL_RenderCopy(renderer, texture_vidas, NULL, &dest_vidas);

    SDL_DestroyTexture(texture_pontos); // libera após usar
    SDL_DestroyTexture(texture_vidas);
}

void reinicia_jogo(Nave* nave, int* pontos, int* timer_spawn, int* intervalo_spawn, int* cooldown_tiro) {
    nave->x = LARGURA / 2;
    nave->y = ALTURA / 2;
    nave->vx = nave->vy = nave->angulo = 0;
    nave->vidas = 3;
    nave->invencivel = 0;
    *pontos = 0;
    *timer_spawn = 0;
    *intervalo_spawn = 30 + rand() % 180;
    *cooldown_tiro = 0;
    inicia_asteroides(asteroides);
    inicia_tiros(tiros);
}

void verifica_colisao_asteroides(Asteroide* asteroides) {
    for (int i = 0; i < MAX_ASTEROIDES; i++) {
        for (int j = i + 1; j < MAX_ASTEROIDES; j++) {
            if (asteroides[i].ativo && asteroides[j].ativo) 
            {
                float dx = asteroides[j].x - asteroides[i].x;
                float dy = asteroides[j].y - asteroides[i].y;
                float distancia = sqrt(dx*dx + dy*dy);

                if (distancia < asteroides[i].raio + asteroides[j].raio)
                {
                    float nx = dx/distancia;
                    float ny = dy/distancia;
                    float dvx = asteroides[i].vx - asteroides[j].vx;
                    float dvy = asteroides[i].vy - asteroides[j].vy;
                    float impulso = dvx*nx + dvy*ny;

                    float sobreposicao = (asteroides[i].raio + asteroides[j].raio) - distancia;
                    asteroides[i].x -= nx * sobreposicao * 0.5f;
                    asteroides[i].y -= ny * sobreposicao * 0.5f;
                    asteroides[j].x += nx * sobreposicao * 0.5f;
                    asteroides[j].y += ny * sobreposicao * 0.5f;

                    if (impulso > 0)
                    {
                        asteroides[i].vx -= impulso * nx;
                        asteroides[i].vy -= impulso * ny;
                        asteroides[j].vx += impulso * nx;
                        asteroides[j].vy += impulso * ny;
                    }
                    
                    
                }
            }
        }
    }
}

void inicia_estrelas(Estrela* estrelas){
    for (int i = 0; i < MAX_ESTRELAS; i++)
    {
        estrelas[i].x = ((float)rand() / RAND_MAX) * LARGURA;
        estrelas[i].y = ((float)rand() / RAND_MAX) * ALTURA;
        estrelas[i].velocidade = 0.1f + ((float)rand() / RAND_MAX) * 0.4f;
        estrelas[i].brilho = 100 + ((float)rand() / RAND_MAX) * 155;
        
    }
    
}

void desenha_estrelas(SDL_Renderer* renderer, Estrela* estrelas) {
    for (int i = 0; i < MAX_ESTRELAS; i++) {
        // move
        estrelas[i].y += estrelas[i].velocidade;
        estrelas[i].x += estrelas[i].velocidade * 0.3f;

        // wrap
        if (estrelas[i].y > ALTURA) estrelas[i].y = 0;
        if (estrelas[i].x > LARGURA) estrelas[i].x = 0;

        // desenha com brilho variável
        SDL_SetRenderDrawColor(renderer, 
            estrelas[i].brilho, 
            estrelas[i].brilho, 
            estrelas[i].brilho, 
            255);
        SDL_RenderDrawPoint(renderer, estrelas[i].x, estrelas[i].y);
    }
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
}

int main() {
    
    TTF_Init();
    TTF_Font* fonte = TTF_OpenFont(FONTE, 24);
    srand(time(NULL));
    SDL_Window* janela = NULL;
    SDL_Renderer* renderer = NULL;
    SDL_Init(SDL_INIT_VIDEO);

    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

    Mix_Music* musica = Mix_LoadMUS("assets/audio/411038__frankum__x-future-synthwave-track-loop.mp3");
    Mix_Chunk* som_tiro = Mix_LoadWAV("assets/audio/laser1.wav");
    Mix_Chunk* som_explosao = Mix_LoadWAV("assets/audio/explosion.wav");

    Mix_PlayMusic(musica, -1); // loop infinito
    Mix_VolumeMusic(64); // volume 50% (0-128)

    janela = SDL_CreateWindow("ASTEROID", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, LARGURA, ALTURA,0);
    renderer = SDL_CreateRenderer(janela, -1, SDL_RENDERER_ACCELERATED);

    int rodando = 1;
    int timer_spawn = 0;
    int intervalo_spawn = 30 + rand() % 180;
    int cooldown_tiro = 0;
    float velocidade = 0;
    int pontos = 0;
    int estado = ESTADO_MENU;
    int opcao_selecionada = 0;

    Nave nave;
    nave.x = LARGURA / 2;
    nave.y = ALTURA / 2;
    nave.vx = 0;
    nave.vy = 0;
    nave.angulo = 0;
    nave.vidas = 3;
    nave.invencivel = 0;
    
    inicia_estrelas(estrelas);
    inicia_asteroides(asteroides);
    inicia_tiros(tiros);
    

     while (rodando)
     {

        SDL_Event evento;

        while (SDL_PollEvent(&evento)) {
            if (evento.type == SDL_QUIT) rodando = 0;
            
            if (evento.type == SDL_KEYDOWN) {
                // pausa e despausa com ESC
                if (estado == ESTADO_JOGANDO && evento.key.keysym.sym == SDLK_ESCAPE)
                    estado = ESTADO_PAUSADO;

                // menu de pausa
                if (estado == ESTADO_PAUSADO) {
                    if (evento.key.keysym.sym == SDLK_UP)   opcao_selecionada = 0;
                    if (evento.key.keysym.sym == SDLK_DOWN) opcao_selecionada = 1;
                    if (evento.key.keysym.sym == SDLK_RETURN) {
                        if (opcao_selecionada == 0) estado = ESTADO_JOGANDO;
                        if (opcao_selecionada == 1) rodando = 0;
                    }
                }

                // menu principal e game over
                if (estado == ESTADO_MENU || estado == ESTADO_GAME_OVER) {
                    if (evento.key.keysym.sym == SDLK_UP)   opcao_selecionada = 0;
                    if (evento.key.keysym.sym == SDLK_DOWN) opcao_selecionada = 1;
                    if (evento.key.keysym.sym == SDLK_RETURN) {
                        if (opcao_selecionada == 0) {
                            reinicia_jogo(&nave, &pontos, &timer_spawn, &intervalo_spawn, &cooldown_tiro);
                            estado = ESTADO_JOGANDO;
                        }
                        if (opcao_selecionada == 1) rodando = 0;
                    }
                }
            }
        }
        const Uint8* teclado = SDL_GetKeyboardState(NULL);

        if (estado == ESTADO_MENU)
        {
            // desenho
            SDL_Color branco  = {255, 255, 255, 255};
            SDL_Color amarelo = {255, 220, 50,  255};

            // título
            SDL_Surface* s = TTF_RenderText_Solid(fonte, "ASTEROIDS", branco);
            SDL_Texture* t = SDL_CreateTextureFromSurface(renderer, s);
            SDL_FreeSurface(s);
            SDL_Rect r = {280, 180, 240, 50};
            SDL_RenderCopy(renderer, t, NULL, &r);
            SDL_DestroyTexture(t);

            // opção jogar
            SDL_Color cor_jogar = (opcao_selecionada == 0) ? amarelo : branco;
            s = TTF_RenderText_Solid(fonte, "> JOGAR", cor_jogar);
            t = SDL_CreateTextureFromSurface(renderer, s);
            SDL_FreeSurface(s);
            r = (SDL_Rect){320, 300, 160, 35};
            SDL_RenderCopy(renderer, t, NULL, &r);
            SDL_DestroyTexture(t);

            // opção sair
            SDL_Color cor_sair = (opcao_selecionada == 1) ? amarelo : branco;
            s = TTF_RenderText_Solid(fonte, "> SAIR", cor_sair);
            t = SDL_CreateTextureFromSurface(renderer, s);
            SDL_FreeSurface(s);
            r = (SDL_Rect){320, 350, 130, 35};
            SDL_RenderCopy(renderer, t, NULL, &r);
            SDL_DestroyTexture(t);
        }
        
        if (estado == ESTADO_JOGANDO)
        {
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
            
            int tem_asteroide = 0;
            for (int i = 0; i < MAX_ASTEROIDES; i++) {
                if (asteroides[i].ativo) { tem_asteroide = 1; break; }
            }
            if (!tem_asteroide) {
                for (int i = 0; i < 4; i++) spawn_asteroid(asteroides);
                timer_spawn = 0;
                intervalo_spawn = 30 + rand() % 180;
            }
            if (timer_spawn >= intervalo_spawn) {
                spawn_asteroid(asteroides);
                timer_spawn = 0;
                intervalo_spawn = 30 + rand() % 180;
            }

            if (cooldown_tiro > 0) cooldown_tiro--;

            if (teclado[SDL_SCANCODE_SPACE] && cooldown_tiro == 0) {
                atira(&nave, tiros);
                Mix_PlayChannel(-1, som_tiro, 0);
                cooldown_tiro = 15; // espera 15 frames (~0.25s) antes do próximo
            }

            verifica_colisao_tiro_asteroide(tiros, asteroides, particulas, &pontos, som_explosao);
            verifica_colisao_asteroides(asteroides);
            
            if (verifica_colisao_nave(&nave, asteroides))
            {
                if (nave.vidas <= 0)
                {
                    estado = ESTADO_GAME_OVER;
                }
                
            }
            
            SDL_SetRenderDrawColor(renderer, 10,10,30,255);
            SDL_RenderClear(renderer);
            desenha_estrelas(renderer, estrelas);
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            if (nave.invencivel == 0 || nave.invencivel % 8 < 4) desenha_nave(renderer, &nave);
            desenha_asteroides(renderer, asteroides);
            desenha_pontos_vidas(renderer, fonte, pontos, nave.vidas);
            desenha_tiro(renderer, tiros, &nave);
            desenha_particulas(renderer, particulas);

            if (nave.invencivel > 0) nave.invencivel--;
            timer_spawn++;
        }
        
        if (estado == ESTADO_PAUSADO) {
            SDL_SetRenderDrawColor(renderer, 10, 10, 30, 255);
            SDL_RenderClear(renderer);
            desenha_estrelas(renderer, estrelas);
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            desenha_nave(renderer, &nave);
            desenha_asteroides(renderer, asteroides);
            desenha_tiro(renderer, tiros, &nave);
            desenha_particulas(renderer, particulas);
            desenha_pontos_vidas(renderer, fonte, pontos, nave.vidas);

            SDL_Color amarelo = {255, 220, 50, 255};
            SDL_Color branco  = {255, 255, 255, 255};

            SDL_Surface* s = TTF_RenderText_Solid(fonte, "PAUSADO", amarelo);
            SDL_Texture* t = SDL_CreateTextureFromSurface(renderer, s);
            SDL_FreeSurface(s);
            SDL_Rect r = {330, 220, 140, 45};
            SDL_RenderCopy(renderer, t, NULL, &r);
            SDL_DestroyTexture(t);

            SDL_Color cor_retomar = (opcao_selecionada == 0) ? amarelo : branco;
            s = TTF_RenderText_Solid(fonte, "> RETOMAR", cor_retomar);
            t = SDL_CreateTextureFromSurface(renderer, s);
            SDL_FreeSurface(s);
            r = (SDL_Rect){310, 300, 180, 35};
            SDL_RenderCopy(renderer, t, NULL, &r);
            SDL_DestroyTexture(t);

            SDL_Color cor_sair = (opcao_selecionada == 1) ? amarelo : branco;
            s = TTF_RenderText_Solid(fonte, "> SAIR", cor_sair);
            t = SDL_CreateTextureFromSurface(renderer, s);
            SDL_FreeSurface(s);
            r = (SDL_Rect){320, 350, 130, 35};
            SDL_RenderCopy(renderer, t, NULL, &r);
            SDL_DestroyTexture(t);
        }

        if (estado == ESTADO_GAME_OVER)
        {
            // desenho
            SDL_Color branco  = {255, 255, 255, 255};
            SDL_Color amarelo = {255, 220, 50,  255};
            SDL_Color vermelho = {255, 80, 80, 255};

            // game over
            SDL_Surface* s = TTF_RenderText_Solid(fonte, "GAME OVER", vermelho);
            SDL_Texture* t = SDL_CreateTextureFromSurface(renderer, s);
            SDL_FreeSurface(s);
            SDL_Rect r = {280, 180, 240, 50};
            SDL_RenderCopy(renderer, t, NULL, &r);
            SDL_DestroyTexture(t);

            // pontuação
            char texto[32];
            sprintf(texto, "Pontos: %d", pontos);
            s = TTF_RenderText_Solid(fonte, texto, branco);
            t = SDL_CreateTextureFromSurface(renderer, s);
            SDL_FreeSurface(s);
            r = (SDL_Rect){320, 250, 160, 35};
            SDL_RenderCopy(renderer, t, NULL, &r);
            SDL_DestroyTexture(t);

            // opções
            SDL_Color cor_reiniciar = (opcao_selecionada == 0) ? amarelo : branco;
            s = TTF_RenderText_Solid(fonte, "> REINICIAR", cor_reiniciar);
            t = SDL_CreateTextureFromSurface(renderer, s);
            SDL_FreeSurface(s);
            r = (SDL_Rect){300, 330, 200, 35};
            SDL_RenderCopy(renderer, t, NULL, &r);
            SDL_DestroyTexture(t);

            SDL_Color cor_sair = (opcao_selecionada == 1) ? amarelo : branco;
            s = TTF_RenderText_Solid(fonte, "> SAIR", cor_sair);
            t = SDL_CreateTextureFromSurface(renderer, s);
            SDL_FreeSurface(s);
            r = (SDL_Rect){320, 380, 130, 35};
            SDL_RenderCopy(renderer, t, NULL, &r);
            SDL_DestroyTexture(t);
        }
        
        SDL_RenderPresent(renderer);
        SDL_Delay(16); // 1000ms / 60fps ≈ 16ms por frame
     }
     
    Mix_FreeChunk(som_tiro);
    Mix_FreeChunk(som_explosao);
    Mix_FreeMusic(musica);
    Mix_CloseAudio();
    return 0;
}