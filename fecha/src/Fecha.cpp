#include "Fecha.h"
#include "FechaException.h"
#define ANIO_BASE 1601


const int Fecha::acumDias[][13] =
{
    {0,0,31,59,90,120,151,181,212,243,273,304,334},
    {0,0,31,60,91,121,152,182,213,244,274,305,335}
};


Fecha::Fecha()
{
    this->diaRel = 1;
}

Fecha::Fecha(int dia, int mes, int anio)
{
    this->setDMA(dia,mes,anio);
}

void Fecha::setDMA(int dia, int mes, int anio)
{

    if(!esFechaValida(dia, mes, anio))
        throw FechaException("La fecha es valida");

    int aniosCompletos = anio - ANIO_BASE;
    int diasAniosCompletos = Fecha::diasAniosCompletos(aniosCompletos);
    diaRel = diasAniosCompletos + diaDelAnio(dia, mes, anio);
}


Fecha& Fecha::operator +=(int dias)
{
    this->diaRel += dias;

    return *this;
}

Fecha Fecha::operator +(int dias) const
{
    Fecha fSuma(*this); //constructor copia por defecto
    fSuma.diaRel += dias;

    return fSuma;
}

int Fecha::diaDelAnio(int dia, int mes, int anio)
{
    return Fecha::acumDias[esBiciesto(anio)? 1: 0][mes] + dia;
}

bool Fecha::esBiciesto(int anio)
{
    return (anio%4==0 && anio%100!=0) || anio%400==0;
}

bool Fecha::esFechaValida(int dia, int mes, int anio)
{
    if(anio > ANIO_BASE)
    {
        if(mes >= 1 && mes <= 12)
            if(dia >= 1 && dia <= cantDiaMes(mes,anio))
                return true;
    }
    return false;
}

int Fecha::cantDiaMes(int mes, int anio)
{
    static const int vCantDias[] = {0, 31,28,31,30,31,30,31,31,30,31,30,31};
    if(mes == 2 && esBiciesto(anio))
        return 29;


    return vCantDias[mes];
}


void Fecha::getDMA(int& d, int& m, int& a) const
{
    int aniosComplCalc = this->diaRel/365;
    int diasAnioComplCalc;

    while((diasAnioComplCalc = diasAniosCompletos(aniosComplCalc)) >= this->diaRel)
        aniosComplCalc--;

    a = ANIO_BASE + aniosComplCalc;

    int diaDelAnio = this->diaRel - diasAnioComplCalc;

    diaYMes(diaDelAnio,a,d,m);
}


int Fecha::diasAniosCompletos(int aniosCompletos)
{
    return aniosCompletos*365 + aniosCompletos/4 - aniosCompletos/100 + aniosCompletos/400;
}

void Fecha::diaYMes(int diaDelAnio, int anio, int& dia, int& mes)
{
    mes = 1;
    int filaAcum = esBiciesto(anio)? 1: 0;

    while( mes<=12 && diaDelAnio>acumDias[filaAcum][mes])
        mes++;

    mes--;
    dia = diaDelAnio-acumDias[filaAcum][mes];
}

int Fecha::operator -(const Fecha& f2) const
{
    return this->diaRel - f2.diaRel;
}


Fecha operator +(int dias, const Fecha& f)
{
    Fecha fSuma(f); //constructor copia por defecto
    fSuma.diaRel += dias;

    return fSuma;
}

bool Fecha::operator <(const Fecha& f2) const
{
    return this->diaRel < f2.diaRel;
}

ostream& operator <<(ostream& sal, const Fecha& f)
{
    int d,m,a;

    f.getDMA(d,m,a);

    sal<<d<<'/'<<m<<'/'<<a;
    return sal;
}

bool Fecha::operator >=(const Fecha& f2) const
{
    return this->diaRel >=  f2.diaRel;
}

istream& operator >>(istream& ent, Fecha& f)
{
    int d,m,a;
    char c;

    ent >> d >> c >> m >> c >>a;

    f.setDMA(d,m,a);

    return ent;
}
