#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

bool compare( const pair<int, int>& x, const pair<int, int>& y )
{
    return ((double) x.second / x.first > (double) y.second / y.first);
}

int greedy_method(int pos, int total_weight, int number_items, vector<pair<int, int>> &items_list)
{
    int ind = pos, sum_weight = 0, sum_value = 0;
    while(sum_weight < total_weight)
    {
        if(ind == number_items)
            break;
        if(items_list[ind].first <= total_weight)
        {
            sum_weight += items_list[ind].first;
            sum_value += items_list[ind].second;
        }
        ind++;
    }
    if(sum_weight > total_weight)
    {
        sum_weight -= items_list[ind-1].first;
        sum_value -= items_list[ind-1].second;
    }
    if(ind > pos && sum_value < items_list[ind-1].second && items_list[ind-1].first <= total_weight)
    {
        sum_weight = items_list[ind-1].first;
        sum_value = items_list[ind-1].second;
    }
    return sum_value;
}

double UpBest(int pos, int total_weight, int number_items, vector<pair<int, int>> &items_list)
{
    int ind = pos, sum_weight = 0, sum_value = 0;
    double up_sum_value;
    while(sum_weight < total_weight)
    {
        if(ind == number_items)
            break;
        if(items_list[ind].first <= total_weight)
        {
            sum_weight += items_list[ind].first;
            sum_value += items_list[ind].second;
        }
        ind++;
    }
    up_sum_value = sum_value;
    double dif_weight;
    if(sum_weight > total_weight)
    {
        sum_weight -= items_list[ind-1].first;
        sum_value -= items_list[ind-1].second;
        dif_weight = (double) total_weight - sum_weight;
        up_sum_value = sum_value + ((double) dif_weight/items_list[ind-1].first) * items_list[ind-1].second;
    }
    return up_sum_value;
}

int recursion(int pos, int curW, int curP, int &bestP, int total_weight, int number_items, vector<pair<int, int>> &items_list)
{
    if(curW > total_weight)
        return 0;
    if(curP > bestP)
        bestP = curP;
    if(pos == number_items)
        return 0;
    else
    {
        if(curP + UpBest(pos, total_weight - curW, number_items, items_list) > bestP)
            recursion(pos+1, curW + items_list[pos].first, curP + items_list[pos].second, bestP, total_weight, number_items, items_list);
        if(curP + UpBest(pos+1, total_weight - curW, number_items, items_list) > bestP)
            recursion(pos+1, curW, curP, bestP, total_weight, number_items, items_list);
    }
}

int main()
{
    int total_weight, number_items;
    cin >> total_weight;
    cin >> number_items;
    vector <pair<int,int>> items_list;
    int deleted_items = 0;
    int a, b;
    for(int i = 0; i < number_items; i++)
    {
        cin >> a >> b;
        if(a <= total_weight)
            items_list.push_back(make_pair(a,b));
        else
            deleted_items++;
    }
    number_items -= deleted_items;
    sort(items_list.begin(), items_list.end(), compare);
    int bestP = greedy_method(0,total_weight, number_items, items_list);
    recursion(0, 0, 0, bestP, total_weight, number_items, items_list);
    cout << bestP;
    return 0;
}
