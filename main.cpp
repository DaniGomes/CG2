#include <iostream>
#include <string>
#include <GL/freeglut.h>
#include <GL/glut.h>
#include <sstream>
#include <bits/stdc++.h>
#include <irrKlang.h>
using namespace std;
//using namespace irrklang;

irrklang::ISoundEngine* SoundEngine = irrklang::createIrrKlangDevice();

const int obstaculos = 8;
const int p0_sup = 100, p0_inf = p0_sup+4, largura_p0 = 1000;
const int p1_inf = p0_sup-15, p1_sup = p1_inf-4, largura_p1 = 100;
const int barra_dir = largura_p0+largura_p1-10, barra_esq = largura_p0+largura_p1-12, barra_alt = p1_sup-30;
const int LARG_JANELA = 500, ALT_JANELA = 500;
const int font = (int)GLUT_BITMAP_TIMES_ROMAN_24;
const float gravidade = 10.0, tempo_pulo = 3000;

int tempo_novo, tempo_antigo, inicio_pulo;
int frame_sel = 30, ob = 0;
string mensagem = "Clique em iniciar!";
bool espaco_liberado = false, clique_liberado = true, fim_jogo = false, fim = false;
int obsX[8], per_x = 10, per_y = p0_sup, tipos_obs[8];
GLfloat deslize = 0.0, angulo = 0.0, pulo = 0.0, inc_ang = 0.0, diminui = 1.0;

int pulando = 0, vel = 8, bonus = 0;
const int MAX_HEIGHT=40;
GLint jcount=0;
GLfloat height=0.0;

void reset(){
    fim = false;
    bonus = 0;
    frame_sel = 30;
    espaco_liberado = false;
    clique_liberado = true;
    fim_jogo = false;
    per_x = 10;
    per_y = p0_sup;
    for(int i=0; i<8; i++){
        obsX[i] = 0;
        tipos_obs[i] = 0;
    }
    deslize = 0.0;
    angulo = 0.0;
    pulo = 0.0;
    inc_ang = 0.0;
    pulando = 0;
    vel = 8;
    jcount=0;
    height=0.0;
    ob=0;
    diminui=1.0;
}

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

    glutSwapBuffers();
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
    glPushMatrix();
    glTranslatef(deslize, 0.0, 0.0);
    glColor3f(1.0, 0.0, 0.0);
    int i = 0;
    for(int j=0;j<2;j++){
        tipos_obs[i] = 0;
        triangulo(i++);
        tipos_obs[i] = 1;
        ladoAlado(i++);
        tipos_obs[i] = 2;
        emCimaDoOutro(i++);
        int x = rand() % 2;
        switch(x){
            case 0:
                tipos_obs[i] = 0;
                triangulo(i);
                break;
            case 1:
                tipos_obs[i] = 1;
                ladoAlado(i);
                break;
            case 2:
                tipos_obs[i] = 2;
                emCimaDoOutro(i);
                break;
        }
        i++;
    }
    glPopMatrix();
}

void desenhaPersonagem(){
    glPushMatrix();
    glTranslatef(per_x+5, per_y-5, 0);
    glRotatef(angulo, 0.0, 0.0, 1.0);
    glScalef(diminui, diminui, 1.0);
    glTranslatef(-per_x-5, -per_y+5, 0);

    glColor3f(1.0, 1.0, 0.0);
    glBegin(GL_QUADS);
        glVertex2f(per_x, per_y);
        glVertex2f(per_x, per_y-5);
        glVertex2f(per_x+10, per_y-5);
        glVertex2f(per_x+10, per_y);
    glEnd();
    glColor3f(0.0, 0.0, 1.0);
    glBegin(GL_QUADS);
        glVertex2f(per_x, per_y-5);
        glVertex2f(per_x, per_y-10);
        glVertex2f(per_x+10, per_y-10);
        glVertex2f(per_x+10, per_y-5);
    glEnd();
    glPopMatrix();
}

void desenhaChao() {
    glPushMatrix();
    glTranslatef(deslize, 0.0, 0.0);
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
    glPopMatrix();
}

void desenhaChegada(){
    glPushMatrix();
    glTranslatef(deslize, 0.0, 0.0);
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
    glPopMatrix();
}

void desenhaJogo(){

    glClear(GL_COLOR_BUFFER_BIT);

    glPushMatrix();
    glScalef(3.0, 3.0, 1.0);

    if(pulando!=0)
    {
        glTranslatef(0.0,-height/2, 0.0);
        glPushMatrix();
        glTranslatef(per_x+5, per_y-5, 0);
        glTranslatef(0.0,-height,0.0);
        glTranslatef(-per_x-5, -per_y+5, 0);
        desenhaPersonagem();
        glPopMatrix();
        glPushMatrix();
        glTranslatef(0.0, bonus, 0.0);
        desenhaChao();
        desenhaObstaculos();
        desenhaChegada();
        glPopMatrix();
    }else if(pulando == 0){
        desenhaPersonagem();
        desenhaChao();
        desenhaObstaculos();
        desenhaChegada();
    }

    glPopMatrix();

    glutSwapBuffers();
}

bool colidiu(){

    if(ob<8){

        if(tipos_obs[ob]==0){
            if( (obsX[ob]+deslize <= per_x+10 && per_x+10 <= obsX[ob]+10+deslize ) &&
              ( (p0_sup >= per_y - height && per_y - height >= p0_sup-10) || (p0_sup >= per_y - height-10 && per_y - height-10 >= p0_sup-10)) )
            {
                return true;
            }
            if( (per_x <= obsX[ob]+10+deslize && obsX[ob]+10+deslize <= per_x+10 ) &&
              ( (per_y - height >= p0_inf && p0_inf >= per_y - height-10) || (per_y - height >= p0_sup && p0_sup >= per_y - height-10)) )
            {
                return true;
            }

        }else if(tipos_obs[ob]==1){
            if( (obsX[ob]+deslize <= per_x+10 && per_x+10 <= obsX[ob]+20+deslize ) &&
              ( (p0_sup >= per_y - height && per_y - height >= p0_sup-10) || (p0_sup >= per_y - height-10 && per_y - height-10 >= p0_sup-10)) )
            {
                return true;
            }
            if( (per_x <= obsX[ob]+20+deslize && obsX[ob]+20+deslize <= per_x+10 ) &&
              ( (per_y - height >= p0_inf && p0_inf >= per_y - height-10) || (per_y - height >= p0_sup && p0_sup >= per_y - height-10)) )
            {
                return true;
            }
        }else if(tipos_obs[ob]==2){
            if( (obsX[ob]+deslize <= per_x+10 && per_x+10 <= obsX[ob]+10+deslize ) &&
              ( (p0_sup >= per_y - height && per_y - height >= p0_sup-20) || (p0_sup >= per_y - height-10 && per_y - height-10 >= p0_sup-20)) )
            {
                return true;
            }
            if( (per_x <= obsX[ob]+10+deslize && obsX[ob]+10+deslize <= per_x+10 ) &&
              ( (per_y - height >= p0_inf && p0_inf >= per_y - height-10) || (per_y - height >= p0_sup-20 && p0_sup-20 >= per_y - height-10)) )
            {
                return true;
            }
        }
    }

    else
        if( (largura_p0+deslize <= per_x+10 && per_x+10 <= largura_p0+deslize+4 ) &&
          ( (p0_inf >= per_y - height && per_y - height >= p1_sup) || (p0_inf >= per_y - height-10 && per_y - height-10 >= p1_sup)) )
        {
            return true;
        }

    if(obsX[ob]+deslize<0)
        ob++;

    return false;
}

void subindo(){
	if(pulando==1 && height<MAX_HEIGHT){
        height+=1;
        inc_ang += 1.125;
	}
	else if(pulando==1 && (int)height==MAX_HEIGHT)
		pulando=-1;
}

void descendo(){
	if(pulando==-1 && height>0){
        height-=1;
        inc_ang += 1.125;
	}
	else if(pulando==-1 && (int)height==0){
		pulando=0;
		jcount--;
	}
}

void pula(int novo_t){
    if(espaco_liberado == true){
        angulo = inc_ang;
        subindo();
        descendo();
    }
    glutTimerFunc(vel,pula,10);
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

    if(obsX[0] - per_x < 50)
        obsX[0] = +50;

    if(largura_p0 - obsX[obstaculos-1] < 50)
        obsX[obstaculos-1] -= 50;
}

void preparaJogo(){
    geraPosicoesObs();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glutDisplayFunc(desenhaJogo);
}

void finalizaJogo(bool ganhou){
    if(ganhou){
        mensagem = "Voce ganhou!";
    }
    else{
        mensagem = "Voce perdeu!";
    }
    reset();
    glutDisplayFunc(desenhaTelaInicial);
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
    if(key==' '){
        if(jcount==0)
		{
			pulando = 1;
			jcount++;
		}
    }
}

void alteraTamanhoJanela(GLsizei w, GLsizei h){
    glutReshapeWindow(LARG_JANELA, ALT_JANELA);
}

void inicializa(){
    // Definir sistema de coordenadas
    SoundEngine->play2D("unforgettable.ogg", GL_TRUE);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, LARG_JANELA, ALT_JANELA, 0, -1, 1);
    glClearColor(1.0, 1.0, 1.0, 1.0);
}

int main(int argc, char** argv){
    srand(time(NULL));
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    // Iniciar janela
    glutInitWindowSize(LARG_JANELA, ALT_JANELA);
    glutCreateWindow("Trabalho 2");
    // Definir funções
    glutDisplayFunc(desenhaTelaInicial);
    glutReshapeFunc(alteraTamanhoJanela);
    glutMouseFunc(eventoMouse);
    glutKeyboardFunc(eventoTeclado);
    glutTimerFunc(vel,pula,10);
    // Controlar frames
    inicializa();
    tempo_novo = glutGet(GLUT_ELAPSED_TIME);
    tempo_antigo = tempo_novo;
    while(!fim_jogo){
        tempo_novo = glutGet(GLUT_ELAPSED_TIME);
        float delta_tempo = (float)(tempo_novo - tempo_antigo);
        if(frame_sel==0 || delta_tempo > 1000/frame_sel){
            tempo_antigo = tempo_novo;
            if(espaco_liberado && !fim){
                deslize -= 0.9;
                if(per_x+10 >= largura_p0 + deslize && !colidiu()){
                    per_y = p1_sup;
                }
                if(per_x+5 >= barra_esq + deslize)
                    finalizaJogo(true);
                if(colidiu()){
                    fim = true;
                }
            }
            if(fim){
                diminui -= 0.05;
                if(diminui <= 0)
                    finalizaJogo(false);
            }
            glutMainLoopEvent();
            glutPostRedisplay();
        }
    }

    return 0;
}
