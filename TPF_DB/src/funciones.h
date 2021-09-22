
//Pide el nombre del usuario
void nombre(){
	char nombre[20];
	printf("Ingrese su nombre por favor: ");
	fgets(nombre, 20, stdin);
	printf("\nTu nombre es: %s", nombre);
}

//Decide quien inicia
void jugador(){
	int opciones = 0;
	//Consulta si desea elegir o no
	printf("\nDesea elegir quien comienza? --> 1 = SI || 0 = NO: ");
	scanf("%d", &opciones);
	while ( opciones !=1 && opciones!= 0 ){  			//Pide el valor hasta que ingrese el correcto
		printf("\nError, el valor ingresado no corresponde. Vuelva a ingresar por favor (1 = SI || 0 = NO): ");
		scanf("%d", &opciones);
	}

	if(opciones == 1){
		printf("\nQuien iniciara el juego?--> 1 = Usted || 0 = La computadora : ");
		scanf("%d", &opciones);
		while (opciones !=1 && opciones != 0){		//Pide el valor hasta que ingrese el correcto
			printf("\nError, los valores ingresados no son correctos. Vuelva a ingresar por favor (1 = Usted || 0 = La computadora): ");
			scanf("%d", &opciones);
		}
	}else{
		//Elige de manera aleatoria numeros menores a 15 pero mayores a 1
		srand(time(NULL));
		opciones=rand() % 2;
	}

	//return opciones; IMPRESION DE PRUEBA
	if (opciones == 0){
		printf("\n Comienza la computadora.\n");
	}else{
		printf("\n Comienza la usted.\n");
	}
}

//Decide el color que empieza
void color(){
	int c = 0;
	printf("\nInician los Verdes o Rojas?--> 1 = VERDE || 0 = ROJO: ");
	scanf("%d", &c);
	while (c !=1 && c != 0){		//Pide el valor hasta que ingrese el correcto
		printf("\n Error, los valores ingresados no son correctos. Vuelva a ingresar por favor (1 = VERDE || 0 = ROJO): ");
		scanf("%d", &c);
	}
	//return c; IMPRESION DE PRUEBA
	if (c == 0){
		printf("\n Comienzan los rojos.\n");
	}else{
		printf("\n Comienzan los verdes.\n");
	}
}

//Define el tamaño de la matriz
int dim_matriz(){
	int dim, a = 0;
	printf("\nDesea elegir la dimension de la matriz? --> 1 = SI || 0 = NO: ");
	scanf("%d", &a);
	while ( a !=1 && a!= 0 ){  			//Pide el valor hasta que ingrese el correcto
		printf("\nError, el valor ingresado no corresponde. Vuelva a ingresar por favor (1 = SI || 0 = NO): ");
		scanf("%d", &a);
	}

	//Ingresa el tamaño
	if (a == 1){
		printf("\nPor favor ingrese la dimension de la matriz: ");
		scanf("%d", &dim);
	}else{
		//Elige de manera aleatoria numeros menores a 15 pero mayores a 1
		dim=rand() % 15;
		while (dim <= 1){
			dim=rand() % 15;
		}
	}
	return dim;
}


 char crear_matriz(int dim){
	int i,j=0;
	char matriz[dim][dim];
	//primero se cera la matriz
	for (i=0; i<dim; i++){
		for (j=0; j<dim ; j++){
			matriz[i][j]=0;
		}
	}

	for (i=0; i<dim; i++){
		for (j=0; j<dim ; j++){
			matriz[i][j]= '*';
		}
	}
	return matriz[dim][dim];
 }

 //Imprime lindo la matriz
/* void imprimir_matriz(int dim, char (*matriz)[dim][dim]){
    printf("\n");
 	int i, j;
 	//Imprime los numeros de arriba
 	printf("    ");
 	for(i=0; i < dim; i++){
 		printf(" %d |", i+1);
 	}
 	printf("\n------------------------------------\n");

 	//Imprime los numeros del costado
 	for(i=0; i < dim; i++){
 		printf(" %d |", i+1);
 		for (j=0; j < dim; j++){
 			printf(" %s |", matriz[i][j] );
 		}
 	printf("\n------------------------------------\n");
 	}
 }*/
