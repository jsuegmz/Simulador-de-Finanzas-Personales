#include <iostream>
#include <stdio.h>
#include <string.h>

using namespace std;

struct fecha{
    int dia, mes, ano;
};

struct general{
    float saldoActual;
    fecha fechaActual;
} datosGenerales;

struct diario{
    fecha fechaDia;
    int tipoTransaccion;
    char descripcion[100];
    float monto;
} dias;

struct datosAutomatizados{
    int tipoTransaccion;
    char descripcion[100];
    float monto;
    int diaRepeticion;
} automatizado;

FILE *archivoGeneral, *archivoHistorial, *datosAutomatizados;

void ingresarDatosDiarios(); //Ingresar datos de un día
void agregarDatosAutomatizados(); //Automatizar gastos o ingresos para un día
void guardarDatosAutomatizados(); //Guarda los datos automatizados en el historial si la fecha actual concuerda con la regsitrada
void ajustarFechaDatoAutomatizado(); //Si el mes tiene menos dias que la fecha registrada en el dato automatizado, se ajusta al último día del mes
void sumarUnDia(); //Suma en uno a la fecha actual
void actualizarGeneral(); //Actualiza los datos generales: saldo actual y fecha actual
void datosIniciales(); //Solicita los datos generales si es la primera vez
void conseguirDatosGenerales(); //Guarda los datos generales en una variable
void mostrarDatosGenerales();//Muestra los datos generales
void mostrarHistorial();//Muestra todos los gastos e ingresos a detalle

void ingresarDatosDiarios(){

    guardarDatosAutomatizados();

    char opcion;
    archivoHistorial = fopen("HISTORIAL.txt", "a"); //Modo de agregar

    do {
        cout << "¿Desea ingresar un dato? (S/N): ";
        cin >> opcion;
        if(opcion == 'N' || opcion == 'n'){
            break;
        }

        dias.fechaDia = datosGenerales.fechaActual;
        cout << "Ingresa el tipo de transaccion (1 = Ingreso o 2 = Gasto): ";
        cin >> dias.tipoTransaccion;
        cout << "Ingresa la descripcion: ";
        cin.ignore();
        cin.getline(dias.descripcion, 100);
        cout << "Ingresa el monto: ";
        cin >> dias.monto;

        fprintf(archivoHistorial, "%d\n%d\n%d\n%d\n%s\n%f\n", dias.fechaDia.dia, dias.fechaDia.mes, dias.fechaDia.ano, dias.tipoTransaccion, dias.descripcion, dias.monto);
    } while (opcion == 'S' || opcion == 's');

    fclose(archivoHistorial);

    sumarUnDia();
    actualizarGeneral();
}

void agregarDatosAutomatizados(){

    datosAutomatizados = fopen("DATOSAUTOAMATIZADOS.txt", "a");

    cout << "Ingresa el tipo de transaccion (1 = Ingreso o 2 = Gasto): ";
    cin >> automatizado.tipoTransaccion;
    cout << "Ingresa la descripcion: ";
    cin.ignore();
    cin.getline(automatizado.descripcion, 100);
    cout << "Ingresa el monto: ";
    cin >> automatizado.monto;
    cout << "Ingrese el dia que quiere que se repita: ";
    cin >> automatizado.diaRepeticion;

    fprintf(datosAutomatizados, "%d\n%s\n%f\n%d\n", automatizado.tipoTransaccion, automatizado.descripcion, automatizado.monto, automatizado.diaRepeticion);
    fclose(datosAutomatizados);
}

void guardarDatosAutomatizados()
{
    datosAutomatizados = fopen("DATOSAUTOMATIZADOS.txt", "r");
    archivoHistorial = fopen("HISTORIAL.txt", "a");

    conseguirDatosGenerales();

    int contador;

    while (fscanf(datosAutomatizados, "%d%s%f%d", &automatizado.tipoTransaccion, &automatizado.descripcion, &automatizado.monto, &automatizado.diaRepeticion) != EOF){

        ajustarFechaDatoAutomatizado();

        if(automatizado.diaRepeticion == datosGenerales.fechaActual.dia){

            dias.fechaDia = datosGenerales.fechaActual;
            dias.tipoTransaccion = automatizado.tipoTransaccion;
            dias.monto = automatizado.monto;
            strcpy(dias.descripcion, automatizado.descripcion);
            
            fprintf(archivoHistorial, "%d\n%d\n%d\n%d\n%s\n%f\n", dias.fechaDia.dia, dias.fechaDia.mes, dias.fechaDia.ano, dias.tipoTransaccion, dias.descripcion, dias.monto);
            
            contador++;
        }
    }

    fclose(datosAutomatizados);
    fclose(archivoHistorial);

    if(contador >= 1){
        cout << "Se han ingresado " << contador << " datos automatizados" << endl;
    }
}

void ajustarFechaDatoAutomatizado(){
    if(automatizado.diaRepeticion = 31 && (datosGenerales.fechaActual.mes == (4||6||9||11))){
        automatizado.diaRepeticion = 30; //Si la fecha de repetición es 31 y el mes tiene menos de 31 dias, se vuelve 30
    }else if(automatizado.diaRepeticion >= 29 &&(datosGenerales.fechaActual.mes == 2)){
        automatizado.diaRepeticion = 28; //Si la fecha de repetición es 29 o más y es febrero, se vuelve 28
    }
}

void sumarUnDia() {
    if (datosGenerales.fechaActual.mes == 2) {
        if (datosGenerales.fechaActual.dia == 28) {
            if ((datosGenerales.fechaActual.ano % 4 == 0 && datosGenerales.fechaActual.ano % 100 != 0) || (datosGenerales.fechaActual.ano % 400 == 0)) {
                datosGenerales.fechaActual.dia++;
            } else {
                datosGenerales.fechaActual.dia = 1;
                datosGenerales.fechaActual.mes++;
            }
        } else if (datosGenerales.fechaActual.dia == 29) {
            datosGenerales.fechaActual.dia = 1;
            datosGenerales.fechaActual.mes++;
        } else {
            datosGenerales.fechaActual.dia++;
        }
    } else if (datosGenerales.fechaActual.mes == 4 || datosGenerales.fechaActual.mes == 6 || datosGenerales.fechaActual.mes == 9 || datosGenerales.fechaActual.mes == 11) {
        if (datosGenerales.fechaActual.dia < 30) {
            datosGenerales.fechaActual.dia++;
        } else {
            datosGenerales.fechaActual.dia = 1;
            datosGenerales.fechaActual.mes++;
        }
    } else if (datosGenerales.fechaActual.dia < 31) {
        datosGenerales.fechaActual.dia++;
    } else if (datosGenerales.fechaActual.mes < 12) {
        datosGenerales.fechaActual.dia = 1;
        datosGenerales.fechaActual.mes++;
    } else {
        datosGenerales.fechaActual.dia = 1;
        datosGenerales.fechaActual.mes = 1;
        datosGenerales.fechaActual.ano++;
    }
}

void actualizarGeneral() {
    archivoGeneral = fopen("GENERAL.txt", "w");
    fprintf(archivoGeneral, "%f\n%d\n%d\n%d\n", datosGenerales.saldoActual, datosGenerales.fechaActual.dia, datosGenerales.fechaActual.mes, datosGenerales.fechaActual.ano);
    fclose(archivoGeneral);
}

void datosIniciales()
{   archivoGeneral = fopen("GENERAL.txt", "r");
    if (archivoGeneral == NULL) {
    	fclose(archivoGeneral);
        cout << "Bienvenido al sistema de simulador de finanzas personales" << endl;
        cout << "Como es tu primera vez iniciando el programa, ingresa estos datos iniciales" << endl;
        cout << "Ingresa tu saldo actual: ";
        cin >> datosGenerales.saldoActual;
        cout << "Ingresa la fecha actual" << endl;
        cout << "Dia: ";
        cin >> datosGenerales.fechaActual.dia;
        cout << "Mes: ";
        cin >> datosGenerales.fechaActual.mes;
        cout << "Año: ";
        cin >> datosGenerales.fechaActual.ano;
        archivoGeneral = fopen("GENERAL.txt", "w");
        fprintf(archivoGeneral, "%f\n%d\n%d\n%d\n", datosGenerales.saldoActual, datosGenerales.fechaActual.dia, datosGenerales.fechaActual.mes, datosGenerales.fechaActual.ano);
        fclose(archivoGeneral);
    }
}

void conseguirDatosGenerales(){
    archivoGeneral = fopen("GENERAL.txt", "r");
	
	fscanf(archivoGeneral, "%f", &datosGenerales.saldoActual);
    fscanf(archivoGeneral, "%d", &datosGenerales.fechaActual.dia);
    fscanf(archivoGeneral, "%d", &datosGenerales.fechaActual.mes);
    fscanf(archivoGeneral, "%d", &datosGenerales.fechaActual.ano);
        
    fclose(archivoGeneral);
}

void mostrarDatosGenerales(){

    conseguirDatosGenerales();

    cout << "Saldo actual: " << datosGenerales.saldoActual << endl;
    cout << "Fecha actual: " << datosGenerales.fechaActual.dia << "/" << datosGenerales.fechaActual.mes << "/" << datosGenerales.fechaActual.ano << endl;
}

void mostrarHistorial() {
    archivoHistorial = fopen("HISTORIAL.txt", "r");
    
    if (archivoHistorial == NULL) {
        cout << "No hay datos para mostrar." << endl;
    } else {
        while (fscanf(archivoHistorial, "%d%d%d%d%s%f", &dias.fechaDia.dia, &dias.fechaDia.mes, &dias.fechaDia.ano, &dias.tipoTransaccion, dias.descripcion, &dias.monto) != EOF) {
            cout << "Fecha: " << dias.fechaDia.dia << "/" << dias.fechaDia.mes << "/" << dias.fechaDia.ano << endl;
            if(dias.tipoTransaccion == 1)
            {
            	cout << "Tipo: Ingreso" << endl;
			}else if(dias.tipoTransaccion == 2){ 
				cout << "Tipo: Gasto" << endl;
			}
            cout << "Descripcion: " << dias.descripcion << endl;
            cout << "Monto: " << dias.monto << endl;
            cout << "------------------------" << endl;
        }
        
        fclose(archivoHistorial);
    }
}

int main()
{
    datosIniciales();

    int opcion;

    do {
    	system("cls||clear");
    	mostrarDatosGenerales();
        cout << "Elige una opcion:" << endl;
        cout << "1. Ingresar datos del dia (" << datosGenerales.fechaActual.dia << "/" << datosGenerales.fechaActual.mes << "/" << datosGenerales.fechaActual.ano << ")" << endl;
        cout << "2. Agregar datos automatizados" << endl;
        cout << "3. Mostrar historial" << endl;
        cout << "4. Salir del programa" << endl;
        cin >> opcion;

        switch (opcion) {
            case 1:
                ingresarDatosDiarios();
                system("pause");
                break;
            case 2:
                agregarDatosAutomatizados();
                system("pause");
                break;
            case 3:
                mostrarHistorial();
                system("pause");
                break;
            case 4:
                cout << "Saliendo del programa..." << endl;
                system("pause");
                break;
            default:
                cout << "Opcion no valida. Por favor, elige una opcion del 1 al 4." << endl;
                system("pause");
                break;
        }
    } while (opcion != 4);
}
