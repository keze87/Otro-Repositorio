#include "TDAGlosario.h"

int Proceso(char* RutaDoc, char* RutaConf, char* RutaIns);

int main(int argc, char *argv[])
{

	/* recibe 3 rutas
	 * documento a leer
	 * archivo de configuraciones
	 * archivo de instrucciones */
	if (argc != 4)
	{

		fprintf(stderr,"Parámetros necesarios:\n");
		fprintf(stderr,"<Documento> <Configuraciones> <Instrucciones>\n");

		return FALSE;

	}

	return Proceso(argv[1],argv[2],argv[3]);

}

int Proceso(char* RutaDoc, char* RutaConf, char* RutaIns)
{

	FILE* Documento;
	FILE* Configuraciones;
	FILE* Instrucciones;
	char Linea[255];
	TDAGlosario Glosario;
	TListaSimple Lista;

	int error = OK;

	int destruido = FALSE;

	/* Compruebo que los archivos existan */
	Documento = fopen(RutaDoc,"r");

	if (Documento == NULL)
	{
		fprintf(stderr,"Archivo %s Inexistente\n",RutaDoc);
		return FALSE;
	}
	else
		fclose(Documento);

	Configuraciones = fopen(RutaConf,"r");

	if (Configuraciones == NULL)
	{
		fprintf(stderr,"Archivo %s Inexistente\n",RutaConf);
		return FALSE;
	}
	else
		fclose(Configuraciones);

	Instrucciones = fopen(RutaIns,"r");

	if (Instrucciones == NULL)
	{
		fprintf(stderr,"Archivo %s Inexistente\n",RutaIns);
		return FALSE;
	}

	error = CrearGlosario(&Glosario,RutaDoc,RutaConf);

	while ((!feof(Instrucciones)) && (error == OK)) /* Leo hasta que llegue al final */
	{

		fgets(Linea, 5, Instrucciones); /* Las instrucciones solo miden 4 caracteres (con el espacio) */

		if (strcmp(Linea,"-cp ") == 0)
		{
			
			fscanf(Instrucciones, "%s", Linea);
			/*fgets(Linea, 250, Instrucciones);*/

			error = ConsultarPalabraGlosario(&Glosario,Linea,&Lista);

		}
		else
		{
			if (strcmp(Linea,"-rp") >= 0)
			{

				error = Ranking_palabras_Glosario(&Glosario,&Lista);

			}
			else
			{
				if ((strcmp(Linea,"-dg") >= 0) && (destruido == FALSE))
				{

					error = DestruirGlosario(&Glosario);

					destruido = TRUE;

				}
			}
		}

	}

	if (destruido == FALSE)
		error = DestruirGlosario(&Glosario);

	fclose(Instrucciones);

	return error;

}
