#include<iostream.h>
#include<vector.h>
#include<map.h>
#include<algorithm.h>
#include<fstream.h>
#include<sstream.h>
#include<cmath.h>
using namespace std;

typedef  pair<int, int> iPair;
 
struct DisjointSets{
  int *parent, *rnk;
  int n;
  DisjointSets(int n){
  	this->n = n;
    parent = new int[n+1];
    rnk = new int[n+1];
    for (int i = 0; i <= n; i++){
    	rnk[i] = 0;
      parent[i] = i;
    }
  }
  int find(int u){
  	if (u != parent[u]) parent[u] = find(parent[u]);
    return parent[u];
  }
  void merge(int x, int y){
  	x = find(x), y = find(y);
    if (rnk[x] > rnk[y]) parent[y] = x;
    else parent[x] = y;
    if (rnk[x] == rnk[y]) rnk[y]++;
  }
};
 
struct Graph{
	int V, E;
  vector< pair<float, iPair> > edges;
  Graph(int V, int E){
  	this->V = V;
    this->E = E;
  }
  void addEdge(int u, int v, float w){
  	edges.push_back({w, {u, v}});
  }
 	
 	void MaximalSubTree(float lambda){
   	sort(edges.rbegin(), edges.rend());
   	DisjointSets ds(V);
   	vector< pair<float, iPair> >::iterator it;
   	for (it=edges.begin(); it!=edges.end(); it++){
     	int u = it->second.first;
     	int v = it->second.second;
     	int set_u = ds.find(u);
     	int set_v = ds.find(v);
     	if (set_u != set_v){
				if(it->first>=lambda){
					cout <<"{"<<u<<","<<v<<"} -> "<<it->first<<endl;
     			ds.merge(set_u, set_v);
				}
     	}
   	}
	}
};
 
class FuzzyClustering{
private:
	vector< vector<float> > dataset;
	vector< vector<float> > fuzzy_similar_matrix;
public:
	void getDataSet(char *fileName){
		vector<float> *temp;
		ifstream  data(fileName);
    string line;
    while(getline(data,line)){
        stringstream lineStream(line);
        string cell;
				temp=new vector<float>;
        while(getline(lineStream,cell,',')){
					temp->push_back(atof(cell.c_str()));
        }
				dataset.push_back(*temp);
    }
	}

	void printDataSet(){
		int i,j;
		for(i=0;i<dataset.size();i++){
			cout<<"Row "<<i+1<<" : (";
			for(j=0;j<dataset[i].size()-1;j++){
				cout<<dataset[i][j]<<",";
			}
			cout<<dataset[i][j]<<")"<<endl;
		}
	}
	void getClassifierRows(){
		int n;
		float temp_attr;
		vector<float> *temp;
		cout<<"Enter no. of classifier rows :";
		cin>>n;
		for(int i=0;i<n;i++){
			temp=new vector<float>;
			for(int j=0;j<dataset[0].size();j++){
				cin>>temp_attr;
				temp->push_back(temp_attr);	
			}
			dataset.push_back(*temp);
		}
	}	

	void getStandardDeviationTransformationData(){
		vector<float> mean,standard_deviation;
		float sum;
		for(int j=0;j<dataset[0].size();j++){
			sum=0.0;
			for(int i=0;i<dataset.size();i++){
				sum+=dataset[i][j];
			}
			mean.push_back(sum/dataset.size());
			sum=0.0;
			for(int i=0;i<dataset.size();i++){
				sum+=pow((dataset[i][j]-mean[j]),2);
			}
			standard_deviation.push_back(sqrt(sum/dataset.size()));
			for(int i=0;i<dataset.size();i++){
				dataset[i].at(j)=(dataset[i].at(j)-mean.at(j))/standard_deviation.at(j);
			}
		}
	}

	void getRangeTransformationData(){
		vector<float> *temp;
		float maxele,minele;
		for(int j=0;j<dataset.at(0).size();j++){
			minele=100;
			maxele=-100;
			for(int i=0;i<dataset.size();i++){
				maxele=dataset.at(i).at(j)>maxele ? dataset.at(i).at(j) : maxele;
				minele=dataset.at(i).at(j)<minele ? dataset.at(i).at(j) : minele;
			}
			for(int i=0;i<dataset.size();i++){
				dataset.at(i).at(j)=(dataset.at(i).at(j)-minele)/(maxele-minele);
			}
		}	
	}

	void generateFuzzySimilarDataSheet(){
		vector<float> *temp;
		float minsum,maxsum;
		for(int i=0;i<dataset.size();i++){
			temp=new vector<float>;
			for(int j=0;j<dataset.size();j++){
				if(i<j){
					minsum=0.0;
					maxsum=0.0;
					for(int k=0;k<dataset.at(0).size();k++){
						minsum+=(dataset.at(i).at(k)<dataset.at(j).at(k) ? dataset.at(i).at(k) : dataset.at(j).at(k));
						maxsum+=(dataset.at(i).at(k)>dataset.at(j).at(k) ? dataset.at(i).at(k) : dataset.at(j).at(k));
					}
					temp->push_back(minsum/maxsum);
				}
			}
			fuzzy_similar_matrix.push_back(*temp);
		}
	}
	
	void printFuzzySimilarDataSheet(){
		for(int i=0;i<fuzzy_similar_matrix.size();i++){
			for(int j=0;j<fuzzy_similar_matrix.at(i).size();j++){
				cout<<fuzzy_similar_matrix.at(i).at(j)<<"|";
			}
			cout<<endl;
		}
	}
	void createFuzzySimilarMaximalSubTree(float lambda){
		int num_nodes = fuzzy_similar_matrix.size()-1;
		int num_edges = num_nodes*(num_nodes-1)/2;
		Graph g(num_nodes,num_edges);
		for(int i=0;i<fuzzy_similar_matrix.size();i++){
			for(int j=0;j<fuzzy_similar_matrix.at(i).size();j++){
				g.addEdge(i+1,i+j+2,fuzzy_similar_matrix.at(i).at(j));	
			}	
		}
		g.MaximalSubTree(lambda);	
	}
};

int main(){
	char fname[10];
	float lambda;
	FuzzyClustering fc;
	cout<<"Enter filename : ";
	cin>>fname;
	fc.getDataSet(fname);
	cout<<"Original Dataset"<<endl;
	fc.printDataSet();
	fc.getClassifierRows();
	cout<<"Dataset along with evaluation rows"<<endl;
	fc.printDataSet();
	fc.getStandardDeviationTransformationData();
	cout<<"SD Transformed Data"<<endl;
	fc.printDataSet();
	cout<<"Range Transformed Data"<<endl;
	fc.getRangeTransformationData();
	fc.printDataSet();
	fc.generateFuzzySimilarDataSheet();
	fc.printFuzzySimilarDataSheet();
	cout<<"Enter the value of lambda : ";
	cin>>lambda;
	fc.createFuzzySimilarMaximalSubTree(lambda);
	return 0;
}
