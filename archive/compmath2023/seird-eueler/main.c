#include <stdio.h>

const double alphaE = 0.999;
const double alphaI = 0.999;
const double k = 0.042;
const double ro = 0.952;
const double beta = 0.999;
const double mu = 0.0188;
const double c = 1;

double dS(double N, double S, double E, double I)
{
    return (-c * ((alphaI * S * I) / N) + ((alphaE * S * E) / N));
}

double dE(double N, double S, double E, double I)
{
    return (c * ((alphaI * S * I) / N) + ((alphaE * S * E) / N) - ((k + ro) * E));
}

double dI(double E, double I)
{
    return (k * E - beta * I - mu * I);
}

double dR(double E, double I)
{
    return (beta * I + ro * E);
}

double dD(double I)
{
    return (mu * I);
}

double count_N(double S, double E, double I, double R, double D)
{
    return S + E + I + R + D;
}

double SEIRD(double S0, double E0, double I0, double R0, double D0, int t0, double t, double h)
{
    double S = S0, E = E0, I = I0, R = R0, D = D0;
    double Sn, En, In;
    int i = t0;
    int N;
    for (; i < t; i++)
    {

        N = count_N(S, E, I, R, D);
        Sn = S0 + h * dS(N, S0, E0, I0);
        En = E0 + h * dE(N, S0, E0, I0);
        In = I0 + h * dI(E0, I0);

        S = S0 + h * 0.5 * (dS(N, S0, E0, I0) + dS(N, Sn, En, In));
        E = E0 + h * 0.5 * (dE(N, S0, E0, I0) + dE(N, Sn, En, In));
        I = I0 + h * 0.5 * (dI(E0, I0) + dI(En, In));
        R = R0 + h * 0.5 * (dR(E0, I0) + dR(En, In));
        D = D0 + h * 0.5 * (dD(I0) + dD(In));
        S0 = S;
        E0 = E;
        I0 = I;
        R0 = R;
        D0 = D;
    }
    return k * E / 0.58;
}

int main()
{
    double S0 = 2800000, E0 = 99, I0 = 0, R0 = 24, D0 = 0;
    for (int i = 1; i <= 90; i++)
    {
        printf("%d %f\n", i, SEIRD(S0, E0, I0, R0, D0, 0, i, 1));
    }
}