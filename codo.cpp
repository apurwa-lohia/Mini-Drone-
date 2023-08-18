#include<bits/stdc++.h>
using namespace std;

int main() {
	int n;
	cin>>n;

	string s, t;
	cin>>s;
	cin>>t;

	int cnt = 0;
	for(int i=0;i<n;i++) {
		if(s[i]!=t[i]) {
			cnt++;
		}
	}

	if(cnt&1) {
		cout<<-1<<endl;
		return 0;
	}

	string ans;
	int p1 = 0, p2 = 0;
	for(int i=0;i<n;i++) {
		if(s[i]=='0' && t[i]=='0') ans.push_back('0');
		else if(s[i]=='1' && t[i]=='0') {
			if(p2<(cnt/2)) {
				ans.push_back('0');
				p2++;
			} else {
				ans.push_back('1');
				p1++;
			}
		} else if(s[i]=='0' && t[i]=='1') {
			if(p1<(cnt/2)) {
				ans.push_back('0');
				p1++;
			} else {
				ans.push_back('1');
				p2++;
			}
		} else {
			ans.push_back('0');
		}
	}

	cout<<ans<<endl;

	return 0;
}