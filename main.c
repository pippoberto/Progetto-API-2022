#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 64

typedef struct node {

	char * parola;     // chiave
	struct node * dx;
	struct node * sx;
	struct node * padre;
	struct node * succ_lista;
	int fil;

} node;

typedef struct regole_s {

	char * lettere_gg;  
	char * lettere_gs;
	char * no_lettere;
	char * lettere_occ;
	int * pos_gg;
	int * pos_gs;
	int * occ_min;
	int * occ_esatt;

} regole_s;

int ordina(char *, char *);

node * inserisci(node *);  // inserisce un valore nel bst in ordine

int f_confronto(char *, char *);

node * cerca(char *);  // cerca un valore nell'albero restituisce l'indirizzo

void inizializza(void);

int filtro(char *);

int conta(void);

void stampa_filtrate(node *);

void inizializza_tree(void);

regole_s * regole;
node * root, * first_valid, * last_valid, * last;
int k, nuove_info;

int main (int argc, char * argv[]) {

	int i, j, val, tentativi, vittoria, parole_valide; // lunghezza delle parole
	char parola[400], riferimento[400];
	node * nodo = NULL, * tmp;;

	root = NULL;
	parole_valide = 0;

	regole = malloc(sizeof(regole_s));

	val = fscanf(stdin, "%d", &k);

	regole->lettere_gg = malloc(sizeof(char) * (k+1));
	regole->lettere_gs = malloc(sizeof(char) * k*k);
	regole->no_lettere = malloc(sizeof(char) * SIZE);
	regole->lettere_occ = malloc(sizeof(char) * (k+1));
	regole->pos_gg = malloc(sizeof(int) * (k+1));
	regole->pos_gs = malloc(sizeof(int) * k*k);
	regole->occ_min = malloc(sizeof(int) * (k+1));
	regole->occ_esatt = malloc(sizeof(int) * (k+1));

	do {
		val = fscanf(stdin, "%s", parola);

		if (parola[0] != '+') {                       // ricevo la sequenza iniziale di parole
			
			nodo = malloc(sizeof(node) + (sizeof(char) * (k+1)));
			nodo->parola = (char *)nodo + sizeof(node);


			for (j = 0; j < k; ++j) {
				nodo->parola[j] = parola[j];
			}
			nodo->parola[j] = '\0';
			nodo->sx = NULL;
			nodo->dx = NULL;
			nodo->padre = NULL;
			nodo->fil = 1;
			
			if (last == NULL) {
				last = nodo;            // il primo 
				last_valid = nodo;
			}
			nodo->succ_lista = first_valid;
			first_valid = nodo; 

			root = inserisci(nodo); 

		}

	} while (parola[0] != '+');                       // esco quando leggo un +

	do {
		if (parola[0] == '+' && parola[1] == 'i') {							// +inserisci_inizio
			do {
				val = fscanf(stdin, "%s", parola);
				if (parola[0] != '+') {                         // serie di parole da aggiungere al vocabolario
					
					nodo = malloc(sizeof(node) + (sizeof(char) * (k+1)));
					nodo->parola = (char *)nodo + sizeof(node);

					for (j = 0; j < k; ++j) {
						nodo->parola[j] = parola[j];
					}
					nodo->parola[j] = '\0';
					nodo->sx = NULL;
					nodo->dx = NULL;
					nodo->padre = NULL;
					nodo->fil = 1;
					
					nodo->succ_lista = first_valid;
					first_valid = nodo;

					root = inserisci(nodo); 
				}
			} while (parola[0] != '+');					// +inserisci_fine
		} else {
			                                            // +nuova_partita
			if (parola[0] == '+' && parola[1] == 'n') {
				vittoria = 0;
				inizializza();			// inizializzo la struttura errori		
				inizializza_tree();
				val = fscanf(stdin, "%s", riferimento);       // parola da indovinare
				val = fscanf(stdin, "%d", &tentativi);        // nuomero tentativi
				for (i = 0; i < tentativi && vittoria == 0; ++i) {
					
					val = fscanf(stdin, "%s", parola);

					if (parola[0] == '+') {
						
						if (parola[1] == 'i') { 			// +inserisci_inizio
							
							do {
								val = fscanf(stdin, "%s", parola);
								if (parola[0] != '+') {             // inserimento

									nodo = malloc(sizeof(node) + (sizeof(char) * (k+1)));
									nodo->parola = (char *)nodo + sizeof(node);  
									
									for (j = 0; j < k; ++j) {
										nodo->parola[j] = parola[j];
									}
									nodo->parola[j] = '\0';
									nodo->sx = NULL;
									nodo->dx = NULL;
									nodo->padre = NULL;
									
									if (filtro(parola) == 1) {
										nodo->fil = 1;
										nodo->succ_lista = first_valid;
										first_valid = nodo;       // le pongo tutte valide
										parole_valide++;
									} else {
										nodo->fil = 0;
										tmp = last_valid->succ_lista;
										last_valid->succ_lista = nodo;
										nodo->succ_lista = tmp;
									}
									
									root = inserisci(nodo); 

								}
							} while (parola[0] != '+');  // +inserisci_fine
						} else {
							                            // +stampa_filtrate
							if (parola[1] == 's') {
								stampa_filtrate(root);
							}
						}
						i--;
						
					} else {
						
						nodo = cerca(parola);

						if (nodo != NULL) {
							
							vittoria = f_confronto(riferimento, parola);
							
							if (vittoria == 0) {
								if (nuove_info == 1) {
									
									parole_valide = conta();
									
								}
																
								fprintf(stdout, "%d\n", parole_valide);

							}

						} else {
							
							fprintf(stdout, "not_exists\n");

							i--;
						}

					}
				}
				if (vittoria == 0) {
					
					fprintf(stdout, "ko\n");

				}
			}
		}
		val = fscanf(stdin, "%s", parola);
	} while (val != EOF);

	return 0;

}

int ordina(char * parola_1, char * parola_2) {  

	for (int i = 0; i < k; i++) {
		if (parola_1[i] < parola_2[i]) {
			return 1;
		} else if (parola_1[i] > parola_2[i]) {
			return 2;
		}
	}

	return 0;

}

node * inserisci (node * nodo) {

	int pos;
	node * curr, * prev;

	curr = root;
	prev = root;
	while (curr != NULL) {

		pos = 0;
		for (int i = 0; i < k && pos == 0; ++i) {
			if (nodo->parola[i] < curr->parola[i]) {
				pos = 1;
			} else if (nodo->parola[i] > curr->parola[i]) {
				pos = 2;
			}
		}

		if (pos == 1) {
			prev = curr;
			curr = curr->sx;
		} else {
			prev = curr;
			curr = curr->dx;
		}

	}
	if (prev == NULL) {
		root = nodo;
	} else if (pos == 1) {
		nodo->padre = prev;
		prev->sx = nodo;
	} else {
		nodo->padre = prev;
		prev->dx = nodo;
	}

	return root;

}

int f_confronto(char * parola_0, char * parola_1) {

	int somma, uguali, i, j, esci, p; // k lunghezza delle parola
	char occorrenze[k], confronto[k];    // parola_0 quella da trovare

	somma = 0;
	uguali = 0;
	nuove_info = 0;

	for (i = 0; i < k; ++i) {            // segno le giuste al posto giusto
		if (parola_0[i] == parola_1[i]) {
			confronto[i] = '+';
			occorrenze[i] = '1';
			++somma;

			j = 0; 							// aggiungo info alla struttura errori (solo se non c'è già)
			esci = 0;
			while (esci == 0 && regole->lettere_gg[j] != '*') {  
				if (regole->lettere_gg[j] == parola_0[i] && regole->pos_gg[j] == i) {
					esci = 1;
				} else {
					++j;
				}
			}
			if (esci == 0) {
				regole->lettere_gg[j] = parola_0[i];
				regole->pos_gg[j] = i;
				nuove_info = 1;
			}

		} else {
			confronto[i] = '/';
			occorrenze[i] = '0';
		}

	}

	if (somma == k) {
		
		fprintf(stdout, "ok\n");

		uguali = 1;
	} else {

		for (i = 0; i < k && somma < k; ++i) {
			if (confronto[i] == '/') { 
				for (j = 0; j < k; ++j) {
					if (occorrenze[j] == '0' && parola_0[j] == parola_1[i]) {
						confronto[i] = '|';
						occorrenze[j] = '1';
						j = k;
						++somma;

						p = 0; 							// aggiungo info alla struttura errori
						esci = 0;
						while (esci == 0 && regole->lettere_gs[p] != '*') {
							if (regole->lettere_gs[p] == parola_1[i] && regole->pos_gs[p] == i) {
								esci = 1;
							} else {
							++p;
							}
						}
						if (esci == 0) {
							regole->lettere_gs[p] = parola_1[i];
							regole->pos_gs[p] = i;
							nuove_info = 1;
						}
					}
				}
			}
		}

		for (i = 0; i < k; ++i) {                  // occ esatte
			occorrenze[i] = '0';
		}

		for (i = 0; i < k; ++i) {
			
			if (confronto[i] == '/' && occorrenze[i] == '0') {
				
				somma = 0;
				for (j = 0; j < k; ++j) {
					if (occorrenze[j] == '0' && parola_1[j] == parola_1[i]) {
						if (confronto[j] != '/') {
							++somma;
						}
						occorrenze[j] = '1';
					}
				}
				if (somma > 0) {                // aggiungo info di dove non deve essere
					
					for (j = 0; j < k; j++) {
						if (parola_1[j] == parola_1[i] && confronto[j] == '/') {
							p = 0; 
							esci = 0;
							while (esci == 0 && regole->lettere_gs[p] != '*') {
								if (regole->lettere_gs[p] == parola_1[j] && regole->pos_gs[p] == j) {
									esci = 1;
								} else {
									++p;
								}
							}
							if (esci == 0) {
								regole->lettere_gs[p] = parola_1[j];
								regole->pos_gs[p] = j;
								nuove_info = 1;
							}
						}

					}
					j = 0;
					while (regole->lettere_occ[j] != parola_1[i] && regole->lettere_occ[j] != '*') { 
						++j;
					}
					if (regole->lettere_occ[j] == '*') {
						regole->lettere_occ[j] = parola_1[i];
						regole->occ_esatt[j] = somma;
						nuove_info = 1;
					} else if (regole->occ_esatt[j] != somma) {
						regole->occ_esatt[j] = somma;
						nuove_info = 1;
					}	

				} else {
					j = 0; 
					while (regole->no_lettere[j] != parola_1[i] && regole->no_lettere[j] != '*') {
						j++;
					}
					if (regole->no_lettere[j] == '*') {
						regole->no_lettere[j] = parola_1[i];
						nuove_info = 1;
					}
				}
			}
		}

		for (i = 0; i < k; ++i) {						// occ min
			if (occorrenze[i] == '0') {
				somma = 0;
				for (j = 0; j < k; ++j) {
					if (occorrenze[j] == '0' && parola_1[j] == parola_1[i]) {
						++somma;
						occorrenze[j] = '1';
					}
				}
				j = 0;
				while (regole->lettere_occ[j] != parola_1[i] && regole->lettere_occ[j] != '*') {
					++j;
				}
				if (regole->lettere_occ[j] == '*') {
					regole->lettere_occ[j] = parola_1[i];
				}
				if (regole->occ_esatt[j] == 0 && somma > regole->occ_min[j]) {
					regole->occ_min[j] = somma;
					nuove_info = 1;
				}
			}
		}

		confronto[k] = '\0';

		fprintf(stdout, "%s\n", confronto);
	
	}
	
	return uguali;

}

node * cerca(char * parola){

	node * nodo;
	int found, pos;

	found = 0;
	nodo = root;
	while (nodo != NULL && found == 0) {
		
		pos = 0;
		for (int i = 0; i < k && pos == 0; ++i) {
			if (parola[i] < nodo->parola[i]) {
				pos = 1;
			} else if (parola[i] > nodo->parola[i]) {
				pos = 2;
			}
		}

		if (pos == 0) {  // ho trovato la parola
			found = 1;
		} else if (pos == 1) {  // la parola viene prima
			nodo = nodo->sx;
		} else {
			nodo = nodo->dx;
		}
	}

	return nodo;

}

void inizializza(void) {

	
	memset(regole->lettere_gg, '*', k+1);
	memset(regole->lettere_gs, '*', k*k);
	memset(regole->no_lettere, '*', SIZE);
	memset(regole->lettere_occ, '*', k+1);

	memset(regole->occ_min, 0, (k+1)*4);
	memset(regole->occ_esatt, 0, (k+1)*4);

}

int filtro(char * parola) {


	int count, i, j, giusta, found;

	giusta = 1;

	for (i = 0; giusta == 1 && i < k && regole->lettere_gg[i] != '*'; ++i) { // giuste al posto giusto
		if (parola[regole->pos_gg[i]] != regole->lettere_gg[i]) {
			giusta = 0;
		}
		
	}

	for (i = 0; giusta == 1 && regole->lettere_gs[i] != '*' ; ++i) { // giuste al posto sbagliato
		if (parola[regole->pos_gs[i]] == regole->lettere_gs[i]) {
			giusta = 0;
		} else {
			found = 0;
			for (j = 0; found == 0 && j < k; ++j) {
				if (parola[j] == regole->lettere_gs[i]) {       // dovrei controllare se ho già guardato quella lettera
					found = 1;
				}
			}
			if (found == 0) {
				giusta = 0;
			}
		}
	}

	for (i = 0; giusta == 1 && i < SIZE && regole->no_lettere[i] != '*'; ++i) { // lettere che non devono esserci
		for (j = 0; giusta == 1 && j < k; ++j) {
			if (parola[j] == regole->no_lettere[i]) {
				giusta = 0;
			}
		} 
	}

	for (i = 0; giusta == 1 && i < k && regole->lettere_occ[i] != '*'; ++i) {  // conto occorrenze
		count = 0;
		if (regole->occ_esatt[i] != 0) {									// esatte
			for (j = 0; j < k && count <= regole->occ_esatt[i]; ++j) {  
				if (parola[j] == regole->lettere_occ[i]) {
					count++;
				}
			}
			if (count != regole->occ_esatt[i]) {
				giusta = 0;
			}
		} else {															// minime
			for (j = 0; j < k && count <= regole->occ_min[i]; ++j) {
				if (parola[j] == regole->lettere_occ[i]) {
					count++;
				}
			}
			if (count < regole->occ_min[i]) {
				giusta = 0;
			}
		}
	}

	return giusta;

}

int conta(void) {

	node * elem, * prev;
	int valide;

	valide = 0;

	if (first_valid != NULL) {

		while (first_valid != last_valid && filtro(first_valid->parola) == 0) {    // sposto first valide a destra fino al primo 1
			first_valid->fil = 0;
			elem = first_valid;
			first_valid = first_valid->succ_lista;
			elem->succ_lista = last_valid->succ_lista;
			last_valid->succ_lista = elem;
			if (elem->succ_lista == NULL) {
				last = elem;
			}
		}
		
		valide = 1;

		if (first_valid != last_valid) {

			prev = first_valid;
			elem = prev->succ_lista;

			while (elem != last_valid) {       // cicla finchè arrivo a last_valid e non lo controllo
				if (filtro(elem->parola) == 1) {
					valide += 1;
					prev = elem;
					elem = prev->succ_lista;
				} else {
					elem->fil = 0;
					prev->succ_lista = elem->succ_lista;
					elem->succ_lista = last_valid->succ_lista;
					last_valid->succ_lista = elem;
					if (elem->succ_lista == NULL) {
						last = elem;
					}
					elem = prev->succ_lista;
				}
			}

			if (elem == last_valid) {                   // controllo l'ultimo
				if (filtro(last_valid->parola) == 1) {
					valide += 1;        
				} else {
					last_valid->fil = 0;
					last_valid = prev;
				}
			}
		}
	}

	return valide;

}

void stampa_filtrate(node * curr) {

	if (curr != NULL) {
		if (curr->sx != NULL) {
			stampa_filtrate(curr->sx);
		}

		if (curr->fil == 1) {
			printf("%s\n", curr->parola);
		}

		if (curr->dx != NULL) {
			stampa_filtrate(curr->dx);
		}
	}

}

void inizializza_tree(void) {

	node * curr;

	curr = last_valid->succ_lista;
	while (curr != NULL) {
		curr->fil = 1;
		curr = curr->succ_lista;
	}

	last_valid = last;

}