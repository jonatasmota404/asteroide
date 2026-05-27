# 🚀 Asteroids

Um clone do clássico Asteroids feito do zero em **C puro** com SDL2, sem engines ou frameworks. Projeto desenvolvido para aprender os fundamentos de game development — game loop, física, colisão, renderização e áudio.

---

## 🎮 Gameplay

- Destrua asteroides antes que te destruam
- Asteroides grandes levam mais tiros e se fragmentam em pedaços menores
- Asteroides colidem entre si com física de rebatimento real
- 3 vidas com invencibilidade temporária ao ser atingido
- Pontuação acumulada por asteroide destruído

## 🕹️ Controles

| Tecla | Ação |
|-------|------|
| ← → | Rotacionar nave |
| ↑ | Acelerar |
| Espaço | Atirar |
| ESC | Pausar |

---

## 🛠️ Build

### Dependências

```bash
sudo apt install libsdl2-dev libsdl2-ttf-dev libsdl2-mixer-dev
```

### Compilar

```bash
make linux
./asteroids
```

### Instalar ícone no sistema (opcional)

Edite o arquivo `asteroids.desktop` com os caminhos absolutos corretos e copie para:

```bash
cp asteroids.desktop ~/.local/share/applications/
```

---

## 📁 Estrutura

```
asteroid/
├── src/
│   └── asteroids.c
├── assets/
│   ├── audio/
│   ├── fonts/
│   └── logo.png
├── asteroids.desktop
├── Makefile
└── .gitignore
```

---

## 🎵 Créditos de Áudio

- **Música:** [X-Future Synthwave Track Loop](https://freesound.org/people/frankum/sounds/411038/) por frankum — CC BY 3.0
- **Explosão:** explosion.wav — freesound.org
- **Tiro:** laser1.wav — freesound.org

## 🔤 Fonte

- **Lato Heavy** — [Google Fonts](https://fonts.google.com/specimen/Lato) — SIL Open Font License

---

## 👨‍💻 Autor

**jonatasmota404** — [github.com/jonatasmota404](https://github.com/jonatasmota404)

---

## 📄 Licença

MIT — faça o que quiser, só da crédito.