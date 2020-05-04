#include <set>
#include <map>
#include <iostream>
#include <vector>

void two_diff_set_with_same_elements()
{
    std::set<std::set<int> > s;

    std::set<int> s1;
    std::set<int> s2;

    s1.insert(2);
    s1.insert(1);
    s1.insert(3);
    s2.insert(1);
    s2.insert(2);
    s2.insert(3);

    s.insert(s1);

    auto res = s.find(s2);

    if (res != s.end())
    {
        printf("FIND it!!!!");
    }
    else
    {
        printf("NOT inside");
    }
}

void test_empty_map()
{
    std::map<int, int> m;
    m[1] = 3;
    m[4] = 2;

    // std::cout<<"For 1 is "<<m[1]<<std::endl;
    // std::cout<<"For 2 is "<<m[2]<<std::endl;
    // std::cout<<"For 9 is "<<m[9]<<std::endl;
    // std::cout<<set_to_string(&transit_table['1'])<<std::endl;
    std::map<char, std::set<int> > transit_table;
    std::set<int> s((transit_table['1']).begin(), (transit_table['1']).end());

    printf("-----\n");
    for (auto i : s)
    {
        printf("%d ", i);
    }
    printf("-----\n");
    printf("%p\n", &s);
    printf("%d\n", s.empty());
}
class S
{
public:
    std::set<int> s;
    S(std::set<int> s) : s(s){};
};

void vector_insert(){
    std::vector<S> st; // all transition for all sets
    std::set<int> s;

    s.insert(1);
    s.insert(2);

    //S ss(s);
    st.insert(st.begin(), S(s));
    printf("st is empty : %d", st.empty());
}
void empty_or_nonexist_set(){
    std::set<int> s;
    s.insert(1);
    s.erase(2);
    printf("---");
    s.erase(s.end());
}

int main()
{
    char buf[256];
    std::cin>>buf;
    std::cout<<buf;
    return 0;
}