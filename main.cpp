#include "src/models/Ray.h"
#include <vector>
int main(){
    vector<int> d{1, 2, 3};
    vector<int> o{0, 0, 0};
    int t = 5;
    Ray ray = new Ray(d = d, o = o, t = t);
    cout << ray.toString();

}