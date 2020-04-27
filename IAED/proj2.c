/*Antonio Venancio, tg, 93689*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE_NAME 1014
#define SIZE_EMAIL 512
#define SIZE_NUMBER 64
#define SIZE_HASHTABLE 1021

/*structures*/
typedef struct{
	char *name;
	char *local;
	char *domain;
	char *number;
	char *original_name;
} Contact;

typedef struct node node;
typedef struct node* link;

struct node{
	Contact *contact;
	node *next;
};

/*global variables*/
link hashtable_name[SIZE_HASHTABLE];
link hashtable_domain[SIZE_HASHTABLE];

static link top = NULL;
static link bottom = NULL;

/*functions*/
void initialize_hashtable(link hashtable[]);
/*memory release functions*/
void free_memory();
void free_memory_list(link head, int clear_contact);
void free_node(link j, int clear_contact);
void free_memory_contact(Contact *contact);

/*list manipulation*/
link new_node(Contact *C);
link lookup(char* text);
link delete(link head, const char* text, int clear_contact);

/*hashtable manipulation*/
void add_contact_hashtable(char *key, Contact *Pcontact, link hashtable[]);
int hash_function(char* string, int M);

/*command functions*/
void main_loop();
void add_contact();
void list_contacts();
void find_contact();
void del_contact();
void alter_email();
void count_domain();
void count_contacts();

int main(){
	main_loop();
	return 0;
}

void main_loop(){
	int c;
	initialize_hashtable(hashtable_name);	/*set all elements to NULL*/
	initialize_hashtable(hashtable_domain);
	while ((c = getchar()) != 'x'){
		switch(c){
			case 'a':
				getchar();
				add_contact();
				break;	
			case 'l':
				list_contacts();
				break;
			case 'p':
				getchar();
				find_contact();
				break;
			case 'r':
				getchar();
				del_contact();
				break;
			case 'e':
				getchar();
				alter_email();
				break;
			case 'c':
				getchar();
				count_domain();
				break;
			case 'P':
				getchar();
				count_contacts();
			default:
				break;
		}
	}
	free_memory();	
	return;
}

link new_node(Contact *C){	/*given a pointer to a contact, it created a node with the information
							of the pointer*/
	link x = malloc(sizeof(node));
	x->contact = C;
	x->next = NULL;
	return x; 
}

void add_contact(){		/*given a name, an email address and a phone number, creates a contact*/
	Contact *new_contact;
	link i;
	int p = 0;
	char name[SIZE_NAME];
	char local[SIZE_EMAIL];
	char domain[SIZE_EMAIL];
	char number[SIZE_NUMBER];
	
	
	scanf("%[^ ] %[^@]@%[^ ] %[^\n]", name,
									local,
									domain,
									number);
	
	new_contact = (Contact*)malloc(sizeof(Contact));

	i = lookup(name);
	if (i != NULL){
		new_contact->original_name = (char*)malloc(sizeof(char) * (strlen(name)+1));
		strcpy(new_contact->original_name, name);
	} 
	else{
		new_contact->original_name = NULL;
	}
	
	while (i != NULL){
		while (name[p] != '\0'){
			p++;
		}
		name[p] = '_';
		name[p + 1] = '1';
		name[p + 2] = '\0';
		p = 0;
		i = lookup(name);
	}	
	
	new_contact->name = (char*)malloc(sizeof(char) * (strlen(name)+1));	/*alloc only the memory scanf used*/
	new_contact->local = (char*)malloc(sizeof(char) * (strlen(local)+1));
	new_contact->domain = (char*)malloc(sizeof(char) * (strlen(domain)+1));
	new_contact->number = (char*)malloc(sizeof(char) * (strlen(number)+1));
	
	strcpy(new_contact->name, name);
	strcpy(new_contact->local, local);
	strcpy(new_contact->domain, domain);
	strcpy(new_contact->number, number);

	if (top == NULL){	/*binds new_contact to the main_list*/
		top = new_node(new_contact);
		bottom = top;
	}
	else{
		bottom->next = new_node(new_contact);
		bottom = bottom->next;
	}
	
	add_contact_hashtable(new_contact->name, new_contact, hashtable_name);	/*binds contact to hashtable_name*/
	add_contact_hashtable(new_contact->domain, new_contact, hashtable_domain);	/*binds contact to hashtable_domain*/
	
	return;
}

void list_contacts(){	/*lists all contacts by order of creation, from oldest to newest*/
	link i = top;
	while(i != NULL){
		if (i->contact->original_name == NULL)
			printf("%s %s@%s %s\n", i->contact->name,
								  i->contact->local,
								  i->contact->domain,
								  i->contact->number);
		else	
			printf("%s (%s) %s@%s %s\n", i->contact->name,
									i->contact->original_name,
							  		i->contact->local,
							  		i->contact->domain,
							  		i->contact->number);
		i = i->next;
	}
	return;
}

void find_contact(){	/*given a contact name by input, finds a contact in the hashtable_name*/
	int k;
	link i;
	
	char *string;
	string = (char*)malloc(sizeof(char)*SIZE_NAME);
	scanf("%s", string);

	
	k = hash_function(string, SIZE_HASHTABLE);
	i = hashtable_name[k];
	while (i != NULL){
		if (!strcmp(i->contact->name, string)){
			printf("%s %s@%s %s\n", i->contact->name,
						     		i->contact->local,
							  		i->contact->domain,
							  		i->contact->number);
			free(string);
			return;
		}
		i = i->next;
	}
	printf("Nome inexistente.\n");
	free(string);
	return;
}

void del_contact(){	/*given a contact name by input, deletes that contact, if it exists*/
	link i;
	int k , l;
	char *string_name, *string_domain;
	string_name = malloc(sizeof(char) * SIZE_NAME);
	string_domain = malloc(sizeof(char) * SIZE_EMAIL);

	scanf("%s", string_name);
	i = lookup(string_name);
	if (i == NULL){
		printf("Nome inexistente.\n");
		free(string_name);
		free(string_domain);
		return;
	}
	/*clear nodes from the hashtables first, then clear contacts with the nodes
	from the main list*/
	strcpy(string_domain, i->contact->domain);
	k = hash_function(string_name, SIZE_HASHTABLE);
	hashtable_name[k] = delete(hashtable_name[k], string_name, 0);

	l = hash_function(string_domain, SIZE_HASHTABLE);
	hashtable_domain[l] = delete(hashtable_domain[l], string_name, 0);

	top = delete(top, string_name, 1);

	free(string_name);
	free(string_domain);
	return;
}

void alter_email(){	/*recieves input from a name and email and changes the old email
					address to the new one*/
	int k;
	link i;
	char *name;
	char *new_local;
	char *new_domain;
	name = (char*)malloc(sizeof(char) * SIZE_NAME);
	new_local = (char*)malloc(sizeof(char) * SIZE_EMAIL);
	new_domain = (char*)malloc(sizeof(char) * SIZE_EMAIL);

	scanf("%[^ ] %[^@]@%[^\n]", name, new_local, new_domain);
	name = (char*)realloc(name, sizeof(char) * (strlen(name) + 1));
	new_local = (char*)realloc(new_local, sizeof(char) * (strlen(new_local) + 1));
	new_domain = (char*)realloc(new_domain, sizeof(char) * (strlen(new_domain) + 1));

	i = lookup(name);
	if (i == NULL){
		printf("Nome inexistente.\n");
		free(name);
		free(new_local);
		free(new_domain);
		return;
	}
		/*since the domain has changed, its position in the hashtable_domain
		needs to change as well*/
	k = hash_function(i->contact->domain, SIZE_HASHTABLE);	
	hashtable_domain[k] = delete(hashtable_domain[k], i->contact->name, 0);
	
	free(i->contact->local);
	i->contact->local = new_local;
	free(i->contact->domain);
	i->contact->domain = new_domain;

	add_contact_hashtable(new_domain, i->contact, hashtable_domain);
	free(name);
	return;
}

void count_domain(){	/*counts the number of occorrunces of a domain given by input*/
	int k, counter = 0;
	link i;
	char *string;
	string = malloc(sizeof(char) * SIZE_EMAIL);

	scanf("%s", string);
	k = hash_function(string, SIZE_HASHTABLE);
	i = hashtable_domain[k];
	while(i != NULL){
		if (!strcmp(i->contact->domain, string)){
			counter++;
		}
		i = i->next;
	}
	printf("%s:%d\n", string, counter);
	free(string);
	return;
}

void count_contacts(){
	int counter = 0;
	char *token, pref[SIZE_NAME];
	link i = top;
	scanf("%s", pref);
	while (i != NULL){
		token = strtok(i->contact->name, "-");
		if (!strcmp(token, pref)){
			counter++;
		}
		i = i->next;
	}
	printf("%d\n", counter);
}


int hash_function(char *string, int M){	/*recieves a string and returns the index from the
										hash algorithm*/
	int h = 0, a = 127;

	for (; *string != '\0'; string++){
		h = (a*h + *string) % M;
	}
	return h;
}

void add_contact_hashtable(char *key, Contact *Pcontact, link hashtable[]){	/*recieves a hashkey, and binds a pointer
																			to a contact to hashtable*/ 
	link i;
	int k = hash_function(key, SIZE_HASHTABLE);
	i = new_node(Pcontact);
	i->next = hashtable[k];
	hashtable[k] = i;
	return;
}

link lookup(char* text){	/*see if contact named text exists, if so return a pointer from hashtable_name 
							linked to that contact*/
	link t;
	int k;
	k = hash_function(text, SIZE_HASHTABLE);
	for(t = hashtable_name[k]; t != NULL; t = t->next)
		if(strcmp(t->contact->name, text) == 0)
			return t;
	return NULL;
}

link delete(link head, const char* text, int clear_contact){	/*delete a node from a list*/
	link t, prev;
	int k = 0;
	for(t = head, prev = NULL; t != NULL;
		prev = t, t = t->next, k++){
		if (strcmp(t->contact->name, text) == 0){
			if (t == head){	
				head = t->next;
			}
			else{
				if (head == top)	/*if the list recieved is the main list*/
					if (t->next == NULL)	/*if it is the last element, the pointer bottom,
											is changed to the previous node*/
						bottom = prev;
				prev->next = t->next;
			}
			free_node(t, clear_contact);
			return head;
		}
	}
	return head;
}

void free_memory(){	/*free ALL memory allocated by the program*/
	int i;
	free_memory_list(top, 1);	/*clear the main list and all the contacts created*/
	for (i = 0; i < SIZE_HASHTABLE; i++){
		free_memory_list(hashtable_name[i], 0);	/*clear the memory allocated by the nodes from the hashtables*/
		free_memory_list(hashtable_domain[i], 0);
	}
	return;
}

void free_memory_list(link head, int clear_contact){  	/*free ALL memory allocated by a list, if clear contact is 1,
														clears the contacts binded to the list*/ 
	link i = head;
	link next;

   while (i != NULL){
    	next = i->next;	
    	free_node(i, clear_contact);
    	i = next;
    }
}

void free_node(link node, int clear_contact){	/*frees memory allocated by node, if flag clear_contact is 1,
												clears the contact binded to that node*/
	if (clear_contact){
		free_memory_contact(node->contact);
	}
	free(node);
}

void free_memory_contact(Contact *contact){	/*free ALL memory allocated by a contact*/
	free(contact->name);
	free(contact->local);
	free(contact->domain);
	free(contact->number);
	free(contact);
	return;
}

void initialize_hashtable(link hashtable[]){	/*sets all elements in the hashtable to NULL*/
	int i;
	for (i = 0; i < SIZE_HASHTABLE; i++){
		hashtable[i] = NULL;
	}
	return;
}

