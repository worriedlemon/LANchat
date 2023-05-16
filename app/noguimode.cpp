#include "app/noguimode.h"

#include <QStringList>

#include <iostream>

/// This program module has not been implemented yet.
/// For now it only contents _stdio_ interpreter, which knows only "exit" keyword.
void NoGUIMode::Enter()
{
    char* input = new char[128];
    while (true)
    {
        std::cout << "> ";
        std::cin.getline(input, 128);

        QStringList args = QString(input).split(' ');

        if (args.isEmpty() || args[0].isEmpty()) continue;

        QString command = args.takeFirst();

        if (command == "exit") break;

        /*try {
            Command(&command, &args).Execute();
        }
        catch (CommandException& e)
        {
            qerr << e.message;
        }*/

        std::cout << "Command: " << input << "\n";
    }
}
