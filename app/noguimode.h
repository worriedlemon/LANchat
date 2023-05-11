#ifndef NOGUIMODE_H
#define NOGUIMODE_H

class NoGUIMode
{
private:
    NoGUIMode();
    NoGUIMode(NoGUIMode const &);
    NoGUIMode operator=(NoGUIMode&);

public:
    static void Enter();
};

#endif // NOGUIMODE_H
