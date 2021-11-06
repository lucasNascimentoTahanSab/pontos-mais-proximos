/**
 * Pontifícia Universidade Católica de Minas Gerais - PUC Minas
 * Projeto e Análise de Algoritmos - 08/11/2021
 *
 * Laura Cozzi Ribeiro
 * Lucas Nascimento Tahan Sab
 * Rafael Tristão Schettino César
 *
 * Solução proposta ao problemas dos pontos mais próximos num
 * plano recebido, apresentando complexidade O(n²).
 */
#include <iostream>
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

Ponto *obterParPontosMaisProximos(Ponto *, const int);
double calcularDistanciaEntrePontos(Ponto, Ponto);
Ponto *obterPontosOrdenados(const int);
double obterCoordenada();
int obterQuantidade();

int main(int argc, char const *argv[])
{
  const int QUANTIDADE_PONTOS = obterQuantidade();
  Ponto *pontos = obterPontosOrdenados(QUANTIDADE_PONTOS);
  Ponto *pontosMaisProximos = obterParPontosMaisProximos(pontos, QUANTIDADE_PONTOS);
  cout << "Pontos mais proximos:\n";
  cout << pontosMaisProximos[0].x << " " << pontosMaisProximos[0].y << "\n";
  cout << pontosMaisProximos[1].x << " " << pontosMaisProximos[1].y << "\n";

  return 0;
}

/**
 * Método responsável por percorrer cada um dos pontos no plano
 * comparando todos os pontos entre si. Para cada distância calculada,
 * caso menor que a menor distância armazenada, os pontos mais próximos
 * e a menor distância são atualizados. A menor distância é inicializada
 * com o maior valor permitido na linguagem para ponto flutuante.
 * Complexidade: O(n²).
 */
Ponto *obterParPontosMaisProximos(Ponto *pontos, const int QUANTIDADE_PONTOS)
{
  Ponto *pontosMaisProximos = (Ponto *)malloc(sizeof(Ponto) * 2);
  double menorDistanciaEntrePontos = __DBL_MAX__;

  for (int i = 0; i < QUANTIDADE_PONTOS - 1; i++)
    for (int j = i + 1; j < QUANTIDADE_PONTOS; j++)
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
 * Método responsável pelo cálculo da distância entre os pontos
 * recebidos, baseado no Teorema de Pitágoras.
 */
double calcularDistanciaEntrePontos(Ponto primeiroPonto, Ponto segundoPonto)
{
  double deltaX = segundoPonto.x - primeiroPonto.x;
  double deltaY = segundoPonto.y - primeiroPonto.y;

  return sqrt(pow(deltaX, 2) + pow(deltaY, 2));
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

  return pontos;
}

void ordenarPontos(Ponto *pontos, const int QUANTIDADE_PONTOS)
{
  int posicaoMenorX = 0;
  double menorX = pontos[0].x;
  for (int i = 0; i < QUANTIDADE_PONTOS - 1; i++)
    for (int j = i + 1; j < QUANTIDADE_PONTOS; j++)
      if (pontos[j].x < menorX)
      {
        menorX = pontos[j].x;
        posicaoMenorX = j;
      }
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