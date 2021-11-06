/**
 * Pontifícia Universidade Católica de Minas Gerais - PUC Minas
 * Projeto e Análise de Algoritmos - 08/11/2021
 *
 * Laura Cozzi Ribeiro
 * Lucas Nascimento Tahan Sab
 * Rafael Tristão Schettino César
 *
 * Solução proposta ao problemas dos pontos mais próximos num
 * plano recebido, apresentando complexidade O(nlogn).
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

Ponto *obterParPontosMaisProximosPorDivisaoConquista(Ponto *, Ponto *, const int, const int);
Ponto *obterParPontosMaisProximos(Ponto *, const int, const int);
Ponto *obterParPontosMaisProximosEmFaixaCentral(Ponto *, const int, const double);
Ponto *obterPontosOrdenadosEmYAEsquerda(Ponto *, Ponto, const int, const int, const int);
Ponto *obterPontosOrdenadosEmYADireita(Ponto *, Ponto, const int, const int, const int);
Ponto *obterFaixaCentralDePontosOrdenada(Ponto *, const int, const int, const int, const double);
Ponto *obterParPontosDeMenorDistancia(Ponto *, Ponto *);
Ponto *obterPontosOrdenadosEmFuncaoDeX(Ponto *, const int);
Ponto *obterPontosOrdenadosEmFuncaoDeY(Ponto *, const int);
Ponto *obterPontos(const int);
int obterQuantidadePontosNaFaixa(Ponto *, const int, const int, const double);
double calcularDistanciaEntrePontos(Ponto, Ponto);
void trocarPontosDePosicao(Ponto *, Ponto *);
double obterCoordenada();
int obterQuantidade();

int main(int argc, char const *argv[])
{
  const int QUANTIDADE_PONTOS = obterQuantidade();
  Ponto *pontos = obterPontos(QUANTIDADE_PONTOS);
  Ponto *pontosOrdenadosEmX = obterPontosOrdenadosEmFuncaoDeX(pontos, QUANTIDADE_PONTOS);
  Ponto *pontosOrdenadosEmY = obterPontosOrdenadosEmFuncaoDeY(pontos, QUANTIDADE_PONTOS);
  Ponto *pontosMaisProximos = obterParPontosMaisProximosPorDivisaoConquista(pontosOrdenadosEmX, pontosOrdenadosEmY, 0, QUANTIDADE_PONTOS);
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
Ponto *obterParPontosMaisProximosPorDivisaoConquista(Ponto *pontosOrdenadosEmX, Ponto *pontosOrdenadosEmY, const int COMECO, const int FIM)
{
  if (FIM - COMECO <= 3)
    return obterParPontosMaisProximos(pontosOrdenadosEmX, COMECO, FIM);

  const int POSICAO_PONTO_CENTRAL = FIM / 2;
  Ponto pontoCentralEmX = pontosOrdenadosEmX[POSICAO_PONTO_CENTRAL];
  Ponto *pontosOrdenadosEmYAEsquerda = obterPontosOrdenadosEmYAEsquerda(pontosOrdenadosEmY, pontoCentralEmX, POSICAO_PONTO_CENTRAL, COMECO, FIM);
  Ponto *pontosOrdenadosEmYADireita = obterPontosOrdenadosEmYADireita(pontosOrdenadosEmY, pontoCentralEmX, POSICAO_PONTO_CENTRAL, COMECO, FIM);

  Ponto *pontosMaisProximosAEsquerda = obterParPontosMaisProximosPorDivisaoConquista(pontosOrdenadosEmX, pontosOrdenadosEmYADireita, COMECO, POSICAO_PONTO_CENTRAL);
  Ponto *pontosMaisProximosADireita = obterParPontosMaisProximosPorDivisaoConquista(pontosOrdenadosEmX, pontosOrdenadosEmYAEsquerda, POSICAO_PONTO_CENTRAL, FIM);

  const double MENOR_DISTANCIA_A_ESQUERDA = calcularDistanciaEntrePontos(pontosMaisProximosAEsquerda[0], pontosMaisProximosAEsquerda[1]);
  const double MENOR_DISTANCIA_A_DIREITA = calcularDistanciaEntrePontos(pontosMaisProximosADireita[0], pontosMaisProximosADireita[1]);
  const double MENOR_DISTANCIA = MENOR_DISTANCIA_A_ESQUERDA < MENOR_DISTANCIA_A_DIREITA ? MENOR_DISTANCIA_A_ESQUERDA : MENOR_DISTANCIA_A_DIREITA;
  Ponto *pontosMaisProximos = MENOR_DISTANCIA == MENOR_DISTANCIA_A_ESQUERDA ? pontosMaisProximosAEsquerda : pontosMaisProximosADireita;

  const int QUANTIDADE_PONTOS_FAIXA = obterQuantidadePontosNaFaixa(pontosOrdenadosEmY, COMECO, FIM, MENOR_DISTANCIA);
  Ponto *faixaCentralDePontos = obterFaixaCentralDePontosOrdenada(pontosOrdenadosEmY, COMECO, FIM, QUANTIDADE_PONTOS_FAIXA, MENOR_DISTANCIA);
  if (faixaCentralDePontos == NULL)
    return pontosMaisProximos;

  Ponto *pontosMaisProximosNaFaixa = obterParPontosMaisProximosEmFaixaCentral(faixaCentralDePontos, QUANTIDADE_PONTOS_FAIXA, MENOR_DISTANCIA);

  return obterParPontosDeMenorDistancia(pontosMaisProximos, pontosMaisProximosNaFaixa);
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
 * central, calculando as distâncias entre os pontos até que esta distância
 * ultrapasse o mínimo determinado. Para cada distância calculada, caso menor
 * que a menor distância armazenada, os pontos mais próximos e a menor
 * distância são atualizados.
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
 * Método responsável por separar os pontos, à esquerda, da lista
 * de pontos ordenados em relação à coordenada y.
 */
Ponto *obterPontosOrdenadosEmYAEsquerda(Ponto *pontos, Ponto pontoCentralEmX, const int POSICAO_PONTO_CENTRAL, const int COMECO, const int FIM)
{
  Ponto *pontosOrdenadosAEsquerda = (Ponto *)malloc(sizeof(Ponto) * POSICAO_PONTO_CENTRAL);
  for (int i = COMECO, posicaoAEsquerda = 0; i < FIM || posicaoAEsquerda < POSICAO_PONTO_CENTRAL; i++)
    if (pontos[i].x < pontoCentralEmX.x || (pontos[i].x == pontoCentralEmX.x && pontos[i].y < pontoCentralEmX.y))
      pontosOrdenadosAEsquerda[posicaoAEsquerda++] = pontos[i];

  return pontosOrdenadosAEsquerda;
}

/**
 * Método responsável por separar os pontos, à direita, da lista
 * de pontos ordenados em relação à coordenada y.
 */
Ponto *obterPontosOrdenadosEmYADireita(Ponto *pontos, Ponto pontoCentralEmX, const int POSICAO_PONTO_CENTRAL, const int COMECO, const int FIM)
{
  Ponto *pontosOrdenadosADireita = (Ponto *)malloc(sizeof(Ponto) * (FIM - POSICAO_PONTO_CENTRAL));
  for (int i = COMECO, posicaoADireita = 0; i < FIM || posicaoADireita < (FIM - POSICAO_PONTO_CENTRAL); i++)
    if (pontos[i].x >= pontoCentralEmX.x && (pontos[i].x != pontoCentralEmX.x || pontos[i].y >= pontoCentralEmX.y))
      pontosOrdenadosADireita[posicaoADireita++] = pontos[i];

  return pontosOrdenadosADireita;
}

/**
 * Método responsável pela obtenção dos pontos contidos na faixa central
 * de pontos do plano, definidos os limites inferior e superior para a faixa
 * a partir da menor distância já calculada.
 */
Ponto *obterFaixaCentralDePontosOrdenada(Ponto *pontos, const int COMECO, const int FIM, const int QUANTIDADE_PONTOS_FAIXA, const double MENOR_DISTANCIA)
{
  const int QUANTIDADE_PONTOS = FIM - COMECO;
  const int X_CENTRAL = pontos[QUANTIDADE_PONTOS / 2].x;
  Ponto *faixaCentralDePontos = (Ponto *)malloc(sizeof(Ponto) * QUANTIDADE_PONTOS_FAIXA);

  for (int i = COMECO, j = 0; i < FIM && j < QUANTIDADE_PONTOS_FAIXA; i++)
    if (abs(pontos[i].x - X_CENTRAL) < MENOR_DISTANCIA)
      faixaCentralDePontos[j++] = pontos[i];

  return faixaCentralDePontos;
}

/**
 * Método responsável pela leitura e armazenamento das coordenadas
 * recebidas em pontos.
 */
Ponto *obterPontos(const int QUANTIDADE_PONTOS)
{
  Ponto *pontos = (Ponto *)malloc(sizeof(Ponto) * QUANTIDADE_PONTOS);
  for (int i = 0; i < QUANTIDADE_PONTOS; i++)
    pontos[i] = *(new Ponto(obterCoordenada(), obterCoordenada()));

  return pontos;
}

Ponto *obterParPontosDeMenorDistancia(Ponto *primeiroPar, Ponto *segundoPar)
{
  const double DISTANCIA_PRIMEIRO_PAR = calcularDistanciaEntrePontos(primeiroPar[0], primeiroPar[1]);
  const double DISTANCIA_SEGUNDO_PAR = calcularDistanciaEntrePontos(segundoPar[0], segundoPar[1]);

  return DISTANCIA_PRIMEIRO_PAR < DISTANCIA_SEGUNDO_PAR ? primeiroPar : segundoPar;
}

/**
 * Método responsável pela obtenção dos pontos ordenados em função
 * de sua coordenada x com base no algoritmo Selection Sort. Caso x
 * seja igual, ordena-se em função de y.
 * Complexidade: O(n²).
 */
Ponto *obterPontosOrdenadosEmFuncaoDeX(Ponto *pontos, const int QUANTIDADE_PONTOS)
{
  Ponto *pontosOrdenados = (Ponto *)malloc(sizeof(Ponto) * QUANTIDADE_PONTOS);
  for (int i = 0; i < QUANTIDADE_PONTOS; i++)
    pontosOrdenados[i] = *(new Ponto(pontos[i].x, pontos[i].y));

  for (int i = 0; i < QUANTIDADE_PONTOS - 1; i++)
  {
    int posicaoMenorX = i;
    double menorX = pontosOrdenados[i].x;
    for (int j = i + 1; j < QUANTIDADE_PONTOS; j++)
    {
      if (pontosOrdenados[j].x < menorX)
      {
        menorX = pontosOrdenados[j].x;
        posicaoMenorX = j;
      }
    }

    trocarPontosDePosicao(&pontosOrdenados[i], &pontosOrdenados[posicaoMenorX]);
  }

  for (int i = 0; i < QUANTIDADE_PONTOS - 1; i++)
    if (pontosOrdenados[i].x == pontosOrdenados[i + 1].x && pontosOrdenados[i].y > pontosOrdenados[i + 1].y)
      trocarPontosDePosicao(&pontosOrdenados[i], &pontosOrdenados[i + 1]);

  return pontosOrdenados;
}

/**
 * Método responsável pela obtenção dos pontos ordenados em função
 * de sua coordenada y (em caso de coordenada y distinta entre pontos)
 * com base no algoritmo Selection Sort. Caso y seja igual, ordena-se
 * em função de x.
 * Complexidade: O(n²).
 */
Ponto *obterPontosOrdenadosEmFuncaoDeY(Ponto *pontos, const int QUANTIDADE_PONTOS)
{
  Ponto *pontosOrdenados = (Ponto *)malloc(sizeof(Ponto) * QUANTIDADE_PONTOS);
  for (int i = 0; i < QUANTIDADE_PONTOS; i++)
    pontosOrdenados[i] = *(new Ponto(pontos[i].x, pontos[i].y));

  for (int i = 0; i < QUANTIDADE_PONTOS - 1; i++)
  {
    int posicaoMenorY = i;
    double menorY = pontosOrdenados[i].y;
    for (int j = i + 1; j < QUANTIDADE_PONTOS; j++)
    {
      if (pontosOrdenados[j].y < menorY)
      {
        menorY = pontosOrdenados[j].y;
        posicaoMenorY = j;
      }
    }

    trocarPontosDePosicao(&pontosOrdenados[i], &pontosOrdenados[posicaoMenorY]);
  }

  for (int i = 0; i < QUANTIDADE_PONTOS - 1; i++)
    if (pontosOrdenados[i].y == pontosOrdenados[i + 1].y && pontosOrdenados[i].x > pontosOrdenados[i + 1].x)
      trocarPontosDePosicao(&pontosOrdenados[i], &pontosOrdenados[i + 1]);

  return pontosOrdenados;
}

/**
 * Método responsável pela obtenção da quantidade de pontos que estarão
 * contidos na faixa central. Este método foi utilizado pois não conseguíamos
 * acessar a quantidade de elementos contidos na faixa central apenas por meio
 * do vetor construído, sendo um ponto de melhoria no código.
 */
int obterQuantidadePontosNaFaixa(Ponto *pontos, const int COMECO, const int FIM, const double MENOR_DISTANCIA)
{
  const int QUANTIDADE_PONTOS = FIM - COMECO;
  const int X_CENTRAL = pontos[QUANTIDADE_PONTOS / 2].x;
  int quantidadePontosNaFaixa = 0;

  for (int i = COMECO; i < FIM; i++)
    if (abs(pontos[i].x - X_CENTRAL) < MENOR_DISTANCIA)
      quantidadePontosNaFaixa++;

  return quantidadePontosNaFaixa;
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