#include <Grapic.h>
#include <math.h>
#include <iostream>
using namespace grapic;
using namespace std;
const int DIMW = 700;
const int MAX_ASTEROIDE = 50;

struct Complex
{
    float x,y;
};

Complex make_complex(float r, float i)
{
    Complex c;
    c.x = r;
    c.y = i;
    return c;
}

Complex make_complex_expo(float r, float theta)
{
    Complex c;
    c.x = r*cos(theta);
    c.y = r*sin(theta);
    return c;
}

Complex operator+(Complex a, Complex b)
{
    Complex c = make_complex( a.x+b.x, a.y+b.y );
    return c;
}

Complex operator-(Complex a, Complex b)
{
    Complex c = make_complex( a.x-b.x, a.y-b.y );
    return c;
}

Complex translate(Complex p, float dx, float dy)
{
    return p + make_complex(dx,dy);
}

Complex operator*(float a, Complex b)
{
    Complex c = make_complex( a*b.x, a*b.y );
    return c;
}
Complex operator/(Complex b, float d)
{
    Complex c = make_complex( b.x/d, b.y/d );
    return c;
}

Complex scale(Complex p, float cx, float cy, float sc)
{
    Complex tr = make_complex( cx, cy);
    return (sc*(p-tr))+tr;
}

float to_degree(float rad)
{
    return 180.f * rad/M_PI;
}

float to_rad(float deg)
{
    return M_PI*deg/180.f;
}

Complex operator*(Complex a, Complex b)
{
    Complex c = make_complex( a.x*b.x - a.y*b.y, a.x*b.y + a.y*b.x );
    return c;
}

float norm(Complex c)
{
    return sqrt( c.x*c.x + c.y*c.y);
}

struct Vect2
{
    float x,y;
};

Vect2 make_Vect2(float x, float y)
{
    Vect2 v;
    v.x=x;
    v.y=y;
    return v;
}

Vect2 operator+(Vect2 a, Vect2 b)
{
    Vect2 c;
    c.x=a.x+b.x;
    c.y=a.y+b.y;
    return c;
}

Vect2 operator-(Vect2 a, Vect2 b)
{
    Vect2 c;
    c.x=a.x-b.x;
    c.y=a.y-b.y;
    return c;
}

Vect2 operator*(float lambda, Vect2 b){
    Vect2 c;
    c.x=lambda*b.x;
    c.y=lambda*b.y;
    return c;
}

Vect2 operator/(Vect2 a, float b)
{
    Vect2 c=make_Vect2(a.x/b,a.y/b);
    return c;
}

struct SolarSystem
{
    Vect2 TN,sun,mars,earth,moon;
    Vect2 TNf,sunf,marsf,earthf,moonf;
    Vect2 TNv,sunv,marsv,earthv,moonv;
    float TNm,sunm,marsm,earthm,moonm;
    Image sun1;
    Image mars1;
    Image earth1;
    Image moon1;
    Image TN1;
};

struct Asteroides
{
    float Am;
    Vect2 A,Af,Av;
    Image A1;
};

struct Vaisseau
{
    float Vm;
    Vect2 V,Vf,Vv;
    Image V1;
};

struct Tableau
{
    Image S1,E1,M1,L1,T1,tab1;
};

struct World
{
    int nbA;
    float dt;
    float g = 365;
    Asteroides tab[MAX_ASTEROIDE];
    Vect2 BG;
    Image BG1;
    int VAR;
    char VAR1[100];
    SolarSystem p;
    Vaisseau v;
    Tableau t;
};

void init(World& w)
{
    w.BG = make_Vect2(DIMW/2,DIMW/2); /// INIT DE L'ARRIERE PLAN
    w.BG1 = image("data/Projet/BG2.png");

    w.p.sun = make_Vect2(DIMW/2+50, DIMW/2);
    w.p.sun1 = image("data/Projet/soleil.png");
    w.p.mars = w.p.sun + make_Vect2(30, 0);
    w.p.mars1 = image("data/Projet/mars.png");
    w.p.earth = w.p.sun + make_Vect2(80, 0);
    w.p.earth1 = image("data/Projet/Terre.png");
    w.p.moon = w.p.earth + make_Vect2(15, 0);
    w.p.moon1 = image("data/Projet/Lune.png");
    w.p.TN = make_Vect2(DIMW/2, DIMW/2);
    w.p.TN1 = image("data/Projet/TN1.png");
    w.t.E1 = image ("data/Projet/Terre.png");
    w.t.L1 = image ("data/Projet/Lune.png");
    w.t.M1 = image ("data/Projet/mars.png");
    w.t.S1 = image ("data/Projet/soleil.png");
    w.t.T1 = image ("data/Projet/TN1.png");
    w.t.tab1 = image ("data/Projet/tab.png");


    w.p.sunm=(10);            ///1,98892*10e30    valeur réel a équilibrer a grapic
    w.p.earthm=(7);          ///5,972*10e24
    w.p.marsm=(6);             ///6,39*10e23
    w.p.moonm=(2);             ///7,36*10e22
    w.p.TNm=(20);              ///w.p.sunm*10

    w.p.sunv.x=1;
    w.p.sunv.y=1;

    w.p.earthv.x=1;
    w.p.earthv.y=1;

    w.p.marsv.x=1;
    w.p.marsv.y=1;

    w.p.moonv.x=1;
    w.p.moonv.y=1;

    w.p.TNv.x=1;
    w.p.TNv.y=1;

    w.p.sunf.x=0;
    w.p.sunf.y=0;

    w.p.earthf.x=0;
    w.p.earthf.y=0;

    w.p.marsf.x=0;
    w.p.marsf.y=0;

    w.p.moonf.x=0;
    w.p.moonf.y=0;

    w.p.TNf.x=0;
    w.p.TNf.y=0;
}

void draw(World w)
{
    int i;           /// AFFICHAGE DU BACKGROUND
    image_draw( w.BG1, 0, 0, DIMW, DIMW );

    image_draw( w.p.sun1, w.p.sun.x-30/2, w.p.sun.y-30/2, 30, 30 );

    image_draw( w.p.mars1, w.p.mars.x-8/2, w.p.mars.y-8/2, 8, 8 );

    image_draw( w.p.earth1, w.p.earth.x-10/2, w.p.earth.y-10/2, 10, 10 );

    image_draw( w.p.moon1, w.p.moon.x-4/2, w.p.moon.y-4/2, 4, 4 );

    image_draw( w.p.TN1, w.p.TN.x-40/2, w.p.TN.y-40/2, 60, 60 );

    if(w.nbA>0)
    {
        for(i=0;i<w.nbA;i++)
        {
            image_draw( w.tab[i].A1, w.tab[i].A.x-30/2, w.tab[i].A.y-30/2, 30, 30 );
        }
    }

    image_draw( w.v.V1, w.v.V.x-30/2, w.v.V.y-30/2, 30, 30 );

    image_draw( w.t.E1, 15 ,280, 10, 10 );
    image_draw( w.t.L1, 15 ,260, 4, 4 );
    image_draw( w.t.M1, 15 ,240, 8, 8 );
    image_draw( w.t.S1, 15 ,200, 30, 30 );
    image_draw( w.t.T1, 15 ,160, 40, 40 );
    image_draw( w.t.tab1, 0, 120, 100, 240);
}

void SSAddForce(Vect2 &f, Vect2 force)
{
    f=f+force;
}

float norm(Vect2 v)
{
	return sqrt(v.x*v.x + v.y*v.y);
}

float distance(Vect2 a, Vect2 b)
{
    return norm(b-a);
}

float normalize(Vect2& v)
{
    float n = norm(v);
	v.x /= n;
	v.y /= n;
	return n;
}

void update(World &w) /// FORCES ENTRE LES ASTRES (ET ASTEROIDES)
{
    float d,d1,d2,d3,d4,d5,d6,d7,d8,d9;
    Vect2 er,er1,er2,er3,er4,er5,er6,er7,er8,er9;
    int i;

    er=w.p.sun-w.p.earth; /// FORCE TERRE -> SOLEIL
    d=distance(w.p.sun,w.p.earth);
    w.p.sunf=-w.g*(w.p.sunm*w.p.earthm/(d*d))*er;
    Vect2 f1=make_Vect2(0, w.p.sunm*-9.81);
    SSAddForce(w.p.sunf,f1);

    er1=w.p.earth-w.p.sun; /// FORCE SOLEIL -> TERRE
    d1=distance(w.p.earth,w.p.sun);
    w.p.earthf=-w.g*(w.p.earthm*w.p.sunm/(d1*d1))*er1;
    Vect2 f2=make_Vect2(0, w.p.earthm*-9.81);
    SSAddForce(w.p.earthf,f2);

    er7=w.p.sun-w.p.mars; /// FORCE MARS -> SOLEIL
    d7=distance(w.p.mars,w.p.sun);
    w.p.marsf=-w.g*(w.p.marsm*w.p.sunm/(d7*d7))*er7;
    Vect2 f8=make_Vect2(0, w.p.marsm*-9.81);
    SSAddForce(w.p.marsf,f2);

    er2=w.p.mars-w.p.sun;  /// FORCE SOLEIL -> MARS
    d2=distance(w.p.mars,w.p.sun);
    w.p.marsf=-w.g*(w.p.marsm*w.p.sunm/(d2*d2))*er2;
    Vect2 f3=make_Vect2(0, w.p.marsm*-9.81);
    SSAddForce(w.p.marsf,f3);

    er3=w.p.moon-w.p.earth;  /// FORCE TERRE -> LUNE
    d3=distance(w.p.moon,w.p.earth);
    w.p.moonf=-w.g*(w.p.moonm*w.p.earthm/(d3*d3))*er3;
    Vect2 f4=make_Vect2(0, w.p.moonm*-9.81);
    SSAddForce(w.p.moonf,f3);

    er8=w.p.earth-w.p.moon;  /// FORCE LUNE -> TERRE
    d8=distance(w.p.moon,w.p.earth);
    w.p.earthf=-w.g*(w.p.earthm*w.p.moonm/(d8*d8))*er8;
    Vect2 f9=make_Vect2(0, w.p.earthm*-9.81);
    SSAddForce(w.p.earthf,f3);

    er4=w.p.sun-w.p.TN;  /// FORCE TROU NOIR -> SOLEIL
    d4=distance(w.p.sun,w.p.TN);
    w.p.sunf=-w.g*(w.p.sunm*w.p.TNm/(d4*d4))*er4;
    Vect2 f5=make_Vect2(0, w.p.sunm*-9.81);
    SSAddForce(w.p.sunf,f5);

    er5=w.p.TN-w.p.sun;  /// FORCE SOLEIL -> TROU NOIR
    d5=distance(w.p.TN,w.p.sun);
    w.p.TNf=-w.g*(w.p.TNm*w.p.sunm/(d5*d5))*er5;
    Vect2 f6=make_Vect2(0, w.p.TNm*-9.81);
    SSAddForce(w.p.TNf,f6);


    if(w.nbA>0)
    {
        for(i=0;i<w.nbA;i++)
        {
            er6=w.tab[i].A-w.p.TN;  /// FORCE TROU NOIR -> ASTEROIDES
            d6=distance(w.tab[i].A,w.p.TN);
            w.tab[i].Af=-w.g*(w.tab[i].Am*w.p.TNm/(d6*d6))*er;
            Vect2 f7=make_Vect2(0, w.tab[i].Am*-9.81);
            SSAddForce(w.tab[i].Af,f7);
        }
    }

    er9=w.v.V-w.p.TN;  /// FORCE TROU NOIR -> VAISSEAU
    d9=distance(w.v.V,w.p.TN);
    w.v.Vf=-w.g*(w.v.Vm*w.p.TNm/(d9*d9))*er9;
    Vect2 f10=make_Vect2(0, w.v.Vm*-9.81);
    SSAddForce(w.v.Vf,f10);

}

void updatePlanete(World& w)
{
    int i;

    w.p.sunv = w.p.sunv + (w.dt/w.p.sunm)*w.p.sunf;
    w.p.sun = w.p.sun + w.dt*w.p.sunv;

    w.p.earthv = w.p.earthv + (w.dt/w.p.earthm)*w.p.earthf;
    w.p.earth = w.p.earth + w.dt*w.p.earthv;

    w.p.marsv = w.p.marsv + (w.dt/w.p.marsm)*w.p.marsf;
    w.p.mars = w.p.mars + w.dt*w.p.marsv;

    w.p.moonv = w.p.moonv + (w.dt/w.p.moonm)*w.p.moonf;
    w.p.moon = w.p.moon + w.dt*w.p.moonv;
                                                          /// Mis en commentaire car tombe, pas intéressant du coup.
    //w.p.TNv = w.p.TNv + (w.dt/w.p.TNm)*w.p.TNf;
    //w.p.TN = w.p.TN + w.dt*w.p.TNv;

    if(w.nbA>0)
    {
        for(i=0;i<w.nbA;i++)
        {
            w.tab[i].Av = w.tab[i].Av + (w.dt/w.tab[i].Am)*w.tab[i].Af;
            w.tab[i].A = w.tab[i].A + w.dt*w.tab[i].Av;
        }
    }

    w.v.Vv = w.v.Vv + (w.dt/w.v.Vm)*w.v.Vf;
    w.v.V = w.v.V + w.dt*w.v.Vv;
}

void CHGT_CONSTANTE(World &w) /// PERMET A L'UTILISATEUR DE MODIFIER LA CONSTANTE
{
    float i;
    cout<<"Modification de la constante universelle de gravitation : ";
    cin>>i;
    w.g=i;
}

void ASTEROIDE(World &w,int x,int y,int i) /// INIT D'ASTEROIDES, AFFICHE UN ASTEROIDE PARMIS LES 3 IMAGES DISPONIBLE (ALEATOIRE).
{
    w.tab[w.nbA].A = make_Vect2(x, y);

    if(i==1)
    {
        w.tab[w.nbA].A1 = image("data/Projet/asteroide.png");
        w.tab[w.nbA].Am=(0.2);
    }
    if(i==2)
    {
        w.tab[w.nbA].A1 = image("data/Projet/asteroide2.png");
        w.tab[w.nbA].Am=(0.8);
    }
    if(i==3)
    {
        w.tab[w.nbA].A1 = image("data/Projet/asteroide3.png");
        w.tab[w.nbA].Am=(0.5);
    }

    w.tab[w.nbA].Av.x=1;
    w.tab[w.nbA].Av.y=1;

    w.tab[w.nbA].Af.x=0;
    w.tab[w.nbA].Af.y=0;
}

void VAISSEAU(World &w)
{
    w.v.V=make_Vect2(DIMW/3,DIMW/2);
    w.v.V1=image("data/Projet/V3.png");

    w.v.Vm=8;

    w.v.Vv.x=1;
    w.v.Vv.y=1;

    w.v.Vf.x=0;
    w.v.Vf.y=0;
}

void ASTRE (World &w)
{
    cout<<"Qu'elle astre voulez vous modifier la masses ?(Soleil,Terre,Mars,Lune,Trou noir) : ";
    cin>>w.VAR1;

    if(strcmp(w.VAR1,"Soleil")==0)
    {
        cout<<"Entrer la nouvelle valeur de la masse du Soleil : ";
        cin>>w.p.sunm;
    }

    if(strcmp(w.VAR1,"Terre")==0)
    {
        cout<<"Entrer la nouvelle valeur de la masse de la Terre : ";
        cin>>w.p.earthm;
    }

    if(strcmp(w.VAR1,"Mars")==0)
    {
        cout<<"Entrer la nouvelle valeur de la masse de Mars : ";
        cin>>w.p.marsm;
    }

    if(strcmp(w.VAR1,"Lune")==0)
    {
        cout<<"Entrer la nouvelle valeur de la masse de la Lune : ";
        cin>>w.p.moonm;
    }

    if(strcmp(w.VAR1,"Trou noir")==0)
    {
        cout<<"Entrer la nouvelle valeur de la masse du Trou noir : ";
        cin>>w.p.TNm;
    }
}

void InitAll(World &w)
{
    init(w);
    VAISSEAU(w);
}

void Tableau(World &w)
{
    color(255,255,0);
    print( 20, 315, "g = ");
    print( 40, 315, w.g);
    print( 10, 295, "Masse : ");
    print( 35, 276, w.p.earthm);
    print( 30, 253, w.p.moonm);
    print( 35, 235, w.p.marsm);
    print( 50, 208, w.p.sunm);
    print( 60, 170, w.p.TNm);
}

void Deplacement(World &w)
{
    if(isKeyPressed(SDLK_UP))
    {
        w.v.Vv.y+=w.VAR;
    }
    if(isKeyPressed(SDLK_LEFT))
    {
        w.v.Vv.x-=w.VAR;
    }
    if(isKeyPressed(SDLK_RIGHT))
    {
        w.v.Vv.x+=w.VAR;
    }
    if(isKeyPressed(SDLK_DOWN))
    {
        w.v.Vv.y-=w.VAR;
    }
}

void Vit(World &w)
{
    cout<<"Vitesse augmenter !";
    w.VAR+=10;
}

void Vit1(World &w)
{
    cout<<"Vitesse diminuer !";
    w.VAR-=10;
}

void update1(World& w) /// UPDATE DES FORCES ET APPARITION D'ASTEROIDES
{
    w.dt=0.01;
    update(w);
    updatePlanete(w);
    int x,y;
    mousePos(x,y);
    if (isMousePressed(SDL_BUTTON_LEFT))
    {
        int i=irand(1,3);
        ASTEROIDE(w,x,y,i);
        w.nbA++;
    }

    Tableau(w);

    Deplacement(w);
}

int main(int , char** )
{
    bool stop=false;
    World w;
    w.nbA=0;
    w.VAR=10;
	winInit("Mini-Projet", DIMW, DIMW);
	Menu menu;
	menu_add(menu,"Changer la constante");
	menu_add(menu,"Changer la masses des astres");
	menu_add(menu,"Réinitialiser les positions");
	menu_add(menu,"Augmenter la vitesse du vaisseau");
	menu_add(menu,"Diminuer la vitesse du vaisseau");
	menu_add(menu,"Run");
    const int run = 5;
    backgroundColor( 0, 0, 0 );
    menu_setSelect(menu,0);
    while ( !stop)
    {
        winClear();
        init(w);
        VAISSEAU(w);
        while ( !stop)
                {
                switch(menu_select(menu))
                {
                    case 0: CHGT_CONSTANTE(w);
                    menu_setSelect(menu,run);
                    break;

                    case 1: ASTRE(w);
                    menu_setSelect(menu,run);
                    break;

                    case 2: InitAll(w);
                    menu_setSelect(menu,run);
                    break;

                    case 3: Vit(w);
                    menu_setSelect(menu,run);
                    break;

                    case 4: Vit1(w);
                    menu_setSelect(menu,run);
                    break;
                }
                    winClear();
                    draw(w);
                    update1(w);
                    menu_draw(menu);
                    stop = winDisplay();
                }
    }
    winQuit();
	return 0;
}
