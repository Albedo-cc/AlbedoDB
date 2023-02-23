#include <database.h>

#include <string_view>
#include <iostream>
#include <unordered_set>

#include <memory>
#include <unordered_map>

bool check_user_input(std::string_view input)
{
    static const std::unordered_set<char> forbidden{ '\\',  '(' , ')' };
    return true;
}

int main(int argc, char* argv[])
{
    using namespace albedo::server;
    auto& database = Database::instance();
    try
    {
        database.add_user("ABC", "123321");
        //mysql.exec("insert into user(name) VALUES('Laurel')");
        /*auto qa = mysql.query("select `id` from user", true)->get();
        std::cout << qa->row_count << "\n";
        auto qb = mysql.query("select * from user", false)->get();
        std::cout << qb->row_count << "\n";*/
        //mysql.query("select * from user");
        //while (true) std::cout << mysql.ping() << std::endl;
    }
    catch (std::runtime_error& e)
    {
        std::cerr << "[Runtime Error]>> " << e.what() << '\n';
    }
    catch (std::exception& e)
    {
        std::cerr << "[Other Error]>> " << e.what() << '\n';
    }

    return EXIT_SUCCESS;
}