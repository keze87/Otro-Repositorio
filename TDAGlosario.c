#include "TDAGlosario.h"

int CrearGlosario(TDAGlosario* g, char* documento, char* arch_config)
{

	int error = 1; /* OK */

	TDAParser ListaParser;

	error = PA_Crear(documento,arch_config,&ListaParser);
	if (error != 1)
	{
		PA_Destruir(&ListaParser);
		return error;
	}

	AB_Crear(&g->arbol,sizeof(TPalabra));

	lista_a_arbol(g, ListaParser);

	/* Magia */

	error = PA_Destruir(&ListaParser);

	return error;

}

int lista_a_arbol(TDAGlosario* g, TDAParser ListaParser){
    int mov=OK;
    TPalabra elem;
    TElemParser aux_parser;
    TPosicion posicion;

    if (L_Vacia(ListaParser.parser))
        return 0; /*si esta vacia, deja vacio el glosario tambien*/

    mov=L_Mover_Cte(&(ListaParser.parser), L_Primero);
    L_Elem_Cte(ListaParser.parser, &aux_parser);

    while (mov==OK){

        strcpy(elem.palabra, aux_parser.palabra);
        /*printf("ASD: %s\n", elem.palabra);*/
        L_Crear(&(elem.posiciones), sizeof(TPosicion));
        elem.cont=0; /*inicializo el contador*/
        while((mov==OK)&&(strcmp(elem.palabra, aux_parser.palabra)==0)){   /*mientras no termine y sea igual la palabra*/
            posicion.linea=aux_parser.linea;
            posicion.pag=aux_parser.pagina;
            posicion.pos=aux_parser.pos;
            if (L_Vacia(elem.posiciones))   /*si esta vacia la lista, lo agrego en el primero. si no, en el siguiente*/
                L_Insertar_Cte(&(elem.posiciones), L_Primero, &posicion);
            else
                L_Insertar_Cte(&(elem.posiciones), L_Siguiente, &posicion);
            (elem.cont)++;
            mov=L_Mover_Cte(&(ListaParser.parser), L_Siguiente);
            L_Elem_Cte(ListaParser.parser, &aux_parser);
        }
        /*guardo el elem en el arbol*/

        if (AB_Vacio(g->arbol)){ /*si el arbol esta vacio, lo inserto en el raiz*/
            AB_Insertar(&(g->arbol), RAIZ, &elem);
            /*printf("%s. %d\n", elem.palabra, elem.cont);*/
        }
        else{
            AB_MoverCte(&(g->arbol), RAIZ); /*como no esta vacio el arbol, no puede dar error*/
            guardar_ordenado(&(g->arbol), elem);
            /*printf("%s. %d\n", elem.palabra, elem.cont);*/
        }

    }
    printf("\n--MUESTRO LO QUE HAY EN EL ARBOL--\n");
    in_order(g->arbol, RAIZ); /*para chequear que este bien guardado en el arbol*/
    printf("\n----------------\n");
    if (mov!=OK)
        return 0; /*termino de recorrer la lista parser*/

	return 1;

}

int guardar_ordenado(TAB *arbol, TPalabra elem){    /*esta hecho recursivo*/
    int error=OK;
    TPalabra aux;

    if (AB_Vacio(*arbol)){ /*si el arbol esta vacio, lo inserto en el raiz*/
        error=AB_Insertar(arbol, RAIZ, &elem);
        return error;
    }

    AB_ElemCte(*arbol, &aux);

    if(strcmp(elem.palabra, aux.palabra)<0){
        error=AB_MoverCte(arbol, IZQ);
        if (error==FALSE) /*si no se pudo mover es porque no tiene un hijo IZQ. entonces inserto a la izquierda*/
            return AB_Insertar(arbol, IZQ, &elem);
        else
            return(guardar_ordenado(arbol, elem));
    }

    if(strcmp(elem.palabra, aux.palabra)>0){
        error=AB_MoverCte(arbol, DER);
        if (error==FALSE) /*si no se pudo mover es porque no tiene un hijo DER. entonces inserto a la derecha*/
            return AB_Insertar(arbol, DER, &elem);
        else
            return(guardar_ordenado(arbol, elem));
    }

    return error;

}

void in_order(TAB arbol, int mov){
    TPalabra elem;
    int error;
    int movim=OK;
    TPosicion pos;

    error=AB_MoverCte(&arbol, mov);
    if(error==TRUE){
        in_order(arbol, IZQ);
        AB_ElemCte(arbol, &elem);
        printf("\n%s. %d\n", elem.palabra, elem.cont);
        L_Mover_Cte(&(elem.posiciones), L_Primero);

        while(movim==OK){   /*recorro la lista de las posiciones de cada palabra*/
            L_Elem_Cte(elem.posiciones, &pos);
            printf("PAG: %d - LIN: %d - POS: %d\n", pos.pag, pos.linea, pos.pos);

            movim=L_Mover_Cte(&(elem.posiciones), L_Siguiente);
        }
        in_order(arbol, DER);

    }


}

int DestruirGlosario(TDAGlosario* g)
{

	AB_Vaciar(&g->arbol);

	return TRUE;

}

int ConsultarPalabraGlosario(TDAGlosario* g, char* palabra, TListaSimple* LResultado)
{
    int error=OK;
    TPalabra elem;

    printf("\nPALABRA BUSCADA: %s\n", palabra);

    /*si busqueda   devuelve 1 es porque no se encontro
                    devuelve 0 si se encontro
    */
    error=busqueda(&(g->arbol), palabra);
    if (error==1){  /*si no lo encontro*/
        printf("La palabra %s no se encontro\n", palabra);
        return error;
    }
    AB_ElemCte(g->arbol, &elem);
    printf("PAL: %s %d\n", elem.palabra, elem.cont);

	/* Magia */

	return TRUE;

}

int busqueda(TAB *arbol, char *palabra){
    TPalabra elem;
    int error=OK;
    int mov;

    if (AB_Vacio(*arbol)){
        printf("VACIOO");
        return 1;
    }
    AB_ElemCte(*arbol, &elem);

    if(strcmp(elem.palabra, palabra)==0)
        return 0;
    if(strcmp(elem.palabra, palabra)>0)
        mov=IZQ;
    else
        mov=DER;
    printf("MOV=%d\n",mov);
    error=AB_MoverCte(arbol, mov);
    if(error==TRUE){
        error=busqueda(arbol, palabra);
        return error;
    }
    else{

        return 1;
    }

}

int Ranking_palabras_Glosario(TDAGlosario* g, TListaSimple* LResultado)
{

	/* Más Magia */

	return TRUE;

}
