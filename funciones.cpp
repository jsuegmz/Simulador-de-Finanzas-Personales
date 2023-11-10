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
    int diasRepeticion[31];
} automatizado;

struct reporteDia{
    int dia, mes, ano;
    float totalIngresos;
    float totalGastos;
    float totalDia;
    float finalDia;
} reporteDiario;

struct reporteMes{
    int mes, ano;
    float totalIngresos;
    float totalGastos;
    float promedioIngresosDiario;
    float promedioGastosDiario;
    float totalMes;
    float finalMes;
} reporteMensual;

struct reporteAno{
    int ano;
    float totalIngresos;
    float totalGastos;
    float promedioIngresosMensual;
    float promedioGasrosMensal;
    float totalAno;
    float finalAno;
} reporteAnual;

FILE *archivoGeneral, *archivoHistorial, *archivoDatosAutomatizados, *archivoReporteDiario, *archivoReporteMensual, *archivoReporteAnual;

void ingresarDatosDiarios(); //Ingresar datos de un día
void agregarDatosAutomatizados(); //Automatizar gastos o ingresos para un día
void guardarDatosAutomatizados(); //Guarda los datos automatizados en el historial si la fecha actual concuerda con la regsitrada
void ajustarFechaDatoAutomatizado(int i); //Si el mes tiene menos dias que la fecha registrada en el dato automatizado, se ajusta al último día del mes
void sumarUnDia(); //Suma en uno a la fecha actual
void actualizarGeneral(); //Actualiza los datos generales: saldo actual y fecha actual
void datosIniciales(); //Solicita los datos generales si es la primera vez
void editarOEliminarDatos(); //Busca una fecha y permite cambiar los datos en esta, luego actualiza los reportes
void conseguirDatosGenerales(); //Guarda los datos generales en una variable
void mostrarDatosGenerales();//Muestra los datos generales
void mostrarHistorial();//Muestra todos los gastos e ingresos a detalle
void crearReporteDiario(fecha fechaABuscar); //Crea un reporte diario y lo guarda en "REPORTESDIARIOS.txt"
void mostrarReporteDiario(fecha fechaABuscar); //Muestra el reporte diario de una fecha específica
void crearReporteMensual(int mesABuscar, int anoABuscar); //Crea un reporte mensual y lo guarda en "REPORTESMENSUALES.txt"
void mostrarReporteMensual(int mesABuscar, int anoABuscar); //Muestra el reporte mensual de un mes y año específicos
void mostrarReporteMensualTemporal();
void crearReporteAnualTemporal();
void crearReporteAnual(int anoABuscar); //Crea un reporte anual y lo guarda en "REPORTESANUALES.txt"
void crearReporteAnualTemporal();
void mostrarReporteAnual(int anoABuscar); //Muestra el reporte anual de un año específico
void mostrarReporteAnualTemporal();
void elegirYMostrarReporte(); //Pregunta qué tipo de reporte (diario, mensual o anual) mostrar y muestra el reporte correspondiente
void crearReportes(); //Verifica la fecha y se crean los reportes. Todos los dias se crean los diarios, los mensuales a fin de mes y los anuales a fin de año
void compararReportes(); //Llama a las demás funciones de comparar reportes
void compararReportesDiarios(fecha fechaABuscar1, fecha fechaABuscar2); //Compara los reportes diarios
void compararReportesMensuales(int mesABuscar1, int anoABuscar1, int mesABuscar2, int anoABuscar2); //Compara los reportes mensuales
void compararReportesAnuales(int anoABuscar1, int anoABuscar2); //Compara los reportes anuales

void ingresarDatosDiarios(){
    archivoHistorial = fopen("HISTORIAL.txt", "a");
    char opcion;
    do {
        dias.fechaDia = datosGenerales.fechaActual;
        cout << "Ingresa el tipo de transaccion (1 = Ingreso o 2 = Gasto): ";
        cin >> dias.tipoTransaccion;
        cout << "Ingresa la descripcion: ";
        cin.ignore();
        cin.getline(dias.descripcion, 100);
        cout << "Ingresa el monto: ";
        cin >> dias.monto;

        fprintf(archivoHistorial, "%d,%d,%d,%d,%f,%s\n", dias.fechaDia.dia, dias.fechaDia.mes, dias.fechaDia.ano, dias.tipoTransaccion, dias.monto, dias.descripcion);
        cout << "¿Desea ingresar otro dato? (S/N): ";
        cin >> opcion;
    } while (opcion == 'S' || opcion == 's');

    fclose(archivoHistorial);
}

void crearReporteDiario(fecha fechaABuscar) {
    archivoHistorial = fopen("HISTORIAL.txt", "r");
    FILE *archivoReportesDiarios = fopen("REPORTESDIARIOS.txt", "a");

    reporteDiario.dia = fechaABuscar.dia;
    reporteDiario.mes = fechaABuscar.mes;
    reporteDiario.ano = fechaABuscar.ano;
    reporteDiario.totalIngresos = 0;
    reporteDiario.totalGastos = 0;

    while (fscanf(archivoHistorial, "%d,%d,%d,%d,%f,%s", &dias.fechaDia.dia, &dias.fechaDia.mes, &dias.fechaDia.ano, &dias.tipoTransaccion, &dias.monto, dias.descripcion) != EOF) {
        if (dias.fechaDia.dia == fechaABuscar.dia && dias.fechaDia.mes == fechaABuscar.mes && dias.fechaDia.ano == fechaABuscar.ano) {
            if (dias.tipoTransaccion == 1) {
                reporteDiario.totalIngresos += dias.monto;
            } else if (dias.tipoTransaccion == 2) {
                reporteDiario.totalGastos += dias.monto;
            }
        }
    }

    reporteDiario.totalDia = reporteDiario.totalIngresos - reporteDiario.totalGastos;
    datosGenerales.saldoActual += reporteDiario.totalDia;
    actualizarGeneral();

    fprintf(archivoReportesDiarios, "%d,%d,%d,%f,%f,%f\n", reporteDiario.dia, reporteDiario.mes, reporteDiario.ano, reporteDiario.totalIngresos, reporteDiario.totalGastos, reporteDiario.totalDia);

    fclose(archivoHistorial);
    fclose(archivoReportesDiarios);

    cout << "Se ha creado un reporte diario" << endl;
}

void crearReporteMensual(int mesABuscar, int anoABuscar) {
    FILE *archivoReportesDiarios = fopen("REPORTESDIARIOS.txt", "r");
    FILE *archivoReportesMensuales = fopen("REPORTESMENSUALES.txt", "a");

    reporteMensual.mes = mesABuscar;
    reporteMensual.ano = anoABuscar;
    reporteMensual.totalIngresos = 0;
    reporteMensual.totalGastos = 0;
    int diasDelMes = 0;

    while (fscanf(archivoReportesDiarios, "%d,%d,%d,%f,%f,%f", &reporteDiario.dia, &reporteDiario.mes, &reporteDiario.ano, &reporteDiario.totalIngresos, &reporteDiario.totalGastos, &reporteDiario.totalDia) != EOF) {
        if (reporteDiario.mes == mesABuscar && reporteDiario.ano == anoABuscar) {
            reporteMensual.totalIngresos += reporteDiario.totalIngresos;
            reporteMensual.totalGastos += reporteDiario.totalGastos;
            diasDelMes++;
        }
    }

    reporteMensual.promedioIngresosDiario = reporteMensual.totalIngresos / diasDelMes;
    reporteMensual.promedioGastosDiario = reporteMensual.totalGastos / diasDelMes;
    reporteMensual.totalMes = reporteMensual.totalIngresos - reporteMensual.totalGastos;

    fprintf(archivoReportesMensuales, "%d,%d,%f,%f,%f,%f,%f\n", reporteMensual.mes, reporteMensual.ano, reporteMensual.totalIngresos, reporteMensual.totalGastos, reporteMensual.promedioIngresosDiario, reporteMensual.promedioGastosDiario, reporteMensual.totalMes);

    fclose(archivoReportesDiarios);
    fclose(archivoReportesMensuales);

    cout << "Se ha creado un reporte mensual" << endl;
}

void crearReporteMensualTemporal() {
    FILE *archivoReportesDiarios = fopen("REPORTESDIARIOS.txt", "r");
    FILE *archivoReporteMensualTemporal = fopen("REPORTEMENSUALTEMPORAL.txt", "w");

    reporteMensual.mes = datosGenerales.fechaActual.mes;
    reporteMensual.ano = datosGenerales.fechaActual.ano;
    reporteMensual.totalIngresos = 0;
    reporteMensual.totalGastos = 0;
    int diasDelMes = 0;

    while (fscanf(archivoReportesDiarios, "%d,%d,%d,%f,%f,%f", &reporteDiario.dia, &reporteDiario.mes, &reporteDiario.ano, &reporteDiario.totalIngresos, &reporteDiario.totalGastos, &reporteDiario.totalDia) != EOF) {
        if (reporteDiario.mes == datosGenerales.fechaActual.mes && reporteDiario.ano == datosGenerales.fechaActual.ano) {
            reporteMensual.totalIngresos += reporteDiario.totalIngresos;
            reporteMensual.totalGastos += reporteDiario.totalGastos;
            diasDelMes++;
        }
    }

    reporteMensual.promedioIngresosDiario = reporteMensual.totalIngresos / diasDelMes;
    reporteMensual.promedioGastosDiario = reporteMensual.totalGastos / diasDelMes;
    reporteMensual.totalMes = reporteMensual.totalIngresos - reporteMensual.totalGastos;

    fprintf(archivoReporteMensualTemporal, "%d,%d,%f,%f,%f,%f,%f\n", reporteMensual.mes, reporteMensual.ano, reporteMensual.totalIngresos, reporteMensual.totalGastos, reporteMensual.promedioIngresosDiario, reporteMensual.promedioGastosDiario, reporteMensual.totalMes);

    fclose(archivoReportesDiarios);
    fclose(archivoReporteMensualTemporal);
}

void crearReporteAnual(int anoABuscar) {
    FILE *archivoReportesMensuales = fopen("REPORTESMENSUALES.txt", "r");
    FILE *archivoReportesAnuales = fopen("REPORTESANUALES.txt", "a");

    reporteAnual.ano = anoABuscar;
    reporteAnual.totalIngresos = 0;
    reporteAnual.totalGastos = 0;
    int mesesDelAno = 0;

    while (fscanf(archivoReportesMensuales, "%d,%d,%f,%f,%f,%f,%f", &reporteMensual.mes, &reporteMensual.ano, &reporteMensual.totalIngresos, &reporteMensual.totalGastos, &reporteMensual.promedioIngresosDiario, &reporteMensual.promedioGastosDiario, &reporteMensual.totalMes) != EOF) {
        if (reporteMensual.ano == anoABuscar) {
            reporteAnual.totalIngresos += reporteMensual.totalIngresos;
            reporteAnual.totalGastos += reporteMensual.totalGastos;
            mesesDelAno++;
        }
    }

    reporteAnual.promedioIngresosMensual = reporteAnual.totalIngresos / mesesDelAno;
    reporteAnual.promedioGasrosMensal = reporteAnual.totalGastos / mesesDelAno;
    reporteAnual.totalAno = reporteAnual.totalIngresos - reporteAnual.totalGastos;

    fprintf(archivoReportesAnuales, "%d,%f,%f,%f,%f,%f\n", reporteAnual.ano, reporteAnual.totalIngresos, reporteAnual.totalGastos, reporteAnual.promedioIngresosMensual, reporteAnual.promedioGasrosMensal, reporteAnual.totalAno);

    fclose(archivoReportesMensuales);
    fclose(archivoReportesAnuales);

    cout << "Se ha creado un reporte anual";
}

void crearReporteAnualTemporal() {
    FILE *archivoReportesMensuales = fopen("REPORTESMENSUALES.txt", "r");
    FILE *archivoReporteMensualTemporal = fopen("REPORTEMENSUALTEMPORAL.txt", "r");
    FILE *archivoReporteAnualTemporal = fopen("REPORTEANUALTEMPORAL.txt", "w");

    reporteAnual.ano = datosGenerales.fechaActual.ano;
    reporteAnual.totalIngresos = 0;
    reporteAnual.totalGastos = 0;
    int mesesDelAno = 0;

    while (fscanf(archivoReportesMensuales, "%d,%d,%f,%f,%f,%f,%f", &reporteMensual.mes, &reporteMensual.ano, &reporteMensual.totalIngresos, &reporteMensual.totalGastos, &reporteMensual.promedioIngresosDiario, &reporteMensual.promedioGastosDiario, &reporteMensual.totalMes) != EOF) {
        if (reporteMensual.ano == datosGenerales.fechaActual.ano) {
            reporteAnual.totalIngresos += reporteMensual.totalIngresos;
            reporteAnual.totalGastos += reporteMensual.totalGastos;
            mesesDelAno++;
        }
    }

    fscanf(archivoReporteMensualTemporal, "%d,%d,%f,%f,%f,%f,%f", &reporteMensual.mes, &reporteMensual.ano, &reporteMensual.totalIngresos, &reporteMensual.totalGastos, &reporteMensual.promedioIngresosDiario, &reporteMensual.promedioGastosDiario, &reporteMensual.totalMes);
    reporteAnual.totalIngresos += reporteMensual.totalIngresos;
    reporteAnual.totalGastos += reporteMensual.totalGastos;
    mesesDelAno++;
    
    reporteAnual.promedioIngresosMensual = reporteAnual.totalIngresos / mesesDelAno;
    reporteAnual.promedioGasrosMensal = reporteAnual.totalGastos / mesesDelAno;
    reporteAnual.totalAno = reporteAnual.totalIngresos - reporteAnual.totalGastos;

    fprintf(archivoReporteAnualTemporal, "%d,%f,%f,%f,%f,%f\n", reporteAnual.ano, reporteAnual.totalIngresos, reporteAnual.totalGastos, reporteAnual.promedioIngresosMensual, reporteAnual.promedioGasrosMensal, reporteAnual.totalAno);

    fclose(archivoReportesMensuales);
    fclose(archivoReporteAnualTemporal);
    fclose(archivoReporteMensualTemporal);

}

void agregarDatosAutomatizados() {
    archivoDatosAutomatizados = fopen("DATOSAUTOMATIZADOS.txt", "a");

    cout << "Ingresa el tipo de transaccion (1 = Ingreso o 2 = Gasto): ";
    cin >> automatizado.tipoTransaccion;
    cout << "Ingresa la descripcion: ";
    cin.ignore();
    cin.getline(automatizado.descripcion, 100);
    cout << "Ingresa el monto: ";
    cin >> automatizado.monto;
    cout << "Ingresa los dias de repeticion (hasta 31 dias, ingresa 0 para terminar): ";
    for(int i=0; i<31; i++) {
        cin >> automatizado.diasRepeticion[i];
        if(automatizado.diasRepeticion[i] == 0) {
            break;
        }
    }

    fprintf(archivoDatosAutomatizados, "%d,%f,", automatizado.tipoTransaccion, automatizado.monto);
    for(int i=0; i<31; i++) {
        if(automatizado.diasRepeticion[i] != 0) {
            fprintf(archivoDatosAutomatizados, "%d,", automatizado.diasRepeticion[i]);
        } else {
            break;
        }
    }
    fprintf(archivoDatosAutomatizados, ",%s\n", automatizado.descripcion);

    fclose(archivoDatosAutomatizados);
}

void guardarDatosAutomatizados()
{
    archivoDatosAutomatizados = fopen("DATOSAUTOMATIZADOS.txt", "r");
    archivoHistorial = fopen("HISTORIAL.txt", "a");

    conseguirDatosGenerales();

    int contador = 0;

    while (fscanf(archivoDatosAutomatizados, "%d,%f,%d,%s", &automatizado.tipoTransaccion, &automatizado.monto, automatizado.diasRepeticion, automatizado.descripcion) != EOF){

        for(int i=0; i<31; i++) {
            if(automatizado.diasRepeticion[i] != 0) {
                ajustarFechaDatoAutomatizado(i);
        
                if(automatizado.diasRepeticion[i] == datosGenerales.fechaActual.dia){

                    dias.fechaDia = datosGenerales.fechaActual;
                    dias.tipoTransaccion = automatizado.tipoTransaccion;
                    dias.monto = automatizado.monto;
                    strcpy(dias.descripcion, automatizado.descripcion);
            
                    fprintf(archivoHistorial, "%d,%d,%d,%d,%f,%s\n", dias.fechaDia.dia, dias.fechaDia.mes, dias.fechaDia.ano, dias.tipoTransaccion, dias.monto, dias.descripcion);
            
                    contador++;
                }
            }
        }
    }

    fclose(archivoDatosAutomatizados);
    fclose(archivoHistorial);

    if(contador >= 1){
        cout << "Se han ingresado " << contador << " datos automatizados para el dia " << datosGenerales.fechaActual.dia << "/" << datosGenerales.fechaActual.mes << "/" << datosGenerales.fechaActual.ano << endl;
    }
}

void ajustarFechaDatoAutomatizado(int i){
    if(automatizado.diasRepeticion[i] == 31 && (datosGenerales.fechaActual.mes == (4||6||9||11))){
        automatizado.diasRepeticion[i] = 30; //Si la fecha de repetición es 31 y el mes tiene menos de 31 dias, se vuelve 30
    }else if(automatizado.diasRepeticion[i] >= 29 &&(datosGenerales.fechaActual.mes == 2)){
        automatizado.diasRepeticion[i] = 28; //Si la fecha de repetición es 29 o más y es febrero, se vuelve 28
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

    actualizarGeneral();
}

void actualizarGeneral() {
    archivoGeneral = fopen("GENERAL.txt", "w");
    fprintf(archivoGeneral, "%f,%d,%d,%d\n", datosGenerales.saldoActual, datosGenerales.fechaActual.dia, datosGenerales.fechaActual.mes, datosGenerales.fechaActual.ano);
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
        fprintf(archivoGeneral, "%f,%d,%d,%d\n", datosGenerales.saldoActual, datosGenerales.fechaActual.dia, datosGenerales.fechaActual.mes, datosGenerales.fechaActual.ano);
        fclose(archivoGeneral);
    }
}

void editarOEliminarDatos() {
    fecha fechaABuscar;
    int opcion, indice, tipoTransaccion;
    float monto;
    char descripcion[100];

    cout << "Ingresa la fecha de los datos que deseas editar o eliminar." << endl;
    cout << "Dia: ";
    cin >> fechaABuscar.dia;
    cout << "Mes: ";
    cin >> fechaABuscar.mes;
    cout << "Año: ";
    cin >> fechaABuscar.ano;

    FILE *archivoHistorialTemp = fopen("HISTORIAL_TEMP.txt", "w");
    archivoHistorial = fopen("HISTORIAL.txt", "r");

    bool hayDatos = false;

    while (fscanf(archivoHistorial, "%d,%d,%d,%d,%f,%s", &dias.fechaDia.dia, &dias.fechaDia.mes, &dias.fechaDia.ano, &dias.tipoTransaccion, &dias.monto, dias.descripcion) != EOF) {
        if (dias.fechaDia.dia == fechaABuscar.dia && dias.fechaDia.mes == fechaABuscar.mes && dias.fechaDia.ano == fechaABuscar.ano) {
            cout << indice << ". " << (dias.tipoTransaccion == 1 ? "Ingreso" : "Gasto") << ", Descripcion: " << dias.descripcion << ", Monto: " << dias.monto << endl;
            indice++;
            hayDatos = true;
        }
        fprintf(archivoHistorialTemp, "%d,%d,%d,%d,%f,%s\n", dias.fechaDia.dia, dias.fechaDia.mes, dias.fechaDia.ano, dias.tipoTransaccion, dias.monto, dias.descripcion);
    }

    fclose(archivoHistorial);
    fclose(archivoHistorialTemp);

    if (!hayDatos) {
        cout << "No hay datos para la fecha proporcionada." << endl;
        return;
    }

    cout << "Elige el indice del dato que deseas editar o eliminar: ";
    cin >> indice;

    archivoHistorialTemp = fopen("HISTORIAL_TEMP.txt", "r");
    archivoHistorial = fopen("HISTORIAL.txt", "w");

    int i = 0;
    while (fscanf(archivoHistorialTemp, "%d,%d,%d,%d,%f,%s", &dias.fechaDia.dia, &dias.fechaDia.mes, &dias.fechaDia.ano, &dias.tipoTransaccion, &dias.monto, dias.descripcion) != EOF) {
        if (i == indice) {
            cout << "Elige una opcion:" << endl;
            cout << "1. Editar" << endl;
            cout << "2. Eliminar" << endl;
            cin >> opcion;

            if (opcion == 1) {
                cout << "Ingresa el nuevo tipo de transaccion (1 = Ingreso o 2 = Gasto): ";
                cin >> tipoTransaccion;
                cout << "Ingresa la nueva descripcion: ";
                cin.ignore();
                cin.getline(descripcion, 100);
                cout << "Ingresa el nuevo monto: ";
                cin >> monto;

                fprintf(archivoHistorial, "%d,%d,%d,%d,%f,%s\n", dias.fechaDia.dia, dias.fechaDia.mes, dias.fechaDia.ano, tipoTransaccion, monto, descripcion);
            } else if (opcion == 2) {
                continue;
            }
        } else {
            fprintf(archivoHistorial, "%d,%d,%d,%d,%f,%s\n", dias.fechaDia.dia, dias.fechaDia.mes, dias.fechaDia.ano, dias.tipoTransaccion, dias.monto, dias.descripcion);
        }
        i++;
    }

    fclose(archivoHistorial);
    fclose(archivoHistorialTemp);

    // Recrear los reportes
    crearReporteDiario(fechaABuscar);
    if (fechaABuscar.dia == 30 && (fechaABuscar.mes == 4 || fechaABuscar.mes == 6 || fechaABuscar.mes == 9 || fechaABuscar.mes == 11)) {
        crearReporteMensual(fechaABuscar.mes, fechaABuscar.ano);
    } else if (fechaABuscar.dia == 28 && fechaABuscar.mes == 2) {
        crearReporteMensual(fechaABuscar.mes, fechaABuscar.ano);
    } else if (fechaABuscar.dia == 31) {
        crearReporteMensual(fechaABuscar.mes, fechaABuscar.ano);
        if (fechaABuscar.mes == 12) {
            crearReporteAnual(fechaABuscar.ano);
        }
    }
}

void conseguirDatosGenerales(){
    archivoGeneral = fopen("GENERAL.txt", "r");
	
	fscanf(archivoGeneral, "%f,%d,%d,%d", &datosGenerales.saldoActual, &datosGenerales.fechaActual.dia, &datosGenerales.fechaActual.mes, &datosGenerales.fechaActual.ano);
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
        while (fscanf(archivoHistorial, "%d,%d,%d,%d,%f,%s", &dias.fechaDia.dia, &dias.fechaDia.mes, &dias.fechaDia.ano, &dias.tipoTransaccion, &dias.monto, dias.descripcion) != EOF) {
            cout << "Fecha: " << dias.fechaDia.dia << "/" << dias.fechaDia.mes << "/" << dias.fechaDia.ano << endl;
            if(dias.tipoTransaccion == 1){
            	cout << "Tipo: Ingreso" << endl;
			}else if(dias.tipoTransaccion == 2){ 
				cout << "Tipo: Gasto" << endl;
			}
            cout << "Descripcion: ";
            printf("%s\n", dias.descripcion);
            cout << "Monto: " << dias.monto << endl;
            cout << "------------------------" << endl;
        }
        
        fclose(archivoHistorial);
    }
}

void mostrarReporteDiario(fecha fechaABuscar) {
    FILE *archivoReportesDiarios = fopen("REPORTESDIARIOS.txt", "r");

    while (fscanf(archivoReportesDiarios, "%d,%d,%d,%f,%f,%f", &reporteDiario.dia, &reporteDiario.mes, &reporteDiario.ano, &reporteDiario.totalIngresos, &reporteDiario.totalGastos, &reporteDiario.totalDia) != EOF) {
        if (reporteDiario.dia == fechaABuscar.dia && reporteDiario.mes == fechaABuscar.mes && reporteDiario.ano == fechaABuscar.ano) {
            cout << "Fecha: " << reporteDiario.dia << "/" << reporteDiario.mes << "/" << reporteDiario.ano << endl;
            cout << "Total de ingresos: " << reporteDiario.totalIngresos << endl;
            cout << "Total de gastos: " << reporteDiario.totalGastos << endl;
            cout << "Total del dia: " << reporteDiario.totalDia << endl;
            break;
        }
    }

    fclose(archivoReportesDiarios);
}

void mostrarReporteMensual(int mesABuscar, int anoABuscar) {
    FILE *archivoReportesMensuales = fopen("REPORTESMENSUALES.txt", "r");

    while (fscanf(archivoReportesMensuales, "%d,%d,%f,%f,%f,%f,%f", &reporteMensual.mes, &reporteMensual.ano, &reporteMensual.totalIngresos, &reporteMensual.totalGastos, &reporteMensual.promedioIngresosDiario, &reporteMensual.promedioGastosDiario, &reporteMensual.totalMes) != EOF) {
        if (reporteMensual.mes == mesABuscar && reporteMensual.ano == anoABuscar) {
            cout << "Mes: " << reporteMensual.mes << endl;
            cout << "Año: " << reporteMensual.ano << endl;
            cout << "Total de ingresos: " << reporteMensual.totalIngresos << endl;
            cout << "Total de gastos: " << reporteMensual.totalGastos << endl;
            cout << "Promedio de ingresos diarios: " << reporteMensual.promedioIngresosDiario << endl;
            cout << "Promedio de gastos diarios: " << reporteMensual.promedioGastosDiario << endl;
            cout << "Total del mes: " << reporteMensual.totalMes << endl;
            break;
        }
    }

    fclose(archivoReportesMensuales);
}

void mostrarReporteMensualTemporal(){
    FILE *archivoReporteMensualTemporal = fopen("REPORTEMENSUALTEMPORAL.txt", "r");
    fscanf(archivoReporteMensualTemporal, "%d,%d,%f,%f,%f,%f,%f", &reporteMensual.mes, &reporteMensual.ano, &reporteMensual.totalIngresos, &reporteMensual.totalGastos, &reporteMensual.promedioIngresosDiario, &reporteMensual.promedioGastosDiario, &reporteMensual.totalMes);

    cout << "Mes: " << reporteMensual.mes << endl;
    cout << "Año: " << reporteMensual.ano << endl;
    cout << "Total de ingresos: " << reporteMensual.totalIngresos << endl;
    cout << "Total de gastos: " << reporteMensual.totalGastos << endl;
    cout << "Promedio de ingresos diarios: " << reporteMensual.promedioIngresosDiario << endl;
    cout << "Promedio de gastos diarios: " << reporteMensual.promedioGastosDiario << endl;
    cout << "Total del mes: " << reporteMensual.totalMes << endl;

    fclose(archivoReporteMensualTemporal);
}

void mostrarReporteAnual(int anoABuscar) {
    FILE *archivoReportesAnuales = fopen("REPORTESANUALES.txt", "r");

    while (fscanf(archivoReportesAnuales, "%d,%f,%f,%f,%f,%f", &reporteAnual.ano, &reporteAnual.totalIngresos, &reporteAnual.totalGastos, &reporteAnual.promedioIngresosMensual, &reporteAnual.promedioGasrosMensal, &reporteAnual.totalAno) != EOF) {
        if (reporteAnual.ano == anoABuscar) {
            cout << "Año: " << reporteAnual.ano << endl;
            cout << "Total de ingresos: " << reporteAnual.totalIngresos << endl;
            cout << "Total de gastos: " << reporteAnual.totalGastos << endl;
            cout << "Promedio de ingresos mensuales: " << reporteAnual.promedioIngresosMensual << endl;
            cout << "Promedio de gastos mensuales: " << reporteAnual.promedioGasrosMensal << endl;
            cout << "Total del año: " << reporteAnual.totalAno << endl;
            break;
        }
    }

    fclose(archivoReportesAnuales);
}

void mostrarReporteAnualTemporal() {
    FILE *archivoReporteAnualTemporal = fopen("REPORTEANUALTEMPORAL.txt", "r");

    fscanf(archivoReporteAnualTemporal, "%d,%f,%f,%f,%f,%f", &reporteAnual.ano, &reporteAnual.totalIngresos, &reporteAnual.totalGastos, &reporteAnual.promedioIngresosMensual, &reporteAnual.promedioGasrosMensal, &reporteAnual.totalAno);
    
    cout << "Año: " << reporteAnual.ano << endl;
    cout << "Total de ingresos: " << reporteAnual.totalIngresos << endl;
    cout << "Total de gastos: " << reporteAnual.totalGastos << endl;
    cout << "Promedio de ingresos mensuales: " << reporteAnual.promedioIngresosMensual << endl;
    cout << "Promedio de gastos mensuales: " << reporteAnual.promedioGasrosMensal << endl;
    cout << "Total del año: " << reporteAnual.totalAno << endl;

    fclose(archivoReporteAnualTemporal);
}

void elegirYMostrarReporte() {
    int opcion;
    fecha fechaABuscar;

    cout << "Elige una opcion:" << endl;
    cout << "1. Ver reporte diario" << endl;
    cout << "2. Ver reporte mensual" << endl;
    cout << "3. Ver reporte anual" << endl;
    cout << "4. Ver reporte mensual temporal" << endl;
    cout << "5. Ver reporte anual temporal" << endl;
    cout << "6. Comparar reportes" << endl;

    cin >> opcion;

    switch (opcion) {
        case 1:
            cout << "Ingresa la fecha del reporte diario que deseas ver." << endl;
            cout << "Dia: ";
            cin >> fechaABuscar.dia;
            cout << "Mes: ";
            cin >> fechaABuscar.mes;
            cout << "Año: ";
            cin >> fechaABuscar.ano;

            if (fopen("REPORTESDIARIOS.txt", "r") != NULL) {
                mostrarReporteDiario(fechaABuscar);
            } else {
                cout << "No hay informacion sobre el reporte diario de la fecha proporcionada." << endl;
            }
            break;
        case 2:
            cout << "Ingresa el mes y ano del reporte mensual que deseas ver." << endl;
            cout << "Mes: ";
            cin >> fechaABuscar.mes;
            cout << "Año: ";
            cin >> fechaABuscar.ano;

            if (fopen("REPORTESMENSUALES.txt", "r") != NULL) {
                mostrarReporteMensual(fechaABuscar.mes, fechaABuscar.ano);
            } else {
                cout << "No hay informacion sobre el reporte mensual del mes y ano proporcionados." << endl;
            }
            break;
        case 3:
            cout << "Ingresa el ano del reporte anual que deseas ver." << endl;
            cout << "Año: ";
            cin >> fechaABuscar.ano;

            if (fopen("REPORTESANUALES.txt", "r") != NULL) {
                mostrarReporteAnual(fechaABuscar.ano);
            } else {
                cout << "No hay informacion sobre el reporte anual del ano proporcionado." << endl;
            }
            break;
        case 4:
            mostrarReporteMensualTemporal();
            break;
        case 5:
            mostrarReporteAnualTemporal();
            break;
        case 6:
            compararReportes();
            break;
        default:
            cout << "Opcion no valida. Por favor, elige una opcion del 1 al 3." << endl;
            break;
    }
}

void compararReportes() {
    int opcion;
    fecha fechaABuscar1, fechaABuscar2;

    cout << "Elige una opcion:" << endl;
    cout << "1. Comparar reportes diarios" << endl;
    cout << "2. Comparar reportes mensuales" << endl;
    cout << "3. Comparar reportes anuales" << endl;
    cin >> opcion;

    switch (opcion) {
        case 1:
            cout << "Ingresa la fecha del primer reporte diario que deseas comparar." << endl;
            cout << "Dia: ";
            cin >> fechaABuscar1.dia;
            cout << "Mes: ";
            cin >> fechaABuscar1.mes;
            cout << "Año: ";
            cin >> fechaABuscar1.ano;

            cout << "Ingresa la fecha del segundo reporte diario que deseas comparar." << endl;
            cout << "Dia: ";
            cin >> fechaABuscar2.dia;
            cout << "Mes: ";
            cin >> fechaABuscar2.mes;
            cout << "Año: ";
            cin >> fechaABuscar2.ano;

            if (fopen("REPORTESDIARIOS.txt", "r") != NULL) {
                compararReportesDiarios(fechaABuscar1, fechaABuscar2);
            } else {
                cout << "No hay informacion sobre los reportes diarios de las fechas proporcionadas." << endl;
            }
            break;
        case 2:
            cout << "Ingresa el mes y ano del primer reporte mensual que deseas comparar." << endl;
            cout << "Mes: ";
            cin >> fechaABuscar1.mes;
            cout << "Año: ";
            cin >> fechaABuscar1.ano;

            cout << "Ingresa el mes y ano del segundo reporte mensual que deseas comparar." << endl;
            cout << "Mes: ";
            cin >> fechaABuscar2.mes;
            cout << "Año: ";
            cin >> fechaABuscar2.ano;

            if (fopen("REPORTESMENSUALES.txt", "r") != NULL) {
                compararReportesMensuales(fechaABuscar1.mes, fechaABuscar1.ano, fechaABuscar2.mes, fechaABuscar2.ano);
            } else {
                cout << "No hay informacion sobre los reportes mensuales de los meses y anos proporcionados." << endl;
            }
            break;
        case 3:
            cout << "Ingresa el ano del primer reporte anual que deseas comparar." << endl;
            cout << "Año: ";
            cin >> fechaABuscar1.ano;

            cout << "Ingresa el ano del segundo reporte anual que deseas comparar." << endl;
            cout << "Año: ";
            cin >> fechaABuscar2.ano;

            if (fopen("REPORTESANUALES.txt", "r") != NULL) {
                compararReportesAnuales(fechaABuscar1.ano, fechaABuscar2.ano);
            } else {
                cout << "No hay informacion sobre los reportes anuales de los anos proporcionados." << endl;
            }
            break;
        default:
            cout << "Opcion no valida. Por favor, elige una opcion del 1 al 3." << endl;
            break;
    }
}

void compararReportesDiarios(fecha fechaABuscar1, fecha fechaABuscar2) {
    FILE *archivoReportesDiarios = fopen("REPORTESDIARIOS.txt", "r");
    reporteDia reporteDiario1, reporteDiario2;

    while (fscanf(archivoReportesDiarios, "%d,%d,%d,%f,%f,%f", &reporteDiario1.dia, &reporteDiario1.mes, &reporteDiario1.ano, &reporteDiario1.totalIngresos, &reporteDiario1.totalGastos, &reporteDiario1.totalDia) != EOF) {
        if (reporteDiario1.dia == fechaABuscar1.dia && reporteDiario1.mes == fechaABuscar1.mes && reporteDiario1.ano == fechaABuscar1.ano) {
            break;
        }
    }

    while (fscanf(archivoReportesDiarios, "%d,%d,%d,%f,%f,%f", &reporteDiario2.dia, &reporteDiario2.mes, &reporteDiario2.ano, &reporteDiario2.totalIngresos, &reporteDiario2.totalGastos, &reporteDiario2.totalDia) != EOF) {
        if (reporteDiario2.dia == fechaABuscar2.dia && reporteDiario2.mes == fechaABuscar2.mes && reporteDiario2.ano == fechaABuscar2.ano) {
            break;
        }
    }

    fclose(archivoReportesDiarios);

    float cambioTotalIngresos = (reporteDiario2.totalIngresos - reporteDiario1.totalIngresos) / reporteDiario1.totalIngresos * 100;
    float cambioTotalGastos = (reporteDiario2.totalGastos - reporteDiario1.totalGastos) / reporteDiario1.totalGastos * 100;
    float cambioTotalDia = (reporteDiario2.totalDia - reporteDiario1.totalDia) / reporteDiario1.totalDia * 100;

    cout << "Reporte diario 1: " << reporteDiario1.totalIngresos << ", " << reporteDiario1.totalGastos << ", " << reporteDiario1.totalDia << endl;
    cout << "Reporte diario 2: " << reporteDiario2.totalIngresos << ", " << reporteDiario2.totalGastos << ", " << reporteDiario2.totalDia << endl;
    cout << "Cambio: " << cambioTotalIngresos << "%, " << cambioTotalGastos << "%, " << cambioTotalDia << "%" << endl;
}

void compararReportesMensuales(int mesABuscar1, int anoABuscar1, int mesABuscar2, int anoABuscar2) {
    FILE *archivoReportesMensuales = fopen("REPORTESMENSUALES.txt", "r");
    reporteMes reporteMensual1, reporteMensual2;

    while (fscanf(archivoReportesMensuales, "%d,%d,%f,%f,%f,%f,%f", &reporteMensual1.mes, &reporteMensual1.ano, &reporteMensual1.totalIngresos, &reporteMensual1.totalGastos, &reporteMensual1.promedioIngresosDiario, &reporteMensual1.promedioGastosDiario, &reporteMensual1.totalMes) != EOF) {
        if (reporteMensual1.mes == mesABuscar1 && reporteMensual1.ano == anoABuscar1) {
            break;
        }
    }

    while (fscanf(archivoReportesMensuales, "%d,%d,%f,%f,%f,%f,%f", &reporteMensual2.mes, &reporteMensual2.ano, &reporteMensual2.totalIngresos, &reporteMensual2.totalGastos, &reporteMensual2.promedioIngresosDiario, &reporteMensual2.promedioGastosDiario, &reporteMensual2.totalMes) != EOF) {
        if (reporteMensual2.mes == mesABuscar2 && reporteMensual2.ano == anoABuscar2) {
            break;
        }
    }

    fclose(archivoReportesMensuales);

    float cambioTotalIngresos = (reporteMensual2.totalIngresos - reporteMensual1.totalIngresos) / reporteMensual1.totalIngresos * 100;
    float cambioTotalGastos = (reporteMensual2.totalGastos - reporteMensual1.totalGastos) / reporteMensual1.totalGastos * 100;
    float cambioPromedioIngresosDiario = (reporteMensual2.promedioIngresosDiario - reporteMensual1.promedioIngresosDiario) / reporteMensual1.promedioIngresosDiario * 100;
    float cambioPromedioGastosDiario = (reporteMensual2.promedioGastosDiario - reporteMensual1.promedioGastosDiario) / reporteMensual1.promedioGastosDiario * 100;
    float cambioTotalMes = (reporteMensual2.totalMes - reporteMensual1.totalMes) / reporteMensual1.totalMes * 100;

    cout << "Reporte mensual 1: " << reporteMensual1.totalIngresos << ", " << reporteMensual1.totalGastos << ", " << reporteMensual1.promedioIngresosDiario << ", " << reporteMensual1.promedioGastosDiario << ", " << reporteMensual1.totalMes << endl;
    cout << "Reporte mensual 2: " << reporteMensual2.totalIngresos << ", " << reporteMensual2.totalGastos << ", " << reporteMensual2.promedioIngresosDiario << ", " << reporteMensual2.promedioGastosDiario << ", " << reporteMensual2.totalMes << endl;
    cout << "Cambio: " << cambioTotalIngresos << "%, " << cambioTotalGastos << "%, " << cambioPromedioIngresosDiario << "%, " << cambioPromedioGastosDiario << "%, " << cambioTotalMes << "%" << endl;
}

void compararReportesAnuales(int anoABuscar1, int anoABuscar2) {
    FILE *archivoReportesAnuales = fopen("REPORTESANUALES.txt", "r");
    reporteAno reporteAnual1, reporteAnual2;

    while (fscanf(archivoReportesAnuales, "%d,%f,%f,%f,%f,%f", &reporteAnual1.ano, &reporteAnual1.totalIngresos, &reporteAnual1.totalGastos, &reporteAnual1.promedioIngresosMensual, &reporteAnual1.promedioGasrosMensal, &reporteAnual1.totalAno) != EOF) {
        if (reporteAnual1.ano == anoABuscar1) {
            break;
        }
    }

    while (fscanf(archivoReportesAnuales, "%d,%f,%f,%f,%f,%f", &reporteAnual2.ano, &reporteAnual2.totalIngresos, &reporteAnual2.totalGastos, &reporteAnual2.promedioIngresosMensual, &reporteAnual2.promedioGasrosMensal, &reporteAnual2.totalAno) != EOF) {
        if (reporteAnual2.ano == anoABuscar2) {
            break;
        }
    }

    fclose(archivoReportesAnuales);

    float cambioTotalIngresos = (reporteAnual2.totalIngresos - reporteAnual1.totalIngresos) / reporteAnual1.totalIngresos * 100;
    float cambioTotalGastos = (reporteAnual2.totalGastos - reporteAnual1.totalGastos) / reporteAnual1.totalGastos * 100;
    float cambioPromedioIngresosMensual = (reporteAnual2.promedioIngresosMensual - reporteAnual1.promedioIngresosMensual) / reporteAnual1.promedioIngresosMensual * 100;
    float cambioPromedioGastosMensual = (reporteAnual2.promedioGasrosMensal - reporteAnual1.promedioGasrosMensal) / reporteAnual1.promedioGasrosMensal * 100;
    float cambioTotalAno = (reporteAnual2.totalAno - reporteAnual1.totalAno) / reporteAnual1.totalAno * 100;

    cout << "Reporte anual 1: " << reporteAnual1.totalIngresos << ", " << reporteAnual1.totalGastos << ", " << reporteAnual1.promedioIngresosMensual << ", " << reporteAnual1.promedioGasrosMensal << ", " << reporteAnual1.totalAno << endl;
    cout << "Reporte anual 2: " << reporteAnual2.totalIngresos << ", " << reporteAnual2.totalGastos << ", " << reporteAnual2.promedioIngresosMensual << ", " << reporteAnual2.promedioGasrosMensal << ", " << reporteAnual2.totalAno << endl;
    cout << "Cambio: " << cambioTotalIngresos << "%, " << cambioTotalGastos << "%, " << cambioPromedioIngresosMensual << "%, " << cambioPromedioGastosMensual << "%, " << cambioTotalAno << "%" << endl;
}


void crearReportes() {
    // Crear un reporte diario con la fecha actual
    crearReporteDiario(datosGenerales.fechaActual);

    //Crear reportes temporales
    crearReporteMensualTemporal();
    crearReporteAnualTemporal();

    // Si es el último día del mes, crear un reporte mensual
    if ((datosGenerales.fechaActual.mes == 4 || datosGenerales.fechaActual.mes == 6 || datosGenerales.fechaActual.mes == 9 || datosGenerales.fechaActual.mes == 11) && datosGenerales.fechaActual.dia == 30) {
        crearReporteMensual(datosGenerales.fechaActual.mes, datosGenerales.fechaActual.ano);
    } else if (datosGenerales.fechaActual.mes == 2 && datosGenerales.fechaActual.dia == 28) {
        crearReporteMensual(datosGenerales.fechaActual.mes, datosGenerales.fechaActual.ano);
    } else if (datosGenerales.fechaActual.dia == 31) {
        crearReporteMensual(datosGenerales.fechaActual.mes, datosGenerales.fechaActual.ano);
    }

    // Si es el último día del año, crear un reporte anual
    if (datosGenerales.fechaActual.mes == 12 && datosGenerales.fechaActual.dia == 31) {
        crearReporteAnual(datosGenerales.fechaActual.ano);
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
        cout << "1. Ingresar datos del dia" << endl;
        cout << "2. Automatizar datos" << endl;
        cout << "3. Ver reportes" << endl;
        cout << "4. Mostrar historial" << endl;
        cout << "5. Saltar al próximo dia" << endl;
        cout << "6. Eliminar o editar datos" << endl;
        cout << "7. Salir del programa" << endl;
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
                elegirYMostrarReporte();
                system("pause");
                break;
            case 4:
                mostrarHistorial();
                system("pause");
                break;
            case 5:
                crearReportes();
                sumarUnDia();
                guardarDatosAutomatizados();
                system("pause");
                break;
            case 6:
                editarOEliminarDatos();
                system("pause");
                break;
            case 7:
                cout << "Saliendo del programa..." << endl;
                system("pause");
                break;
            default:
                cout << "Opcion no valida. Por favor, elige una opcion del 1 al 7." << endl;
                system("pause");
                break;
        }
    } while (opcion != 7);
}
