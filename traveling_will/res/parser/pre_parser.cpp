#include <bits/stdc++.h>

using namespace std;

int main(int argc, char * argv[]){
    string s;
    char c1[40000];

    if(argc == 2){
        while(getline(cin, s)){
            if(s[2] != '|')
                cout << s << endl;
        }

        return 0;
    }

    if(argc == 3){
        char c;
        int printa = 0;
        while((c = getchar()) != EOF){
            if(printa == 0){
                if(c == '<')
                    printa++;
                else
                    printf("%c", c);
            }else{
                if(c == '>')
                    printa = 0;
                else if(printa == 1){
                    printf("%c", c);
                    while(c = getchar(), c != ' '){
                        printf("%c", c);
                    }
                    printa++;
                }
            }
        }

        return 0;
    }

    int t = 4;
    while(t--){
        getline(cin, s);
        cout << s << endl;
    }

    int num, mult;
    double den;
    while(cin >> s){
        sscanf(s.c_str(), "%[^0-9]%d*%d/%lf", c1, &mult, &num, &den);
        
        int res = round(mult*num/den);
        if(res)
            printf("%s%d ", c1, res);
    }

    return 0;
}