#include <stdbool.h>
#include <math.h>
#include <GL/glut.h>

// Variáveis de controle de rotação, translação e zoom
GLfloat rotationX = 0.0, rotationY = 0.0, translateX = 0.0, translateY = 0.0, zoom = 1.0;

// Variáveis de rotação das luas, de saturno e dos seus anéis
GLfloat moonRotation[10] = { 0.0 };
GLfloat ringRotation = 0.0;
GLfloat planetRotation = 0.0;

// Declaração das variáveis de órbita das luas
GLfloat moonOrbitInclination[10] = { 0.0, 30.0, -45.0, 60.0, -15.0, 90.0, -75.0, 45.0, -60.0, 30.0 };
GLfloat moonOrbitAngle[10] = { 0.0, 120.0, 240.0, 60.0, 180.0, 300.0, 45.0, 135.0, 225.0, 315.0 };
GLfloat moonOrbitRadius[10] = { 1.5, 1.7, 2.0, 2.2, 2.5, 2.8, 3.0, 3.3, 3.5, 3.8 };

// Função para desenhar uma esfera
void drawSphere(GLfloat radius, GLint stacks, GLint slices) {
    glutSolidSphere(radius, stacks, slices);
}

// Função para desenhar um anel
void drawRing(GLfloat innerRadius, GLfloat outerRadius, GLint slices) {
    glBegin(GL_QUAD_STRIP);
    for (int i = 0; i <= slices; i++) {
        GLfloat angle = 2.0 * 3.1415926 * i / slices;
        GLfloat x = cos(angle);
        GLfloat y = sin(angle);

        glVertex3f(innerRadius * x, innerRadius * y, 0.0);
        glVertex3f(outerRadius * x, outerRadius * y, 0.0);
    }
    glEnd();
}

// Função de desenho da cena
void drawScene() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // Translação, rotação e zoom
    glTranslatef(0.0, 0.0, -5.0);
    glTranslatef(translateX, translateY, 0.0);
    glRotatef(rotationX, 1.0, 0.0, 0.0);
    glRotatef(rotationY, 0.0, 1.0, 0.0);
    glScalef(zoom, zoom, zoom);
    
    // Desenhar Saturno
    glPushMatrix();
    glRotatef(planetRotation, 0.0, 1.0, 0.0);  // Rotação do planeta em torno do eixo Y
    glColor3f(0.85, 0.65, 0.12);
    drawSphere(1.0, 50, 50);
    glPopMatrix();

    // Desenhar os anéis de Saturno
    glPushMatrix();
    glRotatef(ringRotation, 0.0, 0.0, 1.0);  // Rotação dos anéis
    glColor3f(0.55, 0.47, 0.42);
    drawRing(1.3, 1.8, 50);
    glColor3f(0.5, 0.4, 0.35);
    drawRing(1.9, 2.4, 50);
    glColor3f(0.45, 0.37, 0.32);
    drawRing(2.5, 3.0, 50);
    glPopMatrix();

    // Desenhar as luas de Saturno
    for (int i = 0; i < 10; i++) {
        glPushMatrix();
        glRotatef(moonRotation[i], 0.0, 0.0, 1.0);  // Rotação das luas

        // Ajustar a órbita das luas
        glRotatef(moonOrbitInclination[i], 1.0, 0.0, 0.0); // Inclinação da órbita
        glRotatef(moonOrbitAngle[i], 0.0, 0.0, 1.0);  // Ângulo de órbita
        glTranslatef(moonOrbitRadius[i], 0.0, 0.0);  // Raio da órbita

        glColor3f(0.5, 0.5, 0.5);  // Definir a cor cinza para as luas
        drawSphere(0.1, 20, 20);
        glPopMatrix();
    }

    glutSwapBuffers();
}

// Função de inicialização
void initialize() {
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glEnable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, 1.0, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

// Função de redimensionamento da janela
void resize(int width, int height) {
    glViewport(0, 0, width, height);
}

// Função para lidar com eventos de teclado
void keyboard(unsigned char key, int x, int y) {
    if (key == 'q' || key == 'Q' || key == 27) {  // 'q', 'Q' ou ESC para sair
        exit(0);
    }
}

// Função para lidar com eventos do mouse (scroll)
void mouse(int button, int state, int x, int y) {
    if (button == 3) {  // Scroll up
        zoom += 0.1;
    } else if (button == 4) {  // Scroll down
        zoom -= 0.1;
        if (zoom < 0.1) {
            zoom = 0.1;
        }
    }
    glutPostRedisplay();
}

// Função para lidar com eventos do mouse (movimento com botão pressionado)
void motion(int x, int y) {
    rotationY = x;
    rotationX = y;
    glutPostRedisplay();
}

// Função para atualização das animações
void update(int value) {
    // Atualizar a rotação dos anéis
    ringRotation += 1;
    planetRotation += 0.5;

    // Atualizar a rotação das luas
    for (int i = 0; i < 10; i++) {
        moonRotation[i] += 1.0;
    }

    glutPostRedisplay();
    glutTimerFunc(30, update, 0);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Saturno");
    glutDisplayFunc(drawScene);
    glutReshapeFunc(resize);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    initialize();
    glutTimerFunc(0, update, 0);
    glutMainLoop();
    return 0;
}
