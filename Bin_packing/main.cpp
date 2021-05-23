#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

bool compare(const pair<int, int> &x, const pair<int, int> &y) {
    return (x.first > y.first);
}

int main() {
    int number_items = 0, capacity = 0;
    cin >> number_items >> capacity;
    vector<pair<int, int>> items(number_items);
    for (int i = 0; i < number_items; i++) {
        cin >> items[i].first;
        items[i].second = i;
    }
    sort(items.begin(), items.end(), compare);
    vector<int> bins(1);
    vector<int> items_bins(number_items);
    for(int i = 0; i < number_items; i++){
        int j = 0;
        while(capacity - bins[j]  < items[i].first){
            j++;
            if(bins.size() == j){
                bins.push_back(0);
            }
        }
        bins[j] += items[i].first;
        items_bins[items[i].second] = j+1;
    }
    for(int i = 0; i < number_items; i++){
        cout << items_bins[i] << ' ';
    }
    return 0;
}
