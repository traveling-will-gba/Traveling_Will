#include <bits/stdc++.h>

using namespace std;

int main(){
    fstream level_design("level_design.txt");
    int alturas[100000], coletaveis[10000], valo[2][10000], inimigos[2][10000], high = 0, low = 100000, stand = 380;

    if(not level_design.is_open()){
        printf("Level design txt not available\n");
        exit(0);
    }

    string s;
    string t[100000];
    getline(level_design, s);
    cout << s << endl;
    getline(level_design, s);
    cout << s << endl;

    int i = 0;
    int val;
    while(getline(level_design, s)){
        stringstream cim(s);
        cim >> alturas[i];
        cim >> valo[0][i];
        if(valo[0][i])
            cim >> inimigos[0][i] >> inimigos[1][i];
        cim >> valo[1][i];
        if(valo[1][i])
            cim >> coletaveis[i];
        // printf("%d%s\n", alturas[i], t[i].c_str());
        high = max(high, alturas[i]);
        low = min(low, alturas[i]);
        ++i;
    }

    double range = high - low;
    double diff = stand / range;

    int new_high = 0, new_low = 100000;
    for(int j = 0; j < i; ++j){
        alturas[j] = 380*((alturas[j]-low)/range) + 50;
        coletaveis[j] = 380*((coletaveis[j]-low)/range) + 50;
        new_high = max(new_high, alturas[j]);
        new_low = min(new_low, alturas[j]);
    }

    for(int j = 0; j < i; ++j){
        printf("%d 0 %d", alturas[j], valo[1][j]);
        if(valo[1][j])
            printf(" %d", coletaveis[j]);
        printf("\n");
    }
    printf("%d, %d %f\n", low, high, diff);
    printf("%d, %d\n", new_low, new_high);

    level_design.close();

    return 0;
}