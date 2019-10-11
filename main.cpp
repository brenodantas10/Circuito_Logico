#include <iostream>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

enum tporta{
    tand=1,
    tor=2,
    tnot=3

};

enum bool_3{
    indefinido=-1,
    falso=0,
    verdade=1
};

class Porta
{
protected:
    bool_3 **input, output;
    int *ConIn;
public:
    Porta(){
        input=NULL;
        ConIn=NULL;
        output=indefinido;
    };
    Porta(unsigned a){
        input=new bool_3*[a];
        ConIn=new int[a];

        for(unsigned i=0; i<a; i++){
            if(i<a){
                input[i]=NULL;
                ConIn[i]=0;
            }
        }
        output=indefinido;
    }
    ~Porta(){
        if(input!=NULL) delete[] input;
        if(ConIn!=NULL) delete[] ConIn;
    };
    virtual void funcao()=0;
    virtual unsigned get_Nin()=0;
    virtual tporta get_Tipo()=0;
    virtual string get_Nome()=0;
    bool_3 get_Output(){return output;};
    bool_3* get_Output_Ref(){return &output;};
    bool_3 get_Input(int k){
        if(k<0 || k>=(int)get_Nin()){
            cerr << "\nERROR\n";
            exit(0);
        }
        return *input[k];
    };
    int get_Connection(const int i){
        if(i<0 || i>(int)get_Nin()){
            cerr << "\nERROR\n";
            exit(0);
        }
        return ConIn[i];
    };
    void Set_Input_Connection(int i, int k){
        if(i>=(int)get_Nin() || i<0){
            cerr << "\nERROR\n";
            exit(0);
        }
        ConIn[i]=k;
    };
    void Connect_Input(const int i, bool_3 *a){
        input[i]=a;
    };
    void Limpar(){output=indefinido;};
};

class Porta_Not: public Porta
{
public:
    Porta_Not(): Porta(get_Nin()){};
    void funcao(){
        if(*input[0]==verdade)    output=falso;
        else if(*input[0]==falso) output=verdade;
        else                      output=indefinido;
    };
    unsigned get_Nin(){return 1;};
    tporta get_Tipo(){return tnot;};
    string get_Nome(){return "Not";};
} ;

class Porta_Or: public Porta
{
public:
    Porta_Or(): Porta(get_Nin()){};
    void funcao(){
        if(*input[0]==verdade || *input[1]==verdade)    output=verdade;
        else if(*input[0]==falso && *input[1]==falso)   output=falso;
        else                                            output=indefinido;
    };
    unsigned get_Nin(){return 2;};
    tporta get_Tipo(){return tor;};
    string get_Nome(){return "Or";};
};

class Porta_And : public Porta
{
public:
    Porta_And():Porta(get_Nin()){};
    void funcao(){
        if(*input[0]==verdade && *input[1]==verdade)    output=verdade;
        else if(*input[0]==falso || *input[1]==falso)   output=falso;
        else                                            output=indefinido;
    };
    unsigned get_Nin(){return 2;};
    tporta get_Tipo(){return tand;};
    string get_Nome(){return "And";};
};


class Circuito
{
private:
    unsigned Nin, Nout, Nporta;
    int *ConOut;
    bool_3 *input, **output;
    Porta **porta;
public:
    Circuito(){
        input=NULL;
        output=NULL;
        porta=NULL;
        ConOut=NULL;
    }
    ~Circuito(){
        if(input!=NULL)     delete[] input;
        if(output!=NULL)    delete[] output;
        if(ConOut!=NULL)    delete[] ConOut;
        if(porta!=NULL)     delete[] porta;

    }
    void Montar(){
        int k;

        do{
            cout << "Quantas entradas tem o circuito?\n";
            cin >> k;

            if(k<1){
                cout << "\nO numero de entradas nao pode ser menor que 1";
                fflush(stdin); getchar();
            }
            system("cls");
        }while(k<1);
        Nin=k;
        do{
            cout << "Quantas saidas tem o circuito?\n";
            cin >> k;

            if(k<1){
                cout << "\nO numero de saidas nao pode ser menor que 1";
                fflush(stdin); getchar();
            }
            system("cls");
        }while(k<1);
        Nout=k;
        do{
            cout << "Quantas portas tem o circuito?\n";
            cin >> k;

            if(k<1){
                cout << "\nO numero de portas nao pode ser menor que 1";
                fflush(stdin); getchar();
            }
            system("cls");
        }while(k<1);
        Nporta=k;

        input=new bool_3[Nin];
        output=new bool_3*[Nout];
        porta=new Porta*[Nporta];
        ConOut=new int[Nout];

        for(unsigned i=0; i<Nout; i++) ConOut[i]=0;
        for(unsigned i=0; i<Nporta; i++){
            porta[i]=NULL;
            do{
                cout << "Qual o tipo da porta " << i+1 << "? ((1)-And, (2)-Or, (3)-Not\n";
                cin >> k;

                if(k==3) porta[i]=new Porta_Not;
                else if(k==2) porta[i]=new Porta_Or;
                else if(k==1) porta[i]=new Porta_And;
                else{
                    cout << "\nValor indefinido\n";
                    fflush(stdin); getchar();
                }
                system("cls");
            }while(k<1 || k>3);
        }
    };
    void Conectar(){
        int k;
        for(unsigned i=0; i<Nporta; i++){
            for(unsigned j=0; j<porta[i]->get_Nin(); j++){
                cout << "Deseja conectar a entrada " << j+1 << " da porta(" << porta[i]->get_Nome() <<") " << i+1 << " na porta/entrada\n(+)-Portas, (-)-Entradas\n";
                cin >> k;
                if(-k<=(int)Nin && k<=(int)Nporta){
                    porta[i]->Set_Input_Connection(j,k);
                }else{
                    j--;
                    cout << "\nentrada/porta invalida\n";
                    fflush(stdin); getchar();
                }
                system("cls");
            }
        }

        for(unsigned i=0; i<Nout; i++){
            cout << "A qual porta/entrada vai querer conectar a saida " << i+1 << " do circuito?\n(+)-Portas, (-)-Entradas\n";
            cin >> k;

            if(-k<=(int)Nin && k<=(int)Nporta){
                ConOut[i]=k;
            }else{
                i--;
                cout << "\nentrada/porta invalida\n";
                fflush(stdin); getchar();
            }
            system("cls");
        }
        Conectado();
    }
    void Simular(){
        bool batata=Check();

        int k;
        if(batata && Nin>0 && Nout>0 && Nporta>0){
            for(unsigned i=0; i<Nin; i++){
                do{
                    cout << "Qual o sinal da entrada " << i+1 << "?\n";
                    cin >> k;
                }while(k<-1 || k>1);
                if(k==-1) input[i]=indefinido;
                else if(k==0) input[i]=falso;
                else if(k==1) input[i]=verdade;
            }

            bool_3 l;
            do{
                k=0;
                for(unsigned i=0; i<Nporta; i++){
                    l=porta[i]->get_Output();
                    porta[i]->funcao();
                    if(l!=porta[i]->get_Output()) k++;
                }
            }while(k>0);

            cout << "In\tOut\n";
            for(unsigned i=0; i<Nin || i<Nout; i++){
                if(i<Nin) cout << input[i];
                cout << '\t';
                if(i<Nout) cout << *output[i];
                cout << '\n';
            }
            fflush(stdin); getchar();
        }else{
            cout << "Nao pode simular, o circuito pode nao estar montado devidamente\n";
            fflush(stdin); getchar();
            system("cls");
        }
    };
    bool Check(){
        unsigned k=0;

        for(unsigned i=0; i<Nporta; i++){
            for(unsigned j=0; j<porta[i]->get_Nin(); j++)
                if(porta[i]->get_Connection(j)==0 || porta[i]->get_Connection(j)>(int)Nporta || -porta[i]->get_Connection(j)>(int)Nin) k++;
        }

        for(unsigned i=0; i<Nout; i++) if(ConOut[i]==0 || ConOut[i]>(int)Nporta || -ConOut[i]>(int)Nin) k++;

        return k==0;
    };
    void Limpar(){
        for(unsigned i=0; i<Nporta; i++){
            porta[i]->Limpar();
        }
    };
    void Salvar(const char* arq){
        FILE *f=fopen(arq, "w");
        if(f==NULL || Nin==0 || Nout==0 || Nporta==0){
            cout << "Falha ao ao tentar Salvar Circuito\n";
            fflush(stdin); getchar();
            return;
        }
        fprintf(f, "CIRCUITO %u %u %u\nPORTAS:\n", Nin, Nout, Nporta);
        for(unsigned i=0; i<Nporta; i++){
            fprintf(f, "%d)\tTIPO: %d\tENTRADAS: ", i+1, (int) porta[i]->get_Tipo());
            for(unsigned j=0; j<porta[i]->get_Nin(); j++){
                fprintf(f, " %d", porta[i]->get_Connection(j));
            }
            fprintf(f,"\n");
        }
        fprintf(f, "SAIDAS:\n");
        for(unsigned i=0; i<Nout; i++){
            fprintf(f, "%d)\tCONEXAO: %d\n", i+1, ConOut[i]);
        }
        fclose(f);
    };
    void Abrir(const char* arq){
        FILE *f=fopen(arq, "r");
        if(f==NULL){
            cout << "Falha ao abrir arquivo\n";
            return;
        }
        Zerar();

        bool ok;

        unsigned Nin_prov, Nout_prov, Nporta_prov;

        ok = fscanf(f,"CIRCUITO %u %u %u\nPORTAS:\n", &Nin_prov, &Nout_prov, &Nporta_prov)==3;

        if(ok){
            Nin=Nin_prov;
            input=new bool_3[Nin];
            for(unsigned i=0; i<Nin; i++) input[i]=indefinido;
            Nout=Nout_prov;
            output=new bool_3*[Nout];
            ConOut=new int[Nout];
            for(unsigned i=0; i<Nout; i++){
                output[i]=NULL;
                ConOut[i]=0;
            }
            Nporta=Nporta_prov;
            porta=new Porta*[Nporta];
            for(unsigned i=0; i<Nporta; i++) porta[i]=NULL;
        }else{
            cout << "Erro ao ler parametros base do arquivo " << arq << " pro Circuito\n";
        }

        int id_prov, tipo_prov, id_io_prov;
        for(unsigned i=0; i<Nporta && ok; i++){
            ok=fscanf(f, "%d)\tTIPO: %d\tENTRADAS: ", &id_prov, &tipo_prov)==2;
            if(ok){
                if(tipo_prov==1) porta[i]=new Porta_And;
                else if(tipo_prov==2) porta[i]=new Porta_Or;
                else if(tipo_prov==3) porta[i]=new Porta_Not;
                else                  ok=false;
            }else cerr << "Erro ao ler parametros do arquivo\n";

            for(unsigned j=0; j<porta[i]->get_Nin() && ok; j++){
                ok=fscanf(f, " %d", &id_io_prov)==1;
                if(ok){
                    porta[i]->Set_Input_Connection(j,id_io_prov);
                }else cerr << "Erro ao ler parametros de saida de porta\n";
            }
            fscanf(f, "\n");
        }

        if(ok) fprintf(f, "SAIDAS:\n");
        int id_out_prov, ConOut_prov;
        for(unsigned i=0; i<Nout && ok; i++){
            ok=fscanf(f, "%d)\tCONEXAO: %d\n", &id_out_prov, &ConOut_prov)==2;
            if(ok){
                ConOut[i]=ConOut_prov;
            }else cerr << "Erro ao ler parametros de saida\n";
        }

        fclose(f);
        if(ok && Check()){
            Conectado();
        }else{
            cerr << "Ha partes corrompidas do circuito, zerando...\n";
            Zerar();
            cerr << "Zerado!!!\n";
            fflush(stdin); getchar();
        }
    };
    void Zerar(){
        if(input!=NULL)     delete[] input;
        if(output!=NULL)    delete[] output;
        if(ConOut!=NULL)    delete[] ConOut;
        if(porta!=NULL)     delete[] porta;
        input=NULL;
        output=NULL;
        ConOut=NULL;
        porta=NULL;
        Nin=0; Nout=0; Nporta=0;
    };
    void Conectado(){
        if(!Check()){
            cerr << "ERROR\n";
            return;
        }
        for(unsigned i=0; i<Nporta; i++){
            for(unsigned j=0; j<porta[i]->get_Nin(); j++){
                if(porta[i]->get_Connection(j)>0 && porta[i]->get_Connection(j)<=(int)Nporta){
                    porta[i]->Connect_Input(j, porta[porta[i]->get_Connection(j)-1]->get_Output_Ref());

                }else if(porta[i]->get_Connection(j)<0 && -porta[i]->get_Connection(j)<=(int)Nin){
                    porta[i]->Connect_Input(j, &input[0-porta[i]->get_Connection(j)-1]);
                }
            }
        }

        for(unsigned i=0; i<Nout; i++){
            if(ConOut[i]>0 && ConOut[i]<=(int)Nporta){
                output[i]=porta[ConOut[i]-1]->get_Output_Ref();
            }else if(ConOut[i]<0 && -ConOut[i]<=(int)Nin){
                output[i]=&input[0-ConOut[i]-1];
            }
        }
    };
    void Imprimir(){
        if(!Check()){
            cerr << "ERROR: O Circuito Pode Nao Estar Montado\n";
            fflush(stdin); getchar();
            return;
        }

        printf("CIRCUITO %u %u %u\nPORTAS:\n", Nin, Nout, Nporta);
        for(unsigned i=0; i<Nporta; i++){
            printf("%d)\tTIPO: %d\tENTRADAS: ", i+1, (int) porta[i]->get_Tipo());
            for(unsigned j=0; j<porta[i]->get_Nin(); j++){
                printf(" %d", porta[i]->get_Connection(j));
            }
            printf("\n");
        }
        printf("SAIDAS:\n");
        for(unsigned i=0; i<Nout; i++){
            printf("%d)\tCONEXAO: %d\n", i+1, ConOut[i]);
        }
    }
};

int main()
{
    Circuito a;
    char* c;
    string arq;
    int b;
    do{
        cout << "O que desejas fazer?\n"
             << "1 - Criar um Circuito\n"
             << "2 - Salvar Circuito em Arquivo\n"
             << "3 - Abrir Circuito de um Arquivo\n"
             << "4 - Imprimir um Circuito\n"
             << "5 - Simular um Circuito\n"
             << "6 - Limpar Circuito\n"
             << "0 - Sair do Programa\n";

        cin >> b;
        system("cls");
        switch(b){
            case 1:
                a.Montar();
                a.Conectar();
                break;
            case 2:
                cout << "Qual o nome do arquivo que desejas salvar?\n";
                cin >> arq;
                c=new char[arq.length()+1];
                std::copy(arq.begin(), arq.end(), c);
                c[arq.length()]=0;
                a.Salvar(c);
                delete[] c;
                break;
            case 3:
                cout << "Qual o nome do arquivo que desejas abrir?\n";
                cin >> arq;
                c=new char[arq.length()+1];
                std::copy(arq.begin(), arq.end(), c);
                c[arq.length()]=0;
                a.Abrir(c);
                delete[] c;
                break;
            case 4:
                a.Imprimir();
                fflush(stdin); getchar();
                break;
            case 5:
                a.Simular();
                a.Limpar();
                break;
            case 6:
                a.Zerar();
                break;
            default:
                break;
        }
        system("cls");
    }while(b!=0);

    return 0;
}
