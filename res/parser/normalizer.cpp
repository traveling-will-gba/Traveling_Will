#include <bits/stdc++.h>

using namespace std;

int main(){
    fstream level_design("level_design.txt");
    int highs[100000], collectables[10000], values[2][10000], enemies[2][10000], high = 0, low = 100000, stand = 300;

    if(not level_design.is_open()){
        printf("Level design txt not available\n");
        exit(0);
    }

    string s;
    getline(level_design, s);
    cout << s << endl;
    getline(level_design, s);
    cout << s << endl;

    int i = 0;
    while(getline(level_design, s)){
        stringstream cim(s);
        cim >> highs[i];
        cim >> values[0][i];
        if(values[0][i])
            cim >> enemies[0][i] >> enemies[1][i];
        cim >> values[1][i];
        if(values[1][i])
            cim >> collectables[i];
        high = max(high, highs[i]);
        low = min(low, highs[i]);
        ++i;
    }

    double range = high - low;
    double diff = stand / range;

    int new_high = 0, new_low = 100000;
    for(int j = 0; j < i; ++j){
        highs[j] = 300*((highs[j]-low)/range) + 50;
        collectables[j] = highs[j] + 40;
        new_high = max(new_high, highs[j]);
        new_low = min(new_low, highs[j]);
    }

    for(int j = 0; j < i; ++j){
        printf("%d 0 %d", highs[j], values[1][j]);
        if(values[1][j])
            printf(" %d", collectables[j]);
        printf("\n");
    }
    printf("%d, %d %f\n", low, high, diff);
    printf("%d, %d\n", new_low, new_high);

    level_design.close();

    return 0;
}
