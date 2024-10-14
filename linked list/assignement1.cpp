#include <iostream>
#include <string>

class Token
{
public:
    std::string t;
    Token *next;
    Token *prev;

    Token(const std::string t) : t(t), next(nullptr), prev(nullptr) {}
};

class Rule
{
public:
    Rule *prev;
    Rule *next;

    Token *tokenHead;
    Token *tokenTail;

    Rule() : prev(nullptr), next(nullptr),
             tokenHead(nullptr), tokenTail(nullptr) {}

    void addToken(const std::string t)
    {
        Token *newToken = new Token(t);
        if (!tokenHead)
        {
            tokenHead = tokenTail = newToken;
        }
        else
        {
            newToken->prev = tokenTail;
            tokenTail->next = newToken;
            tokenTail = newToken;
        }
    }

    ~Rule()
    {
        Token *currentToken = tokenHead;
        while (currentToken)
        {
            Token *tempToken = currentToken;
            currentToken = currentToken->next;
            delete tempToken;
        }
    }
};

class DoublyLinkedList
{
public:
    struct Prod
    {
        std::string nt;
        Rule *ruleHead;
        Rule *ruleTail;
        Prod *prev;
        Prod *next;

        Prod(const std::string t) : nt(t), ruleHead(nullptr), ruleTail(nullptr), prev(nullptr), next(nullptr) {}

        void addRule(const std::string s)
        {
            Rule *newRule = new Rule();

            std::string currentToken;
            for (size_t i = 0; i < s.length(); ++i)
            {
                char c = s[i];
                if (c == ' ')
                {
                    newRule->addToken(currentToken);
                    currentToken.clear();
                }
                else
                {
                    currentToken.push_back(c);
                }
            }
            if (!currentToken.empty())
            {
                newRule->addToken(currentToken);
            }

            if (!ruleHead)
            {
                ruleHead = ruleTail = newRule;
            }
            else
            {
                newRule->prev = ruleTail;
                ruleTail->next = newRule;
                ruleTail = newRule;
            }
        }

        ~Prod()
        {
            Rule *currentRule = ruleHead;
            while (currentRule)
            {
                Rule *tempRule = currentRule;
                currentRule = currentRule->next;
                delete tempRule;
            }
        }
    };

    DoublyLinkedList() : head(nullptr), tail(nullptr) {}

    void insertBack(Prod *p)
    {
        if (!head)
        {
            head = tail = p;
        }
        else
        {
            p->prev = tail;
            tail->next = p;
            tail = p;
        }
    }

    Prod *getHead() const { return head; }

    ~DoublyLinkedList()
    {
        Prod *current = head;
        while (current)
        {
            Prod *temp = current;
            current = current->next;
            delete temp;
        }
    }

private:
    Prod *head;
    Prod *tail;
};

int main()
{
    DoublyLinkedList prods;

    while (true)
    {
        printf("Enter a prod (type 'end' to stop): ");

        std::string line;
        std::getline(std::cin, line);

        if (line == "end")
        {
            break;
        }

        size_t arrow = line.find("->");

        if (arrow == std::string::npos)
        {
            std::cerr << "Invalid prod\n";
            continue;
        }

        std::string nt_str = line.substr(0, arrow);

        DoublyLinkedList::Prod *node = new DoublyLinkedList::Prod(nt_str);

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

    DoublyLinkedList::Prod *node = prods.getHead();

    while (node)
    {
        std::cout << node->nt << " -> ";

        Rule *rule = node->ruleHead;

        while (rule)
        {
            // Print tokens
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