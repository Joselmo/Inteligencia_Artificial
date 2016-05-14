#include "belkan.h"
#include "conexion.h"
#include "environment.h"
#include <iostream>
#include <cstdlib>
#include <vector>
#include <utility>
#include <cstring>

using namespace std;
//-----------------------------------------------------------
bool Agent::hayObstaculos(int y,int x){
	if( (mapa_entorno_[y][x]=='B' && EN_USO_ != '6') || mapa_entorno_[y][x] == 'D' ||
	 mapa_entorno_[y][x] == 'M' || mapa_entorno_[y][x] == 'P' || (mapa_entorno_[y][x] == 'A' && EN_USO_ != '0' )
	 || (mapa_objetos_[y][x] >= 'a' && mapa_objetos_[y][x] <= 'z')){

		return true;
	}else
		return false;

}
// ---------------------------------------------------------
void Agent::rotar_mapa(int grados){
	int giros = grados/90;

	char matriz_temp1[200][200];
	char matriz_temp2[200][200];
	int matriz_temp3[200][200];
	cout << endl<<"giros=" << giros << endl;

	if (giros==2){
		for(int i=0;i<200;i++)
			for(int j=0;j<200;j++){
				matriz_temp1[i][j] = mapa_entorno_[199-i][199-j];
				matriz_temp2[i][j] = mapa_objetos_[199-i][199-j];
				matriz_temp3[i][j] = mapa_pulgarcito_[199-i][199-j];
			}
		
	}
	else if (giros==3){
		for(int i=0;i<200;i++)
			for(int j=0;j<200;j++){
				matriz_temp1[i][j] = mapa_entorno_[j][199-i];
				matriz_temp2[i][j] = mapa_objetos_[j][199-i];
				matriz_temp3[i][j] = mapa_pulgarcito_[j][199-i];
			}		
	}
	else if (giros==1){
		for(int i=0;i<200;i++)
			for(int j=0;j<200;j++){
				matriz_temp1[i][j] = mapa_entorno_[199-j][i];
				matriz_temp2[i][j] = mapa_objetos_[199-j][i];
				matriz_temp3[i][j] = mapa_pulgarcito_[199-j][i];
			}		
	}


	for (int i=0; i<200; i++){
  		for (int j=0; j<200; j++){
    	    mapa_entorno_[i][j]='?';
       		mapa_objetos_[i][j]='?';
		}
	}	

	for(int i=0;i<200;i++)
		for(int j=0;j<200;j++){ 
			mapa_entorno_[i][j] = matriz_temp1[i][j];
			mapa_objetos_[i][j] = matriz_temp2[i][j];
			mapa_pulgarcito_[i][j] = matriz_temp3[i][j];
		}




}
// ---------------------------------------------------------
void Agent::ajustar_cero(char m[200][200],char m2[200][200]){
	char matrix[100][100];

	for(int i=0;i<100;i++)
		for(int j=0;j<100;j++){
			matrix[i][j] = m[i+myPk2f-pk2f][j+myPk2c-pk2c];
			
		}
	for(int i=0;i<200;i++)
		for(int j=0;j<200;j++){
			m[i][j]='?';
		}
	for(int i=0;i<100;i++)
		for(int j=0;j<100;j++){
			m[i][j] = matrix[i][j];
			
		}
	
}
//-----------------------------------------------------------
int Agent::ValorPulgarcito(int y,int x){
	if(hayObstaculos(y,x)){
		return prohibido_;
	}else{
		if(mapa_pulgarcito_[y][x] == prohibido_){
			mapa_pulgarcito_[y][x] = 0;
		}
		return mapa_pulgarcito_[y][x];
	}
}
//---------------------------------------------------------
void Agent::bolaMagica(){
		int terreno[5];	//Nº de casillas adyacentes de A B M S T
		
		for(int i=3;i<97;i++){
			for(int j=3;j<97;j++){
				if(mapa_entorno_[i][j] == '?'){
					for(int i=0;i<5;i++) 
						terreno[i] = 0;

					for(int k=i-1;k<=i+1;k++) // Compruebo que tengo alrededor
						for(int l=j-1;l<=j+1;l++){
							if(mapa_entorno_[k][l]!='?'){
								switch(mapa_entorno_[k][l]){
									case 'A': terreno[0]++;
										break; 
									case 'B': terreno[1]++;
										break;
									case 'M':
										if(k == i || l == j)
									 		terreno[2]=10;
									 	else
									 		terreno[3]=10;
										break;
									case 'S': terreno[3]++;
										break;
									case 'T': terreno[4]++;
										break;	
								} // fin switch
							} // fin if
						} // fin for l
					
					//Elijo la mejor opcion
					int mejor_opcion = 0;
					char ganador = '?';
					for(int m=0;m<5;m++){
						if(mejor_opcion < terreno[m]){
							mejor_opcion = terreno[m];
							switch(m){
								case 0: ganador = 'A';
									break;
								case 1: ganador = 'B';
									break;
								case 2: ganador = 'M';
									break;
								case 3: ganador = 'S';
									break;
								case 4: ganador = 'T';
									break;	
							} // fin switch
						}
					}
					mapa_entorno_[i][j]=ganador;
					mapa_solucion_[i][j]=ganador;
				} // fin if


			}// fin for j
		}

		//Ajustes del mapa solución
		for(int i=0;i<100;i++){ // norte
			mapa_solucion_[0][i]='P';
			mapa_solucion_[1][i]='P';
			mapa_solucion_[2][i]='P';
		}
		for(int i=0;i<100;i++){ // oeste
			mapa_solucion_[i][0]='P';
			mapa_solucion_[i][1]='P';
			mapa_solucion_[i][2]='P';
		}
		for(int i=0;i<100;i++){ // Este
			mapa_solucion_[i][97]='P';
			mapa_solucion_[i][98]='P';
			mapa_solucion_[i][99]='P';
		}
		for(int i=0;i<100;i++){ // Sur
			mapa_solucion_[97][i]='P';
			mapa_solucion_[98][i]='P';
			mapa_solucion_[99][i]='P';
		}

}
//----------------------------------------------------------
// ------- Devuleve en fila y en columna, la fila y la columna que hay en el mensaje que devuelve el entorno.
void CapturaFilaColumnaPK (string mensaje, int &fila, int &columna){
	if (mensaje.substr(0,8)=="PK fila:"){
		int pos = mensaje.find('c');
		string valor = mensaje.substr(9,pos-8);
		fila = atoi(valor.c_str());
		
		int pos2 = mensaje.find('.');
		pos = pos+8;
		valor = mensaje.substr(pos,pos2-1);
		columna = atoi(valor.c_str());
	}
}

//-----------------------------------------------------------
void PasarVectoraMapaPulgarcito(int fila, int columna, int m[200][200], int v, int brujula){
  m[fila][columna]=v;

    switch(brujula){
        case 0: // Orientacion Norte

		//m[fila-1][columna]=v;
	/*	for (int i=0; i<3; i++){
			if(i!=columna)
		  		m[fila-2][columna+i-1]+=1;
		}*/
		for (int j=0; j<5; j++){
			if(j != columna)
		  		m[fila-3][columna+j-2]+=1;
		}
	        break;
	case 1: // Orientacion Este
		//m[fila][columna+1]=v;
		/*for (int i=0; i<3; i++){
			if(i != fila)
		  		m[fila+i-1][columna+2]+=1;
		}*/
		for (int j=0; j<5; j++){
		  if( j != fila)
		  	m[fila+j-2][columna+3]+=1;
		}
	        break;
        case 2: // Orientacion Sur
		//m[fila+1][columna]=v;
		/*for (int i=0; i<3; i++){
			if( i != columna)
		  		m[fila+2][columna+1-i]+=1;
		}*/
		for (int j=0; j<5; j++){
			if(j!=columna)
		  		m[fila+3][columna+2-j]+=1;
		}
		                break;
        case 3: // Orientacion Oeste
		//m[fila][columna-1]=v;
	/*	for (int i=0; i<3; i++){
			if(i!=columna)
		  		m[fila+1-i][columna-2]+=1;
		}*/
		for (int j=0; j<5; j++){
			if(j != columna)
		  		m[fila+2-j][columna-3]+=1;
		}

                break;
    }

}
// ----------------------------------------------------------
void PasarVectoraMapaSolucion(int fila, int columna, char m[100][100], char *v, int brujula){
  m[fila][columna]=v[0];

    switch(brujula){
        case 0: // Orientacion Norte

		m[fila-1][columna]=v[1];
		for (int i=0; i<3; i++){
		  m[fila-2][columna+i-1]=v[2+i];
		}
		for (int j=0; j<5; j++){
		  m[fila-3][columna+j-2]=v[5+j];
		}
	        break;
	case 1: // Orientacion Este
		m[fila][columna+1]=v[1];
		for (int i=0; i<3; i++){
		  m[fila+i-1][columna+2]=v[2+i];
		}
		for (int j=0; j<5; j++){
		  m[fila+j-2][columna+3]=v[5+j];
		}
	        break;
        case 2: // Orientacion Sur
		m[fila+1][columna]=v[1];
		for (int i=0; i<3; i++){
		  m[fila+2][columna+1-i]=v[2+i];
		}
		for (int j=0; j<5; j++){
		  m[fila+3][columna+2-j]=v[5+j];
		}
		                break;
        case 3: // Orientacion Oeste
		m[fila][columna-1]=v[1];
		for (int i=0; i<3; i++){
		  m[fila+1-i][columna-2]=v[2+i];
		}
		for (int j=0; j<5; j++){
		  m[fila+2-j][columna-3]=v[5+j];
		}

                break;
    }

}

// -----------------------------------------------------------
void PasarVectoraMapaCaracteres(int fila, int columna, char m[200][200], char *v, int brujula){
  m[fila][columna]=v[0];

    switch(brujula){
        case 0: // Orientacion Norte

		m[fila-1][columna]=v[1];
		for (int i=0; i<3; i++){
		  m[fila-2][columna+i-1]=v[2+i];
		}
		for (int j=0; j<5; j++){
		  m[fila-3][columna+j-2]=v[5+j];
		}
	        break;
	case 1: // Orientacion Este
		m[fila][columna+1]=v[1];
		for (int i=0; i<3; i++){
		  m[fila+i-1][columna+2]=v[2+i];
		}
		for (int j=0; j<5; j++){
		  m[fila+j-2][columna+3]=v[5+j];
		}
	        break;
        case 2: // Orientacion Sur
		m[fila+1][columna]=v[1];
		for (int i=0; i<3; i++){
		  m[fila+2][columna+1-i]=v[2+i];
		}
		for (int j=0; j<5; j++){
		  m[fila+3][columna+2-j]=v[5+j];
		}
		                break;
        case 3: // Orientacion Oeste
		m[fila][columna-1]=v[1];
		for (int i=0; i<3; i++){
		  m[fila+1-i][columna-2]=v[2+i];
		}
		for (int j=0; j<5; j++){
		  m[fila+2-j][columna-3]=v[5+j];
		}

                break;
    }

}


// -----------------------------------------------------------
void Agent::Perceive(Environment &env)
{
	env.SenSorStatus(VISTA_, SURFACE_, MENSAJE_, REINICIADO_, EN_USO_, MOCHILLA_, PUNTUACION_, FIN_JUEGO_, SALUD_, false);

}


bool Agent::Perceive_Remote(conexion_client &Cliente, Environment &env)
{
	bool actualizado=false;


	actualizado = env.Perceive_Remote(Cliente);
	if (actualizado)
		env.SenSorStatus(VISTA_, SURFACE_, MENSAJE_, REINICIADO_, EN_USO_, MOCHILLA_, PUNTUACION_, FIN_JUEGO_, SALUD_, true);

    return actualizado;
}


// -----------------------------------------------------------
string ActionStr(Agent::ActionType accion)
{
	switch (accion)
	{
	case Agent::actFORWARD: return "FORWARD";
	case Agent::actTURN_L: return "TURN LEFT";
	case Agent::actTURN_R: return "TURN RIGHT";
	case Agent::actIDLE: return "IDLE";
	case Agent::actPICKUP: return "PICK UP";
	case Agent::actPUTDOWN: return "PUT DOWN";
	case Agent::actPUSH: return "PUSH";
	case Agent::actPOP: return "POP";
	case Agent::actGIVE: return "GIVE";
	case Agent::actTHROW: return "THROW";
	default: return "????";
	}
}

// -----------------------------------------------------------
void Agent::ActualizarInformacion(Environment *env){

	// Actualizar mi informacion interna
	if (REINICIADO_){ 
		// Lo que tengas que hacer si eres reposicionado en el juego
		for (int i=0; i<200; i++){
      		for (int j=0; j<200; j++){
	        mapa_entorno_[i][j]='?';
	        mapa_objetos_[i][j]='?';
      		}	
    	}

    	// Si he sido reiniciado guardo mapa de pulgarcito
    	// Y limpio el pulgarcito para que empiece de 0.
    	for (int i=0;i<100;i++)
		    for(int j=0;j<100;j++){
		    	mapa_pulgarcito_temp[i][j] = mapa_pulgarcito_[i][j];
		      	mapa_pulgarcito_[i][j]=0;
		    }

		for(int i=0; i<10;i++){
			items[i]=false;
		}
		zapatillas_ = false;
		banador_ = false;
    	x_=99;
    	y_=99;
    	n_pk_visto=0;
    	rotacion_correcta = false;
    	orientacion_=3;
    	tenia_solucion = en_solucion;
    	en_solucion = false;
    	fui_reiniciado = true;
    	cout << "------ REINICIADO -------"<< endl;
	}

	

	switch(last_accion_){
	  case 0: //avanzar
	  	if(MENSAJE_ != "Has chocado con un obstaculo movil. "){
	  	  switch(orientacion_){
	  	    case 0: // norte
	  		    y_--;
	  		    break;
	  	    case 1: // este
	  		    x_++;
	  		    break;
	  	    case 2: // sur
	  		    y_++;
	  		    break;
	  	    case 3: // este
	  		    x_--;
	  		    break;
	  	  }
	 	}
	  break;
	  case 1: // girar izq
		  orientacion_=(orientacion_+3)%4;
		  break;
	  case 2: // girar dch
		  orientacion_=(orientacion_+1)%4;
		  break;
	}

	ciclo++;
	PasarVectoraMapaPulgarcito(y_,x_,mapa_pulgarcito_,ciclo,orientacion_);
	//mapa_pulgarcito_[y_][x_]=ciclo;


	// Si estoy en un PK
	if(VISTA_[0] == 'K' && !rotacion_correcta && n_pk_visto<3){
		n_pk_visto++;
		cout << "Estoy en pk="<<n_pk_visto<< endl;
		switch(n_pk_visto){
			case 1: CapturaFilaColumnaPK (MENSAJE_, pk1f, pk1c);
					myPk1f = y_;
					myPk1c = x_;
					cout << "PK: pk1f ="<< pk1f << " pk1c=" << pk1c;
					cout << " myPk1f="<< myPk1f<<" myPk1c="<< myPk1c << endl;
			break;
			case 2:CapturaFilaColumnaPK (MENSAJE_, pk2f, pk2c);
					myPk2f = y_;
					myPk2c = x_;
					cout << "PK: pk2f ="<< pk2f << " pk2c=" << pk2c;
					cout << " myPk2f="<< myPk2f<<" myPk2c="<< myPk2c << endl;
			break;
		}
		if(myPk1c == myPk2c || myPk1f == myPk2f)
			n_pk_visto--;
	}


	//Si encuentro 2 PK obtengo la posición del mapa real.
	if(n_pk_visto == 2 && !rotacion_correcta){
		f1 = pk1f - pk2f;
		f2 = myPk1f - myPk2f;
		c1 = pk1c - pk2c;
		c2 = myPk1c - myPk2c;
		cout <<"Roto el mapa" << endl;
		if(f1 == -f2){ // rotar 180
			rotar_mapa(180);
			cout << "Mapa rotado 180º"<< endl;
			orientacion_ = (orientacion_+2)%4;
			myPk2c = 200-1 - myPk2c;
			myPk2f = 200-1 - myPk2f;

		}else if(f1 == c2){ // rotar 90
			rotar_mapa(90);
			cout << "Mapa rotado 90º "<< endl;
			orientacion_ = (orientacion_+1)%4;
			swap(myPk2f,myPk2c);
			myPk2c=199 -myPk2c;
		}else if(f1 == -c2){ // rotar -90
			rotar_mapa(270);
			cout << "Mapa rotado 270 º"<< endl;
			orientacion_ = (orientacion_+3)%4;
			swap(myPk2c,myPk2f);
			myPk2f=199- myPk2f;
		}

		ajustar_cero(mapa_entorno_,mapa_entorno_);
		ajustar_cero(mapa_objetos_,mapa_objetos_);

		
		y_ = pk2f;
		x_ = pk2c;
		rotacion_correcta = true;

		
		//inicializar el mapa solución
		
		for(int i=3;i<97;i++){
			for(int j=3;j<97;j++){
				if(mapa_solucion_[i][j] == '?' && mapa_entorno_[i][j] !='?')
					mapa_solucion_[i][j]= mapa_entorno_[i][j];
			}
		}
		//empezar con el mapa solución
		en_solucion = true;

		// Si he sido reiniciado y ya estoy orientado de nuevo
		if(fui_reiniciado){
			cout << "Recupero los datos" << endl;
			// Vuelvo el mapa de pulgarcito antigo con el nuevo
			for(int i=0;i<100;i++){
				for(int j=0;j<100;j++){
					if(mapa_pulgarcito_[i][j] < mapa_pulgarcito_temp[i][j])
						mapa_pulgarcito_[i][j] = mapa_pulgarcito_temp[i][j];
				}
			}
		
			// Si estaba rellenando la solución, es decir, si estaba bien 
			// orientado copio mi solucion en entorno
			if(tenia_solucion){
				cout << "vuelco solución en entorno actual"<< endl;
				for (int i=3;i<97;i++){
				    for(int j=3;j<97;j++){
				    	if(mapa_solucion_[i][j] != '?' && mapa_entorno_[i][j]=='?'){
				    		mapa_entorno_[i][j] = mapa_solucion_[i][j];
				    	}
				   		
				    }
				}
				
				tenia_solucion = false;
			}
			
			fui_reiniciado = false;
		}

	}
	// Si estaba rellenando la solución, es decir, si estaba bien 
	// orientado copio mi solucion en entorno
	if(en_solucion){
		PasarVectoraMapaSolucion(y_,x_, mapa_solucion_, VISTA_, orientacion_);
	}

	

	// Relleno las posiciones sin descubrir, segun me dice la bola magica.
	if(ciclo == 19999){
		bolaMagica();
	}

	// Comprobacion para no salirme del rango del mapa
	bool algo_va_mal=false;
	if (y_<0){
		y_=0;
		algo_va_mal=true;
	} 
	else if (y_>199){
		y_=199;
		algo_va_mal=true;
	}
	if (x_<0){
		x_=0;
		algo_va_mal=true;
	} 
	else if (x_>199){
		x_=199;
		algo_va_mal=true;
	}

	if (algo_va_mal){
		cout << "CUIDADO: NO ESTAS CONTROLANDO BIEN LA UBICACION DE TU AGENTE\n";
	}


	PasarVectoraMapaCaracteres(y_,x_,mapa_entorno_,VISTA_,orientacion_);
	PasarVectoraMapaCaracteres(y_,x_,mapa_objetos_,SURFACE_,orientacion_);

	env->ActualizarMatrizUsuario(mapa_entorno_);

}


// -----------------------------------------------------------
Agent::ActionType Agent::Think()
{
	Agent::ActionType accion = actFORWARD; // Por defecto avanza
	
	// Variables temporales
	int enx=x_;
	int eny=y_;
	int izy=y_;
	int izx=x_;
	int dey=y_;
	int dex=x_;

	switch(orientacion_){
	case 0: // norte
	    eny--;
	    izx--;
	    dex++;
		break;
	case 1: // este
	    enx++;
	    izy--;
	    dey++;
		break;
	case 2: // sur
	    eny++;
		izx++;
		dex--;
		break;
	case 3: // oeste
	    enx--;
	    izy++;
	    dey--;
	    break;
	}

	char objetos[5];
	strcpy(objetos,MOCHILLA_);
	//objetos[5] = (EN_USO_ == '' ?'-':EN_USO_);
	objetos[4] = EN_USO_;
	
	//Equipamiento, me quedo con el ultimo objeto que encuentro
	if(SURFACE_[0] == '6'){ // Si me encuentro las zapatillas
		if(EN_USO_ != '-')
			accion = actPUSH;
		else{
			zapatillas_ = true;
			accion = actPICKUP;
			cout << "Recojo zapatillas"<< endl;
		}
		cout << "Encuentra zapatillas ñike corté" <<endl;
	}else if(SURFACE_[0] == '0'){  // Si me encuentro el bañador
		if(EN_USO_ != '-')
			accion = actPUSH;
		else{
			accion = actPICKUP;
			banador_= true;
			cout << "Cojo bañador.\n";
		}
		cout << "Encuentra bañador de flores" << endl;
	}else if( (SURFACE_[0] == '4' || SURFACE_[0] == '5' || SURFACE_[0]=='8' || SURFACE_[0]=='9') 
		&& ( ( strstr(objetos,"--")   != NULL && (zapatillas_ + banador_) == 2 ) || 
			 ( strstr(objetos,"---")  != NULL && (zapatillas_ + banador_) == 1 ) ||
			 ( strstr(objetos,"----") != NULL && (zapatillas_ + banador_) == 0 ) ) ){
		if(EN_USO_ != '-'){
			accion = actPUSH;
		}else{
			accion = actPICKUP;
			items[atoi(&SURFACE_[0])] = true;
			cout << "Recojo "<<SURFACE_[0] << " y lo activo a "<<SURFACE_[0]-'0'<< endl;
		}
	}else if(mapa_entorno_[eny][enx] == 'A' && banador_ && EN_USO_ != '0'){
		// CAMBIO DE EQUIPO si tengo los 2 objetos
		// Si tengo Agua, voy guardando objetos y cogiendo hasta tener el bañador
		if(EN_USO_ != '-'){
			accion = actPUSH;
		}else{
			accion = actPOP;
		}
		cout << "Se equipa Bañador" << endl;
	}else if(mapa_entorno_[eny][enx] == 'B' && zapatillas_ && EN_USO_ != '6'){
		if(EN_USO_ != '-'){
			accion = actPUSH;
		}else{
			accion = actPOP;
		}
		cout << "Se equipa zapatillas"<< endl;
	}else if(((SURFACE_[1] >='g' && SURFACE_[1] <='l') || SURFACE_[1] == 'o' 
			|| SURFACE_[1] == 'p') && items[4]){ //Tengo oro y la doy
		if(EN_USO_ == '-'){
			accion = actPOP;
		}else if(EN_USO_ != '4'){
			accion = actPUSH;
		}else{
			accion = actGIVE;
			items[4] = false;
			cout << "Entrego el lingote de oro."<< endl;
		}
	}else if(((SURFACE_[1] >='g' && SURFACE_[1] <='j') || SURFACE_[1] == 'm' 
			|| SURFACE_[1] == 'n') && items[5]){ //Tengo el oscar y la doy
		if(EN_USO_ == '-'){
			accion = actPOP;
		}else if(EN_USO_ != '5'){
			accion = actPUSH;
		}else{
			accion = actGIVE;
			items[5] = false;
			cout << "Entrego el Oscar."<< endl;
		}
	}else if((SURFACE_[1] >='i' && SURFACE_[1] <='p')&& items[8]){ //Tengo la manzana y la doy
		if(EN_USO_ == '-'){
			accion = actPOP;
		}else if(EN_USO_ != '8'){
			accion = actPUSH;
		}else{
			accion = actGIVE;
			items[8] = false;
			cout << "Entrego el algoritmo."<< endl;
		}
	}else if((SURFACE_[1] >='k' && SURFACE_[1] <='p')&& items[9]){ //Tengo el alg y la doy
		if(EN_USO_ == '-'){
			accion = actPOP;
		}else if(EN_USO_ != '9'){
			accion = actPUSH;
		}else{
			accion = actGIVE;
			items[9] = false;
			cout << "Entrego el algoritmo."<< endl;
		}
	}else{
		
		int vfrente,vizquierda,vder;
		vfrente = ValorPulgarcito(eny,enx);
		vizquierda = ValorPulgarcito(izy,izx);
		vder = ValorPulgarcito(dey,dex);


		// Movimiento Por defecto
		if(vfrente <= vizquierda && vfrente <= vder){
			// enfrente mejor
			if(vfrente == prohibido_){
				accion = actTURN_R;
				cout << "Tengo un obstaculo enfrente"<<endl;
			}else{
				accion = actFORWARD;
				//cout << "Avanzo!."<<endl;
			}
		}else if(vizquierda <= vder){
			accion = actTURN_L;
			//cout<< " Giro izquierda" << endl;
		}else{
			accion = actTURN_R;
			//cout << "Giro derecha "<<endl;
		}
	}//Else de objetos



	// recuerdo la ultima accion realizada
	last_accion_ = accion;

	return accion;

}
