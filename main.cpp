#include <iostream>
#include <fstream>
#include <cstring>
using namespace std;
ifstream fin("robots.in");
ofstream fout("robots.out");

int **matrice,Lungime_Stack = 20,Pozitie_Stack = 0;
int *nrBoxes;
struct deque{
char CommandType[11];
int Line,Column,Boxes,ID,Timee;
deque *next,*back;
}**roboti;

struct last_executed_command{
char CommandType[11];
int RobotId,Line,Column,Boxes;
int ExecuteOrAdd,Timee;
}*LastExecuted;

void RESIZE(last_executed_command* &a){
    last_executed_command *b;
    Lungime_Stack *= 2;
    b = new last_executed_command[Lungime_Stack]();
    for(int i = 0 ; i < Lungime_Stack/2 ; i++)
    b[i] = a[i];
    delete[] a;
    a = b;
}

void add_que(deque *&p,int newID,char newCommandType[5],int newLine,int newColumn,int newBoxes,bool priority){
    deque *q;
    if(p == NULL){
        p = new deque;
        p -> ID = newID;
        p -> next = NULL;
        p -> back = NULL;
        p -> Timee = 1;
        strcpy(p -> CommandType , newCommandType);
        p -> Boxes = newBoxes;
        p -> Line = newLine;
        p -> Column = newColumn;
    }
    else if(priority == 0){
            p -> back = new deque;
            p -> back -> back = NULL;
            p -> back -> next = p;
            p = p -> back;
            p -> Timee = 1;
            strcpy(p -> CommandType , newCommandType);
            p -> Boxes = newBoxes;
            p -> Line = newLine;
            p -> Column = newColumn;
            p -> ID = newID;
         }
    else if(priority == 1){
            q = p;
            while(q -> next != NULL)
            q = q -> next;
            q -> next = new deque;
            q -> next -> back = q;
            q -> next -> next = NULL;
            q = q -> next;
            q-> Timee = 1;
            strcpy(q -> CommandType , newCommandType);
            q -> Boxes = newBoxes;
            q -> Line = newLine;
            q -> Column = newColumn;
         }
    if(Pozitie_Stack < Lungime_Stack){
        LastExecuted[Pozitie_Stack].Boxes = newBoxes;
        LastExecuted[Pozitie_Stack].Line = newLine;
        LastExecuted[Pozitie_Stack].Column = newColumn;
        LastExecuted[Pozitie_Stack].RobotId = newID;
        strcpy(LastExecuted[Pozitie_Stack].CommandType , newCommandType);
        LastExecuted[Pozitie_Stack].ExecuteOrAdd = 0;
        Pozitie_Stack++;
    }
    else if(Pozitie_Stack == Lungime_Stack){
            RESIZE(LastExecuted);
            LastExecuted[Pozitie_Stack].Boxes = newBoxes;
            LastExecuted[Pozitie_Stack].Line = newLine;
            LastExecuted[Pozitie_Stack].Column = newColumn;
            LastExecuted[Pozitie_Stack].RobotId = newID;
            strcpy(LastExecuted[Pozitie_Stack].CommandType , newCommandType);
            LastExecuted[Pozitie_Stack].ExecuteOrAdd = 0;
            Pozitie_Stack++;
         }
}

void HOW_MUCH_TIME(){
    int x = Pozitie_Stack-2;
    if(LastExecuted[Pozitie_Stack-1].ExecuteOrAdd==1){
        fout<<LastExecuted[Pozitie_Stack-1].Timee<<'\n';
    }
    else{
        while(x >= 0){
            if(LastExecuted[x].ExecuteOrAdd == 1){
                fout<<LastExecuted[x].Timee<<'\n';
                break;
            }
            x--;
        }
    if(x == -1)
    fout<<"No command was executed\n";
    }
}

void add_que1(deque *&p,int newID,char newCommandType[5],int newLine,int newColumn,int newBoxes,bool priority){
    deque *q;
    if(p == NULL){
        p = new deque;
        p -> ID = newID;
        p -> next = NULL;
        p -> back = NULL;
        p -> Timee = 1;
        strcpy(p -> CommandType , newCommandType);
        p -> Boxes = newBoxes;
        p -> Line = newLine;
        p -> Column = newColumn;
    }
    else if(priority == 0){
            p -> back = new deque;
            p -> back -> back = NULL;
            p -> back -> next = p;
            p = p -> back;
            p -> Timee = 1;
            strcpy(p -> CommandType , newCommandType);
            p -> Boxes = newBoxes;
            p -> Line = newLine;
            p -> Column = newColumn;
            p -> ID = newID;
         }
    else if(priority == 1){
            q = p;
            while(q -> next != NULL)
            q = q -> next;
            q -> next = new deque;
            q -> next -> back = q;
            q -> next -> next = NULL;
            q = q -> next;
            q-> Timee = 1;
            strcpy(q -> CommandType , newCommandType);
            q -> Boxes = newBoxes;
            q -> Line = newLine;
            q -> Column = newColumn;
         }
}

void UNDO(){
    if(Pozitie_Stack == 0)
        fout<<"UNDO: No History\n";
    else if(Pozitie_Stack >= 1 && LastExecuted[Pozitie_Stack-1].ExecuteOrAdd == 0){
        deque *q = roboti[LastExecuted[Pozitie_Stack-1].RobotId];
        if(q!=NULL && q -> Boxes == LastExecuted[Pozitie_Stack-1].Boxes && q -> Line == LastExecuted[Pozitie_Stack-1].Line && q -> Column == LastExecuted[Pozitie_Stack-1].Column && strcmp(q -> CommandType , LastExecuted[Pozitie_Stack-1].CommandType) == 0){
            if(q -> next != NULL){
                roboti[LastExecuted[Pozitie_Stack-1].RobotId] = roboti[LastExecuted[Pozitie_Stack-1].RobotId] -> next;
                roboti[LastExecuted[Pozitie_Stack-1].RobotId] -> back = NULL;
                delete q;
            }
            else if(q -> next == NULL){
                delete q;
                roboti[LastExecuted[Pozitie_Stack-1].RobotId] = NULL;
            }
        }
        else{
            if(q != NULL && q -> next != NULL && q -> next -> next != NULL){
                while(q -> next -> next != NULL)
                    q = q -> next;
                deque *q1 = q -> next;
                q -> next = NULL;
                delete q1;
            }
            else if(q != NULL && q -> next != NULL && q -> next -> next == NULL){
                    deque *q1 = q -> next;
                    q -> next = NULL;
                    delete q1;
                 }

        }
    }
    else if(Pozitie_Stack >= 1 && LastExecuted[Pozitie_Stack-1].ExecuteOrAdd == 1){
        if(strcmp(LastExecuted[Pozitie_Stack-1].CommandType , "GET") == 0){
            matrice[LastExecuted[Pozitie_Stack-1].Line][LastExecuted[Pozitie_Stack-1].Column] += LastExecuted[Pozitie_Stack-1].Boxes;
            nrBoxes[LastExecuted[Pozitie_Stack-1].RobotId] -= LastExecuted[Pozitie_Stack-1].Boxes;
        }
        else if(strcmp(LastExecuted[Pozitie_Stack-1].CommandType , "DROP") == 0){
                matrice[LastExecuted[Pozitie_Stack-1].Line][LastExecuted[Pozitie_Stack-1].Column] -= LastExecuted[Pozitie_Stack-1].Boxes;
                nrBoxes[LastExecuted[Pozitie_Stack-1].RobotId] += LastExecuted[Pozitie_Stack-1].Boxes;
             }
        add_que1(roboti[LastExecuted[Pozitie_Stack-1].RobotId],LastExecuted[Pozitie_Stack-1].RobotId,LastExecuted[Pozitie_Stack-1].CommandType,LastExecuted[Pozitie_Stack-1].Line,LastExecuted[Pozitie_Stack-1].Column,LastExecuted[Pozitie_Stack-1].Boxes,0);
        }
    if(Pozitie_Stack >= 1){
        Pozitie_Stack--;
        LastExecuted[Pozitie_Stack].Boxes = 0;
        LastExecuted[Pozitie_Stack].Column = 0;
        LastExecuted[Pozitie_Stack].Line = 0;
        LastExecuted[Pozitie_Stack].ExecuteOrAdd = 2;
        LastExecuted[Pozitie_Stack].RobotId = 0;
        strcpy(LastExecuted[Pozitie_Stack].CommandType , "");
    }

}

void EXECUTE(deque *&p){
    deque *q = p;
    if(q == NULL)
        fout << "EXECUTE: No command to execute\n";
    if(q != NULL && strcmp(q -> CommandType , "GET") == 0){
            if(matrice[q -> Line][q -> Column] >= q -> Boxes){
                matrice[q -> Line][q -> Column] -= q -> Boxes;
                nrBoxes[q -> ID] += q -> Boxes;
            if(Pozitie_Stack < Lungime_Stack){
                LastExecuted[Pozitie_Stack].Boxes = q -> Boxes;
                LastExecuted[Pozitie_Stack].Line = q -> Line;
                LastExecuted[Pozitie_Stack].Column = q -> Column;
                LastExecuted[Pozitie_Stack].RobotId = q -> ID;
                LastExecuted[Pozitie_Stack].Timee = q -> Timee;
                strcpy(LastExecuted[Pozitie_Stack].CommandType , q -> CommandType);
                LastExecuted[Pozitie_Stack].ExecuteOrAdd = 1;
                Pozitie_Stack++;
            }
            else if(Pozitie_Stack == Lungime_Stack){
                    RESIZE(LastExecuted);
                    LastExecuted[Pozitie_Stack].Boxes = q -> Boxes;
                    LastExecuted[Pozitie_Stack].Line = q -> Line;
                    LastExecuted[Pozitie_Stack].Column = q -> Column;
                    LastExecuted[Pozitie_Stack].RobotId = q -> ID;
                    LastExecuted[Pozitie_Stack].Timee = q -> Timee;
                    strcpy(LastExecuted[Pozitie_Stack].CommandType , q -> CommandType);
                    LastExecuted[Pozitie_Stack].ExecuteOrAdd = 1;
                    Pozitie_Stack++;
                }
            }
            else if(matrice[q -> Line][q -> Column] < q-> Boxes){
                    nrBoxes[q -> ID] += matrice[q -> Line][q -> Column];

                    if(Pozitie_Stack < Lungime_Stack){
                    LastExecuted[Pozitie_Stack].Boxes = matrice[q -> Line][q -> Column];
                    LastExecuted[Pozitie_Stack].Line = q -> Line;
                    LastExecuted[Pozitie_Stack].Column = q -> Column;
                    LastExecuted[Pozitie_Stack].RobotId = q -> ID;
                    LastExecuted[Pozitie_Stack].Timee = q -> Timee;
                    strcpy(LastExecuted[Pozitie_Stack].CommandType , q -> CommandType);
                    LastExecuted[Pozitie_Stack].ExecuteOrAdd = 1;
                    Pozitie_Stack++;
                 }
                 else if(Pozitie_Stack == Lungime_Stack){
                        RESIZE(LastExecuted);
                        LastExecuted[Pozitie_Stack].Boxes = matrice[q -> Line][q -> Column];
                        LastExecuted[Pozitie_Stack].Line = q -> Line;
                        LastExecuted[Pozitie_Stack].Column = q -> Column;
                        LastExecuted[Pozitie_Stack].RobotId = q -> ID;
                        LastExecuted[Pozitie_Stack].Timee = q -> Timee;
                        strcpy(LastExecuted[Pozitie_Stack].CommandType , q -> CommandType);
                        LastExecuted[Pozitie_Stack].ExecuteOrAdd = 1;
                        Pozitie_Stack++;
                     }
                    matrice[q -> Line][q -> Column] = 0;
            }
      }
      if(q != NULL && strcmp(q -> CommandType , "DROP") == 0){
            if(nrBoxes[q -> ID] >= q -> Boxes){
                matrice[q -> Line][q -> Column] += q -> Boxes;
                nrBoxes[q -> ID] -= q -> Boxes;
            if(Pozitie_Stack < Lungime_Stack){
                LastExecuted[Pozitie_Stack].Boxes = q -> Boxes;
                LastExecuted[Pozitie_Stack].Line = q -> Line;
                LastExecuted[Pozitie_Stack].Column = q -> Column;
                LastExecuted[Pozitie_Stack].RobotId = q -> ID;
                LastExecuted[Pozitie_Stack].Timee = q -> Timee;
                strcpy(LastExecuted[Pozitie_Stack].CommandType , q -> CommandType);
                LastExecuted[Pozitie_Stack].ExecuteOrAdd = 1;
                Pozitie_Stack++;
            }
            else if(Pozitie_Stack == Lungime_Stack){
                    RESIZE(LastExecuted);
                    LastExecuted[Pozitie_Stack].Boxes = q -> Boxes;
                    LastExecuted[Pozitie_Stack].Line = q -> Line;
                    LastExecuted[Pozitie_Stack].Column = q -> Column;
                    LastExecuted[Pozitie_Stack].RobotId = q -> ID;
                    LastExecuted[Pozitie_Stack].Timee = q -> Timee;
                    strcpy(LastExecuted[Pozitie_Stack].CommandType , q -> CommandType);
                    LastExecuted[Pozitie_Stack].ExecuteOrAdd = 1;
                    Pozitie_Stack++;
                }
            }
            else if(nrBoxes[q -> ID] < q-> Boxes){
                    matrice[q -> Line][q -> Column] += nrBoxes[q -> ID];
                 if(Pozitie_Stack < Lungime_Stack){
                    LastExecuted[Pozitie_Stack].Boxes = nrBoxes[q -> ID];
                    LastExecuted[Pozitie_Stack].Line = q -> Line;
                    LastExecuted[Pozitie_Stack].Column = q -> Column;
                    LastExecuted[Pozitie_Stack].RobotId = q -> ID;
                    LastExecuted[Pozitie_Stack].Timee = q -> Timee;
                    strcpy(LastExecuted[Pozitie_Stack].CommandType , q -> CommandType);
                    LastExecuted[Pozitie_Stack].ExecuteOrAdd = 1;
                    Pozitie_Stack++;
                 }
                 else if(Pozitie_Stack == Lungime_Stack){
                        RESIZE(LastExecuted);
                        LastExecuted[Pozitie_Stack].Boxes = nrBoxes[q -> ID];
                        LastExecuted[Pozitie_Stack].Line = q -> Line;
                        LastExecuted[Pozitie_Stack].Column = q -> Column;
                        LastExecuted[Pozitie_Stack].RobotId = q -> ID;
                        LastExecuted[Pozitie_Stack].Timee = q -> Timee;
                        strcpy(LastExecuted[Pozitie_Stack].CommandType , q -> CommandType);
                        LastExecuted[Pozitie_Stack].ExecuteOrAdd = 1;
                        Pozitie_Stack++;
                     }
                nrBoxes[q -> ID] = 0;
            }
      }
   if(q != NULL && q -> next != NULL){
        q -> next -> ID = q -> ID;
        p = p -> next;
        p -> back = NULL;
        delete q;
   }
   else if(q != NULL && q -> next == NULL){
            delete q;
            p=NULL;
        }


}

void PRINT_COMMANDS(deque *&p){
    if(p == NULL)
        fout<<"No command found\n";
    else{
        fout<<p -> ID<<":"<<' ';
        deque *q=p;
        while(q!=NULL){
            fout<<q -> CommandType<<' '<<q -> Line<<' '<<q -> Column<<' '<<q -> Boxes;
            if(q -> next != NULL)
                fout<<"; ";
            q = q -> next;
        }
        fout<<'\n';
    }
}

void HOW_MANY_BOXES(int ID){
    fout<<nrBoxes[ID]<<'\n';
}

int main()
{
    int numarul_de_roboti,lines,columns,x,Linie,Coloana,IdRobot,Cutii,Priorietate;
    char comanda[50];
    fin >> numarul_de_roboti;
    roboti = new deque*[numarul_de_roboti]();
    fin >> lines >> columns;
    matrice = new int*[lines]();
    for(int i = 0 ; i < lines ; i++)
        matrice[i] = new int[columns]();
    for(int i = 0 ; i < lines ; i++)
        for(int j = 0 ; j < columns ; j++){
            fin >> x;
            matrice[i][j] = x;
        }
    nrBoxes= new int[numarul_de_roboti]();
    LastExecuted=new last_executed_command[Lungime_Stack]();
    while(fin>>comanda){
        if(strcmp(comanda , "ADD_GET_BOX") == 0){
            for(int i = 0 ; i < numarul_de_roboti ; i++){
                deque *q = roboti[i];
                while(q!=NULL){
                    q -> Timee++;
                    q = q -> next;

                }
            }
            fin>>IdRobot>>Linie>>Coloana>>Cutii>>Priorietate;
            add_que(roboti[IdRobot],IdRobot,"GET",Linie,Coloana,Cutii,Priorietate);
        }
        else if(strcmp(comanda , "ADD_DROP_BOX") == 0){
              for(int i = 0 ; i < numarul_de_roboti ; i++){
                  deque *q = roboti[i];
                  while(q!=NULL){
                    q -> Timee++;
                    q = q -> next;
                  }
               }
                fin>>IdRobot>>Linie>>Coloana>>Cutii>>Priorietate;
                add_que(roboti[IdRobot],IdRobot,"DROP",Linie,Coloana,Cutii,Priorietate);
            }
        else if(strcmp(comanda , "EXECUTE") == 0){
                fin>>IdRobot;
                EXECUTE(roboti[IdRobot]);
                for(int i = 0 ; i < numarul_de_roboti ; i++){
                    deque *q = roboti[i];
                    while(q!=NULL){
                        q -> Timee++;
                        q = q -> next;

                    }
                }
            }
        else if(strcmp(comanda , "PRINT_COMMANDS") == 0){
                fout<<"PRINT_COMMANDS: ";
                for(int i = 0 ; i < numarul_de_roboti ; i++){
                    deque *q = roboti[i];
                    while(q!=NULL){
                        q -> Timee++;
                        q = q -> next;

                    }
                }
                fin>>IdRobot;
                PRINT_COMMANDS(roboti[IdRobot]);
            }
        else if(strcmp(comanda , "LAST_EXECUTED_COMMAND") == 0){
                fout<<"LAST_EXECUTED_COMMAND: ";
                for(int i = 0 ; i < numarul_de_roboti ; i++){
                    deque *q = roboti[i];
                    while(q!=NULL){
                        q -> Timee++;
                        q = q -> next;

                    }
                }
                int x = Pozitie_Stack-2;
                if(LastExecuted[Pozitie_Stack-1].ExecuteOrAdd==1){
                    fout<<LastExecuted[Pozitie_Stack-1].RobotId<<":"<<' '<<LastExecuted[Pozitie_Stack-1].CommandType<<' ';
                    fout<<LastExecuted[Pozitie_Stack-1].Line<<' '<<LastExecuted[Pozitie_Stack-1].Column<<' '<<LastExecuted[Pozitie_Stack-1].Boxes<<'\n';
                }
                else{
                    while(x >= 0){
                        if(LastExecuted[x].ExecuteOrAdd == 1){
                            fout<<LastExecuted[x].RobotId<<":"<<' '<<LastExecuted[x].CommandType<<' ';
                            fout<<LastExecuted[x].Line<<' '<<LastExecuted[x].Column<<' '<<LastExecuted[x].Boxes<<'\n';
                            break;
                        }
                        x--;
                    }
                    if(x == -1)
                    fout<<"No command was executed\n";
                }

            }
        else if(strcmp(comanda , "UNDO") == 0){
                for(int i = 0 ; i < numarul_de_roboti ; i++){
                    deque *q = roboti[i];
                    while(q!=NULL){
                        q -> Timee++;
                        q = q -> next;

                    }
                }
                UNDO();
            }
        else if(strcmp(comanda , "HOW_MANY_BOXES") == 0){
                fout<<"HOW_MANY_BOXES: ";
                for(int i = 0 ; i < numarul_de_roboti ; i++){
                    deque *q = roboti[i];
                    while(q!=NULL){
                        q -> Timee++;
                        q = q -> next;

                    }
                }
                fin>>IdRobot;
                HOW_MANY_BOXES(IdRobot);
            }
        else if(strcmp(comanda , "HOW_MUCH_TIME") == 0){
                fout<<"HOW_MUCH_TIME: ";
                for(int i = 0 ; i < numarul_de_roboti ; i++){
                    deque *q = roboti[i];
                    while(q!=NULL){
                        q -> Timee++;
                        q = q -> next;
                    }
                }
                HOW_MUCH_TIME();
            }
    }
    for(int i = 0 ; i < numarul_de_roboti ; i++){
        deque *q = roboti[i];
        deque *q1 = roboti[i];
        while(q1!=NULL){
        q = q1;
        q1 = q1 -> next;
        delete q;
        }
    }
    delete[] roboti;
    for(int i = 0 ; i < lines ; i++)
    delete[] matrice[i];
    delete[] matrice;
    delete[] nrBoxes;
    delete[] LastExecuted;
    return 0;
}
