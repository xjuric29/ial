
/* ******************************* c204.c *********************************** */
/*  Předmět: Algoritmy (IAL) - FIT VUT v Brně                                 */
/*  Úkol: c204 - Převod infixového výrazu na postfixový (s využitím c202)     */
/*  Referenční implementace: Petr Přikryl, listopad 1994                      */
/*  Přepis do jazyka C: Lukáš Maršík, prosinec 2012                           */
/*  Upravil: Kamil Jeřábek, říjen 2017                                        */
/* ************************************************************************** */
/*
** Implementujte proceduru pro převod infixového zápisu matematického
** výrazu do postfixového tvaru. Pro převod využijte zásobník (tStack),
** který byl implementován v rámci příkladu c202. Bez správného vyřešení
** příkladu c202 se o řešení tohoto příkladu nepokoušejte.
**
** Implementujte následující funkci:
**
**    infix2postfix .... konverzní funkce pro převod infixového výrazu 
**                       na postfixový
**
** Pro lepší přehlednost kódu implementujte následující pomocné funkce:
**    
**    untilLeftPar .... vyprázdnění zásobníku až po levou závorku
**    doOperation .... zpracování operátoru konvertovaného výrazu
**
** Své řešení účelně komentujte.
**
** Terminologická poznámka: Jazyk C nepoužívá pojem procedura.
** Proto zde používáme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako
** procedury (v jazyce C procedurám odpovídají funkce vracející typ void).
**
**/

#include "c204.h"

int solved;


/*
** Pomocná funkce untilLeftPar.
** Slouží k vyprázdnění zásobníku až po levou závorku, přičemž levá závorka
** bude také odstraněna. Pokud je zásobník prázdný, provádění funkce se ukončí.
**
** Operátory odstraňované ze zásobníku postupně vkládejte do výstupního pole
** znaků postExpr. Délka převedeného výrazu a též ukazatel na první volné
** místo, na které se má zapisovat, představuje parametr postLen.
**
** Aby se minimalizoval počet přístupů ke struktuře zásobníku, můžete zde
** nadeklarovat a používat pomocnou proměnnou typu char.
*/
void untilLeftPar ( tStack* s, char* postExpr, unsigned* postLen ) {
	
	char top;	

	if (s != NULL && postExpr != NULL && postLen != NULL && !stackEmpty (s)) { // Osetreni proti operaci s nenaalokovanymi parametry a prazdnym zasobnikem
		do {
			stackTop (s, &top);
			stackPop (s);
			postExpr[*postLen] = top;
			(*postLen)++;
		} while (top != '(' && !stackEmpty (s)); // Osetreni proti zacykleni kdyby ve stacku zavorka chybela - vyuzivam to i v pripade, ze mam vypsat veci ze stacku pred =
		if (postExpr[*postLen - 1] == '(') (*postLen)--; // Kvuli jednoduchosti cyklu nakonec pridam do stringu i zavorku, timto krokem ji zase odstranim
	}

}

/*
** Pomocná funkce doOperation.
** Zpracuje operátor, který je předán parametrem c po načtení znaku ze
** vstupního pole znaků.
**
** Dle priority předaného operátoru a případně priority operátoru na
** vrcholu zásobníku rozhodneme o dalším postupu. Délka převedeného 
** výrazu a taktéž ukazatel na první volné místo, do kterého se má zapisovat, 
** představuje parametr postLen, výstupním polem znaků je opět postExpr.
*/
void doOperation ( tStack* s, char c, char* postExpr, unsigned* postLen ) {
	
	char top;

	if (s != NULL && postExpr != NULL && postLen != NULL) {	// Osetreni proti operaci s nenaalokovanymi parametry
		if (!stackEmpty (s)) stackTop (s, &top); // Nejdrive nactu znak z vrcholu zasobniku pokud je, bohuzel tohle nejde preskocit, protoze funkce stackTop nevraci znak pomoci navratove hodnoty
		if (stackEmpty (s) || top == '(' || ((top == '+' || top == '-') && (c == '*' || c == '/'))) stackPush (s, c); // Ciste na zasobnik vlozim operator, pokud je prazdny, je pred nim l. zavorka nebo operator s nizsi prioritou
		else { // jinak operator ze zasobniku premistim do stringu a spustim fci znovu
			postExpr[*postLen] = top;
                        stackPop (s);
                        (*postLen)++;
			doOperation (s, c, postExpr, postLen);
		}
	}

}

/* 
** Konverzní funkce infix2postfix.
** Čte infixový výraz ze vstupního řetězce infExpr a generuje
** odpovídající postfixový výraz do výstupního řetězce (postup převodu
** hledejte v přednáškách nebo ve studijní opoře). Paměť pro výstupní řetězec
** (o velikosti MAX_LEN) je třeba alokovat. Volající funkce, tedy
** příjemce konvertovaného řetězce, zajistí korektní uvolnění zde alokované
** paměti.
**
** Tvar výrazu:
** 1. Výraz obsahuje operátory + - * / ve významu sčítání, odčítání,
**    násobení a dělení. Sčítání má stejnou prioritu jako odčítání,
**    násobení má stejnou prioritu jako dělení. Priorita násobení je
**    větší než priorita sčítání. Všechny operátory jsou binární
**    (neuvažujte unární mínus).
**
** 2. Hodnoty ve výrazu jsou reprezentovány jednoznakovými identifikátory
**    a číslicemi - 0..9, a..z, A..Z (velikost písmen se rozlišuje).
**
** 3. Ve výrazu může být použit předem neurčený počet dvojic kulatých
**    závorek. Uvažujte, že vstupní výraz je zapsán správně (neošetřujte
**    chybné zadání výrazu).
**
** 4. Každý korektně zapsaný výraz (infixový i postfixový) musí být uzavřen 
**    ukončovacím znakem '='.
**
** 5. Při stejné prioritě operátorů se výraz vyhodnocuje zleva doprava.
**
** Poznámky k implementaci
** -----------------------
** Jako zásobník použijte zásobník znaků tStack implementovaný v příkladu c202. 
** Pro práci se zásobníkem pak používejte výhradně operace z jeho rozhraní.
**
** Při implementaci využijte pomocné funkce untilLeftPar a doOperation.
**
** Řetězcem (infixového a postfixového výrazu) je zde myšleno pole znaků typu
** char, jenž je korektně ukončeno nulovým znakem dle zvyklostí jazyka C.
**
** Na vstupu očekávejte pouze korektně zapsané a ukončené výrazy. Jejich délka
** nepřesáhne MAX_LEN-1 (MAX_LEN i s nulovým znakem) a tedy i výsledný výraz
** by se měl vejít do alokovaného pole. Po alokaci dynamické paměti si vždycky
** ověřte, že se alokace skutečně zdrařila. V případě chyby alokace vraťte namísto
** řetězce konstantu NULL.
*/
char* infix2postfix (const char* infExpr) {

	unsigned infIndex = 0, postIndex = 0;
	char *postExpr = malloc (sizeof (char) * MAX_LEN);
	tStack stack; 

	if (infExpr == NULL || postExpr == NULL) return NULL; // Osetreni chyby alokace a zadani spatneo vstupniho argumentu
	stackInit (&stack);
	while (42) {
/* Zpracovava vstuni vyraz znak po znaku, dokud se neobjevi =, dale jsem zpracovani rozlozil do nasledujicich kroku:
	1. '(' uloz do stacku			3. pri vyskytu operatoru nech vzhodnotit fci doOperation
	2. pri ')' zavolej fci untilLeftPar	4. pri '=' zavolej znovu fci untiLeftPar, ktera je pro tento ucel upravena a pak rozbij cyklus
*/
		if (infExpr[infIndex] == '(') stackPush (&stack, '(');
		else if (infExpr[infIndex] == ')') untilLeftPar (&stack, postExpr, &postIndex);
		else if (infExpr[infIndex] == '+' || infExpr[infIndex] == '-' || infExpr[infIndex] == '*' || infExpr[infIndex] == '/') doOperation (&stack, infExpr[infIndex], postExpr, &postIndex);
		else if (infExpr[infIndex] == '=') {
			untilLeftPar (&stack, postExpr, &postIndex);
			break;
		}
		else {
			postExpr[postIndex] = infExpr[infIndex];
			postIndex++;
		}
		infIndex++;
	}
	postExpr[postIndex] = '=';
	postExpr[postIndex + 1] = '\0'; // Nakonec se doplni rovna se a konec stringu
	return postExpr;
}

/* Konec c204.c */
