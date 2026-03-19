char pppppp[] = "printf `sn` meow;";
#define compile 0

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TRUE 1
#define FALSE 0

#define smalestInt -214748364

#define byte unsigned char
#define bry ;break;

int licznikFromTo = 0;

byte leftCount = 0, rightCount = 0, ignoreOrNot = false;

char * codeFile;
int codeBytes = 1;

int * konce;
int konceAmount = 1;

typedef struct {int value; char * name;  } variable;
int amountOfVariables = 1;
variable * variables;

typedef struct {char * name; variable * list; int count;} matrix;
matrix * matrixs;
int matrixAmount = 1;

char line[1024], * args[64];
int argsCounter = 0;

typedef struct {int x; byte ifOrWhile; byte czyBylBrake; } pointOfWhile2;
pointOfWhile2 * pointsOfWhile;
int amountOfPoints = 1;

typedef struct {int lineOf; char * name;} defs;
defs * nyaDef;
int nyaDefAmount = 1;

typedef struct {int x; char * name; } dfjps;

dfjps * defJumps;
int defJumpsAmount = 1;

int pierwszy = 0;

#define keyWordsAmount 12
char * keyWords[] = {"STOP","var","printf","mat","readline","if","from","to","while","break","def","return"};

#define keyWordsGetAmount 4
char * keyWordsForGet[] = {"Lenght","Max","Min","len"};

char * nazwaZmiennejKtoramUstawiamy = "";

byte defOrNot = false;


variable returnedWartoscZwrotna;

byte ignorancjaDoReturn = false;

char *blad = "";

void iforlalj(byte x,int mniejCzyWiecej) {
    amountOfPoints+=mniejCzyWiecej;
    pointOfWhile2 * zzz = realloc(pointsOfWhile,sizeof(pointOfWhile2) * amountOfPoints);
    pointsOfWhile = zzz;
    if (mniejCzyWiecej == 1) {
        pointsOfWhile[amountOfPoints-1].x = pierwszy;
        pointsOfWhile[amountOfPoints-1].ifOrWhile = x;
        pointsOfWhile[amountOfPoints-1].czyBylBrake = false;
    }
}


int strlenA(char * x) {
    int i = 0;
    for (; x[i]; i++);
    return i;
}

int compareString(char * x, char * y) {
    if (x == NULL || y == NULL) {printf("NULL IN COMPARE\n"); return 0; }

    for (int i = 0; TRUE; i++) {
        if (x[i] != y[i]) return FALSE;
        else if (x[i] == 0) return TRUE;
    }
}

char * skanf() {
    char buffer[1024];
    fgets(buffer, 1024, stdin);
    for (int i = 0; i < 1024; i++) if (buffer[i] == '\n') {buffer[i] = 0; break;}

    for (int i = 1; i < matrixAmount; i++) if (compareString(matrixs[i].name,args[1] ) ) {
        int rozmiar = strlenA(buffer);
        if (matrixs[i].count < rozmiar ) return "Buffer is to small.";
        for (int j = 0; j < rozmiar; j++) {
            matrixs[i].list[j].value = buffer[j];
        }
    }
    return "";
}

int lenghtOf(char * x) {
    for (int i = 1; i < matrixAmount; i++) if (compareString(x, matrixs[i].name )) {
        return matrixs[i].count;
    }
    return 0;
}

int maxMinValue(char * x, char z) {
    for (int i = 1; i < matrixAmount; i++) if (compareString(x, matrixs[i].name )) {
        int max = smalestInt * z;
        for (int j = 0; j < matrixs[i].count; j++) {
            if ((matrixs[i].list[j].value > max && z == 1) || (matrixs[i].list[j].value < max && z == -1)    ) max = matrixs[i].list[j].value;
        }
        return max;
    }
    return 0;
}

void backSpace(char * x) {
    if (x[0] != ' ' && x[0] != '\n' && x[0] != '#' || x[0] == 9 ) return;
    int doConfiecia = 0;

    byte comment = FALSE;
    while (x[doConfiecia] == ' ' || x[doConfiecia] == '\n' || x[doConfiecia] == 9  || x[doConfiecia] == '#' || comment) {
        if (x[doConfiecia] == '#') comment = TRUE;
        else if (x[doConfiecia] == '\n' ) comment = FALSE;
        doConfiecia++;
    }
    for (int i = 0; x[i+doConfiecia-1]; i++) {
        x[i] = x[i+doConfiecia];
    }
}
void fromNya() {
    iforlalj(0,1);
    leftCount++;
    licznikFromTo++;
}

char * defUwUNya(char * x[],int argsCounterLocal) {
    bool czyBylJuz = false;
    if (argsCounterLocal >= 3) {
        if (compareString("new",x[2])) {
            czyBylJuz = true;
            ignorancjaDoReturn = true;
            defOrNot = true;
            if (licznikFromTo) return "Def must be created in void.";
            ignoreOrNot = true;
            fromNya();

            nyaDefAmount++;
            defs * tmps = realloc(nyaDef, sizeof(defs) * nyaDefAmount );
            nyaDef = tmps;

            nyaDef[nyaDefAmount-1].lineOf = pierwszy;
            nyaDef[nyaDefAmount-1].name = strdup(x[1]);
        }

    }
    if (!czyBylJuz) for (int i = 1; i < nyaDefAmount; i++) if (compareString(nyaDef[i].name, x[1] )) {
        defJumpsAmount++;
        dfjps * tmps = realloc(defJumps, sizeof(dfjps) * defJumpsAmount );
        defJumps = tmps;
        defJumps[defJumpsAmount - 1].x = pierwszy;
        defJumps[defJumpsAmount - 1].name = "";

        if (argsCounterLocal == 4) if (compareString("to",x[2])) defJumps[defJumpsAmount - 1].name = strdup(x[3]);

        pierwszy = nyaDef[i].lineOf;

        break;
    }

    return "";
}

int stringToValue(char * x) {
    char nameOfString[100];

    int pierwszyStringa = -1;
    int ostatni = -1;

    int pierwszyNawiasu = -1;
    int ostatniNawiasu = -1;

    int iloscLewych = 0;
    int iloscPrawych = 0;

    int endValue = 0;
    int oneValue = 0;
    char operrant = '+';
    int len = strlenA(x);
    for (int i = 0; i <= len; i++) {
        if ((x[i] == '+' || x[i] == '-' || x[i] == '*' || x[i] == '/' || x[i] == 0 || x[i] == '`') && (pierwszyNawiasu == -1 || ostatniNawiasu != -1) && iloscLewych == iloscPrawych ) {
            if (pierwszyNawiasu != -1) {
                char buf[ostatniNawiasu-pierwszyNawiasu+4];
                for (int j = pierwszyNawiasu; j < ostatniNawiasu; j++) buf[j-pierwszyNawiasu] = x[j];
                buf[ostatniNawiasu - pierwszyNawiasu] = 0;
                oneValue = stringToValue(buf);
            }
            if (pierwszyStringa != -1) {
                for (int j = pierwszyStringa; j < ostatni; j++) nameOfString[j-pierwszyStringa] = x[j];
                nameOfString[ostatni-pierwszyStringa] = 0;
                char * liczbaNya;
                char * afterDot;
                byte czyTablica = FALSE;
                byte czyInnyParametr = FALSE;

                for (int j = 0; j < ostatni-pierwszyStringa; j++) {
                    if (nameOfString[j] == '|') {nameOfString[j] = 0; czyTablica = TRUE; liczbaNya = &nameOfString[j+1] bry }
                    else if (nameOfString[j] == '.') {nameOfString[j] = 0; czyInnyParametr = TRUE; afterDot = &nameOfString[j+1] bry   }
                }

                int strlenNameOfString = strlenA(nameOfString);
                if (nameOfString[0] == '"' && nameOfString[strlenNameOfString-1] == '"') {
                    for (int n = 1; n < strlenNameOfString-1; n++) oneValue+= nameOfString[n];
                }
                else if (czyInnyParametr) {
                    int codeOf = -1;
                    for (int j = 0; j < keyWordsGetAmount; j++) if (compareString(afterDot, keyWordsForGet[j] )) {codeOf = j bry}
                    if (codeOf == -1) blad = "Doesn't existing method.";

                    int o = 0;

                    switch (codeOf) {
                        case 0: o = lenghtOf(nameOfString) bry
                        case 1: o = maxMinValue(nameOfString, 1) bry
                        case 2: o = maxMinValue(nameOfString,-1) bry
                    }
                    oneValue = o;
                }
                else if (czyTablica) {
                    int b = stringToValue(liczbaNya);
                    byte czyJestTablica = FALSE;
                    for (int j = 1; j < matrixAmount; j++) if (compareString(matrixs[j].name, nameOfString)) {oneValue=matrixs[j].list[b].value; czyJestTablica = TRUE bry}
                    if (!czyJestTablica) blad = "Doesn't existing var name";
                }
                else for (int j = 1; j < amountOfVariables; j++) if (compareString(variables[j].name, nameOfString)) {oneValue=variables[j].value bry}

            }
            if (oneValue) {
                if (operrant == '+') endValue+=oneValue;
                else if (operrant == '-') endValue-=oneValue;
                else if (operrant == '*') endValue*=oneValue;
                else if (operrant == '/') endValue/=oneValue;
            }
            if (x[i] != '`')  operrant = x[i];

            pierwszyStringa = -1;

            pierwszyNawiasu = -1;
            ostatniNawiasu = -1;

            oneValue = 0;
        }
        else if (x[i] == '(' || x[i] == ')' ) {
            if (x[i] == '(') iloscLewych++;
            else iloscPrawych++;

            if (pierwszyNawiasu == -1) pierwszyNawiasu = i+1;
            else ostatniNawiasu = i;
        }
        else if (x[i] > 47 && x[i] < 58 && pierwszyStringa == -1 && pierwszyNawiasu == -1) {
            oneValue = oneValue * 10 + (x[i] - 48);
        }
        else if (x[i] != ' ' && pierwszyNawiasu == -1) {
            if (pierwszyStringa == -1) pierwszyStringa = i;
            ostatni = i+1;
        }
    }
    return endValue;
}

char *  napisz() {
    int zupa = strlenA(args[1]);
    int nextArg = 2;
    for (int i = 1; i < zupa-1; i++) {
        switch (args[1][i]) {
            case 's': printf("%s",args[i+1]); nextArg++ bry
            case 'd': printf("%d",stringToValue(args[nextArg++])) bry
            case 'c': printf("%c",stringToValue(args[nextArg++])) bry
            case 'n': printf("\n") bry
            case ' ': printf(" ") bry
        }
    }
    fflush(stdout);
    return "";
}

char *  newVariable() {

    int val = -1;
    for (int i = 1; i < amountOfVariables; i++) if (compareString(args[1], variables[i].name )) val = i;
    byte czyJestNew = compareString(args[argsCounter-1],"new");

    for (int i = 1; i < matrixAmount; i++) if (compareString(matrixs[i].name, args[1] )) {
        return "Matrix with this name arleady exist";
    }


    if (czyJestNew && val == -1) {
        val = amountOfVariables;
        variable tmp = {0,strdup(args[1])};
        amountOfVariables++;
        variable * tmpVars = realloc(variables, amountOfVariables * sizeof(variable));
        tmpVars[amountOfVariables-1] = tmp;
        variables = tmpVars;
    }
    else if (!czyJestNew && val == -1 ) return "Selected variable doesn't exist.";
    else if (czyJestNew && val != -1) return "This variable exist,";

    if (compareString(args[2],"=")) variables[val].value = stringToValue(args[3]);
    return "";
}


char * matrixCreating() {
    int lenghtOfMat = strlenA(args[1]);
    char nameOfMatVar[100];
    char * liczba = NULL;
    int codeOfMat = -1;

    for (int i = 0; i <= lenghtOfMat; i++) {
        nameOfMatVar[i] = args[1][i];
        if (args[1][i] == '|') {
            nameOfMatVar[i] = 0;
            liczba = &args[1][i+1];
        }
    }
    if (liczba != NULL) codeOfMat = stringToValue(liczba);

    int code = matrixAmount;
    for (int i = 1; i < matrixAmount; i++) if (compareString(matrixs[i].name, nameOfMatVar )) code = i ;


    for (int i = 1; i < amountOfVariables; i++) if (compareString(variables[i].name, nameOfMatVar)) return "Variable with this name arleady exist";
    byte czyJestNew = compareString(args[argsCounter-1], "new");
    byte czyAppend = compareString(args[argsCounter-1], "add");
    if (czyAppend && code < matrixAmount) {
        matrixs[code].count++;
        variable * fortnite = realloc(matrixs[code].list, sizeof(variable) * matrixs[code].count );
        fortnite[ matrixs[code].count-1 ].value = stringToValue(args[2]);
        matrixs[code].list = fortnite;

    }
    else if (czyJestNew && code == matrixAmount) {
        int rozmiar = stringToValue(args[2]);
        matrix new = {strdup(args[1]),malloc(sizeof(variable) * rozmiar) ,rozmiar};

        variable uwu = {0,""};
        for (int i = 0; i < rozmiar; i++) new.list[i] = uwu;

        matrixAmount++;
        matrix * tmp = realloc(matrixs, sizeof(matrix) * matrixAmount);
        tmp[matrixAmount-1] = new;
        matrixs = tmp;
    }
    else if (!czyJestNew && code != matrixAmount) {
        if (codeOfMat == -1) {
            int wartosc = stringToValue(args[2]);
            matrixs[code].list[wartosc].value = stringToValue(args[4]);
        }
        else {
            int wartosc = codeOfMat;
            matrixs[code].list[wartosc].value = stringToValue(args[3]);
        }
    }
    else return "Variable doesn't exist or `new` isn't used";
    return "";
}

char * jezeli() {
    byte result = false;
    int one = stringToValue(args[1]);
    if (argsCounter != 2) {
        int two = stringToValue(args[3]);

        char * op = args[2];

        if (argsCounter < 4) result = one;
        else if (compareString(op, "==")) result = (one == two);
        else if (compareString(op, "!=")) result = (one != two);
        else if (compareString(op, ">"))  result = (one > two);
        else if (compareString(op, "<"))  result = (one < two);
        else if (compareString(op, ">=")) result = (one >= two);
        else if (compareString(op, "<=")) result = (one <= two);
        else return "error, ";
    }
    else result = one;



    ignoreOrNot = !result;
    leftCount = 0;
    rightCount = 0;

    fromNya();


    return "";
}

char * pentlaWhile() {
    iforlalj(1,1);

    leftCount = 0;
    rightCount = 0;
    return "";
}


void rightNya() {
    rightCount++;
    licznikFromTo--;

    if (amountOfPoints > 0) {
        ignoreOrNot = false;
        if (pointsOfWhile[amountOfPoints-1].ifOrWhile) {
            if (!pointsOfWhile[amountOfPoints-1].czyBylBrake) pierwszy = pointsOfWhile[amountOfPoints-1].x;
            else iforlalj(0,-1);
        }
        else iforlalj(0,-1);
    }
    else {}
}

char * brejk() {
    for (int i = amountOfPoints-1; i >= 0; i--) {
        if (pointsOfWhile[i].ifOrWhile) {
            pointsOfWhile[i].czyBylBrake = true;
            break;
        }
    }
    return "";
}


char * powrot() {
    ignoreOrNot = false;
    licznikFromTo--;
    if (defOrNot) {
        defOrNot = false;
        return "";
    }
    pierwszy = defJumps[defJumpsAmount - 1].x;

    if (defJumps[defJumpsAmount - 1].name[0]) {
        for (int i = 1; i < amountOfVariables; i++) if (compareString(defJumps[defJumpsAmount - 1].name, variables[i].name)) {
            int z = stringToValue(args[1]);
            variables[i].value = z;
            break;
        }
        free(defJumps[defJumpsAmount - 1].name);
    }


    defJumpsAmount--;
    dfjps * tmps = realloc(defJumps, sizeof(dfjps) * defJumpsAmount );
    defJumps = tmps;

    rightNya();

    if (argsCounter > 1) returnedWartoscZwrotna.value = stringToValue(args[1]);
    else returnedWartoscZwrotna.value = 0;

    return "";
}


int main(int argc, char *argv[])
{
    nyaDef = malloc(sizeof(defs));
    variables = malloc(sizeof(variable));
    matrixs = malloc(sizeof(matrix));
    pointsOfWhile = malloc(sizeof(pointOfWhile2));
    defJumps = malloc(sizeof(dfjps));
    int fileSize = 0;
    if (!compile) {
        FILE *f;
        byte z;
        if (argc > 1) f = fopen(argv[1], "r");
        else f = fopen("main.nya", "r");

            fseek(f, 0, SEEK_END);
            fileSize = ftell(f);
            codeFile = malloc(fileSize);
            for (int i = 0; i < fileSize; i++) {
                fseek(f, i, SEEK_SET);
                codeFile[i] = fgetc(f);
            }
        fclose(f);
    }
    else {
        fileSize = sizeof(pppppp);
        codeFile = malloc(fileSize);
        for (int i = 0; i < fileSize; i++) codeFile[i] = pppppp[i];
    }

    for (int i = 0; i < fileSize; i++) if (codeFile[i] == ';' || codeFile[i] == ':' ) konceAmount++;
    konce = malloc(sizeof(int) * konceAmount+2);
    int licznikKoncow = 1;
    for (int i = 0; i < fileSize; i++) if (codeFile[i] == ';' || codeFile[i] == ':'  ) {konce[licznikKoncow] = i; licznikKoncow++;}
    konce[0] = -1;

    pierwszy = 0;
    //konceAmount = 0;

    while (TRUE) {
        int i;
        for (i = konce[pierwszy]+1; i < konce[pierwszy+1]; i++) line[i-(konce[pierwszy]+1)] = codeFile[i];
        line[i-(konce[pierwszy]+1)] = 0;
        backSpace(line);

        byte nothing = FALSE;
        if (strlen(line) > 1) if (line[0] == '/' && line[1] == '/' ) nothing = TRUE;

        if (!nothing){
            for (i=1;i<50;i++)args[i]=NULL;
            args[0] = &line[0];

            argsCounter = 1;
            int lenLine = strlenA(line);
            byte userMode = FALSE;
            for (i = 0; i <= lenLine; i++) {
                if (line[i] == '`') userMode = !userMode;
                else if ((line[i] == ' ' || line[i] == '\n')  && !userMode) line[i] = 0;
            }
            for (i = 1; i < lenLine; i++) if (line[i-1] == 0 && line[i] != 0 ) args[argsCounter++] = &line[i];
            args[argsCounter] = NULL;


            //wywoływanie funkcji
            int syscallCode = -1;
            for (i = 0; i < keyWordsAmount; i++) if (compareString(args[0],keyWords[i])) syscallCode = i;

            char * e = "";

            if (syscallCode == 6) fromNya();
            else if (syscallCode == 7) rightNya();
            else if (syscallCode == 11) e = powrot();
            else if (!ignoreOrNot && !defOrNot) {
                switch (syscallCode) {
                    case 0: e = "program was correctly stoped" bry
                    case 1: e = newVariable() bry
                    case 2: e = napisz() bry
                    case 3: e = matrixCreating() bry
                    case 4: e = skanf() bry
                    case 5: e = jezeli() bry
                    case 8: e = pentlaWhile() bry
                    case 9: e = brejk() bry
                    case 10: e = defUwUNya(args,argsCounter) bry
                }
            }

            if (e[0] || blad[0]) {
                if (blad[0])  printf("%s, line %d\n",blad,pierwszy+1);
                else printf("%s, line %d\n",e,pierwszy+1);
                return 0;
            }
        }

        //nie dodakaj
        pierwszy++;
        if (pierwszy == licznikKoncow-1) break;
    }


    if (codeFile != NULL) free(codeFile);
    if (konce != NULL) free(konce);

    for (int i = 0; i < amountOfVariables; i++) free(variables[i].name);
    for (int i = 0; i < matrixAmount; i++) free(matrixs[i].name);

    free(variables);
    free(matrixs);
    free(pointsOfWhile);
    free(defJumps);
    free(nyaDef);
}