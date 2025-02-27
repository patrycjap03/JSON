#include <cstdint>
#include <cstdio>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>



using namespace std;

void operation_h();
void operation_n();
void operation_c();
//funkcje do operacji c *****

 
bool check_obj(fstream& file);
bool check_val(fstream& file);
bool check_val(fstream& file, bool);
bool check_dict(fstream& file);
bool check_arr(fstream& file);
bool check_bool(fstream& file);
bool check_str(fstream& file);
bool check_num(fstream& file);

char next_no_ws(fstream& file) {
    file >> ws;
    return file.peek();
}

bool check_num(fstream& file) {
    char current_ch; bool minus = false, firstDigit = true, decimal = false; 


    file >> skipws >> current_ch;
    

    
    if ((current_ch < '0' || current_ch > '9') && current_ch != '-')
        return false;
    if (current_ch != '-') {
        firstDigit = false;
    }
    if (current_ch == '-') {
        if (file.peek() == EOF || file.peek()< '0' || file.peek()> '9') {
            return false;
        }
    }
    if (current_ch == '0' && file.peek()!= '.' && (file.peek()>= '0' && file.peek()<= '9'))
        return false;

    if (!firstDigit &&file.peek()!= '.' && (file.peek() < '0' || file.peek() > '9'))
        return true;

    while (true) {
        
        if (file.peek() == EOF)
            break;
        
        file >> noskipws >>  current_ch;
         


        if (firstDigit && (current_ch < '0' || current_ch > '9'))
            return false;

        if (firstDigit && current_ch == '0' && (file.peek()  != EOF && file.peek() != '.' && file.peek()  >= '0' && file.peek()  <= '9'))
            return false;


        if (current_ch == '.') {  
            if (decimal)
                return false;
            decimal = true;
            if (file.peek()  == EOF || file.peek() < '0' ||  file.peek() > '9')
                return false;
        }

        if (firstDigit)
            firstDigit = false;

        if (file.peek() != '.' && (file.peek() < '0' || file.peek() > '9')) {
            break;
        }

    }

    return true;
}
bool check_str(fstream& file) {
    char current_ch; 

    file >> skipws >> current_ch;
    
    
    
    if (current_ch != '"')
        return false;

    while (true) {
        if (next_no_ws(file) == EOF)
            return false;

        file >> noskipws >> current_ch; 
         

        if (current_ch == '\\') {
            if(next_no_ws(file) != '\\' && next_no_ws(file) != '"' && next_no_ws(file) != 't')
                return false;
            file.ignore(1);
            
        }

        if (current_ch == '\"')
            break;

    }

    return true;
}




bool check_bool(fstream& file) {
    const char false_name[] = {'f', 'a', 'l', 's', 'e'};
    const char true_name[] = {'t', 'r', 'u', 'e'};
    const char null_name[] = {'n', 'u', 'l', 'l'};




    char current_ch;
    
    file >> skipws >> current_ch;
    
    bool is_false = false, is_null = false, is_true = false; 


    if (current_ch == 'f')
        is_false = true;
    else if (current_ch == 't')
        is_true = true;
    else if (current_ch == 'n')
        is_null = true;
    else return false;

    if (is_false) {
        for (int i = 1; i < 5; i++) {
            if (next_no_ws(file) == EOF)
                return false;
            file >> noskipws >> current_ch; 

            if (current_ch != false_name[i])
                return false;
        }
    }
    else if (is_true) {
        for (int i = 1; i < 4; i++) {
            if (next_no_ws(file) == EOF)
                return false;
            file >> noskipws >> current_ch; 

            if (current_ch != true_name[i])
                return false;
        }
    }
    else  {
        for (int i = 1; i < 4; i++) {
            if (next_no_ws(file) == EOF)
                return false;
            file >> noskipws >> current_ch; 

            if (current_ch != null_name[i])
                return false;
        }
    }


    return true;
};


bool check_val(fstream& file) {
    char next = next_no_ws(file); 

        if(next == '"') {
            if (!check_str(file))
                return false;
        }
        else if (next =='f') {
            if (!check_bool(file))
                return false;
        }
        else if (next == 'n') {
            if (!check_bool(file))
                return false;
            }
        else if (next == 't') {
            if (!check_bool(file))
                return false;
            }
        else if (next == '[') {
            if (!check_arr(file))
                return false;
            }
        else if (next == '{') {
            if (!check_obj(file))
                return false;
            }
        else {
            if (next >= '0' && next <= '9' || next == '-')  { // liczba 
                if (!check_num(file))
                    return false;
            }
         
            else 
                return false; //zaden znany typ nie rozpoczyna sie nastepnym znakiem
        }
        ;

    return true;    
}
bool check_val(fstream& file, bool isTop) {
    return check_val(file) && next_no_ws(file) == EOF;
}
bool check_dict(fstream& file) {
    if (!check_str(file))
        return false;

    if (next_no_ws(file) != ':')
        return false;

    char current_ch; 
    file >> skipws >> current_ch;

    if (!check_val(file)) {
        return false;
    }
    return true;
}

bool check_obj(fstream& file) {
    char current_ch;

    file >> skipws >> current_ch;  

    
    while (true)
    {
        if (next_no_ws(file) == EOF || !check_dict(file))
            return false;
        if (next_no_ws(file) == EOF) // po sprawdzeniu slownika sprawdzamy czy nie ma konca pliku
            return false;

        file >> skipws >> current_ch; 
        
        

        if (current_ch == '}')
            break;
        if (current_ch == ',')
            continue;
        return false;
    }
    

    return true;
};
bool check_arr(fstream& file) {
    char current_ch;

    file >> skipws >> current_ch;  




    while (true)
    {
        if (next_no_ws(file) == EOF || !check_val(file))
            return false;

        if (next_no_ws(file) == EOF) // po sprawdzeniu wartosci sprawdzamy czy nie ma konca pliku 
            return false;

        file >> skipws >> current_ch;
        


        if (current_ch == ']')
            break;
        if (current_ch == ',')
            continue;
        return false;
    }
    

    return true;
};


//***************************
void draw_menu();
void menu();



fstream plik;
string content;


void operation_n()
{

    plik.open("./test/input/1.json", ios::in);

    if (plik.good() == false)
    {
        cout << "Plik nie istnieje\n";
    }
    else {
        cout << "Plik zostal otwarty\n";
    }




    menu();
}

void operation_h()
{

    cout << "---------------HELP---------------" << endl;
    cout << "autor: Patrycja Pienkowska s193452" << endl;
    cout << "operation plikjson - otwieranie pliku" << endl;
    cout << "------------------------------------" << endl;

    cout;
    cout;

    menu();
}

void draw_menu() {

    cout << "1. Operation [H] (help)" << endl;
    cout << "2. Operation otworz plik [N]" << endl;
    cout << "3. Operation sprawdz json [C]" << endl;
    menu();
    cout;
}

//funkcje do operacji c *****
void get_file_content() {
    vector<char> bytes; //vector zamiast tablicy o ustalonej wielkosci zeby nie przekroczyc limitu pamieci dla malych plikow
    while (plik.peek() != EOF) {
        char current; plik.get(current);
        bytes.push_back(current);
    }
    content = string(bytes.begin(), bytes.end());
};

void check_value(string value) {
    switch (value[0]) {
        case '"':
            break;
        case '{':
            break;
        case '[':
            break;
        case 'f':
        case 't':
            break;
        case 'n':
            break;
    }
    if ((value[0] >= '0' && value[0] <= 9 ) || value[0] == '-') {
        
    }
    
}

void operation_c() {
    if(check_val(plik)) {
        cout << "Poprawny json" << endl;
    } 
    else {
        cout << "Błąd w: " << plik.tellg() << endl;
    }
}

//****************************

void menu() {

    bool correct_char = false;
    char wybor;
    do {
        cin >> wybor;

        switch (wybor)
        {
        case 'h':
        case 'H':
            correct_char = true;
            operation_h();
            break;
        case 'n':
        case 'N':
            correct_char = true;
            operation_n();
            break;
        case 'c':
        case 'C':
            correct_char = true;
            operation_c();
            break;
        }

    } while (!correct_char);
}



int main()
{
    draw_menu();



    if (plik.is_open())
        plik.close();
    return 0;
}
