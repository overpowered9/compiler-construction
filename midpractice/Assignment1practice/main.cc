#include <iostream>
#include <string>
#include "DoublyLinkedList.h"

int main()
{
    DoublyLinkedList prods;

    while (true)
    {
        std::cout << "Enter a production (type 'end' to stop): ";

        std::string line;
        std::getline(std::cin, line);

        if (line == "end")
        {
            break;
        }

        size_t arrow = line.find("->");

        if (arrow == std::string::npos)
        {
            std::cerr << "Invalid production\n";
            continue;
        }

        std::string nt_str = line.substr(0, arrow);

        Prod *node = new Prod(nt_str);

        size_t start = arrow + 2;
        size_t pipe = line.find("|", start);

        while (true)
        {
            size_t end = (pipe != std::string::npos) ? pipe : line.length();
            std::string ruleStr = line.substr(start, end - start);

            node->addRule(ruleStr);

            if (pipe == std::string::npos)
                break;

            start = pipe + 1;
            pipe = line.find("|", start);
        }

        prods.insertBack(node);
    }

    Prod *node = prods.getHead();

    while (node)
    {
        std::cout << node->nt << " -> ";

        Rule *rule = node->ruleHead;

        while (rule)
        {
            Token *token = rule->tokenHead;

            while (token)
            {
                std::cout << token->t;
                token = token->next;

                if (token)
                    std::cout << " ";
            }

            rule = rule->next;

            if (rule)
                std::cout << " | ";
        }

        std::cout << std::endl;

        node = node->next;
    }

    return 0;
}
