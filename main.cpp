#include <iostream>
#include <string>
#include <GL/freeglut.h>
#include <GL/glut.h>
#include <sstream>
#include <bits/stdc++.h>
using namespace std;



int obstaculos = 8, frame_sel = 0;
string mensagem = "Clique em iniciar!";
bool espaco_liberado = false, clique_liberado = true;
int obsX[8];
GLfloat deslize = 0.0;

const int p0_sup = 100, p0_inf = p0_sup+4, largura_p0 = 1000;
const int p1_inf = p0_sup-15, p1_sup = p1_inf-4, largura_p1 = 100;
const int barra_dir = largura_p0+largura_p1-10, barra_esq = largura_p0+largura_p1-12, barra_alt = p1_sup-30;
const int LARG_JANELA = 500, ALT_JANELA = 500;
const int font = (int)GLUT_BITMAP_TIMES_ROMAN_24;

void renderBitmapString(float x, float y, void *font,const char *string){
    const char *c;
    glRasterPos2f(x, y);

    for (c=string; *c != '\0'; c++) {
        glutBitmapCharacter(font, *c);
    }
}

void desenhaTelaInicial(){
    glClear(GL_COLOR_BUFFER_BIT);

    // Desenha blocos frames
    if(frame_sel==0)
        glColor3f(1.0, 0.0, 0.0);
    else
        glColor3f(0.0, 1.0, 0.3);
    glBegin(GL_QUADS);
        glVertex2f(50, 50);
        glVertex2f(50, 150);
        glVertex2f(150, 150);
        glVertex2f(150, 50);
    glEnd();
    glColor3f(0.0, 0.20, 0.50);
    stringstream ss00;
    ss00 << "00";
    string s00 = ss00.str();
    char* c00 = (char*) s00.c_str();
    renderBitmapString(90,110,(void *)font,c00);

    if(frame_sel==30)
        glColor3f(1.0, 0.0, 0.0);
    else
        glColor3f(0.0, 1.0, 0.3);
    glBegin(GL_QUADS);
        glVertex2f(200, 50);
        glVertex2f(300, 50);
        glVertex2f(300, 150);
        glVertex2f(200, 150);
    glEnd();
    glColor3f(0.0, 0.20, 0.50);
    stringstream ss30;
    ss30 << "30";
    string s30 = ss30.str();
    char* c30 = (char*) s30.c_str();
    renderBitmapString(240,110,(void *)font,c30);

    if(frame_sel==60)
        glColor3f(1.0, 0.0, 0.0);
    else
        glColor3f(0.0, 1.0, 0.3);
    glBegin(GL_QUADS);
        glVertex2f(350, 50);
        glVertex2f(450, 50);
        glVertex2f(450, 150);
        glVertex2f(350, 150);
    glEnd();
    glColor3f(0.0, 0.20, 0.50);
    stringstream ss60;
    ss60 << "60";
    string s60 = ss60.str();
    char* c60 = (char*) s60.c_str();
    renderBitmapString(390,110,(void *)font,c60);

    // Desenha botão iniciar
    glColor3f(0.0, 1.0, 0.3);
    glBegin(GL_QUADS);
        glVertex2f(50, 200);
        glVertex2f(450, 200);
        glVertex2f(450, 300);
        glVertex2f(50, 300);
    glEnd();
    glColor3f(0.0, 0.20, 0.50);
    stringstream ssini;
    ssini << "INICIAR";
    string sini = ssini.str();
    char* cini = (char*) sini.c_str();
    renderBitmapString(200,260,(void *)font,cini);

    // Desenha texto
    glColor3f(0.0, 1.0, 0.3);
    glBegin(GL_QUADS);
        glVertex2f(50, 350);
        glVertex2f(450, 350);
        glVertex2f(450, 450);
        glVertex2f(50, 450);
    glEnd();
    glColor3f(0.0, 0.20, 0.50);
    stringstream ss;
    ss << mensagem;
    string s = ss.str();
    char* char_type = (char*) s.c_str();
    renderBitmapString(150,410,(void *)font,char_type);

    glFlush();
}

void triangulo(int i){
    glBegin(GL_POLYGON);
        glVertex2f(obsX[i], p0_sup);
        glVertex2f((obsX[i]+(obsX[i]+10))/2.0, p0_sup-10);
        glVertex2f(obsX[i]+10, p0_sup);
    glEnd();
}

void ladoAlado(int i){
    glBegin(GL_POLYGON);
        glVertex2f(obsX[i], p0_sup);
        glVertex2f((obsX[i]+(obsX[i]+10))/2.0, p0_sup-10);
        glVertex2f(obsX[i]+10, p0_sup);
    glEnd();
    glBegin(GL_POLYGON);
        glVertex2f(obsX[i]+10, p0_sup);
        glVertex2f((((obsX[i]+20)+(obsX[i]+10))/2.0), p0_sup-10);
        glVertex2f(obsX[i]+20, p0_sup);
    glEnd();
}

void emCimaDoOutro(int i){
    glBegin(GL_POLYGON);
        glVertex2f(obsX[i], p0_sup);
        glVertex2f((obsX[i]+(obsX[i]+10))/2.0, p0_sup-10);
        glVertex2f(obsX[i]+10, p0_sup);
    glEnd();
    glBegin(GL_POLYGON);
        glVertex2f(obsX[i], p0_sup-10);
        glVertex2f((obsX[i]+(obsX[i]+10))/2.0, p0_sup-20);
        glVertex2f(obsX[i]+10, p0_sup-10);
    glEnd();
}

void desenhaObstaculos(){
    glColor3f(1.0, 0.0, 0.0);
    int i = 0;
    for(int j=0;j<2;j++){
        triangulo(i++);
        ladoAlado(i++);
        emCimaDoOutro(i++);
        int x = rand() % 2;
        switch(x){
            case 0:
                triangulo(i);
                break;
            case 1:
                ladoAlado(i);
                break;
            case 2:
                emCimaDoOutro(i);
                break;
        }
        i++;
    }
}

void desenhaPersonagem(){
    glColor3f(1.0, 1.0, 0.0);
    glBegin(GL_QUADS);
        glVertex2f(10, p0_sup);
        glVertex2f(10, p0_sup-5);
        glVertex2f(20, p0_sup-5);
        glVertex2f(20, p0_sup);
    glEnd();
    glColor3f(0.0, 0.0, 1.0);
    glBegin(GL_QUADS);
        glVertex2f(10, p0_sup-5);
        glVertex2f(10, p0_sup-10);
        glVertex2f(20, p0_sup-10);
        glVertex2f(20, p0_sup-5);
    glEnd();
}

void desenhaChao() {
    glColor3f(0.0, 1.0, 0.3);
    glBegin(GL_QUADS);
        glVertex2f(0, p0_sup);
        glVertex2f(largura_p0, p0_sup);
        glVertex2f(largura_p0, p0_inf);
        glVertex2f(0, p0_inf);
    glEnd();
    glBegin(GL_POLYGON);
        glVertex2f(largura_p0, p1_sup);
        glVertex2f(largura_p0+4, p1_sup);
        glVertex2f(largura_p0+4, p0_inf);
        glVertex2f(largura_p0, p0_inf);
    glEnd();
    glBegin(GL_POLYGON);
        glVertex2f(largura_p0, p1_sup);
        glVertex2f(largura_p0+largura_p1, p1_sup);
        glVertex2f(largura_p0+largura_p1, p1_inf);
        glVertex2f(largura_p0, p1_inf);
    glEnd();
}

void desenhaChegada(){
    glColor3f(1.0, 0.8, 1.0);
    glBegin(GL_POLYGON);
        glVertex2f(barra_esq, barra_alt);
        glVertex2f(barra_dir, barra_alt);
        glVertex2f(barra_dir, p1_sup);
        glVertex2f(barra_esq, p1_sup);
    glEnd();
    glColor3f(1.0, 0.0, 1.0);
    glBegin(GL_POLYGON);
        glVertex2f(barra_dir, barra_alt);
        glVertex2f(barra_dir+5, (barra_alt+barra_alt+5)/2.0);
        glVertex2f(barra_dir, barra_alt+5);
    glEnd();
}

void desenhaJogo(){
    glClear(GL_COLOR_BUFFER_BIT);

    glTranslatef(deslize, 0.0, 0.0);
    desenhaChao();
    desenhaObstaculos();
    desenhaPersonagem();
    desenhaChegada();
    deslize -= 5;

    glFlush();
}

void pula(){
    cout<<"pulei";
}

bool colidiu(){
    return false;
}

void geraPosicoesObs() {
    int incremento = 0, n;
    for(int i=0;i<obstaculos;i++){
        n=0;
        while(n<75){
            n = rand() % 150;
        }
        incremento += n;
        obsX[i] = incremento;
    }
    if(largura_p0 - obsX[obstaculos-1] < 25)
        obsX[obstaculos-1] = 25;
}

void preparaJogo(){
    glMatrixMode(GL_MODELVIEW);
    glScalef(3.0, 3.0, 1.0);
    glutDisplayFunc(desenhaJogo);
}

void eventoMouse(int button, int state, int x, int y){
    if(clique_liberado == true){
        if(x>50 && x<150 && y>50 && y<150) frame_sel = 0;
        else if(x>200 && x<300 && y>50 && y<150) frame_sel = 30;
        else if(x>350 && x<450 && y>50 && y<150) frame_sel = 60;
        else if(x>50 && x<450 && y>200 && y<300) {
            clique_liberado = false;
            espaco_liberado = true;
            preparaJogo();
        }
    }
}

void eventoTeclado(unsigned char key, int x, int y){
    if(key == ' ' && espaco_liberado == true){
        pula();
    }
}

void alteraTamanhoJanela(GLsizei w, GLsizei h){
    glutReshapeWindow(LARG_JANELA, ALT_JANELA);
}

void inicializa(){
    // Definir sistema de coordenadas
    geraPosicoesObs();
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, LARG_JANELA, ALT_JANELA, 0, -1, 1);
    glClearColor(1.0, 1.0, 1.0, 1.0);
}

int main(int argc, char** argv)
{
    srand(time(NULL));
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    // Iniciar janela
    glutInitWindowSize(LARG_JANELA, ALT_JANELA);
    glutCreateWindow("Trabalho 2");
    // Definir funções
    glutDisplayFunc(desenhaTelaInicial);
    glutReshapeFunc(alteraTamanhoJanela);
    glutMouseFunc(eventoMouse);
    glutKeyboardFunc(eventoTeclado);
    // Controlar frames
    inicializa();
    glutMainLoop();
    return 0;
}
