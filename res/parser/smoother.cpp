#include <bits/stdc++.h>

using namespace std;

int main(){
    int a, b, c, d = 0;
    int v[100000][4];
    int count = 0, tot;

    cin >> a >> tot >> c;
    printf("%d\n%d %d\n", a, tot, c);

    while(cin >> a >> b >> c){
        if(c) cin >> d;
        v[count][0] = a;
        v[count][1] = b;
        v[count][2] = c;
        v[count++][3] = d;
    }

    for(int i = 0; i < count; ++i){
        if(i){
            if(abs(v[i-1][0] - v[i][0]) <= 60){
                v[i][0] = min(v[i-1][0], v[i][0]); 
            }
        }

        if(i != count - 1){
            if(abs(v[i+1][0] - v[i][0]) <= 60){
                v[i][0] = min(v[i+1][0], v[i][0]); 
            }
        }

        printf("%d %d %d", v[i][0], v[i][1], v[i][2]);
        if(v[i][2]){
            printf(" %d\n", v[i][3]);
        }else{
            printf("\n");
        }
    }

    return 0;
}