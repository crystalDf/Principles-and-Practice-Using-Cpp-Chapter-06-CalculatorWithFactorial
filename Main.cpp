# include "std_lib_facilities.h"

class Token
{
public:
    char kind;
    double value;

    Token(char ch) : kind { ch }, value { 0 } { }
    Token(char ch, double val) : kind { ch }, value { val } { }
};

class Token_stream
{
public:
    Token_stream() : full { false }, buffer { ' ' } { }
    Token get();
    void putback(Token t);
private:
    bool full;
    Token buffer;
};

void Token_stream::putback(Token t)
{
    if (full)
    {
        error("putback() into a full buffer");
    }

    buffer = t;
    full = true;
}

Token Token_stream::get()
{
    if (full)
    {
        full = false;
        return buffer;
    }

    char ch;
    std::cin >> ch;

    switch (ch)
    {
        case '=':
        case 'x':
        case '(':
        case ')':
        case '{':
        case '}':
        case '+':
        case '-':
        case '*':
        case '/':
        case '!':
            return Token(ch);
        case '.':
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            {
                std::cin.putback(ch);
                double val;
                std::cin >> val;
                return Token('8', val);
            }
        default:
            error("Bad token");
            return Token { ' ' };
    }
}

double expression();
double term();
double factorial();
double primary();
double get_factorial(double val);

Token_stream ts;

int main()
{
    try
    {
        double val;

        while (std::cin)
        {
            Token t = ts.get();

            if (t.kind == 'x')
            {
                break;
            }

            if (t.kind == '=')
            {
                std::cout << "=" << val << std::endl;
            }
            else
            {
                ts.putback(t);
            }

            val = expression();
        }

        keep_window_open();
    }
    catch (exception& e)
    {
        std::cerr << e.what() << std::endl;
        keep_window_open();

        return 1;
    }
    catch (...)
    {
        std::cerr << "exception" << std::endl;
        keep_window_open();

        return 2;
    }
}

double expression()
{
    double left = term();
    Token t = ts.get();

    while (true)
    {
        switch (t.kind)
        {
            case '+':
                {
                    left += term();
                    t = ts.get();
                    break;
                }
            case '-':
                {
                    left -= term();
                    t = ts.get();
                    break;
                }
            default:
                ts.putback(t);
                return left;
        }
    }
}

double term()
{
    double left = factorial();
    Token t = ts.get();

    while (true)
    {
        switch (t.kind)
        {
            case '*':
                {
                    left *= factorial();
                    t = ts.get();
                    break;
                }
            case '/':
                {
                    double d = factorial();

                    if (!d)
                    {
                        error("divide by zero");
                    }

                    left /= d;
                    t = ts.get();
                    break;
                }
            default:
                ts.putback(t);
                return left;
        }
    }
}

double factorial()
{
    double left = primary();
    Token t = ts.get();

    while (true)
    {
        switch (t.kind)
        {
            case '!':
                {
                    left = get_factorial(left);
                    t = ts.get();
                    break;
                }
            default:
                ts.putback(t);
                return left;
        }
    }
}

double primary()
{
    Token t = ts.get();

    switch (t.kind)
    {
        case '(':
            {
                double d = expression();
                t = ts.get();

                if (t.kind != ')')
                {
                    error("')' expected");
                }
                return d;
            }
        case '{':
            {
                double d = expression();
                t = ts.get();

                if (t.kind != '}')
                {
                    error("'}' expected");
                }
                return d;
            }
        case '8':
            return t.value;
        default:
            error("primary expected");
            return 0;
    }
}

double get_factorial(double val)
{
    int result = 1;

    for (int i = 1; i <= val; ++i)
    {
        result *= i;
    }

    return result;
}