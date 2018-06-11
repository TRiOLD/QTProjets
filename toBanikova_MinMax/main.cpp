//////////////////////////////
#include <iostream>
using namespace std;

//////////////////////////////
///ЭТИ ПАРАМЕТРЫ МОЖНО МЕНЯТЬ!
const int V = 5;    //Кол-во комплексных обедов
const int Q = 3;    //Кол-во блюд в комплексном обеде

//////////////////////////////
struct Dish         //Структура "Блюдо"
{
    int N;              //Номер "блюда"
    int M;              //Номер "Обеда" где это "Блюдо"
    float caloricity;   //Калорийность блюда
    float price;        //Цена блюда
};

struct Dinner       //Структура "Обед"
{
    int N;              //Номер обеда
    Dish dishes[Q];     //Состоит из Q блюд
    float fullCalor;    //Сумма всех калорий обеда
    float fullPrice;    //Общая цена за обед
};
//////////////////////////////
Dinner dinners[V];     //Обьявляем "комплексные обеды"

//////////////////////////////
void initDinners()   //Функция для рандомной инициализации "обедов"
{
    for( int v = 0; v < V; v++ )
    {
        for( int q = 0; q < Q; q++ )
        {
            float calor = (rand() % 10000) / 10.f;
            float price = (rand() % 2000) / 100.0f;
            dinners[v].dishes[q].caloricity = calor;
            dinners[v].dishes[q].price = price;
            dinners[v].fullCalor += calor;
            dinners[v].fullPrice += price;
            dinners[v].dishes[q].N = q + 1;
            dinners[v].dishes[q].M = v + 1;
            dinners[v].N = v + 1;
        }
        dinners[v].N = v + 1;
    }
}

void showDinners()   //Функция для отображения информации об "обедах"
{
    cout << "-- All possible dinners --" << endl;
    for( int v = 0; v < V; v++ )
    {
        cout << "Dinner #" << dinners[v].N << ":" << endl;
        for( int q = 0; q < Q; q++ )
        {
            cout << "\t" << dinners[v].dishes[q].N << ")" << " ";
            cout << "Calor = " << dinners[v].dishes[q].caloricity << ", ";
            cout << "Price = " << dinners[v].dishes[q].price << ";" << endl;
        }
        cout << "\t\t" << "FullCalor = " << dinners[v].fullCalor << ", ";
        cout << "FullPrice = " <<  dinners[v].fullPrice << ";" << endl;
    }
    cout << "-------------------------" << endl << endl;
}

//////////////////////////////
int main()
{
    if( !Q || !V )  return -1;  //Проверка на корректность "входных" данных

    initDinners();              //Забиваем случайными значениями параметры обедов и блюд
    showDinners();              //Выводим все на экран

    //Блок кода с переменными для хранения "минимальных и максимальных" обедов и блюд
    //(Правильно было бы сделать с указателями и ссылками, но так понятнее...)
    Dinner maxCalorDinner, minCalorDinner, maxPriceDinner, minPriceDinner;
    Dish maxCalorDish, minCalorDish, maxPriceDish, minPriceDish;
    maxCalorDinner = minCalorDinner = maxPriceDinner = minPriceDinner = dinners[0];
    maxCalorDish = minCalorDish = maxPriceDish = minPriceDish = dinners[0].dishes[0];

    for( int v = 0; v < V; v++ )//Тут сравнивается все со всем в поисках min & max
    {
        for( int q = 0; q < Q; q++ )
        {
            if( dinners[v].dishes[q].caloricity > maxCalorDish.caloricity )
                maxCalorDish = dinners[v].dishes[q];
            if( dinners[v].dishes[q].caloricity < minCalorDish.caloricity )
                minCalorDish = dinners[v].dishes[q];
            if( dinners[v].dishes[q].price > maxPriceDish.price )
                maxPriceDish = dinners[v].dishes[q];
            if( dinners[v].dishes[q].price < minPriceDish.price )
                minPriceDish = dinners[v].dishes[q];
        }
        if( dinners[v].fullCalor > maxCalorDinner.fullCalor )
            maxCalorDinner = dinners[v];
        if( dinners[v].fullCalor < minCalorDinner.fullCalor )
            minCalorDinner = dinners[v];
        if( dinners[v].fullPrice > maxPriceDinner.fullPrice )
            maxPriceDinner = dinners[v];
        if( dinners[v].fullPrice < minPriceDinner.fullPrice )
            minPriceDinner = dinners[v];
    }

    //Выводим на экран номера всего маленького и всего большого
    cout << "Maximum calories in Dinner #" << maxCalorDinner.N << endl;
    cout << "Minimum calories in Dinner #" << minCalorDinner.N << endl;
    cout << "Maximum price at Dinner #" << maxPriceDinner.N << endl;
    cout << "Minimum price at Dinner #" << minPriceDinner.N << endl;
    cout << "Maximum calories in Dish #" << maxCalorDish.N <<", Dinner # " << maxCalorDish.M << endl;
    cout << "Minimum calories in Dish #" << minCalorDish.N <<", Dinner # " << minCalorDish.M << endl;
    cout << "Maximum price at Dish #" << maxPriceDish.N <<", Dinner # " << maxPriceDish.M << endl;
    cout << "Minimum price at Dish #" << minPriceDish.N <<", Dinner # " << minPriceDish.M << endl;

    return 0;
}
