#include<bits/stdc++.h>
using namespace std;

int min_c = INT_MAX;

int fun(int n, int k, int a[], int turn, vector<vector<vector<int>>> &dp) {
	if(k<0) return 0;
	if(dp[n][k][turn]!=-1) return dp[n][k][turn];

	if(a[k]<=n) {
		if(turn) return dp[n][k][turn] = max(a[k] + fun(n-a[k], k, a, !turn, dp), fun(n, k-1, a, turn, dp));
		else return dp[n][k][turn] = max(fun(n-a[k], k, a, !turn, dp), fun(n, k-1, a, turn, dp));
	} else {
		return dp[n][k][turn] = fun(n, k-1, a, turn, dp);
	}
}

int main() {
	int n, k;
	cin>>n>>k;

	int a[n];
	for(int i=0;i<n;i++) {
		cin>>a[i];
		min_c = min(min_c, a[i]);
	}

	vector<vector<vector<int>>> dp(n+1, vector<vector<int>>(k+1, vector<int>(2, -1)));
	cout<<fun(n, k, a, 1, dp)<<endl;

	return 0;
}