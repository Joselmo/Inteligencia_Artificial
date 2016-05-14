
#ifndef AGENT__
#define AGENT__

#include <string>
#include <iostream>
#include <cstdlib>
#include "conexion.h"
using namespace std;

// -----------------------------------------------------------
//				class Agent
// -----------------------------------------------------------
class Environment;
class Agent
{
public:
	Agent(){
		x_= 99;
		y_= 99;
		orientacion_=3;
		role_="PlYR";
		last_accion_=3;
		REINICIADO_=false;
		size_=200;
		for (int i=0;i<200;i++)
		    for(int j=0;j<200;j++){
		      mapa_entorno_[i][j]='?';
		      mapa_objetos_[i][j]='?';
		    }

		for (int i=0;i<100;i++)
		    for(int j=0;j<100;j++)
		      mapa_solucion_[i][j]='?';
		
		for (int i=0;i<200;i++)
		    for(int j=0;j<200;j++){
		    	mapa_pulgarcito_[i][j]=0;
		    	mapa_pulgarcito_temp[i][j]=0;
		    }
		avance = 0;
		fui_reiniciado = false;
		tenia_solucion = false;
		prohibido_ = 50000;
		ciclo = 0;
		zapatillas_= false;
		banador_=false;
		for(int i=0; i<10;i++){
			items[i]=false;
		}
		rotacion_correcta = false;
		en_solucion = false;
		n_pk_visto = 0;
	}

	enum ActionType
	{
	    actFORWARD, // avanzar
	    actTURN_L,  // Girar Izquierda
	    actTURN_R,  // Girar Derecha
	    actIDLE,    // No hacer nada
	    actPICKUP,  // Recoger un objeto
	    actPUTDOWN, // Soltar un objeto
	    actPUSH,    // Meter en la mochila
	    actPOP,     // Sacar de la mochila
	    actGIVE,	// Dar un objeto a un personaje
	    actTHROW	// Lanzar un objeto

	};

	void Perceive(Environment &env);
	bool Perceive_Remote(conexion_client &Cliente, Environment &env);
	void ActualizarInformacion(Environment *env);
	ActionType Think();
	void FixLastAction(Agent::ActionType x){last_accion_=x;};

	char mapa_entorno_[200][200]; // mapa que muestra el tipo de terreno
	char mapa_objetos_[200][200]; // mapa que muestra los objetos que estan encima del terreno
	char mapa_solucion_[100][100]; // Mapa que almacena la solucion que el alumno propone
	// Funciones de acceso a los datos
	void GetCoord(int &fila, int &columna, int &brujula){fila=y_;columna=x_;brujula=orientacion_;};

	bool hayObstaculos(int y,int x);
	int ValorPulgarcito(int y,int x);
	void rotar_mapa(int grados);
	void ajustar_cero(char m[200][200],char m2[200][200]);
	void bolaMagica();

private:
	//Variables de interaccion con el entorno grafico
	int size_;	

	//SENSORES
	char VISTA_[10];	//Sensor de terreno
	char SURFACE_[10];	// Objetos sobre el terreno (personajes, obj, etc)
	bool REINICIADO_;	// Si hemos sido reiniciados
	string MENSAJE_;
	char EN_USO_;		// Objeto en uso
	char MOCHILLA_[5];	// 
	char PUNTUACION_[9];
	bool FIN_JUEGO_;
	char SALUD_;

	//Variables de estado
	int x_,y_, orientacion_;
	int last_accion_;
	string role_;
	int avance;
	int prohibido_;
	int mapa_pulgarcito_[200][200];
	int mapa_pulgarcito_temp[200][200];
	int ciclo;
	bool zapatillas_,banador_;
	bool items[10];
	int n_pk_visto;
	bool rotacion_correcta,en_solucion,fui_reiniciado,tenia_solucion;
	int f1,c1,c2,f2, pk1f,pk2f,myPk1f,myPk2f,pk1c,pk2c,myPk1c,myPk2c;

};

string ActionStr(Agent::ActionType);

#endif
