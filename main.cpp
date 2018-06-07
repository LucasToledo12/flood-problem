// Grafos - Lista de adjacência

#include <iostream>
#include <list>
#include <fstream>
#include <chrono>
using namespace std::chrono;
using namespace std;
//int newpiv = 0;
class Grafo
{
	int V; // número de vértices
	list<int> *adj; // ponteiro para um array contendo as listas de adjacências
	int* cores;//array com as cores de cada vertice do grafo
	bool *verifV;
	int*pivos;
	int numpivos;
	bool *inundV;
public:
	Grafo(int V); // construtor
	void adicionarAresta(int v1, int v2); // adiciona uma aresta no grafo
	// obtém o grau de saída de um dado vértice
	// grau de saída é o número de arcos que saem de "v"
	int obterGrauDeSaida(int v);
	void adicionarCor(int indexV, int cor);//adiciona uma cor no vertice desejado
	int retornaCor(int indexV);//retorna a cor do vertice
	bool VerificaVertices();
	void inundacao(int pivo, int numCores);
	int inundarPivo(int pivo, int corPInundar, int corPivo);
};

Grafo::Grafo(int V)
{
	this->V = V; // atribui o número de vértices
	adj = new list<int>[V]; // cria as listas
	cores = new int[V];
	verifV = new bool[V];
	for (int i = 0; i < V; i++) verifV[i] = false;
	pivos = new int[V];
	numpivos = 0;
	inundV = new bool[V];
	for (int i = 0; i < V; i++) inundV[i] = false;

}

void Grafo::adicionarAresta(int v1, int v2)
{
	// adiciona vértice v2 à lista de vértices adjacentes de v1
	adj[v1].push_back(v2);
}

int Grafo::obterGrauDeSaida(int v)
{
	// basta retornar o tamanho da lista que é a quantidade de vizinhos
	return adj[v].size();
}

void Grafo::adicionarCor(int indexV, int cor) {
	cores[indexV] = cor;
}

int Grafo::retornaCor(int indexV) {
	return cores[indexV];
}

bool Grafo::VerificaVertices() {
	for (int i = 0; i < V; i++) {
		if (!verifV[i]) return true;
	}
	return false;
}

void Grafo::inundacao(int pivo, int numCores) {
	pivos[0] = pivo;
	for (int i = 0; i < V; i++) {//printa todos os vertices pos inundacao
		cout << retornaCor(i) << " ";
	}
	int corPiv = cores[pivo];
	int pivaux = pivo;
	int corPInundar = 0;
	int passos = 0;
	numpivos++;
	int *coresAux = new int[numCores];
	for (list<int>::iterator it = adj[pivaux].begin(); it != adj[pivaux].end(); ++it) {
		int iter = *it;
		if (cores[iter] != corPiv) {
			corPInundar = cores[iter];
			break;
		}
	}
	while (numpivos < V) {
		int auxNum = numpivos;
		int cont = 0;
		while (cont < auxNum) {
			if (inundV[pivos[cont]] == false) {
				//verifV[pivos[cont]] == true;
				auxNum = inundarPivo(pivos[cont], corPInundar, corPiv);
			}
			cont++;
		}
		//if (inundV[pivaux] == true) {
			//for (int i = 0; i < numpivos; i++) {
				//if (inundV[pivos[i]] == false) {
					//pivaux = pivos[i];
					//break;
				//}
			//}
		//}
		//for (int i = 0; i < V; i++) {//printa todos os vertices pos inundacao
		//	cout << retornaCor(i) << " ";
		//}
		//for (int i = 0; i < numpivos; i++) {
			//cout << endl;
			//cout << pivos[i] << " ";
		//}
		//for (int i = 0; i < V; i++) {
			//verifV[i] = false;
		//}
		cout << endl;
		for (int i = 0; i < numCores; i++) {
			coresAux[i] = 0;
		}
		for (int i = 0; i < numpivos; i++) {
			if (inundV[pivos[i]] == false) {
				for (list<int>::iterator it = adj[pivos[i]].begin(); it != adj[pivos[i]].end(); ++it) {
					int iter = *it;
					if (cores[iter] != corPiv) {
						coresAux[cores[iter] - 1]++;
					}
				}
			}
		}
		int maior = 0;
		for (int i = 0; i < numCores; i++) {
			if (coresAux[i] > maior) {
				maior = i;
			}
		}
		corPInundar = maior + 1;
		passos++;
		//cout << "a " << passos << endl;
	}
	cout << "passos " << passos << endl;
}
int Grafo::inundarPivo(int pivo, int corPInundar, int corPivo) {
	int grauPivo = obterGrauDeSaida(pivo);
	int newp = 0;
	bool verif = false;
	int posi = pivo;
	if (grauPivo > 0) {
		for (list<int>::iterator it = adj[pivo].begin(); it != adj[pivo].end(); ++it) {
			int iter = *it;
			bool verifIgual = false;
			if (cores[iter] == corPivo) {
				for (int i = 0; i < numpivos; i++) {
					if (iter == pivos[i]) {
						verifIgual = true;
						break;
					}
				}
				if (verifIgual == false) {
					pivos[numpivos] = iter;
					numpivos++;
				}
			}
			if (cores[iter] != corPivo) {
				if (cores[iter] == corPInundar) {
					cores[iter] = corPivo;
					for (int i = 0; i < numpivos; i++) {
						if (iter == pivos[i]) {
							verifIgual = true;
							break;
						}
					}
					if (verifIgual == false) {
						pivos[numpivos] = iter;
						numpivos++;
					}
					inundarPivo(iter, corPInundar, corPivo);
				}
			}
			if (cores[iter] != corPInundar && cores[iter] != corPivo) {
				newp++;
			}
		}
	}
	if (newp == 0) inundV[pivo] = true;
	return numpivos;
}

int main()
{
	int numVertices, numArestas, numCores, pivo;
	ifstream dados;
	dados.open("1.txt");
	if (!dados) {
		cout << "ARQUIVO INEXISTENTE!" << endl;
	}
	dados >> numVertices;
	Grafo grafo(numVertices);
	dados >> numArestas;
	dados >> numCores;
	dados >> pivo;
	int cor;
	for (int i = 0; i<numVertices; i++) {
		dados >> cor;
		grafo.adicionarCor(i, cor);
	}
	int Vorigem, Vdestino;
	for (int i = 0; i<numArestas; i++) {
		dados >> Vorigem;
		dados >> Vdestino;
		grafo.adicionarAresta(Vorigem-1, Vdestino-1);
		grafo.adicionarAresta(Vdestino-1, Vorigem-1);
	}
	dados.close();
	cout << "DADOS IMPORTADOS DE 'CLIENTES.TXT' COM SUCESSO!" << endl;
	//calcula o tempo
	duration<double> time_span2;
	high_resolution_clock::time_point t1 = high_resolution_clock::now();
	grafo.inundacao(pivo-1, numCores);//inunda o grafo
	high_resolution_clock::time_point t2 = high_resolution_clock::now();
	duration<double> time_span = duration_cast<duration<double> >(t2 - t1);
	cout << "Tempo: "<< time_span.count() << endl;
	cout << "Grafo inundado!" << endl;
	for (int i = 0; i < numVertices; i++) {//printa todos os vertices pos inundacao
		cout << grafo.retornaCor(i) << " ";
	}
	cout << endl;
	return 0;
}