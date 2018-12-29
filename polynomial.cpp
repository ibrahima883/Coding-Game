#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <iterator>
#include <exception>

using namespace std;

/**
 * Auto-generated code below aims at helping you parse
 * the standard input according to the problem statement.
 **/
void split(const string& poly, string& sub1, string& sub2)
{ 
    // ex: poly = (x+1)(2x^3-1) ...(x^4-x^2+6x+5) ou poly = (x+1)^2
    const auto pos_closed_par = poly.find_first_of(')');
    sub1 = poly.substr(1, pos_closed_par - 1);
    auto pos = pos_closed_par + 1;
    sub2 = (pos + 1 < poly.size()) ? poly.substr(pos) : "";
}
void getCoefAndDeg(const string& expanded_poly, vector<int>& coefs)
{
    int degree(0), count(0);
    string rest(expanded_poly);
    while (!rest.empty())
    {
        const auto pos_x = rest.find_first_of('x');
        auto fin = rest.length();
        int coef(0), deg(0);
        if (pos_x != string::npos)
        {
            // getting coef
            if (pos_x > 0)
            {
                if (rest[0] == '-' && pos_x == 1)
                {
                    coef = -1;
                }
                else if (rest[0] == '+' && pos_x == 1)
                {
                    coef = 1;
                }
                else
                {
                    coef = stoi(rest.substr(0, pos_x));
                }
            } 
            else
                coef = 1;
                
            // getting degree  
            if (rest.length() > 2 && rest[pos_x+1] == '^')
            {
                // first position of sign
                auto pos_minus = rest.find_first_of('-');
                auto pos_plus = rest.find_first_of('+');
                auto pos_sign = min(pos_plus, pos_minus);
                if (pos_sign != string::npos)
                {
                    // second position of sign
                    pos_minus = rest.find('-', pos_x+3);
                    pos_plus = rest.find('+', pos_x+3);
                    pos_sign = min(pos_plus, pos_minus);
                    if (pos_sign != string::npos)
                        fin = pos_sign;
                    deg = stoi(rest.substr(pos_x+2, fin));
                }
                else // ex: poly = x^2
                    deg = stoi(rest.substr(pos_x+2));
            }
            else // ex: poly = x ou poly = x+4
            {
                deg = 1;
                fin = pos_x+1;
            }
        }
        else // ex: poly = -23 or poly = +5;
        {
            coef = (expanded_poly[0] != '+') ? stoi(rest) : stoi(rest.substr(1, fin));
        }
        
        // get degree of poly in the first round et initialise the coeffs to 0
        if (count == 0)
        {
            degree = deg;
            coefs.resize(degree+1, 0);
        }
        try
        {
            coefs.at(degree-deg) = coef;
        }
        catch(const exception& e)
        {
            cerr << "ERROR: " << e.what() << endl;
        }
        rest.erase(0, fin);
        ++count;
    }
}
void buildPolynome(vector<int> coefs, string& polynome)
{
    polynome = "";
    int size = coefs.size();
    for(int i = 0; i < size; i++)
    {
        string monome = "";
        int coef = coefs[i];
        if (coef != 0)
        { 
            string sign = coef > 0 ? "+" : "-";
            if (i < size-1) 
            {
                monome = (i == size -2) ? "x" : "x^" + to_string(size-i-1);
                if (abs(coef) != 1)
                {
                    monome.insert(0, to_string(abs(coef)));
                }
                if (i > 0 || (coef == -1 && i == 0))
                {
                    monome.insert(0, sign);
                }
            }
            else // i == size - 1 donc monome constant
            {
                monome = sign + to_string(abs(coef));
                //if (coef > 0) monome.insert(0, 1, '+');
            }
            polynome += monome;
        }
    }
}
void multiply(const string& poly1, const string& poly2, string& result)
{
    if (poly1 == "0" || poly2 == "0")
        result = "0";
    else if (poly1 == "1")
        result = poly2;
    else if (poly2 == "1")
        result = poly1;
    else
    {
        vector<int> coefs1, coefs2;
        getCoefAndDeg(poly1, coefs1);
        getCoefAndDeg(poly2, coefs2);
        
        size_t result_deg = coefs1.size() + coefs2.size() - 2;
        vector<int> result_coefs(result_deg+1, 0);
        for (size_t k(0); k <= result_deg; k++)
        {
            
            for (size_t i(0); i <= k; i++)
            {
                if (i < coefs1.size() && k-i < coefs2.size())
                    result_coefs[k] += coefs1[i] * coefs2[k-i];
            }
        }
        
        buildPolynome(result_coefs, result);
    }
}

int main()
{
    string poly;
    cin >> poly ; cin.ignore();
    string expandedpolynomial(poly);
    const auto number_open_parenthesis = count(begin(poly), end(poly), '(');
    const auto number_closed_parenthesis = count(begin(poly), end(poly), ')');
    if (number_open_parenthesis == number_closed_parenthesis) // potentially it's a correct poly
    {
        if (number_open_parenthesis > 1)
        {
            string rest_of_poly, sub;
            copy_if(begin(poly), end(poly), back_inserter(rest_of_poly), [](const char& c){return c!='*';});
            split(rest_of_poly, expandedpolynomial, rest_of_poly);
            do
            {
                split(rest_of_poly, sub, rest_of_poly);
                multiply(expandedpolynomial, sub, expandedpolynomial);
            } while (!rest_of_poly.empty());
        }
        else if (number_open_parenthesis == 1) // power case
        {
            string unused;
            split(poly, expandedpolynomial, unused);
            const auto index = poly.find(')');
            if (poly[index+1] == '^' && index+2 < poly.length())
            {
                const string initial_poly(expandedpolynomial);
                int puissance = stoi(poly.substr(index+2));
                for (int i(0); i < puissance-1; i++)
                {
                   multiply(expandedpolynomial, initial_poly, expandedpolynomial);
                }
            }
        }
    }
    cout << expandedpolynomial << endl;
}
