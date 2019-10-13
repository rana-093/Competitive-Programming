#include <bits/stdc++.h>

using namespace std;

#define maxn 1000008
#define ll long long int
#define db(x) cout<<#x<<" -> "<<x<<" "<<endl
#define db2(x,y) cout<<#x<<"-> "<<x<<", "<<#y<<" -> "<<y<<endl
#define db3(x,y,z) cout<<#x<<" -> "<<x<<" , "<<#y<<" -> "<<y<<" , "<<#z<<" -> "<<z<<endl
#define prt(x) for(auto it = x.begin(); it!=x.end(); it++) { cout<<*it; } cout<<endl
#define IN freopen("input.txt","r",stdin)

const int inf = (int)1e7;

set< int >g[ maxn ];
int lvl[ maxn ] , P[maxn][21] , sub[ maxn ] ;
int parent[ maxn ] , ans[ maxn ];
int nn = 0 ; ///# of nodes

void dfs_lca(int src, int l){
    lvl[ src ] = l;
    for(auto i : g[ src ]){
        if(lvl[ i ]==-1){
            parent[ i ] = src;
            dfs_lca(i,l+1);
        }
    }
}

void preprocess_lca(int root, int N){
    memset(parent, -1, sizeof(parent));
    memset(lvl, -1, sizeof(lvl));
    dfs_lca(root, 0);
    for(int i = 0 ; i <= N ; ++i)  P[i][0] = parent[i];

    for(int j = 1; (1<<j) < N; ++j){
        for(int i = 1; i <= N; ++i){
            if(P[i][j-1] != -1){
                P[i][j] = P[P[i][j-1]][j-1] ;
            }
        }
    }
}

int lca(int u , int v)
{
	if(lvl[u] < lvl[v])
	swap(u , v);

	int log = 0;
	while( (1<<log) <= lvl[u]) log++;
	log -=1;

	for(int i= log; i >= 0; i--)
	{
		if( (lvl[u] - (1<<i)) >= lvl[v])
		{
			u = P[u][i];
		}
	}
	if(u == v) return u;
	for(int i= log; i >= 0; i--)
	{
		if(P[u][i]!=-1 && P[u][i] != P[v][i])
		{
			u = P[u][i];
			v = P[v][i];
		}
	}
	return P[u][0];
}

int dist(int u, int v){
    int l = lca(u,v);
    int ans = lvl[ u ] + lvl[ v ] - (2 * lvl[ l ] ); return ans;
}

void getSize(int src, int par){
    sub[ src ] = 1; nn++;
    for(auto i : g[ src ]){
        if(i==par){ continue ; }
        getSize(i,src);
        sub[ src ]+=sub[ i ];
    }
}

int getCentroid(int u, int par){
    for(auto v: g[ u ]){
        if(v!=par){
            if(sub[ v ] > nn/2){
                return getCentroid(v, u);
            }
        }
    }
    return u;
}

void decompose(int u, int par){ ///Call with (1,-1)
    nn = 0;
    getSize(u, -1);
    int centroid = getCentroid(u, -1);
    parent[ centroid ] = par;
    for(auto v: g[ centroid ]){
        g[v].erase( centroid );
        if(v != par){
            decompose(v, centroid);
        }
    }
    g[ centroid ].clear();
}


int main(){
    ios_base::sync_with_stdio(false)  ; cin.tie(0);
    IN;
    int n,q;
    cin>>n;
    for(int i = 1; i < n; i++){
        int u,v;
        cin>>u>>v;
        g[ u ].insert( v );
        g[ v ].insert( u );
    }
    for(int i = 0; i < maxn ; i++){ ans[ i ] = inf ; }
    preprocess_lca(1,n);
    decompose(1,-1);

    return 0;
}
