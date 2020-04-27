#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*constantes*/
#define MAX_STR 64	/*tamanho maximo de uma cadeira de caracteres*/
#define MAX_INPUT 343	/*maximo de input de um comando*/
#define MAX_EVENT 1000	/*maximo de eventos*/
#define MAX_PARTICIPANT 3	/*maximo de participantes por evento*/


typedef struct{										
	int data;
	int inicio;
	int duracao;
	int sala;
	char descricao[MAX_STR+1];
	char responsavel[MAX_STR+1];
	int n_participantes;
	char participantes[MAX_PARTICIPANT][MAX_STR];
} Evento;

/*variaveis globais*/
Evento eventos[MAX_EVENT];		/*vetor onde sao guardados os eventos*/
int contador_eventos = 0;		/*inteiro que contabiliza o numero de eventos*/

/*funcoes axuiliares*/
int eventos_sobrepostos(Evento a, Evento b);
int hora_fim(int inicio, int duracao);
void escreve_evento(int i);
int verificar_evento(Evento a);
int existe_evento(char descricao[]);

/*funcoes de ordenacao*/
void insertion_sort(Evento a[], int l, int r);
int less(Evento a, Evento b);
int converte_data(int data);

/*funcoes de comandos*/
int loop_principal();
void add_evento();
void list_eventos();
void list_eventos_por_sala();
void del_evento();
void alt_evento_hora();
void alt_evento_duracao();
void alt_evento_sala();
void add_participante();
void del_participante();
void find_ocorrencias();


int main(){
	loop_principal();	/*inicializar o loop principal*/
	return 0;
}
	
int loop_principal(){	/*dado um comando introduzido, e executada determinada
						 funcao, correspondente a esse comando*/
	int c;
	while ((c = getchar()) != 'x'){
		switch(c) {
			case 'a':
				getchar();
				add_evento();
				break;			
			case 'l':
				list_eventos();
				break;			
			case 's':
				getchar(); 
				list_eventos_por_sala();
				break;			
			case 'r':
				getchar();
				del_evento();
				break;		
			case 'i':
				getchar();
				alt_evento_hora();
				break;			
			case 't':
				getchar();
				alt_evento_duracao();
				break;
			case 'm':
				getchar();
				alt_evento_sala();
				break;
			case 'A':
				getchar();
				add_participante();
				break;
			case 'R':
				getchar();
				del_participante();
				break;
			case 'Z':
				find_ocorrencias();
			default:
				break;
		}
	}
	return 0;
}



/*FUNCOES DE COMANDOS*/

void add_evento(){	/*adiciona um evento ao vetor de estruturas*/
	Evento novo_evento;	/*variavel de tipo evento que recebe os varios conteudos do input*/
	char str_participantes[MAX_INPUT];	/*string que recebe as descricoes dos participantes*/
	char *token;

	if (contador_eventos < MAX_EVENT){
		scanf("%[^:]:%d:%d:%d:%d:%[^:]:%[^\n]", novo_evento.descricao, 
												&novo_evento.data, 
												&novo_evento.inicio, 
												&novo_evento.duracao, 
												&novo_evento.sala,
												novo_evento.responsavel,
												str_participantes);

		novo_evento.n_participantes = 0;
		token = strtok(str_participantes, ":");

		while (token != NULL){
			strcpy(novo_evento.participantes[novo_evento.n_participantes], token);			
			token = strtok(NULL, ":");
			novo_evento.n_participantes++;
		}
	
		/*verificar a validade do evento (sobreposicao de eventos em relacao a salas e participantes)*/
		if (verificar_evento(novo_evento)){
			
			eventos[contador_eventos] = novo_evento;	
		
			/*ordenar o novo evento do vetor por ordem cronológica*/
			contador_eventos++;
			insertion_sort(eventos, 0, contador_eventos - 1);
		}
		else
			return;
	}	
	
	else{
		printf("Erro: Limite de Eventos atingido.\n");
		return;
	}	
}


void list_eventos(){	/*lista todos os eventos por ordem cronologica*/
	int i;
	for (i = 0; i < contador_eventos; i++){
		escreve_evento(i);
	}
}


void list_eventos_por_sala(){	/*lista todos os eventos de uma determinada sala por ordem cronologica*/
	int i; 
	int room;
	scanf("%d", &room);
	for (i = 0; i < contador_eventos; i++){
		if (eventos[i].sala == room){
			escreve_evento(i);
		}
	}
}


void del_evento(){	/*apaga um evento do vetor*/
	char descricao[MAX_STR];
	int i;

	scanf("%[^\n]", descricao);
	i = existe_evento(descricao);

	if (i != -1){
		for (; i < contador_eventos; i++){	/*o evento é apagado e no lugar dele e escrito o evento imediatamente a
											seguir, chegando ao ultimo elemento sendo preenchido por um evento vazio*/
			eventos[i] = eventos[i + 1];		
		}
		contador_eventos--;	
	}
	else
		return;	
}


void alt_evento_hora(){	/*altera a hora de um evento*/
	Evento eve_alterado;	
	char descricao[MAX_STR];	
	int novo_inicio;	
	int i;	

	scanf("%[^:]:%d", descricao, &novo_inicio);
	i = existe_evento(descricao);
	
	if (i != -1){
		eve_alterado = eventos[i];
		eve_alterado.inicio = novo_inicio;
	
		if (verificar_evento(eve_alterado)){
			eventos[i] = eve_alterado;	
			insertion_sort(eventos, 0, contador_eventos - 1);	/*com o inicio do evento alterado, ordena-lo no vetor por
																ordem cronologica*/	
		}
	}
	else 
		return;
}


void alt_evento_duracao(){	/*altera a duracao de um evento*/
	Evento eve_alterado;
	char descricao[MAX_STR];
	int nova_duracao, i;

	scanf("%[^:]:%d", descricao, &nova_duracao);
	i = existe_evento(descricao);	
	if (i != -1){
		eve_alterado = eventos[i];
		eve_alterado.duracao = nova_duracao;

		if (verificar_evento(eve_alterado)){
			eventos[i] = eve_alterado;
			insertion_sort(eventos, 0, contador_eventos -1);
		}
	}
	else
		return;
}


void alt_evento_sala(){	/*altera a sala de um evento*/
	Evento eve_alterado;
	char descricao[MAX_STR];
	int nova_sala, i;
 
	scanf("%[^:]:%d", descricao, &nova_sala);
	i = existe_evento(descricao);	
	if (!(i ==-1)){
		eve_alterado = eventos[i];
		eve_alterado.sala = nova_sala;
		if (verificar_evento(eve_alterado)){

			eventos[i] = eve_alterado;	
			insertion_sort(eventos, 0, contador_eventos -1);
		}
	}
	else
		return;
}


void add_participante(){	/*adiciona um participante a um evento*/
	Evento eve_alterado;
	char descricao[MAX_STR], novo_participante[MAX_STR];
	int i, j, c;
	
	scanf("%[^:]:%[^\n]", descricao, novo_participante);
	i = existe_evento(descricao);
	
	if (i != -1){
		if (eventos[i].n_participantes == 3){
			printf("Impossivel adicionar participante. Evento %s ja tem 3 participantes.\n", descricao);
			return;
		}
		for (j = 0; j < eventos[i].n_participantes; j++){
			if (!strcmp(eventos[i].participantes[j], novo_participante) || !strcmp(eventos[i].responsavel, novo_participante))
				return;
		}

		eve_alterado = eventos[i];	
		eve_alterado.n_participantes++;
		strcpy(eve_alterado.participantes[eve_alterado.n_participantes-1], novo_participante);
			
		for (j = 0; j < contador_eventos; j++){	/*verificar se a adicao do novo participante nao origina conflito entre eventos*/
			if (!strcmp(eventos[j].responsavel, novo_participante)){
				if (eventos_sobrepostos(eventos[j], eve_alterado)){
					printf("Impossivel adicionar participante. Participante %s tem um evento sobreposto.\n", novo_participante);
					return;
				}
			}

			for (c = 0; c < eventos[j].n_participantes; c++){
				if (!strcmp(eventos[j].participantes[c], novo_participante)){
					if (eventos_sobrepostos(eventos[j], eve_alterado)){
						printf("Impossivel adicionar participante. Participante %s tem um evento sobreposto.\n", novo_participante);
						return;				
					}
				}
			}
		}
		eventos[i] = eve_alterado;
	}
	else 
		return;
}


void del_participante(){	/*apaga um participante de um dado evento*/
	char descricao[MAX_STR], part_a_remover[MAX_STR];
	int i, j, index = 0;
	
	scanf("%[^:]:%[^\n]", descricao, part_a_remover);
	i = existe_evento(descricao);

	if (i != -1){
		if (eventos[i].n_participantes == 1 && !strcmp(eventos[i].participantes[0], part_a_remover)){
			printf("Impossivel remover participante. Participante %s e o unico participante no evento %s.\n", part_a_remover, descricao);
			return;
		}
		else{
			for (j = 0; j < eventos[i].n_participantes; j++){
				if (!strcmp(eventos[i].participantes[j], part_a_remover))
					break;
				else
					index++;
			}

			if (index == eventos[i].n_participantes)	/*se o participante nao existir no evento em causa*/
				return;

			for (; index < eventos[i].n_participantes; index++){	/*deslocar todos os eventos do vetor, uma posicao para a esquerda*/
				strcpy(eventos[i].participantes[index], eventos[i].participantes[index +1]);
			}
			eventos[i].n_participantes--;
		}
	}
}


void find_ocorrencias(){
	int i, counter = 0;
	char string[2] = {'Z', 'e'};
	for (i = 0; i < contador_eventos; i++){
		if (!strcmp(eventos[i].responsavel, string)){
			counter++;
		}
	}
	printf("%d\n", counter);
}




/*FUNCOES AUXILIARES*/

int hora_fim(int inicio, int duracao){	/*recebe uma hora de inicio de um evento e um periodo de tempo e retorna
										a essa hora passada tal duracao*/
	int horas_em_minutos, hora_final, min_final;
	horas_em_minutos = (inicio / 100) * 60 + inicio % 100;
	horas_em_minutos += duracao;
	hora_final = horas_em_minutos / 60;
	min_final = horas_em_minutos - (hora_final * 60);
	return hora_final * 100 + min_final;
}


void insertion_sort(Evento a[], int l, int r){	/*algoritmo de ordenacao que devolve os eventos ordenados
												por ordem cronológica*/
	Evento v;
	int i,j; 
	for (i = l+1; i <= r; i++) { 
		v = a[i]; 
		j = i-1; 
		while (j>=l && less(v, a[j]) ) {	 
			a[j+1] = a[j]; 
			j--; 
		} 
		eventos[j+1] = v; 
	} 
}


int less(Evento a, Evento b){	/*funcao auxiliar para o algoritmo de ordenacao de eventos*/
	if (converte_data(a.data) < converte_data(b.data))
		return 1;
	else if(converte_data(a.data) == converte_data(b.data)){
		if (a.inicio == b.inicio)
			return (a.sala < b.sala);
		else
			return (a.inicio < b.inicio);
		}
	else
		return 0;
}	


int converte_data(int data){	/*recebe uma data no formato DDMMAAAA e converte para AAAAMMDD*/
	int ano_mes;
	int dia;
	ano_mes = data % 1000000;	/*ano_mes = AAAAMM*/
	dia = data / 1000000;		/*dia = DD*/
	return ano_mes * 100 + dia;	/*AAAAMM * 100 + DD*/
}

int eventos_sobrepostos(Evento a, Evento b){	/*recebe dois eventos e retorna o valor logico correspondente
												ao conflito de horarios entre si*/
	if (a.data == b.data){
		if (a.inicio <= b.inicio && b.inicio < hora_fim(a.inicio, a.duracao))
			return 1;
		else if (a.inicio < hora_fim(b.inicio, b.duracao) && hora_fim(b.inicio, b.duracao) <= hora_fim(a.inicio, a.duracao))
			return 1;
		else if (b.inicio <= a.inicio && hora_fim(b.inicio, b.duracao) >= hora_fim(a.inicio, a.duracao))
			return 1;
		else if (b.inicio >= a.inicio && hora_fim(b.inicio, b.duracao) <= hora_fim(a.inicio, a.duracao))
			return 1;
		else
			return 0;		
	}
		return 0;
}

int verificar_evento(Evento a){	/*verifica se dado evento nao apresenta conflitos com outros eventos ja criados*/
	int i, j, c, evento_impossivel = 1;	

	for (i = 0; i < contador_eventos; i++){
		if (!strcmp(a.descricao, eventos[i].descricao))
			continue;
		
		if (a.sala == eventos[i].sala)	/*se os dois eventos têm a mesma sala*/
			if (eventos_sobrepostos(a, eventos[i])){
				printf("Impossivel agendar evento %s. Sala%d ocupada.\n", a.descricao, a.sala);
				evento_impossivel = 0;
				return evento_impossivel;
			}		

		if (!strcmp(a.responsavel, eventos[i].responsavel))	/*se os dois eventos têm o mesmo responsavel*/
			if (eventos_sobrepostos(a, eventos[i])){
				printf("Impossivel agendar evento %s. Participante %s tem um evento sobreposto.\n", a.descricao, a.responsavel);
				evento_impossivel = 0;
			}

		for (j = 0; j < a.n_participantes; j++)
			if (!strcmp(a.participantes[j], eventos[i].responsavel))	/*se algum participante do evento a corresponde ao responsavel do segundo evento*/
				if (eventos_sobrepostos(a, eventos[i])){
					printf("Impossivel agendar evento %s. Participante %s tem um evento sobreposto.\n", a.descricao, a.participantes[j]);
					evento_impossivel = 0;
				}

		for (j = 0; j < eventos[i].n_participantes; j++){
			if (!strcmp(a.responsavel, eventos[i].participantes[j]))	/*se o responsavel do evento a e algum participante do segundo event*/
				if (eventos_sobrepostos(a, eventos[i])){
					printf("Impossivel agendar evento %s. Participante %s tem um evento sobreposto.\n", a.descricao, a.responsavel);
					evento_impossivel = 0;
				}
			for (c = 0; c < a.n_participantes; c++)
				if (!strcmp(a.participantes[c], eventos[i].participantes[j]))	/*se existem algum participante nos dois eventos*/
					if (eventos_sobrepostos(a, eventos[i])){
						printf("Impossivel agendar evento %s. Participante %s tem um evento sobreposto.\n", a.descricao, a.participantes[c]);
						evento_impossivel = 0;
					}
		}
	}
	return evento_impossivel;
}

void escreve_evento(int i){	/*escreve dado evento com indice i no ecra*/
	int j; 

	printf("%s %.8d %.4d %d Sala%d %s\n", eventos[i].descricao,
										eventos[i].data,
										eventos[i].inicio,
										eventos[i].duracao,
										eventos[i].sala,
										eventos[i].responsavel);

	printf("*");
	for(j = 0; j < eventos[i].n_participantes; j++){
		printf(" %s", eventos[i].participantes[j]);
	}
	printf("\n");
}


int existe_evento(char string[]){ /*verifica se existe um evento dada uma descricao*/
	int i;
	for (i = 0; i < contador_eventos; i++){
		if (!strcmp(eventos[i].descricao, string))
			return i;				/*existe, logo retorna o indice do evento*/
		
	}
	printf("Evento %s inexistente.\n", string);
	return -1;						/*nao existe, logo retorna -1*/
}


