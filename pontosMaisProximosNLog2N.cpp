/**
 * Pontifícia Universidade Católica de Minas Gerais - PUC Minas
 * Projeto e Análise de Algoritmos - 08/11/2021
 *
 * Laura Cozzi Ribeiro
 * Lucas Nascimento Tahan Sab
 * Rafael Tristão Schettino César
 *
 * Solução proposta ao problemas dos pontos mais próximos num
 * plano recebido, apresentando complexidade O(nlog²n).
 */
#include <iostream>
#include <bits/stdc++.h>
#include <math.h>

using namespace std;

/**
 * Classe responsável pelo armazenamento das coordenadas de cada
 * ponto no plano recebido.
 */
class Ponto
{
public:
  double x;
  double y;

  Ponto(double x, double y)
  {
    this->x = x;
    this->y = y;
  }
};

Ponto *obterParPontosMaisProximosPorDivisaoConquista(Ponto *, const int, const int);
Ponto *obterParPontosMaisProximos(Ponto *, const int, const int);
Ponto *obterParPontosMaisProximosEmFaixaCentral(Ponto *, const int, const double);
Ponto *obterFaixaCentralDePontosOrdenada(Ponto *, const int, const int, const double);
Ponto *obterPontosOrdenados(const int);
Ponto *obterParPontosDeMenorDistancia(Ponto *, Ponto *);
double calcularDistanciaEntrePontos(Ponto, Ponto);
void ordenarPontosEmFuncaoDeX(Ponto *, const int);
void ordenarPontosEmFuncaoDeY(Ponto *, const int);
void trocarPontosDePosicao(Ponto *, Ponto *);
double obterCoordenada();
int obterQuantidade();

int main(int argc, char const *argv[])
{
  const int QUANTIDADE_PONTOS = obterQuantidade();
  Ponto *pontos = obterPontosOrdenados(QUANTIDADE_PONTOS);
  Ponto *pontosMaisProximos = obterParPontosMaisProximosPorDivisaoConquista(pontos, 0, QUANTIDADE_PONTOS);
  cout << "Pontos mais proximos:\n";
  cout << pontosMaisProximos[0].x << " " << pontosMaisProximos[0].y << "\n";
  cout << pontosMaisProximos[1].x << " " << pontosMaisProximos[1].y << "\n";

  return 0;
}

/**
 * Método responsável por obter o par de pontos de menor distância dentre
 * os pontos do plano recebido. O plano é dividido em metades até que se alcance
 * o caso base com quantidade menor ou igual à 3 pontos, sempre obtendo as menores
 * distâncias dentre os pares de cada metade. Em seguida, comparam-se os pontos
 * presentes na faixa central entre as metades para garantir a não existência
 * de uma distância menor à mínima entre as duas metades.
 * Complexidade: O(nlog²n).
 */
Ponto *obterParPontosMaisProximosPorDivisaoConquista(Ponto *pontos, const int COMECO, const int FIM)
{
  if (FIM - COMECO <= 3)
    return obterParPontosMaisProximos(pontos, COMECO, FIM);

  const int POSICAO_PONTO_CENTRAL = FIM / 2;
  Ponto *pontosMaisProximosAEsquerda = obterParPontosMaisProximosPorDivisaoConquista(pontos, COMECO, POSICAO_PONTO_CENTRAL);
  Ponto *pontosMaisProximosADireita = obterParPontosMaisProximosPorDivisaoConquista(pontos, POSICAO_PONTO_CENTRAL, FIM);

  const double MENOR_DISTANCIA_A_ESQUERDA = calcularDistanciaEntrePontos(pontosMaisProximosAEsquerda[0], pontosMaisProximosAEsquerda[1]);
  const double MENOR_DISTANCIA_A_DIREITA = calcularDistanciaEntrePontos(pontosMaisProximosADireita[0], pontosMaisProximosADireita[1]);
  const double MENOR_DISTANCIA = MENOR_DISTANCIA_A_ESQUERDA < MENOR_DISTANCIA_A_DIREITA ? MENOR_DISTANCIA_A_ESQUERDA : MENOR_DISTANCIA_A_DIREITA;

  Ponto *pontosMaisProximos = MENOR_DISTANCIA == MENOR_DISTANCIA_A_ESQUERDA ? pontosMaisProximosAEsquerda : pontosMaisProximosADireita;
  Ponto *faixaCentralDePontos = obterFaixaCentralDePontosOrdenada(pontos, COMECO, FIM, MENOR_DISTANCIA);

  return obterParPontosDeMenorDistancia(
      pontosMaisProximos,
      obterParPontosMaisProximosEmFaixaCentral(faixaCentralDePontos, sizeof(faixaCentralDePontos) / sizeof(Ponto *), MENOR_DISTANCIA));
}

/**
 * Método responsável por percorrer cada um dos pontos no plano
 * comparando todos os pontos entre si. Para cada distância calculada,
 * caso menor que a menor distância armazenada, os pontos mais próximos
 * e a menor distância são atualizados. A menor distância é inicializada
 * com o maior valor permitido na linguagem para ponto flutuante.
 * Complexidade: O(n²).
 */
Ponto *obterParPontosMaisProximos(Ponto *pontos, const int COMECO, const int FIM)
{
  Ponto *pontosMaisProximos = (Ponto *)malloc(sizeof(Ponto) * 2);
  double menorDistanciaEntrePontos = __DBL_MAX__;

  for (int i = COMECO; i < FIM - 1; i++)
    for (int j = i + 1; j < FIM; j++)
    {
      const double DISTANCIA_ENTRE_PONTOS = calcularDistanciaEntrePontos(pontos[i], pontos[j]);
      if (DISTANCIA_ENTRE_PONTOS < menorDistanciaEntrePontos)
      {
        pontosMaisProximos[0] = pontos[i];
        pontosMaisProximos[1] = pontos[j];
        menorDistanciaEntrePontos = DISTANCIA_ENTRE_PONTOS;
      }
    }

  return pontosMaisProximos;
}

/**
 * Método responsável por percorrer cada um dos pontos na faixa
 * central as distâncias entre os pontos até que esta distância
 * ultrapasse o mínimo determinado. Para cada distância calculada,
 * caso menor que a menor distância armazenada, os pontos mais próximos
 * e a menor distância são atualizados.
 * Complexidade: O(n), visto que o 'for' interno opera até 7 vezes.
 */
Ponto *obterParPontosMaisProximosEmFaixaCentral(Ponto *pontos, const int QUANTIDADE_PONTOS, const double MENOR_DISTANCIA)
{
  Ponto *pontosMaisProximos = (Ponto *)malloc(sizeof(Ponto) * 2);
  double menorDistanciaEntrePontos = MENOR_DISTANCIA;

  for (int i = 0; i < QUANTIDADE_PONTOS; i++)
    for (int j = i + 1; j < QUANTIDADE_PONTOS && pontos[j].y - pontos[i].y < MENOR_DISTANCIA; j++)
    {
      const double DISTANCIA_ENTRE_PONTOS = calcularDistanciaEntrePontos(pontos[i], pontos[j]);
      if (DISTANCIA_ENTRE_PONTOS < menorDistanciaEntrePontos)
      {
        pontosMaisProximos[0] = pontos[i];
        pontosMaisProximos[1] = pontos[j];
        menorDistanciaEntrePontos = DISTANCIA_ENTRE_PONTOS;
      }
    }

  return pontosMaisProximos;
}

/**
 * Método responsável pela obtenção dos pontos contidos na faixa central
 * de pontos do plano, definidos os limites inferior e superior para a faixa
 * a partir da menor distância já calculada.
 */
Ponto *obterFaixaCentralDePontosOrdenada(Ponto *pontos, const int COMECO, const int FIM, const double MENOR_DISTANCIA)
{
  const int QUANTIDADE_PONTOS = FIM - COMECO;
  const int POSICAO_PONTO_CENTRAL = (pontos[0].x + pontos[FIM - 1].x) / 2;
  Ponto *faixaCentralDePontos = (Ponto *)malloc(sizeof(Ponto) * QUANTIDADE_PONTOS);
  int quantidadePontosNaFaixa = 0;
  for (int i = 0, j = 0; i < QUANTIDADE_PONTOS; i++)
    if (abs(pontos[i].x - MENOR_DISTANCIA) < MENOR_DISTANCIA)
    {
      faixaCentralDePontos[j++] = pontos[i];
      quantidadePontosNaFaixa++;
    }

  ordenarPontosEmFuncaoDeY(faixaCentralDePontos, quantidadePontosNaFaixa);

  return faixaCentralDePontos;
}

/**
 * Método responsável pela leitura e armazenamento das coordenadas
 * recebidas em pontos.
 */
Ponto *obterPontosOrdenados(const int QUANTIDADE_PONTOS)
{
  Ponto *pontos = (Ponto *)malloc(sizeof(Ponto) * QUANTIDADE_PONTOS);
  for (int i = 0; i < QUANTIDADE_PONTOS; i++)
    pontos[i] = *(new Ponto(obterCoordenada(), obterCoordenada()));

  ordenarPontosEmFuncaoDeX(pontos, QUANTIDADE_PONTOS);

  return pontos;
}

Ponto *obterParPontosDeMenorDistancia(Ponto *primeiroPar, Ponto *segundoPar)
{
  const double DISTANCIA_PRIMEIRO_PAR = calcularDistanciaEntrePontos(primeiroPar[0], primeiroPar[1]);
  const double DISTANCIA_SEGUNDO_PAR = calcularDistanciaEntrePontos(segundoPar[0], segundoPar[1]);

  return DISTANCIA_PRIMEIRO_PAR < DISTANCIA_SEGUNDO_PAR ? primeiroPar : segundoPar;
}

/**
 * Método responsável pelo cálculo da distância entre os pontos
 * recebidos, baseado no Teorema de Pitágoras.
 */
double calcularDistanciaEntrePontos(Ponto primeiroPonto, Ponto segundoPonto)
{
  const double DELTA_X = segundoPonto.x - primeiroPonto.x;
  const double DELTA_Y = segundoPonto.y - primeiroPonto.y;

  return sqrt(pow(DELTA_X, 2) + pow(DELTA_Y, 2));
}

/**
 * Método responsável pela obtenção dos pontos ordenados em função
 * de sua coordenada x com base no algoritmo Selection Sort.
 * Complexidade: O(n²).
 */
void ordenarPontosEmFuncaoDeX(Ponto *pontos, const int QUANTIDADE_PONTOS)
{
  for (int i = 0; i < QUANTIDADE_PONTOS - 1; i++)
  {
    int posicaoMenorX = i;
    double menorX = pontos[i].x;
    for (int j = i + 1; j < QUANTIDADE_PONTOS; j++)
    {
      if (pontos[j].x < menorX)
      {
        menorX = pontos[j].x;
        posicaoMenorX = j;
      }
    }

    trocarPontosDePosicao(&pontos[i], &pontos[posicaoMenorX]);
  }
}

/**
 * Método responsável pela obtenção dos pontos ordenados em função
 * de sua coordenada y com base no algoritmo Selection Sort.
 * Complexidade: O(n²).
 */
void ordenarPontosEmFuncaoDeY(Ponto *pontos, const int QUANTIDADE_PONTOS)
{
  for (int i = 0; i < QUANTIDADE_PONTOS - 1; i++)
  {
    int posicaoMenorY = i;
    double menorY = pontos[i].y;
    for (int j = i + 1; j < QUANTIDADE_PONTOS; j++)
    {
      if (pontos[j].y < menorY)
      {
        menorY = pontos[j].y;
        posicaoMenorY = j;
      }
    }

    trocarPontosDePosicao(&pontos[i], &pontos[posicaoMenorY]);
  }
}

void trocarPontosDePosicao(Ponto *primeiroPonto, Ponto *segundoPonto)
{
  Ponto pontoAuxiliar = *primeiroPonto;
  *primeiroPonto = *segundoPonto;
  *segundoPonto = pontoAuxiliar;
}

double obterCoordenada()
{
  double coordenada = 0.0;
  cin >> coordenada;

  return coordenada;
}

int obterQuantidade()
{
  int quantidade = 0;
  cin >> quantidade;

  return quantidade;
}