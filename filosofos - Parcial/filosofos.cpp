#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

int n;
pthread_mutex_t tenedores[5]; //

int comida = 1000; 
int cnt = 0; //contador de veces que se reinicia la comida

void restaurar(){  //Funcion para restaurar la comida cuando llegue a 0.
    if (comida == 0){
        comida = 1000;
        cnt++; 
    }
}

int min(int num1, int num2){  //Funcion para determinar minimo, esta ayuda a identificar los tenedores del ultimo filosofo.
    int result;
    if(num1<num2){
        result = num1;
    }
    else{
        result = num2;
    }
    return result;
}

int max(int num1, int num2){ //Funcion para determinar maximo, esta ayuda a identificar los tenedores del ultimo filosofo.
    int result;
    if(num1>num2){
        result = num1;
    }
    else{
        result = num2;
    }
    return result;
}

void *filosofo(void *i2){  //Funcion que ejecuta el hilo
    int i = *((int *)i2);  //Variable que contiene el numero del filosofo
    int estomagototal = 50 + rand() % 50;  //Estomago total que puede iniciar de 50 a 100 unidades.
    int estomago = 0; //recurso propio del hilo
    int energiagastada=1; //La energia empieza en 1 ya que el filosofo va a pensar, aun no afecta al estomago.
    printf("Filosofo %d pensando\n",i);
	sleep(random() % 10);
    for (;;){
        int m1= min(i, (i+1)%n); //Determinando tenedores del filosofo
        int m2= max(i, (i+1)%n); //Determinando tenedores del filosofo
        pthread_mutex_lock(&tenedores[m1]); //Bloqueando el recurso compartido para que el hilo que lo quiera usar espere hasta que este hilo lo desbloquee
        printf("Filosofo %d consiguio el tenedor derecho\n",i);
        pthread_mutex_lock(&tenedores[m2]); //Bloqueando el recurso compartido para que el hilo que lo quiera usar espere hasta que este hilo lo desbloquee
        printf("Filosofo %d consiguio el tenedor izquierdo\n",i);
        restaurar(); //Antes de consumir se verifica para restaurar.
        comida--; //Se resta al recurso compartido, como ya bloquee los tenedores no hay necesidad de bloquear la comida ya que lo haran ordenadamente los hilos.
        estomago++; //El contador del estomago aumenta en 1 cada vez que come.
        printf("Filosofo %d comio y cesa, Comida Total: %d, Estomago: %d/%d, Energia Gastada: %d,\n",i, comida, estomago, estomagototal, energiagastada);
        if(estomago==estomagototal){ //Cuando el filosofo se llena se pone a pensar, por ende gasta energia.
            energiagastada++;
            estomago--; //Como gasto energia se resta el contador del estomago.
            printf("Filosofo %d lleno, se puso a pensar, Estomago: %d/%d, Energia Gastada: %d,\n",i, estomago, estomagototal, energiagastada);
        }
        pthread_mutex_unlock(&tenedores[i]); //Terminando el filosofo de comer, se desbloquea el recurso compartido (tenedor)
        pthread_mutex_unlock(&tenedores[(i+1)%n]); //Terminando el filosofo de comer, se desbloquea el recurso compartido (tenedor)
        sleep(random() % 10); //se manda a pausar un rato el hilo para que otro tenga la oportunidad de comer y asi todos lleguen a comer.
    }
}

int main(void){
    printf("Ingrese Cantidad de Filosofos a iniciar: ");
    scanf("%d",&n);
    &tenedores[n];
    
    pthread_t filosofos[n];
    int iden[n];
    
    int i;
    for(i=0;i<n;i++){
        iden[i]=i+1;
        pthread_create(&filosofos[i],NULL,&filosofo,&iden[i]);
    }
    for(i=0;i<n;i++){
        pthread_join(filosofos[i],NULL);
    }
    return 0;
}



