#include <iostream>
#include <stdio.h>

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

FILE *archivoGeneral, *archivoHistorial;

void ingresarDatosDiarios();
void sumarUnDia();
void actualizarFecha();
void ingresarDatosDiarios();
void mostrarHistorial();
void datosIniciales();
void mostrarDatosGenerales();

void ingresarDatosDiarios() {
    char opcion;
    archivoHistorial = fopen("HISTORIAL.txt", "a");
    
    do {
        dias.fechaDia = datosGenerales.fechaActual;
        cout << "Ingresa el tipo de transacción (1 = Ingreso o 2 = Gasto): ";
        cin >> dias.tipoTransaccion;
        cout << "Ingresa la descripción: ";
        cin.ignore();
        cin.getline(dias.descripcion, 100);
        cout << "Ingresa el monto: ";
        cin >> dias.monto;

        fprintf(archivoHistorial, "%d\n%d\n%d\n%d\n%s\n%f\n", dias.fechaDia.dia, dias.fechaDia.mes, dias.fechaDia.ano, dias.tipoTransaccion, dias.descripcion, dias.monto);

        cout << "¿Deseas ingresar más datos? (S/N): ";
        cin >> opcion;
    } while (opcion == 'S' || opcion == 's');
    sumarUnDia();
    actualizarFecha();
    fclose(archivoHistorial);
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

void actualizarFecha() {
    archivoGeneral = fopen("GENERAL.txt", "w");
    fprintf(archivoGeneral, "%f\n%d\n%d\n%d\n", datosGenerales.saldoActual, datosGenerales.fechaActual.dia, datosGenerales.fechaActual.mes, datosGenerales.fechaActual.ano);
    fclose(archivoGeneral);
}

void mostrarHistorial() {
    archivoHistorial = fopen("HISTORIAL.txt", "r");
    
    if (archivoHistorial == NULL) {
        cout << "No hay datos para mostrar." << endl;
    } else {
        while (fscanf(archivoHistorial, "%d%d%d%d%s%f", &dias.fechaDia.dia, &dias.fechaDia.mes, &dias.fechaDia.ano, &dias.tipoTransaccion, dias.descripcion, &dias.monto) != EOF) {
            cout << "Fecha: " << dias.fechaDia.dia << "/" << dias.fechaDia.mes << "/" << dias.fechaDia.ano << endl;
            cout << "Tipo de transacción: " << dias.tipoTransaccion << endl;
            cout << "Descripción: " << dias.descripcion << endl;
            cout << "Monto: " << dias.monto << endl;
            cout << "------------------------" << endl;
        }
        
        fclose(archivoHistorial);
    }
}

void datosIniciales()
{   
    if (archivoGeneral == NULL) {
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

void mostrarDatosGenerales(){
	
	archivoGeneral = fopen("GENERAL.txt", "r");
	
	fscanf(archivoGeneral, "%f", &datosGenerales.saldoActual);
    fscanf(archivoGeneral, "%d", &datosGenerales.fechaActual.dia);
    fscanf(archivoGeneral, "%d", &datosGenerales.fechaActual.mes);
    fscanf(archivoGeneral, "%d", &datosGenerales.fechaActual.ano);
    fclose(archivoGeneral);
    
    cout << "Saldo actual: " << datosGenerales.saldoActual << endl;
    cout << "Fecha actual: " << datosGenerales.fechaActual.dia << "/" << datosGenerales.fechaActual.mes << "/" << datosGenerales.fechaActual.ano << endl;
    fclose(archivoGeneral);

}

int main()
{
    datosIniciales();

    int opcion;

    do {
    	mostrarDatosGenerales();
        cout << "Elige una opción:" << endl;
        cout << "1. Ingresar datos diarios" << endl;
        cout << "2. Mostrar datos diarios" << endl;
        cout << "3. Salir del programa" << endl;
        cin >> opcion;

        switch (opcion) {
            case 1:
                ingresarDatosDiarios();
                break;
            case 2:
                mostrarHistorial();
                break;
            case 3:
                cout << "Saliendo del programa..." << endl;
                break;
            default:
                cout << "Opción no válida. Por favor, elige una opción del 1 al 3." << endl;
                break;
        }
    } while (opcion != 3);
}
